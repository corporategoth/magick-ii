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

/////////////////////////////////////////////////////////////////////////////
// Name:        stream.cpp
// Purpose:     wxStream base classes
// Author:      Guilhem Lavaux
// Modified by: W. King for use with Magick II
// Created:     11/07/98
// RCS-ID:      $Id$
// Copyright:   (c) Guilhem Lavaux
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////
#include "ace/OS.h"
#include <math.h>
#include <zlib.h>
#include <stdarg.h>
#include "mstream.h"
#include "utils.h"
#include "log.h"

#define BUF_TEMP_SIZE 10000
void ConvertToIeeeExtended(double num, unsigned char *bytes);
double ConvertFromIeeeExtended(const unsigned char *bytes);

#define CHECK_ERROR(err) \
   if (m_stream->m_lasterror == wxStream_NOERROR) \
     m_stream->m_lasterror = err

FILE *fd_invalid=NULL;

wxStreamBuffer::wxStreamBuffer(wxStreamBase& stream, BufMode mode)
{
	m_buffer_start=m_buffer_end=m_buffer_pos=m_wback=NULL;
    m_buffer_size=m_wbacksize=m_wbackcur=0;
    m_fixed=m_flushable=true;
	m_stream=&stream;
    m_mode=mode;
	m_destroybuf=m_destroystream=false;
}

wxStreamBuffer::wxStreamBuffer(BufMode mode)
{
	m_buffer_start=m_buffer_end=m_buffer_pos=m_wback=NULL;
    m_buffer_size=m_wbacksize=m_wbackcur=0;
    m_fixed=true;
	m_flushable=false;
	m_stream=NULL;
    m_mode=mode;
	m_destroybuf=false;
	m_destroystream=true;
	m_stream = new wxStreamBase();
}

wxStreamBuffer::wxStreamBuffer(const wxStreamBuffer& buffer)
{
  m_buffer_start = buffer.m_buffer_start;
  m_buffer_end = buffer.m_buffer_end;
  m_buffer_pos = buffer.m_buffer_pos;
  m_buffer_size = buffer.m_buffer_size;
  m_fixed = buffer.m_fixed;
  m_flushable = buffer.m_flushable;
  m_stream = buffer.m_stream;
  m_mode = buffer.m_mode;
  m_destroybuf = true;
  m_destroystream = false;
  m_wback = NULL;
  m_wbacksize = 0;
  m_wbackcur = 0;
}

wxStreamBuffer::~wxStreamBuffer()
{
  if (m_wback)
	  ACE_OS::free(m_wback);
  if (m_destroybuf)
    delete [] m_buffer_start;
  if (m_destroystream)
    delete m_stream;
}

size_t wxStreamBuffer::WriteBack(const char *buf, size_t bufsize)
{
  char *ptrback;

  ptrback = AllocSpaceWBack(bufsize);
  if (!ptrback)
    return 0;

  ACE_OS::memcpy(ptrback, buf, bufsize);
  return bufsize;
}

bool wxStreamBuffer::WriteBack(char c)
{
  char *ptrback;

  ptrback = AllocSpaceWBack(1);
  if (!ptrback)
    return false;

  *ptrback = c;
  return true;
}

void wxStreamBuffer::SetBufferIO(char *buffer_start, char *buffer_end)
{
  if (m_destroybuf)
    delete [] m_buffer_start;
  m_buffer_start = buffer_start;
  m_buffer_end   = buffer_end;

  m_buffer_size = m_buffer_end-m_buffer_start;
  m_destroybuf = false;
  ResetBuffer();
}

void wxStreamBuffer::SetBufferIO(size_t bufsize)
{
  char *b_start;

  if (m_destroybuf)
    delete [] m_buffer_start;

  if (!bufsize) {
    m_buffer_start = NULL;
    m_buffer_end = NULL;
    m_buffer_pos = NULL;
    m_buffer_size = 0;
    return;
  }

  b_start = new char[bufsize];
  SetBufferIO(b_start, b_start + bufsize);
  m_destroybuf = true;
}

void wxStreamBuffer::ResetBuffer()
{
  m_stream->m_lasterror = wxStream_NOERROR;
  if (m_mode == read)
    m_buffer_pos = m_buffer_end;
  else
    m_buffer_pos = m_buffer_start;
}

char *wxStreamBuffer::AllocSpaceWBack(size_t needed_size)
{
  char *temp_b;

  m_wbacksize += needed_size;

  if (!m_wback)
	  temp_b = (char *)ACE_OS::malloc(m_wbacksize);
  else
	  temp_b = (char *)ACE_OS::realloc(m_wback, m_wbacksize);

  if (!temp_b)
    return NULL;
  m_wback = temp_b;

  return (char *)(m_wback+(m_wbacksize-needed_size));
}

size_t wxStreamBuffer::GetWBack(char *buf, size_t bsize)
{
  size_t s_toget = m_wbacksize-m_wbackcur;

  if (bsize < s_toget)
    s_toget = bsize;

  ACE_OS::memcpy(buf, (m_wback+m_wbackcur), s_toget);

  m_wbackcur += s_toget;
  if (m_wbackcur == m_wbacksize) {
    free(m_wback);
    m_wback = (char *)NULL;
    m_wbacksize = 0;
    m_wbackcur = 0;
  }

  return s_toget;
}

bool wxStreamBuffer::FillBuffer()
{
  size_t count;

  count = m_stream->OnSysRead(m_buffer_start, m_buffer_size);
  m_buffer_end = m_buffer_start+count;
  m_buffer_pos = m_buffer_start;

  if (count == 0)
    return false;
  return true;
}

