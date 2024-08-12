#include "operation.h"
#include <fltKernel.h>
#include <ntddk.h>
#include <dontuse.h>
#include <suppress.h>
#include "global.h"
#include "rc4.h"

#define ULONG_MAX (4294967295U)

#define PATH_LEN (8192)
WCHAR infoFilePath[PATH_LEN];

//PREOP_CALLBACK(PreCreate) {
//    UNREFERENCED_PARAMETER(FltObjects);
//    PFLT_FILE_NAME_INFORMATION fileNameInfo = NULL;
//    NTSTATUS status;
//
//    status = FltGetFileNameInformation(Data, FLT_FILE_NAME_NORMALIZED | FLT_FILE_NAME_QUERY_DEFAULT, &fileNameInfo);
//    if (!NT_SUCCESS(status)) {
//        goto EXIT;
//    }
//    status = FltParseFileNameInformation(fileNameInfo);
//    if (!NT_SUCCESS(status)) {
//        goto EXIT;
//    }
//    if (wcsstr(fileNameInfo->Name.Buffer, gPath)) {
//        KdPrint(("Minifilter: CREATE %ws\n", fileNameInfo->Name.Buffer));
//    }
//
//EXIT:
//    CompletionContext = NULL;
//    if (fileNameInfo)
//        FltReleaseFileNameInformation(fileNameInfo);
//    return FLT_PREOP_SUCCESS_NO_CALLBACK;
//}

//PREOP_CALLBACK(PreClose) {
//    UNREFERENCED_PARAMETER(FltObjects);
//    KdPrint(("Minifilter: PreClose activated"));
//    PFLT_FILE_NAME_INFORMATION fileNameInfo = NULL;
//    NTSTATUS status;
//
//    status = FltGetFileNameInformation(Data, FLT_FILE_NAME_NORMALIZED | FLT_FILE_NAME_QUERY_DEFAULT, &fileNameInfo);
//    if (!NT_SUCCESS(status)) {
//        goto EXIT;
//    }
//    status = FltParseFileNameInformation(fileNameInfo);
//    if (!NT_SUCCESS(status)) {
//        goto EXIT;
//    }
//    if (wcsstr(fileNameInfo->Name.Buffer, gPath)) {
//        KdPrint(("Minifilter: CLOSE %ws\n", fileNameInfo->Name.Buffer));
//    }
//
//EXIT:
//    CompletionContext = NULL;
//    if (fileNameInfo)
//        FltReleaseFileNameInformation(fileNameInfo);
//    return FLT_PREOP_SUCCESS_NO_CALLBACK;
//}

PREOP_CALLBACK(PreRead) {
    UNREFERENCED_PARAMETER(FltObjects);
    PFLT_FILE_NAME_INFORMATION fileNameInfo = NULL;
    NTSTATUS status;
    FLT_PREOP_CALLBACK_STATUS retval = FLT_PREOP_SUCCESS_NO_CALLBACK;
    PEPROCESS process = PsGetCurrentProcess();
    PUNICODE_STRING processImageName = NULL;

    status = SeLocateProcessImageName(process, &processImageName);
    if (!NT_SUCCESS(status) || !processImageName || !processImageName->Buffer) {
        goto EXIT;
    }
    status = FltGetFileNameInformation(Data, FLT_FILE_NAME_NORMALIZED | FLT_FILE_NAME_QUERY_DEFAULT, &fileNameInfo);
    if (!NT_SUCCESS(status) || !fileNameInfo) {
        goto EXIT;
    }
    status = FltParseFileNameInformation(fileNameInfo);
    if (!NT_SUCCESS(status) || !fileNameInfo->Name.Buffer) {
        goto EXIT;
    }
    if (wcscmp(processImageName->Buffer, gPath) == 0) {
        for (int i = 0; gIgnoredDirs[i]; i++) {
            if (wcsstr(fileNameInfo->Name.Buffer, gIgnoredDirs[i]) == fileNameInfo->Name.Buffer) {
                goto EXIT;
            }
        }
        for (int i = 0; gIgnoredRead[i]; i++) {
            if (wcsstr(fileNameInfo->Name.Buffer, gIgnoredRead[i])) {
                goto EXIT;
            }
        }
        //KdPrint(("Minifilter: Process %ws invokes READ.", processImageName->Buffer));
        ULONG length = Data->Iopb->Parameters.Read.Length;
        LONGLONG offset = Data->Iopb->Parameters.Read.ByteOffset.QuadPart;
        KdPrint(("Minifilter: READ %lu BYTES FROM %ws AT %lld by %wZ\n", length, fileNameInfo->Name.Buffer, offset, processImageName));
        retval = FLT_PREOP_SUCCESS_WITH_CALLBACK;
    }
    //else {
    //    KdPrint(("Minifilter: PROCESS %ws IGNORED\n", processImageName->Buffer));
    //}

EXIT:
    CompletionContext = NULL;
    if (processImageName)
        ExFreePool(processImageName);
    if (fileNameInfo)
        FltReleaseFileNameInformation(fileNameInfo);
    return retval;
}

