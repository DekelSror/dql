#include <stdio.h>
#include <unistd.h>


// #include "framework.h"
#include "event.h"


static size_t counter = 0; 


static void CounterTrigger1(void)
{
    printf("I am the first listener\n");
}

static void CounterTrigger2(void)
{
    printf("I am the second listener\n");
}

int main(void)
{
//     FrameworkInit();

//     // basic test - add an event and listen to it, then trigger it
//     void* test = EventRegister("counter is 1");

//     void* listener1 = EventAddListener(test, CounterTrigger1);
//     void* listener2 = EventAddListener(test, CounterTrigger2);

//     for (;counter < 3; counter++) 
//     {
//         printf("counter is at %lu...\n", counter);
//         if (1 == counter) TriggerEvent(test);
//     }

//     sleep(1);

//     // unsub, trigger again. listerner1 doen not fire
//     Event.unsubscribe(test, listener1);

//     for (counter = 0 ;counter < 3; counter++) 
//     {
//         printf("counter is at %lu...\n", counter);
//         if (1 == counter) TriggerEvent(test);
//     }

//     sleep(1);
    // FrameworkCleanup();

    return 0;
}
