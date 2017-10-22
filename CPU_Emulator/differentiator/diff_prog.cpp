#include "header.h"

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

    treeElem_t * tree = getAdd();
    ASSERT_OK(tree);

    if (CUR_TOKEN.Descriptor || Lexems.warn)
    {
        EMPT SLASHES printf("SYNTAX ERROR! CHECK YOUR FUNCTION!\n");
        printf("(%s)\n", CUR_OPER);
        printf("cnt_lex = %d\n", cnt_lex);
        SLASHES EMPT
    }

    FILE *output = openFile("commands.txt");
    FILE *dump = openFile("dump.txt");
    FILE *viewTree = openFile("tree.txt");
    FILE *viewDiff = openFile("diff.txt");

    if (Lexems.warn == FALSE)
    {
        optimizeTree(tree);
        dumpTree(tree, NODEBUG, BEGIN, stdout);
        dumpTree(tree, DEBUG, BEGIN, dump);

        treeElem_t * D_Func = diffFunc(tree);
        SLASHES printf("NON-OPTIMIZED!\n");
        dumpTree(D_Func, NODEBUG, BEGIN, stdout);
        SLASHES printf("OPTIMIZED!\n");
        optimizeTree(D_Func);
        dumpTree(D_Func, NODEBUG, BEGIN, stdout);
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
    puts(buffer);

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

    while (buffer[cnt])
    {
        while (isspace(buffer[cnt]) || buffer[cnt] == '\n')
            cnt++;

        if (isdigit(buffer[cnt]))
            AddLexem(NUMBER, findNum(), NULL);
        else
        {
            int typeVar =  findVar(var);
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

    while('0' <= buffer[cnt] && buffer[cnt] <= '9')
    {
        val = val*10 + buffer[cnt] - '0';
        cnt++;
    }

    return val;
}
//!------------------------------------------
int findVar(char var[])
{
    assert(buffer);
    assert(flen);

    int type = getType();

    if (type == ERROR)
    {
        printf("ERROR IN READING! %c\n", buffer[cnt]);
        exit(1);
    }

    unsigned int i_var = 0;

    if (type == VAR)
    {
        while(isalpha(buffer[cnt]))
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
        var[0] = buffer[cnt];
        cnt++;
        return type;
    }
}
//!-----------------------------------------
int getType()
{
    if (isalpha(buffer[cnt]))
        return VAR;

    char *symb = strchr(Opers, buffer[cnt]);
    int result = *symb;

    if (symb)
        return result;
    else
        return ERROR;
}
//!-----------------------------------------
void AddLexem(int type, int number, char express[STRLIM])
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

    for (unsigned int i = 0; i <= Lexems.cnt; i++)
    {
        assert(0 <= i && i <= Lexems.cnt);
        FSLASHES fprintf(output, "(%d) ", i);
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
    buffer = NULL;

    destroyTree(root);
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
            #define CASE_TYPE (name, oper)  case name:\
                                                TAB fprintf(output, "oper\n");\
                                                break;
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
            case POW:
                TAB fprintf(output, "^\n");
                break;
            case VAR:
                TAB fprintf(output, "%s\n", node->oper);
                break;
            case SIN:
                TAB fprintf(output, "SIN\n");
                break;
            case COS:
                TAB fprintf(output, "COS\n");
                break;
            case LN:
                TAB fprintf(output, "LN\n");
                break;
            case EXP:
                TAB fprintf(output, "EXP\n");
                break;
            default:
                printf("ERROR! Wrong Descriptor %d\n!", node->type);
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
//!------------------------------------------
treeElem_t *ctor (int value, char operation[], int paramType,\
                                                  treeElem_t * theLeft, treeElem_t * theRight)
{
    //printf("paramType = %d\n", paramType);
    assert(paramType == ADD || paramType == SUB || paramType == DIV || paramType == MUL || paramType == POW
        || paramType == VAR || paramType == NUMBER || paramType == SIN || paramType == COS || paramType == LN ||
           paramType == EXP);

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

    if (work->left)
        work->left = NULL;
    if (work->right)
        work->right = NULL;
    work->data = NULL;
    work->type = NUMBER;
    free(work);
}
//!-----------------------------------------
treeElem_t * copyTree(treeElem_t *root)
{
    assert(root);

    treeElem_t * theCopy = ctor(root->data, root->oper, root->type, root->left, root->right);

    if (root->left)
        theCopy->left =  copyTree(root->left);
    if (root->right)
        theCopy->right = copyTree(root->right);

    return theCopy;
}
//!-----------------------------------------
void optimizeTree(treeElem_t *root)
{
    #define LEFT root->left
    #define RIGHT root->right
    ASSERT_OK(root);

    if (root->left)
        optimizeTree(root->left);
    if (root->right)
        optimizeTree(root->right);

    if (root->left && root->right && root->left->type == NUMBER && root->right->type == NUMBER)
        cutConst(root);

    switch(root->type)
    {
        case MUL:
            foldMul(root);
            break;
        //case DIV:
           // foldDiv(root);
            //break;
        case ADD:
            foldAdd(root);
            break;
        case SUB:
            foldSub(root);
            break;
        case POW:
            foldPow(root);
            break;
        case EXP:
            foldExp(root);
            break;
    }
}
//!-----------------------------------------
void foldMul(treeElem_t *root)
{
    #define CLEAN_LR root->left = NULL; root->right = NULL;
    assert(root);

    if ((root->left->type == NUMBER && root->left->data == 0) || (root->right->type == NUMBER && root->right->data == 0))
    {
        root->type = NUMBER;
        root->data = 0;
        root->oper[0] = '\0';
        destroyTree(root->left);
        destroyTree(root->right);
        CLEAN_LR;
        return;
    }

    if (root->left->type == NUMBER && root->left->data == 1)
    {
        dtor(root->left);
        root->left = NULL;
        *root = *(root->right);
        return;
    }

    if (root->right->type == NUMBER && root->right->data == 1)
    {
        dtor(root->right);
        root->right = NULL;
        *root = *(root->left);
        return;
    }
}
//!-----------------------------------------
void foldExp(treeElem_t *root)
{
    assert(root);

    if (LEFT->type == LN)
    {
        root->right = NULL;
        *root = *(root->left->left);
        return;
    }

    if (LEFT->type == MUL)
    {
        if (LEFT->left->type == LN)
        {
            root->right = NULL;
            *(LEFT->left) = *(LEFT->left->left);
            *root = *(LEFT);
            root->type = POW;
            /*treeElem_t *NewPow = ctor(0, 0, POW, COPY(LEFT), COPY(RIGHT));
            destroyTree(LEFT);
            *root = *NewPow*/
            return;
        }

        if (LEFT->right->type == LN)
        {
            root->right = NULL;
            *(LEFT->right) = *(LEFT->right->left);
            *root = *(LEFT);
            root->type = POW;
            return;
        }
    }
}
//!-----------------------------------------
void foldDiv(treeElem_t *root)
{
    assert(root);

    if (root->right->type == NUMBER && root->right->data == 1)
    {
        dtor(root->right);
        root->right= NULL;
        *root = *(root->left);
        return ;
    }
}
//!-----------------------------------------
void foldPow(treeElem_t *root)
{
    assert(root);

    if (root->right->type == NUMBER && root->right->data == 1)
    {
        dtor(root->right);
        root->right = NULL;
        *root = *(root->left);
        return;
    }

    if ((root->right->type == NUMBER && root->right->data == 0) || (root->left->type == NUMBER && root->left->data == 1))
    {
        #define CUT_LR root->left = NULL; root->right = NULL; free(root->left); free(root->right);
        CUT_LR;
        root->type = NUMBER;
        root->data = 1;
        root->oper[0] = '\0';
    }
}
//!-----------------------------------------
void foldAdd(treeElem_t *root)
{
    assert(root);

    if (root->left->type == NUMBER && root->left->data == 0)
    {
        dtor(root->left);
        root->left = NULL;
        *root= *(root->right);
        return;
    }

    if (root->right->type == NUMBER && root->right->data == 0)
    {
        dtor(root->right);
        root->right = NULL;
        *root = *(root->left);
        return;
    }
}
//!-----------------------------------------
void foldSub(treeElem_t *root)
{
    assert(root);

    if (root->left->type == NUMBER && root->left->data == 0)
    {
        dtor(root->left);
        root->left= NULL;
        *root = * ctor(0, 0, MUL, ctor(-1, 0, NUMBER, NULL, NULL), root->right);
        return;
    }

    if (root->right->type == NUMBER && root->left->data == 0)
    {
        dtor(root->right);
        root->right = NULL;
        *root = *(root->left);
        return;
    }
}
//!-----------------------------------------
void cutConst(treeElem_t *root)
{
    assert(root);

    #define CUT_LR root->left = NULL; root->right = NULL; free(root->left); free(root->right);
    if (root->type == ADD)
    {
        root->type = NUMBER;
        root->data = root->left->data + root->right->data;
        CUT_LR
    }

    if (root->type == SUB)
    {
        root->type = NUMBER;
        root->data = root->left->data - root->right->data;
        CUT_LR
    }

    if (root->type == MUL)
    {
        root->type = NUMBER;
        root->data = root->left->data * root->right->data;
        CUT_LR
    }

    if (root->type == DIV)
    {
        root->type = NUMBER;
        root->data = root->left->data / root->right->data;
        CUT_LR
    }
    #undef CUT_LR
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
treeElem_t * diffFunc(treeElem_t * root)
{
    assert(root);

    switch(root->type)
    {
        case NUMBER:
            return ctor(0, 0, NUMBER, NULL, NULL);
            break;
        case VAR:
            return ctor(1, 0, NUMBER, NULL, NULL);
            break;
        case MUL:
        {
            treeElem_t * newRoot = ctor(0, "+", ADD, NULL, NULL);
            treeElem_t * theLeft = ctor(0, "*", MUL, D(LEFT), COPY(RIGHT));
            treeElem_t * theRight = ctor(0, "*", MUL, COPY(LEFT), D(RIGHT));
            newRoot->left = theLeft;
            newRoot->right = theRight;

            return newRoot;
            break;
        }
        case ADD:
            return ctor(0, "+", ADD, D(LEFT), D(RIGHT));
            break;
        case SUB:
            return ctor(0, "-", SUB, D(LEFT), D(RIGHT));
            break;
        case POW:
        {
            if (RIGHT->type == NUMBER)
            {
                root->right->data -= 1;
                treeElem_t * theRoot = ctor(0, 0, MUL, ctor(root->right->data + 1, 0, NUMBER, NULL, NULL) , COPY(root));
                root->right->data += 1;
                treeElem_t * newRoot = ctor(0, 0, MUL, theRoot, D(root->left));
                return newRoot;
                break;
            }
            else
            {
                treeElem_t * theExp = ctor(0, 0, EXP, ctor(0, 0, MUL, COPY(RIGHT), ctor(0, 0, LN, COPY(LEFT), NULL)), NULL);
                SLASHES printf("exp is founded!\n");
                dumpTree(theExp, NODEBUG, BEGIN, stdout);
                return D(theExp);
            }

            break;
        }
        case DIV:
        {
            treeElem_t *UpSide = ctor(0, 0, SUB, ctor(0, 0, MUL, D(LEFT), COPY(RIGHT)), ctor(0, 0, MUL, COPY(LEFT), D(RIGHT)));
            treeElem_t *DownSide = ctor(0, 0, POW, COPY(root->right), ctor(2, 0, NUMBER, NULL, NULL));
            treeElem_t *Result = ctor(0, 0, DIV, UpSide, DownSide);

            return Result;
            break;
        }
        case SIN:
        {
            treeElem_t *CosNode = ctor(0, 0, COS, COPY(LEFT), NULL);
            treeElem_t *Result = ctor(0, 0, MUL, CosNode, D(LEFT));
            return Result;
            break;
        }
        case COS:
        {
            treeElem_t *SinNode = ctor(0, 0, SIN, COPY(LEFT), NULL);
            treeElem_t *ComplFunc = D(LEFT);
            treeElem_t *Result = ctor(0, 0, MUL, ctor(0, 0, MUL, SinNode, ComplFunc), ctor(-1, 0, NUMBER, NULL, NULL));
            return Result;
            break;
        }
        case LN:
        {
            treeElem_t *LnDiff = ctor(0, 0, DIV, ctor(1, 0, NUMBER, NULL, NULL), COPY(LEFT));
            treeElem_t *ComplFunc = D(LEFT);
            treeElem_t *Result = ctor(0, 0, MUL, LnDiff, ComplFunc);
            return Result;
            break;
        }
        case EXP:
        {
            treeElem_t *ExpBase = COPY(root);
            treeElem_t *PowDiff = D(LEFT);
            treeElem_t *Result = ctor(0, 0, MUL, ExpBase, PowDiff);
            return Result;
            break;
        }
    }

}
//!-----------------------------------------
treeElem_t * getNum()
{
    assert(Lexems.data);
    assert(cnt_lex <= Lexems.cnt);

    treeElem_t *tree = NULL;

    if (CUR_TOKEN.Descriptor == NUMBER && Lexems.warn == FALSE)
    {
        if (Lexems.data[cnt_lex + 1].oper[0] == '^')
        {
            printf("pow is spotted!\n");
            int newNum = pow(Lexems.data[cnt_lex].data, Lexems.data[cnt_lex + 2].data);
            tree = ctor(newNum, "+", NUMBER, NULL, NULL);
            cnt_lex += 3;
            return tree;
        }

        tree = ctor(NULL, NULL, NUMBER, NULL, NULL);
        ASSERT_OK(tree);
        tree->data = CUR_NUM;
        ASSERT_OK(tree);
        cnt_lex++;
    }
    else
        Lexems.warn = TRUE;

    return tree;
}
//!-----------------------------------------
treeElem_t * getVar()
{
    assert(Lexems.data);
    assert(cnt_lex <= Lexems.cnt);

    treeElem_t * tree = NULL;

    if (CUR_TOKEN.Descriptor == VAR && Lexems.warn == FALSE)
    {
        FUNC(SIN_STR, SIN)
        FUNC(COS_STR, COS)
        FUNC(LN_STR, LN)
        FUNC(EXP_STR, EXP)
        #undef FUNC

        tree = ctor(NULL, NULL, VAR, NULL, NULL);
        strcpy(tree->oper, CUR_OPER);
        ASSERT_OK(tree);
        cnt_lex++;
    }

    return tree;
}
//!------------------------------------------
treeElem_t * getAdd()
{
    assert(Lexems.data);
    assert(cnt_lex <= Lexems.cnt);

    treeElem_t * tree = getTerm();

    while(CUR_TOKEN.Descriptor == ADD || CUR_TOKEN.Descriptor == SUB)
    {
        if (CUR_TOKEN.Descriptor == ADD)
        {
            cnt_lex++;
            tree = ctor(0, "+", ADD, tree, getTerm());
            ASSERT_OK(tree);
        }

        if (CUR_TOKEN.Descriptor == SUB)
        {
            cnt_lex++;
            tree = ctor(0, "-", SUB, tree, getTerm());
            ASSERT_OK(tree);
        }
    }

    return tree;
}
//!-------------------------------------------
treeElem_t * getTerm()
{
    assert(Lexems.data);
    assert(cnt_lex <= Lexems.cnt);

    treeElem_t * tree = getPeek();

    while(CUR_TOKEN.Descriptor == MUL || CUR_TOKEN.Descriptor == DIV)
    {
        switch(CUR_TOKEN.Descriptor)
        {
            case MUL:
                cnt_lex++;
                tree = ctor(0, "*", MUL, tree, getPeek());
                ASSERT_OK(tree);
                break;
            case DIV:
                cnt_lex++;
                tree = ctor(0, "/", DIV, tree, getPeek());
                ASSERT_OK(tree);
                break;
        }
    }

    return tree;
}
//!-------------------------------------------
treeElem_t * getPeek()
{
    assert(Lexems.data);
    assert(cnt_lex <= Lexems.cnt);

    if (CUR_SYMB == '(')
    {
        cnt_lex++;
        treeElem_t * tree = getAdd();

        if (CUR_SYMB == ')')
        {
            cnt_lex++;

            if (CUR_TOKEN.Descriptor == POW)
            {
                cnt_lex++;
                treeElem_t * thePow = getPeek();
                return ctor(0, 0, POW, tree, thePow);
            }

            return tree;
        }
        else
        {
            printf("ERROR! ')' is missing!\n");
            Lexems.warn = TRUE;
            return tree;
        }
    }
    else
    {
        if (CUR_TOKEN.Descriptor == VAR && Lexems.data[cnt_lex + 1].Descriptor == POW)
        {
            treeElem_t *theBase = getVar();
            cnt_lex++;
            treeElem_t *thePow = getPeek();
            return ctor(0, 0, POW, theBase, thePow);
        }
        else
        {
            if (CUR_TOKEN.Descriptor == VAR)
                return getVar();

            return getNum();
        }
    }
}
