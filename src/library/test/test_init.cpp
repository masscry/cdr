#include <cdr.h>
#include <cutest.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include "../MemFile.h"

void TestNewDelete(CuTest *tc) {
    cdr_handle *h = 0;
    CuAssertPtrEquals(tc, 0, h);
    h = cdr_new();
    CuAssertPtrNotNull(tc, h);
    cdr_delete(h);
}

void TestErrorText(CuTest *tc) {
    for (int32_t err = CDR_OK; err < CDR_LAST_CODE; ++err) {
        CuAssert(tc, "Not all error codes defined", strcmp(cdr_strerror(err), "Undefined error") != 0);
    }
}

void TestWarAndPeaceEncode(CuTest *tc) {
    memfile mf;
    CuAssert(tc, "Must open file", mf.Open("book1.txt", OM_READ));

    double txt = cdr_entropy(mf.Pointer(), mf.Length());

    cdr_handle *h = cdr_new();
    CuAssertPtrNotNull(tc, h);
    CuAssertIntEquals(tc, CDR_NO_FREQ_TABLE, cdr_prepare_decoder(h));
    CuAssertIntEquals(tc, CDR_OK, cdr_new_model(h, mf.Pointer(), mf.Length()));
    CuAssertIntEquals(tc, CDR_NO_DECODER_TREE, cdr_prepare_encoder(h));
    CuAssertIntEquals(tc, CDR_OK, cdr_prepare_decoder(h));
    CuAssertIntEquals(tc, CDR_OK, cdr_prepare_encoder(h));

    CuAssertIntEquals(tc, CDR_OK, cdr_encode(h, mf.Pointer(), mf.Length()));

    size_t size = 0;
    void *result = cdr_result(h, &size);

    CuAssertPtrNotNull(tc, result);
    CuAssert(tc, "Size must longer than 0", size > 0);

    FILE *outp = fopen("book1.cdr", "w");
    CuAssertPtrNotNull(tc, outp);
    CuAssertIntEquals(tc, size, fwrite(result, 1, size, outp));
    fclose(outp);

    double cdr = cdr_entropy(result, size);

    CuAssert(tc, "Entropy must rise", cdr > txt);

    CuAssertIntEquals(tc, CDR_OK, cdr_decode(h, result, size, mf.Length()));
    cdr_release_result(result);

    result = cdr_result(h, &size);

    outp = fopen("book1-w.txt", "w");
    CuAssertPtrNotNull(tc, outp);
    CuAssertIntEquals(tc, size, fwrite(result, 1, size, outp));
    fclose(outp);

    CuAssertPtrNotNull(tc, result);
    CuAssertIntEquals(tc, mf.Length(), size);

    CuAssertIntEquals(tc, 0, memcmp(mf.Pointer(), result, mf.Length()));

    cdr_release_result(result);

    cdr_delete(h);
}

void TestSaveLoadModel(CuTest *tc) {

    memfile mf;
    CuAssert(tc, "Must open file", mf.Open("book1.txt", OM_READ));

    cdr_handle *h = cdr_new();
    CuAssertPtrNotNull(tc, h);

    CuAssertIntEquals(tc, CDR_OK, cdr_new_model(h, mf.Pointer(), mf.Length()));
    CuAssertIntEquals(tc, CDR_OK, cdr_prepare_decoder(h));
    CuAssertIntEquals(tc, CDR_OK, cdr_prepare_encoder(h));

    CuAssertIntEquals(tc, CDR_OK, cdr_encode(h, mf.Pointer(), mf.Length()));

    size_t size = 0;
    void *result = cdr_result(h, &size);

    CuAssertPtrNotNull(tc, result);
    CuAssert(tc, "Size must longer than 0", size > 0);

    void *tree = 0;
    size_t treesize = 0;

    tree = cdr_get_tree(h, &treesize);

    CuAssertPtrNotNull(tc, tree);

    FILE *outp = fopen("book1.tre", "w");
    CuAssertPtrNotNull(tc, outp);
    CuAssertIntEquals(tc, treesize, fwrite(tree, 1, treesize, outp));
    fclose(outp);

    CuAssertIntEquals(tc, CDR_OK, cdr_set_tree(h, tree, treesize));

    cdr_release_result(tree);

    CuAssertIntEquals(tc, CDR_OK, cdr_decode(h, result, size, mf.Length()));

    result = cdr_result(h, &size);

    outp = fopen("book1-w.txt", "w");
    CuAssertPtrNotNull(tc, outp);
    CuAssertIntEquals(tc, size, fwrite(result, 1, size, outp));
    fclose(outp);

    CuAssertPtrNotNull(tc, result);
    CuAssertIntEquals(tc, mf.Length(), size);

    CuAssertIntEquals(tc, 0, memcmp(mf.Pointer(), result, mf.Length()));

    cdr_release_result(result);


    cdr_delete(h);
}

CuSuite *RegisterTest() {
    CuSuite *suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, TestNewDelete);
    SUITE_ADD_TEST(suite, TestErrorText);
    SUITE_ADD_TEST(suite, TestWarAndPeaceEncode);
    SUITE_ADD_TEST(suite, TestSaveLoadModel);
    return suite;
}

int main(int argc, char *argv[]) {
    CuSuite *root = CuSuiteNew();
    CuString *str = CuStringNew();

    CuSuiteAddSuite(root, RegisterTest());

    CuSuiteRun(root);

    CuSuiteSummary(root, str);
    CuSuiteDetails(root, str);

    printf("%s", str->buffer);

    CuStringDelete(str);
    CuSuiteDelete(root);
    return 0;
}