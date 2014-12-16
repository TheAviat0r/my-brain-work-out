#include "header.h"

int main()
{
    setlocale(LC_ALL, "rus");
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
    FILE * input = input = formBuffer();
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

    FILE *dump = openFile("dump.txt");
    FILE *viewTree = openFile("tree.txt");
    FILE *viewDiff = openFile("Final.tex");

    if (Lexems.warn == FALSE)
    {
        optimizeTree(tree);
        dumpTree(tree, NODEBUG, BEGIN, viewTree);
        dumpTree(tree, DEBUG, BEGIN, dump);

        processMultDiff(tree, viewDiff);/*EMPT SLASHES
        char varD = 0;
        CountVars(tree);
        printf("Vars are counted! Amount = %u\n", G_Var.cnt);

        printIntro(tree, viewDiff);

        treeElem_t *D_arr[10] = {};
        for (int i = 0; i < G_Var.cnt; i++)
        {
            printDiff(tree, G_Var.data[i], viewDiff);
            D_arr[i] = diffFunc(tree, G_Var.data[i],viewDiff);
            optimizeTree(D_arr[i]);
            finishDiff(D_arr[i], G_Var.data[i], viewDiff);
        }*/

        //SLASHES printf("NON-OPTIMIZED!\n");
        //dumpTree(D_Func, NODEBUG, BEGIN, stdout);
        //SLASHES printf("OPTIMIZED!\n");
        //optimizeTree(D_Func);
        //finishTex(D_arr[0], viewDiff);
        //(D_Func, NODEBUG, BEGIN, stdout);
    }

    //system("commands.bat");
    FinishWork(input, dump, viewTree, lex_dump, tree, viewDiff);
}
//!------------------------------------------
void processMultDiff(treeElem_t *tree, FILE *output)
{
    assert(tree);
    assert(output);

    CountVars(tree);
    printf("Vars are counted! Amount = %u\n", G_Var.cnt);

    printIntro(tree, output);

    treeElem_t *D_arr[10] = {};
    for (int i = 0; i < G_Var.cnt; i++)
    {
        printDiff(tree, G_Var.data[i], output);
        D_arr[i] = diffFunc(tree, G_Var.data[i],output);
        optimizeTree(D_arr[i]);
        finishDiff(D_arr[i], G_Var.data[i], output);
    }

    finishTex(D_arr, output);
}
FILE * formBuffer()
{
    FILE *input = fopen("input_task.txt", "r");
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
        assert(i <= Lexems.cnt);
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
void FinishWork(FILE *input,  FILE *dump, FILE *viewTree,
                                            FILE * lex_dump, treeElem_t *root, FILE * viewDiff)
{
    assert(input);
    assert(dump);
    assert(lex_dump);
    assert(viewTree);
    assert(buffer);
    assert(viewDiff);
    assert(root);

    EMPT SLASHES printf("Work is finished!\n"); SLASHES
    fprintf(viewDiff, "\\end{document}\n");

    fclose(input);
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
void dumpTex(treeElem_t *root, treeElem_t *parent, FILE *output)
{
    assert(root);

    switch(root->type)
    {
        case SIN:
            fprintf(output, "\\sin (");
            break;
        case COS:
            fprintf(output, "\\cos (");
            break;
        case EXP:
            fprintf(output, "e^{");
            break;
        case LN:
            fprintf(output, "ln(");
            break;
        case DIV:
            fprintf(output, "\\dfrac{");
            break;
        case MUL:
        if (parent && (parent->type != ADD && parent->type != SUB && parent->type != MUL))
                if (LEFT->type == ADD || LEFT->type == SUB || RIGHT->type == ADD || RIGHT->type == SUB)
                {
                    printf("prefix mul!\n");
                    fprintf(output, "(");
                }

            break;
        case POW:
            if (LEFT->type == ADD || LEFT->type == SUB || LEFT->type == MUL || LEFT->type == DIV || LEFT->type == POW)
                fprintf(output, "(");
            break;
        case ADD:
            if (parent && (parent->type == MUL || parent->type == DIV))
            {
                fprintf(output, "(");

            printf("prefix add skob!\n");
            }
            break;
        case SUB:
            if (parent && (parent->type == MUL || parent->type == DIV))
                fprintf(output, "(");
            break;
    }


    //!------------------------------
    if (root->left)
        dumpTex(root->left, root, output);
    //!------------------------------

    switch(root->type)
    {
        case NUMBER:
            if (root->data > 0)
                fprintf(output, "%d", root->data);
            else
                fprintf(output, "(%d)", root->data);
            break;
        case VAR:
            fprintf(output, "%s", root->oper);
            break;
        case ADD:
            fprintf(output, "+");
            break;
        case SUB:
            fprintf(output, "-");
            break;
        case MUL:
            fprintf(output, " \\cdot ");
            break;
        case DIV:
            fprintf(output, "}{");
            break;
        case POW:
            if (LEFT->type == POW)
                fprintf(output, ")");
            fprintf(output, "^{");
            break;
    }

    if (root->right)
        dumpTex(root->right, root, output);

    if (root->type == SIN || root->type == COS || root->type == LN)
        fprintf(output, ")");

    if (root->type == POW || root->type == EXP || root->type == DIV)
        fprintf(output, "}");

    if (root->type == MUL && parent && (parent->type != ADD && parent->type != SUB && parent->type != MUL))
    {
        if (LEFT->type == ADD || LEFT->type == SUB || RIGHT->type == ADD || RIGHT->type == SUB)
        {
        fprintf(output, ")");

        printf("postfix mul!\n");
        printf("L: %d <%s> (%d)\n", LEFT->data, LEFT->oper, LEFT->type);
        printf("R: %d <%s> (%d)\n", RIGHT->data, RIGHT->oper, RIGHT->type);
        }
    }

    if ((root->type == ADD || root->type == SUB) && parent &&
            (parent->type == MUL || parent->type == DIV || (parent->type == POW && parent->left == root)))
    {
        printf ("postfix add skob!\n");
        fprintf(output, ")");
    }
}
//!------------------------------------------
void printIntro(treeElem_t *root_func, FILE *output)
{
    FILE *prefix = fopen("title.txt", "r");

    int len = filelength(fileno(prefix));
    unsigned char *buff = (unsigned char *) calloc(len + 1, sizeof(char));
    fread(buff, sizeof(char), len ,prefix);

    int i = 0;
    while(buff[i])
    {
        fprintf(output, "%c", buff[i]);
        i++;
    }
}
//!------------------------------------------
void printDiff(treeElem_t *root, char varD, FILE * output)
{
    fprintf(output, "\\section*{Производная следующей функции по %c:}\n", varD);
    fprintf(output, "\\begin{center}\n$f(");
    fprintf(output, "%c", G_Var.data[0]);

    for (int i = 1; i < G_Var.cnt; i++)
    {
        assert(1 <= i && i <= G_Var.cnt);
        fprintf(output, ",%c", G_Var.data[i]);
    }

    fprintf(output, ") = ");
    dumpTex(root, NULL,  output);
    fprintf(output, "$\n\\end{center}\n");

    fprintf(output, "На этом тривиальном примере наглядно покажем практическую реализацию правил дифференцирования\n");
    fprintf(output, "\\section*{Взятие производной по %c}\n", varD);
    fprintf(output, "Продифференцируем нашу функцию по %c. Будем пользоваться стандартными правилами.\n", varD);
}
//!------------------------------------------
void finishDiff(treeElem_t *root, char varD, FILE * output)
{
    assert(root);
    assert(varD);
    assert(output);

    fprintf(output, "\\section*{Результат по %c}\n", varD);
    fprintf(output, "В итоге имеем:\n");
    fprintf(output, "\\\\*$ f ^{\\prime}(%c) = ", varD);
    dumpTex(root, NULL, output);
    fprintf(output, "$");
}
//!------------------------------------------
void finishTex(treeElem_t *root[], FILE *output)
{
    assert(root);
    assert(output);
    fprintf(output, "\n\\section*{Финальная стадия - полная производная:}\n");
    fprintf(output, "Полная производная есть квадратный корень из суммы всех квадратов частных производных. Найдем ее\n");
    fprintf(output, "\\begin{center}\n$F^{\\prime}(");
    fprintf(output, "%c", G_Var.data[0]);

    for (int i = 1; i < G_Var.cnt; i++)
    {
        assert(1 <= i && i <= G_Var.cnt);
        fprintf(output, ",%c", G_Var.data[i]);
    }

    fprintf(output, ") = \\sqrt{");
    for (int i = 0; i < G_Var.cnt; i++)
    {
        assert(0 <= i && i <= G_Var.cnt);
        fprintf(output, "(");
        dumpTex(root[i], NULL, output);
        fprintf(output, ")^2");

        if (i != G_Var.cnt - 1)
            fprintf(output, "+");
    }
    fprintf(output, "}$\n\\end{center}\n");


    fprintf(output, "\n\\section*{Использованная литература}\n");
    fprintf(output, "\\textbf{1)} Больное воображение автора программы\n");
    fprintf(output, "\\\\* \\textbf{2)} Г.Е. Иванов - Математический анализ, ч.1\n");
}
//!------------------------------------------
void CountVars(treeElem_t *root)
{
    if (root->type == VAR)
    {
        int result = searchVar(root);
        if (result)
        {
            G_Var.data[G_Var.cnt] = root->oper[0];
            G_Var.cnt++;
        }
    }


    if (LEFT)
        CountVars(LEFT);

    if (RIGHT)
        CountVars(RIGHT);

}
//!------------------------------------------
int searchVar(treeElem_t *root)
{
    for (int i = 0; i <= G_Var.cnt; i++)
    {
        if (root->oper[0] == G_Var.data[i])
            return 0;
    }

    return TRUE;
}
//!------------------------------------------
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
            //foldDiv(root);
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
            //foldExp(root);
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
            /*treeElem_t *NewPow = ctor(0, 0, POW, COPY(LEFT), NULL);
            destroyTree(LEFT);
            *root = *NewPow;*/
            return;
        }

        if (LEFT->right->type == LN)
        {
            root->right = NULL;
            *(LEFT->right) = *(LEFT->right->left);
            *root = *(LEFT);
            root->type = POW;

            treeElem_t * save = NULL;
            save = root->left;
            root->left = root->right;
            root->right = save;
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
treeElem_t * diffFunc(treeElem_t * root, char varD, FILE* output)
{
    assert(root);

    switch(root->type)
    {
        case NUMBER:
            return ctor(0, 0, NUMBER, NULL, NULL);
            break;
        case VAR:
            if (root->oper[0] == varD)
                return ctor(1, 0, NUMBER, NULL, NULL);
            else
                return ctor(0, 0, NUMBER, NULL, NULL);

            break;
        case MUL:
        {
            switch(G_rules.mul)
            {
                case 0:
                    fprintf(output, "\\\\*Умножая свои добрые деяния, светлую сторону приближаешь ты\n");
                    G_rules.mul++;
                    break;
                case 1:
                    fprintf(output, "\\\\*Свет на стороне умножения. Ты в правильном направлении!\n");
                    G_rules.mul++;
                    break;
            }

            fprintf(output, "\\\\*$f(x) = ");
            dumpTex(root, NULL, output);
            fprintf(output, " \\Rightarrow ");

            fprintf(output, "f ^{\\prime}_%c(x) = ", varD);
            fprintf(output, "(");
            dumpTex(LEFT, NULL, output);
            fprintf(output, ")^{\\prime}_%c(x)\\cdot(", varD);
            dumpTex(RIGHT, NULL, output);
            fprintf(output, ") + (");
            dumpTex(LEFT, NULL, output);
            fprintf(output, ")\\cdot(");
            dumpTex(RIGHT, NULL, output);
            fprintf(output, ")^{\\prime}_%c)", varD);
            fprintf(output, "$\n");


            treeElem_t * newRoot = ctor(0, "+", ADD, NULL, NULL);
            treeElem_t * theLeft = ctor(0, "*", MUL, D(LEFT), COPY(RIGHT));
            treeElem_t * theRight = ctor(0, "*", MUL, COPY(LEFT), D(RIGHT));
            newRoot->left = theLeft;
            newRoot->right = theRight;

            return newRoot;
            break;
        }
        case ADD:
        {
            switch(G_rules.add)
            {
                case 0:
                    fprintf(output, "\\\\*Прибавляй и умножай, и голландцев побеждай\n");
                    G_rules.add++;
                    break;
                case 1:
                    fprintf(output, "\\\\*Производная суммы настолько сложна, насколько наполнены смыслом песни Стаса Михайлова\n");
                    G_rules.add++;
                    break;
            }

            fprintf(output, "\\\\*$f(x) = ");
            dumpTex(root, NULL, output);
            fprintf(output, " \\Rightarrow f ^{\\prime}_%c = ( ", varD);

            dumpTex(LEFT, NULL, output);
            fprintf(output, ")^{\\prime}_%c + (", varD);
            dumpTex(RIGHT, NULL, output);
            fprintf(output, ")^{\\prime}_%c$\n", varD);

            return ctor(0, "+", ADD, D(LEFT), D(RIGHT));
            break;
        }
        case SUB:
            switch(G_rules.sub)
            {
                case 0:
                    fprintf(output, "\\\\*Вычитание очевидно и тривиально\n");
                    G_rules.sub++;
                    break;
                case 1:
                    fprintf(output, "\\\\*Познай себя, убрав все лишнее\n");
                    G_rules.sub++;
                    break;
            }

            fprintf(output, "\\\\*$");
            dumpTex(root, NULL, output);
            fprintf(output, " \\Rightarrow f ^{\\prime}_%c = ( ", varD);

            dumpTex(LEFT, NULL, output);
            fprintf(output, ")^{\\prime}_%c - (", varD);
            dumpTex(RIGHT, NULL, output);
            fprintf(output, ")^{\\prime}_%c$\n", varD);
            return ctor(0, "-", SUB, D(LEFT), D(RIGHT));
            break;
        case POW:
        {
            if (RIGHT->type == NUMBER)
            {
                switch(G_rules.pow)
                {
                    case 0:
                        fprintf(output, "\\\\*Простепенись!\n");
                        G_rules.pow++;
                        break;
                    case 1:
                        fprintf(output, "\\\\*Степень - штука полезная, ее производная - тем более\n");
                        G_rules.pow++;
                        break;
                }

                fprintf(output, "$");
                dumpTex(root, NULL, output);
                fprintf(output, " \\Rightarrow f ^{\\prime}_%c = (", varD);
                dumpTex(root, NULL, output);
                fprintf(output, ")^{\\prime}_%c$\n", varD);

                root->right->data -= 1;
                treeElem_t * theRoot = ctor(0, 0, MUL, ctor(root->right->data + 1, 0, NUMBER, NULL, NULL) , COPY(root));
                root->right->data += 1;
                treeElem_t * newRoot = ctor(0, 0, MUL, theRoot, D(root->left));
                return newRoot;
                break;
            }
            else
            {
                switch(G_rules.exp)
                {
                    case 0:
                        fprintf(output, "\\\\*Экспоненциальность существования... Да, она такая, суровая и неожиданная\n");
                        G_rules.exp++;
                        break;
                    case 1:
                        fprintf(output, "\\\\*В любой непонятной ситуации бери экспоненту\n");
                        G_rules.exp++;
                        break;
                }

                fprintf(output, "\\\\*$");
                dumpTex(root, NULL, output);
                treeElem_t * theExp = ctor(0, 0, EXP, ctor(0, 0, MUL, COPY(RIGHT), ctor(0, 0, LN, COPY(LEFT), NULL)), NULL);
                SLASHES printf("exp is founded!\n");

                fprintf(output, " \\Rightarrow f^{\\prime}_%c = (", varD);
                dumpTex(theExp, NULL, output);
                fprintf(output, ")^{\\prime}_%c$\n", varD);

                dumpTree(theExp, NODEBUG, BEGIN, stdout);
                return D(theExp);
            }

            break;
        }
        case DIV:
        {
            switch (G_rules.div)
            {
                case 0:
                    fprintf(output, "\\\\*Разделяй и властвуй!\n");
                    G_rules.div++;
                    break;
                case 1:
                    fprintf(output, "\\\\*Покажи себя! Возьми производную частного!\n");
                    G_rules.div++;
                    break;
            }

            fprintf(output, "\\\\*$f(x) = ");
            dumpTex(root, NULL, output);
            fprintf(output, "\\Rightarrow f^{\\prime}(x) = \\dfrac{(");
            dumpTex(LEFT, NULL, output);
            fprintf(output, ")^{\\prime}_%c\\cdot (", varD);
            dumpTex(RIGHT, NULL, output);
            fprintf(output, ")-");
            dumpTex(LEFT, NULL, output);
            fprintf(output, "\\cdot (");
            dumpTex(RIGHT, NULL, output);
            fprintf(output, ")^{\\prime}_%c ", varD);
            fprintf(output, "}{(");
            dumpTex(RIGHT, NULL, output);
            fprintf(output, ")^2}$\n");

            treeElem_t *UpSide = ctor(0, 0, SUB, ctor(0, 0, MUL, D(LEFT), COPY(RIGHT)), ctor(0, 0, MUL, COPY(LEFT), D(RIGHT)));
            treeElem_t *DownSide = ctor(0, 0, POW, COPY(root->right), ctor(2, 0, NUMBER, NULL, NULL));
            treeElem_t *Result = ctor(0, 0, DIV, UpSide, DownSide);

            return Result;
            break;
        }
        case SIN:
        {
            switch(G_rules.sin)
            {
                case 0:
                    fprintf(output, "\\\\*На темную сторону силы поглядывать стал? Аккуратнее\n");
                    G_rules.sin++;
                    break;
                case 1:
                    fprintf(output, "\\\\*Темная сторона овладевает тобой, Люк! Борись с Темным Лордом!\n");
                    G_rules.sin++;
                    break;
            }

            fprintf(output, "$\\\\*f(x) = ");
            dumpTex(root, NULL, output);
            fprintf(output, "\\Rightarrow f^{\\prime}_%c = cos(", varD);
            dumpTex(LEFT, NULL, output);
            fprintf(output, " )\\cdot (");
            dumpTex(LEFT, NULL, output);
            fprintf(output, ")^{\\prime}$\n");
            treeElem_t *CosNode = ctor(0, 0, COS, COPY(LEFT), NULL);
            treeElem_t *Result = ctor(0, 0, MUL, CosNode, D(LEFT));
            return Result;
            break;
        }
        case COS:
        {
            switch(G_rules.cos)
            {
                case 0:
                    fprintf(output, "\\\\*В чем сила, брат? А сила в косинусе\n ");
                    fprintf(output, "$\\\\*cos^{\\prime}(...) = -sin(...)$\n");
                    G_rules.cos++;
                    break;
                case 1:
                    fprintf(output, "\\\\*Прими светлую сторону силы, о юный падаван\n");
                    fprintf(output, "$\\\\*cos^{\\prime}(...) = -sin(...)$\n");
                    G_rules.cos++;
                    break;
            }

            treeElem_t *SinNode = ctor(0, 0, SIN, COPY(LEFT), NULL);
            treeElem_t *ComplFunc = D(LEFT);
            treeElem_t *Result = ctor(0, 0, MUL, ctor(0, 0, MUL, SinNode, ComplFunc), ctor(-1, 0, NUMBER, NULL, NULL));
            return Result;
            break;
        }
        case LN:
        {
            switch(G_rules.ln)
            {
                case 0:
                    fprintf(output, "\\\\*Логарифмируй и побеждай!\n");
                    G_rules.ln++;
                    break;
                case 1:
                    fprintf(output, "\\\\*ЛОГАРИФМ МЫ БРАТЬ НЕ БРОСИМ, 1488!!!\n");
                    G_rules.ln++;
                    break;
            }

            fprintf(output, "\\\\*$ f(x) = ");
            dumpTex(root, NULL, output);
            fprintf(output, " \\Rightarrow f^{\\prime}_%c(x) = ", varD);
            dumpTex(ctor(0, 0, DIV, ctor(1, 0, NUMBER, NULL, NULL), LEFT), NULL, output);
            fprintf(output, " \\cdot (");
            dumpTex(LEFT, NULL, output);
            fprintf(output, ")^{\\prime}_%c$\n", varD);

            treeElem_t *LnDiff = ctor(0, 0, DIV, ctor(1, 0, NUMBER, NULL, NULL), COPY(LEFT));
            treeElem_t *ComplFunc = D(LEFT);
            treeElem_t *Result = ctor(0, 0, MUL, LnDiff, ComplFunc);
            return Result;
            break;
        }
        case EXP:
        {

            treeElem_t *ExpBase = COPY(root);
            //!-----------------------------
            fprintf(output, "\\\\*Экспонента $f(x) = ");
            dumpTex(ExpBase, NULL, output);
            fprintf(output, "\\Rightarrow f^{\\prime}_%c = ", varD);
            dumpTex(ExpBase, NULL, output);
            fprintf(output, "\\cdot (");
            dumpTex(LEFT, NULL, output);
            fprintf(output, ")^{\\prime}_%c$\n", varD);
            //!-----------------------------
            treeElem_t *PowDiff = D(LEFT);
            treeElem_t *Result = ctor(0, 0, MUL, ExpBase, PowDiff);
            return Result;
            break;
        }
    }

    return 0;
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

            if (Lexems.data[cnt_lex + 2].Descriptor == NUMBER)
            {
            int newNum = pow(Lexems.data[cnt_lex].data, Lexems.data[cnt_lex + 2].data);
            tree = ctor(newNum, "+", NUMBER, NULL, NULL);
            cnt_lex += 3;
            dumpTree(tree, NODEBUG, BEGIN, stdout);
            return tree;
            }

            else
            {
                cnt_lex++;
                cnt_lex++;
                treeElem_t *theBase = ctor(Lexems.data[cnt_lex-2].data, 0, NUMBER, NULL, NULL);
                treeElem_t *thePow = getPeek();
                treeElem_t *tree = ctor(0, 0, POW, theBase, thePow);
                dumpTree(tree, NODEBUG, BEGIN, stdout);
                return tree;
            }
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
