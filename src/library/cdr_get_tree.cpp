#include <cdr.h>
#include "cdr_handle.h"

CDR_API void *cdr_get_tree(cdr_handle *handle, size_t *treesize) {
    if ((handle != 0) && (treesize != 0)) {
        return handle->GetTree(treesize);
    }
    return 0;
}

void RecursiveGetTree(obitstream<cdr_deque> &cdr, freqList::iterator head) {
    switch (head->fet) {
        case FET_BRANCH:
            cdr.Push(0x0, 0x1);
            RecursiveGetTree(cdr, head->child[0]);
            RecursiveGetTree(cdr, head->child[1]);
            break;
        case FET_LEAF:
            cdr.Push(0x1, 0x1);
            cdr.Push(head->byte, 8);
            break;
    }
}

void *cdr_handle::GetTree(size_t *treesize) {
    try {
        cdr_deque result;
        obitstream<cdr_deque> coder(result);
        RecursiveGetTree(coder, this->top);
        coder.Flush();
        std::cout << std::endl;

        uint8_t *out = (uint8_t *) malloc(result.size() * sizeof(uint8_t));
        if (out != 0) {
            std::copy(result.begin(), result.end(), out);
            *treesize = result.size() * sizeof(uint8_t);
            return out;
        }
    } catch (std::bad_alloc &err) {
        cdr_print_error("%s", err.what());
    } catch (std::exception &err) {
        cdr_print_error("Unknown: %s", err.what());
    }
    return 0;
}
