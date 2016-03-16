//
// Created by timur on 16.03.16.
//

#include <cdr.h>
#include "cdr_handle.h"

CDR_API int32_t cdr_decode(cdr_handle *handle, const void *data, size_t datasize, size_t outsize) {
    if (handle != 0) {
        return handle->Decode(data, datasize, outsize);
    }
    return CDR_BAD_PTR;
}

int32_t cdr_handle::Decode(const void *buffer, size_t bufferlen, size_t outlen) {
    try {
        const uint8_t *incursor = (const uint8_t *) buffer;
        cdr_deque result;

        int32_t bitcursor = 7;
        while (((incursor - (const uint8_t *) buffer) < bufferlen) && (outlen > 0)) {

            auto pos = this->top;

            while (pos->fet == FET_BRANCH) {
                pos = pos->child[((0x1 << bitcursor) & (*incursor)) != 0];
                --bitcursor;
                if (bitcursor < 0) {
                    bitcursor = 7;
                    ++incursor;
                }
            }
            --outlen;
            result.push_back(pos->byte);
        }
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
