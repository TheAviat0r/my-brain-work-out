#include "syntax_analizer.h"

void CFunctionsTable::add_func(const std::string& functionName)
{
    CFunctionsTable::FuncData.push_back(FunctionData_t(functionName));
}

int  checkArguments    (const CTreeNode* root, FunctionData_t& Buf, int& cnt);
void checkArgAmount    (const CTreeNode* Node, const CTokenBuffer& TokenBuffer, FunctionData_t& FData);
void checkFunctionCall (const CTreeNode* Node, const CTokenBuffer& TokenBuffer, FunctionData_t& caller,
                                                                                FunctionData_t& executer);

bool FunctionData_t::var_exist(const std::string& varName) const
{
    for (size_t i = 0; i < (*this).funcVariables.size(); i++)
    {
        if ((*this).funcVariables[i] == varName)
            return true;
    }

    return false;
}

int  FunctionData_t::arg_amount() const
{
    return argCnt;
}

void FunctionData_t::push_var(const std::string& varName)
{
    funcVariables.push_back(varName);
}

void FunctionData_t::push_arg(const std::string& argName)
{
    push_var(argName);
    ++argCnt;
}

void FunctionData_t::dump() const
{
    std::fstream output("dump.txt", std::ios_base::out | std::ios_base:: ate);
    std::cout << "------------------\n";
    std::cout << "FUNC NAME: " << funcName << "\n";
    std::cout << "argAmount: " << argCnt   << "\n";
    std::cout << "VARS:\n";

    for (size_t i =0; i < funcVariables.size(); i++)
        std::cout << funcVariables[i] << "\n";

    std::cout << "------------------\n";
}

struct GlobalVariables
{
    std::vector<std::string> data;
    void push_var(const std::string& varName);
};

void GlobalVariables::push_var(const std::string& varName)
{
    data.push_back(varName);
}

CFunctionsTable::CFunctionsTable(): FuncData()
{
}

FunctionData_t&  CFunctionsTable::search_function(const std::string& functionName)
{
    for (size_t i = 0; i < FuncData.size(); i++)
    {
        if (FuncData[i].funcName == functionName)
            return FuncData[i];
    }

    throw CSyntaxError("Function doesn't exist!", __LINE__, functionName);
}

bool CFunctionsTable::function_exist(const std::string& functionName) const
{
    for (size_t i = 0; i < FuncData.size(); i++)
    {
        if (FuncData[i].funcName == functionName)
            return true;
    }

    return false;
}

void CFunctionsTable::dump() const
{
    for (size_t i = 0; i < FuncData.size(); i++)
    {
        FuncData[i].dump();
    }
}
//!---------------------------------------------
//!                PARSER CODE
//!---------------------------------------------
CTreeNode* parseNumber(CTokenBuffer& TokenBuffer)
{
    assert(&TokenBuffer);

    CTreeNode* numberNode;
    if (TokenBuffer.CUR_DESCR == NUMBER)
    {
        numberNode = new CTreeNode(TokenBuffer.current(), NUMBER);
        TokenBuffer.go_next();
        return numberNode;
    }

    assert(numberNode);
    return numberNode;
}

CTreeNode* parseVariableRV(CTokenBuffer& TokenBuffer, FunctionData_t& FData)
{
    assert(&TokenBuffer);

    if (TokenBuffer.CUR_DESCR == NUMBER)
        return parseNumber(TokenBuffer);

    if (TokenBuffer.NEXT_DESCR == LSKOB)
        return parseFunctionCall(TokenBuffer, FData);

    if (!FData.var_exist(TokenBuffer.CUR_OPER))
    {
        FData.dump();
        throw  CSyntaxError("Undeclared variable!", TokenBuffer.CUR_LINE, TokenBuffer.CUR_OPER);
    }

    return parseVariableLV(TokenBuffer, USUAL, FData);
}

