#include "factorial.h"

uint32_t
factorialCal(uint32_t n)
{
    if (n == 1) {
        return 1;
    } else {
        return n*factorialCal(n-1);
    }
}