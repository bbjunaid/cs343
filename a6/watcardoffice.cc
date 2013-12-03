#include "watcardoffice.h"

#include "printer.h"
#include "bank.h"
#include "MPRNG.h"

extern MPRNG mprng;

// WATCardOffice task code
// WATCardOffice is an administrator task
// Upon receiving new jobs, the watcard office accepts couriers that have requested work
// Through a queue, the office hands off tasks to the couriers
// Courier are externally scheduled until there is a new job for them to do (size of queue is not 0)
void WATCardOffice::main() {
    m_prt.print( Printer::WATCardOffice, 'S' );
    
    for ( ;; ) {
        _Accept ( ~WATCardOffice ) {
            for ( unsigned int i = 0; i < m_numCouriers; i++ ) {
                _Accept( requestWork ) {
                }
            }
            for( unsigned int i = 0; i < m_numCouriers; i++ ) {
                delete couriers[i];
            }       
            break;
        }
        or _When( m_requests.size() > 0 ) _Accept( requestWork ) {  // Externally schedule couriers, unblock if new jobs are to be handed off
            m_prt.print( Printer::WATCardOffice, 'W' );
        }
        or _Accept( create ) {
            m_requests.push( m_newJob );                            // Push new task for courier to pickup
            // Print creation rendezvous complete
            m_prt.print( Printer::WATCardOffice, 'C', m_newJob->args.sid, m_newJob->args.amount );
        } or _Accept( transfer ) {
            m_requests.push( m_newJob );                            // Push new task for courier to pickup
            // Print transfer rendezvous complete
            m_prt.print( Printer::WATCardOffice, 'T', m_newJob->args.sid, m_newJob->args.amount );
        } 
    }
    m_prt.print( Printer::WATCardOffice, 'F' );
}

// Office constructor
// Initialize task variables
// Dynamically create couriers
WATCardOffice::WATCardOffice( Printer &prt, Bank &bank, unsigned int numCouriers ) 
: m_prt(prt)
, m_bank(bank)
, m_numCouriers(numCouriers) {
    for ( unsigned int i; i < m_numCouriers; i += 1 ) {
        couriers.push_back( new Courier( i, *this, prt, bank ) );
    }
}

// Create new job along with a new watcard for the student
// Copy pointer to job so office can handle it as an admin
// Return future to student right away
WATCard::FWATCard WATCardOffice::create( unsigned int sid, unsigned int amount ) {
    Job *job = new Job( Args( sid, amount, new WATCard() ) );
    m_newJob = job;
    return job->result;
}

// Create new job along with the student's watcard passed in
// Copy pointer to job so office can handle it as an admin
// Return future to student right away
WATCard::FWATCard WATCardOffice::transfer( unsigned int sid, unsigned int amount, WATCard *card ) {
    Job *job = new Job( Args( sid, amount, card ) ); 
    m_newJob = job;
    return job->result;
}

// Couriers call request work
// Courier are externaly schedule on this call
// If the size of request queue is zero, a NULL is returned to indicate that office is closing
WATCardOffice::Job *WATCardOffice::requestWork() {
    // When Courier get's unblocked at the very end, there will be no jobs
    if ( m_requests.empty () ) 
        return NULL; 

    Job* job = m_requests.front();
    m_requests.pop();
    return job;
}


//-----------------------------------------------------------------------------
// Implmentation of Courier Task
//-----------------------------------------------------------------------------

// Courier construct
// initialize all task variables
// Enclosing task (WATCardOffice) passes an instance of itself to the courier
WATCardOffice::Courier::Courier( unsigned int id, WATCardOffice &office, Printer &prt, Bank &bank )
: m_id( id )
, m_office(office)
, m_prt( prt )
, m_bank( bank )
{}

// Courier task code
// Courier continuouly requests a job
// The WATCardOffice return a NULL pointer instead of a pointer to a Job if it is closing at which point
// the courier also terminates
// The courier transfer funds for a student from their bank account to their watcard
// There is a 1 in 6 chance that after the funds transfer the student card is lost, watcard is deleted, exception is thrown
// If watcard wasn't lost, future is delivered and student requesting that future is unblocked
// Job is deleted at the end of a courier run
void WATCardOffice::Courier::main() {
    m_prt.print( Printer::Courier, m_id, 'S' );
    for ( ;; ) {
        // Request work - may get blocked
        Job* job = m_office.requestWork();

        // Only gets NULL jobs when WATCardOffice is closing
        if ( job == NULL ) {
            break;
        }

        // Print start funds transfer
        m_prt.print( Printer::Courier, m_id, 't', job->args.sid, job->args.amount );
        // Withdraw from the back
        m_bank.withdraw( job->args.sid, job->args.amount );

        // Deposit after a funds transfer
        job->args.card->deposit( job->args.amount );
        // Print complete funds transfer
        m_prt.print( Printer::Courier, m_id, 'T', job->args.sid, job->args.amount );
        
        if ( mprng( 5 ) == 0 ) {                     // There is a 1 in 6 chance WATCard is lost
            job->result.exception( new Lost );              // Insert Lost exception intor student's WATCard
            delete job->args.card;                          // Delete current WATCard
        }
        else {
            job->result.delivery( job->args.card );         // Deliver future upon a successful transfer of funds
        }
        delete job;
    }
    m_prt.print( Printer::Courier, m_id, 'F' );          // Print finished message
}
