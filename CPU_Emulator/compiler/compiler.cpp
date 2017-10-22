//{
#include <stdio.h>
#include <io.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
//!------------------------------------------
#define SLASHES printf("--------------------------------------\n");
#define FSLASHES fprintf(output, "--------------------------------------\n");
#define EMPT printf("\n");
#define FEMPT fprintf(output, "\n");
#define TAB printTab(depth, output);
//!------------------------------------------
#define ASSERT_OK(name) assert(treeOk(name, 0) == NULL);
//!------------------------------------------
enum Descriptor_t
{
    NUMBER = 1,
    ADD = 2,
    SUB = 3,
    MUL = 4,
    DIV = 5,
    ASSIG = 6,
    LESS = 7,
    BIG = 8,
    LESS_EQ = 9,
    BIG_EQ = 10,
    EQ = 11,
    OPER = 12,
    VAR = 13,
    OP = 14,
    IF = 15,
    WHILE = 16,
    NEQ = 17,
    FUNC = 18,
    ARG = 19,
    BOP = 20,
    CALLFUNC = 21,
    RETURN = 22,
    ERROR = 23,
    SHOW = 24,
    OUT = 25
};
//!------------------------------------------
const int STRLIM = 100;
const int VARLIM = 100;
//!------------------------------------------
struct lexem_t
{
    Descriptor_t Descriptor;
    int data;
    unsigned int stroke;
    char oper[STRLIM];
};
//!------------------------------------------
struct treeElem_t
{
    Descriptor_t type;
    int data;
    char oper[STRLIM];
    treeElem_t * left;
    treeElem_t * right;
};
//!------------------------------------------
struct lexem_arr_t
{
    lexem_t *data;
    unsigned int cnt;
    unsigned int theSize;
    unsigned int warn;
};
//!------------------------------------------
struct Variable_t
{
    char name[STRLIM];
};
//!------------------------------------------
struct VariableArr_t
{
    Variable_t Var[100];
    unsigned int cnt;
};
//!------------------------------------------
struct Func_t
{
    char name[STRLIM];
    char arg[STRLIM];
};
//!------------------------------------------
struct FuncArr_t
{
    Func_t func[STRLIM];
    unsigned int cnt;
};
//!------------------------------------------
const int BEGIN = 0;
const int TRUE = 1;
const int FALSE = 0;
const int NOEXIST = -1;
//!------------------------------------------
const int DEBUG = 10;
const int NODEBUG = 11;
//!------------------------------------------
const int STARTLEXEM = 1000;
const char PREFIX[] = "tipa";
char IFSTR[] = "if";
char WHILESTR[] = "poka";
const char FUNC_STR[] = "func";
const char DECLARE_STR[] = "declare";
const char MAIN_STR[] = "main";
const char RETURN_STR[] = "return";
const char SHOW_STR[] = "show";
const char ja[] = "ja";
const char jac[] = "jac";
const char jb[] = "jb";
const char jbc[] = "jbc";
const char je[] = "je";
const char jne[] = "jne";
//!------------------------------------------
unsigned int cnt = 0;
unsigned int cnt_lex = 0;
unsigned int flen = 0;
unsigned int node_amount = 0;
unsigned int skip_num = 0;
unsigned int if_num = 0;
unsigned int while_num = 0;
unsigned int func_num = 0;
unsigned int func_skip = 0;
//!------------------------------------------
//}
char * buffer = 0;
lexem_arr_t Lexems = {};
VariableArr_t Variables = {};
FuncArr_t Functions = {};
//!------------------------------------------
void greeting();
void processTask();
    //!--------------------------------------
    //!         FILE AND BUF FUNCS
    //!--------------------------------------
    FILE *formBuffer(FILE *input);
    void initiateLexemsArr();
    void formLexems();
        int findNum();
        Descriptor_t findVar(char var[]);
        Descriptor_t getType();
        void AddLexem(Descriptor_t type, int number, char express[STRLIM], unsigned int slashN);
        void dumpLexems(FILE *output);
        void resizeLexems();
    //!--------------------------------------
    FILE *openFile(char file_name[]);
    void FinishWork(FILE *input, FILE *output, FILE *dump, FILE *viewTree,
                                               FILE *lex_dump, treeElem_t *root);
    int searchVar(char word[]);
    int searchFunc(char word[]);
    void dumpVariables(FILE *output);
    //!-------- DOWNHILL FUNCS --------------
    void getDeclarations();
        void dumpDeclarations();
    treeElem_t * getStart();
    treeElem_t * getProg();
    treeElem_t * getFunc();
    treeElem_t * getCallFunc();
    treeElem_t * getOP();
    treeElem_t * getWhile();
    treeElem_t * getIF();
    treeElem_t * getSkob();
    treeElem_t * getComp();
    treeElem_t * getRav();
    treeElem_t * getVar();
    treeElem_t * getNum();
    treeElem_t * getAdd();
    treeElem_t * getTerm();
    treeElem_t * getPeek();
    //!--------------------------------------
    //!         TREE FUNCTIONS
    //!--------------------------------------
    treeElem_t *ctor (double value, char operation[], Descriptor_t paramType,\
                                                      treeElem_t * left, treeElem_t * right);
    void dtor(treeElem_t *work);
    int treeOk(treeElem_t *root, unsigned int counter);
    void destroyTree(treeElem_t *root);
    //!--------------------------------------
    void dumpTree(treeElem_t *node, const unsigned int mode, int depth, FILE *output);
    void printElem(const unsigned int mode, treeElem_t *node, const int depth, FILE *output);
    void printFuncNode(treeElem_t *node, FILE *output);
    void printNode(treeElem_t *node, FILE *output);
    void printUnderNode(treeElem_t *node, FILE *output);
    void printTab (const int depth, FILE *output);


