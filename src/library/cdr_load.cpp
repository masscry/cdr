//
// Created by timur on 17.03.16.
//

#include <cstdio>
#include <cdr.h>
#include "cdr_handle.h"
#include "cdr_file.h"

CDR_API int32_t cdr_load(cdr_handle *handle, FILE *inp) {
    if ((handle != 0) && (inp != 0)) {
        return handle->Load(inp);
    }
    return CDR_BAD_PTR;
}

static char *ReadSection(FILE *inp, uint64_t offset, cdr_section *section) {
    if (section != 0) {
        long int oldpos = ftell(inp);

        if (fseek(inp, offset, SEEK_SET) != 0) {
            cdr_print_error("fseek: %d", ferror(inp));
            return 0;
        }

        if (fread(section, sizeof(cdr_section), 1, inp) != 1) {
            return 0;
        }

        char *temptree = (char *) malloc(section->elength);

        if (temptree == 0) {
            fseek(inp, oldpos, SEEK_SET);
            return 0;
        }

        if (fread(temptree, 1, section->elength, inp) != section->elength) {
            fseek(inp, oldpos, SEEK_SET);
            free(temptree);
            return 0;
        }

        fseek(inp, oldpos, SEEK_SET);
        return temptree;
    }
    return 0;
}


int32_t cdr_handle::Load(FILE *inp) {
    try {
        cdr_file header;
        if (fread(&header, sizeof(cdr_file), 1, inp) != 1) {
            return CDR_BAD_SIZE;
        }
        if (header.magic != CDR_FILE_MAGIC) {
            return CDR_BAD_MAGIC;
        }
        if (header.flags != CDF_HUFFMAN) {
            return CDR_UNKNOWN_FLAGS;
        }
        cdr_section tree;
        char *tree_data = ReadSection(inp, header.tree_offset, &tree);
        int32_t result = this->SetTree(tree_data, tree.elength);
        free(tree_data);
        if (result != CDR_OK) {
            return result;
        }
        cdr_section data;
        char *en_data = ReadSection(inp, header.data_offset, &data);
        result = this->Decode(en_data, data.elength, data.tlength);
        free(en_data);
        if (result != CDR_OK) {
            return result;
        }
        return CDR_OK;
    } catch (std::bad_alloc &err) {
        cdr_print_error("%s", err.what());
        return CDR_NO_MEM;
    } catch (std::exception &err) {
        cdr_print_error("Unknown: %s", err.what());
        return CDR_UNKNOWN;
    }
}