bool wxStreamBuffer::FlushBuffer()
{
  size_t count, current;

  if (m_buffer_pos == m_buffer_start || !m_flushable)
    return false;

  current = m_buffer_pos-m_buffer_start;
  count = m_stream->OnSysWrite(m_buffer_start, current);
  if (count != current)
    return false;
  m_buffer_pos = m_buffer_start;

  return true;
}

void wxStreamBuffer::GetFromBuffer(void *buffer, size_t size)
{
  size_t s_toget = m_buffer_end-m_buffer_pos;

  if (size < s_toget)
    s_toget = size;

  ACE_OS::memcpy(buffer, m_buffer_pos, s_toget);
  m_buffer_pos += s_toget;
}

void wxStreamBuffer::PutToBuffer(const void *buffer, size_t size)
{
  size_t s_toput = m_buffer_end-m_buffer_pos;

  if (s_toput < size && !m_fixed) {
	  m_buffer_start = (char *)ACE_OS::realloc(m_buffer_start, m_buffer_size+size);
    // I round a bit
    m_buffer_size += size;
    m_buffer_end = m_buffer_start+m_buffer_size;
    s_toput = size;
  }
  if (s_toput > size)
    s_toput = size;
  ACE_OS::memcpy(m_buffer_pos, buffer, s_toput);
  m_buffer_pos += s_toput;
}

void wxStreamBuffer::PutChar(char c)
{
  wxASSERT(m_stream != NULL);

  if (!m_buffer_size) {
    m_stream->OnSysWrite(&c, 1);
    return;
  }

  if (!GetDataLeft() && !FlushBuffer()) {
    CHECK_ERROR(wxStream_READ_ERR);
    return;
  }

  PutToBuffer(&c, 1);
  m_stream->m_lastcount = 1;
}

char wxStreamBuffer::GetChar()
{
  char c;

  wxASSERT(m_stream != NULL);

  if (!m_buffer_size) {
    m_stream->OnSysRead(&c, 1);
    return c;
  }

  if (!GetDataLeft()) {
    CHECK_ERROR(wxStream_READ_ERR);
    return 0;
  }

  GetFromBuffer(&c, 1);
  m_stream->m_lastcount = 1;
  return c;
}

size_t wxStreamBuffer::Read(void *buffer, size_t size)
{
  wxASSERT(m_stream != NULL);

  // ------------------
  // Buffering disabled
  // ------------------

  m_stream->m_lasterror = wxStream_NOERROR;
  m_stream->m_lastcount = GetWBack((char *)buffer, size);
  size -= m_stream->m_lastcount;
  if (size == 0)
    return m_stream->m_lastcount;

  buffer = (void *)((char *)buffer+m_stream->m_lastcount);

  if (!m_buffer_size)
    return (m_stream->m_lastcount += m_stream->OnSysRead(buffer, size));

  // -----------------
  // Buffering enabled
  // -----------------
  size_t buf_left, orig_size = size;

  while (size > 0) {
    buf_left = GetDataLeft();

    // First case: the requested buffer is larger than the stream buffer,
    //             we split it.
    if (size > buf_left) {
      GetFromBuffer(buffer, buf_left);
      size  -= buf_left;
      buffer = (char *)buffer + buf_left; // ANSI C++ violation.

      if (!FillBuffer()) {
        CHECK_ERROR(wxStream_READ_ERR);
        return (m_stream->m_lastcount = orig_size-size);
      }
    } else {

      // Second case: we just copy from the stream buffer.
      GetFromBuffer(buffer, size);
      break;
    }
  }
  return (m_stream->m_lastcount += orig_size);
}

size_t wxStreamBuffer::Read(wxStreamBuffer *s_buf)
{
  char buf[BUF_TEMP_SIZE];
  size_t s = 0, bytes_read = BUF_TEMP_SIZE;

  while (bytes_read == BUF_TEMP_SIZE) {
    bytes_read = Read(buf, bytes_read);
    bytes_read = s_buf->Write(buf, bytes_read);
    s += bytes_read;
  }
  return s;
}

size_t wxStreamBuffer::Write(const void *buffer, size_t size)
{
  wxASSERT(m_stream != NULL);

  // ------------------
  // Buffering disabled
  // ------------------

  m_stream->m_lasterror = wxStream_NOERROR;
  if (!m_buffer_size)
    return (m_stream->m_lastcount = m_stream->OnSysWrite(buffer, size));

  // ------------------
  // Buffering enabled
  // ------------------

  size_t buf_left, orig_size = size;

  while (size > 0) {
    buf_left = m_buffer_end - m_buffer_pos;

    // First case: the buffer to write is larger than the stream buffer,
    //             we split it
    if (size > buf_left) {
      PutToBuffer(buffer, buf_left);
      size -= buf_left;
      buffer = (char *)buffer + buf_left; // ANSI C++ violation.

      if (!FlushBuffer()) {
        CHECK_ERROR(wxStream_WRITE_ERR);
        return (m_stream->m_lastcount = orig_size-size);
      }

      m_buffer_pos = m_buffer_start;

    } else {

      // Second case: just copy it in the stream buffer.
      PutToBuffer(buffer, size);
      break;
    }
  }
  return (m_stream->m_lastcount = orig_size);
}

size_t wxStreamBuffer::Write(wxStreamBuffer *sbuf)
{
  char buf[BUF_TEMP_SIZE];
  size_t s = 0, bytes_count = BUF_TEMP_SIZE, b_count2;

  while (bytes_count == BUF_TEMP_SIZE) {
    b_count2 = sbuf->Read(buf, bytes_count);
    bytes_count = Write(buf, b_count2);
    if (b_count2 > bytes_count)
      sbuf->WriteBack(buf+bytes_count, b_count2-bytes_count);
    s += bytes_count;
  }
  return s;
}

