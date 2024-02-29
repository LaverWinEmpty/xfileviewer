#ifndef LWE__MACROS_HPP__
#define LWE__MACROS_HPP__

#define TO_STRING(x) #x
#define STR_MACRO(x) TO_STRING(x)

#define SAFE_RELEASE(ptr)                                                                                              \
    do {                                                                                                               \
        if(ptr) {                                                                                                      \
            ptr->Release();                                                                                            \
            ptr = nullptr;                                                                                             \
        }                                                                                                              \
    } while(false)
#define SAFE_DELETE(ptr)                                                                                               \
    do {                                                                                                               \
        if(ptr) {                                                                                                      \
            delete ptr;                                                                                                \
            ptr = nullptr;                                                                                             \
        }                                                                                                              \
    } while(false)
#define SAFE_DELETES(ptr)                                                                                              \
    do {                                                                                                               \
        if(ptr) {                                                                                                      \
            delete[] ptr;                                                                                              \
            ptr = nullptr;                                                                                             \
        }                                                                                                              \
    } while(false)

#define DEGREE_TO_RADIAN(value) value * 0.017453f
#define RADIAN_TO_DEGREE(value) value * 57.29577f

#define WINDOW_NAME "X File Viewer"
#endif