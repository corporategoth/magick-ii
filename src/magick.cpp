// RCS-ID:      $Id$
#include "magick.h"
void ErrorHandler(InterpreterContextPtr c,int code,void *data);


Magick::Magick(int inargc, const char **inargv)
{
	for(int i=0;i<inargc;i++)
	{
		argv.push_back(inargv[i]);
	}
}

void Magick::Start()
{
	// this is our main routine, when it leaves here, this sucker's done.
	if(StartBob("")==false)
	{
		/*log that bob couldn't start*/
		bobavail=false;
	}
}

bool Magick::StartBob(const string& scriptname)
{
	Stream *s=NULL;
	if((bobic=NewInterpreterContext())==NULL)
		return false;
	PushUnwindTarget(bobic,&bobut);
	if(UnwindCatch(bobic))
		return false;
	InitObjectMemory(bobic,16384,1024);
	bobic->errorHandler=ErrorHandler;

	/*ic->standardInput = (Stream *)&consoleStream;
	ic->standardOutput = (Stream *)&consoleStream;
	ic->standardError = (Stream *)&consoleStream;*/
	// todo redirect the above to logging streams

	EnterLibrarySymbols(bobic);
	bobic->findFunctionHandler=FindLibraryFunctionHandler;

    //todo here add magick specific symbols/functions
    //ObjectPtr ival;
    //ProtectPtr(bobic,&ival);
    //IntegerObject(ival);
    //EnterBuiltInVariable(bobic,"name of variable",ival);
    //    or
    //EnterBuiltInFunction(bobic,"name of function",(CMethodHandlerPtr)ptr_to_fucntion);
    bobcc=InitCompiler(bobic,4096,256);
    ProtectPointer(bobic,&bobval);
    if(scriptname!="")
    {
        s=OpenFileStream((char *)scriptname.c_str(),"r");
        if(s==NULL)
            return false;
        bobval=CompileExpr(bobcc,s);
    }
    UnwindCatch(bobic);
    if(s!=NULL)
    {
        CloseStream(s);
        return true;
    }
    else
        return false;
}

void ErrorHandler(InterpreterContextPtr c,int code,void *data)
{

}
