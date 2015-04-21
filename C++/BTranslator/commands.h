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

const int STD_ENTRY[]             = { 0x55, 0x48, 0x89, //! mov  rbp, rsp
                                      0xe5 };           //! push rbp
const vector<unsigned char> ENTRY(STD_ENTRY, STD_ENTRY + sizeof(STD_ENTRY)/sizeof(unsigned char));

const int STD_LEAVE[]             = { 0xc9,//! leave
                                      0xc3};//!ret
const vector<unsigned char> LEAVE(STD_LEAVE, STD_LEAVE + sizeof(STD_LEAVE)/sizeof(unsigned char));

const unsigned char PUSH_BYTE     =   0x6a; //! push_byte

const int           PUSH_DWORD    =   0x68; //! push + value
const int           PUSH_LEN      =   2;
const int           OLD_PUSH_LEN  =   2;

const int           POP           =   0x58; //! from x58 to x5f - rax -> rdi
const int           POP_LEN       =   1;
const int           OLD_POP_LEN   =   2;

const int           PUSH_REG      =   0x50; //! push rax + reg code
const int           PUSH_REG_LEN  =   1;

const unsigned char ADD_ARR  []   = { 0x5b, //! pop rbx
                                      0x58, //! pop rax
                                      0x48, 0x01, 0xd8, //! add rax, rbx
                                      0x50 }; //! push rax
const vector<unsigned char> ADD (ADD_ARR, ADD_ARR + sizeof(ADD_ARR)/sizeof(unsigned char));
const int           ADD_LEN       = 6;
const int           OLD_ADD_LEN   = 1;

const unsigned char SUB_ARR[]     = { 0x5b,             //! pop  rbx
                                      0x58,             //! pop  rax
                                      0x48, 0x29, 0xd8, //! sub  rax, rbx
                                      0x50 };           //! push rax
const vector<unsigned char> SUB (SUB_ARR, SUB_ARR + sizeof(SUB_ARR)/sizeof(unsigned char));
const int           SUB_LEN       = 6;
const int           OLD_MIN_LEN   = 1;

const unsigned char MUL_ARR[]     = { 0x5b,             //! pop  rbx
                                      0x58, 0x48,       //! pop  rax
                                      0x0f, 0xaf, 0xc3, //! imul  rax, rbx
                                      0x50};            //! push rax

const vector<unsigned char> MUL (MUL_ARR, MUL_ARR + sizeof(MUL_ARR)/sizeof(unsigned char));
const int           MUL_LEN       = 7;
const int           OLD_MUL_LEN   = 1;

const unsigned char DIV_ARR[]     = { 0x5b,             //! pop  rbx
                                      0x58,             //! pop  rax
                                      0x48, 0x31, 0xd2, //! xor  rdx, rdx
                                      0x48, 0xf7, 0xfb, //! idiv rbx
                                      0x50};            //! push rax

const vector<unsigned char> DIV (DIV_ARR, DIV_ARR + sizeof(DIV_ARR)/sizeof(unsigned char));
const int           DIV_LEN       = 9;
const int           OLD_DIV_LEN   = 1;

const unsigned char RET           =   0xc3;
const int           RET_LEN       =   1;
const int           OLD_RET_LEN   =   1;

const unsigned char JMP_CODE      =   0xe9;         //! relative jump, adress - dword
const int           JMP_LEN       =   5;
const int           OLD_JMP_LEN   =   2;

const unsigned char CALL_CODE     =   0xe8;         //! relative call, adress - dword
const int           CALL_LEN      =   5;
const int           OLD_CALL_LEN  =   2;

const unsigned char JE_ARR[]      = { 0x5b,         //!     pop rbx
                                      0x58,         //!     pop rax
                                      0x48, 0x39, 0xd8, //!  cmp rax, rbx
                                      0x0f, 0x84 }; //! je code + relative adress (dword)
const vector<unsigned char> JE (JE_ARR, JE_ARR + sizeof(JE_ARR)/sizeof(unsigned char));

const unsigned char JNE_ARR[]     = { 0x5b,         //!     pop rbx
                                      0x58,         //!     pop rax
                                      0x48, 0x39, 0xd8, //!  cmp rax, rbx
                                      0x0f, 0x85 }; //! jne code + relative adress (dword)
const vector<unsigned char> JNE(JNE_ARR, JNE_ARR + sizeof(JNE_ARR)/sizeof(unsigned char));

const unsigned char JA_ARR[]      = { 0x5b,         //!     pop rbx
                                      0x58,         //!     pop rax
                                      0x48, 0x39, 0xd8, //!  cmp rax, rbx
                                      0x0f, 0x87 }; //!     jg code + relative adress(dword)
const vector<unsigned char> JA(JA_ARR, JA_ARR + sizeof(JA_ARR)/sizeof(unsigned char));
const int           OLD_JA_LEN    = 2;
const int           JA_LEN        = 11;

const unsigned char JAC_ARR[]     = { 0x5b,         //!     pop rbx
                                      0x58,         //!     pop rax
                                      0x48, 0x39, 0xd8, //!  cmp rax, rbx
                                      0x0f, 0x83 }; //! jge code + relative adress(dword)

const vector<unsigned char> JAC(JAC_ARR, JAC_ARR + sizeof(JAC_ARR)/sizeof(unsigned char));

const unsigned char JB_ARR[]      = { 0x5b,         //!     pop rbx
                                      0x58,         //!     pop rax
                                      0x48, 0x39, 0xd8, //!  cmp rax, rbx
                                      0x0f, 0x82 }; //! jl code + relative adress(dword)
const vector<unsigned char> JB(JB_ARR, JB_ARR + sizeof(JB_ARR)/sizeof(unsigned char));

const unsigned char JBC_ARR[]     = { 0x5b,         //!     pop rbx
                                      0x58,         //!     pop rax
                                      0x48, 0x39, 0xd8, //!  cmp rax, rbx
                                      0x0f, 0x86 }; //! jle code + relative adress(dword)
const vector<unsigned char> JBC(JBC_ARR, JBC_ARR + sizeof(JBC_ARR)/sizeof(unsigned char));


