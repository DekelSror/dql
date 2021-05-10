

// rule of set - 4 chars input, label is input[0] ^ input[2]

// expectation - decay of input[1] and input[3] 

static const char data[] = {
    0, 0, 0, 1, 
    0, 1, 0, 1, 
    0, 1, 1, 1, 
    0, 0, 1, 1, 
    1, 0, 0, 1, 
    1, 0, 0, 0, 
    1, 0, 1, 0, 
    1, 1, 1, 0, 
};

static const char labels[] = {0, 0, 1, 1, 1, 1, 0, 0};

static const char* GetItem(unsigned index)
{
    if (index > 8) return NULL;
    return data + index * 4;
}

static char GetLabel(unsigned index)
{
    if (index > 8) return -1;
    return labels[index];
}

static unsigned SetSize(void) {return 8;}


typedef struct 
{
    const char*(*get_item)(unsigned index);
    char(*get_label)(unsigned index);
    unsigned(*set_size)(void);
} xorset_accessor_t;

const xorset_accessor_t XorSet = {GetItem, GetLabel, SetSize};