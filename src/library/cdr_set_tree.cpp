//
// Created by timur on 16.03.16.
//

#include <cdr.h>
#include "cdr_handle.h"

CDR_API int32_t cdr_set_tree(cdr_handle *handle, const void *tree, size_t treesize) {
    if (handle != 0) {
        return handle->SetTree(tree, treesize);
    }
    return CDR_BAD_PTR;
}

freqList::iterator RecursiveSetTree(freqList &list, ibitstream &ddr) {
    freq_entry temp;
    if (ddr.Pop(1) != 0) {
        temp.fet = FET_LEAF;
        temp.byte = ddr.Pop(8);
        temp.freq = 0;
        temp.child[0] = list.end();
        temp.child[1] = list.end();

        list.push_back(temp);
    }
    else {
        temp.fet = FET_BRANCH;
        temp.freq = 0;
        temp.byte = 255;
        temp.child[0] = RecursiveSetTree(list, ddr);
        temp.child[1] = RecursiveSetTree(list, ddr);
        list.push_back(temp);
    }
    return std::prev(list.end());
}

int32_t cdr_handle::SetTree(const void *tree, size_t treesize) {

    try {
        ibitstream ddr(tree, treesize);
        freqList result;
        freqList::iterator top = RecursiveSetTree(result, ddr);

        this->decoder.swap(result);
        this->top = top;
        return CDR_OK;
    } catch (std::bad_alloc &err) {
        cdr_print_error("%s", err.what());
        return CDR_NO_MEM;
    } catch (std::exception &err) {
        cdr_print_error("Unknown: %s", err.what());
        return CDR_UNKNOWN;
    }

}