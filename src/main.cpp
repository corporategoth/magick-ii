#include "pch.h"
#ifdef WIN32
#pragma hdrstop
#endif

/* Magick IRC Services
**
** (c) 1997-2002 Preston Elder <prez@magick.tm>
** (c) 1998-2002 William King <ungod@magick.tm>
**
** The above copywright may not be removed under any circumstances,
** however it may be added to if any modifications are made to this
** file.  All modified code must be clearly documented and labelled.
**
** This code is released under the Artistic License v2.0 or better.
** The full text of this license should be contained in a file called
** COPYING distributed with this code.  If this file does not exist,
** it may be viewed here: http://www.magick.tm/m2/license.html
**
** ======================================================================= */
#define RCSID(x,y) const char *rcsid_main_cpp_ ## x () { return y; }
RCSID(main_cpp, "@(#)$Id$");

/* ======================================================================= **
**
** For official changes (by the Magick Development Team),please
** check the ChangeLog* files that come with this distribution.
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** ======================================================================= */

#include "magick.h"

#ifdef WIN32
bool isWinNT()
{
#ifdef __BORLANDC__
    return (Win32Platform == VER_PLATFORM_WIN32_NT)
#elif _MSC_VER
    OSVERSIONINFO osvi;

    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    if (GetVersionEx(&osvi))
	return (osvi.dwPlatformId == VER_PLATFORM_WIN32_NT);
    else
	return false;
#else
    return false;
#endif
}

ACE_NT_SERVICE_DEFINE(Magick, Flow_Control, ACE_TEXT(FULLNAME));

#endif /* WIN32 */

int main(int argc, char **argv)
{
#ifdef MAGICK_TRACE_WORKS
    Trace::levelname.push_back(Trace::levelname_struct("OFF", Trace::Off));
    Trace::levelname.push_back(Trace::levelname_struct("NONE", Trace::Off));
    Trace::levelname.push_back(Trace::levelname_struct("FULL", Trace::Full));
    Trace::levelname.push_back(Trace::levelname_struct("ALL", Trace::Full));
    Trace::levelname.push_back(Trace::levelname_struct("STAT*", Trace::Stat));
    Trace::levelname.push_back(Trace::levelname_struct("L*CK*", Trace::Locking));
    Trace::levelname.push_back(Trace::levelname_struct("S*CK*", Trace::Socket));
    Trace::levelname.push_back(Trace::levelname_struct("BIND*", Trace::Bind));
    Trace::levelname.push_back(Trace::levelname_struct("REG*", Trace::Bind));
    Trace::levelname.push_back(Trace::levelname_struct("HOOK*", Trace::Bind));
    Trace::levelname.push_back(Trace::levelname_struct("EXT*", Trace::External));
    Trace::levelname.push_back(Trace::levelname_struct("CHAT*", Trace::Chatter));
    Trace::levelname.push_back(Trace::levelname_struct("CHE*", Trace::CheckPoint));
    Trace::levelname.push_back(Trace::levelname_struct("C*P*", Trace::CheckPoint));
    Trace::levelname.push_back(Trace::levelname_struct("COM*", Trace::Comment));
    Trace::levelname.push_back(Trace::levelname_struct("EXC*", Trace::Exception));
    Trace::levelname.push_back(Trace::levelname_struct("F*NC*", Trace::Function));
    Trace::levelname.push_back(Trace::levelname_struct("MOD*", Trace::Modify));
    Trace::levelname.push_back(Trace::levelname_struct("CH*G*", Trace::Changing));
    Trace::levelname.push_back(Trace::levelname_struct("SOUR*", Trace::Source));
    Trace::levelname.push_back(Trace::levelname_struct("SRC*", Trace::Source));
#endif /* MAGICK_TRACE_WORKS */

    ACE::init();
    Magick::StartTime(mDateTime::CurrentDateTime());
    mThread::Attach(tt_MAIN);

    Magick *instance = new Magick(argc, argv);

    Magick::register_instance(instance);

    Flow_Control *fc = new Flow_Control(instance);
#ifdef WIN32
    fc->name(ACE_TEXT(PACKAGE), ACE_TEXT(FULLNAME));
#endif

    int Result = instance->Init(fc);

    switch (Result)
    {
#ifdef WIN32
    case MAGICK_RET_SERVICE_INSERT:
	{
	    mstring cmd =
		"\"" + instance->ProgramName() + "\"" + " --dir \"" + instance->Services_Dir() + "\"" + " --config \"" +
		instance->Config_File() + "\"";

	    fc->insert(SERVICE_AUTO_START, SERVICE_ERROR_IGNORE, cmd.c_str());
	}
	Result = MAGICK_RET_NORMAL;
	break;
    case MAGICK_RET_SERVICE_START:
	fc->start_svc();
	Result = MAGICK_RET_NORMAL;
	break;
    case MAGICK_RET_SERVICE_COMMAND:
	Result = MAGICK_RET_NORMAL;
	break;
#endif /* WIN32 */
    case MAGICK_RET_NORMAL:
	if (instance->DoItAll())
	{
	    Result = fc->svc();
#ifdef WIN32
	}
	else
	{
	    ACE_NT_SERVICE_RUN(Magick, fc, err);
	    return err == 0;
#endif /* WIN32 */
	}
	break;
    case MAGICK_RET_LOCKED:
	mFile::Erase(argv[0]);
    }

    delete fc;
    mThread::Detach();
    ACE::fini();

    return Result;
}

