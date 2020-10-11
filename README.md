

**write STEX using dql**
- allocations - strings, list, hash... 
    * no good for time constraints
    * add void*(*alloc)(size_t) to ctors
- data structures
    * offers - heap - implement removal by (?) for offer cancellation
    * deals - queue - implement FSQ with custom alloc


engine 
- listen on mqueue for arrivals
- whenever a new offer arrives, match stock
- when there aren't any mathces - 
    * resolve deals
    * modify / add / remove offers from market as required

expose current_value api for offer generator
whenever a deal resolves, the deal's price is set as the current values for the stock traded.

separate tcp server 
that pulls from another mqueue...







