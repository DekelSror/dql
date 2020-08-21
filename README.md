## missing tests - queue, thread pool, vlist, btree, hash


**framework**
- async operations and async I/O via Task(function, arg, collector)
    * global thread pool accepts tasks to a thread safe queue
    * event dispatches, callbacks
    * await for I/O - index read from disk for query, modify a table on write etc.
- events - add, listen, trigger 
    * a queue of dispatch functions 
    * poll ?
    * once, interval, 
    * event data - time ...
- string management
- network abstractions

**data structures**
- hash map - unique key-value associative array
    * hash any key as a string
    * 
- dlist - sequential, dynamic, double ended 
    * can be used as a queue, stack, array
    * tradeoff - remove is memmove 
    * uses - to parse queries and commands into segments. Task quques. Event / message queues.
- btree - sorted data structure
    * uses - serielizable table indexes. Load from and write to disk. 
    * enables - range queries. Pre sorted by any key. Quick lookup, deletion, addition


new approach to string class - 

framework has global string set with RC.
Strings are mapped in a hash to their RC.
String.get(s) increments RC.
API allows to free by key - String.free("smallak") decrements RC
Can be built on top of exising string_class?

*just thoughts*
- memory layer for modules - abstract malloc away. 

