#ifndef WIN32
#pragma interface
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
#ifndef _STAGES_H
#define _STAGES_H
#include "pch.h"
RCSID(stages_h, "@(#) $Id$");
/* ========================================================== **
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <devel@magick.tm>:
**
** $Log$
** Revision 1.1  2001/06/02 11:04:20  prez
** Initial checkin of stages
**
**
** ========================================================== */

#include "xml/sxp.h"
#ifdef HASCRYPT
#include "crypt/blowfish.h"
#endif

#define	DEF_STAGE_BUFFER	65536

#define STAGE_TAG_XML		0x00000001
#define STAGE_TAG_COMPRESS	0x00000002
#define	STAGE_TAG_CRYPT		0x00000004

enum stage_errors {
	SE_NothingProcessed = 0, SE_ConsumeWithNullInput,
	SE_ReadWithInput, SE_ReadWithNullInput, SE_FileNotOpened,
	SE_FileNotWritable, SE_NoKeys, SE_CryptError,
	SE_CompressError, SE_NoSanity, SE_NoXMLParser,
	SE_ReadWithXMLParser, SE_XMLParseError };

class Stage
{
protected:
    unsigned char tag;

public:
    virtual ~Stage() {}
    virtual unsigned char GetTag() { return tag; }
    virtual long Read(char *buf, size_t size) = 0;
};

class StringStage : public Stage
{
    mstring i_str;
    size_t offset;
    Stage *input;

public:
    StringStage(const mstring &in);
    StringStage(Stage &PrevStage);
    virtual ~StringStage();

    long Consume();
    mstring Result();
    virtual long Read(char *buf, size_t size);
};

class FileStage : public Stage
{
    mFile file;
    Stage *input;

    FileStage();
public:    
    FileStage(const mstring &name);
    FileStage(Stage &PrevStage, const mstring &name, const mstring &mode = "w");
    virtual ~FileStage();

    long Consume();
    virtual long Read(char *buf, size_t size);
};

class CryptStage : public Stage
{
    char buffer[DEF_STAGE_BUFFER];
    unsigned char outbuf[8];
    size_t lastpos, bufsize, outbufsize;
#ifdef HASCRYPT
    BF_KEY bfkey1, bfkey2;
    unsigned char ivec1[8], ivec2[8], ivec3[8];
#endif
    bool encrypt, gotkeys;
    Stage *input;

    CryptStage();
public:
    CryptStage(Stage &PrevStage, const mstring& key1, const mstring& key2);
    virtual ~CryptStage();

    virtual long Read(char *buf, size_t size);
};

class CompressStage : public Stage
{
    char buffer[DEF_STAGE_BUFFER];
    z_stream strm;
    bool compress;
    Stage *input;

    CompressStage();
public:
    CompressStage(Stage &PrevStage, int level = 0);
    virtual ~CompressStage();

    virtual long Read(char *buf, size_t size);
};

class XMLStage : public Stage
{
    SXP::CParser *parser;
    Stage *input;

public:
    XMLStage();
    XMLStage(Stage &PrevStage, SXP::IPersistObj *pRoot);
    virtual ~XMLStage();

    long Consume();
    virtual long Read(char *buf, size_t size);
};

class VerifyStage : public Stage
{
    char *text;
    size_t offset, total, vsize, curpos;
    bool verified;
    Stage *input;

    VerifyStage();
public:
    VerifyStage(Stage &PrevStage, size_t verifyoffset, char *verifytext, size_t verifysize);
    virtual ~VerifyStage();

    virtual long Read(char *buf, size_t size);
};

#endif
