#include "pch.h"
#ifdef WIN32
#pragma hdrstop
#else
#pragma implementation
#endif

/*  Magick IRC Services
**
** (c) 1997-2001 Preston Elder <prez@magick.tm>
** (c) 1998-2001 William King <ungod@magick.tm>
**
** The above copywright may not be removed under any
** circumstances, however it may be added to if any
** modifications are made to this file.  All modified
** code must be clearly documented and labelled.
**
** ========================================================== */
#define RCSID(x,y) const char *rcsid_stages_cpp_ ## x () { return y; }
RCSID(stages_cpp, "@(#)$Id$");
/* ==========================================================
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <devel@magick.tm>:
**
** $Log$
** Revision 1.6  2001/07/12 04:27:47  prez
** Fixed problem with it cutting encrypted db's just a bit too short.
**
** Revision 1.5  2001/06/07 06:21:06  prez
** Think I fixed staging encryption layer ... ugh.
**
** Revision 1.4  2001/06/07 03:58:05  prez
** Aparently we dont need or want a Feed call with 1
**
** Revision 1.3  2001/06/03 02:12:44  prez
** Fixed problem with compress stage not recognizing its end ...
**
** Revision 1.2  2001/06/02 16:27:04  prez
** Intergrated the staging system for dbase loading/saving.
**
** Revision 1.1  2001/06/02 11:04:20  prez
** Initial checkin of stages
**
**
** ========================================================== */

#include "magick.h"

long Stage::Consume()
{
    if (input == NULL)
	return -1 * (long) SE_ConsumeWithoutInput;

    long res, total = 0;
    char buffer[DEF_STAGE_BUFFER];
    while ((res = input->Read(buffer, sizeof(buffer))) > 0)
    {
	total += res;
    }
    if (res < 0)
	return res;
    else
	return total;
}

StringStage::StringStage(const mstring &in)
{
    input = NULL;
    tag = 0;
    offset = 0;
}

StringStage::StringStage(Stage &PrevStage)
{
    input = &PrevStage;
    tag = input->GetTag();
    offset = 0;
}

StringStage::~StringStage()
{
}

long StringStage::Consume()
{
    if (input == NULL)
	return -1 * (long) SE_ConsumeWithoutInput;

    long res, total = 0;
    char buffer[DEF_STAGE_BUFFER];
    while ((res = input->Read(buffer, sizeof(buffer))) > 0)
    {
	i_str.append(buffer, res);
	total += res;
    }
    if (res < 0)
	return res;
    else
	return total;
}

mstring StringStage::Result()
{
    return i_str; 
}

long StringStage::Read(char *buf, size_t size)
{
    memset(buf, 0, size);
    if (input != NULL)
	return -1 * (long) SE_ReadWithInput;

    long retval = 0;
    mstring str = i_str.substr(offset, size);
    retval = i_str.length() - offset;
    if (retval > static_cast<long>(size))
	retval = size;
    memcpy(buf, i_str, retval);
    offset += retval;
    return retval;
}

FileStage::FileStage(const mstring &name)
{
    input = NULL;
    tag = 0;

    file.Open(name, "r");
    if (file.IsOpened())
	file.Read(&tag, 1);
}

FileStage::FileStage(Stage &PrevStage, const mstring &name, const mstring &mode)
{
    input = &PrevStage;
    tag = input->GetTag();

    file.Open(name, mode);
    if (file.IsOpened())
	file.Write(&tag, 1);
}

FileStage::~FileStage()
{
    file.Close();
}

long FileStage::Consume()
{
    if (input == NULL)
	return -1 * (long) SE_ConsumeWithoutInput;
    if (!file.IsOpened())
	return -1 * (long) SE_FLE_NotOpened;
    if (!file.IsWritable())
	return -1 * (long) SE_FLE_NotWritable;

    char buffer[DEF_STAGE_BUFFER];
    long ret = 0, total = 0;
    do {
	ret = input->Read(buffer, sizeof(buffer));
	if (ret > 0)
	{
	    file.Write(buffer, ret);
	    total += ret;
	}
    } while (ret > 0);
    if (ret < 0)
	return ret;
    else
	return total;
}

long FileStage::Read(char *buf, size_t size)
{
    memset(buf, 0, size);
    if (input != NULL)
	return -1 * (long) SE_ReadWithInput;
    if (!file.IsOpened())
	return -1 * (long) SE_FLE_NotOpened;

    return file.Read(buf, size);
}