CTreeNode* parseVariableLV(CTokenBuffer& TokenBuffer, int mode, FunctionData_t& FData)
{
    assert(&TokenBuffer);

    if (TokenBuffer.CUR_DESCR == VAR)
    {
        CTreeNode* variableNode = new CTreeNode(TokenBuffer.current(), VAR);
        assert(variableNode);

        TokenBuffer.go_next();

        if (TokenBuffer.CUR_DESCR == LINDEX && mode)
        {
            TokenBuffer.go_next();
            variableNode = parseIndex(TokenBuffer, variableNode, FData);
        }

        assert(variableNode);
        return      variableNode;
    }

    return 0;
}

CTreeNode* parseIndex(CTokenBuffer& TokenBuffer, CTreeNode* variableNode, FunctionData_t& FData)
{
    variableNode->set_left(parseAdd(TokenBuffer, FData));
    variableNode->set_type(VARINDEX);
    /*switch(TokenBuffer.CUR_DESCR)
    {
        case NUMBER:
            variableNode->set_left(parseNumber(TokenBuffer));
            variableNode->left    ()->set_type(NUMINDEX);
            break;
        case VAR:
            variableNode->set_left(parseVariableLV(TokenBuffer, USUAL, FData));
            variableNode->left    ()->set_type(VARINDEX);
            break;
        default:
            throw CSyntaxError("Error in indexsation operator!", TokenBuffer.CUR_LINE, TokenBuffer.CUR_OPER);
    }*/

    if (TokenBuffer.CUR_DESCR != RINDEX)
        throw CSyntaxError("] is missing!", TokenBuffer.CUR_LINE);

    TokenBuffer.go_next();

    assert(variableNode);
    return variableNode;
}

CTreeNode* parseFunctionCall (CTokenBuffer& TokenBuffer, FunctionData_t& FData)
{
    if (TokenBuffer.CUR_DESCR != VAR)
        throw CSyntaxError("Wrong function call!", TokenBuffer.CUR_LINE, TokenBuffer.CUR_OPER);

    CTreeNode* callNode = new CTreeNode(token_t(CALL), CALL, 0, parseVariableLV(TokenBuffer, NOINDEX, FData));

    if (!FunctionsTable.function_exist(callNode->right()->stroke()))
        throw CSyntaxError("Function doesn't exist", TokenBuffer.CUR_LINE, callNode->stroke());

    if (TokenBuffer.CUR_DESCR != LSKOB)
    {
        std::cout << "DESCR " << TokenBuffer.CUR_DESCR << " OPER: " << TokenBuffer.CUR_OPER << "\n";
        throw CSyntaxError(") is missing!", TokenBuffer.CUR_LINE);
    }

    TokenBuffer.go_next();
    callNode->set_left(parseArguments(TokenBuffer, USUAL, FData, NUMBERS_ON));

    checkFunctionCall(callNode->left(), TokenBuffer, FData,
                                        FunctionsTable.search_function(callNode->right()->stroke()));

    if (TokenBuffer.CUR_DESCR != RSKOB)
        throw CSyntaxError(") is missing!", TokenBuffer.CUR_LINE);

    TokenBuffer.go_next();

    assert(callNode);
    return callNode;
}

void checkFunctionCall(const CTreeNode* Node, const CTokenBuffer& TokenBuffer, FunctionData_t& caller,
                                                                     FunctionData_t& executer)
{
    assert(Node);
    int arg_cnt = 0;
    arg_cnt = checkArguments(Node, caller, arg_cnt);

    if (arg_cnt < executer.arg_amount())
        throw CSyntaxError("Too few arguments of function!", TokenBuffer.CUR_LINE);
    if (arg_cnt > executer.arg_amount())
        throw CSyntaxError("Too many arguments of function", TokenBuffer.CUR_LINE);
}

CTreeNode* parseCode     (CTokenBuffer& TokenBuffer)
{
    assert(&TokenBuffer   );
    assert(&FunctionsTable);

    parseDeclarations(TokenBuffer);

    return parseProgram(TokenBuffer);
}

