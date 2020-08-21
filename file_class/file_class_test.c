#include <stdio.h>

#include "file_class.h"
#include "string_class.h"

static size_t errors = 0;

static void CreateSuccessWhenNoFile(void)
{
    file_t file = File.create("dummy");

    if (NULL == file) ++errors;
}

static void CreateFailsWhenFileExists(void)
{
    file_t file = File.create("dummy");
    if (NULL != file) ++errors;
}

static void ReadFailsOnEmptyFile(void)
{
    file_t file = File.create("dummy_empty");

    char* text = File.read(file, 10);

    if (NULL != text) ++errors;
}


static const void(*tests[])(void) = 
{    
    CreateSuccessWhenNoFile,
    CreateFailsWhenFileExists,
    ReadFailsOnEmptyFile
};

static const char* test_names[] = 
{
    "CreateSuccessWhenNoFile",
    "CreateFailsWhenFileExists",
    "ReadFailsOnEmptyFile"
};


int main(void)
{

    for (size_t i = 0; i < (sizeof(tests) / sizeof(tests[0])); i++)
    {
        tests[i]();
        printf("errors for %s: %lu\n", test_names[i], errors);

        errors = 0;
    }

    
    return 0;
}
