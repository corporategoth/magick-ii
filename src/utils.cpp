#include "pch.h"
#ifdef WIN32
#pragma hdrstop
#else
#pragma implementation
#endif

/*  Magick IRC Services
**
** (c) 1997-2000 Preston Elder <prez@magick.tm>
** (c) 1998-2000 William King <ungod@magick.tm>
**
** The above copywright may not be removed under any
** circumstances, however it may be added to if any
** modifications are made to this file.  All modified
** code must be clearly documented and labelled.
**
** ========================================================== */
static const char *ident = "@(#)$Id$";
/* ==========================================================
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <magick-devel@magick.tm>:
**
** $Log$
** Revision 1.41  2000/06/28 18:52:42  prez
** OOps, forgot to #include des/spr.h
**
** Revision 1.40  2000/06/28 12:20:50  prez
** Lots of encryption stuff, but essentially, we now have random
** key generation for the keyfile keys, and we can actually encrypt
** something, and get it back as we sent it in (specifically, the
** keyfile itself).
**
** Revision 1.39  2000/06/27 18:56:59  prez
** Added choosing of keys to configure, also created the keygen,
** and scrambler (so keys are not stored in clear text, even in
** the .h and binary files).  We should be set to do the decryption
** process now, as encryption (except for encryption of db's) is
** all done :)
**
** Revision 1.38  2000/05/28 05:05:14  prez
** More makefile stuff ... Now we should work on all platforms.
** Added alot of checking for different .h files, functions, etc.
** So now all #define's are config.h based (also added a default
** windows config.h, which will need to be copied on these systems).
**
** Revision 1.37  2000/05/21 04:49:41  prez
** Removed all wxLog tags, now totally using our own logging.
**
** Revision 1.36  2000/05/20 15:17:00  prez
** Changed LOG system to use ACE's log system, removed wxLog, and
** added wrappers into pch.h and magick.cpp.
**
** Revision 1.35  2000/05/13 07:05:47  prez
** Added displaying of sizes to all file fields..
**
** Revision 1.34  2000/04/30 03:48:30  prez
** Replaced all system calls with ACE_OS equivilants,
** also removed any dependancy on ACE from sxp (xml)
**
** Revision 1.33  2000/03/28 16:20:59  prez
** LOTS of RET() fixes, they should now be safe and not do double
** calculations.  Also a few bug fixes from testing.
**
** Revision 1.32  2000/03/19 08:50:57  prez
** More Borlandization -- Added WHAT project, and fixed a bunch
** of minor warnings that appear in borland.
**
** Revision 1.31  2000/02/27 03:58:40  prez
** Fixed the WHAT program, also removed RegEx from Magick.
**
** Revision 1.30  2000/02/23 12:21:04  prez
** Fixed the Magick Help System (needed to add to ExtractWord).
** Also replaced #pragma ident's with static const char *ident's
** that will be picked up by what or version, and we can now
** dump from a binary what versions of each file were used.
**
** Revision 1.29  2000/02/17 12:55:08  ungod
** still working on borlandization
**
** Revision 1.28  2000/02/16 12:59:41  ungod
** fixing for borland compilability
**
** Revision 1.27  2000/02/15 13:27:04  prez
** *** empty log message ***
**
** Revision 1.26  2000/02/15 10:37:51  prez
** Added standardized headers to ALL Magick source files, including
** a #pragma ident, and history log.  ALL revisions of files from
** now on should include what changes were made to the files involved.
**
**
** ========================================================== */

#include "utils.h"
#include "trace.h"
#include "magick.h"
#ifdef HASCRYPT
#include "des/spr.h"
#endif