int main()
{
    greeting();
    processTask();
    return 0;
}
//!------------------------------------------
void greeting()
{
    SLASHES
    printf("Aviator MIPT, %s, %s\n", __DATE__, __TIME__);
    SLASHES
    printf("Hello! I am the Recursive Read!\n");
    SLASHES
}
//!------------------------------------------
void processTask()
{
    FILE * input = NULL;
    input = formBuffer(input);
    FILE * lex_dump = openFile("lex_dump.txt");

    initiateLexemsArr();
    formLexems();
    dumpLexems(lex_dump);

    treeElem_t * tree = getStart();
    ASSERT_OK(tree);

    if (Lexems.data[cnt_lex].Descriptor || Lexems.warn)
    {
        EMPT SLASHES printf("SYNTAX ERROR! RECOMPILE YOUR GOPNIK PROG!\n");
        printf("stroke = %d\n", Lexems.data[cnt_lex].stroke);
        printf("(%s)\n", Lexems.data[cnt_lex].oper);
        printf("cnt_lex = %d\n", cnt_lex);
        SLASHES EMPT
    }

    SLASHES printf("Amount of nodes - (%d)\n", node_amount);
    SLASHES printf("cnt_lex = %d\n", cnt_lex);

    FILE *output = openFile("commands.txt");
    FILE *dump = openFile("dump.txt");
    FILE *viewTree = openFile("tree.txt");

    if (Lexems.warn == FALSE)
    {
        dumpTree(tree, NODEBUG, BEGIN, viewTree);
        dumpTree(tree, DEBUG, BEGIN, dump);
        printFuncNode(tree, output);
    }

    FinishWork(input, output, dump, viewTree, lex_dump, tree);
}
//!------------------------------------------
FILE * formBuffer(FILE *input)
{
    input = fopen("input_task.txt", "r");
    if (input == NULL)
    {
        SLASHES printf("Unable to open input_task.txt!\n");
        exit(1);
    }

    flen = filelength(fileno(input));

    buffer = (char *) calloc(flen + 1, sizeof(char));
    fread(buffer, sizeof(*buffer), flen, input);

    SLASHES printf("Buffer is ready!\n"); SLASHES

    return input;
}
//!------------------------------------------
void initiateLexemsArr()
{
    Lexems.data = (lexem_t *) calloc(STARTLEXEM, sizeof(lexem_t));
    Lexems.cnt = 0;
    Lexems.theSize = STARTLEXEM;
    Lexems.warn = FALSE;
}
//!------------------------------------------
void formLexems()
{
    char var[STRLIM] = {};

    unsigned int slashN = 1;
    while (buffer[cnt])
    {
        while (isspace(buffer[cnt]) || buffer[cnt] == '\n')
        {
            if (buffer[cnt] == '\n')
                slashN++;

            cnt++;
        }

        if (isdigit(buffer[cnt]))
            AddLexem(NUMBER, findNum(), NULL, slashN);
        else
        {
            Descriptor_t typeVar =  findVar(var);
            AddLexem(typeVar, 0, var, slashN);
            memset(var, '\0', strlen(var));
        }
    }
}
//!------------------------------------------
int findNum()
{
    assert(buffer);
    assert(flen);

    int val = 0;

    if (!isdigit(buffer[cnt]))
    {
        printf("ERROR_FINDNUM!\n");
        printf("cnt = %d, str = %s\n", cnt, buffer+cnt);
        exit(2);
    }

    while('0' <= buffer[cnt] && buffer[cnt] <= '9')
    {
        val = val * 10 + buffer[cnt] - '0';
        cnt++;
    }

    return val;
}
//!-----------------------------------------
Descriptor_t findVar(char var[])
{
    assert(buffer);
    assert(flen);

    Descriptor_t type = getType();

    unsigned int i_var = 0;

    if (type == VAR)
    {
        while(isalnum(buffer[cnt]))
        {
            assert(i_var <= STRLIM);
            var[i_var] = buffer[cnt];
            i_var++;
            cnt++;
        }

        return VAR;
    }
    else
    {
        if (type == ADD || type == SUB || type == MUL || type == DIV || type == ASSIG ||
            type == LESS || type == BIG || type == OPER)
        {
            var[0] = buffer[cnt];
            cnt++;
            return type;
        }
        else
        {
            var[0] = buffer[cnt];
            cnt++;
            var[1] = buffer[cnt];
            cnt++;
            return type;
        }
    }
}
//!-----------------------------------------
Descriptor_t getType()
{
    if (isalpha(buffer[cnt]))
        return VAR;


    if (buffer[cnt] == '+')
        return ADD;
    if (buffer[cnt] == '-')
        return SUB;
    if (buffer[cnt] == '*')
        return MUL;
    if (buffer[cnt] == '/')
        return DIV;
    if (buffer[cnt] == '=')
    {
        if (buffer[cnt + 1] == '=')
            return EQ;
        if (buffer[cnt + 1] == '?')
            return NEQ;

        return ASSIG;
    }
    if (buffer[cnt] == '<')
    {
        if (buffer[cnt + 1] == '=')
            return LESS_EQ;
        else
            return LESS;
    }

    if (buffer[cnt]  == '>')
    {
        if (buffer[cnt + 1] == '=')
            return BIG_EQ;
        else
            return BIG;
    }

    if (buffer[cnt] == '(')
        return OPER;
    if (buffer[cnt] == ')')
        return OPER;
    if (buffer[cnt] == '{')
        return OPER;
    if (buffer[cnt] == '}')
        return OPER;
    if (buffer[cnt] == ';')
        return OPER;
}
//!-----------------------------------------
void AddLexem(Descriptor_t type, int number, char express[STRLIM], unsigned int slashN)
{
    Lexems.data[Lexems.cnt].Descriptor = type;

    if (Lexems.cnt == Lexems.theSize - 1)
        resizeLexems();

    if (type == NUMBER)
    {
        Lexems.data[Lexems.cnt].data = number;
        Lexems.data[Lexems.cnt].stroke = slashN;
        //printf("(%s) - %u\n", Lexems.data[Lexems.cnt].oper, Lexems.data[Lexems.cnt].stroke);
        Lexems.cnt++;
        return;
    }

    strcpy(Lexems.data[Lexems.cnt].oper, express);
    Lexems.data[Lexems.cnt].stroke = slashN;
    Lexems.cnt++;
}
//!-----------------------------------------
void dumpLexems(FILE *output)
{
    FSLASHES fprintf(output, "\t LEXEMS\n"); FSLASHES
    fprintf(output, "cnt = %d\n", Lexems.cnt);
    fprintf(output, "cnt_lex = %d\n", cnt_lex);
    fprintf(output, "Lexems = [0x%p]\n", Lexems);
    FSLASHES

    for (int i = 0; i <= Lexems.cnt; i++)
    {
        assert(0 <= i && i <= Lexems.cnt);
        FSLASHES fprintf(output, "(%d) ", i);
        fprintf(output, "type [%d] data <%d> oper <%s> stroke = %u\n",
                        Lexems.data[i].Descriptor, Lexems.data[i].data, Lexems.data[i].oper, Lexems.data[i].stroke);
    }
}
//!-----------------------------------------
void resizeLexems()
{
    Lexems.theSize *= 2;
    Lexems.data = (lexem_t *) realloc(Lexems.data, Lexems.theSize*sizeof(lexem_t));

    for (int i = 0; i < Lexems.theSize; i++)
    {
        assert(0 <= i && i <Lexems.theSize);

        if (Lexems.data[i].Descriptor != NUMBER)
            Lexems.data[i].data = 0;
        else
            Lexems.data[i].oper[0] = '\0';
    }
}
//!-----------------------------------------
FILE *openFile(char file_name[])
{
    assert(file_name);

    FILE * workFile = fopen(file_name, "w+");
    if (!workFile)
    {
        SLASHES printf("#ERROR_<%s>! Unable to open this file!\n", file_name);
        exit(7);
    }

    return workFile;
}
//!-----------------------------------------
void FinishWork(FILE *input, FILE *output,  FILE *dump, FILE *viewTree,
                                            FILE * lex_dump, treeElem_t *root)
{
    assert(input);
    assert(output);
    assert(dump);
    assert(lex_dump);
    assert(viewTree);
    assert(buffer);
    assert(root);

    EMPT SLASHES printf("Work is finished!\n"); SLASHES
    fprintf(output, "end");

    fclose(input);
    fclose(output);
    fclose(dump);
    fclose(viewTree);
    fclose(lex_dump);

    free(buffer);
    free(Lexems.data);
    free(Variables.Var);
    buffer = NULL;

    destroyTree(root);
}
//!-----------------------------------------
//!           TREE FUNCTIONS
//!-----------------------------------------
void printFuncNode(treeElem_t *node, FILE * output)
{
    assert(node);
    assert(output);
    ASSERT_OK(node);

    if (node->left && node->left->left && node->left->right)
    {
        treeElem_t * FuncCode = node->left->left;
        treeElem_t * FuncArgs = node->left->right;
        treeElem_t * theFunc = node->left;

        if (strcmp(theFunc->oper, MAIN_STR) == NULL)
            fprintf(output, "call f_skip0\n");

        fprintf(output, "jmp f_skip%u\n\n", func_skip);
        fprintf(output, "func%d:\n", func_num);

        if (strlen(FuncArgs->oper))
            fprintf(output, "pop %s\n", FuncArgs->oper);

        printNode(FuncCode, output);

        fprintf(output, "ret\n");
        fprintf(output, "\nf_skip%u:\n", func_skip);
        func_num++;
        func_skip++;
    }

    if (node->right)
        printFuncNode(node->right, output);
}
//!-----------------------------------------
void printNode(treeElem_t *node, FILE *output)
{
    assert(node);
    assert(output != NULL);
    ASSERT_OK(node);

    if (output == NULL)
    {
        printf("#ERR_OUTPUT! Unable to create or open output file!\n");
        exit(3);
    }

    if (node->left && node->left->type == OP)
        printNode(node->left, output);

    if (node->left && node->left->type == ASSIG)
    {
        printf("= is spotted!\n");
        treeElem_t * newNode = node->left;
        printUnderNode(newNode->left, output);
        fprintf(output, "pop %s\n", newNode->right->oper);
    }

    if (node && node->type == ASSIG)
    {
        printf("= is spotted!\n");
        printUnderNode(node->left, output);
        fprintf(output, "pop %s\n", node->right->oper);
    }


    if (node->left && node->left->type == IF)
    {
        printf("if is spotted!\n");
        treeElem_t * IFNode = node->left;

        switch(IFNode->left->type)
        {
            #define CASE_COMP(name) printUnderNode(IFNode->left->left, output);\
                                    printUnderNode(IFNode->left->right, output);\
                                                                                 \
                                    fprintf(output, "%s ifY_%d\n", name, if_num);\
                                    fprintf(output, "\njmp ifN_%d\n", skip_num);\
                                    fprintf(output, "\nifY_%d: ", if_num);\
                                    printNode(IFNode->right, output);\
                                    fprintf(output, "\nifN_%d:  ", skip_num);\
                                                                             \
                                    if_num++;\
                                    skip_num++;
            case LESS:
                CASE_COMP(jb)
                break;
            case LESS_EQ:
                CASE_COMP(jbc)
                break;
            case BIG:
                CASE_COMP(ja)
                break;
            case BIG_EQ:
                CASE_COMP(jac)
                break;
            case EQ:
                CASE_COMP(je)
                break;
            case NEQ:
                CASE_COMP(jne)
                break;
            #undef CASE_COMP

        }

    }

    if (node->left && node->left->type == WHILE)
    {
        printf("while is spotted!\n");
        treeElem_t * newNode = node->left;

        #define UNDERTREE_LEFT  printUnderNode(newNode->left->left, output);
        #define UNDERTREE_RIGHT printUnderNode(newNode->left->right, output);
        #define LABEL_YES          fprintf(output, "\nwhileY_%d:   \n", while_num );
        #define LABEL_NO           fprintf(output, "\nwhileN_%d:\n", while_num);
        switch(newNode->left->type)
        {
            case LESS:
                LABEL_YES
                UNDERTREE_LEFT
                UNDERTREE_RIGHT
                fprintf(output, "jac whileN_%d\n", skip_num);

                printNode(newNode->right, output);

                UNDERTREE_LEFT
                UNDERTREE_RIGHT
                fprintf(output, "jb whileY_%d\n", while_num);
                LABEL_NO
                while_num++;
                skip_num++;
                break;
            case LESS_EQ:
                LABEL_YES
                UNDERTREE_LEFT
                UNDERTREE_RIGHT
                //fprintf(output, "push 1\nmin\n");
                fprintf(output, "ja whileN_%d\n", skip_num);

                printNode(newNode->right, output);

                UNDERTREE_LEFT
                UNDERTREE_RIGHT
                //fprintf(output, "push 1\min\n");
                fprintf(output, "jbc whileY_%d \n", while_num);
                LABEL_NO
                while_num++;
                skip_num++;
                break;
            case BIG:
                LABEL_YES
                UNDERTREE_LEFT
                UNDERTREE_RIGHT
                fprintf(output, "jbc whileN_%d\n", skip_num);

                printNode(newNode->right, output);

                UNDERTREE_LEFT
                UNDERTREE_RIGHT
                fprintf(output, "ja whileY_%d\n", while_num);
                LABEL_NO
                while_num++;
                skip_num++;
                break;
            case BIG_EQ:
                LABEL_YES
                UNDERTREE_LEFT
                UNDERTREE_RIGHT
                fprintf(output, "jb whileN_%d\n", skip_num);

                printNode(newNode->right, output);

                UNDERTREE_LEFT
                UNDERTREE_RIGHT
                fprintf(output, "jac whileY_%d\n", while_num);
                LABEL_NO
                while_num++;
                skip_num++;
                break;
            case EQ:
                LABEL_YES
                UNDERTREE_LEFT
                UNDERTREE_RIGHT
                fprintf(output, "jne whileN_%d\n", skip_num);

                printNode(newNode->right, output);

                UNDERTREE_LEFT
                UNDERTREE_RIGHT
                fprintf(output, "je whileY_%d\n", while_num);
                LABEL_NO
                while_num++;
                skip_num++;
                break;
            case NEQ:
                LABEL_YES
                UNDERTREE_LEFT
                UNDERTREE_RIGHT
                fprintf(output, "je whileN_%d\n", skip_num);

                printNode(newNode->right, output);

                UNDERTREE_LEFT
                UNDERTREE_RIGHT
                fprintf(output, "jne whileY_%d\n", skip_num);
                LABEL_NO
                while_num++;
                skip_num++;
                break;
        }
    }

    if (node->right)
        printNode(node->right, output);

}

