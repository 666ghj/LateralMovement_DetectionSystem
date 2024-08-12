#include"rc4.h"
#include <fltKernel.h>
#include "global.h"

RC4_CONTEXT NTAPI Rc4GetContext(CONST PVOID Key, SIZE_T KeyLen)
{
    //��ʼ��RC4
    RC4_CONTEXT retval;
    UINT8* TBox = retval.SBox;

    for (SIZE_T i = 0; i < 256; i++) {
        TBox[i] = (UINT8)i; //ǿת��UINT8
    }

    SIZE_T j = 0;
    UINT8 temp;
    for (SIZE_T i = 0; i < 256; i++){
        j = (j + ((UINT8*)Key)[i % KeyLen]) % 256;
        temp = TBox[i];
        TBox[i] = TBox[j];
        TBox[j] = temp;
    }

    return retval;
}

VOID NTAPI RC4Crypt(RC4_CONTEXT Context, _Inout_ PVOID Buffer, SIZE_T Length, SIZE_T Offset)
{
    UINT8* TBox = Context.SBox;
    SIZE_T i = 0;
    SIZE_T j = 0;
    SIZE_T t = 0;
    UINT8 tmp;

    for (SIZE_T index = 0; index < Offset ; index++) {
        i = (i + 1) % 256;
        j = (j + TBox[i]) % 256;

        //����s_box[i] �� s_box[j]

        tmp = TBox[i];
        TBox[i] = TBox[j];
        TBox[j] = tmp;

        //������֮�� �ٰ�s[i] + s[j]����ϵ����±���ȥ���
        t = (TBox[i] + TBox[j]) % 256;
    }

    for (SIZE_T index = Offset; index < Offset + Length; index++) {
        i = (i + 1) % 256;
        j = (j + TBox[i]) % 256;

        //����s_box[i] �� s_box[j]

        tmp = TBox[i];
        TBox[i] = TBox[j];
        TBox[j] = tmp;

        //������֮�� �ٰ�s[i] + s[j]����ϵ����±���ȥ���
        t = (TBox[i] + TBox[j]) % 256;
        ((UINT8*)Buffer)[index - Offset] ^= TBox[t];
    }

    return;
}
