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
//
// Tracing functions -- Include making TraceMap's and
// receiving all trace information.

// Bit	Use
// 1	
// 2	
// 3	
// 4	
// 5	
// 6	
// 7	
// 8	

class Trace
{
public:
    enum {
	FuncTrace = 0x00000001,	// Trace functions
	DataMB    = 0x10000000,	// Data before modification
	DataMA    = 0x20000000,	// Data AFTER modification
    };
    Trace()
    {
    }
    ~Trace() {}
    bool IsOn(int level);

    WriteOut(int level, int tid, mstring &message);
}

class FuncTrace() : public Trace
{
public:
    FuncTrace(const mstring name, mVarArray &args);
    ~FuncTrace() { indent--; }

}


FuncTrace::FuncTrace(const mstring &name, mVarArray &args) {
    indent++;

    if (IsOn(FuncTrace)) {
	mstring message = name + "(";
	for (int i=0; i<args.count(); i++) {
	    message += " (" + args[i].type() + ") " + args[i];
	    if (i < args.count() - 1)
		message +=", ";
	}
	message += " )";

	WriteOut(FuncTrace, ThreadID, message);
    }
}
