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
#ifndef _CRYPTSTREAM_H
#define _CRYPTSTREAM_H
#include "mstream.h"
#include "mstring.h"
extern "C"
{
#include "des/des_locl.h"
};

class mDecryptStream : public wxFilterInputStream
{
public:
    mDecryptStream(wxInputStream& stream, const mstring& passphrase);
    //virtual ~mDecryptStream();
protected:
    size_t OnSysRead(void *buffer, size_t size);
private:
    bool ppgiven;
    des_key_schedule key1;
    des_key_schedule key2;
    des_cblock ivec;
};

class mEncryptStream : public wxFilterOutputStream
{
public:
    mEncryptStream(wxOutputStream& stream, const mstring& passphrase);
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
