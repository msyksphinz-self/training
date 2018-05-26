[BITS 32]

GLOBAL api_getlang

api_getlang: ; int api_getlang (void);
    mov      edx, 27
    int      0x40
    ret
