#include <fltKernel.h>
#include <ntddk.h>
#include "operation.h"
#include "global.h"
#include "rc4.h"

#define MAX_PATH_LENGTH (8192)

CONST FLT_OPERATION_REGISTRATION callbacks[] = {
    //{
    //    IRP_MJ_CREATE,
    //    0,
    //    PreCreate,
    //    NULL,
    //},
    //{
    //    IRP_MJ_CLOSE,
    //    0,
    //    PreClose,
    //    NULL,
    //},
    {
        IRP_MJ_READ,
        0,
        PreRead,
        PostRead,
    },
    {
        IRP_MJ_WRITE,
        0,
        PreWrite,
        PostWrite,
    },
    {
        IRP_MJ_OPERATION_END,
    },
};

NTSTATUS FLTAPI
FilterUnloadCallback(
    FLT_FILTER_UNLOAD_FLAGS Flags)
{
    UNREFERENCED_PARAMETER(Flags);

    FltUnregisterFilter(gFilterHandle);
    KdPrint(("MiniFilter: Unloaded!\n"));
    //if (pPathPrefix) {
    //    ExFreePoolWithTag(pPathPrefix, MY_TAG);
    //}
    return STATUS_SUCCESS;
}

CONST FLT_REGISTRATION filterRegistration = {
    sizeof(FLT_REGISTRATION),   //  Size
    FLT_REGISTRATION_VERSION,   //  Version
    0,                          //  Flags

    NULL,                       //  Context
    callbacks,                  //  Operation callbacks

    FilterUnloadCallback,       //  MiniFilterUnload

    NULL,
    NULL,
    NULL,
    NULL,

    NULL,
    NULL,
    NULL,
};

NTSTATUS FLTAPI
DriverEntry(
    _In_ PDRIVER_OBJECT DriverObject,
    _In_ PUNICODE_STRING RegistryPath)
{
    UNREFERENCED_PARAMETER(RegistryPath);
    NTSTATUS status;

    status = FltRegisterFilter(DriverObject, &filterRegistration, &gFilterHandle);
    if (NT_SUCCESS(status)) {
        KdPrint(("MiniFilter: Compiled at %s, %s.", __TIME__, __DATE__));
        //pPathPrefix = (PWCHAR)ExAllocatePool2(NonPagedPool, MAX_PATH_LENGTH * sizeof(WCHAR), MY_TAG);
        //if (pPathPrefix) {
        //    RtlZeroMemory(pPathPrefix, MAX_PATH_LENGTH * sizeof(WCHAR));
        //    wcscpy_s(pPathPrefix, MAX_PATH_LENGTH, L"C:\\FsFilterDemo\\");
        //}
        status = FltStartFiltering(gFilterHandle);
        if (!NT_SUCCESS(status)) {
            FltUnregisterFilter(gFilterHandle);
        }
        else {
            KdPrint(("MiniFilter: Start Filtering!"));
        }
    }
    return status;
}