CryptStage::CryptStage(Stage &PrevStage, const mstring& key1, const mstring& key2)
{
    input = &PrevStage;
    tag = input->GetTag();

    if (tag & STAGE_TAG_CRYPT)
    {
	encrypt = false;
	tag &= ~STAGE_TAG_CRYPT;
    }
    else
    {
	encrypt = true;
	tag |= STAGE_TAG_CRYPT;
    }

    gotkeys = false;
    if (key1.length() && key2.length())
    {
	gotkeys = true;
#ifdef HASCRYPT
	BF_set_key(&bfkey1, key1.length(), key1.uc_str());
	BF_set_key(&bfkey2, key2.length(), key2.uc_str());
    }
    memset(ivec1, 0, 8);
    memset(ivec2, 0, 8);
    memset(ivec3, 0, 8);
#else
    }
#endif
    memset(buffer, 0, sizeof(buffer));
    memset(outbuf, 0, sizeof(outbuf));
    lastpos = bufsize = outbufsize = 0;
}

CryptStage::~CryptStage()
{
    // Security,  dont leave keys around ...
#ifdef HASCRYPT
    memset(&bfkey1, 0, sizeof(bfkey1));
    memset(&bfkey2, 0, sizeof(bfkey2));
#endif
}

long CryptStage::Read(char *buf, size_t size)
{
    memset(buf, 0, size);
    if (input == NULL)
	return -1 * (long) SE_ReadWithoutInput;
    if (!gotkeys)
	return -1 * (long) SE_CRY_NoKeys;

    unsigned int i=0;
    if (outbufsize)
    {
	if (outbufsize <= size)
	{
	    memcpy(buf, outbuf, outbufsize);
	    i = outbufsize;
	    outbufsize = 0;
	}
	else
	{
	    memcpy(buf, outbuf, size);
	    for (i=0; i<size; i++)
		outbuf[i] = outbuf[i+size];
	    for (; i<outbufsize; i++)
		outbuf[i] = 0;
	    outbufsize -= size;
	    return size;
	}
    }

    unsigned char buf1[8], buf2[8];
    long ret = 0;
    for (; i<size; i+=8)
    {
	memset(buf1, 0, 8);
	memset(buf2, 0, 8);

	if (!lastpos)
	{
	    memset(buffer, 0, sizeof(buffer));
	    ret = input->Read(buffer, sizeof(buffer));
	    if (ret < 0)
		return ret;
	    else
	    {
		bufsize = ret;
		// go up to even boundary ...
		if ((bufsize % 8) != 0)
		    bufsize += 8-(bufsize % 8);
	    }
	}
	else if (lastpos >= bufsize)
	    break;

#ifdef HASCRYPT
	BF_cbc_encrypt(reinterpret_cast<unsigned char *>(&buffer[lastpos]), buf2, 8, &bfkey1, ivec1, encrypt ? BF_ENCRYPT : BF_DECRYPT);
	BF_cbc_encrypt(buf2, buf1, 8, &bfkey2, ivec2, encrypt ? BF_DECRYPT : BF_ENCRYPT);
	BF_cbc_encrypt(buf1, buf2, 8, &bfkey1, ivec3, encrypt ? BF_ENCRYPT : BF_DECRYPT);
#else
	memcpy(buf1, buf2, 8);
#endif
	lastpos += 8;
	if (lastpos >= sizeof(buffer))
	    lastpos = 0;

	if (i+8 < size)
	    memcpy(&buf[i], buf2, 8);
	else
	{
	    memcpy(&buf[i], buf2, size-i);
	    memcpy(outbuf, &buf2[size-i], 8-(size-i));
	    outbufsize = 8-(size-i);
	}
    }
    // Ignore trailing null's ...
    while (buf[i-2]==0) i--;
    return i;
}

CompressStage::CompressStage(Stage &PrevStage, int level)
{
    input = &PrevStage;
    tag = input->GetTag();

    memset(&strm, 0, sizeof(strm));
    strm.zalloc = Z_NULL;
    strm.zfree  = Z_NULL;
    strm.opaque = Z_NULL;

    if (tag & STAGE_TAG_COMPRESS)
    {
	compress = false;
	tag &= ~STAGE_TAG_COMPRESS;
	inflateInit(&strm);
    }
    else
    {
	compress = true;
	tag |= STAGE_TAG_COMPRESS;

	if (level < 0)
	    level = 0;
	if (level > 9)
	    level = 9;
	deflateInit(&strm, level);
    }
    memset(buffer, 0, sizeof(buffer));
    strm.next_in = reinterpret_cast<Bytef *>(buffer);
    strm.avail_in = 0;
    strm.total_in = 0;
}

CompressStage::~CompressStage()
{
    if (compress)
	deflateEnd(&strm);
    else
	inflateEnd(&strm);
}

