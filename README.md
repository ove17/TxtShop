# TxtShop

TxtShop is a small C library for indexed access to strings stored in a null-separated blob.

## Concept

A blob contains multiple null-terminated strings:


"first\0second\0third\0"


TxtShop builds a directory of offsets at load time, allowing O(1) access to any string by index.

## Features

- Zero external dependencies
- O(1) string lookup via precomputed offset directory
- Immutable blob after loading
- Minimal memory overhead
- Designed for embedded systems

## API

```c
void TS_open(uint8_t numTxts);
void TS_close(void);

uint16_t TS_loadBlob(const char* blob);
const char* TS_getTxt(uint8_t txtId);
```

##Usage

```c
TS_open(3);

uint16_t size = TS_loadBlob(
    "hello\0world\0test\0"
);

const char* s = TS_getTxt(1); // "world"
```

##Constraints

txtId must be in range [0, numTxts - 1]
Blob must contain at least numTxts null-terminated strings
All invalid usage results in assertions (debug builds)
Lifetime rules
The blob remains valid until TS_loadBlob is called again
Returned pointers are read-only
Design notes

TxtShop prioritizes simplicity and deterministic behavior over flexibility. It is intended for embedded or performance-sensitive environments where string tables are known at compile-time or load-time.
