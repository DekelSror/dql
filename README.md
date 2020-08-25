

**framework**
- managed strings
- async tasks
- write test for events (figure out what are they and are they needed)

**service**
- goal is to have two types of services
    + one that sits on a FIFO, waiting for writes. this will be used as a writing buffer for a db client
    + could also be a reader waiting for query input
    + one that communicates via protocol (HTTP). this will be the db client itself

**on-disk structures**
- stock exchange
    + time, stock id, current value
    + write to log on disk
    + create sorted tables
    + 
