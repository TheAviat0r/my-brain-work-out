enum REGS
{
    AX,
    CX,
    DX,
    BX,
    SP,
    BP,
    SI,
    DI
};

const int STD_ENTRY[]             = { 0x55, 0x48, 0x89, 0xe5 };
const vector<unsigned char> ENTRY(STD_ENTRY, STD_ENTRY + sizeof(STD_ENTRY)/sizeof(unsigned char));

const int STD_LEAVE[]             = { 0xc9, 0xc3 };
const vector<unsigned char> LEAVE(STD_LEAVE, STD_LEAVE + sizeof(STD_LEAVE)/sizeof(unsigned char));

const unsigned char PUSH_BYTE     =  0x6a;

const int           PUSH_DWORD    =  0x68;

const int           POP           =  0x58; //! from x58 to x5f - rax -> rdi

const unsigned char ADD_ARR  []   = { 0x5b, 0x58, 0x48, 0x01, 0xd8, 0x50 };
const vector<unsigned char> ADD (ADD_ARR, ADD_ARR + sizeof(ADD_ARR)/sizeof(unsigned char));

const unsigned char SUB_ARR[]     = { 0x5b, 0x58, 0x48, 0x29, 0xd8, 0x50 };
const vector<unsigned char> SUB (SUB_ARR, SUB_ARR + sizeof(SUB_ARR)/sizeof(unsigned char));

const unsigned char MUL_ARR[]     = { 0x5b, 0x58, 0x48, 0x0f, 0xaf, 0xc3, 0x50};
const vector<unsigned char> MUL (MUL_ARR, MUL_ARR + sizeof(MUL_ARR)/sizeof(unsigned char));

const unsigned char DIV_ARR[]     = { 0x5b, 0x58, 0x48, 0x31, 0xd2, 0x48, 0xf7, 0xfb, 0x50};
const vector<unsigned char> DIV (DIV_ARR, DIV_ARR + sizeof(DIV_ARR)/sizeof(unsigned char));

const int           RET          =    0xc3;



