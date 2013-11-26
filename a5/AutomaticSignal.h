#include <uC++.h>

/*  This implementation is based off the FIFO implementation of automatic
    signals in the uC++ textbook, section 9.12.2 FIFO Simulation.
*/

/*  Macro AUTOMATIC_SIGNAL creates appropriate data structures and 
    variables to perform FIFO simulation of Priority Non-Block Monitor
*/
#define AUTOMATIC_SIGNAL \
struct ConditionNode_t : public uSeqable { \
    uCondition cond; \
}; \
\
struct ConditionFifo_t { \
    uSequence<ConditionNode_t> waiting; \
    ConditionNode_t *cursor; \
}; \
\
ConditionFifo_t m_waitingService; 

/*  Macro WAITUNTIL causes a task to block if the condition pred
    is false. Before the task blocks, the statement before is executed.
    After a task wakes up, the condition after is executes/
*/
#define WAITUNTIL( pred, before, after ) \
if ( !m_waitingService.waiting.empty() || !(pred) ) { \
    m_waitingService.cursor = m_waitingService.waiting.head(); \
    ConditionNode_t condNode; \
    m_waitingService.waiting.addTail( &condNode ); \
    for ( ;; ) { \
        if ( m_waitingService.cursor != 0 ) \
            m_waitingService.cursor->cond.signal(); \
        before; \
        condNode.cond.wait(); \
        after; \
        if ( (pred) ) break; \
        m_waitingService.cursor = m_waitingService.waiting.succ( &condNode ); \
    } \
    m_waitingService.waiting.remove( &condNode ); \
}

/*  Macro RETURN causes a checking cycle to occur using daisy chaining.
    It also returns any expression passed through to the macro.
*/
#define RETURN( expr... ) \
if ( !m_waitingService.waiting.empty() ) { \
    m_waitingService.cursor = m_waitingService.waiting.head(); \
    m_waitingService.cursor->cond.signal(); \
} \
return expr; 