void wxSplitPath(const char *pszFileName,
                             mstring *pstrPath,
                             mstring *pstrName,
                             mstring *pstrExt)
{
  FT("wxSplitPath", (pszFileName, pstrPath, pstrName, pstrExt));
  wxCHECK_RET( pszFileName, "NULL file name in wxSplitPath" );

  const char *pDot = strrchr(pszFileName, FILE_SEP_EXT);
  const char *pSepUnix = strrchr(pszFileName, FILE_SEP_PATH_UNIX);
  const char *pSepDos = strrchr(pszFileName, FILE_SEP_PATH_DOS);

  // take the last of the two
  size_t nPosUnix = pSepUnix ? pSepUnix - pszFileName : 0;
  size_t nPosDos = pSepDos ? pSepDos - pszFileName : 0;
  if ( nPosDos > nPosUnix )
    nPosUnix = nPosDos;
//  size_t nLen = Strlen(pszFileName);

  if ( pstrPath )
    *pstrPath = mstring(pszFileName, nPosUnix);
  if ( pDot ) {
    size_t nPosDot = pDot - pszFileName;
    if ( pstrName )
      *pstrName = mstring(pszFileName + nPosUnix + 1, nPosDot - nPosUnix);
    if ( pstrExt )
      *pstrExt = mstring(pszFileName + nPosDot + 1);
  }
  else {
    if ( pstrName )
      *pstrName = mstring(pszFileName + nPosUnix + 1);
    if ( pstrExt )
      pstrExt->Empty();
  }
}

mstring &wxGetHomeDir(mstring &pstr)
{
  FT("wxGetHomeDir", (pstr));
  mstring& strDir = pstr;

#ifdef WIN32
      const char *szHome = ACE_OS::getenv("HOMEDRIVE");
      if ( szHome != NULL )
        strDir << szHome;
      szHome = ACE_OS::getenv("HOMEPATH");
      if ( szHome != NULL ) {
        strDir << szHome;

        // the idea is that under NT these variables have default values
        // of "%systemdrive%:" and "\\". As we don't want to create our
        // config files in the root directory of the system drive, we will
        // create it in our program's dir. However, if the user took care
        // to set HOMEPATH to something other than "\\", we suppose that he
        // knows what he is doing and use the supplied value.
        if ( ACE_OS::strcmp(szHome, "\\") != 0 )
          RET(strDir);
      }
    // 260 was taken from windef.h
# ifndef MAX_PATH
#   define MAX_PATH  260
# endif

    mstring strPath;
	char cstrPath[MAX_PATH+1];
    ::GetModuleFileName(::GetModuleHandle(NULL),
                        cstrPath, MAX_PATH);
    strPath=cstrPath;

    // extract the dir name
    wxSplitPath(strPath, &strDir, NULL, NULL);

#else /* WIN32 */
    const char *szHome = ACE_OS::getenv("HOME");
    if ( szHome == NULL ) {
      // we're homeless...try passwd
      struct passwd *pwent = ACE_OS::getpwnam(ACE_OS::cuserid(NULL));
      if (pwent == NULL || pwent->pw_dir == NULL)
      {
	Log(LM_WARNING, Parent->getLogMessage("WX_ERRORS/NOHOMEDIR"));
	strDir = ".";
      }
      else
	strDir = pwent->pw_dir;
    }
    else
       strDir = szHome;

    // add a trailing slash if needed
    if ( strDir.Last() != '/' )
      strDir << '/';

#endif /* WIN32 */

  RET(strDir);
}

bool
wxIsAbsolutePath (const mstring& filename)
{
  FT("wxIsAbsolutePath", (filename));
  if (filename != "")
    {
      if (filename[0U] == '/'
#ifdef __VMS__
      || (filename[0U] == '[' && filename[1U] != '.')
#endif
#ifdef WIN32
      /* MSDOS */
      || filename[0U] == '\\' || (isalpha (filename[0U]) && filename[1U] == ':')
#endif
        )
        RET(true);
    }
  RET(false);
}

const unsigned long TxnIds::min = 1000000000;
const unsigned long TxnIds::keeptime = 60 * 60 * 24;
map<unsigned long, mDateTime> TxnIds::i_Ids;
unsigned long TxnIds::i_Current = TxnIds::min;