void  parseDeclarations  (CTokenBuffer& TokenBuffer)
{
    while (TokenBuffer.CUR_OPER != INT_STR && TokenBuffer.CUR_OPER != VOID_STR && TokenBuffer.cnt() < TokenBuffer.size())
    {
        if (TokenBuffer.CUR_OPER != DECLARE_STR)
            throw CSyntaxError("DECLARE IS MISSING!", TokenBuffer.CUR_LINE);

        TokenBuffer.go_next();
        FunctionsTable.add_func(TokenBuffer.CUR_OPER);
        TokenBuffer.go_next();

        if (TokenBuffer.CUR_DESCR != LSKOB) throw CLSkobMissing(TokenBuffer.CUR_LINE);
        TokenBuffer.go_next();

        addArguments(TokenBuffer, FunctionsTable.last_func());

        if (TokenBuffer.CUR_DESCR != RSKOB) throw CRSkobMissing(TokenBuffer.CUR_LINE);
        TokenBuffer.go_next();
        if (TokenBuffer.CUR_DESCR != ENDOP) throw CDotMissing (TokenBuffer.CUR_LINE);
        TokenBuffer.go_next();
    }
}

void  addArguments       (CTokenBuffer& TokenBuffer, FunctionData_t& Table)
{
    assert(&TokenBuffer); assert(&Table);
    if (TokenBuffer.CUR_DESCR != RSKOB)
    {
        Table.push_arg(TokenBuffer.CUR_OPER);
        TokenBuffer.go_next();
    }

    while(TokenBuffer.CUR_OPER == ",")
    {
        TokenBuffer.go_next();
        Table.push_arg(TokenBuffer.CUR_OPER);
        TokenBuffer.go_next();
    }
}

CTreeNode* parseProgram  (CTokenBuffer& TokenBuffer)
{
    assert(&TokenBuffer);

    CTreeNode* userProgram = new CTreeNode(token_t(BOP), BOP, parseFunctions(TokenBuffer), 0);

    while(TokenBuffer.cnt() < TokenBuffer.size())
    {
        CTreeNode* nextFunc = parseProgram(TokenBuffer);
        userProgram->set_right(nextFunc);
    }

    return userProgram;
}

CTreeNode* parseFunctions(CTokenBuffer& TokenBuffer)
{
    int tokenType = 0;

    if (TokenBuffer.CUR_OPER == VOID_STR)
        tokenType = VOID;
    if (TokenBuffer.CUR_OPER == INT_STR)
        tokenType = INT;
    if (!tokenType)
        throw CSyntaxError("Wrong function specificator!", TokenBuffer.CUR_LINE);

    TokenBuffer.go_next();
    CTreeNode*      funcNode = new CTreeNode(token_t(tokenType, TokenBuffer.CUR_OPER), tokenType);
    FunctionData_t& FData    = FunctionsTable.search_function(funcNode->stroke());

    if (!FunctionsTable.function_exist(TokenBuffer.CUR_OPER))
        throw CSyntaxError("Function is not declared!", TokenBuffer.CUR_LINE, TokenBuffer.CUR_OPER);

    TokenBuffer.go_next();
    if (TokenBuffer.CUR_DESCR != LSKOB)
        throw CSyntaxError("( is missing!", TokenBuffer.CUR_LINE);

    TokenBuffer. go_next();
    funcNode->set_left  (parseArguments(TokenBuffer, NOINDEX, FData, NUMBERS_OFF));

    checkArgAmount(funcNode->left(), TokenBuffer, FData);

    if (TokenBuffer.CUR_DESCR != RSKOB)
        throw CSyntaxError(") is missing!", TokenBuffer.CUR_LINE);

    TokenBuffer.go_next();
    funcNode   ->set_right(parseFSkob(TokenBuffer, FData));

    return funcNode;
}

