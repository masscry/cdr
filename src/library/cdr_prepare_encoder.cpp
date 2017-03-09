#include <cdr.h>
#include <iomanip>
#include <iostream>
#include <bitset>
#include "cdr_handle.h"


CDR_API int32_t cdr_prepare_encoder(cdr_handle *handle) {
    if (handle != 0) {
        return handle->PrepareEncoder();
    }
    return CDR_BAD_PTR;
}

uint32_t RecursiveBuildHuffTree(uint32_t bits, uint32_t level, freqList::iterator head, huffMap *result) {
    uint32_t maxlv[2] = {0, 0};
    switch (head->fet) {
        case FET_BRANCH:
            maxlv[0] = RecursiveBuildHuffTree((bits << 0x1) | 0x0, level + 1, head->child[0], result);
            maxlv[1] = RecursiveBuildHuffTree((bits << 0x1) | 0x1, level + 1, head->child[1], result);
            return (maxlv[0] > maxlv[1]) ? maxlv[0] : maxlv[1];
        case FET_LEAF: {
            hufftable temp;
            temp.value = bits;
            temp.len = level;
            (*result)[head->byte] = temp;
            return level;
        }
        default:
            throw std::runtime_error("Invalid node type");
    }
}

int32_t cdr_handle::PrepareEncoder() {

    if (this->decoder.empty()) {
        return CDR_NO_DECODER_TREE;
    }

    if (this->top == this->decoder.end()) {
        return CDR_INVALID_TREE_TOP;
    }

    try {
        huffMap copy;
        uint32_t maxlen = RecursiveBuildHuffTree(0, 0, this->top, &copy);

        if (maxlen <= CDR_MAX_BIT_LEN) {
            std::swap(copy, this->encoder);
            return CDR_OK;
        }
        return CDR_PREFIX_TREE_TOO_LONG;
    } catch (std::bad_alloc &err) {
        cdr_print_error("%s", err.what());
        return CDR_NO_MEM;
    } catch (std::runtime_error &err) {
        cdr_print_error("%s", err.what());
        return CDR_BAD_NODE;
    } catch (std::exception &err) {
        cdr_print_error("Unknown: %s", err.what());
        return CDR_UNKNOWN;
    }
}
