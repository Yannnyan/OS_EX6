#include "doctest.h"
#include "ActiveObject.hpp"
#include <string.h>
#include <stdio.h>

using namespace ex6;



TEST_CASE("CONVERT CASE LETTERS")
{
    char * abcd = new char[5];
    strncpy(abcd, "abcd\0", 5);
    CHECK(strcmp("ABCD", (char *)convert_opposite_case_letters(abcd)) == 0);
    delete[](abcd);
    char * ABCD = new char[5];
    strncpy(ABCD, "ABCD\0", 5);
    CHECK(strcmp("abcd", (char *) convert_opposite_case_letters(ABCD)) == 0);
    delete[](ABCD);
    char * aBcD = new char[5];
    strncpy(aBcD, "aBcD\0", 5);
    CHECK(strcmp("AbCd", (char *)convert_opposite_case_letters(aBcD)) == 0);
    delete[](aBcD);
}

TEST_CASE("caesar_cipher1")
{
    // testcase1
    char * hal = new char[4];
    strncpy(hal, "HAL\0",4);
    CHECK(strcmp("IBM", (char *) caesar_cipher_1(hal)) == 0);
    // ciclicity
    char * z = new char[3];
    strncpy(z, "zZ\0", 3);
    CHECK(strcmp("aA", (char *) caesar_cipher_1(z)) == 0);
    delete[] hal;
    delete[] z;
}
