// TxtShop.c

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include "TxtShop.h"


static uint8_t NumBlobs;
static uint8_t TotalNumTxts = 0;
static const uint8_t* NumTxts = NULL;
static const char** Blobs = NULL;
static uint16_t** Directory = NULL;


void TS_open(const uint8_t* numTxts,
             const uint8_t numBlobs) {
    assert(numBlobs > 0);
    NumBlobs = numBlobs;
    NumTxts = numTxts;
    Directory = calloc(numBlobs, sizeof(uint16_t*));
    for (uint8_t blob = 0; blob < numBlobs; blob++) {
        Directory[blob] = NULL;
        assert(NumTxts[blob] > 0);
        TotalNumTxts += NumTxts[blob];
        Directory[blob] = calloc(NumTxts[blob] + 1, sizeof(uint16_t));
    }
    Blobs = calloc(NumBlobs, sizeof(char*));
    assert(Directory != NULL);
}


void TS_close() {
    if (Directory != NULL) {
        for (uint8_t blob = 0; blob < NumBlobs; blob++) {
            if (Directory[blob] != NULL) {
                free(Directory[blob]);
            }
        }
        free(Directory);
    }
    if (Blobs != NULL) {
        free(Blobs);
    }
    Directory = NULL;
    Blobs = NULL;
    TotalNumTxts = 0;
    NumTxts = NULL;
    NumBlobs = 0;
}


static uint8_t buildDirectory(const uint8_t blobId) {
    uint16_t c = 0;
    for (uint8_t i = 0; ; i++) {
        Directory[blobId][i] = c;
        if (Blobs[blobId][c] == '\0') {
            return i;  // double \0 found
        }
        while (Blobs[blobId][c] != '\0') {
            c++;
        }
        c++;
    }
}


uint8_t TS_loadBlob(const char* blob,
                    const uint8_t blobId) {
    assert(blobId < NumBlobs);
    assert(Directory != NULL);
    assert(blob != NULL);
    Blobs[blobId] = blob;
    return buildDirectory(blobId);
}


static uint8_t getBlobId(const uint8_t txtId) {
    uint8_t numTxtsInBlob = 0;
    for (uint8_t blob = 0; blob < NumBlobs; blob++) {
        numTxtsInBlob += NumTxts[blob];
        if (txtId < numTxtsInBlob) {
            return blob;
        }
    }
    assert(0 && "Invalid txtId");
}


static uint8_t getBlobTxtId(const uint8_t txtId,
                         const uint8_t blobId) {
    uint8_t dirId = txtId;
    for (uint8_t blob = 0; blob < blobId; blob++) {
        dirId -= NumTxts[blob];
    }
    return dirId;
}


uint8_t TS_getTxt(const char** txtPtr, const uint8_t txtId) {
    assert(txtId < TotalNumTxts);
    assert(Blobs != NULL);
    const uint8_t blobId = getBlobId(txtId);
    const uint8_t blobTxtId = getBlobTxtId(txtId, blobId);
    if (txtPtr != NULL) { // if it is not a length-only call
        const uint16_t offset = Directory[blobId][blobTxtId];
        *txtPtr = &Blobs[blobId][offset];
    }
    return (uint8_t)(Directory[blobId][blobTxtId + 1] - Directory[blobId][blobTxtId]) - 1;
}
