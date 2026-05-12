/*
 * TxtShop.h
 *
 * Library for indexed access to strings in a null-separated blob.
 *
 * The caller is responsible for providing valid input.
 * Invalid txtId values or calling TS_getTxt before loading a blob are
 *  considered programming errors and trigger asserts.
 */


#ifndef TXT_SHOP_H
#define TXT_SHOP_H


/*
 * Opens txtShop by allocating its directory.
 * Note that blob needs to be loaded separately.
 */
void TS_open(const uint8_t numTxts);

/*
 * Closes txtShop, deallocates directory.
 */
void TS_close();

/*
 * Loads a new blob and generates its directory.
 * Returns the number of characters in the blob.
 * A blob must be < 65535 characters.
 * After this, the previous blob becomes inaccessible and using its pointers
 *  results in undefined behaviour.
 * Providing a blob with more characters than 65535 or fewer \0 characters than
 *  numTxts results in undefined behaviour
 *
 * The return value can be used by the caller for sanity check or validation.
 */
uint16_t TS_loadBlob(const char* newBlob);

/*
 * Returns a pointer to a 0 terminated string with index txtId.
 * Valid txtId values are 0 to numTxts - 1
 * Strings are indexed by their position in the blob:
 * the first string has txtId 0, the second txtId 1, etc.
 * The string is immutable and remains valid until a new blob is loaded.
 */
const char* TS_getTxt(const uint8_t txtId);

#endif
