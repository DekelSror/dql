#ifndef dql_generator_h


typedef struct { unsigned long _state; } generator_state_t;


#define PT_THREAD(name_args)  unsigned long name_args

#define PT_BEGIN(pt) switch(pt->_state) { case 0:

#define PT_WAIT_UNTIL(pt, c) pt->_state = __LINE__; case __LINE__: \
    if(!(c)) return 0

#define PT_END(pt) } pt->_state = 0; return 2

#define PT_INIT(pt) pt->_state = 0


#define dql_generator_h
#endif dql_generator_h



