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
    OP = 14
};
//!------------------------------------------
struct lexem_t
{
    Descriptor_t Descriptor;
    int data;
    char oper[100];
};
//!------------------------------------------
struct treeElem_t
{
    Descriptor_t type;
    int data;
    char oper[100];
    treeElem_t * left;
    treeElem_t * right;
};
//!------------------------------------------
struct lexem_arr_t
{
    lexem_t *data;
    unsigned int cnt;
    unsigned int theSize;
};
//!------------------------------------------
const int BEGIN = 0;
const int TRUE = 1;
const int FALSE = 0;
//!------------------------------------------
const int DEBUG = 10;
const int NODEBUG = 11;
//!------------------------------------------
const int STARTLEXEM = 1000;
const int STRLIM = 100;
const char PREFIX[] = "chetkiy";
//!------------------------------------------
unsigned int cnt = 0;
unsigned int cnt_lex = 0;
unsigned int flen = 0;
unsigned int node_amount = 0;
//!------------------------------------------
//}
char * buffer = 0;
lexem_arr_t Lexems = {};
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
        void AddLexem(Descriptor_t type, int number, char express[STRLIM]);
        void dumpLexems(FILE *output);
        void resizeLexems();
    //!--------------------------------------
    FILE *openFile(char file_name[]);
    void FinishWork(FILE *input, FILE *output, FILE *dump, FILE *viewTree,
                                               FILE *lex_dump, char buffer[], treeElem_t *root);
    //!-------- DOWNHILL FUNCS --------------
    treeElem_t * getOP();
    treeElem_t * getSkob();
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
    void printNode(treeElem_t *node, FILE *output);
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

    EMPT SLASHES
    puts(buffer);
    SLASHES EMPT

    treeElem_t * tree = getOP();
    ASSERT_OK(tree);

    if (Lexems.data[cnt_lex].oper[0] != '\0')
    {
        printf("SYNTAX ERROR! RECOMPILE YOUR GOPNIK PROG!\n");
        printf("cnt_lex = %d\n");
        printf("(%s)\n", Lexems.data[cnt_lex].oper);
    }

    SLASHES printf("Amount of nodes - (%d)\n", node_amount);

    FILE *output = openFile("commands.txt");
    FILE *dump = openFile("dump.txt");
    FILE *viewTree = openFile("tree.txt");

    dumpTree(tree, NODEBUG, BEGIN, stdout);
    dumpTree(tree, DEBUG, BEGIN, dump);
    printNode(tree, output);

    FinishWork(input, output, dump, viewTree, lex_dump, buffer, tree);
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
}
//!------------------------------------------
void formLexems()
{
    char var[STRLIM] = {};

    while (buffer[cnt])
    {
        while (isspace(buffer[cnt]) || buffer[cnt] == '\n')
            cnt++;

        if (isdigit(buffer[cnt]))
            AddLexem(NUMBER, findNum(), NULL);
        else
        {
            Descriptor_t typeVar =  findVar(var);
            AddLexem(typeVar, 0, var);
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

    //printf("number! %d\n", val);

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
        else
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
    if (buffer[cnt] == ';');
        return OPER;
}
//!-----------------------------------------
void AddLexem(Descriptor_t type, int number, char express[STRLIM])
{
    Lexems.data[Lexems.cnt].Descriptor = type;

    if (Lexems.cnt == Lexems.theSize - 1)
        resizeLexems();

    if (type == NUMBER)
    {
        Lexems.data[Lexems.cnt].data = number;
        Lexems.cnt++;
        return;
    }

    strcpy(Lexems.data[Lexems.cnt].oper, express);
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

        /*if (Lexems.data[i].Descriptor == VAR)
            fprintf(output, "type = VAR      ");
        if (Lexems.data[i].Descriptor == OPER)
            fprintf(output, "type = OPER     ");
        if (Lexems.data[i].Descriptor == NUMBER)
            fprintf(output, "type = NUMBER   ");*/

        fprintf(output, "type [%d] data <%d> oper <%s>\n",
                        Lexems.data[i].Descriptor, Lexems.data[i].data, Lexems.data[i].oper);
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
                                            FILE * lex_dump, char *buffer, treeElem_t *root)
{
    assert(input);
    assert(output);
    assert(dump);
    assert(lex_dump);
    assert(viewTree);
    assert(buffer);
    assert(root);

    EMPT SLASHES printf("Work is finished!\n"); SLASHES
    fprintf(output, "pop ax\nout ax\nend\n");

    fclose(input);
    fclose(output);
    fclose(dump);
    fclose(viewTree);
    fclose(lex_dump);

    free(buffer);
    free(Lexems.data);
    buffer = NULL;

    //destroyTree(root);
}
//!-----------------------------------------
//!           TREE FUNCTIONS
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

    if (node->left)
        printNode(node->left, output);

    if (node->right)
        printNode(node->right, output);

    if (node->type == NUMBER)
    {
        fprintf(output, "push %lg\n", node->data);
    }

    if (node->type == OPER)
    {
        if (node->oper[0] == '+')
        {
            fprintf(output, "add\n");
        }

        if (node->oper[0] == '-')
        {
            fprintf(output, "min\n");
        }

        if (node->oper[0] == '*')
        {
            fprintf(output, "mul\n");
        }

        if (node->oper[0] == '/')
        {
            fprintf(output, "div\n");
        }
    }
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
    //ASSERT_OK(node);

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
    assert(root);

    if (counter > node_amount)
        return 1;
    counter++;

    if (root->left)
        treeOk(root->left, counter);
    if (root->right)
        treeOk(root->right, counter);

    return 0;
}
//!-----------------------------------------
treeElem_t *ctor (double value, char operation[], Descriptor_t paramType,\
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

//                  RECURSIVE DOWNHILL FUNCS
treeElem_t * getNum()
{
    assert(Lexems.data);
    assert(cnt_lex <= Lexems.cnt);

    treeElem_t *tree = NULL;

    if (Lexems.data[cnt_lex].Descriptor == NUMBER)
    {
        tree = ctor(NULL, NULL, NUMBER, NULL, NULL);
        ASSERT_OK(tree);
        tree->data = Lexems.data[cnt_lex].data;
        ASSERT_OK(tree);
        cnt_lex++;
    }

    return tree;
}
//!------------------------------------------
treeElem_t * getVar()
{
    assert(Lexems.data);
    assert(cnt_lex <= Lexems.cnt);

    treeElem_t * tree = NULL;
    if (Lexems.data[cnt_lex].Descriptor == VAR)
    {
        tree = ctor(NULL, NULL, VAR, NULL, NULL);
        ASSERT_OK(tree);
        strcpy(tree->oper, Lexems.data[cnt_lex].oper);
        ASSERT_OK(tree);
        cnt_lex++;
    }

    return tree;
}
//!------------------------------------------
/*treeElem_t * getOP()
{
    assert(Lexems.data);
    assert(cnt_lex <= Lexems.cnt);

    treeElem_t * theOP = ctor(0, NULL, OP, getSkob(), NULL);
    cnt_lex++;
    EMPT SLASHES  printf("1st theOP cnt_lex = %d\n", cnt_lex);

    whi)
    {
        treeElem_t * nextOP = getOP();
        theOP->right = nextOP;
    }


    return theOP;
}*/
//!------------------------------------------
treeElem_t * getOP()
{
    assert(Lexems.data);
    assert(cnt_lex <= Lexems.cnt);

    treeElem_t * theOP = ctor(0, NULL, OP, getSkob(), NULL);
    SLASHES printf("theOP cnt_lex = %d\n", cnt_lex);

    while (Lexems.data[cnt_lex].oper[0] != '\0')
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

        printf("{ is spotted!\n");
        printf("before 1st getOP (%d): (%s)\n", cnt_lex, Lexems.data[cnt_lex].oper);

        treeElem_t * tree = getOP();
        ASSERT_OK(tree);

        printf("after 1st getOp: (%s)\n", Lexems.data[cnt_lex].oper);

        if (Lexems.data[cnt_lex].oper[0] == '}')
        {
            printf("} is spotted!\n");
            cnt_lex++;
            return tree;
        }
        else
        {
            printf("ERROR! } is missing!\n");
            printf("(%s)\n", Lexems.data[cnt_lex].oper);
            exit(11);
        }
    }
    else
        return getRav();
}
//!------------------------------------------
/*treeElem_t * getSkob()
{
    assert(Lexems.data);
    assert(cnt_lex <= Lexems.cnt);

    if (Lexems.data[cnt_lex].oper[0] == '{')
    {
        cnt_lex++;
        printf("1st is spotted!\n");
        treeElem_t * tree= getOP();
        ASSERT_OK(tree);

        if (Lexems.data[cnt_lex].oper[0] == '}')
        {
            printf("spotted!\n");
            cnt_lex++;
            return tree;
        }
        else
        {
            printf("ERROR! '}' is missing!\n");
            printf("cnt_lex = %d\n", cnt_lex);
            printf("(%s)\n", Lexems.data[cnt_lex].oper);
            exit(10);
        }
    }
    else
        return getRav();
}*/
//!------------------------------------------
treeElem_t * getRav()
{
    assert(Lexems.data);
    assert(cnt_lex <= Lexems.cnt);

    if (Lexems.data[cnt_lex].Descriptor != VAR || Lexems.data[cnt_lex + 1].Descriptor != VAR)
    {
        printf("ERROR! OPERATOR SHOULD BEGIN WITH VAR!\n");
        printf("PLACE: <%d> (%s)\n", cnt_lex, Lexems.data[cnt_lex].oper);
        dumpLexems(stdout);
        exit(5);
    }

    if (strcmp(Lexems.data[cnt_lex].oper, PREFIX))
    {
        printf("ERROR INITIALISATION! VAR isn't CHETKIY!\N");
        exit(6);
    }

    cnt_lex++;

    //printf("2! cnt_lex = %d\n", cnt_lex);
    treeElem_t * Variable = ctor(0, Lexems.data[cnt_lex].oper, VAR, NULL, NULL);
    //printf("3! cnt_lex = %d\n", cnt_lex);
    cnt_lex++;

    //printf("4! cnt_lex = %d\n", cnt_lex);
    if (Lexems.data[cnt_lex].Descriptor == ASSIG)
    {
        cnt_lex++;
       // printf("5! cnt_lex = %d\n", cnt_lex);
        Variable = ctor(0, "=", ASSIG, Variable, getAdd());
        ASSERT_OK(Variable);
    }

    //printf("6! cnt_lex = %d\n", cnt_lex);
    if (Lexems.data[cnt_lex].oper[0] != ';')
    {
        printf("ERROR! ; expected!\n");
        printf("<%s>\n", Lexems.data[cnt_lex].oper);
    }

    cnt_lex++;
    //printf("after OP: <%c>\n", Lexems.data[cnt_lex].oper[0]);
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
            printf("Wrong input! ')' is missing!\n");
            exit(3);
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
