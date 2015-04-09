//!             ANALIZER SECTION
typedef int lex_data_t;         //! I am too lazy to write whole template only for number field, because number
                                //! will be int or double  and nothing else
//!--------------------------------------------
class token_t
{
    public:
        int          Descriptor;
        std::string  Oper;
        lex_data_t   data;
        int          line;
        //!------------------------------------
        token_t(int token_descr, const std::string& token_oper, lex_data_t token_data, int line);
        token_t(int token_descr): Descriptor(token_descr), Oper(""), data(0), line(0) {}
        token_t(int token_descr, const std::string& token_oper): Descriptor(token_descr), Oper(token_oper), data(0), line(0){}
        virtual ~token_t() {}
        //!------------------------------------
};
//!--------------------------------------------
token_t::token_t(int token_descr, const std::string& token_oper, lex_data_t token_data, int token_line):
                                                Descriptor(token_descr), Oper(token_oper), data(token_data),
                                                line(token_line)
{
}

class CLexAnalizer
{
    private:
        std::fstream&         sourceFile_;
        std::string           sourceBuf_;
        //!------------------------------------
        void            analyze     ();
        void            dump_tokens () const;
        lex_data_t      findNumber  (std::string::const_iterator& current_sym);
        unsigned int    getType     (std::string::const_iterator& current_sym, int slashN) const;
        void            findVariable(std::string::const_iterator& current_sym, std::string& stroke);
        //!------------------------------------
    protected:
        std::vector<token_t>  Lexems_;
        unsigned int          cur_token_;
        //!------------------------------------
    public:
        explicit        CLexAnalizer(std::fstream& input);
        virtual        ~CLexAnalizer(){}
};
//!--------------------------------------------
class ICTokenBuffer
{
    public:
        virtual ~ICTokenBuffer() {}
        virtual const token_t&  current     () const = 0;
        virtual int             current_type() const = 0;
        virtual const token_t&  next        () const = 0;
        virtual void            go_next     ()       = 0;
        virtual void            go_prev     ()       = 0;
        virtual std::size_t     size        () const = 0;
};
//!--------------------------------------------
class CTokenBuffer: private CLexAnalizer, public ICTokenBuffer
{
    public:
        explicit        CTokenBuffer(std::fstream& input):  CLexAnalizer(input) {}
        //!------------------------------------
        const token_t&  current     () const;
        int             current_type() const;
        const token_t&  next        () const;
        void            go_next     ()      ;
        void            go_prev     ()      ;
        std::size_t     size        () const;
        int             cnt         () const {      return cur_token_; }
};


    //!     LEX-ANALIZER REALISATION
//!---------------------------------------------
CLexAnalizer::CLexAnalizer(std::fstream& input):    sourceFile_(input), sourceBuf_(""), Lexems_(), cur_token_(0)
{
    if (!sourceFile_.is_open())
        throw CFileError("ERROR! Unable to open sourceFile_!", __LINE__);

    std::copy(std::istreambuf_iterator<char>(sourceFile_), std::istreambuf_iterator<char>(), back_inserter(sourceBuf_));

    analyze();
    dump_tokens();
}
//!--------------------------------------------
void CLexAnalizer::analyze()
{
    unsigned int slashN = 1;
    std::string::const_iterator current_sym = sourceBuf_.begin();

    while (current_sym < --sourceBuf_.end())
    {
        while (isspace(*current_sym) || *current_sym == '\n')
        {
            if (*current_sym == '\n')
                slashN++;

            ++current_sym;
        }

        std::string stroke("");

        if (isdigit(*current_sym))
            Lexems_.push_back(token_t(NUMBER, stroke, findNumber(current_sym), slashN)); //!    findNum
                                                                        //! increases current_sym
        else
        {
            unsigned int token_type = getType(current_sym, slashN);

            if (token_type == VAR)
                findVariable(current_sym, stroke);      //! findVariable increases current_sym
            else
            {
                stroke.push_back(*current_sym);
                current_sym++;
            }

            Lexems_.push_back(token_t(token_type, stroke, 0, slashN));
        }
    }
}
//!--------------------------------------------
lex_data_t  CLexAnalizer::findNumber(std::string::const_iterator& current_sym)
{
    if (!sourceFile_.is_open())
        throw CFileError("ERROR! sourceBuf_ is not opened!", __LINE__);

    lex_data_t  resultNumber = 0;

    while(isdigit(*current_sym))
    {
        resultNumber = resultNumber * 10 + *current_sym - '0';
        if (isdigit(*current_sym + 1))
            current_sym++;
    }

    return resultNumber;
}
//!--------------------------------------------
void CLexAnalizer::findVariable(std::string::const_iterator& current_sym, std::string& stroke)
{
    while (isalnum(*current_sym) || *current_sym == '_')
    {
        stroke.push_back(*current_sym);
        current_sym++;
    }
}
//!--------------------------------------------
unsigned int CLexAnalizer::getType(std::string::const_iterator& current_sym, int slashN)   const
{
    if (isalpha(*current_sym))
        return VAR;

    std::string oper_stroke("");
    oper_stroke.push_back(*current_sym);

    if (*(current_sym + 1) == '=' || *(current_sym + 1) == '!')
    {
        ++current_sym;
        oper_stroke.push_back(*current_sym);
    }

    return searchTOKEN_OP(oper_stroke, slashN);
}
//!--------------------------------------------
void CLexAnalizer::dump_tokens()    const
{
    std::fstream output("TOKENS.txt", std::ios_base::out);
    if (!output.is_open())
        throw CFileError("Unable to open tokens.txt!", __LINE__);

    FSLASHES output << "\tTOKEN DUMP\n"; FSLASHES

    for (std::vector<token_t>::const_iterator it = Lexems_.begin(); it < Lexems_.end(); it++)
    {
        output << searchTOKEN_INFO(it->Descriptor, it->line, NAME);

        switch(it->Descriptor)
        {
            case VAR:
            case OPER:
                output << "  <" << it->Oper << ">";
                break;
            case NUMBER:
                output << "  (" << it->data << ")";
                break;
        }

        output << "\nline: " << it->line << "\n";
        FSHORTSLASH
    }

    output.close();
}
//!--------------------------------------------


    //!     TOKEN-ARRAY INTERFACE
//!--------------------------------------------
const token_t&  CTokenBuffer::current() const
{
    assert(cur_token_ <= Lexems_.size());

    return Lexems_[cur_token_];
}

int  CTokenBuffer::current_type() const
{
    assert(cur_token_ <= Lexems_.size());

    return Lexems_[cur_token_].Descriptor;
}


const token_t&  CTokenBuffer::next()    const
{
    if (cur_token_ > Lexems_.size())
        std::cout << "OUT OF RANGE!\n";

    return Lexems_[cur_token_ + 1];
}

void CTokenBuffer::go_next()
{
    if (cur_token_ < Lexems_.size())
        ++cur_token_;
}

void CTokenBuffer::go_prev()
{
    --cur_token_;
}

std::size_t CTokenBuffer::size()   const
{
    return Lexems_.size();
}
