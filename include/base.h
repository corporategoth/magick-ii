// $Id$
//
// Magick IRC Services
// (c) 1997-1999 Preston A. Elder <prez@magick.tm>
// (c) 1998-1999 W. King <ungod@magick.tm>
//
// The above copywright may not be removed under any
// circumstances, however it may be added to if any
// modifications are made to this file.  All modified
// code must be clearly documented and labelled.
//
// ===================================================
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