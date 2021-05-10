#ifndef dql_mnist_accessor_h
#define dql_mnist_accessor_h


typedef void* mnist_accessor_t;

typedef struct
{
    mnist_accessor_t(*create)();
    void(*free)(mnist_accessor_t mset);
    int(*load_images)(mnist_accessor_t mset, const char* file_path);
    int(*load_labels)(mnist_accessor_t mset, const char* file_path);
    char*(*get_image)(const mnist_accessor_t mset, unsigned index);
    char(*get_label)(const mnist_accessor_t mset, unsigned index);
} mnist_accessor_api_t;

extern const mnist_accessor_api_t Mnist;

#endif // dql_mnist_accessor_h
