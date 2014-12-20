DEF_CMD (PUSH, 1,       {   /*printf("position = [%d]\n", position);*/\
                            position++;\
                            value = list_comm[position];\
                            pushData(&mystack, value);\
                            position++;\
                        })
DEF_CMD (PUSHR, 2,      {   position++;\
                            reg = list_comm[position];\
                            if (reg == ax_code)\
                                pushData(&mystack, mystack.reger[0]);\
                            if (reg == bx_code)\
                                pushData(&mystack, mystack.reger[1]);\
                            if (reg == cx_code)\
                                pushData(&mystack, mystack.reger[2]);\
                            if (reg == dx_code)\
                                pushData(&mystack, mystack.reger[3]);\
                            if (reg == ex_code)\
                                pushData(&mystack, mystack.reger[4]);\
                            if (reg == fx_code)\
                                pushData(&mystack, mystack.reger[5]);\
                            position++;\
                        })
DEF_CMD (POP,   3,      {   /*printf("position = [%d]\n", position);*/\
                            position++;\
                            reg = list_comm[position];\
                            if (reg == ax_code)\
                                mystack.reger[0] = popData(&mystack);\
                            if (reg == bx_code)\
                                mystack.reger[1] = popData(&mystack);\
                            if (reg == cx_code)\
                                mystack.reger[2] = popData(&mystack);\
                            if (reg == dx_code)\
                                mystack.reger[3] = popData(&mystack);\
                            if (reg == ex_code)\
                                mystack.reger[4] = popData(&mystack);\
                            if (reg == fx_code)\
                                mystack.reger[5] = popData(&mystack);\
                            position++;\
                        })
DEF_CMD (ADD,   4,      {   /*printf("position = [%d]\n", position);*/\
                        plusData(&mystack); position++;} )
DEF_CMD (MIN,   5,      {   /*printf("position = [%d]\n", position);*/\
                        nsumData(&mystack); position++;} )
DEF_CMD (JMP,   6,      {   ++position;\
                            int new_position = list_comm[position];\
                            position = new_position;\
                        })
DEF_CMD (MUL,   7,      {   /*printf("position = [%d]\n", position);*/\
                        multData(&mystack); position++; })
DEF_CMD (SQR,   8,      {   /*printf("position = [%d]\n", position);*/\
                        sqrData(&mystack); position++;})
DEF_CMD (SQRT,  9,      {   /*printf("position = [%d]\n", position);*/\
                        sqrtData(&mystack); position++;})
DEF_CMD (OUT,   11,     {   /*printf("position = [%d]\n", position);*/\
                            position++;\
                            reg = list_comm[position];\
                            if (reg == ax_code)\
                                printf("ax = (%lg)\n", mystack.reger[0]);\
                            if (reg == bx_code)\
                                printf("bx = (%lg)\n", mystack.reger[1]);\
                            if (reg == cx_code)\
                                printf("cx = (%lg)\n", mystack.reger[2]);\
                            if (reg == dx_code)\
                                printf("dx = (%lg)\n", mystack.reger[3]);\
                            if (reg == ex_code)\
                                printf("ex = (%lg)\n", mystack.reger[4]);\
                            if (reg == fx_code)\
                                printf("fx = (%lg)\n", mystack.reger[5]);\
                            position++;
                        })
DEF_CMD (DIV,   12,     {\
                            divData(&mystack); position++;\
                        })
DEF_CMD (JB,    13,     {   /*printf("jb position = [%d]\n", position);*/\
                        position++;\
                        double first = popData(&mystack);\
                        double second = popData(&mystack);\
                        pushData(&mystack, second);
                        if (second < first)\
                        {\
                            int new_position = list_comm[position];\
                            /*printf("new jb position = <%d>\n", new_position);*/\
                            position = new_position;\
                        }\
                        else\
                            position++;\
                    })
DEF_CMD (JA,    14,     {   position++;\
                        double first = popData(&mystack);\
                        double second = popData(&mystack);\
                        pushData(&mystack, second);\
                        if (second > first)\
                        {\
                            int new_position = list_comm[position];\
                            position = new_position;\
                        }\
                        else
                            position++;
                    })
