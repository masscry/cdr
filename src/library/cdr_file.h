/**
 * @file cdr_file.h
 */

#pragma once
#ifndef CDR_PROJECT_CDR_FILE_H
#define CDR_PROJECT_CDR_FILE_H

#include <cstdint>

#define CDR_FILE_MAGIC (0x6F526463)

enum cdr_flags {
    CDF_NONE = 0x0,
    CDF_HUFFMAN = 0x1
};

struct cdr_file {
    uint32_t magic;
    uint32_t flags;
    uint64_t tree_offset;
    uint64_t data_offset;
    uint8_t bytes[];
};

struct cdr_section {
    uint64_t elength;
    uint64_t tlength;
    uint8_t bytes[];
};

#endif //CDR_PROJECT_CDR_FILE_H
