
**separation**

1. the framework / modules (tested and in the format mkmodule.sh generates)
    - *./init.sh* then *make* to have it all built (objs, tests ELFs and the libdql.so)
    - take the .so file and the _include folder wherever you wish. Use *make clean* to remove the objs adn tests
2. projects depending on the symbols in the dql lib 
    - STEX - very protorype stage with minimal deals loop and a random order producer
    - db challenge - evrything is high level in db modules, meaning there are no pointers
    - first stage - no persistency, create entire db in each test run
    



