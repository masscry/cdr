#include <cdr.h>
#include "cdr_handle.h"

CDR_API void *cdr_result(cdr_handle *handle, size_t *size) {
    if (handle != 0) {
        return handle->Result(size);
    }
    return 0;
}


CDR_API void cdr_release_result(void *ptr) {
    free(ptr);
}


void *cdr_handle::Result(size_t *size) {
    if (size == 0) {
        return 0;
    }
    try {
        char *buffer = (char *) malloc(this->coded.size());
        std::copy(this->coded.begin(), this->coded.end(), buffer);
        *size = this->coded.size();
        return buffer;
    } catch (std::bad_alloc &err) {
        cdr_print_error("%s", err.what());
    } catch (std::exception &err) {
        cdr_print_error("Unknown: %s", err.what());
    }
    return 0;
}
