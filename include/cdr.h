/**
 * @file cdr.h
 * @author timur
 * @date 15:20 14.03.2016
 */

#pragma once
#ifndef CDR_PROJECT_CDR_H
#define CDR_PROJECT_CDR_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef _WIN32
#ifdef CDR_EXPORT_FUNCS
#define CDR_API __declspec(dllexport)
#else
#define CDR_API __declspec(dllimport)
#endif // CDR_EXPORT_FUNCS
#else
#define CDR_API
#endif

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
 * CDR library error codes
 */
typedef enum cdr_status {
    CDR_OK = 0, /// No error
    CDR_BAD_PTR, /// Bad pointer
    CDR_BAD_SIZE, /// Bad size
    CDR_NO_MEM, /// Not enough memory
    CDR_UNKNOWN, /// Unknown error
    CDR_NO_FREQ_TABLE, /// No prepared frequency table
    CDR_BAD_NODE, /// Bad node tree type
    CDR_NO_DECODER_TREE, /// No prepared decoder tree
    CDR_INVALID_TREE_TOP, /// Bad tree top
    CDR_PREFIX_TREE_TOO_LONG, /// Bad prefix tree length
    CDR_LAST_CODE
} cdr_status;

/**
 * CDR handle
 */
typedef struct cdr_handle cdr_handle;

/**
 * Create new CDR handle
 * @return new CDR handle
 */
CDR_API cdr_handle *cdr_new();

/**
 * Delete CDR handle
 * @param oldhandle CDR handle to delete
 */
CDR_API void cdr_delete(cdr_handle *oldhandle);

/**
 * Build new data model
 * @param handle
 * @param data data to build model
 * @param datasize size of data
 * @return cdr_status
 * @see cdr_status
 */
CDR_API int32_t cdr_new_model(cdr_handle *handle, const void *data, size_t datasize);


/**
 * Calculate entropy of data
 * @param data
 * @param datasize length of array
 */
CDR_API double cdr_entropy(const void *data, size_t datasize);


/**
 * Prepare huffman tree
 * @param handle
 * @return cdr_status
 */
CDR_API int32_t cdr_prepare_decoder(cdr_handle *handle);

/**
 * Prepare huffman map
 * @param handle
 * @return cdr_status
 */
CDR_API int32_t cdr_prepare_encoder(cdr_handle *handle);

/**
 * Encode data
 * @param handle
 * @param data data to encode
 * @param datasize sizeof data
 * @return cdr_status
 */
CDR_API int32_t cdr_encode(cdr_handle *handle, const void *data, size_t datasize);

/**
 * Return pointer to encoded array and its length
 * @param handle
 * @param size returns size of data
 * @return pointer to data
 */
CDR_API void *cdr_result(cdr_handle *handle, size_t *size);


/**
 * Encode data
 * @param handle
 * @param data data to encode
 * @param datasize sizeof data
 * @return cdr_status
 */
CDR_API int32_t cdr_encode(cdr_handle *handle, const void *data, size_t datasize);

/**
 * Decode data
 * @param handle
 * @param data
 * @param datasize size of data
 * @param expected number of bytes
 * @return cdr_status
 */
CDR_API int32_t cdr_decode(cdr_handle *handle, const void *data, size_t datasize, size_t outsize);

/**
 * Release allocated memory
 * @param handle
 * @param ptr
 */
CDR_API void cdr_release_result(void *ptr);



/**
 * Returns pointer to huffman tree
 * @param handle
 * @param treesize size of tree
 * @return serialized huffman tree
 */
CDR_API void *cdr_get_tree(cdr_handle *handle, size_t *treesize);

/**
 * Set handle to use defined tree
 * @param handle
 * @oaram tree pointer to tree
 * @param treesize size of tree
 * @return cdr_status
 */
CDR_API int32_t cdr_set_tree(cdr_handle *handle, const void *tree, size_t treesize);

/**
 * Get error text string
 * @param error CDR handle to get error
 * @return error string
 */
CDR_API const char *cdr_strerror(int32_t error);

CDR_API void cdr_print_model(cdr_handle *handle, FILE *output);
CDR_API void cdr_print_encoder(cdr_handle *handle, FILE *output);


#ifdef __cplusplus
}
#endif // __cplusplus

#endif //CDR_PROJECT_CDR_H
