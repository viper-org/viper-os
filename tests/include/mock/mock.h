#ifndef VIPEROS_TESTS_MOCK_MOCK_H
#define VIPEROS_TESTS_MOCK_MOCK_H 1

#include <vector>

#define MOCK0(nm, className, type, funcName) \
    namespace mock { \
        namespace nm { \
            struct className { \
                struct funcName##Invocation {}; \
                static std::vector<funcName##Invocation> funcName##Invocations; \
                static type funcName() { \
                    funcName##Invocations.push_back({}); \
                } \
            }; \
            std::vector<className::funcName##Invocation> className::funcName##Invocations; \
        } \
    }

#define MOCK1(nm, className, type, funcName, arg1T, arg1Name) \
    namespace mock { \
        namespace nm { \
            struct className { \
                struct funcName##Invocation { \
                    arg1T arg1Name; \
                }; \
                static std::vector<funcName##Invocation> funcName##Invocations; \
                static type funcName(arg1T arg1Name) { \
                    funcName##Invocations.push_back({ \
                        arg1Name, \
                    }); \
                } \
            }; \
            std::vector<className::funcName##Invocation> className::funcName##Invocations; \
        } \
    }

#define MOCK2(nm, className, type, funcName, arg1T, arg1Name, arg2T, arg2Name) \
    namespace mock { \
        namespace nm { \
            struct className { \
                struct funcName##Invocation { \
                    arg1T arg1Name; \
                    arg2T arg2Name; \
                }; \
                static std::vector<funcName##Invocation> funcName##Invocations; \
                static type funcName(arg1T arg1Name, arg2T arg2Name) { \
                    funcName##Invocations.push_back({ \
                        arg1Name, arg2Name, \
                    }); \
                } \
            }; \
            std::vector<className::funcName##Invocation> className::funcName##Invocations; \
        } \
    }

#define MOCK3(nm, className, type, funcName, arg1T, arg1Name, arg2T, arg2Name, arg3T, arg3Name) \
    namespace mock { \
        namespace nm { \
            struct className { \
                struct funcName##Invocation { \
                    arg1T arg1Name; \
                    arg2T arg2Name; \
                    arg3T arg3Name; \
                }; \
                static std::vector<funcName##Invocation> funcName##Invocations; \
                static type funcName(arg1T arg1Name, arg2T arg2Name, arg3T arg3Name) { \
                    funcName##Invocations.push_back({ \
                        arg1Name, arg2Name, arg3Name, \
                    }); \
                } \
            }; \
            std::vector<className::funcName##Invocation> className::funcName##Invocations; \
        } \
    }

#define MOCK4(nm, className, type, funcName, arg1T, arg1Name, arg2T, arg2Name, arg3T, arg3Name, arg4T, arg4Name) \
    namespace mock { \
        namespace nm { \
            struct className { \
                struct funcName##Invocation { \
                    arg1T arg1Name; \
                    arg2T arg2Name; \
                    arg3T arg3Name; \
                    arg4T arg4Name; \
                }; \
                static std::vector<funcName##Invocation> funcName##Invocations; \
                static type funcName(arg1T arg1Name, arg2T arg2Name, arg3T arg3Name, arg4T arg4Name) { \
                    funcName##Invocations.push_back({ \
                        arg1Name, arg2Name, arg3Name, arg4Name, \
                    }); \
                } \
            }; \
            std::vector<className::funcName##Invocation> className::funcName##Invocations; \
        } \
    }

#define MOCK5(nm, className, type, funcName, arg1T, arg1Name, arg2T, arg2Name, arg3T, arg3Name, arg4T, arg4Name, arg5T, arg5Name) \
    namespace mock { \
        namespace nm { \
            struct className { \
                struct funcName##Invocation { \
                    arg1T arg1Name; \
                    arg2T arg2Name; \
                    arg3T arg3Name; \
                    arg4T arg4Name; \
                    arg5T arg5Name; \
                }; \
                static std::vector<funcName##Invocation> funcName##Invocations; \
                static type funcName(arg1T arg1Name, arg2T arg2Name, arg3T arg3Name, arg4T arg4Name, arg5T arg5Name) { \
                    funcName##Invocations.push_back({ \
                        arg1Name, arg2Name, arg3Name, arg4Name, arg5Name, \
                    }); \
                } \
            }; \
            std::vector<className::funcName##Invocation> className::funcName##Invocations; \
        } \
    }

#define MOCK6(nm, className, type, funcName, arg1T, arg1Name, arg2T, arg2Name, arg3T, arg3Name, arg4T, arg4Name, arg5T, arg5Name, arg6T, arg6Name) \
    namespace mock { \
        namespace nm { \
            struct className { \
                struct funcName##Invocation { \
                    arg1T arg1Name; \
                    arg2T arg2Name; \
                    arg3T arg3Name; \
                    arg4T arg4Name; \
                    arg5T arg5Name; \
                    arg6T arg6Name; \
                }; \
                static std::vector<funcName##Invocation> funcName##Invocations; \
                static type funcName(arg1T arg1Name, arg2T arg2Name, arg3T arg3Name, arg4T arg4Name, arg5T arg5Name, arg6T arg6Name) { \
                    funcName##Invocations.push_back({ \
                        arg1Name, arg2Name, arg3Name, arg4Name, arg5Name, arg6Name, \
                    }); \
                } \
            }; \
            std::vector<className::funcName##Invocation> className::funcName##Invocations; \
        } \
    }

#define MOCK7(nm, className, type, funcName, arg1T, arg1Name, arg2T, arg2Name, arg3T, arg3Name, arg4T, arg4Name, arg5T, arg5Name, arg6T, arg6Name, arg7T, arg7Name) \
    namespace mock { \
        namespace nm { \
            struct className { \
                struct funcName##Invocation { \
                    arg1T arg1Name; \
                    arg2T arg2Name; \
                    arg3T arg3Name; \
                    arg4T arg4Name; \
                    arg5T arg5Name; \
                    arg6T arg6Name; \
                    arg7T arg7Name; \
                }; \
                static std::vector<funcName##Invocation> funcName##Invocations; \
                static type funcName(arg1T arg1Name, arg2T arg2Name, arg3T arg3Name, arg4T arg4Name, arg5T arg5Name, arg6T arg6Name, arg7T arg7Name) { \
                    funcName##Invocations.push_back({ \
                        arg1Name, arg2Name, arg3Name, arg4Name, arg5Name, arg6Name, arg7Name, \
                    }); \
                } \
            }; \
            std::vector<className::funcName##Invocation> className::funcName##Invocations; \
        } \
    }

#endif