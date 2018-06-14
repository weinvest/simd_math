#ifndef _FAST_MATH_MACROS_H
#define _FAST_MATH_MACROS_H

#define FM_COMMA() ,
#define FM_BLANK()
#define FM_FUNC() );
#define FM_REPEAT_0(v, sep)
#define FM_REPEAT_1(v, sep) v##0
#define FM_REPEAT_2(v, sep) v##1 sep() FM_REPEAT_1(v,sep)
#define FM_REPEAT_3(v, sep) v##2 sep() FM_REPEAT_2(v,sep)
#define FM_REPEAT_4(v, sep) v##3 sep() FM_REPEAT_3(v,sep)
#define FM_REPEAT_5(v, sep) v##4 sep() FM_REPEAT_4(v,sep)
#define FM_REPEAT_6(v, sep) v##5 sep() FM_REPEAT_5(v,sep)
#define FM_REPEAT_7(v, sep) v##6 sep() FM_REPEAT_6(v,sep)
#define FM_REPEAT_8(v, sep) v##7 sep() FM_REPEAT_7(v,sep)
#define FM_REPEAT_9(v, sep) v##8 sep() FM_REPEAT_8(v,sep)
#define FM_REPEAT_10(v, sep) v##9 sep() FM_REPEAT_9(v,sep)
#define FM_REPEAT_11(v, sep) v##10 sep() FM_REPEAT_10(v,sep)
#define FM_REPEAT_12(v, sep) v##11 sep() FM_REPEAT_11(v,sep)
#define FM_REPEAT_13(v, sep) v##12 sep() FM_REPEAT_12(v,sep)
#define FM_REPEAT_14(v, sep) v##13 sep() FM_REPEAT_13(v,sep)
#define FM_REPEAT_15(v, sep) v##14 sep() FM_REPEAT_14(v,sep)


#define FM_REPEAT(cnt, v, sep) FM_REPEAT##cnt(v, sep)

#endif
