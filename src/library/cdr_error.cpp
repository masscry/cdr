#include "cdr_handle.h"
#include <cstdarg>

static const char *errstr[CDR_LAST_CODE + 1] = {
        "No error",
        "Bad pointer",
        "Bad size",
        "No memory",
        "Unknown",
        "No prepared frequency table",
        "Bad node tree type",
        "No decoder tree",
        "Bad decoder top",
        "Prefix tree too long (>32 bits)",
        "Bad file magic",
        "Unknown flags",
        "Bad write",
        0
};

CDR_API const char *cdr_strerror(int32_t error) {
    if ((error < CDR_LAST_CODE) && (error >= 0)) {
        if (errstr[error] != 0) {
            return errstr[error];
        }
    }
    return "Undefined error";
}

void cdr_print_error(const char *format, ...) {
    va_list vl;
    printf("%s: ", "CDR");
    va_start(vl, format);
    vprintf(format, vl);
    va_end(vl);
    printf("\n");
}
