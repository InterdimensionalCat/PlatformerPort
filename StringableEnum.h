#pragma once


//define a macro for autmatically generating small enum classes that can be converted to and from strings
//there must be 10 or less possible values for the enum

#ifndef _STRINGABLE_ENUM
#ifndef _ES
#define _ES(enumval) \
case enumval:\
    return #enumval;
#endif

#ifndef _SE
#define _SE(enumval) \
if(str == #enumval) return enumval;
#endif


#define s1(_1) _ES(_1)
#define s2(_1, _2) s1(_1) _ES(_2)
#define s3(_1, _2, _3) s2(_1, _2) _ES(_3)
#define s4(_1, _2, _3, _4) s3(_1, _2, _3) _ES(_4)
#define s5(_1, _2, _3, _4, _5) s4(_1, _2, _3, _4) _ES(_5)
#define s6(_1, _2, _3, _4, _5, _6) s5(_1, _2, _3, _4, _5) _ES(_6)
#define s7(_1, _2, _3, _4, _5, _6, _7) s6(_1, _2, _3, _4, _5, _6) _ES(_7)
#define s8(_1, _2, _3, _4, _5, _6, _7, _8) s7(_1, _2, _3, _4, _5, _6, _7) _ES(_8)
#define s9(_1, _2, _3, _4, _5, _6, _7, _8, _9) s8(_1, _2, _3, _4, _5, _6, _7, _8) _ES(_9)
#define s10(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10) s9(_1, _2, _3, _4, _5, _6, _7, _8, _9) _ES(_10)

#define e1(_1) _SE(_1)
#define e2(_1, _2) e1(_1) _SE(_2)
#define e3(_1, _2, _3) e2(_1, _2) _SE(_3)
#define e4(_1, _2, _3, _4) e3(_1, _2, _3) _SE(_4)
#define e5(_1, _2, _3, _4, _5) e4(_1, _2, _3, _4) _SE(_5)
#define e6(_1, _2, _3, _4, _5, _6) e5(_1, _2, _3, _4, _5) _SE(_6)
#define e7(_1, _2, _3, _4, _5, _6, _7) e6(_1, _2, _3, _4, _5, _6) _SE(_7)
#define e8(_1, _2, _3, _4, _5, _6, _7, _8) e7(_1, _2, _3, _4, _5, _6, _7) _SE(_8)
#define e9(_1, _2, _3, _4, _5, _6, _7, _8, _9) e8(_1, _2, _3, _4, _5, _6, _7, _8) _SE(_9)
#define e10(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10) e9(_1, _2, _3, _4, _5, _6, _7, _8, _9) _SE(_10)

#define _EXPAND(args) args
#define GET_ARG_N(_1,_2,_3,_4,_5,_6, _7, _8, _9, _10, N,...) N
#define E_TO_STR(...) _EXPAND(GET_ARG_N(__VA_ARGS__, s10, s9, s8, s7, s6, s5, s4, s3, s2, s1)(__VA_ARGS__))
#define STR_TO_E(...) _EXPAND(GET_ARG_N(__VA_ARGS__, e10, e9, e8, e7, e6, e5, e4, e3, e2, e1)(__VA_ARGS__))



#ifndef _CREATE_STRINGABLE_ENUM
#define _CREATE_STRINGABLE_ENUM(enumname, ...) \
enum class enumname {\
__VA_ARGS__,\
Count\
};\
static_assert((int)enumname::Count <= 10, "Stringable Enum must be 10 or less possible values(Excluding the required Count value)");\
inline std::string enumname##ToString(const enumname& val) {\
    using enum enumname;\
    switch(val) {\
        E_TO_STR(__VA_ARGS__)\
        default:\
			return "Count";\
	}\
}\
inline enumname enumname##FromString(const std::string& str) {\
    using enum enumname;\
    STR_TO_E(__VA_ARGS__)\
    return enumname::Count;\
}
#endif
#endif
