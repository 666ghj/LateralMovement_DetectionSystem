#pragma once
#ifndef __OPERATION__
#define __OPERATION__

#include <fltKernel.h>
#include <ntstrsafe.h>

#define PREOP_CALLBACK(NAME) FLT_PREOP_CALLBACK_STATUS FLTAPI NAME(     \
    _Inout_ PFLT_CALLBACK_DATA Data,        \
    _In_ PCFLT_RELATED_OBJECTS FltObjects,  \
    _Outptr_result_maybenull_ PVOID* CompletionContext)

#define POSTOP_CALLBACK(NAME) FLT_POSTOP_CALLBACK_STATUS FLTAPI NAME(   \
    _Inout_ PFLT_CALLBACK_DATA Data,        \
    _In_ PCFLT_RELATED_OBJECTS FltObjects,  \
    _In_opt_ PVOID CompletionContext,       \
    _In_ FLT_POST_OPERATION_FLAGS Flags)

#ifdef __cplusplus
extern "C" {
#endif

    PREOP_CALLBACK(PreCreate);

    PREOP_CALLBACK(PreClose);

    PREOP_CALLBACK(PostClose);

    PREOP_CALLBACK(PreRead);

    POSTOP_CALLBACK(PostRead);

    PREOP_CALLBACK(PreWrite);

    PREOP_CALLBACK(PreFlushBuffers);

    void CopyFile(PCWSTR sourcePath, PCWSTR destinationPath);

    void AppendTimestampedFilename(PWCHAR basePath);

    POSTOP_CALLBACK(PostWrite);

#ifdef __cplusplus
}
#endif

#endif