DEF_CMD (JAC,   15,     {   \
                            position++;\
                            double first = popData(&mystack);\
                            double second = popData(&mystack);\
                            pushData(&mystack, second);\
                            if (second >= first)\
                            {\
                            int new_position = list_comm[position];\
                            position = new_position;\
                            }\
                            else
                            position++;
                        })
DEF_CMD (JBC,   16,     {   \
                            position++;\
                            double first = popData(&mystack);\
                            double second = popData(&mystack);\
                            pushData(&mystack, second);\
                            if (second <= first)\
                            {\
                                assert(0 <= position && position < list_size);
                                int new_position = list_comm[position];\
                                /*printf("new jbc position = <%d>\n", new_position);*/\
                                position = new_position;\
                            }\
                            else\
                                position++;\
                        })
DEF_CMD (JE,    17,     {   position++;\
                            double first = popData(&mystack);\
                            double second = popData(&mystack);\
                            pushData(&mystack, second);
                            if (second == first)\
                            {\
                                int new_position = list_comm[position];\
                                position = new_position;\
                            }\
                            else\
                                position++;\
                    })
DEF_CMD (JNE,   18,     {   \
                            position++;\
                            double first = popData(&mystack);\
                            double second = popData(&mystack);\
                            pushData(&mystack, second);\
                            if (second != first)\
                            {\
                                int new_position = list_comm[position];\
                                position = new_position;\
                            }\
                    })
DEF_CMD (IN, 19,        {\
                            position++;\
                            reg = list_comm[position];\
                            double value = 0;\
                            scanf("%lf", &value);\
                            if (reg == ax_code)\
                                mystack.reger[0] = value;\
                            if (reg == bx_code)\
                                mystack.reger[1] = value;\
                            if (reg == cx_code)\
                                mystack.reger[2] = value;\
                            if (reg == dx_code)\
                                mystack.reger[3] = value;\
                            if (reg == ex_code)\
                                mystack.reger[4] = value;\
                            if (reg == fx_code)\
                                mystack.reger[5] = value;\
                            position++;\
                        })

DEF_CMD (CALL, 1337,    {   \
                            position++;\
                            int new_position = list_comm[position];\
                            position++;\
                            pushCall(&call_stack, position);
                            position = new_position;\
                        })
DEF_CMD (RET,  1488,    {   \
                            position = popCall(&call_stack);\
                        })
DEF_CMD (VAR, 100,      {
                            //printf("var command!\n");
                            //position++;
                            //position++;
                            //position++;
                        })
DEF_CMD (PUSHV, 105,    {  \
                            position++;\
                            int index = list_comm[position];\
                            position++;\
                            int mode = list_comm[position];\
                            position++;
                            int var_index = list_comm[position];\
                            \
                            if (mode == NUMBER)\
                                pushData(&mystack, G_Var[index + var_index]);\
                            else\
                            {\
                                int var_pos = list_comm[position];\
                                var_index = G_Var[var_pos];\
                                pushData(&mystack, G_Var[index + var_index]);\
                            }\
                            position++;\
                        })
DEF_CMD (POPV, 110,     {\
                            position++;\
                            int index = list_comm[position];\
                            position++;\
                            int mode = list_comm[position];\
                            position++;\
                            int var_index = list_comm[position];\
                            \
                            if (mode == NUMBER)\
                                G_Var[index + var_index] = popData(&mystack);\
                            else\
                            {\
                                int var_pos = list_comm[position];\
                                var_index = G_Var[var_pos];\
                                G_Var[index + var_index] = popData(&mystack);\
                            }\
                            position++;\
                        })
DEF_CMD (OUTV, 115,     {
                            position++;\
                            int index = list_comm[position];\
                            position++;\
                            int mode = list_comm[position];\
                            position++;\
                            int var_index = list_comm[position];\
                            \
                            printf("VAR#%lg ", index);\

                            if (mode == NUMBER)\
                            {\
                                index += var_index;\
                                printf("[%d] = (%lg)\n", index, G_Var[index + var_index]);\
                            }\
                            else\
                            {\
                                int var_pos = list_comm[position];\
                                var_index = G_Var[var_pos];\
                                printf("[%d] = (%lg) INDEX_VAR\n", index + var_index, G_Var[index + var_index]);\
                            }\
                            position++;\
                        })
DEF_CMD (END, 100500, { })
