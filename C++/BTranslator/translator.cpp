#include "translator.h"

struct position_t
{
    int x86cnt;
    int b96cnt;
    position_t (int first, int second): x86cnt(first), b96cnt(second) {}
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
        //!------------------------------
        size_t                buf_size_;
        int                   cmd_amount_;
        int                   opcode_cnt;
        //!------------------------------
        friend void alloc_writeable_page (size_t size);
        friend void emit_code            (void * buf, vector<unsigned char>& code_);
        friend void make_code_executable (void * buffer, size_t size);
    public:
        explicit CProgram(std::fstream& input);
        //!------------------------------
        void Translate      ();
        void dump_code      () const;
        void dump           () const;
        void jit_compile    ();
};

void binPop  (vector<unsigned char>& code, vector<position_t> positions, int reg);
void binPush (vector<unsigned char>& code, vector<position_t> positions, int push_arg);

CProgram::CProgram (std::fstream& input):
        input_(input), code_(), exec_buf_(NULL), positions_(), buf_size_(0), cmd_amount_(0),
        opcode_cnt(0) {}


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
    //code_.push_back(0x58);       //! pop rax
    code_.push_back(RET);        //! ret

    memcpy(buf, code_.data(), code_.size());

    for (int i = 0; i < code_.size(); i++)
    {
        printf("%x ", *(unsigned char *)(buf + i));
    }
}

void CProgram::Translate()
{
    #define READ_CMD            input_.read((char *)&work, sizeof(char))
    #define READ_NUM_INT        input_.read((char *)&work_int, sizeof(int))
    #define GO_BACK             input_.seekp(input_.tellp() - 1);           //!after previewing
    #define INSERT_CODE(binary) insert(code_.end(), binary.begin(), binary.end())

    char work = 0;
    int  work_int = 0;

    input_.read((char *)&cmd_amount_, sizeof(int));

    cout << "command amount: " << cmd_amount_ << endl;

    for (int cmd_cnt = 0; cmd_cnt < cmd_amount_ ; cmd_cnt++)
    {
        READ_CMD;
        printf("work = %d\n", work);

        switch (work)
        {
            case CMD_PUSH:
                READ_NUM_INT;
                binPush(code_, positions_, work_int);
                break;
            case CMD_POP:
                READ_NUM_INT;
                binPop(code_, positions_, work_int);
                break;
            case CMD_ADD:
                code_.INSERT_CODE(ADD);
                break;
            case CMD_MIN: //! SUB
                code_.INSERT_CODE(SUB);
                break;
            case CMD_MUL:
                code_.INSERT_CODE(MUL);
                break;
            case CMD_DIV:
                code_.INSERT_CODE(DIV);
                break;
            /*case CMD_JA:
                code_.INSERT_CODE(JA);
                break;
            case CMD_JAE:
                code_.INSERT_CODE(JAE);
                break;
            case CMD_JB:
                code_.INSERT_CODE(JB);
                break;
            case CMD_JBE:
                code_.INSERT_CODE(JBE);
                break;
            case CMD_JE:
                code_.INSERT_CODE(JE);
                break;
            case CMD_JNE:
                code_.INSERT_CODE(JNE);
                break;
            case CMD_JMP:
                code_.INSERT_CODE(JMP);
                break;*/
            default:
                printf("unknown command - %d\n", work);
                cout << "cmd_cnt = " << cmd_cnt << " cmd_amount = " << cmd_amount_ << endl;
                throw std::invalid_argument("wrong command.");
                break;
        }
    }
}

void binPush (vector<unsigned char>& code, vector<position_t> positions, int push_arg)
{
    if (-127 < push_arg && push_arg < 127)
    {
        code.push_back(PUSH_BYTE);
        code.push_back(push_arg);
        positions.push_back(position_t(opcode_cnt++, );
    }
    else
    {
        code.push_back(PUSH_DWORD);
        vector<unsigned char> reversed_num;
        dword_rev(reversed_num, push_arg);
        code.insert(code.end(), reversed_num.begin(), reversed_num.end());
    }
}

void binPop (vector<unsigned char>& code, vector<position_t> positionsint reg)
{
    assert(reg == ax_code || reg == bx_code || reg == cx_code || reg == dx_code);

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
}

void CProgram::jit_compile()
{
    void* m = alloc_writable_memory(1024);
    emit_code(m, code_);
    make_memory_executable(m, 1024);

    int (*func)(int) = (int (*)(int))m;
    printf("\nresult = %d\n", func(0));
}

int main()
{
    fstream input("source.bin");
    if (!input.is_open())
        throw std::invalid_argument("Unable to open source!");

    CProgram Program(input);
    Program.Translate();
    Program.jit_compile();

    return 0;
}
