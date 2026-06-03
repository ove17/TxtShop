/*
 * TxtShop.h
 *
 * Library for indexed access to strings in a null-separated blob.
 *
 * One or more blobs may be used simultaneously and they are accessed by a
 * continuous index, i.e. if the first blob has 10 strings, string 11 is the
 * first string of the second blob.
 *
 * The caller is responsible for providing valid input.
 * Invalid txtId values or calling TS_getTxt before loading a blob are
 *  considered programming errors and trigger asserts.
 */


#ifndef TXT_SHOP_H
#define TXT_SHOP_H


/*
 * Opens txtShop by allocating its directory.
 * numTxts is an array with numBlobs values, where each element is the length
 *  of a blob.
 * The total length of all blobs must not exceed 255 strings.
 * Note that the blob(s) need to be loaded separately.
 */
void TS_open(const uint8_t* numTxts,
             const uint8_t numBlobs);

/*
 * Closes txtShop, deallocates directory.
 */
void TS_close();


/*
 * Loads a new blob, generates its directory and returns the number of strings
 * found in the blob.
 * A blob is a \0 separated list of strings, terminated by \0\0.
 * Multiple blobs can be accessible simultaneously by loading them using
 * different blobId's, provided that:
 *  - the total number of blobs does not exceed that registered with TS_open()
 *  - the total number of strings does not exceed 255
 *  - the total number of characters < 65535
 *
 * When an existing blob is overwritten, using its pointer may result in
 * undefined behaviour.
 * Providing a blob with more characters than 65535 or more strings than fit in
 *  numTxts results in undefined behaviour
 *
 * The return value can be used by the caller for sanity check or validation.
 */
uint8_t TS_loadBlob(const char* newBlob,
                    const uint8_t blobId);

/*
 * sets txtPtr to the pointer to a 0 terminated string with index txtId.
 * Returns the length of the string, without the trailing \0.
 * Valid txtId values are 0 to numTxts - 1
 * Strings are indexed by their position in the blob:
 * the first string has txtId 0, the second txtId 1, etc.
 * The string is immutable and remains valid until a new blob is loaded.
 */
uint8_t TS_getTxt(const char** txtPtr,
                  const uint8_t txtId);

#endif