off_t wxStreamBuffer::Seek(off_t pos, wxSeekMode mode)
{
  off_t ret_off, diff, last_access;

  last_access = GetLastAccess();

  if (!m_flushable) {
    diff = pos + GetIntPosition();
    if (diff < 0 || diff > last_access)
      return -1;
    SetIntPosition(diff);
    return diff;
  }

  switch (mode) {
  case wxFromStart: {
    // We'll try to compute an internal position later ...
    ret_off = m_stream->OnSysSeek(pos, wxFromStart);
    ResetBuffer();
    return ret_off;
  }
  case wxFromCurrent: {
    diff = pos + GetIntPosition();

    if ( (diff > last_access) || (diff < 0) ) {
      ret_off = m_stream->OnSysSeek(pos, wxFromCurrent);
      ResetBuffer();
      return ret_off;
    } else {
      SetIntPosition(diff);
      return pos;
    }
  }
  case wxFromEnd:
    // Hard to compute: always seek to the requested position.
    ret_off = m_stream->OnSysSeek(pos, wxFromEnd);
    ResetBuffer();
    return ret_off;
  }
  return -1;
}

off_t wxStreamBuffer::Tell() const
{
  off_t pos;

  if (m_flushable) {
    pos = m_stream->OnSysTell();
    if (pos == -1)
      return -1;
    return pos - GetLastAccess() + GetIntPosition();
  } else
    return GetIntPosition();
}

size_t wxStreamBuffer::GetDataLeft()
{
  if (m_buffer_end == m_buffer_pos && m_flushable)
    FillBuffer();
  return m_buffer_end-m_buffer_pos;
}

// ----------------------------------------------------------------------------
// wxStreamBase
// ----------------------------------------------------------------------------

wxStreamBase::wxStreamBase()
{
  m_lasterror = wxStream_NOERROR;
  m_lastcount = 0;
}

wxStreamBase::~wxStreamBase()
{
}

size_t wxStreamBase::OnSysRead(void *buffer, size_t size)
{
  return 0;
}

size_t wxStreamBase::OnSysWrite(const void *buffer, size_t bufsize)
{
  return 0;
}

off_t wxStreamBase::OnSysSeek(off_t seek, wxSeekMode mode)
{
  return -1;
}

off_t wxStreamBase::OnSysTell() const
{
  return -1;
}

// ----------------------------------------------------------------------------
// wxInputStream
// ----------------------------------------------------------------------------

wxInputStream::wxInputStream()
  : wxStreamBase()
{
  m_i_destroybuf = true;
  m_i_streambuf = new wxStreamBuffer(*this, wxStreamBuffer::read);
}

wxInputStream::wxInputStream(wxStreamBuffer *buffer)
  : wxStreamBase()
{
  m_i_destroybuf = false;
  m_i_streambuf = buffer;
}

wxInputStream::~wxInputStream()
{
  if (m_i_destroybuf)
    delete m_i_streambuf;
}

char wxInputStream::GetC()
{
  char c;
  m_i_streambuf->Read(&c, 1);
  return c;
}

wxInputStream& wxInputStream::Read(void *buffer, size_t size)
{
  m_i_streambuf->Read(buffer, size);
  // wxStreamBuffer sets all variables for us
  return *this;
}

char wxInputStream::Peek()
{
  m_i_streambuf->GetDataLeft();

  return *(m_i_streambuf->GetBufferPos());
}

wxInputStream& wxInputStream::Read(wxOutputStream& stream_out)
{
  char buf[BUF_TEMP_SIZE];
  size_t bytes_read = BUF_TEMP_SIZE;

  while (bytes_read == BUF_TEMP_SIZE) {
    bytes_read = Read(buf, bytes_read).LastRead();
    bytes_read = stream_out.Write(buf, bytes_read).LastWrite();
  }
  return *this;
}

off_t wxInputStream::SeekI(off_t pos, wxSeekMode mode)
{
  return m_i_streambuf->Seek(pos, mode);
}

off_t wxInputStream::TellI() const
{
  return m_i_streambuf->Tell();
}

// --------------------
// Overloaded operators
// --------------------

wxInputStream& wxInputStream::operator>>(mstring& line)
{
  wxDataInputStream s(*this);

  line = s.ReadLine();
  return *this;
}

wxInputStream& wxInputStream::operator>>(char& c)
{
  c = GetC();
  return *this;
}

wxInputStream& wxInputStream::operator>>(short& i)
{
	Read(&i,sizeof(short));  
	return *this;
}

wxInputStream& wxInputStream::operator>>(int& i)
{
	Read(&i,sizeof(int));
	return *this;
}

wxInputStream& wxInputStream::operator>>(long& i)
{
	Read(&i,sizeof(long));
	return *this;
}

wxInputStream& wxInputStream::operator>>(double& f)
{
	Read(&f,sizeof(double));
	return *this;
}

// ----------------------------------------------------------------------------
// wxOutputStream
// ----------------------------------------------------------------------------
wxOutputStream::wxOutputStream()
  : wxStreamBase()
{
  m_o_destroybuf = true;
  m_o_streambuf = new wxStreamBuffer(*this, wxStreamBuffer::write);
}

wxOutputStream::wxOutputStream(wxStreamBuffer *buffer)
  : wxStreamBase()
{
  m_o_destroybuf = false;
  m_o_streambuf = buffer;
}

wxOutputStream::~wxOutputStream()
{
  if (m_o_destroybuf)
    delete m_o_streambuf;
}

wxOutputStream& wxOutputStream::Write(const void *buffer, size_t size)
{
  m_o_streambuf->Write(buffer, size);
  return *this;
}

