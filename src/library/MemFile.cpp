//
// Created by timur on 10.03.16.
//

#include "MemFile.h"

#include <sys/mman.h>
#include <sys/stat.h>
#include <stdexcept>
#include <unistd.h>
#include <fcntl.h>

bool memfile::Open(const char *filename, int om) {

    int oflag = 0;
    int protflag = 0;
    int mode = 0;
    switch (om) {
        case OM_READ:
            oflag = O_RDONLY;
            protflag = PROT_READ;
            break;
        case OM_WRITE:
            oflag = O_WRONLY | O_CREAT;
            protflag = PROT_WRITE;
            mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;
            break;
        case (OM_READ | OM_WRITE):
            oflag = O_RDWR | O_CREAT;
            protflag = PROT_WRITE | PROT_READ;
            mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;
            break;
        default:
            return false;
    }

    struct stat fst;
    int descr = open(filename, oflag, mode);

    if (descr == -1) {
        return false;
    }

    if (fstat(descr, &fst) != 0) {
        close(descr);
        return false;
    }

    void *ptr = mmap(0, fst.st_size, protflag, MAP_SHARED, descr, 0);

    if (ptr != MAP_FAILED) {
        if (this->Close()) {
            this->ptr = ptr;
            this->fd = descr;
            this->len = fst.st_size;
            return true;
        }
        munmap(ptr, fst.st_size);
    }
    close(descr);
    return false;
}

bool memfile::Close() {
    bool result = true;
    if (this->ptr != 0) {
        if (munmap(this->ptr, this->len) != 0) {
            result = false;
        }
        if (close(this->fd) != 0) {
            result = false;
        }
        this->len = 0;
        this->fd = 0;
        this->ptr = 0;
    }
    return result;
}

void *memfile::Pointer() const {
    return this->ptr;
}

size_t memfile::Length() const {
    return this->len;
}


memfile::memfile() : fd(0), len(0), ptr(0) {
    ;
}

memfile::memfile(const char *filename, int om) : fd(0), len(0), ptr(0) {
    if (!this->Open(filename, om)) {
        throw std::runtime_error("Failed to open file");
    }
}


memfile::~memfile() {
    if (!this->Close()) {
        throw std::runtime_error("Failed to close file");
    }
}
