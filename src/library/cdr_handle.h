//
// Created by timur on 14.03.16.
//

#ifndef CDR_PROJECT_CDR_HANDLE_H
#define CDR_PROJECT_CDR_HANDLE_H
#pragma once

#include <cstdint>
#include <list>
#include <vector>
#include <unordered_map>
#include <deque>
#include <iostream>

#include <cdr.h>
#include <bitset>
#include <iomanip>

#define CDR_MAX_BIT_LEN (32)
#define CDR_BYTE_TABLE_SIZE (256)

void cdr_print_error(const char *format, ...);

enum freq_entry_type {
    FET_BRANCH,
    FET_LEAF,
};

struct freq_entry {
    uint32_t freq;
    unsigned char byte;
    int fet;
    std::list<freq_entry>::iterator child[2];
};

typedef std::list<freq_entry> freqList;

struct hufftable {
    uint32_t value;
    uint32_t len;
};

typedef std::unordered_map<unsigned short, hufftable> huffMap;
typedef std::deque<uint8_t> cdr_deque;

struct cdr_handle {
private:
    std::vector<freq_entry> table;
    freqList decoder;
    freqList::iterator top;
    huffMap encoder;
    cdr_deque coded;


    cdr_handle(const cdr_handle &copy);

public:
    int32_t BuildModel(const void *buffer, size_t bufferlen);

    int32_t PrepareDecoder();

    int32_t PrepareEncoder();

    int32_t Encode(const void *buffer, size_t bufferlen);

    int32_t Decode(const void *buffer, size_t bufferlen, size_t outlen);

    void *GetTree(size_t *treesize);

    int32_t SetTree(const void *tree, size_t treesize);

    void PrintModel(FILE *outp);

    void PrintEncoder(FILE *outp);

    void *Result(size_t *size);

    cdr_handle() : table(), decoder(), top(decoder.end()), encoder(), coded() {
        ;
    }

    ~cdr_handle() {
        ;
    }

};

template<typename T>
class obitstream {
    T &outp;
    uint32_t len;
    uint8_t mem;

    obitstream(const obitstream &copy);

public:

    void Push(uint32_t byte, uint32_t bytelen) {
        while (bytelen > 0) {
            //                            4            6
            uint32_t shift = std::min((8 - this->len), bytelen);

            // AAAA0000 BBBBBB
            this->mem = this->mem << shift;

            // AAAABBBB BBBBBB
            this->mem = this->mem | (byte >> (bytelen - shift));

            // AAAABBBB 0000BB
            byte = byte & ((1 << (bytelen - shift)) - 1);
            bytelen -= shift;
            this->len += shift;
            if (this->len == 8) {
                this->outp.push_back(mem);
                this->mem = 0;
                this->len = 0;
            }
        }
    }

    void Flush() {
        if (this->len > 0) {
            this->outp.push_back(this->mem << (8 - this->len));
            this->len = 0;
            this->mem = 0;
        }
    }


    obitstream(T &outp) : outp(outp), mem(0), len(0) {
        ;
    }

    ~obitstream() {
        Flush();
    }
};

class ibitstream {
    const char *data;
    size_t datalen;

    uint32_t len;
    uint8_t mem;

    ibitstream(const ibitstream &copy);

public:

    uint32_t Pop(uint32_t bytelen) {

        uint32_t result = 0;
        while (bytelen > 0) {
            if (this->len == 0) {
                this->mem = *(const uint8_t *) data;
                this->len = 8;
                this->data++;
                this->datalen--;
            }

            uint32_t shift = std::min(len, bytelen);
            result = result << shift;
            result = result | (mem >> (len - shift));
            
            mem = mem & ((1 << (len - shift)) - 1);
            bytelen -= shift;
            this->len -= shift;
        }

        return result;
    }

    ibitstream(const void *data, size_t datalen) : data((const char *) data), datalen(datalen), len(0), mem(0) {
        ;
    }

    ~ibitstream() {
        ;
    }

};


#endif //CDR_PROJECT_CDR_HANDLE_H
