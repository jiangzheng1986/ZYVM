//---------------------------------------------------------------------------------------------------------
// Virtual Machine - Preprocessor
// Author£ºAndersJiang(½¯Õþ)
//---------------------------------------------------------------------------------------------------------

#include "Preprocessor.h"

const char *source1 = R"(
#define AA 
#define BB aa
#define CC() aa bb aa bb
#define DD(aa, bb) aa bb aa bb
)";

const char *source2 = R"(
#define AA aa
#define BB(x) AA()
BB(bb)
)";

const char *source3 = R"(
#define AA aa
#define BB(x, y) AA(x, x, y, y)
BB((bb+cc), (dd+ee))
)";

const char *source4 = R"(
#define AA aa\
bb
AA
)";

const char *source5 = R"(
#define AA a
#ifdef CC
#define BB b
#else
#define BB c
#endif
BB
)";

const char *source6 = R"(
#define AA a
#undef AA
#if defined(AA)
#define BB b
#endif
BB
)";

const char *source7 = R"(
#define AA a
#undef DD
#if defined __FILE__
#define BB b
#else
#define BB c
#endif
BB
)";

const char *source8 = R"(
#define DD
#define CC
#ifdef AA
# define BB aa
#elif defined(CC)
# ifdef DD
#  define BB dd
# else
#  define BB cc
# endif
AA
#else
# define BB bb
#endif
BB
)";

const char *source9 = R"(
#error stay foolish, stay hungry
#pragma STDGL
#pragma STDGL invariant(all)
#pragma optimize(on)
#pragma optimize(off)
#pragma debug(on)
#pragma debug(off)
#extension options
#extension extension_name : require
#extension extension_name : enable
#extension extension_name : warn
#extension extension_name : disable
#extension all : enable
#version 330
#line 200 2
AA 12
)";

const char *source10 = R"(
#if defined __FILE__
#define BB b
#else
#define BB c
#endif
BB
+ - * /
&& || ! == != > < >= <=
)";

const char *source11 = R"(
#define BB 1
#if BB && __VERSION__ >= 330
#define AA true
#else
#define AA false
#endif
AA
~ % & | ^
)";

const char *source12 = R"(
#if 1|0
#define AA true
#else
#define AA false
#endif
AA
)";

int main() {
    Scanner scanner;
    scanner.open_source(source12);
    scanner.preprocess();
    scanner.print_macro_list();
    printf("\n");
    scanner.print_token_list();
    printf("\n");
    printf("Done.\n");
    while (1) {
        Sleep(10);
    }
	return 0;
}