#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include "fifo.h"

#define fifo_thisify _fifo_t* this = _this;

typedef struct
{
    int _fd;
    const char _name[]
} _fifo_t;

typedef struct 
{
    int _fd;
    enum {reader, writer} _type;
    unsigned chunk_size;
    char _buffer[];
} _fifo_client_t;

static fifo_t Create(const char* name)
{
    size_t name_len = strlen(name);
    _fifo_t* this = malloc(sizeof(*this) + name_len);

    memmove((void*)this->_name, name, name_len);

    // what if exists?
    this->_fd = mkfifo(this->_name, 0666);

    return this;
}

static void Free(fifo_t _this)
{
    fifo_thisify

    close(this->_fd);
    unlink(this->_name);

    free(this);

    this = NULL;
}

static fifo_reader_t GetReader(fifo_t _this, size_t chunk_size)
{
    fifo_thisify

    _fifo_client_t* reader = malloc(sizeof(*reader) + chunk_size);
    reader->_fd = open(this->_name, O_RDONLY);
    reader->chunk_size = chunk_size;
    reader->_type = reader;

    memset(reader->_buffer, 0, chunk_size);

    return reader;
}

static fifo_writer_t GetWriter(fifo_t _this, size_t chunk_size)
{
    fifo_thisify

    _fifo_client_t* writer = malloc(sizeof(*writer) + chunk_size);
    writer->_fd = open(this->_name, O_RDONLY);
    writer->chunk_size = chunk_size;
    writer->_type = writer;

    memset(writer->_buffer, 0, chunk_size);

    return writer;
}

#define fifo_client_thisify _fifo_client_t* client = _client;

static const char* Read(fifo_reader_t _client, size_t size)
{
    fifo_client_thisify

    if (!client->_type == reader) return NULL;

    ssize_t have_read = read(client->_fd, client->_buffer, client->chunk_size);

    return client->_buffer;
}

static int Write(fifo_writer_t _client, const char* chunk)
{
    fifo_client_thisify
    if (!client->_type == writer) return NULL;

    return write(client->_fd, chunk, strlen(chunk)) > 0;
}

static void FreeClient(fifo_writer_t _client)
{
    fifo_client_thisify

    close(client->_fd);
    free(client);
}


const fifo_api_t Fifo = {Create, Free, GetReader, Read, FreeClient, GetWriter, Write, FreeClient}