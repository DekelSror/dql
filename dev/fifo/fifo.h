#ifndef dql_fifo_h
#define dql_fifo_h

typedef void* fifo_t;

typedef void* fifo_reader_t;
typedef void* fifo_writer_t;

typedef struct 
{
    fifo_t(*create)(const char* name);
    void(*free)(fifo_t);

    fifo_reader_t(*get_reader)(const char* name);
    char*(*read)(fifo_reader_t, size_t);
    void(*free_reader)(fifo_reader_t);

    fifo_writer_t(*get_writer)(const char* name);
    int(*write)(fifo_writer_t, const char*);
    void(*free_writer)(fifo_writer_t);
} fifo_api_t;


extern const fifo_api_t Fifo;



#endif // dql_fifo_h
