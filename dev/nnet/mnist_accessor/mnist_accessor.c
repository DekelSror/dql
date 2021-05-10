#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

#include "mnist_accessor.h"
#include "ufile.h"

#define mnist_accessor_thisify _mnist_accessor_t* this = _this;
#define mnist_accessor_thisify_const const _mnist_accessor_t* this = _this;

unsigned UintReverseChars(unsigned n) 
{

    unsigned i1 = (n & 0xff);
    unsigned i2 = ((n >> 8) & 0xff);
    unsigned i3 = ((n >> 16) & 0xff);
    unsigned i4 = ((n >> 24) & 0xff);

    return i1 << 24 | i2 << 16 | i3 << 8 | i4;
}

typedef struct
{
    unsigned _num_images;
    unsigned _num_rows;
    unsigned _num_cols;
    char* _images;
    char* _labels;
    ufile_t _labels_uf;
    ufile_t _images_uf;
} _mnist_accessor_t;

static mnist_accessor_t Create(void)
{
    return malloc(sizeof(_mnist_accessor_t));
}

static void Free(mnist_accessor_t _this)
{
    mnist_accessor_thisify
    UFile.free(&this->_labels_uf);
    UFile.free(&this->_images_uf);

    free(this);
}

static char GetLabel(const mnist_accessor_t _this, unsigned index)
{
    mnist_accessor_thisify_const
    if (index >= this->_num_images) return -1;

    return this->_labels[index];
}

static char* GetImage(const mnist_accessor_t _this, unsigned index)
{
    mnist_accessor_thisify_const
    if (index >= this->_num_images) return NULL;

    return this->_images + (784 * index);
}

static int LoadLabels(mnist_accessor_t _this, const char* file_path)
{
    mnist_accessor_thisify    
    int uf = UFile.create_st(&this->_labels_uf, file_path);

    if (uf != 0)
    {
        return 3;
    }

    unsigned magic_num = UintReverseChars(*(unsigned*)(this->_labels_uf._map));
    if (magic_num != 2049) 
    {
        UFile.free(&this->_labels_uf);

        return 1;
    }

    unsigned num_items = UintReverseChars(*(unsigned*)(this->_labels_uf._map + 4));
    if (num_items != 60000) 
    {
        UFile.free(&this->_labels_uf);
        
        return 2;
    }

    this->_labels = this->_labels_uf._map + 8;
    
    return 0;
}

static int LoadImages(mnist_accessor_t _this, const char* file_path)
{
    mnist_accessor_thisify
    int uf = UFile.create_st(&(this->_images_uf), file_path);

    if (uf != 0) return 5;

    unsigned magic_num = UintReverseChars(*(unsigned*)this->_images_uf._map);
    if (magic_num != 2051) 
    {
        UFile.free(&this->_images_uf);
        
        return 1;
    }

    unsigned num_items = UintReverseChars(*(unsigned*)(this->_images_uf._map + 4));
    if (num_items != 60000) 
    {
        UFile.free(&this->_images_uf);
        
        return 2;
    }

    unsigned num_rows = UintReverseChars(*(unsigned*)(this->_images_uf._map + 8));
    if (num_rows != 28) 
    {
        UFile.free(&this->_images_uf);
        
        return 3;
    }

    unsigned num_cols = UintReverseChars(*(unsigned*)(this->_images_uf._map + 12));
    if (num_cols != 28) 
    {
        UFile.free(&this->_images_uf);

        return 4;
    }

    this->_images = this->_images_uf._map + 16;
    this->_num_images = num_items;
    this->_num_rows = num_rows;
    this->_num_cols = num_cols;

    return 0;
}


const mnist_accessor_api_t Mnist = {Create, Free, LoadImages, LoadLabels, GetImage, GetLabel};