int checkArguments(const CTreeNode* root, FunctionData_t& Buf, int& cnt)
{
    assert(root);
    if (root->type() == ARG)
    {
        if (root->left())
        {
            cnt++;
            assert(root->left()->type() == VAR || root->left()->type() == NUMBER);
            if (root->left()->type() == VAR && !Buf.var_exist(root->left()->stroke()))
                throw CSyntaxError("Wrong argument! Check proto!", root->left()->stroke());
        }
    }

    if (root->right())
        checkArguments(root->right(), Buf, cnt);

    return cnt;
}

void checkArgAmount(const CTreeNode* Node, const CTokenBuffer& TokenBuffer, FunctionData_t& FData)
{
    assert(Node);

    int arg_cnt = 0;
    arg_cnt = checkArguments(Node, FData, arg_cnt);

    if (arg_cnt < FData.arg_amount())
        throw CSyntaxError("Too few arguments to function!",  TokenBuffer.CUR_LINE);
    if (arg_cnt > FData.arg_amount())
        throw CSyntaxError("Too many arguments to function!", TokenBuffer.CUR_LINE);
}

CTreeNode* parseArguments(CTokenBuffer& TokenBuffer, int indexMode, FunctionData_t& FData, int argMode)
{
    CTreeNode* argNode = new CTreeNode(token_t(ARG), ARG);

    if (TokenBuffer.CUR_DESCR == VAR)
        argNode->set_left(parseVariableLV(TokenBuffer, indexMode, FData));
    if (TokenBuffer.CUR_DESCR == NUMBER && argMode)
        argNode->set_left(parseNumber(TokenBuffer));

    while (TokenBuffer.CUR_DESCR == OPER && TokenBuffer.CUR_OPER == ",")
    {
        TokenBuffer.go_next();
        if (TokenBuffer.CUR_DESCR == VAR)
            argNode = new CTreeNode(token_t(ARG), ARG, parseVariableLV(TokenBuffer, indexMode, FData), argNode);
        if (TokenBuffer.CUR_DESCR == NUMBER && argMode)
            argNode = new CTreeNode(token_t(ARG), ARG, parseNumber(TokenBuffer), argNode);
    }

    assert(argNode);
    return argNode;
}

CTreeNode* parseOperators(CTokenBuffer& TokenBuffer, FunctionData_t& FData)
{
    assert(&TokenBuffer);

    CTreeNode* startOP = new CTreeNode(token_t(OP), OP, parseFSkob(TokenBuffer, FData), NULL);

    while ((TokenBuffer.CUR_DESCR == VAR || TokenBuffer.CUR_DESCR == LFSKOB) && TokenBuffer.cnt() <= TokenBuffer.size())
    {
        CTreeNode* nextOP = parseOperators(TokenBuffer, FData);
        startOP->set_right(nextOP);
    }

    return startOP;
}

CTreeNode* parseFSkob   (CTokenBuffer& TokenBuffer, FunctionData_t& FData)
{
    assert(&TokenBuffer);
    if (TokenBuffer.CUR_DESCR == LFSKOB)
    {
        TokenBuffer.go_next();

        CTreeNode* Node = new CTreeNode(token_t(OP), OP, parseOperators(TokenBuffer, FData), 0);

        if (TokenBuffer.CUR_DESCR != RFSKOB)
            throw CSyntaxError("'}' is missing!", TokenBuffer.CUR_LINE);

        TokenBuffer.go_next();
        return Node;
    }

    return parseKeyword(TokenBuffer, FData);
}