unsigned long TxnIds::Create()
{
    NFT("TxnIds::Create");

    WLOCK(("TxnIds"));
    i_Current++;
    while (i_Current >= min && i_Ids.find(i_Current) != i_Ids.end())
	i_Current++;

    if (i_Current >= min)
    {
	i_Ids[i_Current] = Now();
	RET(i_Current);
    }
    else
    {
	i_Current = min;
	while (i_Current >= min && i_Ids.find(i_Current) != i_Ids.end())
	    i_Current++;
    }

    if (i_Current >= min)
    {
	i_Ids[i_Current] = Now();
	RET(i_Current);
    }
    else
    {
	Log(LM_ERROR, Parent->getLogMessage("WX_ERRORS/OUTOFTXNIDS"));
	RET(0);
    }
}

unsigned long TxnIds::Current()
{
    NFT("TxnIds::Current");
    RET(i_Current);
}

bool TxnIds::Register(unsigned long id)
{
    FT("TxnIds::Register", (id));

    WLOCK(("TxnIds"));
    if (i_Ids.find(id) != i_Ids.end())
	RET(false);

    i_Ids[id] = Now();
    if (i_Current < id)
	i_Current = id;

    RET(true);
}

void TxnIds::Expire()
{
    NFT("TxnIds::Expire");

    WLOCK(("TxnIds"));
    map<unsigned long, mDateTime>::iterator iter;
    vector<unsigned long> kill;
    for (iter=i_Ids.begin(); iter != i_Ids.end(); iter++)
    {
	if (iter->second.SecondsSince() > keeptime)
	    kill.push_back(iter->first);
    }
    for (unsigned int i=0; i<kill.size(); i++)
	i_Ids.erase(kill[i]);
}

vector<int> ParseNumbers(mstring what)
{
    FT("ParseNumbers", (what));
    vector<int> numbers;
    unsigned int i;
    mstring tmp;

    what.Replace(",", " ");
    for (i=1; i<=what.WordCount(" "); i++)
    {
	tmp = what.ExtractWord(i, " ");
	if (!tmp.IsNumber() || tmp[0U] == '-' ||
	    tmp[tmp.size()-1] == '-' || tmp.WordCount("-") > 2)
	{
	    numbers.push_back(-1);
	}
	else if (tmp.Contains("-"))
	{
	    int j, limit;
	    j = ACE_OS::atoi(tmp.Before("-").c_str());
	    limit = ACE_OS::atoi(tmp.After("-").c_str());
	    if (limit >= j)
	    {
		for (; j<=limit; j++)
		    numbers.push_back(j);
	    }
	    else
	    {
		for (; j>=limit; j--)
		    numbers.push_back(j);
	    }
	}
	else
	{
	    numbers.push_back(ACE_OS::atoi(tmp.c_str()));
	}
    }
    NRET(vector<int>, numbers);
}

unsigned long FromHumanTime(mstring in)
{
    FT("FromHumanTime", (in));

    unsigned int i;
    unsigned long number = 0, total = 0;

    for (i=0; i<in.size(); i++)
    {
	switch(in[i])
	{
	case 'Y':
	case 'y':
	    if (number != 0)
	    {
		total += number * (unsigned long)(60.0 * 60.0 * 24.0 * 365.25);
		number = 0;
	    }
	    break;
	case 'W':
	case 'w':
	    if (number != 0)
	    {
		total += number * 60 * 60 * 24 * 7;
		number = 0;
	    }
	    break;
	case 'D':
	case 'd':
	    if (number != 0)
	    {
		total += number * 60 * 60 * 24;
		number = 0;
	    }
	    break;
	case 'H':
	case 'h':
	    if (number != 0)
	    {
		total += number * 60 * 60;
		number = 0;
	    }
	    break;
	case 'M':
	case 'm':
	    if (number != 0)
	    {
		total += number * 60;
		number = 0;
	    }
	    break;
	case 'S':
	case 's':
	    if (number != 0)
	    {
		total += number;
		number = 0;
	    }
	    break;
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	    number *= 10;
	    number += (unsigned long) (in[i] - '0');
	    break;
	default:
	    RET(0);
	}
    }
    if (number != 0)
	total += number;

    RET(total);
}

