#include "pch.h"
#ifdef WIN32
#pragma hdrstop
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
#ifndef WIN32
#pragma ident "$Id$"
#endif
/* ==========================================================
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <magick-devel@magick.tm>:
**
** $Log$
** Revision 1.11  2000/02/17 12:55:05  ungod
** still working on borlandization
**
** Revision 1.10  2000/02/16 12:59:39  ungod
** fixing for borland compilability
**
** Revision 1.9  2000/02/15 13:27:03  prez
** *** empty log message ***
**
** Revision 1.8  2000/02/15 10:37:49  prez
** Added standardized headers to ALL Magick source files, including
** a #pragma ident, and history log.  ALL revisions of files from
** now on should include what changes were made to the files involved.
**
**
** ========================================================== */

#include "cryptstream.h"
#include "log.h"
extern "C"
{
#include "des/spr.h"
};

wxCryptOutputStream::wxCryptOutputStream(wxOutputStream& stream, const mstring& passphrase)
: wxFilterOutputStream(stream)
{
    des_cblock ckey1, ckey2;
    if(passphrase=="")
    {
	ppgiven=false;
	return;
    }
    des_string_to_2keys((char *)passphrase.c_str(),&ckey1,&ckey2);
    des_set_key(&ckey1,key1);
    des_set_key(&ckey2,key2);
}

wxCryptInputStream::wxCryptInputStream(wxInputStream& stream, const mstring& passphrase)
: wxFilterInputStream(stream)
{
    des_cblock ckey1, ckey2;
    if(passphrase=="")
    {
	ppgiven=false;
	return;
    }
    des_string_to_2keys((char *)passphrase.c_str(),&ckey1,&ckey2);
    des_set_key(&ckey1,key1);
    des_set_key(&ckey2,key2);
}

size_t wxCryptOutputStream::OnSysWrite(const void *buffer, size_t size)
{
    wxASSERT(buffer!=NULL);
    if(ppgiven==false)
    {
	Write(buffer,size);
	return size;
    }
    unsigned char *buff=new unsigned char[size];
    unsigned char *buff2=new unsigned char[size];
    try
    {
	des_cfb_encrypt((unsigned char *)buffer,buff2,8,size,key1,&ivec,1);
	des_cfb_encrypt(buff2,buff,8,size,key2,&ivec,0);
	des_cfb_encrypt(buff,buff2,8,size,key1,&ivec,1);
	Write(buff2,size);
    }
    catch(...)
    {
	if(buff!=NULL)
	    delete [] buff;
	if(buff2!=NULL)
	    delete [] buff2;
	throw;
	// below is just to make sure, not sure it's ever used
    }
    if(buff!=NULL)
        delete [] buff;
    if(buff2!=NULL)
        delete [] buff2;
    return size;
}

size_t wxCryptInputStream::OnSysRead(void *buffer, size_t size)
{
    wxASSERT(buffer!=NULL);
    if(ppgiven==false)
    {
	Read(buffer,size);
	return size;
    }
    unsigned char *buff=new unsigned char[size];
    unsigned char *buff2=new unsigned char[size];
    try
    {
	Read(buff,size);
	des_cfb_encrypt(buff,buff2,8,size,key1,&ivec,0);
	des_cfb_encrypt(buff2,buff,8,size,key2,&ivec,1);
	des_cfb_encrypt(buff,(unsigned char *)buffer,8,size,key1,&ivec,0);
    }
    catch(...)
    {
	if(buff!=NULL)
	    delete [] buff;
	if(buff2!=NULL)
	    delete [] buff2;
	throw;
	// below is just to make sure, not sure it's ever used
    }
    if(buff!=NULL)
	delete [] buff;
    if(buff2!=NULL)
        delete [] buff2;
    return size;
}

