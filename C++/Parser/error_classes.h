class ICompilerError
{
    public:
        virtual ~ICompilerError() {}
        virtual void error_dump() = 0;
};
//!------------------------------------------
class CDefaultLog
{
    public:
        std::string errorText;
        int         errorLine;
        CDefaultLog(std::string message, int line): errorText(message), errorLine(line) {}
        virtual    ~CDefaultLog() {}
};
//!------------------------------------------
class CSyntaxError:  public ICompilerError, public CDefaultLog
{
    private:
        CSyntaxError();
    public:
        std::string variable;
        CSyntaxError(std::string message, int line): CDefaultLog(message, line), variable("") {}
        CSyntaxError(std::string message, int line, std::string var):
                                                      CDefaultLog(message, line), variable(var) {}
        CSyntaxError(std::string message, std::string var):
                                                      CDefaultLog(message, 0), variable(var) {}
        void error_dump();
};
//!------------------------------------------
class CLSkobMissing: public CSyntaxError
{
    public:
        CLSkobMissing(int line): CSyntaxError("( is missing!", line) {}
};
//!------------------------------------------
class CRSkobMissing: public CSyntaxError
{
    public:
        CRSkobMissing(int line): CSyntaxError(") is missing!", line) {}
};
//!------------------------------------------
class CDotMissing:   public CSyntaxError
{
    public:
        CDotMissing(int line): CSyntaxError("; is missing!", line) {}
};
//!------------------------------------------
class CLexicError:  public ICompilerError, public CDefaultLog
{
    CLexicError();
    public:
        unsigned int Descriptor;
        //!----------------------------------
        explicit CLexicError(std::string message, int line): CDefaultLog(message, line),
                                                             Descriptor(0){}
        explicit CLexicError(std::string message, int line, unsigned int wrongDescr):
                                                             CDefaultLog(message, line),
                                                             Descriptor(wrongDescr)
                                                             {}
        //!----------------------------------
        void         error_dump();
};
//!------------------------------------------
void CLexicError::error_dump()
{
    std::cout << "LEXIC ERROR: \n";
    std::cout << "LOG: "  << errorText << "\n";
    std::cout << "LINE: " << errorLine << "\n";
    std::cout << "DESCRIPTOR " << Descriptor << "\n";
}
//!------------------------------------------
class CFileError:   public ICompilerError, public CDefaultLog
{
    private:
        CFileError();
    public:
        //std::string     error_text;
        //unsigned int    error_stroke;
        //!----------------------------------
        CFileError      (std::string Message, unsigned int Line);
        //!----------------------------------
        void            error_dump();
};
//!------------------------------------------
void CFileError::error_dump()
{
    std::cout << "FILE ERROR:\n";
    std::cout << "LOG "   << errorText << "\n";
    std::cout << "LINE: " << errorLine << "\n";
}
//!------------------------------------------
CFileError::CFileError(std::string Message, unsigned int Line):     CDefaultLog(Message, Line)
{}
//!------------------------------------------

void CSyntaxError::error_dump()
{
    std::cout << "SYNTAX ERROR:\n";
    std::cout << "LOG: " << errorText << "\n";
    if (errorLine)
        std::cout << "LINE:" << errorLine << "\n";

    if (variable.size())
        std::cout << "VAR: " << variable << "\n";
}
