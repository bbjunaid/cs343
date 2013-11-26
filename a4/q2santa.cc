#include "q2santa.h"
#include "MPRNG.h"

extern MPRNG mprng; // delcared in q2driver.cc

void Santa::main() {
    Workshop::Status status = Workshop::Consulting; // arbitary status other than done

    yield( mprng(10) );                     // yield so all tasks do not start simultaneously
    m_prt.print( 0, Printer::Starting );    // start message

    for( ;; ) {  
        yield( mprng(3) );                  // yield so messages not consecutive

        m_prt.print( 0, Printer::Napping );        // napping message
        status = m_wrk.sleep();                    // block in workshop
        m_prt.print( 0, Printer::Awake );          // Awake message

        if ( status == Workshop::Done ) break;    // if done consulting/deliviers break out

        // Display appropriate messages and yield appropriate amounts of time
        // depending on Consulting/Delivering. Also call the appropraite
        // done functions
        if ( status == Workshop::Delivery ) {
            m_prt.print( 0, Printer::DeliveringToys);
            yield( mprng(5) ); 
            m_wrk.doneDelivering( 0 );
            m_prt.print( 0, Printer::DoneDelivering );
        }
        else {
            m_prt.print( 0, Printer::Consulting ); 
            yield( mprng(3) );
            m_wrk.doneConsulting( 0 );
            m_prt.print( 0, Printer::DoneConsulting );
        }
    }
    m_prt.print( 0, Printer::Finished );  // finish message
}
