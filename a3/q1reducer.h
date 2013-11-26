/* 
    Interface for the Reducer Task
*/

#ifndef _REDUCER
#define _REDUCER

#include <uSemaphore.h>
#include <vector>
#include <map>
#include "q1mapper.h"

using namespace std;

_Task Reducer {
    void main();
  public:
    Reducer(int id, int num_reducers, uSemaphore* signal, const vector<Mapper*>& mappers);
    virtual ~Reducer();
    // DJB2 hash. Include as part of the class interface.
    unsigned long hash(const string& str) {
        unsigned long hash = 5381;
        for (unsigned int i = 0; i < str.size(); ++i) {
            hash = ((hash << 5) + hash) + str[i];
        }
        return hash;
    }
    int getID();
    int getNumReducers();
    uSemaphore* getSignal();
    vector<Mapper*>& getMappers();
  
  private:
    unsigned int m_id;
    int m_numReducers;
    uSemaphore *m_pSignal;
    const vector<Mapper*>& m_mappers;
    map<string,int> m_keyvals; 
};

#endif // _REDUCEr
