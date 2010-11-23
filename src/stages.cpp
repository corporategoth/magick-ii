#include "pch.h"
#ifdef WIN32
#pragma hdrstop
#else
#pragma implementation
#endif

/* Magick IRC Services
**
** (c) 1997-2003 Preston Elder <prez@magick.tm>
** (c) 1998-2003 William King <ungod@magick.tm>
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
#define RCSID(x,y) const char *rcsid_stages_cpp_ ## x () { return y; }
RCSID(stages_cpp, "@(#)$Id$");

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

long Stage::Consume()
{
    BTCB();
    NFT("Stage::Consume");
    if (input == NULL)
    {
	RET(-1 * static_cast < long > (SE_ConsumeWithoutInput));
    }

    long res, total = 0;
    char buffer[DEF_STAGE_BUFFER];

    COM(("Stage: Reading from previous stage ..."));
    FLUSH();
    while ((res = input->Read(buffer, sizeof(buffer))) > 0)
    {
	CP(("Stage: Read from previous stage returned %d", res));
	FLUSH();
	total += res;
	COM(("Stage: Reading from previous stage ..."));
	FLUSH();
    }
    CP(("Stage: Read from previous stage returned %d", res));
    FLUSH();
    if (res < 0)
    {
	RET(res);
    }
    else
    {
	RET(total);
    }
    ETCB();
}

StringStage::StringStage(const mstring & in)
{
    BTCB();
    FT("StringStage::StringStage", (in));
    input = NULL;
    tag = 0;
    offset = 0;
    i_str = in;
    ETCB();
}

StringStage::StringStage(Stage & PrevStage)
{
    BTCB();
    FT("StringStage::StringStage", ("(Stage &) PrevStage"));
    input = & PrevStage;
    tag = input->GetTag();
    offset = 0;
    ETCB();
}

StringStage::~StringStage()
{
    BTCB();
    NFT("StringStage::~StringStage");
    ETCB();
}

bool StringStage::Validate()
{
    BTCB();
    NFT("StringStage::Validate");
    if (!(input != NULL || i_str.length()))
	RET(false);

    RET(true);
    ETCB();
}

long StringStage::Consume()
{
    BTCB();
    NFT("StringStage::Consume");
    if (input == NULL)
    {
	RET(-1 * static_cast < long > (SE_ConsumeWithoutInput));
    }

    long res, total = 0;
    char buffer[DEF_STAGE_BUFFER];

    COM(("StringStage: Reading from previous stage ..."));
    FLUSH();
    while ((res = input->Read(buffer, sizeof(buffer))) > 0)
    {
	CP(("StringStage: Read from previous stage returned %d", res));
	FLUSH();
	i_str.append(buffer, res);
	total += res;
	COM(("StringStage: Reading from previous stage ..."));
	FLUSH();
    }
    CP(("StringStage: Read from previous stage returned %d", res));
    FLUSH();
    if (res < 0)
    {
	RET(res);
    }
    else
    {
	RET(total);
    }
    ETCB();
}

mstring StringStage::Result()
{
    BTCB();
    NFT("StringStage::Result");
    RET(i_str);
    ETCB();
}

long StringStage::Read(char *buf, size_t size)
{
    BTCB();
    FT("StringStage::Read", ("(char *) buf", size));
    memset(buf, 0, size);
    if (input != NULL)
    {
	RET(-1 * static_cast < long > (SE_ReadWithInput));
    }

    long retval = 0;
    mstring str = i_str.substr(offset, size);

    retval = i_str.length() - offset;
    if (retval > static_cast < long > (size))
	retval = size;
    memcpy(buf, i_str, retval);
    offset += retval;
    RET(retval);
    ETCB();
}

FileStage::FileStage(const mstring & name)
{
    BTCB();
    FT("FileStage::FileStage", (name));
    input = NULL;
    tag = 0;

    file.Open(name, "rb");
    if (file.IsOpened())
	file.Read(&tag, 1);
    ETCB();
}

FileStage::FileStage(Stage & PrevStage, const mstring & name, const mstring & mode)
{
    BTCB();
    FT("FileStage::FileStage", ("(Stage &) PrevStage", name, mode));
    input = & PrevStage;
    tag = input->GetTag();

    file.Open(name, mode);
    if (file.IsOpened())
	file.Write(&tag, 1);
    ETCB();
}

FileStage::~FileStage()
{
    BTCB();
    NFT("FileStage::~FileStage");
    file.Close();
    ETCB();
}

bool FileStage::Validate()
{
    BTCB();
    NFT("FileStage::Validate");
    if (!file.IsOpened())
	RET(false);

    if (input != NULL && !file.IsWritable())
	RET(false);

    RET(true);
    ETCB();
}

long FileStage::Consume()
{
    BTCB();
    NFT("FileStage::Consume");
    if (input == NULL)
    {
	RET(-1 * static_cast < long > (SE_ConsumeWithoutInput));
    }
    if (!file.IsOpened())
    {
	RET(-1 * static_cast < long > (SE_FLE_NotOpened));
    }
    if (!file.IsWritable())
    {
	RET(-1 * static_cast < long > (SE_FLE_NotWritable));
    }

    char buffer[DEF_STAGE_BUFFER];
    long res = 0, total = 0;

    do
    {
	COM(("FileStage: Reading from previous stage ..."));
	FLUSH();
	res = input->Read(buffer, sizeof(buffer));
	CP(("FileStage: Read from previous stage returned %d", res));
	FLUSH();
	if (res > 0)
	{
	    file.Write(buffer, res);
	    total += res;
	}
    } while (res > 0);
    if (res < 0)
    {
	RET(res);
    }
    else
    {
	RET(total);
    }
    ETCB();
}

long FileStage::Read(char *buf, size_t size)
{
    BTCB();
    FT("FileStage::Read", ("(char *) buf", size));
    memset(buf, 0, size);
    if (input != NULL)
    {
	RET(-1 * static_cast < long > (SE_ReadWithInput));
    }
    if (!file.IsOpened())
    {
	RET(-1 * static_cast < long > (SE_FLE_NotOpened));
    }

    long retval = file.Read(buf, size);

    RET(retval);
    ETCB();
}

CryptStage::CryptStage(Stage & PrevStage, const mstring & key1, const mstring & key2)
{
    BTCB();
    FT("CryptStage::CryptStage", ("(Stage &) PrevStage", "(const mstring &) key1", "(const mstring &) key2"));
    input = & PrevStage;
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
    ETCB();
}

CryptStage::~CryptStage()
{
    BTCB();
    NFT("CryptStage::~CryptStage");
    // Security,  dont leave keys around ...
#ifdef HASCRYPT
    memset(&bfkey1, 0, sizeof(bfkey1));
    memset(&bfkey2, 0, sizeof(bfkey2));
#endif
    ETCB();
}

bool CryptStage::Validate()
{
    BTCB();
    NFT("CryptStage::Validate");
    if (input == NULL)
	RET(false);

    if (!gotkeys)
	RET(false);

    RET(true);
    ETCB();
}

long CryptStage::Read(char *buf, size_t size)
{
    BTCB();
    FT("CryptStage::Read", ("(char *) buf", size));
    memset(buf, 0, size);
    if (input == NULL)
    {
	RET(-1 * static_cast < long > (SE_ReadWithoutInput));
    }
    if (!gotkeys)
    {
	RET(-1 * static_cast < long > (SE_CRY_NoKeys));
    }

    unsigned int i = 0;

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
	    for (i = 0; i < size; i++)
		outbuf[i] = outbuf[i + size];
	    for (; i < outbufsize; i++)
		outbuf[i] = 0;
	    outbufsize -= size;
	    RET(size);
	}
    }

    unsigned char buf1[8], buf2[8];
    long res = 0;

    for (; i < size; i += 8)
    {
	memset(buf1, 0, 8);
	memset(buf2, 0, 8);

	if (!lastpos)
	{
	    memset(buffer, 0, sizeof(buffer));
	    COM(("CryptStage: Reading from previous stage ..."));
	    FLUSH();
	    res = input->Read(buffer, sizeof(buffer));
	    CP(("CryptStage: Read from previous stage returned %d", res));
	    FLUSH();
	    if (res < 0)
	    {
		RET(res);
	    }
	    else
	    {
		bufsize = res;
		// go up to even boundary ...
		if ((bufsize % 8) != 0)
		    bufsize += 8 - (bufsize % 8);
	    }
	}
	else if (lastpos >= bufsize)
	    break;

#ifdef HASCRYPT
	BF_cbc_encrypt(reinterpret_cast < unsigned char * > (&buffer[lastpos]), buf2, 8, &bfkey1, ivec1,
		       encrypt ? BF_ENCRYPT : BF_DECRYPT);
	BF_cbc_encrypt(buf2, buf1, 8, &bfkey2, ivec2, encrypt ? BF_DECRYPT : BF_ENCRYPT);
	BF_cbc_encrypt(buf1, buf2, 8, &bfkey1, ivec3, encrypt ? BF_ENCRYPT : BF_DECRYPT);
#else
	memcpy(buf1, buf2, 8);
#endif
	lastpos += 8;
	if (lastpos >= sizeof(buffer))
	    lastpos = 0;

	if (i + 8 < size)
	    memcpy(&buf[i], buf2, 8);
	else
	{
	    memcpy(&buf[i], buf2, size - i);
	    memcpy(outbuf, &buf2[size - i], 8 - (size - i));
	    outbufsize = 8 - (size - i);
	}
    }
    // Ignore trailing null's when decrypting (for what we added ...)
    if (!encrypt)
	while (i > 2 && buf[i - 2] == 0)
	    i--;
    RET(i);
    ETCB();
}

CompressStage::CompressStage(Stage & PrevStage, int level)
{
    BTCB();
    FT("CompressStage::CompressStage", ("Stage &) PrevStage", level));
    input = & PrevStage;
    tag = input->GetTag();

    memset(&strm, 0, sizeof(strm));
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
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
    strm.next_in = reinterpret_cast < Bytef * > (buffer);
    strm.avail_in = 0;
    strm.total_in = 0;
    ETCB();
}

CompressStage::~CompressStage()
{
    BTCB();
    NFT("CompressStage::~CompressStage");
    if (compress)
	deflateEnd(&strm);
    else
	inflateEnd(&strm);
    ETCB();
}

bool CompressStage::Validate()
{
    BTCB();
    NFT("CompressStage::Validate");
    if (input == NULL)
	RET(false);

    RET(true);
    ETCB();
}

long CompressStage::Read(char *buf, size_t size)
{
    BTCB();
    FT("CompressStage::Read", ("(char *) buf", size));
    memset(buf, 0, size);
    if (input == NULL)
    {
	RET(-1 * static_cast < long > (SE_ReadWithoutInput));
    }

    strm.next_out = reinterpret_cast < Bytef * > (buf);
    strm.avail_out = size;

    long res = 0;

    while (res >= 0 && strm.avail_out)
    {
	if (strm.avail_in == 0)
	{
	    memset(buffer, 0, sizeof(buffer));
	    COM(("CompressStage: Reading from previous stage ..."));
	    FLUSH();
	    res = input->Read(buffer, sizeof(buffer));
	    CP(("CompressStage: Read from previous stage returned %d", res));
	    FLUSH();
	    if (res < 0)
	    {
		RET(res);
	    }

	    strm.next_in = reinterpret_cast < Bytef * > (buffer);
	    strm.avail_in = res;
	    if (res == 0)
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
		res = deflate(&strm, Z_NO_FLUSH);
	    else
		res = inflate(&strm, Z_NO_FLUSH);

	    if (res < 0)
	    {
		RET(res);
	    }
	    if (res != Z_OK)
		strm.avail_in = 0;
	}
    }

    RET(size - strm.avail_out);
    ETCB();
}

XMLStage::XMLStage(SXP::IPersistObj * pRoot, SXP::dict & attribs)
{
    BTCB();
    FT("XMLStage::XMLStage", ("(SXP::IPersistObj *) pRoot", "(SXP::dict &) attribs"));
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
    ETCB();
}

XMLStage::XMLStage(Stage & PrevStage, SXP::IPersistObj * pRoot)
{
    BTCB();
    FT("XMLStage::XMLStage", ("(Stage &) PrevStage", "(SXP::IPersistObj *) pRoot"));
    input = & PrevStage;
    tag = input->GetTag();
    tag &= ~STAGE_TAG_XML;

    parser = NULL;
    generator = NULL;
    curpos = 0;
    if (pRoot != NULL)
	parser = new SXP::CParser(pRoot);
    ETCB();
}

XMLStage::~XMLStage()
{
    BTCB();
    NFT("XMLStage::~XMLStage");
    if (parser != NULL)
	delete parser;

    if (generator != NULL)
	delete generator;
    ETCB();
}

bool XMLStage::Validate()
{
    BTCB();
    NFT("XMLStage::Validate");
    if (input == NULL ? generator == NULL : parser == NULL)
	RET(false);

    RET(true);
    ETCB();
}

long XMLStage::Consume()
{
    BTCB();
    NFT("XMLStage::Consume");
    if (input == NULL)
    {
	RET(-1 * static_cast < long > (SE_ConsumeWithoutInput));
    }
    if (parser == NULL)
    {
	RET(-1 * static_cast < long > (SE_XML_NoParser));
    }
    if (generator != NULL)
    {
	RET(-1 * static_cast < long > (SE_XML_HaveGenerator));
    }

    long res = 0, total = 0, xres = SXP::err_no_error;
    char buffer[DEF_STAGE_BUFFER];

    COM(("XMLStage: Reading from previous stage ..."));
    FLUSH();
    while ((xres == SXP::err_no_error) && (res = input->Read(buffer, sizeof(buffer))) > 0)
    {
	CP(("XMLStage: Read from previous stage returned %d", res));
	FLUSH();
	if (res < static_cast < long > (sizeof(buffer)))
	    xres = parser->Feed(buffer, res, 1);
	else
	    xres = parser->Feed(buffer, res, 0);
	COM(("XMLStage: Parser returned %d\n", xres));
	total += res;
	COM(("XMLStage: Reading from previous stage ..."));
	FLUSH();
    }
    parser->DoShutdown();
    CP(("XMLStage: Read from previous stage returned %d", res));
    FLUSH();
    if (res < 0)
    {
	RET(res);
    }
    else if (xres != SXP::err_no_error)
    {
	RET(-1 * static_cast < long > (SE_XML_ParseError));
    }
    else
    {
	RET(total);
    }
    ETCB();
}

long XMLStage::Read(char *buf, size_t size)
{
    BTCB();
    FT("XMLStage::Read", ("(char *) buf", size));
    memset(buf, 0, size);
    if (input != NULL)
    {
	RET(-1 * static_cast < long > (SE_ReadWithInput));
    }
    if (parser != NULL)
    {
	RET(-1 * static_cast < long > (SE_XML_HaveParser));
    }
    if (generator == NULL)
    {
	RET(-1 * static_cast < long > (SE_XML_NoGenerator));
    }

    if (curpos >= generator->BufSize())
    {
	RET(0);
    }

    if (size > generator->BufSize() - curpos)
	size = generator->BufSize() - curpos;
    memcpy(buf, &(generator->Buf()) [curpos], size);
    curpos += size;

    RET(size);
    ETCB();
}

VerifyStage::VerifyStage(Stage & PrevStage, size_t verifyoffset, const char *verifytext, size_t verifysize)
{
    BTCB();
    FT("VerifyStage::VerifyStage", ("(Stage &) PrevStage", verifyoffset, verifytext, verifysize));
    input = & PrevStage;
    tag = input->GetTag();

    text = NULL;
    offset = total = vsize = curpos = 0;
    verified = false;
    if (verifysize != 0)
    {
	offset = verifyoffset;
	vsize = verifysize;
	text = new char [vsize];

	if (text != NULL)
	    memcpy(text, verifytext, verifysize);
    }
    ETCB();
}

VerifyStage::~VerifyStage()
{
    BTCB();
    NFT("VerifyStage::~VerifyStage");
    if (text != NULL)
	delete [] text;
    ETCB();
}

bool VerifyStage::Validate()
{
    BTCB();
    NFT("VerifyStage::Validate");
    if (input == NULL)
	RET(false);

    if (text == NULL || vsize <= 0)
	RET(false);

    RET(true);
    ETCB();
}

long VerifyStage::Read(char *buf, size_t size)
{
    BTCB();
    FT("VerifyStage::Read", ("(char *) buf", size));
    memset(buf, 0, size);
    if (input == NULL)
    {
	RET(-1 * static_cast < long > (SE_ReadWithoutInput));
    }

    size_t end, begin = total;

    COM(("VerifyStage: Reading from previous stage ..."));
    FLUSH();
    long cres, res = input->Read(buf, size);

    CP(("VerifyStage: Read from previous stage returned %d", res));
    FLUSH();
    if (!vsize || res <= 0)
    {
	RET(res);
    }

    total += res;

    // Check condition where we missed the boat!
    if (!verified && begin > offset + vsize)
    {
	RET(-1 * static_cast < long > (SE_VFY_Failed));
    }

    // If the last byte ISNT below our first, and
    // the first byte ISNT above our last
    if (!(total - 1 < offset || begin > offset + vsize))
    {
	if (begin < offset)
	    begin = offset;
	begin -= (total - res);
	end = begin + vsize - curpos;
	if (res < static_cast < long > (end))
	    end = res;

	cres = memcmp(&buf[begin], &text[curpos], end - begin);
	curpos += end - begin;
	if (cres != 0)
	{
	    RET(-1 * static_cast < long > (SE_VFY_Failed));
	}
	if (curpos >= vsize)
	    verified = true;
    }

    RET(res);
    ETCB();
}
