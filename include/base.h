#ifndef _base_h
#define _base_h
#include "mstring.h"
#include "trace.h"
#include <algorithm>
#include <queue>
using namespace std;

class mBase
{
    friend void *thread_handler(void *owner);
protected:
    deque<pair<mstring,mstring> > inputbuffer; // pair of sentto,datastring
public:
    bool on;
    void FlagTerminate(void) {on=false;}

    mBase();
    void push_message(const mstring& servicename, const mstring& message);
    void init();

    virtual void execute(const mstring& servicename, const mstring& message)=0;
    virtual threadtype_enum Get_TType() const=0;
    //virtual mBase *GetOwner()=0;
    virtual mstring GetInternalName() const=0;
    operator mVariant() const { mVariant locvar(GetInternalName()); locvar.truevaluetype=GetInternalName(); return locvar; };
    
};
#endif