void printUnderNode(treeElem_t * node, FILE *output)
{
    assert(node);
    assert(output != NULL);
    ASSERT_OK(node);

    if (node->left)
        printUnderNode(node->left, output);
    if (node->right)
        printUnderNode(node->right, output);

    if (node->type == NUMBER)
        fprintf(output, "push %d\n", node->data);
    if (node->type == VAR)
        fprintf(output, "pushr %s\n", node->oper);
    if (node->type == ADD)
        fprintf(output, "add\n");
    if (node->type == SUB)
        fprintf(output, "min\n");
    if (node->type == MUL)
        fprintf(output, "mul\n");
    if (node->type == DIV)
        fprintf(output, "div\n");
}
//!-----------------------------------------
void printElem(const unsigned int mode, treeElem_t *node, const int depth, FILE *output)
{
    assert(mode == DEBUG || mode == NODEBUG);
    assert(node);
    assert(output);
    ASSERT_OK(node);

    if (mode == NODEBUG)
    {
        switch(node->type)
        {
            case NUMBER:
                TAB fprintf(output, "%d\n", node->data);
                break;
            case ADD:
                TAB fprintf(output, "+\n");
                break;
            case SUB:
                TAB fprintf(output, "-\n");
                break;
            case MUL:
                TAB fprintf(output, "*\n");
                break;
            case DIV:
                TAB fprintf(output, "/\n");
                break;
            case ASSIG:
                TAB fprintf(output, "=\n");
                break;
            case EQ:
                TAB fprintf(output, "==\n");
                break;
            case LESS:
                TAB fprintf(output, "<\n");
                break;
            case LESS_EQ:
                TAB fprintf(output, "<=\n");
                break;
            case BIG:
                TAB fprintf(output, ">\n");
                break;
            case BIG_EQ:
                TAB fprintf(output, ">=\n");
                break;
            case VAR:
                TAB fprintf(output, "%s\n", node->oper);
                break;
            case OP:
                TAB fprintf(output, "OP\n");
                break;
            case IF:
                TAB fprintf(output, "if\n");
                break;
            case WHILE:
                TAB fprintf(output, "while\n");
                break;
            case FUNC:
                TAB fprintf(output, "FUNC\n");
                TAB fprintf(output, "%s\n", node->oper);
                break;
            case CALLFUNC:
                TAB fprintf(output, "CALLFUNC\n");
                TAB fprintf(output, "%s\n", node->oper);
                break;
            case RETURN:
                TAB fprintf(output, "RETURN\n");
                break;
            case ARG:
                if (strlen(node->oper))
                {
                    TAB fprintf(output, "ARG\n");
                    TAB fprintf(output, "%s\n", node->oper);
                    TAB fprintf(output, "\n");
                }
                else
                {
                    TAB fprintf(output, "NO ARG\n");
                    TAB fprintf(output, "\n");
                }
                break;
            case BOP:
                TAB fprintf(output, "BOP\n");
                break;
            case ERROR:
                TAB fprintf(output, "ERROR!!!\n");
                break;
            case SHOW:
                TAB fprintf(output, "SHOW\n");
                break;
            case OUT:
                TAB fprintf(output, "OUT\n");
                break;
            default:
                printf("ERROR! %u Descriptor doesn't exist!\n", node->type);
                break;
        }
    }
    else
    {
        FEMPT FSLASHES;
        TAB TAB fprintf(output, "    node [0x%p]\n", node);
        TAB TAB fprintf(output, "    type = <%d>\n", node->type);
        TAB TAB fprintf(output, "    oper = <%s>\n", node->oper);
        TAB TAB fprintf(output, "    data = <%d>\n", node->data);
        TAB TAB fprintf(output, "    left = [0x%p]\n", node->left);
        TAB TAB fprintf(output, "    right = [0x%p]\n", node->right);
    }
}
//!-----------------------------------------
void printTab (const int depth, FILE *output)
{
    assert(output);
    for (int i = 0; i < depth; i++)
    {
        assert(0 <= i && i < depth);
        fprintf(output, "       ");
    }
}
//!-----------------------------------------
void dumpTree(treeElem_t *node, const unsigned int mode, int depth, FILE *output)
{
    assert(node);
    assert(mode == DEBUG || mode || NODEBUG);
    ASSERT_OK(node);

    printElem(mode, node, depth, output);

    if (node->left)
        dumpTree(node->left, mode, ++depth, output);

    depth--;

    if (node->right)
            dumpTree(node->right, mode, ++depth, output);
}
//!-----------------------------------------
int treeOk(treeElem_t *root, unsigned int counter)
{
    if (Lexems.warn == FALSE)
    {
        assert(root);

        if (counter > node_amount)
            return 1;
        counter++;

        if (root->left)
            treeOk(root->left, counter);
        if (root->right)
            treeOk(root->right, counter);
    }

    return 0;
}
//!-----------------------------------------
treeElem_t *ctor (int value, char operation[], Descriptor_t paramType,\
                                                  treeElem_t * theLeft, treeElem_t * theRight)
{
    treeElem_t * work = (treeElem_t *) calloc(1, sizeof(treeElem_t));
    node_amount++;

    work->data = value;
    if (operation)
        strcpy(work->oper, operation);

    work->type = paramType;

    work->left = theLeft;
    work->right = theRight;

    return work;
}
//!-----------------------------------------
void dtor (treeElem_t *work)
{
    assert(work);

    work->left = NULL;
    work->right = NULL;
    work->data = NULL;
    work->type = NUMBER;
    free(work);
}
//!-----------------------------------------
void destroyTree(treeElem_t *root)
{
    assert(root);

    if (root->left)
        destroyTree(root->left);
    if (root->right)
        destroyTree(root->right);

    dtor(root);
}
//!-----------------------------------------
int searchVar(char word[])
{
    assert(word);
    assert(Variables.Var);
    assert(Variables.cnt <= VARLIM);

    for (int i = 0; i <= Variables.cnt; i++)
    {
        assert(0 <= i && i <= Variables.cnt);
        if (!strcmp(word, Variables.Var[i].name))
            return i;
    }

    return NOEXIST;
}
//!-----------------------------------------
int searchFunc(char word[])
{
    assert(word);
    assert(Functions.func);
    assert(Functions.cnt <= VARLIM);

    for (int i = 0; i <= Functions.cnt; i++)
    {
        assert(0 <= i && i <= Functions.cnt);
        if (!strcmp(word, Functions.func[i].name))
            return i;
    }

    return NOEXIST;
}
//!-----------------------------------------
void dumpVariables(FILE * output)
{
    assert(output);
    assert(Variables.Var);

    FEMPT
    fprintf(output, "Variables.cnt = %d\n", Variables.cnt);

    for (int i = 0; i <= Variables.cnt; i++)
    {
        assert(0 <= i && i <= Variables.cnt);
        FSLASHES fprintf(output, "(%d) <%s>\n", i, Variables.Var[i].name);
    }
}
//!-----------------------------------------
//                  RECURSIVE DOWNHILL FUNCS
treeElem_t * getNum()
{
    assert(Lexems.data);
    assert(cnt_lex <= Lexems.cnt);

    treeElem_t *tree = NULL;

    if (Lexems.data[cnt_lex].Descriptor == NUMBER && Lexems.warn == FALSE)
    {
        tree = ctor(NULL, NULL, NUMBER, NULL, NULL);
        ASSERT_OK(tree);
        tree->data = Lexems.data[cnt_lex].data;
        ASSERT_OK(tree);
        cnt_lex++;
    }
    else
        Lexems.warn = TRUE;

    return tree;
}
//!------------------------------------------
treeElem_t * getVar()
{
    assert(Lexems.data);
    assert(cnt_lex <= Lexems.cnt);

    treeElem_t * tree = NULL;
    if (Lexems.data[cnt_lex].Descriptor == VAR && Lexems.warn == FALSE)
    {
        tree = ctor(NULL, NULL, VAR, NULL, NULL);
        if (Lexems.data[cnt_lex + 1].oper[0] == '(')
        {
            tree = getCallFunc();
            return tree;
        }

        if (searchVar(Lexems.data[cnt_lex].oper) != NOEXIST)
            strcpy(tree->oper, Lexems.data[cnt_lex].oper);
        else
        {
            printf("Undeclared var (%s)!\n", Lexems.data[cnt_lex].oper);
            Lexems.warn = TRUE;
            return tree;
        }

        ASSERT_OK(tree);
        cnt_lex++;
    }
    else
    {
        Lexems.warn = TRUE;
        printf("Unable to scan value!\n");
        exit(0);
    }

    return tree;
}
//!------------------------------------------
treeElem_t * getShow()
{
    assert(Lexems.data);
    assert(cnt_lex <= Lexems.cnt);

    treeElem_t * ShowCommand = ctor(0, 0, SHOW, NULL, NULL);

    if (Lexems.data[cnt_lex].Descriptor == VAR)
    {
        if (strcmp(Lexems.data[cnt_lex].oper, SHOW_STR) == NULL)
        {
            cnt_lex++;
            ShowCommand->left = getVar();

            if (Lexems.data[cnt_lex].oper[0] != ';')
            {
                printf("ERROR! ';' is missing in stroke %u!\n", Lexems.data[cnt_lex].stroke);
                Lexems.warn = TRUE;
                return ShowCommand;
            }

            cnt_lex++;
            return ShowCommand;
        }
        else
        {
            printf("ERROR! error in show command!\n");
            Lexems.warn = TRUE;
            return ShowCommand;
        }
    }
    else
    {
        printf("ERROR! in show command in stroke %u!\n", Lexems.data[cnt_lex].stroke);
        Lexems.warn = TRUE;
        return NULL;
    }
}
//!------------------------------------------
treeElem_t * getStart()
{
    assert(Lexems.data);
    assert(cnt_lex <= Lexems.cnt);

    getDeclarations();
    treeElem_t * Start = (treeElem_t *) calloc(1, sizeof(treeElem_t));
    Start = getProg();

    if (strcmp(Start->left->oper, MAIN_STR))
    {
        printf("ERROR! 'main' function should be 1st!\n");
        printf("%s\n", Start->left->oper);
        Lexems.warn = TRUE;;
    }
    return Start;
}
//!------------------------------------------
treeElem_t * getProg()
{
    assert(Lexems.data);
    assert(cnt_lex <= Lexems.cnt);
    treeElem_t * theProg = ctor(0, 0, BOP, getFunc(), NULL);

    while(cnt_lex < Lexems.cnt && Lexems.warn == FALSE)
    {
        treeElem_t * nextFunc = getProg();
        theProg->right = nextFunc;
    }

    return theProg;
}
//!------------------------------------------
treeElem_t * getFunc()
{
    assert(Lexems.data);
    assert(cnt_lex <= Lexems.cnt);

    if (Lexems.data[cnt_lex].Descriptor == VAR)
    {

        if (strcmp(Lexems.data[cnt_lex].oper, FUNC_STR) == NULL)
        {
            cnt_lex++;
            treeElem_t * Func = ctor(0, 0, FUNC, NULL, NULL);

            if (Lexems.data[cnt_lex].Descriptor == VAR)
            {
                if (searchFunc(Lexems.data[cnt_lex].oper) == NOEXIST)
                {
                    printf("Function '%s' in stroke %u doesn't exist!\n", Lexems.data[cnt_lex].oper, Lexems.data[cnt_lex].stroke);
                    Lexems.warn = TRUE;
                    return Func;
                }
                strcpy(Func->oper, Lexems.data[cnt_lex].oper);
                cnt_lex++;
            }
            else
            {
                printf("Unable to create Func! Wrong name type in stroke <%d>\n", Lexems.data[cnt_lex].stroke);
                Lexems.warn == TRUE;
            }

            if (Lexems.data[cnt_lex].oper[0] == '(')
            {
                cnt_lex++;

                char arg_name [STRLIM] = {};
                if (Lexems.data[cnt_lex].Descriptor == VAR)
                {
                    strcpy(arg_name, Lexems.data[cnt_lex].oper);
                    strcpy(Variables.Var[Variables.cnt].name , Lexems.data[cnt_lex].oper);
                    Variables.cnt++;
                    cnt_lex++;
                }

                if (Lexems.data[cnt_lex].oper[0] == ')')
                {
                    cnt_lex++;
                    treeElem_t *func_arg = ctor(0, arg_name, ARG, NULL, NULL);
                    Func = ctor(0, Func->oper, FUNC, getSkob(),func_arg);
                    return Func;
                }
                else
                {
                    printf("ERROR! ')' is missing in stroke %d\n", Lexems.data[cnt_lex].stroke);
                    Lexems.warn = TRUE;
                    return NULL;
                }
            }
        }
    }
}
//!------------------------------------------
treeElem_t * getCallFunc()
{
    assert(Lexems.data);
    assert(cnt_lex <= Lexems.cnt);

    if (Lexems.data[cnt_lex].Descriptor == VAR && Lexems.warn == FALSE)
    {
        if (searchFunc(Lexems.data[cnt_lex].oper) == NOEXIST)
        {
            printf("ERROR! Function '%s' doesn't exist!\n", Lexems.data[cnt_lex].oper);
            Lexems.warn = TRUE;
            return NULL;
        }

        treeElem_t * CallFunc = ctor(0, Lexems.data[cnt_lex].oper, CALLFUNC, NULL, NULL);
        cnt_lex++;
        if (Lexems.data[cnt_lex].oper[0] == '(')
        {
            cnt_lex++;
            CallFunc->left = getAdd();
            if (Lexems.data[cnt_lex].oper[0] == ')')
            {
                cnt_lex++;
                return CallFunc;
            }
            else
            {
                printf("ERROR! ')' is missing in stroke (%u)\n", Lexems.data[cnt_lex].stroke);
                Lexems.warn == TRUE;
                return CallFunc;
            }
        }
    }

    return ctor(0, 0, ERROR, NULL, NULL);
}
//!------------------------------------------
treeElem_t * getFuncReturn()
{
    assert(Lexems.data);
    assert(Functions.func);

    cnt_lex++;
    treeElem_t * theReturn =  ctor(0, 0, RETURN, getAdd(), NULL);
    if (Lexems.data[cnt_lex].oper[0] != ';')
    {
        printf("';' is missing in stroke %u!\n", Lexems.data[cnt_lex].oper);
        Lexems.warn = TRUE;
    }
    cnt_lex++;

    return theReturn;
}
//!------------------------------------------
void getDeclarations()
{
    assert(Lexems.data);
    assert(cnt_lex <= Lexems.cnt);
    assert(Functions.func);

    while (strcmp(Lexems.data[cnt_lex].oper, FUNC_STR))
    {
        if (strcmp(Lexems.data[cnt_lex].oper, DECLARE_STR) == NULL)
        {
            cnt_lex++;
            char func_name[STRLIM] = {};

            if(strcmp(Lexems.data[cnt_lex].oper, FUNC_STR))
            {
                strcpy(Functions.func[Functions.cnt].name, Lexems.data[cnt_lex].oper);
                cnt_lex++;

                if (Lexems.data[cnt_lex].oper[0] ==  '(')
                {
                    cnt_lex++;
                    if (Lexems.data[cnt_lex].Descriptor == VAR)
                    {
                        strcpy(Functions.func[Functions.cnt].arg, Lexems.data[cnt_lex].oper);
                        strcpy(Variables.Var[Variables.cnt].name, Lexems.data[cnt_lex].oper);
                        cnt_lex++;
                        Functions.cnt++;
                        Variables.cnt++;
                    }

                    if (Lexems.data[cnt_lex].oper[0] == ')')
                    {
                        cnt_lex++;
                        if (Lexems.data[cnt_lex].oper[0] == ';')
                            cnt_lex++;
                        else
                        {
                            Lexems.warn = TRUE;
                            printf("; is missing in stroke %u\n", Lexems.data[cnt_lex].stroke);
                            return;
                        }
                    }
                    else
                    {
                        Lexems.warn = TRUE;
                        printf(") is missing in stroke %u\n", Lexems.data[cnt_lex].stroke);
                        break;
                    }
                }
                else
                {
                    printf("( is missing in stroke %u!\n", Lexems.data[cnt_lex].stroke);
                    break;
                }
            }

        }
        else
        {
            printf("Mistake in declaration in stroke %u\n", Lexems.data[cnt_lex].stroke);
            Lexems.warn = TRUE;
            return;
        }
    }

    dumpDeclarations();
    //dumpVariables(stdout);
}
//!------------------------------------------
void dumpDeclarations()
{
    assert(Functions.func);
    assert(Functions.cnt >= 0);

    SLASHES printf("\t FUNCTIONS DECLARE DUMP\n");
    for (int i = 0; i < Functions.cnt; i++)
    {
        assert(0 <= i && i < Functions.cnt);
        SLASHES printf("%d) func <%s> arg (%s)\n", i, Functions.func[i].name, Functions.func[i].arg);
    } SLASHES EMPT
}
//!------------------------------------------
treeElem_t * getOP()
{
    assert(Lexems.data);
    assert(cnt_lex <= Lexems.cnt);

    treeElem_t * theOP = ctor(0, NULL, OP, getSkob(), NULL);

    while (Lexems.warn == FALSE && (Lexems.data[cnt_lex].Descriptor == VAR || Lexems.data[cnt_lex].oper[0] == '{') )
    {
        treeElem_t * nextOP = getOP();
        theOP->right = nextOP;
    }
    return theOP;
}
//!------------------------------------------
treeElem_t * getSkob()
{
    assert(Lexems.data);
    assert(cnt_lex <= Lexems.cnt);

    if (Lexems.data[cnt_lex].oper[0] == '{')
    {
        cnt_lex++;

        treeElem_t * tree = ctor(0, 0, OP, getOP(), NULL);
        ASSERT_OK(tree);


        if (Lexems.data[cnt_lex].oper[0] == '}' )
        {
            cnt_lex++;
            return tree;
        }
        else
        {
            if (Lexems.warn == FALSE)
            {
                SLASHES printf("ERROR! '}' is missing in stroke %u!\n", Lexems.data[cnt_lex].stroke);
                printf("(%s)\n", Lexems.data[cnt_lex].oper);
                Lexems.warn = TRUE;
            }
            return tree;
        }
    }
    else
    {
        if (Lexems.data[cnt_lex].Descriptor == VAR)
        {
            //printf("this is Skob simple case %u\n", Lexems.data[cnt_lex].stroke);
            if (strcmp(Lexems.data[cnt_lex].oper, "while") == NULL)
                return getWhile();
            if (strcmp(Lexems.data[cnt_lex].oper, "if") == NULL)
                return getIF();
            if (Lexems.data[cnt_lex].Descriptor == VAR)
            {
                if (strcmp(Lexems.data[cnt_lex].oper, RETURN_STR) == NULL)
                    return getFuncReturn();
                if (strcmp(Lexems.data[cnt_lex].oper, SHOW_STR) == NULL)
                    return getShow();

                    return getRav();
            }

            Lexems.warn = TRUE;
            return NULL;
        }
    }
}
//!------------------------------------------
treeElem_t * getIF()
{
    assert(Lexems.data);
    assert(cnt_lex <= Lexems.cnt);

    if (Lexems.data[cnt_lex].Descriptor != VAR)
    {
        printf("Unable to create IF construction!\n");
        printf("(%d) [%s]\n", Lexems.data[cnt_lex].Descriptor, Lexems.data[cnt_lex].oper);

        Lexems.warn = TRUE;
        return NULL;
    }

    if (strcmp(Lexems.data[cnt_lex].oper, IFSTR) != NULL)
    {
        printf("wrong in if condition!\n");
    }

    cnt_lex++;

    if (Lexems.data[cnt_lex].oper[0] == '(')
    {
        cnt_lex++;
        treeElem_t * cond = getComp();

        if (Lexems.data[cnt_lex].oper[0] == ')')
        {
            cnt_lex++;
            treeElem_t * condition = ctor(0, "if", IF, cond, getSkob());;
            return condition;
        }
        else
        {
            if (Lexems.warn == FALSE)
            {
                printf("')' is missing in stroke %u!\n", Lexems.data[cnt_lex].stroke);
                Lexems.warn = TRUE;
            }
            return cond;
        }
    }
    else
    {
        printf("'(' is missing in stroke %u\n", Lexems.data[cnt_lex].stroke);
        Lexems.warn = TRUE;
        return NULL;
    }

}
//!------------------------------------------
treeElem_t * getWhile()
{
    assert(Lexems.data);
    assert(cnt_lex <= Lexems.cnt);

    if (strcmp(Lexems.data[cnt_lex].oper, "while") == NULL)
    {
        cnt_lex++;
        //printf("getWhile is called!\n");
        if (Lexems.data[cnt_lex].oper[0] == '(')
        {
            cnt_lex++;
            treeElem_t * cond = getComp();

            if (Lexems.data[cnt_lex].oper[0] == ')')
            {
                cnt_lex++;
                treeElem_t * whileNode = ctor(0, "while", WHILE, cond, getSkob());
                return whileNode;
            }
            else
            {
                if (Lexems.warn == FALSE)
                {
                    printf(") is missing!\n");
                    Lexems.warn = TRUE;
                }
                return cond;
            }
        }
    }
}
//!------------------------------------------
treeElem_t * getComp()
{
    assert(Lexems.data);
    assert(cnt_lex <= Lexems.cnt);

    treeElem_t * one = getAdd();
    treeElem_t * comparison = NULL;

    switch(Lexems.data[cnt_lex].Descriptor)
    {
        case LESS:
            cnt_lex++;
            comparison = ctor(0, "<", LESS, one, getAdd());
            break;
        case LESS_EQ:
            cnt_lex++;
            comparison = ctor(0, "<=", LESS_EQ, one, getAdd());
            break;
        case BIG:
            cnt_lex++;
            comparison = ctor(0, ">", BIG, one, getAdd());
            break;
        case BIG_EQ:
            cnt_lex++;
            comparison = ctor(0, ">=", BIG_EQ, one, getAdd());
            break;
        case EQ:
            cnt_lex++;
            comparison = ctor(0, "==", EQ, one, getAdd());
            break;
        case NEQ:
            cnt_lex++;
            comparison = ctor(0, "=?", NEQ, one, getAdd());
            break;
        default:
            SLASHES printf("Unable to compare!\n");
            Lexems.warn = TRUE;
            return NULL;
            break;
    }
}
//!------------------------------------------
treeElem_t * getRav()
{
    assert(Lexems.data);
    assert(cnt_lex <= Lexems.cnt);

    treeElem_t * Variable = NULL;

    if (Lexems.data[cnt_lex].Descriptor == VAR)
    {
        if (strcmp(Lexems.data[cnt_lex].oper, PREFIX) == NULL)
        {
            cnt_lex++;

            if (searchVar(Lexems.data[cnt_lex].oper) == NOEXIST)
            {
                strcpy(Variables.Var[Variables.cnt].name, Lexems.data[cnt_lex].oper);
                Variables.cnt++;
            }
            else
            {
                printf("ERROR! Redefenition of %s in stroke %u!\n", Lexems.data[cnt_lex].oper, Lexems.data[cnt_lex].stroke);
                Lexems.warn = TRUE;
                return NULL;
            }
        }

        if (searchVar(Lexems.data[cnt_lex].oper) != NOEXIST)
        {
            Variable = ctor(0, Lexems.data[cnt_lex].oper, VAR, NULL, NULL);
            cnt_lex++;

            if (Lexems.data[cnt_lex].Descriptor == ASSIG)
            {
                cnt_lex++;
                Variable = ctor(0, "=", ASSIG, getAdd(), Variable);
                ASSERT_OK(Variable);
            }
            else
            {
                printf("ERROR! '=' is missing in stroke %u!\n", Lexems.data[cnt_lex].stroke);
                Lexems.warn = TRUE;
                return Variable;
            }

            if (Lexems.data[cnt_lex].oper[0] != ';' && Lexems.warn == FALSE)
            {
                printf("ERROR! ; expected in stroke <%d>!\n", Lexems.data[cnt_lex].stroke);
                Variable = ctor(0, "ERR", OP, NULL, NULL);
                Lexems.warn = TRUE;
                return Variable;
            }

            cnt_lex++;
        }
        else
        {
            printf("Undeclared var %s in stroke %u!\n", Lexems.data[cnt_lex].oper, Lexems.data[cnt_lex].stroke);
            Lexems.warn = TRUE;
            return NULL;
        }
    }

    return Variable;
}
//!------------------------------------------
treeElem_t * getAdd()
{
    assert(Lexems.data);
    assert(cnt_lex <= Lexems.cnt);

    treeElem_t * tree = getTerm();
    ASSERT_OK(tree);

    while (Lexems.data[cnt_lex].Descriptor == ADD || Lexems.data[cnt_lex].Descriptor == SUB)
    {
        cnt_lex++;

        if (Lexems.data[cnt_lex - 1].Descriptor == ADD)
        {
            tree = ctor(0, "+", ADD, tree, getTerm());
            ASSERT_OK(tree);
        }
        if (Lexems.data[cnt_lex - 1].Descriptor == SUB)
        {
            tree = ctor(0, "-", SUB, tree, getTerm());
            ASSERT_OK(tree);
        }

    }

    return tree;
}
//!------------------------------------------
treeElem_t * getTerm()
{
    assert(cnt_lex <= Lexems.cnt);

    treeElem_t * tree = getPeek();
    ASSERT_OK(tree);

    while (Lexems.data[cnt_lex].Descriptor == MUL || Lexems.data[cnt_lex].Descriptor == DIV)
    {
        cnt_lex++;

        if (Lexems.data[cnt_lex - 1].Descriptor == MUL)
        {
            tree = ctor(0, "*", MUL, tree, getPeek());
            ASSERT_OK(tree);
        }
        if (Lexems.data[cnt_lex - 1].Descriptor == DIV)
        {
            tree = ctor(0, "/", DIV, tree, getPeek());
            ASSERT_OK(tree);
        }
    }

    return tree;
}
//!------------------------------------------
treeElem_t * getPeek()
{
    assert(Lexems.data);
    assert(cnt_lex <= Lexems.cnt);

    if (Lexems.data[cnt_lex].oper[0] == '(')
    {
        cnt_lex++;
        treeElem_t * tree = getAdd();
        ASSERT_OK(tree);

        if (Lexems.data[cnt_lex].oper[0] == ')')
        {
            cnt_lex++;
            return tree;
        }
        else
        {
            printf("Wrong input! ')' is missing in stroke %u\n", Lexems.data[cnt_lex].stroke);
            Lexems.warn = TRUE;
            return tree;
        }

    }
    else
    {
        if (Lexems.data[cnt_lex].Descriptor == NUMBER)
            return getNum();
        if (Lexems.data[cnt_lex].Descriptor == VAR)
            return getVar();
    }
}
