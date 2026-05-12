// TestTxtShop.cpp

#include "CppUTest/TestHarness.h"

extern "C" {
    #include "TxtShop.h"
}


TEST_GROUP(TxtShop) {
    void setup() {
        TS_open(5);
    }

    void teardown() {
        TS_close();
    }
};


static const char* blob1 = "First\0second\0""3rd\0vierde\0cinquieme\0";
static const char* blob2 = "1\0two\0\x80\0""4\0""5\0";


TEST(TxtShop, loadBlobReturnsTheNumberOfCharactersInBlob) {
    LONGS_EQUAL(34, TS_loadBlob(blob1));
}


TEST(TxtShop, getTxt0returnsThe1stSubstringInBlob) {
    TS_loadBlob(blob1);
    STRNCMP_EQUAL("First\0", TS_getTxt(0), 6);
}


TEST(TxtShop, getTxt2returnsThe3rdSubstringInBlob) {
    TS_loadBlob(blob1);
    STRNCMP_EQUAL("3rd\0", TS_getTxt(2), 4);
}


TEST(TxtShop, getTxt4returnsThe5thSubstringInBlob) {
    TS_loadBlob(blob1);
    STRNCMP_EQUAL("cinquieme\0", TS_getTxt(4), 10);
}


TEST(TxtShop, loadingNewBlobForgetsOldOne) {
    TS_loadBlob(blob1);
    TS_loadBlob(blob2);
    STRNCMP_EQUAL("two\0", TS_getTxt(1), 4);
}


TEST(TxtShop, getTxtReturnsSingleCharacterStringFromBlob) {
    TS_loadBlob(blob2);
    STRNCMP_EQUAL("5\0", TS_getTxt(4), 2);
}


TEST(TxtShop, getTxtReturnsHexGT7FCharacterFromBlob) {
    TS_loadBlob(blob2);
    STRNCMP_EQUAL("\x80\0", TS_getTxt(2), 2);
}