CTreeNode* parseKeyword (CTokenBuffer& TokenBuffer, FunctionData_t& FData)
{
    assert(&TokenBuffer);
    if (TokenBuffer.CUR_DESCR != VAR)
        throw CSyntaxError("Wrong operator! Read man-page!", TokenBuffer.CUR_LINE, TokenBuffer.CUR_OPER);

    if (TokenBuffer.CUR_OPER == WHILE_STR || TokenBuffer.CUR_OPER == IF_STR)
        return parseIfAndWhile(TokenBuffer, FData);

    if (TokenBuffer.CUR_OPER == INPUT_STR)
        return parseInputOutput(TokenBuffer, FData, INPUT);
    if (TokenBuffer.CUR_OPER == OUTPUT_STR)
        return parseInputOutput(TokenBuffer, FData, OUTPUT);

    if (TokenBuffer.NEXT_DESCR == LSKOB)
    {
        CTreeNode* callNode = parseFunctionCall(TokenBuffer, FData);

        if (TokenBuffer.CUR_DESCR != ENDOP)
            throw CSyntaxError("; is missing!", TokenBuffer.CUR_LINE);

        TokenBuffer.go_next();

        return callNode;
    }

    if (TokenBuffer.CUR_OPER == ARRAY_STR)
        return parseArrayDeclare(TokenBuffer, FData);

    if (TokenBuffer.CUR_OPER == RETURN_STR)
        return parseReturn(TokenBuffer, FData);

    return parseAssign(TokenBuffer, FData);
}

CTreeNode* parseIfAndWhile(CTokenBuffer& TokenBuffer, FunctionData_t& FData)
{
    assert(TokenBuffer.CUR_DESCR == VAR);
    int rootType = 0;

    if (TokenBuffer.CUR_OPER == IF_STR)
        rootType = IF;
    if (TokenBuffer.CUR_OPER == WHILE_STR)
        rootType = WHILE;

    if (!rootType)
        throw CSyntaxError("Something went wrong in IF operator!", __LINE__);

    TokenBuffer.go_next();

    if (TokenBuffer.CUR_DESCR != LSKOB)
        throw CSyntaxError("( is missing in condition!", TokenBuffer.CUR_LINE);

    TokenBuffer.go_next();
    CTreeNode * condNode = new CTreeNode(token_t(COND), COND, parseLogicOper(TokenBuffer, FData), 0);
    assert(condNode);

    if (TokenBuffer.CUR_DESCR != RSKOB)
        throw CSyntaxError(") is missing in condition!", TokenBuffer.CUR_LINE);

    TokenBuffer.go_next();

    return new CTreeNode(token_t(rootType), rootType, condNode, parseFSkob(TokenBuffer, FData));
}

CTreeNode* parseReturn(CTokenBuffer& TokenBuffer, FunctionData_t& FData)
{
    assert(&TokenBuffer);
    assert(TokenBuffer.CUR_OPER == RETURN_STR);

    TokenBuffer.go_next();

    CTreeNode* returnNode = new CTreeNode(token_t(RETURN), RETURN, parseAdd(TokenBuffer, FData), 0);
    assert(returnNode);

    if (TokenBuffer.CUR_DESCR != ENDOP)
        throw CDotMissing(TokenBuffer.CUR_LINE);

    TokenBuffer.go_next();

    return returnNode;
}

CTreeNode* parseInputOutput(CTokenBuffer& TokenBuffer, FunctionData_t& FData, int Mode)
{
    assert(TokenBuffer.CUR_OPER == INPUT_STR || TokenBuffer.CUR_OPER == OUTPUT_STR);
    assert(Mode == INPUT || Mode == OUTPUT);
    TokenBuffer.go_next();

    if (TokenBuffer.CUR_DESCR != LSKOB) throw CLSkobMissing(TokenBuffer.CUR_LINE);

    TokenBuffer.go_next();

    CTreeNode* showNode = new CTreeNode(token_t(Mode), Mode, parseVariableLV(TokenBuffer, USUAL, FData), 0);
    assert(showNode);

    if (TokenBuffer.CUR_DESCR != RSKOB) throw CRSkobMissing(TokenBuffer.CUR_LINE);

    TokenBuffer.go_next();
    if (TokenBuffer.CUR_DESCR != ENDOP) throw CDotMissing(TokenBuffer.CUR_LINE);
    TokenBuffer.go_next();

    return showNode;
}

