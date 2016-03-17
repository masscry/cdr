//
// Created by timur on 17.03.16.
//

#include <cstdio>
#include <cdr.h>
#include "cdr_handle.h"
#include "cdr_file.h"

CDR_API int32_t cdr_save(cdr_handle *handle, FILE *outp) {
    if ((handle != 0) && (outp != 0)) {
        return handle->Save(outp);
    }
    return CDR_BAD_PTR;
}

size_t cdr_handle::SourceLength() const {
    return this->encodedlen;
}

int32_t cdr_handle::Save(FILE *outp) {
    try {
        size_t tree_size = 0;
        void *tree_data = this->GetTree(&tree_size);
        if (tree_data == 0) {
            return CDR_NO_DECODER_TREE;
        }

        cdr_section tree;
        tree.tlength = tree_size;
        tree.elength = tree_size;

        size_t data_size = 0;
        void *data_data = this->Result(&data_size);
        if (data_data == 0) {
            return CDR_NO_MEM;
        }

        cdr_section data;
        data.elength = data_size;
        data.tlength = this->SourceLength();

        cdr_file header;
        header.magic = CDR_FILE_MAGIC;
        header.flags = CDF_HUFFMAN;
        header.tree_offset = sizeof(cdr_file);
        header.data_offset = sizeof(cdr_file) + sizeof(cdr_section) + tree_size;

        if (fwrite(&header, sizeof(cdr_file), 1, outp) != 1) {
            cdr_release(tree_data);
            cdr_release(data_data);
            return CDR_BAD_FILE_WRITE;
        }

        if (fwrite(&tree, sizeof(cdr_section), 1, outp) != 1) {
            cdr_release(tree_data);
            cdr_release(data_data);
            return CDR_BAD_FILE_WRITE;
        }

        if (fwrite(tree_data, 1, tree_size, outp) != tree_size) {
            cdr_release(tree_data);
            cdr_release(data_data);
            return CDR_BAD_FILE_WRITE;
        }

        if (fwrite(&data, sizeof(cdr_section), 1, outp) != 1) {
            cdr_release(tree_data);
            cdr_release(data_data);
            return CDR_BAD_FILE_WRITE;
        }

        if (fwrite(data_data, 1, data_size, outp) != data_size) {
            cdr_release(tree_data);
            cdr_release(data_data);
            return CDR_BAD_FILE_WRITE;
        }

        cdr_release(tree_data);
        cdr_release(data_data);
        return CDR_OK;
    } catch (std::bad_alloc &err) {
        cdr_print_error("%s", err.what());
        return CDR_NO_MEM;
    } catch (std::exception &err) {
        cdr_print_error("Unknown: %s", err.what());
        return CDR_UNKNOWN;
    }

}
