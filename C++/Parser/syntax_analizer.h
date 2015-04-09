#define CUR_DESCR        current().Descriptor
#define CUR_OPER         current().Oper
#define CUR_LINE         current().line
#define NEXT_DESCR       next().Descriptor
#define NEXT_OPER        next().Oper
#define NEXT_LINE        next().line
#define VAR_EXIST(name)  last_func().var_exist(name)
#define PUSH_VAR (name)  last_func().push_var (name)

const std::string EMPTY      ("");
const std::string PREFIX     ("var");
const std::string IF_STR     ("if");
const std::string WHILE_STR  ("while");
const std::string VOID_STR   ("void");
const std::string INT_STR    ("int");
const std::string DOT_STR    (",");
const std::string ARRAY_STR  ("array");
const std::string RETURN_STR ("return");
const std::string DECLARE_STR("declare");
const std::string OUTPUT_STR ("output");
const std::string INPUT_STR  ("input");
const std::string GLOBAL_STR ("global");

const int USUAL   = 1;
const int NOINDEX = 0;

const int NUMBERS_ON  = 1;
const int NUMBERS_OFF = 0;

//!-----------------------------------------------------
struct FunctionData_t
{
    std::string                 funcName;
    std::vector<std::string>    funcVariables;
    int                         argCnt;

    FunctionData_t(const std::string& name): funcName(name), funcVariables(), argCnt(0) {}

    bool var_exist (const std::string& varName)      const;
    void push_var  (const std::string& varName);
    void push_arg  (const std::string& argName);
    void inc_cnt   () { argCnt++;}
    int  arg_amount() const;
    int  var_amount() const { return funcVariables.size(); }
    void dump      () const;
};
//!-----------------------------------------------------
class  CFunctionsTable
{
    private:
        std::vector<FunctionData_t> FuncData;
    public:
        FunctionData_t& search_function       (const std::string& functionName);
        CFunctionsTable           ();
        void add_func             (const std::string& functionName);
        bool function_exist       (const std::string& functionName) const;
        FunctionData_t& find_func (const std::string& functionName);
        FunctionData_t& last_func () { return FuncData.back(); }
        void dump                 () const;
};

//!-----------------------------------------------------
//! GLOBAL:
CFunctionsTable FunctionsTable;
//!-----------------------------------------------------
CTreeNode* parseCode         (CTokenBuffer& TokenBuffer);
void       parseDeclarations (CTokenBuffer& TokenBuffer);
void       addArguments      (CTokenBuffer& TokenBuffer, FunctionData_t&  Table);
CTreeNode* parseProgram      (CTokenBuffer& TokenBuffer);
CTreeNode* parseFunctions    (CTokenBuffer& TokenBuffer);
CTreeNode* parseFunctionCall (CTokenBuffer& TokenBuffer, FunctionData_t& FData);
CTreeNode* parseArguments    (CTokenBuffer& TokenBuffer, int mode, FunctionData_t& FData, int argMode);
CTreeNode* parseOperators    (CTokenBuffer& TokenBuffer, FunctionData_t& FData);
CTreeNode* parseFSkob        (CTokenBuffer& TokenBuffer, FunctionData_t& FData);
CTreeNode* parseKeyword      (CTokenBuffer& TokenBuffer, FunctionData_t& FData);
CTreeNode* parseInputOutput  (CTokenBuffer& TokenBuffer, FunctionData_t& FData, int Mode);
CTreeNode* parseArrayDeclare (CTokenBuffer& TokenBuffer, FunctionData_t& FData);
CTreeNode* parseReturn       (CTokenBuffer& TokenBuffer, FunctionData_t& FData);
CTreeNode* parseLogicOper    (CTokenBuffer& TokenBuffer, FunctionData_t& FData);
CTreeNode* parseLogicSkob    (CTokenBuffer& TokenBuffer, FunctionData_t& FData);
CTreeNode* parseIfAndWhile   (CTokenBuffer& TokenBuffer, FunctionData_t& FData);
CTreeNode* parseComparison   (CTokenBuffer& TokenBuffer, FunctionData_t& FData);
CTreeNode* parseAssign       (CTokenBuffer& TokenBuffer, FunctionData_t& FData);
CTreeNode* parseAdd          (CTokenBuffer& TokenBuffer, FunctionData_t& FData);
CTreeNode* parseMul          (CTokenBuffer& TokenBuffer, FunctionData_t& FData);
CTreeNode* parseAssociative  (CTokenBuffer& TokenBuffer, FunctionData_t& FData);
CTreeNode* parseNumber       (CTokenBuffer& TokenBuffer);
CTreeNode* parseVariableLV   (CTokenBuffer& TokenBuffer, int mode, FunctionData_t& FData);
CTreeNode* parseVariableRV   (CTokenBuffer& TokenBuffer, FunctionData_t& FData);
CTreeNode* parseIndex        (CTokenBuffer& TokenBuffer, CTreeNode* variableNode, FunctionData_t& FData);
