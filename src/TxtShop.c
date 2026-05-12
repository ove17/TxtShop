// TxtShop.c

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>


static uint8_t NumTxts = 0;
static const char* Blob = NULL;
static uint16_t* Directory = NULL;


void TS_open(const uint8_t numTxts) {
    assert(numTxts > 0);
    NumTxts = numTxts;
    Directory = calloc(NumTxts, sizeof(uint16_t));
    assert(Directory != NULL);
}


void TS_close() {
    if (Directory != NULL) {
        free(Directory);
    }
    Directory = NULL;
    Blob = NULL;
    NumTxts = 0;
}


static uint16_t buildDirectory() {
    uint16_t c = 0;
    for (uint8_t i = 0; i < NumTxts; i++) {
        Directory[i] = c;
        while (Blob[c] != '\0') {
            c++;
        }
        c++;
    }
    return c;
}


uint16_t TS_loadBlob(const char* blob) {
    assert(NumTxts > 0);
    assert(Directory != NULL);
    assert(blob != NULL);
    Blob = blob;
    return buildDirectory();
}


const char* TS_getTxt(const uint8_t txtId) {
    assert(txtId < NumTxts);
    assert(Blob != NULL);
    uint16_t offset = Directory[txtId];
    return &Blob[offset];
}
