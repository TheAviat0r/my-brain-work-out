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

const unsigned char STD_ENTRY_ARR[]   = { 0x55,               //! push rbp
                                          0x48, 0x89,0xe5 };  //! mov rbp, rsp
const vector<unsigned char> STD_ENTRY(STD_ENTRY_ARR, STD_ENTRY_ARR + sizeof(STD_ENTRY_ARR)/sizeof(unsigned char));
const int STD_ENTRY_LEN           =  4;
const int SOURCE_ENTRY_LEN        =  1;

const unsigned char STD_LEAVE_ARR[]   = { 0x5d,             //! pop  rbp
                                          0xc3};            //!ret
const vector<unsigned char> STD_LEAVE(STD_LEAVE_ARR, STD_LEAVE_ARR + sizeof(STD_LEAVE_ARR)/sizeof(unsigned char));
const int STD_LEAVE_LEN           =  2;
const int SOURCE_LEAVE_LEN        =  1;

const unsigned char PUSH_BYTE     =   0x6a; //! push_byte

const int           PUSH_DWORD    =   0x68; //! push + value
const int           PUSHB_LEN     =   2;
const int           PUSHDW_LEN    =   5;
const int           OLD_PUSH_LEN  =   2;

const int           POP           =   0x58; //! from x58 to x5f - rax -> rdi
const int           POP_LEN       =   1;
const int           OLD_POP_LEN   =   2;

const int           PUSH_REG      =   0x50; //! push rax + reg code
const int           PUSH_REG_LEN  =   1;

const unsigned char ADD_ARR  []   = { 0x41, 0x5f,       //! pop  r15
                                      0x41, 0x5e,       //! pop  r14
                                      0x4d, 0x01, 0xfe, //! add  r14, r15
                                      0x41, 0x56 };     //! push r14
const vector<unsigned char> ADD (ADD_ARR, ADD_ARR + sizeof(ADD_ARR)/sizeof(unsigned char));
const int           ADD_LEN       = 9;
const int           OLD_ADD_LEN   = 1;

const unsigned char SUB_ARR[]     = { 0x41, 0x5f,       //! pop  r15
                                      0x41, 0x5e,       //! pop  r14
                                      0x4d, 0x29, 0xfe, //! sub  r14, r15
                                      0x41, 0x56 };     //! push r14
const vector<unsigned char> SUB (SUB_ARR, SUB_ARR + sizeof(SUB_ARR)/sizeof(unsigned char));
const int           SUB_LEN       = 9;
const int           OLD_MIN_LEN   = 1;

const unsigned char MUL_ARR[]     = { 0x41, 0x5f,       //! pop  r15
                                      0x41, 0x5e,       //! pop  r14
                                      0x4d, 0x0f, 0xaf, 0xf7, //! sub  r14, r15
                                      0x41, 0x56 };     //! push r14

const vector<unsigned char> MUL (MUL_ARR, MUL_ARR + sizeof(MUL_ARR)/sizeof(unsigned char));
const int           MUL_LEN       = 10;
const int           OLD_MUL_LEN   = 1;

const unsigned char DIV_ARR[]     = { 0x41, 0x5f,       //! pop  r15
                                      0x41, 0x5e,       //! pop  r14
                                      0x50,             //! push rax
                                      0x48, 0x31, 0xd2, //! xor  rdx, rdx
                                      0x4c, 0x89, 0xf0, //! mov  rax, r14
                                      0x49, 0xf7, 0xff, //! idiv r15
                                      0x49, 0x89, 0xc6, //! mov  r14, rax
                                      0x58,             //! pop  rax
                                      0x41, 0x56 };     //! push r14

const vector<unsigned char> DIV (DIV_ARR, DIV_ARR + sizeof(DIV_ARR)/sizeof(unsigned char));
const int           DIV_LEN       = 20;
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

const unsigned char JE_ARR[]      = { 0x41, 0x5f,       //! pop  r15
                                      0x41, 0x5e,       //! pop  r14
                                      0x4d, 0x39, 0xfe, //! cmp  r14, r15
                                      0x0f, 0x84 };     //! je code + relative adress (dword)
