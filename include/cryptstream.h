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
** ==========================================================
#pragma ident "$Id$"
** ==========================================================
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <magick-devel@magick.tm>:
**
** $Log$
** Revision 1.7  2000/02/15 10:37:47  prez
** Added standardized headers to ALL Magick source files, including
** a #pragma ident, and history log.  ALL revisions of files from
** now on should include what changes were made to the files involved.
**
**
** ========================================================== */

#ifndef _CRYPTSTREAM_H
#define _CRYPTSTREAM_H
#include "mstream.h"
#include "mstring.h"
extern "C"
{
#include "des/des_locl.h"
};

class wxCryptInputStream : public wxFilterInputStream
{
public:
    wxCryptInputStream(wxInputStream& stream, const mstring& passphrase);
    //virtual ~mDecryptStream();
protected:
    size_t OnSysRead(void *buffer, size_t size);
private:
    bool ppgiven;
    des_key_schedule key1;
    des_key_schedule key2;
    des_cblock ivec;
};

class wxCryptOutputStream : public wxFilterOutputStream
{
public:
    wxCryptOutputStream(wxOutputStream& stream, const mstring& passphrase);
    //virtual ~mEncryptStream();
protected:
    size_t OnSysWrite(void *buffer, size_t size);
private:
    bool ppgiven;
    des_key_schedule key1;
    des_key_schedule key2;
    des_cblock ivec;
};

#endif
