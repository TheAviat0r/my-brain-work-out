#include "translator.h"

struct position_t
{
    int x86cnt;
    int b96cnt;
    position_t (int x86, int b96): x86cnt(x86), b96cnt(b96) {}
};

inline void push_cnts (vector<position_t>& positions, int& old_cnt, int& new_cnt, int old_inc, int new_inc);

struct jump_t
{
    size_t  from;
    int     old_to;
    bool    is_conditional;
    //!------------------------------------
    jump_t(int From, int Old_to, bool is_cond):
        from(From),
        old_to(Old_to),
        is_conditional(is_cond)
    {}
};

class CProgram
{
    private:
        CProgram(const CProgram&);
        CProgram& operator = (const CProgram&);
        //!------------------------------
        fstream&              input_;
        vector<unsigned char> code_;
        void *                exec_buf_;
        vector<position_t>    positions_;
        vector<jump_t>        jumps_;
        //!------------------------------
        size_t                buf_size_;
        int                   cmd_amount_;
        int                   opcode_cnt;
        //!------------------------------
        void        insertJumps();
    public:
        explicit CProgram(std::fstream& input);
        ~CProgram();
        //!------------------------------
        friend void alloc_writeable_page (size_t size);
        friend void emit_code            (void * buf, vector<unsigned char>& code_);
        friend void make_code_executable (void * buffer, size_t size);
        //!------------------------------
        void Translate      ();
        void dump_code      () const;
        void dump           () const;
        void jit_compile    ();
};

void binPopPushR  (vector<unsigned char>& code, vector<position_t>& positions, int reg, int mode);
void binPush (vector<unsigned char>& code, vector<position_t>& positions, int push_arg);
void binJump (vector<unsigned char>& code, vector<jump_t>& jumps,
                                           int old_to, bool is_conditional);
void insertJump();

CProgram::CProgram (std::fstream& input):
        input_(input), code_(), exec_buf_(NULL), positions_(), jumps_(), buf_size_(0), cmd_amount_(0),
        opcode_cnt(0) {}

CProgram::~CProgram()
{
    munmap(exec_buf_, code_.size());
}

void CProgram::dump_code() const
{
    cout << "-------------------" << endl;
    cout << "     CODE DUMP"      << endl;
    cout << "-------------------" << endl;

    for (size_t i = 0; i < positions_.size() - 1; i++)
    {
        cout << positions_[i].b96cnt << " | " << positions_[i].x86cnt << " | ";

        for (int j = positions_[i].x86cnt; j < positions_[i + 1].x86cnt; j++)
            printf("%x ", code_[j]);

        cout << endl;
    }

    cout << positions_.back().b96cnt << " | " << positions_.back().x86cnt << " | ";

    for (size_t j = positions_.back().x86cnt; j < code_.size(); j++)
        printf("%x ", code_[j]);

    cout << endl;

    cout << "-------------------" << endl;
    cout << "     JUMPS DUMP"     << endl;
    cout << "-------------------" << endl;
    for (size_t i = 0; i < jumps_.size(); i++)
    {
        printf("from - [0x%x] old_to = %d\n", jumps_[i].from, jumps_[i].old_to);
    }
}

