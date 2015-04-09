#include "main.h"

int main()
{
    try
    {
        CFileInfo       CompilerFiles("input.txt", "output.txt", "tree.txt");
        CTokenBuffer    TokenBuffer  (CompilerFiles.inputFile);

        std::string s("");
        CTreeNode * theRoot = parseCode(TokenBuffer);

        FunctionsTable.dump();

        CBinTree BinTree(*theRoot, CompilerFiles.treeFile);
        BinTree.print_tree();
    }

    catch (ICompilerError& UserMistake)
    {
        UserMistake.error_dump();
    }

    return 0;
}
