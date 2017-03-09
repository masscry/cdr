//
// Created by timur on 10.03.16.
//

#ifndef RANGE_CODER_MEMFILE_H
#define RANGE_CODER_MEMFILE_H


#include <cstdlib>

enum open_mode {
    OM_READ = 0x1,
    OM_WRITE = 0x2
};

class memfile {
    int fd;
    size_t len;
    void *ptr;

    memfile(const memfile &mfile);

public:

    bool Open(const char *filename, int om);

    bool Open(int id, int om);

    bool Close();

    void *Pointer() const;

    size_t Length() const;

    memfile();

    memfile(const char *filename, int om);

    virtual ~memfile();
};


#endif //RANGE_CODER_MEMFILE_H
