/* 
    Interface for the Mapper Task
*/

#ifndef _MAPPER
#define _MAPPER

#include <uSemaphore.h>
using namespace std;

class KVQueue; // forward declare KVQueue

_Task Mapper {
    void main();
  public:
    struct KeyValue {
        string key;
        int value;

        KeyValue(const string& key, int value) : key(key), value(value) {}
        KeyValue() {}
    };
    Mapper(const string& filename, int queue_len, uSemaphore* signal);
    virtual ~Mapper();
    uSemaphore* getSignal();
    const string& getFilename();
    KVQueue* q;

  private:
    string m_filename;
    uSemaphore *m_pSignal;
    int m_queueLen;
};

#endif // _MAPPER