POSTOP_CALLBACK(PostRead) {
    UNREFERENCED_PARAMETER(FltObjects);
    UNREFERENCED_PARAMETER(CompletionContext);
    UNREFERENCED_PARAMETER(Flags);
    ULONG length = Data->Iopb->Parameters.Read.Length;
    LONGLONG offset = Data->Iopb->Parameters.Read.ByteOffset.QuadPart;
    PUCHAR buffer = (PUCHAR)Data->Iopb->Parameters.Read.ReadBuffer;

    SIZE_T key_len = strlen(gRc4Key);
    RC4_CONTEXT TBox = Rc4GetContext((CONST PVOID)gRc4Key, key_len);
    RC4Crypt(TBox, buffer, length, offset);

    return FLT_POSTOP_FINISHED_PROCESSING;
}

PREOP_CALLBACK(PreWrite) {
    UNREFERENCED_PARAMETER(FltObjects);
    PFLT_FILE_NAME_INFORMATION fileNameInfo = NULL;
    NTSTATUS status;
    FLT_PREOP_CALLBACK_STATUS retval = FLT_PREOP_SUCCESS_NO_CALLBACK;
    PEPROCESS process = PsGetCurrentProcess();
    PUNICODE_STRING processImageName = NULL;

    status = SeLocateProcessImageName(process, &processImageName);
    if (!NT_SUCCESS(status) || !processImageName || !processImageName->Buffer) {
        goto EXIT;
    }
    status = FltGetFileNameInformation(Data, FLT_FILE_NAME_NORMALIZED | FLT_FILE_NAME_QUERY_DEFAULT, &fileNameInfo);
    if (!NT_SUCCESS(status) || !fileNameInfo) {
        goto EXIT;
    }
    status = FltParseFileNameInformation(fileNameInfo);
    if (!NT_SUCCESS(status) || !fileNameInfo->Name.Buffer) {
        goto EXIT;
    }
    if (wcscmp(processImageName->Buffer, gPath) == 0) {
        for (int i = 0; gIgnoredDirs[i]; i++) {
            if (wcsstr(fileNameInfo->Name.Buffer, gIgnoredDirs[i]) == fileNameInfo->Name.Buffer) {
                goto EXIT;
            }
        }
        for (int i = 0; gIgnoredWrite[i]; i++) {
            if (wcsstr(fileNameInfo->Name.Buffer, gIgnoredWrite[i])) {
                goto EXIT;
            }
        }
        //KdPrint(("Minifilter: Process %ws invokes WRITE.", processImageName->Buffer));
        ULONG length = Data->Iopb->Parameters.Write.Length;
        LONGLONG offset = Data->Iopb->Parameters.Write.ByteOffset.QuadPart;
        KdPrint(("Minifilter: WRITE %lu BYTES TO %ws AT %lld by %ws\n", length, fileNameInfo->Name.Buffer, offset, processImageName->Buffer));
        PUCHAR buffer = (PUCHAR)Data->Iopb->Parameters.Write.WriteBuffer;

        SIZE_T key_len = strlen(gRc4Key);
        RC4_CONTEXT TBox = Rc4GetContext((CONST PVOID)gRc4Key, key_len);
        RC4Crypt(TBox, buffer, length, offset);

        retval = FLT_PREOP_SUCCESS_WITH_CALLBACK;
    }

EXIT:
    CompletionContext = NULL;
    if (processImageName)
        ExFreePool(processImageName);
    if (fileNameInfo)
        FltReleaseFileNameInformation(fileNameInfo);
    return retval;
}

