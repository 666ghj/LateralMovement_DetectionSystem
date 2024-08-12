#include "global.h"

PFLT_FILTER gFilterHandle = NULL;
PWCHAR gIgnoredDirs[] = {
    L"\\Device\\HarddiskVolume3\\Windows\\",
    L"\\Device\\HarddiskVolume3\\Program Files",
    L"\\Device\\HarddiskVolume3\\Users\\xjy\\AppData\\",
    NULL
};
PWCHAR gIgnoredRead[] = {
    L".ini",
    NULL
};
PWCHAR gIgnoredWrite[] = {
    L".bak",
    NULL
};
