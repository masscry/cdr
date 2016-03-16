#include <queue>
#include <cdr.h>
#include "cdr_handle.h"

CDR_API int32_t cdr_prepare_decoder(cdr_handle *handle) {
    if (handle) {
        return handle->PrepareDecoder();
    }
    return CDR_BAD_PTR;
}

class compare_freq_entry_pointer {
public:
    bool operator()(const freqList::iterator &a, const freqList::iterator &b) {
        return a->freq > b->freq;
    }
};

int32_t cdr_handle::PrepareDecoder() {

    if (this->table.empty()) {
        return CDR_NO_FREQ_TABLE;
    }

    try {

        freqList copies;
        std::priority_queue<freqList::iterator,
                std::vector<freqList::iterator>, compare_freq_entry_pointer> pq;

        copies.assign(this->table.begin(), this->table.end());

        for (auto item = copies.begin(), e = copies.end(); item != e; ++item) {
            item->child[0] = copies.end();
            item->child[1] = copies.end();
            pq.push(item);
        }

        while (pq.size() > 1) {
            auto a = pq.top();
            pq.pop();
            auto b = pq.top();
            pq.pop();

            freq_entry temp;
            temp.freq = a->freq + b->freq;
            temp.byte = 0;
            temp.fet = FET_BRANCH;
            temp.child[0] = a;
            temp.child[1] = b;

            copies.push_back(temp);
            pq.push(std::prev(copies.end()));
        }

        this->decoder.swap(copies);
        this->top = pq.top();
        return CDR_OK;

    } catch (std::bad_alloc &err) {
        cdr_print_error("%s", err.what());
        return CDR_NO_MEM;
    } catch (std::exception &err) {
        cdr_print_error("Unknown: %s", err.what());
        return CDR_UNKNOWN;
    }

}
