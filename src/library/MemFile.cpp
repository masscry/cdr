//
// Created by timur on 10.03.16.
//

#include "MemFile.h"

#include <sys/mman.h>
#include <sys/stat.h>
#include <stdexcept>
#include <unistd.h>
#include <fcntl.h>

bool memfile::Open(int id, int om) {

    int protflag = 0;
    switch (om) {
        case OM_READ:
            protflag = PROT_READ;
            break;
        case OM_WRITE:
            protflag = PROT_WRITE;
            break;
        case (OM_READ | OM_WRITE):
            protflag = PROT_WRITE | PROT_READ;
            break;
        default:
            return false;
    }

    struct stat fst;

    if (fstat(id, &fst) != 0) {
        close(id);
        return false;
    }

    void *ptr = mmap(0, fst.st_size, protflag, MAP_SHARED, id, 0);

    if (ptr != MAP_FAILED) {
        if (this->Close()) {
            this->ptr = ptr;
            this->fd = id;
            this->len = fst.st_size;
            return true;
        }
        munmap(ptr, fst.st_size);
    }
    close(id);
    return false;
}

bool memfile::Open(const char *filename, int om) {

    int oflag = 0;
    int mode = 0;
    switch (om) {
        case OM_READ:
            oflag = O_RDONLY;
            break;
        case OM_WRITE:
            oflag = O_WRONLY | O_CREAT;
            mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;
            break;
        case (OM_READ | OM_WRITE):
            oflag = O_RDWR | O_CREAT;
            mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;
            break;
        default:
            return false;
    }

    int descr = open(filename, oflag, mode);

    if (descr == -1) {
        return false;
    }

    return this->Open(descr, om);
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
