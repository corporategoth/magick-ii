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
** Revision 1.7  2002/01/12 14:42:08  prez
** Pretty-printed all code ... looking at implementing an auto-prettyprint.
**
** Revision 1.6  2001/12/25 05:57:27  prez
** Updated SXP and EXPAT -- untested, but should work.
**
** Revision 1.5  2001/11/12 01:05:01  prez
** Added new warning flags, and changed code to reduce watnings ...
**
** Revision 1.4  2001/11/03 21:02:51  prez
** Mammoth change, including ALL changes for beta12, and all stuff done during
** the time GOTH.NET was down ... approx. 3 months.  Includes EPONA conv utils.
**
** Revision 1.3  2001/07/01 05:02:46  prez
** Added changes to dependancy system so it wouldnt just remove a dependancy
** after the first one was satisfied.
**
** Revision 1.2  2001/06/02 16:27:04  prez
** Intergrated the staging system for dbase loading/saving.
**
** Revision 1.1  2001/06/02 11:04:20  prez
** Initial checkin of stages
**
**
** ========================================================== */

#include "xml/sxp.h"

#define	DEF_STAGE_BUFFER	65536
#define STAGE_TAG_XML		0x00000001
#define STAGE_TAG_COMPRESS	0x00000002
#define	STAGE_TAG_CRYPT		0x00000004

enum stage_errors
{
    SE_NothingProcessed = 0, SE_ReadWithInput,
    SE_ReadWithoutInput, SE_ConsumeWithoutInput,
    // String Stage ...
    // File Stage ...
    SE_FLE_NotOpened = 20, SE_FLE_NotWritable,
    // Crypt Stage ...
    SE_CRY_NoKeys = 30, SE_CRY_CryptError,
    // Compress Stage ...
    SE_CPS_StreamError = 40,
    // Verify Stage ...
    SE_VFY_Failed = 50,
    // XML Stage ...
    SE_XML_NoParser = 60, SE_XML_HaveParser,
    SE_XML_NoGenerator, SE_XML_HaveGenerator,
    SE_XML_ParseError
};

class Stage
{
  protected:
    unsigned char tag;
    Stage *input;

  public:
      virtual ~ Stage()
    {
    }

    virtual bool Validate() = 0;
    virtual unsigned char GetTag()
    {
	return tag;
    }
    virtual long Consume();
    virtual long Read(char *buf, size_t size) = 0;
};

class StringStage:public Stage
{
    mstring i_str;
    size_t offset;

      StringStage();
  public:
      StringStage(const mstring & in);
      StringStage(Stage & PrevStage);
      virtual ~ StringStage();

    virtual bool Validate();
    mstring Result();
    virtual long Consume();
    virtual long Read(char *buf, size_t size);
};

class FileStage:public Stage
{
    mFile file;

      FileStage();
  public:
      FileStage(const mstring & name);
      FileStage(Stage & PrevStage, const mstring & name, const mstring & mode = "w");
      virtual ~ FileStage();

    virtual bool Validate();
    virtual long Consume();
    virtual long Read(char *buf, size_t size);
};

class CryptStage:public Stage
{
    char buffer[DEF_STAGE_BUFFER];
    unsigned char outbuf[8];
    size_t lastpos, bufsize, outbufsize;
#ifdef HASCRYPT
    BF_KEY bfkey1, bfkey2;
    unsigned char ivec1[8], ivec2[8], ivec3[8];
#endif
    bool encrypt, gotkeys;

      CryptStage();
  public:
      CryptStage(Stage & PrevStage, const mstring & key1, const mstring & key2);
      virtual ~ CryptStage();

    virtual bool Validate();
    virtual long Read(char *buf, size_t size);
};

class CompressStage:public Stage
{
    char buffer[DEF_STAGE_BUFFER];
    z_stream strm;
    bool compress;

      CompressStage();
  public:
      CompressStage(Stage & PrevStage, int level = 0);
      virtual ~ CompressStage();

    virtual bool Validate();
    virtual long Read(char *buf, size_t size);
};

class XMLStage:public Stage
{
    SXP::MOutStream * generator;
    SXP::CParser * parser;
    size_t curpos;

      XMLStage();
  public:
      XMLStage(SXP::IPersistObj * pRoot, SXP::dict & attribs = SXP::blank_dict);
      XMLStage(Stage & PrevStage, SXP::IPersistObj * pRoot);
      virtual ~ XMLStage();

    virtual bool Validate();
    virtual long Consume();
    virtual long Read(char *buf, size_t size);
};

class VerifyStage:public Stage
{
    char *text;
    size_t offset, total, vsize, curpos;
    bool verified;

      VerifyStage();
  public:
      VerifyStage(Stage & PrevStage, size_t verifyoffset, const char *verifytext, size_t verifysize);
      virtual ~ VerifyStage();

    virtual bool Validate();
    virtual long Read(char *buf, size_t size);
};

#endif
