//!        TABLE OF TOKEN TYPES
enum TOKEN_TYPES
{
    NUMBER  = 1,
    ADD     = 2,
    SUB     = 3,
    MUL     = 4,
    DIV     = 5,
    MOD     = 6,
    EQUAL   = 7,
    ASSIGN  = 8,
    VAR     = 9,
    OPER    = 10,
    ENDOP   = 11,
    LSKOB   = 12,
    RSKOB   = 13,
    LFSKOB  = 14,
    RFSKOB  = 15,
    LESS    = 16,
    BIG     = 17,
    LESS_EQ = 18,
    BIG_EQ  = 19,
    N_EQUAL = 20,
    OP      = 21,
    NEWVAR  = 22,
    AND     = 23,
    OR      = 24,
    COND    = 25,
    IF      = 26,
    WHILE   = 27,
    VOID    = 28,
    INT     = 29,
    ARG     = 30,
    BOP     = 31,
    LINDEX  = 32,
    RINDEX  = 33,
    NUMINDEX = 34,
    VARINDEX = 35,
    CALL    = 36,
    ARRINIT = 37,
    RETURN  = 38,
    OUTPUT  = 39,
    INPUT   = 40
};
//!--------------------------------------------
struct TOKEN_TYPE_INFO
{
    std::vector<std::string>    names;
    std::vector<std::string>    operations;
    TOKEN_TYPE_INFO();

    friend std::string  searchTOKEN_INFO(unsigned int, int line, int MODE);
    friend unsigned int searchTOKEN_OP  (const std::string&, int line);
};
//!--------------------------------------------
TOKEN_TYPE_INFO::TOKEN_TYPE_INFO(): names(), operations()
{
    #define PUT_N(name, oper)     names.push_back(name); operations.push_back(oper);
    PUT_N("NUMBER", ""  )
    PUT_N("ADD"   , "+" )
    PUT_N("SUB"   , "-" )
    PUT_N("MUL"   , "*" )
    PUT_N("DIV"   , "/" )
    PUT_N("MOD"   , "$" )
    PUT_N("EQUAL" , "==")
    PUT_N("ASSIGN", "=" )
    PUT_N("VAR"   , ""  )
    PUT_N("OPER"  , ""  )
    PUT_N("ENDOP" , ";" )
    PUT_N("LSKOB" , "(" )
    PUT_N("RSKOB" , ")" )
    PUT_N("LFSKOB", "{" )
    PUT_N("RFSKOB", "}" )
    PUT_N("LESS"  , "<" )
    PUT_N("BIG"   , ">" )
    PUT_N("LESS_EQ","<=")
    PUT_N("BIG_EQ", ">=")
    PUT_N("N_EQUAL","=!")
    PUT_N("OP"    , "")
    PUT_N("NEWVAR", "")
    PUT_N("AND"   , "&")
    PUT_N("OR"    , "|")
    PUT_N("COND"  , "")
    PUT_N("IF"    , "IF")
    PUT_N("WHILE" , "WHILE")
    PUT_N("VOID"  , "VOID")
    PUT_N("INT"   , "INT")
    PUT_N("ARG"   , "ARG")
    PUT_N("BOP"   , "BOP")
    PUT_N("LINDEX", "[");
    PUT_N("RINDEX", "]");
    PUT_N("NUMINDEX","");
    PUT_N("VARINDEX","");
    PUT_N("CALL"  , "");
}
//!--------------------------------------------
TOKEN_TYPE_INFO GTOKEN_TYPE_INFO;

const int NAME = 1, OPERATION = 2;
//!--------------------------------------------
std::string  searchTOKEN_INFO(unsigned int Descriptor, int line, int MODE)
{
    if (1 <= Descriptor && Descriptor <= GTOKEN_TYPE_INFO.names.size())
    {
        if (MODE == NAME)
            return GTOKEN_TYPE_INFO.names[--Descriptor];
        if (MODE == OPERATION)
            return GTOKEN_TYPE_INFO.operations[--Descriptor];
    }
    else
        throw CLexicError("WRONG DESCRIPTOR!", line);

    return "OPER";
}
//!--------------------------------------------
unsigned int searchTOKEN_OP (const std::string& current_sym, int slashN)
{
    for (unsigned int i = 1; i <= GTOKEN_TYPE_INFO.operations.size(); i++)
    {
        if (GTOKEN_TYPE_INFO.operations[i-1] == current_sym)
            return i;
    }

    return OPER;
    throw CLexicError(current_sym, slashN);
}
//!--------------------------------------------