wxOutputStream& wxOutputStream::Write(wxInputStream& stream_in)
{
  stream_in.Read(*this);
  return *this;
}

off_t wxOutputStream::TellO() const
{
  return m_o_streambuf->Tell();
}

off_t wxOutputStream::SeekO(off_t pos, wxSeekMode mode)
{
  return m_o_streambuf->Seek(pos, mode);
}

void wxOutputStream::Sync()
{
  m_o_streambuf->FlushBuffer();
}

wxOutputStream& wxOutputStream::operator<<(const char *string)
{
  return Write(string, strlen(string));
}

wxOutputStream& wxOutputStream::operator<<(mstring& string)
{
  return Write(string, string.length());
}

wxOutputStream& wxOutputStream::operator<<(char c)
{
  return Write(&c, 1);
}

wxOutputStream& wxOutputStream::operator<<(short i)
{
  return Write(&i, sizeof(short));
}

wxOutputStream& wxOutputStream::operator<<(int i)
{
  return Write(&i, sizeof(int));
}

wxOutputStream& wxOutputStream::operator<<(long i)
{
  return Write(&i, sizeof(long));
}

wxOutputStream& wxOutputStream::operator<<(double f)
{
  return Write(&f, sizeof(double));
}

// ----------------------------------------------------------------------------
// wxFilterInputStream
// ----------------------------------------------------------------------------
wxFilterInputStream::wxFilterInputStream()
  : wxInputStream(NULL)
{
  // WARNING streambuf set to NULL !
}

wxFilterInputStream::wxFilterInputStream(wxInputStream& stream)
  : wxInputStream(stream.InputStreamBuffer())
{
  m_parent_i_stream = &stream;
}

wxFilterInputStream::~wxFilterInputStream()
{
}

// ----------------------------------------------------------------------------
// wxFilterOutputStream
// ----------------------------------------------------------------------------
wxFilterOutputStream::wxFilterOutputStream()
  : wxOutputStream(NULL)
{
}

wxFilterOutputStream::wxFilterOutputStream(wxOutputStream& stream)
  : wxOutputStream(stream.OutputStreamBuffer())
{
  m_parent_o_stream = &stream;
}

wxFilterOutputStream::~wxFilterOutputStream()
{
}

// ----------------------------------------------------------------------------
// Some IOManip function
// ----------------------------------------------------------------------------

wxOutputStream& wxEndL(wxOutputStream& stream)
{
#ifdef WIN32
  return stream.Write("\r\n", 2);
#else
  return stream.Write("\n", 1);
#endif
}

// ---------------------------------------------------------------------------
// wxDataInputStream
// ---------------------------------------------------------------------------

wxDataInputStream::wxDataInputStream(wxInputStream& s)
  : wxFilterInputStream(s)
{
}

wxDataInputStream::~wxDataInputStream()
{
}

unsigned long wxDataInputStream::Read32()
{
  char buf[4];

  Read(buf, 4);

  return (unsigned long)buf[0] |
         ((unsigned long)buf[1] << 8) |
         ((unsigned long)buf[2] << 16) |
         ((unsigned long)buf[3] << 24);
}

unsigned short wxDataInputStream::Read16()
{
  char buf[2];

  Read(buf, 2);

  return (unsigned short)buf[0] |
         ((unsigned short)buf[1] << 8);
}

unsigned char wxDataInputStream::Read8()
{
  char buf;

  Read(&buf, 1);
  return (unsigned char)buf;
}

double wxDataInputStream::ReadDouble()
{
  char buf[10];

  Read(buf, 10);
  return ConvertFromIeeeExtended((unsigned char *)buf);
}

mstring wxDataInputStream::ReadLine()
{
  char c, last_endl = 0;
  bool end_line = false;
  mstring line;

  while (!end_line) {
    c = GetC();
    switch (c) {
    case '\n':
      end_line = true;
      break;
    case '\r':
      last_endl = '\r';
      break;
    default:
      if (last_endl == '\r') {
        end_line = true;
        InputStreamBuffer()->WriteBack(c);
        break;
      }
      line =line + mstring(c);
      break;
    }
  }
  return line;
}

mstring wxDataInputStream::ReadString()
{
  mstring wx_string;
  char *string;
  unsigned long len;

  len = Read32();
  string = new char[len+1];

  Read(string, len);

  string[len] = 0;
  wx_string = string;
  delete string;

  return wx_string;
}

// ---------------------------------------------------------------------------
// wxDataOutputStream
// ---------------------------------------------------------------------------

wxDataOutputStream::wxDataOutputStream(wxOutputStream& s)
 : wxFilterOutputStream(s)
{
}

wxDataOutputStream::~wxDataOutputStream()
{
}

void wxDataOutputStream::Write32(unsigned long i)
{
  char buf[4];

  buf[0] = i & 0xff;
  buf[1] = (i >> 8) & 0xff;
  buf[2] = (i >> 16) & 0xff;
  buf[3] = (i >> 24) & 0xff;
  Write(buf, 4);
}

void wxDataOutputStream::Write16(unsigned short i)
{
  char buf[2];

  buf[0] = i & 0xff;
  buf[1] = (i >> 8) & 0xff;
  Write(buf, 2);
}

void wxDataOutputStream::Write8(unsigned char i)
{
  Write(&i, 1);
}

void wxDataOutputStream::WriteLine(const mstring& line)
{
#ifdef WIN32
  mstring tmp_string = line + "\r\n";
#else
  mstring tmp_string = line + '\n';
#endif

  Write(tmp_string.c_str(), tmp_string.length());
}

void wxDataOutputStream::WriteString(const mstring& string)
{
  Write32(string.length());
  Write(string.c_str(), string.length());
}

