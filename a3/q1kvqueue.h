/* 
    Interface for the KVQueue
*/

#ifndef _KVQUEUE
#define _KVQUEUE

#include <uSemaphore.h>
#include <queue>
#include "q1mapper.h"

class KVQueue {
  public:
    struct EmptyAndClosed {
        int num_thrown;
        EmptyAndClosed(int num_thrown) : num_thrown(num_thrown) {}
    };
    KVQueue(int size);
    void pushBack(const Mapper::KeyValue& item);
    Mapper::KeyValue popFront();
    // Copy the front value to "val" without removing it from the queue. Return 0 on success and -1
    // if the queue is empty. Throw an EmptyAndClosed exception if the queue is closed and empty.
    int peekFront(Mapper::KeyValue* val) throw(EmptyAndClosed);
    void close();
  
  private:
    int m_numThrown;
    std::queue<Mapper::KeyValue> m_queue;
    bool m_isClosed;
    bool m_isEmptyAndClosed;
    uSemaphore m_emptySem;
    uSemaphore m_fullSem;
    uSemaphore m_binSemQueue; //  binary semaphore to protect queue
    uSemaphore m_binSemGeneral; // general purpose binary semaphore
};

#endif // _KVQUEUE
