#include "q2workshop.h"
#include "q2reindeer.h"
#include "q2elf.h"

/*  Workshop Constructor
    Takes in the printer, the bound on number of times reindeer go instead of elves,
    the number of elves, as well as the number of deliveries needed to achieve
    by the reindeer.

    Some important variables initialized is the bound counter which is updates as
    reindeer claim priority, the number of elves to signal which is a variable 
    used to signal elves in a daisy chain fashion but prevent extra elves from being 
    signalled.
*/
Workshop::Workshop( Printer &prt, unsigned int N, unsigned int E, unsigned int D )
: m_prt( prt )
, m_bound( N )
, m_boundCounter( N )
, m_elfCount( E )
, m_numElvesToSignal( 0 )
, m_numDeliveries( D )
, m_deliverCount( 0 )
, m_doneDeliverCount( 0 )
, m_consultCount( 0 )
, m_doneConsultCount( 0 )
{} 

/*  Santa calls to nap; when Santa wakes status of next action
    
    Santa return Done if the deliveries are completed and there are less than
    3 elves left which is the number of elves for consultation.  

    The Santa blocks on sleep if there are no reindeer or elves to serve.

    When santa wakes up, if it can serve both reindeer or elves, then the boundCounter
    is used to determine who goes. Reindeers have priority as long as the counter > 0. 
    
    If santa is doing a delivery, the m_numDeliveries is decremented and the reindeers are signalled
    to proceed as they are blocked in the delivery function.

    If santa is doing consultation, it resets the priority counter, and signals the elves to proceed
    from their blocked state in consulting.
*/
Workshop::Status Workshop::sleep() {
    Status returnStatus;
   
    // Test in a loop since if we wake up, we want to retest the conditions 
    while ( true ) { 
        if ( m_numDeliveries == 0 && m_elfCount == 0 ) {
            return Done;
        }
        
        // If cannot set reindeer or elves, then block
        if ( m_deliverCount != Reindeer::MAX_NUM_REINDEER && m_consultCount < Elf::CONSULTING_GROUP_SIZE ) {
            m_prt.print( 0, Printer::Blocked ); 
            m_santaCond.wait();
            m_prt.print( 0, Printer::Unblocked ); 
            continue; // start the loop again to test
        }
            
        // If you can serve reindeer or elves, do so and unblock them appropriately
        if ( m_deliverCount == Reindeer::MAX_NUM_REINDEER || m_consultCount >= Elf::CONSULTING_GROUP_SIZE ) {
            // if raindeer and elves competing, let raindeer win if the bound counter > 0
            if ( m_deliverCount == Reindeer::MAX_NUM_REINDEER && m_consultCount >= Elf::CONSULTING_GROUP_SIZE ) {
                returnStatus = m_boundCounter > 0 ? Delivery : Consulting;  
                m_boundCounter--;   
            }
            else {
                returnStatus = m_deliverCount == Reindeer::MAX_NUM_REINDEER ? Delivery : Consulting;
            }
        }
        
        if ( returnStatus == Delivery ) {
            m_numDeliveries--;                  // one delivery is performed by all reindeers
            m_deliverCond.signal();             // allow reindeer to perform delivery
        }
        else if ( returnStatus == Consulting ) {
            m_boundCounter = m_bound;           // reset priority counter when elves have a chance to go
            m_consultCond.signal();             // allow elves to perform consulting
            m_numElvesToSignal--;
        }
        break; // done delivery/consultation so break out
    }
    
    return returnStatus;
}

/*  Reindeer call to deliver toys
    
    If the max number of reindeers has arrived, then signal santa to wake up from sleep.

    Otherwise block the reindeers. If a reindeer gets signalled, then wake up all
    other reindeer in a daisy chain fashion by calling signal before leaving. 

*/
void Workshop::deliver( unsigned int id ) {
    m_deliverCount++;
            
    if ( m_deliverCount == Reindeer::MAX_NUM_REINDEER ) 
        m_santaCond.signal();
                    
    m_prt.print( id, Printer::Printer::Blocked, m_deliverCount );
    m_deliverCond.wait();
    m_prt.print( id, Printer::Printer::Unblocked, m_deliverCount );
    m_deliverCount--;
    m_deliverCond.signal(); // wake up next reindeer
}

/*  Block Santa/reindeer until all toys are delivered

    When the last task arrives, it wakes up all other blocked
    tasks in a daisy chain fashion by calling signal. 
*/
void Workshop::doneDelivering( unsigned int id ) {
    m_doneDeliverCount++;
    
    if ( m_doneDeliverCount == Reindeer::MAX_NUM_REINDEER + 1 ) {
        m_doneDeliverCond.signal();
    }
    else {
        m_prt.print( id, Printer::Blocked, m_doneDeliverCount );
        m_doneDeliverCond.wait();
        m_prt.print( id, Printer::Unblocked, m_doneDeliverCount );
        m_doneDeliverCond.signal(); // wake up next task
    }

    m_doneDeliverCount--;
}

/*  Elves call to consult Santa
    true => consultation successful, false => consultation failed

    Do not bother going into blocking state if the total elves
    left are less than 3 and return false.

    If 3 elves have arrive, wake up santa and set the signal counter to 3

    Once the elves wake up, they wake up other elves in a daisy chain fashion
    up to 3 elves in total. Then reset the counter
     
*/
bool Workshop::consult( unsigned int id ) {
    if ( m_elfCount < 3) 
        return false;

    m_consultCount++;
                
    if ( m_consultCount == Elf::CONSULTING_GROUP_SIZE ) {
        m_santaCond.signal();
        m_numElvesToSignal = 3;
    }
                    
    m_prt.print( id, Printer::Blocked, m_consultCount );
    m_consultCond.wait();
    m_prt.print( id, Printer::Unblocked, m_consultCount );
    m_consultCount--;
    if ( m_numElvesToSignal > 0 ) {
        m_consultCond.signal(); // wake up next elf
        m_numElvesToSignal--;
    }
    else if ( m_numElvesToSignal == 0 && m_consultCount >= 3 ) {
        m_numElvesToSignal = 3; // if I'm the 3rd elf to exit the consulting and there are
                                // 3 or more elves waiting for consultation, reset the
                                // counter since Santa will wake them eventually in a
                                // daisy chain fashion
    }
    return m_elfCount < 3 ? false : true;
}

// block Santa/elves until meeting over
void Workshop::doneConsulting( unsigned int id ) {
    m_doneConsultCount++;

    // Last task to enter the consulting condition. 
    // Realease all tasks on this condition queue.
    if ( m_doneConsultCount == Elf::CONSULTING_GROUP_SIZE + 1 ) {
        m_doneConsultCond.signal(); // wake up task 
    }
    else {
        m_prt.print( id, Printer::Blocked, m_doneConsultCount );
        m_doneConsultCond.wait();
        m_prt.print( id, Printer::Unblocked, m_doneConsultCount );
        m_doneConsultCond.signal(); // daisy chain wakeup next task
    }
    m_doneConsultCount--;
} 

// elves call to indicate termination
void Workshop::termination( unsigned int id ) {
    m_elfCount--;

    if ( m_elfCount < 3 ) {
        m_consultCond.signal();
        m_santaCond.signal();
        m_numElvesToSignal = 1;
    }
}
