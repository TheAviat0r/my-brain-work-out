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
typedef int NodeType;
//!------------------------------------------
struct treeElem_t
{
    NodeType type;
    double data;
    char oper;
    treeElem_t * left;
    treeElem_t * right;
};
//!------------------------------------------
struct treeHead_t
{
    treeElem_t * root;
    unsigned int amount;
};
//!------------------------------------------
const int NUMBER = 1;
const int OPERATOR = 2;
const int BEGIN = 0;
const int TRUE = 1;
const int FALSE = 0;
//!------------------------------------------
const int DEBUG = 10;
const int NODEBUG = 11;
//!------------------------------------------
unsigned int cnt = 0;
unsigned int flen = 0;
unsigned int node_amount = 0;
unsigned int calls_cnt = 0;
//!------------------------------------------
char * buffer = 0;
treeElem_t ** Elem_arr = 0;
treeHead_t theHead = {};
//!------------------------------------------
void greeting();
void processTask();
    //!--------------------------------------
    //!         FILE AND BUF FUNCS
    //!--------------------------------------
    FILE *formBuffer(FILE *input);
    FILE *openFile(char file_name[]);
    void FinishWork(FILE *input, FILE *output, FILE *dump, FILE *viewTree, char buffer[], treeElem_t *root);
    //!--------------------------------------
    //!     RECURSIVE DOWNHILL FUNCS
    //!--------------------------------------
    treeElem_t * getNum();
    treeElem_t * getAdd();
    treeElem_t * getTerm();
    treeElem_t * getPeek();
    //!--------------------------------------
    //!         TREE FUNCTIONS
    //!--------------------------------------
    treeElem_t *ctor (double value, char operation, int paramType, treeElem_t * left, treeElem_t * right);
    void dtor(treeElem_t *work);
    int treeOk(treeElem_t *root, unsigned int counter);
    unsigned int passTree(treeElem_t *currentEl, treeElem_t *root, unsigned int elCnt, unsigned int currCnt);
    void destroyTree (treeElem_t *root);
    //!--------------------------------------
    //!         PRINT FUNCTIONS
    void dumpTree(treeElem_t *node, const unsigned int mode, int depth, FILE *output);
    void printElem(const unsigned int mode, treeElem_t *node, const int depth, FILE *output);
    void printNode(treeElem_t *node, FILE *output);
    void printTab (const int depth, FILE *output);
