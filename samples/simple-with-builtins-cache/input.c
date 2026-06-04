
#pragma message "It's working!"

#define foo(a, b) a+b+a+b

printf("x = %d", foo(10, MY_MACRO) == __TPP_EVAL(foo(10, MY_MACRO)));
