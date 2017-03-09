//
// Created by timur on 14.03.16.
//

#include <new>
#include <iostream>
#include "cdr.h"
#include "cdr_handle.h"

CDR_API cdr_handle *cdr_new() {
    try {
        cdr_handle *result = new cdr_handle;
        return result;
    } catch (std::bad_alloc &err) {
        cdr_print_error("%s", err.what());
    }
    return 0;
}

CDR_API void cdr_delete(cdr_handle *oldhandle) {
    delete oldhandle;
}