Flow_Control::Flow_Control(Magick * i)
{
    magick_instance = i;
#ifdef WIN32
    reactor(&magick_instance->reactor());
#endif
}

Flow_Control::~Flow_Control()
{
    if (magick_instance != NULL)
    {
	magick_instance->Finish();
	delete magick_instance;
    }
#ifdef WIN32
    reactor(NULL);
#endif
}

int Flow_Control::svc()
{
#ifdef WIN32
    if (report_status(SERVICE_START_PENDING) == 0)
	reactor()->owner(ACE_Thread::self());
    Magick::register_instance(magick_instance);
#endif

#ifdef WIN32
    report_status(SERVICE_START_PENDING);
#endif
    int Result = magick_instance->Start();

    if (Result == MAGICK_RET_NORMAL)
    {
#ifdef WIN32
	report_status(SERVICE_RUNNING);
#endif
	Result = magick_instance->Run();
#ifdef WIN32
	report_status(SERVICE_STOP_PENDING);
#endif
	if (Result == MAGICK_RET_NORMAL)
	    Result = magick_instance->Stop();
#ifdef WIN32
	report_status(SERVICE_STOPPED);
#endif
	if (Result == MAGICK_RET_RESTART)
	{
	    vector < mstring > argv = magick_instance->arguments();
	    magick_instance->Finish();
	    Magick::deregister_instance();
	    delete magick_instance;
	    magick_instance = NULL;
	    magick_instance = new Magick(argv);
	    Magick::register_instance(magick_instance);

	    Result = magick_instance->Init(this);
	    if (Result == MAGICK_RET_NORMAL)
		Result = svc();
	}
    }
#ifdef WIN32
    else
	report_status(SERVICE_STOPPED);
#endif

    return Result;
}

void Flow_Control::stop_requested(DWORD control_code)
{
    static_cast < void > (control_code);

#ifdef WIN32
    if (state() != SERVICE_RUNNING && state() != SERVICE_PAUSED)
	return;
    report_status(SERVICE_STOP_PENDING);
#endif
    magick_instance->Pause(false);
    magick_instance->Stop();
#ifdef WIN32
    report_status(SERVICE_STOPPED);
#endif
}

void Flow_Control::pause_requested(DWORD control_code)
{
    static_cast < void > (control_code);

#ifdef WIN32
    if (state() != SERVICE_RUNNING)
	return;

    report_status(SERVICE_PAUSE_PENDING);
#endif
    magick_instance->Pause(true);
#ifdef WIN32
    suspend();
    report_status(SERVICE_PAUSED);
#endif
}

void Flow_Control::continue_requested(DWORD control_code)
{
    static_cast < void > (control_code);

#ifdef WIN32
    if (state() != SERVICE_PAUSED)
	return;

    report_status(SERVICE_CONTINUE_PENDING);
#endif
    magick_instance->Pause(false);
#ifdef WIN32
    resume();
    report_status(SERVICE_RUNNING);
#endif
}

int Flow_Control::dependancy(const char *deps)
{
    SC_HANDLE svc = this->svc_sc_handle ();
    if (svc == 0)
	return -1;

    BOOL ok = ChangeServiceConfig (svc,
	(DWORD) SERVICE_NO_CHANGE,// No change to service type
	(DWORD) SERVICE_NO_CHANGE,// New startup type
	(DWORD) SERVICE_NO_CHANGE,// No change to error ctrl
	0,                        // No change to pathname
	0,                        // No change to load group
	0,                        // No change to tag
	deps,                     // No change to dependencies
	0, 0,                     // No change to acct/passwd
	0);                       // No change to name
    return ok ? 0 : -1;
}

int Flow_Control::user(const char *u, const char *p)
{
    SC_HANDLE svc = this->svc_sc_handle ();
    if (svc == 0)
	return -1;

    BOOL ok = ChangeServiceConfig (svc,
	(DWORD) SERVICE_NO_CHANGE,// No change to service type
	(DWORD) SERVICE_NO_CHANGE,// New startup type
	(DWORD) SERVICE_NO_CHANGE,// No change to error ctrl
	0,                        // No change to pathname
	0,                        // No change to load group
	0,                        // No change to tag
	0,                        // No change to dependencies
	u, p,                     // No change to acct/passwd
	0);                       // No change to name
    return ok ? 0 : -1;
}

