#ifndef _CRYPTSTREAM_H
#define _CRYPTSTREAM_H
#include "mstream.h"
#include "mstring.h"
extern "C"
{
#include "des_locl.h"
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