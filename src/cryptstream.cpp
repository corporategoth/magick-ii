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
** Revision 1.18  2000/05/20 16:05:07  prez
** Finished off the log conversion (still via. wrappers)
**
** Revision 1.17  2000/04/16 06:12:13  prez
** Started adding body to the documentation...
**
** Revision 1.16  2000/04/15 11:11:45  ungod
** starting xmlage of magick
**
** Revision 1.15  2000/04/06 12:52:50  prez
** Various code changes, but mainly added AUTOMAKE/AUTOCONF files :)
**
** Revision 1.14  2000/03/30 11:24:53  prez
** Added threads to the filesys establishment.
**
** Revision 1.13  2000/02/27 03:58:39  prez
** Fixed the WHAT program, also removed RegEx from Magick.
**
** Revision 1.12  2000/02/23 12:21:03  prez
** Fixed the Magick Help System (needed to add to ExtractWord).
** Also replaced #pragma ident's with static const char *ident's
** that will be picked up by what or version, and we can now
** dump from a binary what versions of each file were used.
**
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
#include "trace.h"

#ifdef HASCRYPT
extern "C"
{
#include "des/spr.h"
};
#endif

wxCryptOutputStream::wxCryptOutputStream(wxOutputStream& stream, const mstring& passphrase)
: wxFilterOutputStream(stream)
{
#ifdef HASCRYPT
    des_cblock ckey1, ckey2;
    if(passphrase=="")
    {
	ppgiven=false;
	return;
    }
    des_string_to_2keys((char *)passphrase.c_str(),&ckey1,&ckey2);
    des_set_key(&ckey1,key1);
    des_set_key(&ckey2,key2);
#endif
}

wxCryptInputStream::wxCryptInputStream(wxInputStream& stream, const mstring& passphrase)
: wxFilterInputStream(stream)
{
#ifdef HASCRYPT
    des_cblock ckey1, ckey2;
    if(passphrase=="")
    {
	ppgiven=false;
	return;
    }
    des_string_to_2keys((char *)passphrase.c_str(),&ckey1,&ckey2);
    des_set_key(&ckey1,key1);
    des_set_key(&ckey2,key2);
#endif
}

size_t wxCryptOutputStream::OnSysWrite(const void *buffer, size_t size)
{
    //FT("wxCryptOutputStream::OnSysWrite", ("(const viod *) buffer", size));
    wxASSERT(buffer!=NULL);

#ifdef HASCRYPT
    if(ppgiven==false)
    {
#endif
	Write(buffer,size);
	return size;
	//RET(size);
#ifdef HASCRYPT
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
    //RET(size);
#endif
}

size_t wxCryptInputStream::OnSysRead(void *buffer, size_t size)
{
    //FT("wxCryptOutputStream::OnSysRead", ("(const viod *) buffer", size));
    wxASSERT(buffer!=NULL);
#ifdef HASCRYPT
    if(ppgiven==false)
    {
#endif
	Read(buffer,size);
	return size;
	//RET(size);
#ifdef HASCRYPT
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
    //RET(size);
#endif
}

