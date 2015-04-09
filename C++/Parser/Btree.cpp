
void fprintfTAB (std::ostream& output, int theDepth);
//!----------------------------------------
class CTreeNode
{
    private:
        token_t    token_;
        CTreeNode* left_ ;
        CTreeNode* right_;
        int        type_;
    public:
        int        depth;
        //!--------------------------------
        CTreeNode  (token_t theToken , int theType):  token_(theToken), left_(0), right_(0), type_(theType), depth(0) {}
        CTreeNode  (token_t& theToken, int theType):  token_(theToken), left_(0), right_(0), type_(theType), depth(0) {}
        CTreeNode  (token_t theToken, int theType, CTreeNode* theLeft, CTreeNode* theRight):
                                                   token_(theToken), left_(theLeft), right_(theRight), type_(theType), depth(0) {}
        CTreeNode  (token_t& theToken, int theType, CTreeNode* theLeft, CTreeNode* theRight):
                                                   token_(theToken), left_(theLeft), right_(theRight), type_(theType), depth(0) {}
    public:
        //!--------------------------------
        friend      std::ostream& operator << (std::ostream& output, const CTreeNode& Node);
        //!--------------------------------
        int          type  () const;
        std::string& stroke() const;
        lex_data_t   data  () const;
        CTreeNode*   left  () const;
        CTreeNode*   right () const;
        void         set_type (int theType)     {    type_ = theType; }
        void         set_right(CTreeNode* node) {    right_ = node; }
        void         set_left (CTreeNode* node) {    left_ = node; }
};

class CBinTree
{
    private:
        std::fstream& output;
    public:
        CTreeNode  root_;
        //!--------------------------------
        void print_tree     (CTreeNode* root, int depth);
        void destroy_tree   (CTreeNode* theRoot);
        //!--------------------------------
    public:
        CBinTree(CTreeNode& Node, std::fstream& treeFile): output(treeFile), root_(Node) {}
        ~CBinTree();
        void       print_tree ();
        void       dump_tree(CTreeNode* theRoot);
        CTreeNode& root       () const;
};
//!----------------------------------------
void fprintfTAB (std::ostream& output, int theDepth)
{
    for (int i = 0; i < theDepth; i++)
        output << "        ";
}
//!----------------------------------------
void CBinTree::dump_tree(CTreeNode* theRoot)
{
    std::cout << "root:\n" << theRoot << "\n";
    std::cout << "left:\n" << theRoot->left() << "\n";
    std::cout << "right:\n" << theRoot->right() << "\n\n";

    if (theRoot->left())
        dump_tree(theRoot->left());
    if (theRoot->right())
        dump_tree(theRoot->right());
}

std::ostream& operator << (std::ostream& output, const CTreeNode& Node)
{
    switch(Node.type())
    {
        #define FTAB fprintfTAB(output, Node.depth);
        #define PRINT_OP(type, oper)  case type: FTAB output << oper << "\n";     break;
        case NUMBER:
            FTAB output << Node.token_.data << "\n";
            break;
        case VAR:
            FTAB output << Node.token_.Oper << "\n";
            break;
        case NEWVAR:
            FTAB output << "\n";
            FTAB output << "NEWVAR\n";
            FTAB output << Node.token_.Oper << "\n";
            break;
        case VOID:
            FTAB output << "\n";
            FTAB output << "VOID\n";
            FTAB output << Node.token_.Oper << "\n";
            break;
        case INT:
            FTAB output << "INT\n";
            FTAB output << Node.token_.Oper << "\n";
            break;
        case NUMINDEX:
            FTAB output << "NUMINDEX:\n";
            FTAB output << Node.token_.data << "\n";
            break;
        case VARINDEX:
            FTAB output << "\n";
            FTAB output << "VARINDEX:\n";
            FTAB output << Node.token_.Oper << "\n";
            break;
        case ARRINIT:
            FTAB output << "ARRINIT:\n";
            FTAB output << Node.token_.Oper << "\n";
            break;
        PRINT_OP(ADD,       "+"      );
        PRINT_OP(SUB,       "-"      );
        PRINT_OP(MUL,       "*"      );
        PRINT_OP(DIV,       "/"      );
        PRINT_OP(MOD,       "MOD"    );
        PRINT_OP(ASSIGN,    "="      );
        PRINT_OP(LESS,      "<"      );
        PRINT_OP(LESS_EQ,   "<="     );
        PRINT_OP(BIG,       ">"      );
        PRINT_OP(BIG_EQ,    ">="     );
        PRINT_OP(EQUAL,     "=="     );
        PRINT_OP(N_EQUAL,   "=!"     );
        PRINT_OP(OP,        "OP"     );
        PRINT_OP(AND,       "AND"    );
        PRINT_OP(OR,        "OR"     );
        PRINT_OP(COND,      "COND:"  );
        PRINT_OP(IF,        "IF"     );
        PRINT_OP(WHILE,     "WHILE"  );
        PRINT_OP(ARG,       "ARG"    );
        PRINT_OP(BOP,       "BOP"    );
        PRINT_OP(CALL,      "CALL:"  );
        PRINT_OP(RETURN,    "RETURN:");
        PRINT_OP(OUTPUT,    "OUTPUT:");
        PRINT_OP(INPUT,     "INPUT:" );
        #undef  FTAB
    }

    return output;
}
//!----------------------------------------
#define PREPARE_LR  assert(&leftNode); assert(&rightNode);\
                    std::string s("");\
                    CTreeNode& LEFT  = const_cast<CTreeNode&> (leftNode);\
                    CTreeNode& RIGHT = const_cast<CTreeNode&> (rightNode);

//!----------------------------------------
int CTreeNode::type() const
{
    return type_;
}
//!----------------------------------------
std::string& CTreeNode::stroke() const
{
    return const_cast<std::string&>(token_.Oper);
}
//!----------------------------------------
lex_data_t CTreeNode::data() const
{
    return token_.data;
}
//!----------------------------------------
CTreeNode* CTreeNode::left() const
{
    return left_;
}
//!----------------------------------------
CTreeNode* CTreeNode::right() const
{
    return right_;
}
//!----------------------------------------
void CBinTree::print_tree()
{
    output << root_;

    if (root_.left())
        print_tree(root_.left(),  1);
    if (root_.right())
        print_tree(root_.right(), 1);

};

CBinTree::~CBinTree()
{
    if (root_.left())
        destroy_tree(root_.left());
    if (root_.right())
        destroy_tree(root_.right());
}

void CBinTree::destroy_tree(CTreeNode* theRoot)
{
    if (theRoot->left())
        destroy_tree(theRoot->left());
    if (theRoot->right())
        destroy_tree(theRoot->right());

    delete theRoot;
}

//!----------------------------------------
void CBinTree::print_tree(CTreeNode* theRoot, int nodeDepth)
{
    theRoot->depth = nodeDepth;
    output   << *theRoot;

    if (theRoot->left())
        print_tree(theRoot->left(),  theRoot->depth + 1);
    if (theRoot->right())
        print_tree(theRoot->right(), theRoot->depth + 1);
}
//!----------------------------------------