CTreeNode* parseArrayDeclare(CTokenBuffer& TokenBuffer, FunctionData_t& FData)
{
    assert(TokenBuffer.CUR_DESCR == VAR);
    assert(TokenBuffer.CUR_OPER  == ARRAY_STR);
    TokenBuffer.go_next();

    CTreeNode* arrayNode = new CTreeNode(token_t(ARRINIT, TokenBuffer.CUR_OPER), ARRINIT);
    FData.push_var(arrayNode->stroke());
    TokenBuffer.go_next ();

    arrayNode->set_left (parseNumber(TokenBuffer));

    for (int i = 0; i < arrayNode->left()->data() - 1; i++)
        FData.push_var("ARRPOINT");

    arrayNode->set_right(parseNumber(TokenBuffer));

    if (TokenBuffer.CUR_DESCR != ENDOP)
        throw CDotMissing(TokenBuffer.CUR_LINE);

    TokenBuffer.go_next();

    assert(arrayNode);
    return arrayNode;
}

CTreeNode* parseLogicOper (CTokenBuffer& TokenBuffer, FunctionData_t& FData)
{
    CTreeNode* condNode = parseLogicSkob(TokenBuffer, FData);

    while (TokenBuffer.CUR_DESCR == AND || TokenBuffer.CUR_DESCR == OR)
    {
        if (TokenBuffer.CUR_DESCR == AND)
        {
            TokenBuffer.go_next();
            condNode = new CTreeNode(token_t(AND), AND, condNode, parseLogicSkob(TokenBuffer, FData));
        }
        if (TokenBuffer.CUR_DESCR == OR)
        {
            TokenBuffer.go_next();
            condNode = new CTreeNode(token_t(OR), OR, condNode, parseLogicSkob(TokenBuffer, FData));
        }
    }

    assert(condNode);
    return condNode;
}

CTreeNode* parseLogicSkob (CTokenBuffer& TokenBuffer, FunctionData_t& FData)
{
    assert(&TokenBuffer);

    if (TokenBuffer.CUR_DESCR == LSKOB)
    {
        TokenBuffer.go_next();
        CTreeNode* Node = parseLogicOper(TokenBuffer, FData);

        if (TokenBuffer.CUR_DESCR != RSKOB)
            throw CSyntaxError(") is missing!", TokenBuffer.CUR_LINE);

        assert(Node);
        return Node;
    }

    return parseComparison(TokenBuffer, FData);
}

CTreeNode* parseComparison(CTokenBuffer& TokenBuffer, FunctionData_t& FData)
{
    assert(&TokenBuffer);

    CTreeNode* leftPart = parseAdd(TokenBuffer, FData);
    CTreeNode* compNode = NULL;

    switch(TokenBuffer.CUR_DESCR)
    {
        #define COMP_CASE(type) case type:\
                    TokenBuffer.go_next();\
                    compNode = new CTreeNode(token_t(type), type, leftPart, parseAdd(TokenBuffer, FData));\
                    break;
        COMP_CASE(LESS   )
        COMP_CASE(LESS_EQ)
        COMP_CASE(BIG    )
        COMP_CASE(BIG_EQ )
        COMP_CASE(EQUAL  )
        COMP_CASE(N_EQUAL)
        default:
            throw CSyntaxError("Wrong comparison operator!", TokenBuffer.CUR_LINE);
    }

    return compNode;
}