long CompressStage::Read(char *buf, size_t size)
{
    memset(buf, 0, size);
    if (input == NULL)
	return -1 * (long) SE_ReadWithoutInput;

    strm.next_out = reinterpret_cast<Bytef *>(buf);
    strm.avail_out = size;

    long ret = 0;
    while (ret >= 0 && strm.avail_out)
    {
	if (strm.avail_in == 0)
	{
	    memset(buffer, 0, sizeof(buffer));
	    ret = input->Read(buffer, sizeof(buffer));
	    if (ret < 0)
		return ret;
	    strm.next_in = reinterpret_cast<Bytef *>(buffer);
	    strm.avail_in = ret;
	    if (ret == 0)
	    {
		if (compress)
		   deflate(&strm, Z_FULL_FLUSH);		
		else
		   inflate(&strm, Z_FULL_FLUSH);
		break;
	    }
	}
	else
	{
	    if (compress)
		ret = deflate(&strm, Z_NO_FLUSH);
	    else
		ret = inflate(&strm, Z_NO_FLUSH);

	    if (ret < 0)
		return ret;
	    if (ret != Z_OK)
		strm.avail_in = 0;
	}
    }

    return size - strm.avail_out;
}

XMLStage::XMLStage(SXP::IPersistObj *pRoot, SXP::dict& attribs)
{
    input = NULL;
    tag = STAGE_TAG_XML;

    parser = NULL;
    generator = NULL;
    curpos = 0;
    if (pRoot != NULL)
    {
	generator = new SXP::MOutStream();
	if (generator != NULL)
	{
	    generator->BeginXML();
	    pRoot->WriteElement(generator, attribs);
	}
    }
}

XMLStage::XMLStage(Stage &PrevStage, SXP::IPersistObj *pRoot)
{
    input = &PrevStage;
    tag = input->GetTag();
    tag &= ~STAGE_TAG_XML;

    parser = NULL;
    generator = NULL;
    curpos = 0;
    if (pRoot != NULL)
	parser = new SXP::CParser(pRoot);
}

XMLStage::~XMLStage()
{
    if (parser != NULL)
	delete parser;
    if (generator != NULL)
	delete generator;
}

long XMLStage::Consume()
{
    if (input == NULL)
	return -1 * (long) SE_ConsumeWithoutInput;
    if (parser == NULL)
	return -1 * (long) SE_XML_NoParser;
    if (generator != NULL)
	return -1 * (long) SE_XML_HaveGenerator;

    long res, xres = 1, total = 0;
    char buffer[DEF_STAGE_BUFFER];
    while ((res = input->Read(buffer, sizeof(buffer))) > 0)
    {
	if (res < static_cast<long>(sizeof(buffer)))
	    xres = parser->Feed(buffer, res, 1);
	else
	    xres = parser->Feed(buffer, res, 0);
	total += res;
    }
    if (res < 0)
	return res;
    else if (!xres)
	return -1 * (long) SE_XML_ParseError;
    else
	return total;
}

long XMLStage::Read(char *buf, size_t size)
{
    memset(buf, 0, size);
    if (input != NULL)
	return -1 * (long) SE_ReadWithInput;
    if (parser != NULL)
	return -1 * (long) SE_XML_HaveParser;
    if (generator == NULL)
	return -1 * (long) SE_XML_NoGenerator;

    if (curpos >= generator->BufSize())
	return 0;

    if (size > generator->BufSize() - curpos)
	size = generator->BufSize() - curpos;
    memcpy(buf, &(generator->Buf())[curpos], size);
    curpos += size;

    return size;
}

VerifyStage::VerifyStage(Stage &PrevStage, size_t verifyoffset, const char *verifytext, size_t verifysize)
{
    input = &PrevStage;
    tag = input->GetTag();

    text = NULL;
    offset = total = vsize = curpos = 0;
    verified = false;
    if (verifysize != 0)
    {
	offset = verifyoffset;
	vsize = verifysize;
	text = new char[vsize];
	memcpy(text, verifytext, verifysize);
    }
}

VerifyStage::~VerifyStage()
{
    if (text != NULL)
	delete text;
}

long VerifyStage::Read(char *buf, size_t size)
{
    memset(buf, 0, size);
    if (input == NULL)
	return -1 * (long) SE_ReadWithoutInput;

    size_t end, begin = total;
    long cres, res = input->Read(buf, size);
    if (!vsize || res <= 0)
	return res;

    total += res;    

    // Check condition where we missed the boat!
    if (!verified && begin > offset+vsize)
	return -1 * (long) SE_VFY_Failed;

    // If the last byte ISNT below our first, and
    // the first byte ISNT above our last
    if (!(total-1 < offset || begin > offset+vsize))
    {
	if (begin < offset)
	    begin = offset;
	begin -= (total-res);
	end = begin + vsize - curpos;
	if (res < static_cast<long>(end))
	    end = res;

	cres = memcmp(&buf[begin], &text[curpos], end-begin);
	curpos += end-begin;
	if (cres != 0)
	    return -1 * (long) SE_VFY_Failed;
	if (curpos >= vsize)
	    verified = true;
    }

    return res;    
}

