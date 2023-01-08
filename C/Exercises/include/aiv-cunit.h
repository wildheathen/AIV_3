#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define CUNIT_STRING_EQ(expect, actual) \
    if(strcmp(expect, actual) != 0){ \
        printf("%s: Expected %s but was %s [%d]\n", __FUNCTION__ ,expect , actual, __LINE__); \
        exit(-1); \
    } \

#define CUNIT_RUNNER(...) \
int main() \
{ \
    void (*tests[])() = { __VA_ARGS__ }; \
    int count = sizeof(tests) / sizeof(void*); \
    for (int i = 0; i < count; ++i) { \
        tests[i](); \
    } \
        printf("All test passed! [%d]", count); \
    return 0; \
} \

#define CUNIT_TEST(name) void name()

#define CUNIT_IS_NULL(actual) \
    if(actual != NULL) { \
        printf("%s: Expected %s but was %p [%d]\n", __FUNCTION__ ,"NULL" , actual, __LINE__); \
        exit(-1); \
    } \
