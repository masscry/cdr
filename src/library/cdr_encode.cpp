#include <cdr.h>
#include "cdr_handle.h"

CDR_API int32_t cdr_encode(cdr_handle *handle, const void *data, size_t datasize) {
    if (handle != 0) {
        return handle->Encode(data, datasize);
    }
    return CDR_BAD_PTR;
}

int32_t cdr_handle::Encode(const void *buffer, size_t bufferlen) {
    try {

        cdr_deque result;
        obitstream<cdr_deque> coder(result);

        const uint8_t *incursor = (const uint8_t *) buffer;

        while (((incursor - (const uint8_t *) buffer) < bufferlen)) {
            hufftable &item = this->encoder[*incursor];
            coder.Push(item.value, item.len);
            ++incursor;
        }
        coder.Flush();

        this->coded.swap(result);
        return CDR_OK;

    } catch (std::bad_alloc &err) {
        cdr_print_error("%s", err.what());
        return CDR_NO_MEM;
    } catch (std::exception &err) {
        cdr_print_error("Unknown: %s", err.what());
        return CDR_UNKNOWN;
    }
}
