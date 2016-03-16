#include <cdr.h>
#include "cdr_handle.h"
#include <cmath>

CDR_API double cdr_entropy(const void *data, size_t datasize) {
    uint32_t freq[CDR_BYTE_TABLE_SIZE] = {0};
    uint64_t total = 0;

    const unsigned char *cursor = (const unsigned char *) data;
    while (datasize-- > 0) {
        ++freq[*cursor++];
        ++total;
    }
    double result = 0;
    for (int i = 0; i < CDR_BYTE_TABLE_SIZE; ++i) {
        if (freq[i] == 0) {
            continue;
        }
        double prob = (double) freq[i] / (double) total;
        result -= prob * log(prob) / log(CDR_BYTE_TABLE_SIZE);
    }
    return result;
}