void wxDataOutputStream::WriteDouble(double d)
{
  char buf[10];

  ConvertToIeeeExtended(d, (unsigned char *)buf);
  Write(buf, 10);
}

/*
 * C O N V E R T _ T O   I E E E   E X T E N D E D
 */

/* Copyright (C) 1988-1991 Apple Computer, Inc.
 * All rights reserved.
 *
 * Machine-independent I/O routines for IEEE floating-point numbers.
 *
 * NaN's and infinities are converted to HUGE_VAL or HUGE, which
 * happens to be infinity on IEEE machines.      Unfortunately, it is
 * impossible to preserve NaN's in a machine-independent way.
 * Infinities are, however, preserved on IEEE machines.
 *
 * These routines have been tested on the following machines:
 *    Apple Macintosh, MPW 3.1 C compiler
 *    Apple Macintosh, THINK C compiler
 *    Silicon Graphics IRIS, MIPS compiler
 *    Cray X/MP and Y/MP
 *    Digital Equipment VAX
 *
 *
 * Implemented by Malcolm Slaney and Ken Turkowski.
 *
 * Malcolm Slaney contributions during 1988-1990 include big- and little-
 * endian file I/O, conversion to and from Motorola's extended 80-bit
 * floating-point format, and conversions to and from IEEE single-
 * precision floating-point format.
 *
 * In 1991, Ken Turkowski implemented the conversions to and from
 * IEEE double-precision format, added more precision to the extended
 * conversions, and accommodated conversions involving +/- infinity,
 * NaN's, and denormalized numbers.
 */

#ifndef HUGE_VAL
#define HUGE_VAL HUGE
#endif /*HUGE_VAL*/

#define FloatToUnsigned(f) ((unsigned long) (((long) (f - 2147483648.0)) + 2147483647L) + 1)

void ConvertToIeeeExtended(double num, unsigned char *bytes)
{
        int                             sign;
        int                             expon;
        double                  fMant, fsMant;
        unsigned long   hiMant, loMant;

        if (num < 0) {
                sign = 0x8000;
                num *= -1;
        } else {
                sign = 0;
        }

        if (num == 0) {
                expon = 0; hiMant = 0; loMant = 0;
        }
        else {
                fMant = frexp(num, &expon);
                if ((expon > 16384) || !(fMant < 1)) {    /* Infinity or NaN */
                        expon = sign|0x7FFF; hiMant = 0; loMant = 0; /* infinity */
                }
                else {    /* Finite */
                        expon += 16382;
                        if (expon < 0) {        /* denormalized */
                                fMant = ldexp(fMant, expon);
                                expon = 0;
                        }
                        expon |= sign;
                        fMant = ldexp(fMant, 32);
                        fsMant = floor(fMant);
                        hiMant = FloatToUnsigned(fsMant);
                        fMant = ldexp(fMant - fsMant, 32);
                        fsMant = floor(fMant);
                        loMant = FloatToUnsigned(fsMant);
                }
        }

        bytes[0] = expon >> 8;
        bytes[1] = expon;
        bytes[2] = (unsigned char) hiMant >> 24;
        bytes[3] = (unsigned char) hiMant >> 16;
        bytes[4] = (unsigned char) hiMant >> 8;
        bytes[5] = (unsigned char) hiMant;
        bytes[6] = (unsigned char) loMant >> 24;
        bytes[7] = (unsigned char) loMant >> 16;
        bytes[8] = (unsigned char) loMant >> 8;
        bytes[9] = (unsigned char) loMant;
}

/*
 * C O N V E R T   F R O M       I E E E   E X T E N D E D
 */

/*
 * Copyright (C) 1988-1991 Apple Computer, Inc.
 * All rights reserved.
 *
 * Machine-independent I/O routines for IEEE floating-point numbers.
 *
 * NaN's and infinities are converted to HUGE_VAL or HUGE, which
 * happens to be infinity on IEEE machines.      Unfortunately, it is
 * impossible to preserve NaN's in a machine-independent way.
 * Infinities are, however, preserved on IEEE machines.
 *
 * These routines have been tested on the following machines:
 *        Apple Macintosh, MPW 3.1 C compiler
 *        Apple Macintosh, THINK C compiler
 *        Silicon Graphics IRIS, MIPS compiler
 *        Cray X/MP and Y/MP
 *        Digital Equipment VAX
 *
 *
 * Implemented by Malcolm Slaney and Ken Turkowski.
 *
 * Malcolm Slaney contributions during 1988-1990 include big- and little-
 * endian file I/O, conversion to and from Motorola's extended 80-bit
 * floating-point format, and conversions to and from IEEE single-
 * precision floating-point format.
 *
 * In 1991, Ken Turkowski implemented the conversions to and from
 * IEEE double-precision format, added more precision to the extended
 * conversions, and accommodated conversions involving +/- infinity,
 * NaN's, and denormalized numbers.
 */


#ifndef HUGE_VAL
# define HUGE_VAL HUGE
#endif /*HUGE_VAL*/

# define UnsignedToFloat(u) (((double) ((long) (u - 2147483647L - 1))) + 2147483648.0)

/****************************************************************
 * Extended precision IEEE floating-point conversion routine.
 ****************************************************************/

