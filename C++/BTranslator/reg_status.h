enum REGS_NUM
{
    RAX = 1,
    RBX = 2,
    RCX = 3,
    RDX = 4,
    RDI = 5,
    RSI = 6
};

class CREG_STATUS
{
    private:
        bool RAX_R;  //!  first  REG for result
        bool RBX_R;  //!  2nd    REG for result
        bool RCX_R;  //!  return REG
        bool RDX_R;  //!  work   REG for calc
        bool RDI_R;
        bool RSI_R;
    //!---------------------------
    public:
        CREG_STATUS(): RAX_R(true), RBX_R(true), RCX_R(true), RDX_R(true), RDI_R(true), RSI_R(true) {}
        int result_reg();
};

int CREG_STATUS::result_reg()
{
    if (RAX_R)
        return RAX;

    if (!RBX_R)
        throw  std::invalid_argument("RAX AND RBX ARE USED! ERROR!");

    return RBX;
}


