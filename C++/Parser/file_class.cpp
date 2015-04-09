#include <fstream>
#include <string>

struct CFileInfo
{
    private:
        CFileInfo();
    public:
        std::string     inputName;
        std::string     outputName;
        std::string     treeFileName;
        //!----------------------------------
        std::fstream    inputFile;
        std::fstream    outputFile;
        std::fstream    treeFile;
        std::fstream    dumpFile;
        //!----------------------------------
        explicit CFileInfo           (std::string input, std::string output, std::string tree);
        virtual ~CFileInfo  ();
        //!----------------------------------
};

CFileInfo::CFileInfo (std::string input, std::string output, std::string tree): inputName    (input),
                                                                                outputName   (output),
                                                                                treeFileName (tree),
                                                                                inputFile    (input.c_str(),  std::ios_base::in),
                                                                                outputFile   (output.c_str(), std::ios_base::out),
                                                                                treeFile     (tree.c_str(),   std::ios_base::out)
{
    if (!inputFile.is_open())
        throw CFileError("Unable to open INPUT File!", __LINE__);

    if (!outputFile.is_open())
        throw CFileError("Unable to open OUTPUT File!", __LINE__);

    if (!treeFile.is_open())
        throw CFileError("Unable to open TREE FILE!", __LINE__);
}

CFileInfo::~CFileInfo()
{
    inputFile.close();
    outputFile.close();
    treeFile.close();
}

