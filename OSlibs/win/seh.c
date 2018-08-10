#include <windows.h>

/// Performs a crash report and aborts.
void rust_seh_handler(DWORD exception_code);

/// Helps us test catching access violations happening in C code.
void c_access_violation()
{
    // Straight from the https://docs.microsoft.com/en-us/windows/desktop/Debug/using-a-vectored-exception-handler.
    char *ptr = 0;
    *ptr = 0;
}

long WINAPI veh_exception_filter(PEXCEPTION_POINTERS info)
{
    // https://docs.microsoft.com/en-us/windows/desktop/debug/using-a-vectored-exception-handler

#ifdef _AMD64_
    info->ContextRecord->Rip++;
#else
    info->ContextRecord->Eip++;
#endif

    rust_seh_handler(info->ExceptionRecord->ExceptionCode);

    return EXCEPTION_CONTINUE_EXECUTION;
}

void init_veh()
{
    AddVectoredExceptionHandler(1, &veh_exception_filter);
}
