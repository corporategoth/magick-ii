#ifndef WIN32
#pragma interface
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
#ifndef _CRYPTSTREAM_H
#define _CRYPTSTREAM_H
static const char *ident_cryptstream_h = "@(#) $Id$";
/* ========================================================== **
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <magick-devel@magick.tm>:
**
** $Log$
** Revision 1.11  2000/04/06 12:44:09  prez
** Removed SXP and EXPAT directories
**
** Revision 1.10  2000/03/30 11:24:53  prez
** Added threads to the filesys establishment.
**
** Revision 1.9  2000/02/23 12:21:01  prez
** Fixed the Magick Help System (needed to add to ExtractWord).
** Also replaced #pragma ident's with static const char *ident's
** that will be picked up by what or version, and we can now
** dump from a binary what versions of each file were used.
**
** Revision 1.8  2000/02/16 12:59:37  ungod
** fixing for borland compilability
**
** Revision 1.7  2000/02/15 10:37:47  prez
** Added standardized headers to ALL Magick source files, including
** a #pragma ident, and history log.  ALL revisions of files from
** now on should include what changes were made to the files involved.
**
**
** ========================================================== */

#include "mstream.h"
#include "mstring.h"

#ifdef HASCRYPT
extern "C"
{
#include "des/des_locl.h"
};
#endif

class wxCryptInputStream : public wxFilterInputStream
{
public:
    wxCryptInputStream(wxInputStream& stream, const mstring& passphrase);
    //virtual ~mDecryptStream();
protected:
    size_t OnSysRead(void *buffer, size_t size);
private:
#ifdef HASCRYPT
    bool ppgiven;
    des_key_schedule key1;
    des_key_schedule key2;
    des_cblock ivec;
#endif
};

class wxCryptOutputStream : public wxFilterOutputStream
{
public:
    wxCryptOutputStream(wxOutputStream& stream, const mstring& passphrase);
    //virtual ~mEncryptStream();
protected:
    size_t OnSysWrite(const void *buffer, size_t size);
private:
#ifdef HASCRYPT
    bool ppgiven;
    des_key_schedule key1;
    des_key_schedule key2;
    des_cblock ivec;
#endif
};

#endif