mstring ToHumanTime(unsigned long in)
{
    FT("ToHumanTime", (in));

    if (in==0)
    {
	RET("unlimited");
    }

    mstring retval = DisectTime((long) in);
    RET(retval);
}

mstring ToHumanNumber(unsigned long in)
{
    FT("ToHumanNumber", (in));

    mstring retval = ltoa(in);

    if (in >= 11 && in <= 13)
    {
	retval += "th";
	RET(retval);
    }

    switch (retval[retval.Len()-1])
    {
    case '3':
	retval += "rd";
	break;
    case '2':
	retval += "nd";
	break;
    case '1':
	retval += "st";
	break;
    default:
	retval += "th";
	break;
    }
    RET(retval);
}


mstring ToHumanSpace(unsigned long in)
{
    FT("ToHumanSpace", (in));
    mstring retval;
    retval.Format("%ub", in);

    unsigned long value = 1024;
    for (int power = 1; power<5; power++)
    {
	CP(("Comparing %d to %d = %d", in, power, value));
	if (in >= value)
	{
	    switch (power) {
	    case 4:
	    	retval.Format("%uTb", in / value);
		break;
	    case 3:
	    	retval.Format("%uGb", in / value);
		break;
	    case 2:
	    	retval.Format("%uMb", in / value);
		break;
	    case 1:
	    	retval.Format("%uKb", in / value);
		break;
	    }
	}
	else
	    break;
	value *= 1024;
    }
    RET(retval);
}

unsigned long FromHumanSpace(mstring in)
{
    FT("FromHumanTime", (in));

    unsigned int i;
    unsigned long number = 0, total = 0;

    for (i=0; i<in.size(); i++)
    {
	switch(in[i])
	{
	case 'T':
	case 't':
	    if (number != 0)
	    {
		total += number * 1024 * 1024 * 1024 * 1024;
		number = 0;
	    }
	    break;
	case 'G':
	case 'g':
	    if (number != 0)
	    {
		total += number * 1024 * 1024 * 1024;
		number = 0;
	    }
	    break;
	case 'M':
	case 'm':
	    if (number != 0)
	    {
		total += number * 1024 * 1024;
		number = 0;
	    }
	    break;
	case 'K':
	case 'k':
	    if (number != 0)
	    {
		total += number * 1024;
		number = 0;
	    }
	    break;
	case 'B':
	case 'b':
	    if (number != 0)
	    {
		total += number;
		number = 0;
	    }
	    break;
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	    number *= 10;
	    number += (unsigned long) (in[i] - '0');
	    break;
	default:
	    RET(0);
	}
    }
    if (number != 0)
	total += number;

    RET(total);
}

void mDES(unsigned char *in, unsigned char *out, size_t size,
	des_key_schedule key1, des_key_schedule key2, int enc)
{
#ifdef HASCRYPT
    DES_LONG tuple[2], t0, t1;
    unsigned char *iptr, *optr, tmp[8];
    int i;

    ACE_OS::memset(out, 0, size);
    for (iptr=in, optr=out, i=0; i<size; i+=8)
    {
	c2l(iptr, t0); tuple[0] = t0;
	c2l(iptr, t1); tuple[1] = t1;
	des_encrypt(tuple, key1, enc ? DES_ENCRYPT : DES_DECRYPT);
	des_encrypt(tuple, key2, enc ? DES_DECRYPT : DES_ENCRYPT);
	des_encrypt(tuple, key1, enc ? DES_ENCRYPT : DES_DECRYPT);
	t0 = tuple[0]; l2c(t0, out);
	t1 = tuple[1]; l2c(t1, out);
    }
#endif
}

