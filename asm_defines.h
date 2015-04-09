#define PROCESSREG(name)                if (streq(reg, str_##name))\
                                        {\
                                            arr_comm[i_comm] = name##_code;\
                                            i_comm++;\
                                            fscanf(input, "%s", command);\
                                            continue;\
                                        }

#define OPER(command, name)             if (streq(command, str_##name))\
                                        {\
                                            arr_comm[i_comm] = CMD_##name;\
                                            i_comm++;\
                                            fscanf(input, "%s", command);\
                                            continue;\
                                        }

#define GETJMP(name)                    if(streq(command, str_##name))\
                                        {\
                                            if (fscanf(input, "%s", jump_label) == 1)\
                                            {\
                                                arr_comm[i_comm] = CMD_##name;\
                                                i_comm++;\
                                                assert(0 <= i_comm && i_comm < comm_size);\
                                                int jumpNum = getLabel(arr_label, jump_label);\
                                                if (jumpNum == NOEXIST && pass == 2)\
                                                {\
                                                    DBG printf("ERR_<jump_label!> UNABLE TO FIND\
                                                                            LABEL <%s>!\n", jump_label);\
                                                    exit(CMD_##name);\
                                                }\
                                                \
                                                if (jumpNum >= 0 && arr_label[jumpNum].adress != NOEXIST)\
                                                    arr_comm[i_comm] = arr_label[jumpNum].adress;\
                                                else\
                                                    arr_comm[i_comm] = NOEXIST;\
                                                i_comm++;\
                                                \
                                                fscanf(input, "%s", command);\
                                                continue;\
                                            }\
                                        }

#define CMP(name)       if (streq(command, str_##name))\
                        {\
                            arr_comm[i_comm] = CMD_##name;\
                            i_comm++;\
                            \
                            if (fscanf(input, "%s", reg) == 1)\
                            {\
                                PROCESSREG(ax)\
                                PROCESSREG(bx)\
                                PROCESSREG(cx)\
                                PROCESSREG(dx)\
                                PROCESSREG(ex)\
                                PROCESSREG(fx)\
                                \
                                SLASHES DBG printf("ERR_<##name>!");\
                                dump_Comm(arr_comm, arr_label, comm_size, i_comm);\
                                exit(CMD_##name);\
                            }\
                            else\
                            {\
                                SLASHES DBG printf("ERR_<##name_arg>");\
                                dump_Comm(arr_comm, arr_label, comm_size, i_comm);\
                                exit(1);\
                            }\
                        }
