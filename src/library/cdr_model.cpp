#include <cdr.h>
#include <algorithm>
#include <vector>
#include "cdr_handle.h"


CDR_API int32_t cdr_new_model(cdr_handle *handle, const void *data, size_t datasize) {
    if (handle) {
        return handle->BuildModel(data, datasize);
    }
    return CDR_BAD_PTR;
}

CDR_API void cdr_print_model(cdr_handle *handle, FILE *output) {
    handle->PrintModel(output);
}


class compare_freq_entry {
public:
    bool operator()(const freq_entry &a, const freq_entry &b) {
        return a.freq < b.freq;
    }
} cfe;

int32_t cdr_handle::BuildModel(const void *buffer, size_t bufferlen) {

    if (buffer == 0) {
        return CDR_BAD_PTR;
    }

    if (bufferlen == 0) {
        return CDR_BAD_SIZE;
    }

    try {
        uint32_t freq[CDR_BYTE_TABLE_SIZE] = {0};
        const unsigned char *cursor = (const unsigned char *) buffer;
        for (size_t i = 0; i < bufferlen; ++i) {
            ++freq[cursor[i]];
        }

        uint64_t max = 0;
        for (size_t i = 0; i < CDR_BYTE_TABLE_SIZE; ++i) {
            max += freq[i];
        }

        std::vector<freq_entry> result;

        for (size_t i = 0; i < CDR_BYTE_TABLE_SIZE; ++i) {
            freq_entry temp;
            if (freq[i] != 0) {
                temp.freq = freq[i];
                temp.byte = (unsigned char) i;
                temp.fet = FET_LEAF;
                result.push_back(temp);
            }
        }

        std::sort(result.begin(), result.end(), cfe);

        this->table.swap(result);

        return CDR_OK;
    } catch (std::bad_alloc &err) {
        cdr_print_error("%s", err.what());
        return CDR_NO_MEM;
    } catch (std::exception &err) {
        cdr_print_error("Unknown: %s", err.what());
        return CDR_UNKNOWN;
    }
}

void cdr_handle::PrintModel(FILE *outp) {
    for (auto i = this->table.begin(), e = this->table.end(); i != e; ++i) {
        fprintf(outp, "0x%X: %d\n", i->byte, i->freq);
    }
}