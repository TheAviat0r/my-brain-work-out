#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <io.h>
#include <ctype.h>
#include <math.h>
#include <locale.h>
//!-------------------------------------------
#define SLASHES printf("--------------------------------------\n");
#define FSLASHES fprintf(output, "--------------------------------------\n");
#define EMPT printf("\n");
#define FEMPT fprintf(output, "\n");
#define TAB printTab(depth, output);
#define CUR_TOKEN Lexems.data[cnt_lex]
#define CUR_OPER  Lexems.data[cnt_lex].oper
#define CUR_SYMB  Lexems.data[cnt_lex].oper[0]
#define CUR_NUM   Lexems.data[cnt_lex].data
//!-------------------------------------------
#define D(name) diffFunc(name)
#define LEFT root->left
#define RIGHT root->right
#define COPY(name) copyTree(name)
//!-------------------------------------------
#define ASSERT_OK(name) assert(treeOk(name, 0) == NULL);
#define streq !strcmp
//!-------------------------------------------
#define FUNC(name, descriptor)  if (streq(CUR_OPER, name))\
                                {\
                                    cnt_lex++;\
                                    \
                                    if (CUR_OPER[0] == '(')\
                                    {\
                                        cnt_lex++;\
                                        treeElem_t *name##_arg = getAdd();\
                                        \
                                        if (CUR_OPER[0] == ')')\
                                        {\
                                            cnt_lex++;\
                                            return ctor(0, 0, descriptor, name##_arg, NULL);\
                                        }\
                                        else\
                                        {\
                                            printf("')' is missing1\n");\
                                            exit(1);\
                                        }\
                                    }\
                                }
//!-------------------------------------------
enum Descriptor_t
{
    NUMBER = 1,
    ADD = '+',
    SUB = '-',
    MUL = '*',
    DIV = '/',
    POW = '^',
    VAR = 7,
    LSKOB = 8,
    RSKOB = 9,
    SIN = 10,
    COS = 11,
    LN = 12,
    EXP = 13,
    ERROR = 1488
};
//!-------------------------------------------
const char Opers[7] = {'+', '-', '*', '/', '^', '(', ')'};
//!-------------------------------------------
const int STRLIM = 100;
const int VARLIM = 100;
//!-------------------------------------------
struct lexem_t
{
    int Descriptor;
    int data;
    char oper[STRLIM];
};
//!-------------------------------------------
struct treeElem_t
{
    int type;
    int data;
    char oper[STRLIM];
    treeElem_t * left;
    treeElem_t * right;
};
//!-------------------------------------------
struct lexem_arr_t
{
    lexem_t *data;
    unsigned int cnt;
    unsigned int theSize;
    unsigned int warn;
};
//!-------------------------------------------
const int BEGIN = 0;
const int TRUE = 1;
const int FALSE = 0;
const int theRIGHT = 1;
const int theLEFT = 2;
const int ROOT = 3;
//!------------------------------------------
const int DEBUG = 10;
const int NODEBUG = 11;
const char SIN_STR [] = "sin";
const char COS_STR [] = "cos";
const char LN_STR []  = "ln";
const char EXP_STR [] = "exp";
//!------------------------------------------
const int STARTLEXEM = 1000;
unsigned int cnt = 0;
unsigned int cnt_lex = 0;
unsigned int flen = 0;
unsigned int node_amount = 0;
treeElem_t * treeHead = NULL;
int isSkobka = 0;
//!------------------------------------------
char * buffer = 0;
lexem_arr_t Lexems = {};
//!------------------------------------------
void greeting();
void processTask();
    //!--------------------------------------
    //!         FILE AND BUF FUNCS
    //!--------------------------------------
    FILE *formBuffer();
    void initiateLexemsArr();
    void formLexems();
        int findNum();
        int findVar(char var[]);
        int getType();
        void AddLexem(int type, int number, char express[STRLIM]);
        void dumpLexems(FILE *output);
        void resizeLexems();
    FILE *openFile(char file_name[]);
    void FinishWork(FILE *input, FILE *output, FILE *dump, FILE *viewTree,
                                               FILE *lex_dump, treeElem_t *root);
    //!--------------------------------------
    //!         DOWNHILL FUNCS
    //!--------------------------------------
    treeElem_t * getVar();
    treeElem_t * getNum();
    treeElem_t * getAdd();
    treeElem_t * getTerm();
    treeElem_t * getPeek();
    //!--------------------------------------
    //!         TREE FUNCTIONS
    //!--------------------------------------
    treeElem_t *ctor (double value, char operation[], int paramType,\
                                                      treeElem_t * left, treeElem_t * right);
    void dtor(treeElem_t *work);
    treeElem_t *copyTree(treeElem_t *root);
    int treeOk(treeElem_t *root, unsigned int counter);
    void destroyTree(treeElem_t *root);
    void optimizeTree(treeElem_t *root);
    treeElem_t *diffFunc(treeElem_t *root);
    void cutConst(treeElem_t *root);
    void foldMul(treeElem_t *root);
    void foldDiv(treeElem_t *root);
    void foldPow(treeElem_t *root);
    void foldAdd(treeElem_t *root);
    void foldSub(treeElem_t *root);
    void foldExp(treeElem_t *root);
    //!--------------------------------------
    void dumpTree(treeElem_t *node, const unsigned int mode, int depth, FILE *output);
    void dumpTex(treeElem_t *root, treeElem_t *parent, FILE * output);
    void printTex(treeElem_t *root_func, treeElem_t *root, FILE *output);
    void printElem(const unsigned int mode, treeElem_t *node, const int depth, FILE *output);
    void printNode(treeElem_t *node, FILE *output);
    void printTab (const int depth, FILE *output);
    //!--------------------------------------