double ConvertFromIeeeExtended(const unsigned char *bytes)
{
        double                  f;
        int                             expon;
        unsigned long   hiMant, loMant;

        expon = ((bytes[0] & 0x7F) << 8) | (bytes[1] & 0xFF);
        hiMant = ((unsigned long)(bytes[2] & 0xFF) << 24)
                | ((unsigned long) (bytes[3] & 0xFF) << 16)
                | ((unsigned long) (bytes[4] & 0xFF) << 8)
                | ((unsigned long) (bytes[5] & 0xFF));
        loMant = ((unsigned long) (bytes[6] & 0xFF) << 24)
                | ((unsigned long) (bytes[7] & 0xFF) << 16)
                | ((unsigned long) (bytes[8] & 0xFF) << 8)
                | ((unsigned long) (bytes[9] & 0xFF));

        if (expon == 0 && hiMant == 0 && loMant == 0) {
                f = 0;
        }
        else {
                if (expon == 0x7FFF) {    /* Infinity or NaN */
                        f = HUGE_VAL;
                }
                else {
                        expon -= 16383;
                        f  = ldexp(UnsignedToFloat(hiMant), expon-=31);
                        f += ldexp(UnsignedToFloat(loMant), expon-=32);
                }
        }

        if (bytes[0] & 0x80)
                return -f;
        else
                return f;
}

// ----------------------------------------------------------------------------
// wxMemoryInputStream
// ----------------------------------------------------------------------------

wxMemoryInputStream::wxMemoryInputStream(const char *data, size_t len)
  : wxInputStream()
{
  m_i_streambuf->SetBufferIO((char*) data, (char*) (data+len));
  m_i_streambuf->Fixed(true);
}

wxMemoryInputStream::~wxMemoryInputStream()
{
}

char wxMemoryInputStream::Peek()
{
  return m_i_streambuf->GetBufferStart()[m_i_streambuf->GetIntPosition()];
}

// ----------------------------------------------------------------------------
// wxMemoryOutputStream
// ----------------------------------------------------------------------------

wxMemoryOutputStream::wxMemoryOutputStream(char *data, size_t len)
  : wxOutputStream()
{
  if (data)
    m_o_streambuf->SetBufferIO(data, data+len);
  m_o_streambuf->Fixed(true);
}

wxMemoryOutputStream::~wxMemoryOutputStream()
{
}

#define ZSTREAM_BUFFER_SIZE 1024

//////////////////////
// wxZlibInputStream
//////////////////////

wxZlibInputStream::wxZlibInputStream(wxInputStream& stream)
  : wxFilterInputStream(stream)
{
  int err;

  // I need a private stream buffer.
  m_i_streambuf = new wxStreamBuffer(*this, wxStreamBuffer::read);
  m_i_destroybuf = true;
  m_inflate = new z_stream_s;

  m_inflate->zalloc = (alloc_func)0;
  m_inflate->zfree = (free_func)0;
  m_inflate->opaque = (voidpf)0;

  err = inflateInit(m_inflate);
  if (err != Z_OK) {
    inflateEnd(m_inflate);
    delete m_inflate;
    return;
  }

  m_z_buffer = new unsigned char[ZSTREAM_BUFFER_SIZE];
  m_z_size = ZSTREAM_BUFFER_SIZE;

  m_inflate->avail_in = 0;
  m_inflate->next_in = NULL;
}

wxZlibInputStream::~wxZlibInputStream()
{
  inflateEnd(m_inflate);
  delete m_inflate;
}
size_t wxZlibInputStream::OnSysRead(void *buffer, size_t size)
{
  int err;

  m_inflate->next_out = (unsigned char *)buffer;
  m_inflate->avail_out = size;

  while (m_inflate->avail_out > 0) {
    if (m_inflate->avail_in == 0) {

      m_parent_i_stream->Read(m_z_buffer, m_z_size);
      m_inflate->next_in = m_z_buffer;
      m_inflate->avail_in = m_parent_i_stream->LastRead();

      if (m_parent_i_stream->LastError() != wxStream_NOERROR)
        return (size - m_inflate->avail_in);
    }
    err = inflate(m_inflate, Z_FINISH);
    if (err == Z_STREAM_END)
      return (size - m_inflate->avail_in);
  }

  return size-m_inflate->avail_in;
}

//////////////////////
// wxZlibOutputStream
//////////////////////

wxZlibOutputStream::wxZlibOutputStream(wxOutputStream& stream)
 : wxFilterOutputStream(stream)
{
  int err;

  m_o_streambuf = new wxStreamBuffer(*this, wxStreamBuffer::write);
  m_o_destroybuf = true;
  m_deflate = new z_stream_s;

  m_deflate->zalloc = (alloc_func)0;
  m_deflate->zfree = (free_func)0;
  m_deflate->opaque = (voidpf)0;

  err = deflateInit(m_deflate, Z_DEFAULT_COMPRESSION);
  if (err != Z_OK) {
    deflateEnd(m_deflate);
    return;
  }

  m_z_buffer = new unsigned char[ZSTREAM_BUFFER_SIZE];
  m_z_size = ZSTREAM_BUFFER_SIZE;

  m_deflate->avail_in = 0;
  m_deflate->next_out = m_z_buffer;
  m_deflate->avail_out = m_z_size;
}

wxZlibOutputStream::~wxZlibOutputStream()
{
  int err;

  Sync();

  err = deflate(m_deflate, Z_FINISH);
  if (err != Z_STREAM_END)
  {
    wxLogDebug( "wxZlibOutputStream: an error occured while closing the stream.\n" );
    return;
  }

  deflateEnd(m_deflate);

  delete[] m_z_buffer;
}

void wxZlibOutputStream::Sync()
{
  int err;

  m_parent_o_stream->Write(m_z_buffer, m_z_size-m_deflate->avail_out);
  m_deflate->next_out  = m_z_buffer;
  m_deflate->avail_out = m_z_size;

  err = deflate(m_deflate, Z_FULL_FLUSH);
  if (err != Z_OK) {
    return;
  }

  m_parent_o_stream->Write(m_z_buffer, m_z_size-m_deflate->avail_out);
  m_deflate->next_out  = m_z_buffer;
  m_deflate->avail_out = m_z_size;
}