CTreeNode* parseAssign  (CTokenBuffer& TokenBuffer, FunctionData_t& FData)
{
    unsigned int type    = VAR;
    CTreeNode * Variable = 0;

    if (TokenBuffer.CUR_DESCR == VAR)
    {
        if (TokenBuffer.CUR_OPER == PREFIX)
        {
            TokenBuffer.go_next();
            if (FData.var_exist(TokenBuffer.CUR_OPER))
            {
                FData.dump();
                throw CSyntaxError("Redefenition of var!", TokenBuffer.CUR_LINE, TokenBuffer.CUR_OPER);
            }

            type = NEWVAR;
            FData.push_var(TokenBuffer.CUR_OPER);
        }
        if (!FData.var_exist(TokenBuffer.CUR_OPER))
        {
            FData.dump();
            throw CSyntaxError("Undeclared var!", TokenBuffer.CUR_LINE, TokenBuffer.CUR_OPER);
        }

        Variable = parseVariableLV(TokenBuffer, USUAL, FData);
        Variable ->set_type(type);
        if (Variable->left())
            Variable->set_type(VARINDEX);

        if (TokenBuffer.CUR_DESCR == ASSIGN)
        {
            TokenBuffer.go_next();
            Variable = new CTreeNode(token_t(type), ASSIGN, parseAdd(TokenBuffer, FData), Variable);
            assert(Variable);
        }
        else
            throw CSyntaxError("= is missing!", TokenBuffer.CUR_LINE);

        if (TokenBuffer.CUR_DESCR != ENDOP)
            throw CSyntaxError("; is missing!", TokenBuffer.CUR_LINE);

        TokenBuffer.go_next();
    }

    return Variable;
}

CTreeNode* parseAdd     (CTokenBuffer& TokenBuffer, FunctionData_t& FData)
{
    CTreeNode* Node = parseMul(TokenBuffer, FData);
    assert(Node);

    while (TokenBuffer.CUR_DESCR == ADD || TokenBuffer.CUR_DESCR == SUB)
    {
        if (TokenBuffer.CUR_DESCR == ADD)
        {
            TokenBuffer.go_next();
            Node = new CTreeNode(token_t(ADD), ADD, Node, parseMul(TokenBuffer, FData));
        }
        if (TokenBuffer.CUR_DESCR == SUB)
        {
            TokenBuffer.go_next();
            Node = new CTreeNode(token_t(SUB), SUB, Node, parseMul(TokenBuffer, FData));
        }
    }

    return Node;
}

CTreeNode* parseMul     (CTokenBuffer& TokenBuffer, FunctionData_t& FData)
{
    CTreeNode* Node = parseAssociative(TokenBuffer, FData);

    while (TokenBuffer.CUR_DESCR == MUL || TokenBuffer.CUR_DESCR == DIV)
    {
        if (TokenBuffer.CUR_DESCR == MUL)
        {
            TokenBuffer.go_next();
            Node = new CTreeNode(token_t(MUL), MUL, Node, parseAssociative(TokenBuffer, FData));
        }
        if (TokenBuffer.CUR_DESCR == DIV)
        {
            TokenBuffer.go_next();
            Node = new CTreeNode(token_t(DIV), DIV, Node, parseAssociative(TokenBuffer, FData));
        }
        if (TokenBuffer.CUR_DESCR == MOD)
        {
            TokenBuffer.go_next();
            Node = new CTreeNode(token_t(MOD), MOD, Node, parseAssociative(TokenBuffer, FData));
        }
    }

    assert(Node);
    return Node;
}

CTreeNode* parseAssociative(CTokenBuffer& TokenBuffer, FunctionData_t& FData)
{
    switch(TokenBuffer.CUR_DESCR)
    {
        case LSKOB:
        {
            TokenBuffer.go_next();
            CTreeNode* resultNode = parseAdd(TokenBuffer, FData);
            assert(resultNode);

            if (TokenBuffer.current().Descriptor == RSKOB)
            {
                TokenBuffer.go_next();
                return resultNode;
            }
            else
                throw CSyntaxError("Wrong input! ')' is missing!", TokenBuffer.current().line);

            break;
        }
        case NUMBER:
            return parseNumber  (TokenBuffer);
        default:
            return parseVariableRV(TokenBuffer, FData);
    }

    return parseVariableRV(TokenBuffer, FData);
}