void CopyFile(PCWSTR sourcePath, PCWSTR destinationPath)
{
    HANDLE sourceHandle = NULL;
    HANDLE destinationHandle = NULL;
    HANDLE infoFileHandle = NULL;
    OBJECT_ATTRIBUTES objectAttributes;
    IO_STATUS_BLOCK ioStatusBlock;
    NTSTATUS status;
    CHAR buffer[8192];
    //ULONG bytesRead;
    UNICODE_STRING usrcpath;
    UNICODE_STRING udstpath;
   

    // 初始化并打开源文件
    RtlInitUnicodeString(&usrcpath, sourcePath);
    InitializeObjectAttributes(&objectAttributes, &usrcpath, OBJ_KERNEL_HANDLE, NULL, NULL);
    //status = ZwCreateFile(&sourceHandle, GENERIC_READ, &objectAttributes, &ioStatusBlock, NULL, FILE_ATTRIBUTE_NORMAL, FILE_SHARE_VALID_FLAGS, FILE_OPEN, FILE_SYNCHRONOUS_IO_NONALERT, NULL, 0);

    IO_SECURITY_CONTEXT securityContext;
    securityContext.SecurityQos = NULL;
    securityContext.AccessState = NULL;
    securityContext.DesiredAccess = GENERIC_READ;
    securityContext.FullCreateOptions = 0;

    status = IoCreateFileEx(
        &sourceHandle,                          // FileHandle
        GENERIC_READ,                           // DesiredAccess
        &objectAttributes,                      // ObjectAttributes
        &ioStatusBlock,                         // IoStatusBlock
        NULL,                                   // AllocationSize (optional, NULL if not setting size)
        FILE_ATTRIBUTE_NORMAL,                  // FileAttributes
        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,  // ShareAccess
        FILE_OPEN,                              // Disposition
        FILE_SYNCHRONOUS_IO_NONALERT,           // CreateOptions
        NULL,                                   // EaBuffer (optional, NULL if no extended attributes)
        0,                                      // EaLength
        CreateFileTypeNone,                     // CreateFileType
        NULL,                                   // InternalParameters (optional)
        IO_NO_PARAMETER_CHECKING | IO_IGNORE_SHARE_ACCESS_CHECK, // Options
        NULL                                    // DriverContext (optional, provide if needed)
    );


    if (!NT_SUCCESS(status)) {
        KdPrint(("Minifilter: Failed to open source file %ws", sourcePath));
        KdPrint(("Minifilter: status %x", status));
        goto EXIT;
    }

    // 初始化并创建目标文件
    RtlInitUnicodeString(&udstpath, destinationPath);
    InitializeObjectAttributes(&objectAttributes, &udstpath, OBJ_KERNEL_HANDLE, NULL, NULL);
    status = ZwCreateFile(&destinationHandle, GENERIC_WRITE, &objectAttributes, &ioStatusBlock, NULL, FILE_ATTRIBUTE_NORMAL, 0, FILE_SUPERSEDE, FILE_SYNCHRONOUS_IO_NONALERT, NULL, 0);
    if (!NT_SUCCESS(status)) {
        KdPrint(("Minifilter: Failed to create target file %ws, status:0x%08X ", destinationPath, status));
        goto EXIT;
    }

    //// 读取源文件并写入到目标文件
    while (NT_SUCCESS(ZwReadFile(sourceHandle, NULL, NULL, NULL, &ioStatusBlock, buffer, sizeof(buffer), NULL, NULL))) {
        if (ioStatusBlock.Information == 0)
            break; // 文件末尾

        if (ioStatusBlock.Information <= ULONG_MAX) {
            ULONG bytesToWrite = (ULONG)ioStatusBlock.Information;
            status = ZwWriteFile(destinationHandle, NULL, NULL, NULL, &ioStatusBlock, buffer, bytesToWrite, NULL, NULL);
        }
        else {
            KdPrint(("Minifilter: The read size exceeds the maximum ULONG value, need to handle specially.\n"));
            goto EXIT;
        }
        if (!NT_SUCCESS(status)) {
            goto EXIT;
        }
    }

    // 构建信息文件路径
    UNICODE_STRING infoFileName;

    swprintf(infoFilePath, L"%ws.info", destinationPath);
    RtlInitUnicodeString(&infoFileName, infoFilePath);
    InitializeObjectAttributes(&objectAttributes, &infoFileName, OBJ_KERNEL_HANDLE, NULL, NULL);

    // 创建并写入信息文件
    status = ZwCreateFile(&infoFileHandle, GENERIC_WRITE, &objectAttributes, &ioStatusBlock, NULL, FILE_ATTRIBUTE_NORMAL, 0, FILE_CREATE, FILE_SYNCHRONOUS_IO_NONALERT, NULL, 0);
    if (NT_SUCCESS(status)) {
        // 写入源文件路径
        ULONG bytesToWrite = (ULONG)(wcslen(sourcePath) * sizeof(WCHAR));
        ZwWriteFile(infoFileHandle, NULL, NULL, NULL, &ioStatusBlock, (PVOID)sourcePath, bytesToWrite, NULL, NULL);
        ZwClose(infoFileHandle);
    }
    else {
        KdPrint(("Minifilter: Failed to create info file %ws", infoFilePath));
    }



EXIT:
    //释放资源
    if (sourceHandle != NULL) {
        ZwClose(sourceHandle);
        sourceHandle = NULL; // 设置为 NULL 避免悬挂引用
    }

    if (destinationHandle != NULL) {
        ZwClose(destinationHandle);
        destinationHandle = NULL; // 设置为 NULL 避免悬挂引用
    }
    if (infoFileHandle != NULL) {
        ZwClose(infoFileHandle);
        infoFileHandle = NULL;
    }
    return;
}

