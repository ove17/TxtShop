// TestTxtShop.cpp

#include "CppUTest/TestHarness.h"

extern "C" {
    #include "TxtShop.h"
}


TEST_GROUP(TxtShop) {
    void setup() {
        const uint8_t numTxts[] = {5};
        TS_open(numTxts, 1);
    }

    void teardown() {
        TS_close();
    }
};


static const char* blob1 = "First\0second\0""3rd\0vierde\0cinquieme\0\0"; // L = 5, 6, 3, 6, 9
static const char* blob2 = "1\0two\0\x80\0""4\0""5\0\0";

static const char* buf = NULL;


TEST(TxtShop, loadBlobReturnsTheNumberOfCharactersInBlob) {
    LONGS_EQUAL(5, TS_loadBlob(blob1, 0));
}


TEST(TxtShop, getTxt0returnsThe1stSubstringInBlob) {
    TS_loadBlob(blob1, 0);
    BYTES_EQUAL(5, TS_getTxt(&buf, 0));
    STRNCMP_EQUAL("First\0", buf, 6);
}


TEST(TxtShop, getTxt2returnsThe3rdSubstringInBlob) {
    TS_loadBlob(blob1, 0);
    BYTES_EQUAL(3, TS_getTxt(&buf, 2));
    STRNCMP_EQUAL("3rd\0", buf, 4);
}


TEST(TxtShop, getTxt4returnsThe5thSubstringInBlob) {
    TS_loadBlob(blob1, 0);
    BYTES_EQUAL(9, TS_getTxt(&buf, 4));
    STRNCMP_EQUAL("cinquieme\0", buf, 10);
}


TEST(TxtShop, loadingNewBlobForgetsOldOne) {
    TS_loadBlob(blob1, 0);
    TS_loadBlob(blob2, 0);
    BYTES_EQUAL(3, TS_getTxt(&buf, 1));
    STRNCMP_EQUAL("two\0", buf, 4);
}


TEST(TxtShop, getTxtReturnsSingleCharacterStringFromBlob) {
    TS_loadBlob(blob2, 0);
    BYTES_EQUAL(1, TS_getTxt(&buf, 4));
    STRNCMP_EQUAL("5\0", buf, 2);
}


TEST(TxtShop, getTxtReturnsHexGT7FCharacterFromBlob) {
    TS_loadBlob(blob2, 0);
    BYTES_EQUAL(1, TS_getTxt(&buf, 2));
    STRNCMP_EQUAL("\x80\0", buf, 4);
}


const uint8_t numTxts[] = {5, 5};

TEST_GROUP(TxtShopWithOffset) {
    void setup() {
        TS_open(numTxts, 2);
    }

    void teardown() {
        TS_close();
    }
};


TEST(TxtShopWithOffset, loadingBlobIdGT0OffsetsIds) {
    BYTES_EQUAL(5, TS_loadBlob(blob1, 1));
    BYTES_EQUAL(6, TS_getTxt(&buf, 8));
    STRNCMP_EQUAL("vierde\0", buf, 6);
}


TEST(TxtShopWithOffset, loadingNewBlobWithStartIdDoesNotForgetsOldOne) {
    TS_loadBlob(blob1, 0);
    TS_loadBlob(blob2, 1);
    BYTES_EQUAL(1, TS_getTxt(&buf, 9));
    STRNCMP_EQUAL("5\0", buf, 7);
    BYTES_EQUAL(3, TS_getTxt(&buf, 6));
    STRNCMP_EQUAL("two\0", buf, 4);
}
