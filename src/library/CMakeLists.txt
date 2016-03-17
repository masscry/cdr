project(cdr)

set(CDR_SOURCE
        cdr_error.cpp
        cdr_new_delete.cpp
        cdr_model.cpp
        MemFile.cpp
        cdr_prepare_decoder.cpp
        cdr_prepare_encoder.cpp
        cdr_encode.cpp cdr_decode.cpp
        cdr_get_result.cpp
        cdr_entropy.cpp
        cdr_get_tree.cpp cdr_set_tree.cpp
        cdr_save.cpp
        cdr_load.cpp
        )

set(CDR_HEADER
        ../../include/cdr.h
        cdr_handle.h
        MemFile.h
        cdr_file.h
        )


add_library(${PROJECT_NAME} SHARED ${CDR_SOURCE} ${CDR_HEADER})

add_subdirectory(test)