void AppendTimestampedFilename(PWCHAR basePath) {
    LARGE_INTEGER currentTime;
    TIME_FIELDS timeFields;
    WCHAR timestampFilename[64] = { 0 };  // 足够大，以存放生成的文件名

    // 获取当前系统时间
    // 获取当前系统时间
    KeQuerySystemTimePrecise(&currentTime);  // 使用更精确的时间获取函数
    ExSystemTimeToLocalTime(&currentTime, &currentTime);

    // 将 LARGE_INTEGER 时间转换为 TIME_FIELDS 结构
    RtlTimeToTimeFields(&currentTime, &timeFields);

    // 获取毫秒（从100纳秒间隔数中计算）
    ULONG milliseconds = (currentTime.QuadPart % 10000000) / 10000;

    // 格式化文件名为 YYYYMMDD-HHMMSS-FFF.bak，其中FFF是毫秒
    RtlStringCbPrintfW(timestampFilename, sizeof(timestampFilename),
        L"%04u%02u%02u-%02u%02u%02u-%03u.bak",
        timeFields.Year, timeFields.Month, timeFields.Day,
        timeFields.Hour, timeFields.Minute, timeFields.Second,
        milliseconds);

    // 将时间戳文件名追加到基本路径
    RtlStringCbCatW(basePath, 512 * sizeof(WCHAR), timestampFilename);
}

POSTOP_CALLBACK(PostWrite) {
    UNREFERENCED_PARAMETER(FltObjects);
    UNREFERENCED_PARAMETER(Flags);
    UNREFERENCED_PARAMETER(CompletionContext);
    PFLT_FILE_NAME_INFORMATION fileNameInfo = NULL;
    NTSTATUS status;
    status = FltGetFileNameInformation(Data, FLT_FILE_NAME_NORMALIZED | FLT_FILE_NAME_QUERY_DEFAULT, &fileNameInfo);
    if (!NT_SUCCESS(status)) {
        goto EXIT;
    }
    KdPrint(("Minifilter: Copy %ws\n", fileNameInfo->Name.Buffer));
    //PWCHAR target_path = L"\\Device\\HarddiskVolume3\\FsFilterDemotemp\\";
    WCHAR target_path[512];
    PWCHAR basePath = L"\\Device\\HarddiskVolume3\\FsFilterDemotemp\\";
    RtlStringCbCopyW(target_path, 512 * sizeof(WCHAR), basePath);
    AppendTimestampedFilename(target_path);
    KdPrint(("Minifilter: Targetpath %ws\n", target_path));
    CopyFile(fileNameInfo->Name.Buffer, target_path);

EXIT:
    if (fileNameInfo)
        FltReleaseFileNameInformation(fileNameInfo);
    return FLT_POSTOP_FINISHED_PROCESSING;
}