void* alloc_writable_memory(size_t size)
{
    void* ptr = mmap(0, size,
                    PROT_READ | PROT_WRITE,
                    MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (ptr == (void*)-1)
    {
        perror("mmap");
        return NULL;
    }

    return ptr;
}

int make_memory_executable(void* m, size_t size)
{
    if (mprotect(m, size, PROT_READ | PROT_EXEC) == -1)
    {
        perror("mprotect");
        return -1;
    }

    return 0;
}

void emit_code(void * buf, vector<unsigned char>& code_)
{
    memcpy(buf, code_.data(), code_.size());
}

void CProgram::Translate()
{
    #define READ_CMD            input_.read((char *)&work, sizeof(char))
    #define READ_NUM_INT        input_.read((char *)&work_int, sizeof(int))
    #define GO_BACK             input_.seekp(input_.tellp() - 1);           //!after previewing
    #define INSERT_CODE(binary) insert(code_.end(), binary.begin(), binary.end())
    #define PUSH_CNT(fst, snd)  push_cnts(positions_, old_op_cnt, new_op_cnt, fst, snd)

    char work = 0;
    int  work_int = 0;
    int  old_op_cnt = 0, new_op_cnt = 0;

    input_.read((char *)&cmd_amount_, sizeof(int));

    cout << "command amount: " << cmd_amount_ << endl;

    for (int cmd_cnt = 0; cmd_cnt < cmd_amount_ ; cmd_cnt++)
    {
        READ_CMD;

        switch (work)
        {
            case CMD_PUSH:
                READ_NUM_INT;
                binPush(code_, positions_, work_int);
                PUSH_CNT(OLD_PUSH_LEN, PUSH_LEN);
                break;
            case CMD_PUSHR:
                READ_NUM_INT;
                binPopPushR(code_, positions_, work_int, 2);
                PUSH_CNT(OLD_PUSH_LEN, PUSH_REG_LEN);
                break;
            case CMD_POP:
                READ_NUM_INT;
                binPopPushR(code_, positions_, work_int, 1);
                PUSH_CNT(OLD_POP_LEN, POP_LEN);
                break;
            case CMD_ADD:
                code_.INSERT_CODE(ADD);
                PUSH_CNT(OLD_ADD_LEN, ADD_LEN);
                break;
            case CMD_MIN: //! SUB
                code_.INSERT_CODE(SUB);
                PUSH_CNT(OLD_MIN_LEN, SUB_LEN);
                break;
            case CMD_MUL:
                code_.INSERT_CODE(MUL);
                PUSH_CNT(OLD_MUL_LEN, MUL_LEN);
                break;
            case CMD_DIV:
                code_.INSERT_CODE(DIV);
                PUSH_CNT(OLD_DIV_LEN, DIV_LEN);
                break;
            case CMD_JMP:
                code_.push_back(JMP_CODE);
                READ_NUM_INT;
                binJump(code_, jumps_, work_int, false);
                PUSH_CNT(OLD_JMP_LEN, JMP_LEN);
                break;
            case CMD_JA:
                code_.INSERT_CODE(JA);
                READ_NUM_INT;
                PUSH_CNT(OLD_JA_LEN, JA_LEN);
                binJump(code_, jumps_, work_int, true);
                break;
            case CMD_JAC:
                code_.INSERT_CODE(JAC);
                READ_NUM_INT;
                PUSH_CNT(OLD_JA_LEN, JA_LEN);
                binJump(code_, jumps_, work_int, true);
                break;
            case CMD_JB:
                code_.INSERT_CODE(JB);
                READ_NUM_INT;
                PUSH_CNT(OLD_JA_LEN, JA_LEN);
                binJump(code_, jumps_, work_int, true);
                break;
            case CMD_JBC:
                code_.INSERT_CODE(JBC);
                READ_NUM_INT;
                PUSH_CNT(OLD_JA_LEN, JA_LEN);
                binJump(code_, jumps_, work_int, true);
                break;
            case CMD_JE:
                code_.INSERT_CODE(JE);
                READ_NUM_INT;
                PUSH_CNT(OLD_JA_LEN, JA_LEN);
                binJump(code_, jumps_, work_int, true);
                break;
            case CMD_JNE:
                code_.INSERT_CODE(JNE);
                READ_NUM_INT;
                PUSH_CNT(OLD_JA_LEN, JA_LEN);
                binJump(code_, jumps_, work_int, true);
                break;
            case CMD_CALL:
                code_.push_back(CALL_CODE);
                READ_NUM_INT;
                binJump(code_, jumps_, work_int, false);
                PUSH_CNT(OLD_CALL_LEN, CALL_LEN);
                break;
            case CMD_RET:
                code_.push_back(RET);
                PUSH_CNT(OLD_RET_LEN, RET_LEN);
                break;
            default:
                printf("unknown command - %d\n", work);
                cout << "cmd_cnt = " << cmd_cnt << " cmd_amount = " << cmd_amount_ << endl;
                throw std::invalid_argument("wrong command.");
                break;
        }
    }

    code_.push_back(RET);
    PUSH_CNT(1,1);

    insertJumps();
    dump_code();
}

inline void push_cnts (vector<position_t>& positions, int& old_cnt, int& new_cnt, int old_inc, int new_inc)
{
    positions.push_back(position_t(new_cnt, old_cnt));
    old_cnt += old_inc;
    new_cnt += new_inc;
}

void binPush (vector<unsigned char>& code, vector<position_t>& positions, int push_arg)
{
    if (-127 < push_arg && push_arg < 127)
    {
        code.push_back(PUSH_BYTE);
        code.push_back(push_arg);
    }
    else
    {
        code.push_back(PUSH_DWORD);
        vector<unsigned char>   reversed_num;
        dword_rev(reversed_num, push_arg);
        code.insert(code.end(), reversed_num.begin(), reversed_num.end());
    }
}

void binPopPushR (vector<unsigned char>& code, vector<position_t>& positions, int reg, int mode)
{
    assert(reg == ax_code || reg == bx_code || reg == cx_code || reg == dx_code);

    switch(mode)
    {
        case 1:
            switch(reg)
            {
                case ax_code:
                    code.push_back(POP + AX);
                    break;
                case cx_code:
                    code.push_back(POP + CX);
                    break;
                case dx_code:
                    code.push_back(POP + DX);
                    break;
                case bx_code:
                    code.push_back(POP + BX);
                    break;
            }
            break;
        case 2:
            switch(reg)
            {
                case ax_code:
                    code.push_back(PUSH_REG + AX);
                    break;
                case cx_code:
                    code.push_back(PUSH_REG + CX);
                    break;
                case dx_code:
                    code.push_back(PUSH_REG + DX);
                    break;
                case bx_code:
                    code.push_back(PUSH_REG + BX);
                    break;
            }
            break;
    }

}

void binJump(vector<unsigned char>& code, vector<jump_t>& jumps,
                                          int old_to, bool is_conditional)
{
    if (!is_conditional)
        jumps.push_back(jump_t(code.size() - 1, old_to, false));
    else
    {
        jumps.push_back(jump_t(code.size() - 7, old_to, true));
    }

    for (int i = 0; i < 4; i++)
        code.push_back(0);
}

void CProgram::insertJumps()
{
    int value = 0;

    for (size_t i = 0; i < jumps_.size(); i++)
    {
        for (size_t j = 0; j < positions_.size(); j++)
        {
            if (positions_[j].b96cnt == jumps_[i].old_to)
                if (!jumps_[i].is_conditional)
                    value = positions_[j].x86cnt - jumps_[i].from - 5;
                else
                    value = positions_[j].x86cnt - jumps_[i].from - 11;
        }

        vector<unsigned char> byte_number;
        dword_rev(byte_number, value);

        for (size_t cnt = 0; cnt < 4; cnt++)
        {
            if (!jumps_[i].is_conditional)
                code_[jumps_[i].from + cnt + 1] = byte_number[cnt];
            else
            {
                code_[jumps_[i].from + cnt + 7] = byte_number[cnt];
            }
        }
    }
}

void CProgram::jit_compile()
{
    exec_buf_ = alloc_writable_memory(code_.size() + 1);    //! emit_code will add ret instr into buf
    emit_code(exec_buf_, code_);
    make_memory_executable(exec_buf_, code_.size());

    int (*func)() = (int (*)())exec_buf_;
    printf("\nresult = %d\n", func());
}

int main()
{
    fstream input("source.bin");
    if (!input.is_open())
        throw std::invalid_argument("Unable to open source!");

    CProgram Program(input);
    Program.Translate();
    //Program.dump_code();
    Program.jit_compile();
    //Program.dump_code();

    return 0;
}