const vector<unsigned char> JE (JE_ARR, JE_ARR + sizeof(JE_ARR)/sizeof(unsigned char));

const unsigned char JNE_ARR[]     = { 0x41, 0x5f,       //! pop  r15
                                      0x41, 0x5e,       //! pop  r14
                                      0x4d, 0x39, 0xfe, //! cmp  r14, r15
                                      0x0f, 0x85 }; //! jne code + relative adress (dword)
const vector<unsigned char> JNE(JNE_ARR, JNE_ARR + sizeof(JNE_ARR)/sizeof(unsigned char));

const unsigned char JA_ARR[]      = { 0x41, 0x5f,       //! pop  r15
                                      0x41, 0x5e,       //! pop  r14
                                      0x4d, 0x39, 0xfe, //! cmp  r14, r15
                                      0x0f, 0x87 }; //!     jg code + relative adress(dword)
const vector<unsigned char> JA(JA_ARR, JA_ARR + sizeof(JA_ARR)/sizeof(unsigned char));
const int           OLD_JA_LEN    = 2;
const int           JA_LEN        = 13;

const unsigned char JAC_ARR[]     = { 0x41, 0x5f,       //! pop  r15
                                      0x41, 0x5e,       //! pop  r14
                                      0x4d, 0x39, 0xfe, //! cmp  r14, r15
                                      0x0f, 0x83 }; //! jge code + relative adress(dword)

const vector<unsigned char> JAC(JAC_ARR, JAC_ARR + sizeof(JAC_ARR)/sizeof(unsigned char));

const unsigned char JB_ARR[]      = { 0x41, 0x5f,       //! pop  r15
                                      0x41, 0x5e,       //! pop  r14
                                      0x4d, 0x39, 0xfe, //! cmp  r14, r15
                                      0x0f, 0x82 }; //! jl code + relative adress(dword)
const vector<unsigned char> JB(JB_ARR, JB_ARR + sizeof(JB_ARR)/sizeof(unsigned char));

const unsigned char JBC_ARR[]     = { 0x41, 0x5f,       //! pop  r15
                                      0x41, 0x5e,       //! pop  r14
                                      0x4d, 0x39, 0xfe, //! cmp  r14, r15
                                      0x0f, 0x86 }; //! jle code + relative adress(dword)
const vector<unsigned char> JBC(JBC_ARR, JBC_ARR + sizeof(JBC_ARR)/sizeof(unsigned char));

const unsigned char POPV_ARR_INDEX[]    = { 0x41, 0x5f,                   //! pop r15
                                            0x4c, 0x89, 0x3c, 0x25,        //! mov [var + index], r15 + adress
                                            0x0,  0x0,  0x0,  0x0 };    //! abs adress will be here
const vector<unsigned char> POPV_NUM(POPV_ARR_INDEX, POPV_ARR_INDEX + sizeof(POPV_ARR_INDEX)/sizeof(unsigned char));
const int POPV_NUM_LEN = 10;
const int OLD_POPV_LEN = 4;

const unsigned char PUSHV_NUM_INDEX_ARR[]   = { 0xff, 0x34, 0x25, 0x0, 0x0, 0x0, 0x0 }; //! last 4 bytes - adress)
const vector<unsigned char> PUSHV_NUM_INDEX (PUSHV_NUM_INDEX_ARR, PUSHV_NUM_INDEX_ARR + sizeof(PUSHV_NUM_INDEX_ARR));
const int SOURCE_PUSHV_LEN    = 4;
const int PUSHV_NUM_INDEX_LEN = 7;

const unsigned char PUSHV_VAR_INDEX_ARR[]  = { 0x4c, 0x8b, 0x34, 0x25, 0x0, 0x0, 0x0, 0x0, //! mov r14, [adress]
                                               0x41, 0xff, 0xb6, 0x0, 0x0, 0x0, 0x0 };
const vector<unsigned char> PUSHV_VAR_INDEX (PUSHV_VAR_INDEX_ARR, PUSHV_VAR_INDEX_ARR + sizeof(PUSHV_VAR_INDEX_ARR));
const int PUSHV_VAR_INDEX_LEN = 15;