size_t wxZlibOutputStream::OnSysWrite(const void *buffer, size_t size)
{
  int err;

  m_deflate->next_in = (unsigned char *)buffer;
  m_deflate->avail_in = size;

  while (m_deflate->avail_in > 0) {

    if (m_deflate->avail_out == 0) {
      m_parent_o_stream->Write(m_z_buffer, m_z_size);
      if (m_parent_o_stream->LastError() != wxStream_NOERROR)
        return (size - m_deflate->avail_in);

      m_deflate->next_out = m_z_buffer;
      m_deflate->avail_out = m_z_size;
    }

    err = deflate(m_deflate, Z_NO_FLUSH);
    if (err != Z_OK)
      return (size - m_deflate->avail_in);
  }
  return size;
}

#ifdef __BORLANDC__
  #define   W_OK        2
  #define   R_OK        4
#endif

// there is no distinction between text and binary files under Unix
#if defined(__UNIX__) || defined(__linux__)
  #define   O_BINARY    (0)
#endif  //__UNIX__

bool wxFile::Exists(const char *name)
{
  struct stat st;
  return !access(name, 0) && !stat((char*) name, &st) && (st.st_mode & S_IFREG);
}

bool wxFile::Access(const char *name, OpenMode mode)
{
  mstring how = "";

  switch ( mode ) {
  case wxFile::read:
      how = "r";
      break;

    case wxFile::write:
      how = "w";
      break;

    default:
      wxFAIL_MSG("bad wxFile::Access mode parameter.");
  }
  FILE *tmpptr=fopen(name,how.c_str());
  if(tmpptr==NULL)
    return false;
  fclose(tmpptr);
  return true;
}

// ----------------------------------------------------------------------------
// opening/closing
// ----------------------------------------------------------------------------

// ctors
wxFile::wxFile(const char *szFileName, OpenMode mode)
{
  m_fd = fd_invalid;
  m_error = false;

  Open(szFileName, mode);
}

// dtor
wxFile::~wxFile()
{
  Close();
}

// create the file, fail if it already exists and bOverwrite
bool wxFile::Create(const char *szFileName, bool bOverwrite)
{
  // if bOverwrite we create a new file or truncate the existing one,
  // otherwise we only create the new file and fail if it already exists
  FILE *fd=NULL;
  if(bOverwrite==true)
	fd = fopen(szFileName, "wb");
  else
	if(Exists(szFileName))
	    return false;
	else
	    fd=fopen(szFileName, "wb");


  if ( fd == NULL ) {
    wxLogSysError("can't create file '%s'", szFileName);
    return false;
  }
  else {
    Attach(fd);
    return true;
  }
}

// open the file
bool wxFile::Open(const char *szFileName, OpenMode mode)
{
  mstring flags = "b";

  switch ( mode ) {
  case wxFile::read:
      flags = "rb";
      break;

  case wxFile::write:
      flags = "wb";
      break;

  case wxFile::write_append:
      flags = "ab+";
      break;

  case wxFile::read_write:
      flags = "rb+";
      break;
  }

  FILE *fd = fopen(szFileName, flags);

  if ( fd == NULL ) {
    wxLogSysError("can't open file '%s'", szFileName);
    return false;
  }
  else {
    Attach(fd);
    return true;
  }
}

// close
bool wxFile::Close()
{
  if ( IsOpened() ) {
    if ( fclose(m_fd) != 0 ) {
      wxLogSysError("can't close file: %p", m_fd);
      m_fd = fd_invalid;
      return false;
    }
    else
      m_fd = fd_invalid;
  }

  return true;
}

// ----------------------------------------------------------------------------
// read/write
// ----------------------------------------------------------------------------

// read
off_t wxFile::Read(void *pBuf, off_t nCount)
{
  wxCHECK( (pBuf != NULL) && IsOpened(), 0 );

  size_t iRc = fread(pBuf, 1, nCount, m_fd);
  if ( ferror(m_fd) ) {
    wxLogSysError("can't read from file: %p", m_fd);
    return -1;
  }
  else
    return iRc;
}

// write
size_t wxFile::Write(const void *pBuf, size_t nCount)
{
  wxCHECK( (pBuf != NULL) && IsOpened(), 0 );

  size_t iRc = fwrite(pBuf, 1, nCount, m_fd);
  if ( ferror(m_fd) ) 
  {
    wxLogSysError("can't write to file: %p", m_fd);
    m_error = true;
    return 0;
  }
  else
    return iRc;
}

// flush
bool wxFile::Flush()
{
    if ( IsOpened() ) 
    {
	if ( fflush(m_fd) != 0 )
	{
	    wxLogSysError("can't flush file: %p", m_fd);
	    return false;
	}
    }
    return true;
}

// ----------------------------------------------------------------------------
// seek
// ----------------------------------------------------------------------------

// seek
int wxFile::Seek(long ofs, wxSeekMode mode) const
{
  wxASSERT( IsOpened() );

  size_t iRc = fseek(m_fd, ofs, mode);
  if ( iRc != 0 ) 
  {
    wxLogSysError("can't seek on file: %p", m_fd);
    return -1;
  }
  else
    return iRc;
}

// get current off_t
long wxFile::Tell() const
{
  wxASSERT( IsOpened() );

  long iRc = ftell(m_fd);
  if ( iRc == -1 ) {
    wxLogSysError("can't get seek position on file: %p", m_fd);
    return -1;
  }
  else
    return iRc;
}

