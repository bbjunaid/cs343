#include "q2reindeer.h"
#include "MPRNG.h"

extern MPRNG mprng; // declared in q2driver.cc

void Reindeer::main() {
    unsigned int deliveryCount = 0;                  // delivery count to know when to break
    
    yield( mprng(10) );                             // yield so all tasks do not start simultaenously
    m_prt.print( m_id, Printer::Starting );         // start message
    
    while ( deliveryCount != m_numDeliveries ) {
        yield( mprng(3) );                          // avoid consecutive messages
        
        m_prt.print( m_id, Printer::OnVacation );   // vacation message
        
        yield( mprng(5) );                          // simulate vacation time
        
        m_prt.print( m_id, Printer::CheckingIn );   // checking-in message
        
        m_wrk.deliver( m_id );                      // wait for delivery
        deliveryCount++;
        
        m_prt.print( m_id, Printer::DeliveringToys );       // delivering-toys message
        
        yield( mprng(5) );                       // simulate toy-delivery time

        m_wrk.doneDelivering( m_id );                       // indicate delivery done
        
        m_prt.print( m_id, Printer::DoneDelivering );       // delivery-done message
    }
    m_prt.print( m_id, Printer::Finished );                 // finished message
}
