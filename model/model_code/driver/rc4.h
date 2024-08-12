#pragma once
#ifndef __RC4__
#define __RC4__

#include<ntdef.h>

#ifdef __cplusplus
extern "C" {
#endif
    typedef struct _RC4_CONTEXT {
        UINT8 SBox[256];
    } RC4_CONTEXT,*PRC4_CONTEXT;

    RC4_CONTEXT NTAPI Rc4GetContext(CONST PVOID Key, SIZE_T KeyLen);

    VOID NTAPI RC4Crypt(RC4_CONTEXT Context, _Inout_ PVOID Buffer, SIZE_T Length, SIZE_T Offset);

#ifdef __cplusplus
}
#endif
#endif