//!------------------------------------------
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
    FILE *input = NULL;
    input = formBuffer(input);

    EMPT SLASHES puts(buffer); SLASHES

    treeElem_t * tree = getAdd();
    ASSERT_OK(tree);

    if (buffer[cnt])
    {
        printf("SYNTAX ERROR <buffer[%d] [%s]>! RECOMPILE YOUR PROG!\n", cnt, buffer+cnt);
        exit(2);
    }

    SLASHES printf("Amount of Nodes - (%d)\n", node_amount); SLASHES

    FILE *output = openFile("commands.txt");
    FILE *dump = openFile("dump.txt");
    FILE *viewTree = openFile("view_tree.txt");

    dumpTree(tree, DEBUG, BEGIN, dump);
    dumpTree(tree, NODEBUG, BEGIN, viewTree);
    printNode(tree, output);

    FinishWork(input, output, dump, viewTree, buffer, tree);
}
//!------------------------------------------
FILE* formBuffer(FILE *input)
{
    input = fopen("input_task.txt", "r");
    if (input == NULL)
    {
        SLASHES printf("Unable to open input.txt! Try again!\n");
        exit(1);
    }

    flen = filelength(fileno(input));

    buffer = (char *) calloc(flen+1, sizeof(*buffer));
    fread(buffer, sizeof(*buffer), flen, input);

    return input;
}
//!------------------------------------------
FILE* openFile(char file_name[])
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
//!------------------------------------------
void FinishWork(FILE *input, FILE *output, FILE *dump, FILE *viewTree, char *buffer, treeElem_t *root)
{
    assert(input);
    assert(output);
    assert(dump);
    assert(viewTree);
    assert(buffer);
    assert(root);

    EMPT SLASHES printf("Work is finished!\n"); SLASHES
    fprintf(output, "pop ax\nout ax\nend\n");

    fclose(input);
    fclose(output);
    fclose(dump);
    fclose(viewTree);

    free(buffer);
    buffer = NULL;

    destroyTree(root);
}
//!------------------------------------------
treeElem_t * getNum()
{
    assert(buffer != NULL);
    assert(cnt <= flen);

    treeElem_t *tree = ctor(NULL, NULL, NUMBER, NULL, NULL);
    ASSERT_OK(tree);

    while ('0' <= buffer[cnt] && buffer[cnt] <= '9')
    {
        tree->data= (tree->data)*10 + buffer[cnt] - '0';
        cnt++;
    }

    return tree;
}
//!------------------------------------------
treeElem_t * getAdd()
{
    assert(buffer != NULL);
    assert(cnt <= flen);

    treeElem_t * tree = getTerm();
    ASSERT_OK(tree);

    while (buffer[cnt] == '+' || buffer[cnt] == '-')
    {
        cnt++;

        if (buffer[cnt - 1] == '+')
        {
            tree = ctor(0, '+', OPERATOR, tree, getTerm());
            ASSERT_OK(tree);
        }
        if (buffer[cnt - 1] == '-')
        {
            tree = ctor(0, '-', OPERATOR, tree, getTerm());
            ASSERT_OK(tree);
        }

    }

    return tree;
}
//!------------------------------------------
treeElem_t * getTerm()
{
    treeElem_t * tree = getPeek();
    ASSERT_OK(tree);

    while (buffer[cnt] == '*' || buffer[cnt] == '/')
    {
        cnt++;

        if (buffer[cnt - 1] == '*')
        {
            tree = ctor(0, '*', OPERATOR, tree, getPeek());
            ASSERT_OK(tree);
        }
        if (buffer[cnt - 1] == '/')
        {
            tree = ctor(0, '/', OPERATOR, tree, getPeek());
            ASSERT_OK(tree);
        }
    }

    return tree;
}
//!------------------------------------------
treeElem_t * getPeek()
{
    if (buffer[cnt] == '(')
    {
        cnt++;
        treeElem_t * tree = getAdd();
        ASSERT_OK(tree);

        if (buffer[cnt] == ')')
        {
            cnt++;
            return tree;
        }
        else
        {
            printf("Wrong input! ')' is missing!\n");
            exit(3);
        }

    }
    else
        return getNum();
}
//!------------------------------------------
treeElem_t * ctor(double value, char operation, int paramType, treeElem_t *theLeft, treeElem_t *theRight)
{
    assert(paramType == NUMBER || paramType == OPERATOR);
    assert(operation == '*' || operation == '/' || operation == '-' || operation == '+' || operation == '\0');

    treeElem_t * work = (treeElem_t *) calloc(1, sizeof(treeElem_t));
    node_amount++;

    work->data = value;
    work->oper = operation;
    work->type = paramType;

    work->left = theLeft;
    work->right = theRight;
    //ASSERT_OK(work);

    return work;
}
//!---------------------------------
void dtor (treeElem_t *work)
{
    assert(work);

    work->left = NULL;
    work->right = NULL;
    work->data = NULL;
    work->type = NULL;
    free(work);
}
//!---------------------------------
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

    if (node->type == OPERATOR)
    {
        if (node->oper == '+')
        {
            fprintf(output, "add\n");
        }

        if (node->oper == '-')
        {
            fprintf(output, "min\n");
        }

        if (node->oper == '*')
        {
            fprintf(output, "mul\n");
        }

        if (node->oper == '/')
        {
            fprintf(output, "div\n");
        }
    }
}
//!---------------------------------
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
//!---------------------------------
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
//!---------------------------------
//!     passTree will be released later in next treeOk version
//!     this version will be much better than current
//!---------------------------------
unsigned int passTree(treeElem_t *currentEl, treeElem_t *root, unsigned int elCnt, unsigned int currCnt)
{
    if (elCnt > node_amount)
        return FALSE;

    if (root == currentEl)
        currCnt++;

    if (currCnt > 1)
        return FALSE;

    elCnt++;

    if (root->left)
        passTree(currentEl, root->left, elCnt, currCnt);
    if (root->right)
        passTree(currentEl, root->right, elCnt, currCnt);

    return TRUE;
}
//!---------------------------------
void printElem(const unsigned int mode, treeElem_t *node, const int depth, FILE *output)
{
    assert(mode == DEBUG || mode == NODEBUG);
    assert(node);
    assert(output);
    ASSERT_OK(node);

    if (mode == NODEBUG)
    {
        if (node->type == NUMBER)
        {
            TAB fprintf(output, "%lg\n", node->data);
        }

        if (node->type == OPERATOR)
        {
            if (node->oper == '+')
            {
                TAB fprintf(output, "+\n");
            }

            if (node->oper == '-')
            {
                TAB fprintf(output, "-\n");
            }

            if (node->oper == '*')
            {
                TAB fprintf(output, "*\n");
            }

            if (node->oper == '/')
            {
                TAB fprintf(output, "/\n");
            }
        }
    }
    else
    {
        FEMPT FSLASHES;
        TAB TAB fprintf(output, "    node [0x%p]\n", node);
        TAB TAB fprintf(output, "    type = <%d>\n", node->type);
        TAB TAB fprintf(output, "    oper = <%c>\n", node->oper);
        TAB TAB fprintf(output, "    data = <%lg>\n", node->data);
        TAB TAB fprintf(output, "    left = [0x%p]\n", node->left);
        TAB TAB fprintf(output, "    right = [0x%p]\n", node->right);
    }
}
//!---------------------------------
void printTab (const int depth, FILE *output)
{
    assert(output);
    for (int i = 0; i < depth; i++)
    {
        assert(0 <= i && i < depth);
        fprintf(output, "   ");
    }
}
//!----------------------------------
void destroyTree(treeElem_t *root)
{
    assert(root);

    if (root->left)
        destroyTree(root->left);
    if (root->right)
        destroyTree(root->right);

    dtor(root);
}