// get current file length
off_t wxFile::Length() const
{
  wxASSERT( IsOpened() );

    int iRc = Tell();
    if(iRc==-1)
    {
	wxLogSysError("can't find length of file: %p", m_fd);
	return -1;
    }
    if(SeekEnd()==-1)
    {
	wxLogSysError("can't find length of file: %p", m_fd);
	return -1;
    }
    int iLen = Tell();
    Seek(iRc);

    if ( iLen == -1 ) 
    {
	wxLogSysError("can't find length of file on file descriptor %d", m_fd);
	return -1;
    }
    else
	return iRc;
}

// is end of file reached?
bool wxFile::Eof() const
{
  wxASSERT( IsOpened() );

  return feof(m_fd)!=0;
}

// ============================================================================
// implementation of wxTempFile
// ============================================================================

// ----------------------------------------------------------------------------
// construction
// ----------------------------------------------------------------------------
wxTempFile::wxTempFile(const mstring& strName)
{
  Open(strName);
}

bool wxTempFile::Open(const mstring& strName)
{
  m_strName = strName;

  // we want to create the file in the same directory as strName because
  // otherwise rename() in Commit() might not work (if the files are on
  // different partitions for example). Unfortunately, the only standard
  // (POSIX) temp file creation function tmpnam() can't do it.
  #if defined(__UNIX__) || defined(__linux__)
    static const char *szMktempSuffix = "XXXXXX";
    m_strTemp << strName << szMktempSuffix;
    mktemp((char *)m_strTemp.c_str()); // will do because length doesn't change
  #else // Windows
    mstring strPath;
    wxSplitPath(strName, &strPath, NULL, NULL);
    if ( strPath.IsEmpty() )
      strPath = '.';  // GetTempFileName will fail if we give it empty string
	char cm_strTemp[MAX_PATH];
    if ( !GetTempFileName(strPath, "wx_",0, cm_strTemp))
      wxLogLastError("GetTempFileName");
    m_strTemp=cm_strTemp;
  #endif  // Windows/Unix

  return m_file.Open(m_strTemp, wxFile::write);
}

// ----------------------------------------------------------------------------
// destruction
// ----------------------------------------------------------------------------

wxTempFile::~wxTempFile()
{
  if ( IsOpened() )
    Discard();
}

bool wxTempFile::Commit()
{
  m_file.Close();

  if ( wxFile::Exists(m_strName) && remove(m_strName) != 0 ) {
    wxLogSysError("can't remove file '%s'", m_strName.c_str());
    return false;
  }

  if ( rename(m_strTemp, m_strName) != 0 ) {
    wxLogSysError("can't commit changes to file '%s'", m_strName.c_str());
    return false;
  }

  return true;
}

void wxTempFile::Discard()
{
  m_file.Close();
  if ( remove(m_strTemp) != 0 )
    wxLogSysError("can't remove temporary file '%s'", m_strTemp.c_str());
}

// ----------------------------------------------------------------------------
// wxFileInputStream
// ----------------------------------------------------------------------------

wxFileInputStream::wxFileInputStream(const mstring& fileName)
  : wxInputStream()
{
  m_file = new wxFile(fileName, wxFile::read);
  m_file_destroy = true;
  m_i_streambuf->SetBufferIO(1024);
}

wxFileInputStream::wxFileInputStream()
  : wxInputStream()
{
  m_file_destroy = false;
  m_file = NULL;
}

wxFileInputStream::~wxFileInputStream()
{
  if (m_file_destroy)
    delete m_file;
}

char wxFileInputStream::Peek()
{
  return 0;
}

size_t wxFileInputStream::StreamSize() const
{
  return m_file->Length();
}

size_t wxFileInputStream::OnSysRead(void *buffer, size_t size)
{
  return m_file->Read(buffer, size);
}

off_t wxFileInputStream::OnSysSeek(off_t pos, wxSeekMode mode)
{
  return m_file->Seek(pos, mode);
}


off_t wxFileInputStream::OnSysTell() const
{
  return m_file->Tell();
}

// ----------------------------------------------------------------------------
// wxFileOutputStream
// ----------------------------------------------------------------------------

wxFileOutputStream::wxFileOutputStream(const mstring& fileName, bool Append)
{
    if(Append==false)
	m_file = new wxFile(fileName, wxFile::write);
    else
	m_file = new wxFile(fileName, wxFile::write_append);
    m_file_destroy = true;
    m_o_streambuf->SetBufferIO(1024);
}

wxFileOutputStream::wxFileOutputStream(wxFile& file)
{
  m_file = &file;
  m_file_destroy = false;
  m_o_streambuf->SetBufferIO(1024);
}

wxFileOutputStream::wxFileOutputStream()
  : wxOutputStream()
{
  m_o_streambuf->SetBufferIO(1024);
  m_file_destroy = false;
  m_file = NULL;
}

wxFileOutputStream::~wxFileOutputStream()
{
  if (m_file_destroy) {
    Sync();
    delete m_file;
  }
}

size_t wxFileOutputStream::OnSysWrite(const void *buffer, size_t size)
{
  size_t ret = m_file->Write(buffer, size);
  m_lasterror = wxStream_EOF; // TODO
  return ret;
}

off_t wxFileOutputStream::OnSysTell() const
{
  return m_file->Tell();
}

off_t wxFileOutputStream::OnSysSeek(off_t pos, wxSeekMode mode)
{
  return m_file->Seek(pos, mode);
}

void wxFileOutputStream::Sync()
{
  wxOutputStream::Sync();
  m_file->Flush();
}

size_t wxFileOutputStream::StreamSize() const
{
  return m_file->Length();
}

// ----------------------------------------------------------------------------
// wxFileStream
// ----------------------------------------------------------------------------
wxFileStream::wxFileStream(const mstring& fileName)
 : wxFileInputStream(fileName), wxFileOutputStream(*wxFileInputStream::m_file)
{
}

