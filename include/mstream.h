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

#ifndef _MSTREAM_H
#define _MSTREAM_H

/////////////////////////////////////////////////////////////////////////////
// Name:        stream.cpp
// Purpose:     wxStream base classes
// Author:      Guilhem Lavaux
// Modified by: Hacked heavily for magick use by Ungod
// Created:     11/07/98
// RCS-ID:      $Id$
// Copyright:   (c) Guilhem Lavaux
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

#include <ace/OS.h>
#include "mstring.h"

class wxStreamBase;
class wxInputStream;
class wxOutputStream;

typedef wxInputStream& (*__wxInputManip)(wxInputStream&);
typedef wxOutputStream& (*__wxOutputManip)(wxOutputStream&);

typedef enum 
{
  wxFromStart,
  wxFromCurrent,
  wxFromEnd
} wxSeekMode;

typedef enum 
{
  wxStream_NOERROR = 0,
  wxStream_EOF,
  wxStream_WRITE_ERR,
  wxStream_READ_ERR
} wxStreamError;

wxOutputStream& wxEndL(wxOutputStream& o_stream);

class wxStreamBuffer 
{
 public:
  typedef enum 
  {
    read = 0, write, read_write
  } BufMode;
  // -----------
  // ctor & dtor
  // -----------
  wxStreamBuffer(wxStreamBase& stream, BufMode mode);
  wxStreamBuffer(BufMode mode);
  wxStreamBuffer(const wxStreamBuffer& buf);
  ~wxStreamBuffer();

  // -----------
  // Filtered IO
  // -----------
  size_t Read(void *buffer, size_t size);
  size_t Read(wxStreamBuffer *buf);
  size_t Write(const void *buffer, size_t size);
  size_t Write(wxStreamBuffer *buf);

  size_t WriteBack(const char *buffer, size_t size);
  bool WriteBack(char c);
  char GetChar();
  void PutChar(char c);
  off_t Tell() const;
  off_t Seek(off_t pos, wxSeekMode mode);

  // --------------
  // Buffer control
  // --------------
  void ResetBuffer();
  void SetBufferIO(char *buffer_start, char *buffer_end);
  void SetBufferIO(size_t bufsize);
  char *GetBufferStart() const { return m_buffer_start; }
  char *GetBufferEnd() const { return m_buffer_end; }
  char *GetBufferPos() const { return m_buffer_pos; }
  off_t GetIntPosition() const { return m_buffer_pos-m_buffer_start; }
  void SetIntPosition(off_t pos) { m_buffer_pos = m_buffer_start+pos; }
  size_t GetLastAccess() const { return m_buffer_end-m_buffer_start; }

  void Fixed(bool fixed) { m_fixed = fixed; }
  void Flushable(bool f) { m_flushable = f; }

  bool FlushBuffer();
  bool FillBuffer();
  size_t GetDataLeft();
  // --------------
  // Administration
  // --------------
  wxStreamBase *Stream() { return m_stream; }

 protected:
  char *AllocSpaceWBack(size_t needed_size);
  size_t GetWBack(char *buf, size_t bsize);

  void GetFromBuffer(void *buffer, size_t size);
  void PutToBuffer(const void *buffer, size_t size);

 protected:
  char *m_buffer_start, *m_buffer_end, *m_buffer_pos;
  size_t m_buffer_size;

  char *m_wback;
  size_t m_wbacksize, m_wbackcur;

  bool m_fixed, m_flushable;

  wxStreamBase *m_stream;
  BufMode m_mode;
  bool m_destroybuf, m_destroystream;
};

class wxStreamBase {
 public:
  wxStreamBase();
  virtual ~wxStreamBase();

  wxStreamError LastError() const { return m_lasterror; }
  virtual size_t StreamSize() const { return ~((size_t)0); }

 protected:
  friend class wxStreamBuffer;

  virtual size_t OnSysRead(void *buffer, size_t bufsize);
  virtual size_t OnSysWrite(const void *buffer, size_t bufsize);
  virtual off_t OnSysSeek(off_t seek, wxSeekMode mode);
  virtual off_t OnSysTell() const;

 protected:
  size_t m_lastcount;
  wxStreamError m_lasterror;
};

class wxInputStream: public wxStreamBase {
 public:
  wxInputStream();
  wxInputStream(wxStreamBuffer *sbuf);
  virtual ~wxInputStream();

  // IO functions
  virtual char Peek();
  char GetC();
  wxInputStream& Read(void *buffer, size_t size);
  wxInputStream& Read(wxOutputStream& stream_out);

  // Position functions
  off_t SeekI(off_t pos, wxSeekMode mode = wxFromStart);
  off_t TellI() const;

  // State functions
  wxStreamBuffer *InputStreamBuffer() { return m_i_streambuf; }
  size_t LastRead() { return wxStreamBase::m_lastcount; }

  // Operators
  wxInputStream& operator>>(wxOutputStream& out) { return Read(out);}
  wxInputStream& operator>>(mstring& line);
  wxInputStream& operator>>(char& c);
  wxInputStream& operator>>(short& i);
  wxInputStream& operator>>(int& i);
  wxInputStream& operator>>(long& i);
  wxInputStream& operator>>(double& i);

  wxInputStream& operator>>(float& f) { double d; operator>>((double&)d); f = (float)d; return *this; }
  wxInputStream& operator>>(unsigned char& c) { return operator>>((char&)c);}
  wxInputStream& operator>>(unsigned short& i) { return operator>>((short&)i);}
  wxInputStream& operator>>(unsigned int& i) { return operator>>((int&)i);}
  wxInputStream& operator>>(unsigned long& i) { return operator>>((long&)i);}
  wxInputStream& operator>>( __wxInputManip func) { return func(*this);}

 protected:
  bool m_i_destroybuf;
  wxStreamBuffer *m_i_streambuf;
};

class wxOutputStream: public wxStreamBase {
 public:
  wxOutputStream();
  wxOutputStream(wxStreamBuffer *sbuf);
  virtual ~wxOutputStream();

  wxOutputStream& Write(const void *buffer, size_t size);
  wxOutputStream& Write(wxInputStream& stream_in);

  off_t SeekO(off_t pos, wxSeekMode mode = wxFromStart);
  off_t TellO() const;

  size_t LastWrite() const { return wxStreamBase::m_lastcount; }
  wxStreamBuffer *OutputStreamBuffer() { return m_o_streambuf; }

  void Sync();

  wxOutputStream& operator<<(wxInputStream& out) { return Write(out); }
  wxOutputStream& operator<<(const char *string);
  wxOutputStream& operator<<(mstring& string);
  wxOutputStream& operator<<(char c);
  wxOutputStream& operator<<(short i);
  wxOutputStream& operator<<(int i);
  wxOutputStream& operator<<(long i);
  wxOutputStream& operator<<(double f);

  wxOutputStream& operator<<(float f) { return operator<<((double)f); }
  wxOutputStream& operator<<(unsigned char c) { return operator<<((char)c); }
  wxOutputStream& operator<<(unsigned short i) { return operator<<((short)i); }
  wxOutputStream& operator<<(unsigned int i) { return operator<<((int)i); }
  wxOutputStream& operator<<(unsigned long i) { return operator<<((long)i); }
  wxOutputStream& operator<<( __wxOutputManip func) { return func(*this); }

 protected:
  bool m_o_destroybuf;
  wxStreamBuffer *m_o_streambuf;
};

class wxFilterInputStream: public wxInputStream
{
 public:
  wxFilterInputStream();
  wxFilterInputStream(wxInputStream& stream);
  ~wxFilterInputStream();

  char Peek() { return m_parent_i_stream->Peek(); }

  wxStreamError LastError() const { return m_parent_i_stream->LastError(); }
  size_t StreamSize() const { return m_parent_i_stream->StreamSize(); }

 protected:
  wxInputStream *m_parent_i_stream;
};

class wxFilterOutputStream: public wxOutputStream {
 public:
  wxFilterOutputStream();
  wxFilterOutputStream(wxOutputStream& stream);
  ~wxFilterOutputStream();

  wxStreamError LastError() const { return m_parent_o_stream->LastError(); }
  size_t StreamSize() const { return m_parent_o_stream->StreamSize(); }

 protected:
  wxOutputStream *m_parent_o_stream;
};

class wxDataInputStream: public wxFilterInputStream {
public:
  wxDataInputStream(wxInputStream& s);
  virtual ~wxDataInputStream();

  unsigned long Read32();
  unsigned short Read16();
  unsigned char Read8();
  double ReadDouble();
  mstring ReadLine();
  mstring ReadString();
};

class wxDataOutputStream: public wxFilterOutputStream {
 public:
  wxDataOutputStream(wxOutputStream& s);
  virtual ~wxDataOutputStream();

  void Write32(unsigned long i);
  void Write16(unsigned short i);
  void Write8(unsigned char i);
  void WriteDouble(double d);
  void WriteLine(const mstring& line);
  void WriteString(const mstring& string);
};

class wxMemoryInputStream: public wxInputStream {
 public:
  wxMemoryInputStream(const char *data, size_t length);
  virtual ~wxMemoryInputStream();

  char Peek();
};

class wxMemoryOutputStream:  public wxOutputStream {
 public:
  wxMemoryOutputStream(char *data = NULL, size_t length = 0);
  virtual ~wxMemoryOutputStream();
};

class wxMemoryStream: public wxMemoryInputStream, public wxMemoryOutputStream {
 public:
  wxMemoryStream(char *data = NULL, size_t length = 0);
  virtual ~wxMemoryStream();
};

class wxZlibInputStream: public wxFilterInputStream {
 public:
  wxZlibInputStream(wxInputStream& stream);
  virtual ~wxZlibInputStream();

 protected:
  size_t OnSysRead(void *buffer, size_t size);

 protected:
  size_t m_z_size;
  unsigned char *m_z_buffer;
  struct z_stream_s *m_inflate;
};

class wxZlibOutputStream: public wxFilterOutputStream {
 public:
  wxZlibOutputStream(wxOutputStream& stream);
  virtual ~wxZlibOutputStream();

  void Sync();

 protected:
  size_t OnSysWrite(const void *buffer, size_t size);

 protected:
  size_t m_z_size;
  unsigned char *m_z_buffer;
  struct z_stream_s *m_deflate;
};

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// we redefine these constants here because S_IREAD &c are _not_ standard
// however, we do assume that the values correspond to the Unix umask bits
#define wxS_IRUSR 00400
#define wxS_IWUSR 00200
#define wxS_IXUSR 00100

#define wxS_IRGRP 00040
#define wxS_IWGRP 00020
#define wxS_IXGRP 00010

#define wxS_IROTH 00004
#define wxS_IWOTH 00002
#define wxS_IXOTH 00001

// default mode for the new files: corresponds to umask 022
#define wxS_DEFAULT   (wxS_IRUSR | wxS_IWUSR | wxS_IRGRP | wxS_IWGRP |\
                       wxS_IROTH | wxS_IWOTH)

// ----------------------------------------------------------------------------
// class wxFile: raw file IO
//
// NB: for space efficiency this class has no virtual functions, including
//     dtor which is _not_ virtual, so it shouldn't be used as a base class.
// ----------------------------------------------------------------------------
class wxFile
{
public:
  // more file constants
  // -------------------
    // opening mode
  enum OpenMode { read, write, read_write, write_append };
    // standard values for file descriptor
  enum { fd_invalid = -1, fd_stdin, fd_stdout, fd_stderr };

  // static functions
  // ----------------
    // check whether a regular file by this name exists
  static bool Exists(const char *name);
    // check whetther we can access the given file in given mode
    // (only read and write make sense here)
  static bool Access(const char *name, OpenMode mode);

  // ctors
  // -----
    // def ctor
  wxFile() { m_fd = fd_invalid; }
    // open specified file (may fail, use IsOpened())
  wxFile(const char *szFileName, OpenMode mode = read);
    // attach to (already opened) file
  wxFile(int fd) { m_fd = fd; }

  // open/close
    // create a new file (with the default value of bOverwrite, it will fail if
    // the file already exists, otherwise it will overwrite it and succeed)
  bool Create(const char *szFileName, bool bOverwrite = false,
              int access = wxS_DEFAULT);
  bool Open(const char *szFileName, OpenMode mode = read,
            int access = wxS_DEFAULT);
  bool Close();  // Close is a NOP if not opened

  // assign an existing file descriptor and get it back from wxFile object
  void Attach(int fd) { Close(); m_fd = fd; }
  void Detach()       { m_fd = fd_invalid;  }
  int  fd() const { return m_fd; }

  // read/write (unbuffered)
    // returns number of bytes read or ofsInvalid on error
  off_t Read(void *pBuf, off_t nCount);
    // returns true on success
  size_t Write(const void *pBuf, size_t nCount);
    // returns true on success
  bool Write(const mstring& s) { return Write(s.c_str(), s.length()) != 0; }
    // flush data not yet written
  bool Flush();

  // file pointer operations (return ofsInvalid on failure)
    // move ptr ofs bytes related to start/current off_t/end of file
  off_t Seek(off_t ofs, wxSeekMode mode = wxFromStart);
    // move ptr to ofs bytes before the end
  off_t SeekEnd(off_t ofs = 0) { return Seek(ofs, wxFromEnd); }
    // get current off_t
  off_t Tell() const;
    // get current file length
  off_t Length() const;

  // simple accessors
    // is file opened?
  bool IsOpened() const { return m_fd != fd_invalid; }
    // is end of file reached?
  bool Eof() const;
    // is an error occured?
  bool Error() const { return m_error; }

  // dtor closes the file if opened
  ~wxFile();

private:
  // copy ctor and assignment operator are private because
  // it doesn't make sense to copy files this way:
  // attempt to do it will provoke a compile-time error.
  wxFile(const wxFile&);
  wxFile& operator=(const wxFile&);

  int m_fd; // file descriptor or INVALID_FD if not opened
  bool m_error; // error memory
};

class wxTempFile
{
public:
  // ctors
    // default
  wxTempFile() { }
    // associates the temp file with the file to be replaced and opens it
  wxTempFile(const mstring& strName);

  // open the temp file (strName is the name of file to be replaced)
  bool Open(const mstring& strName);

  // is the file opened?
  bool IsOpened() const { return m_file.IsOpened(); }

  // I/O (both functions return true on success, false on failure)
  bool Write(const void *p, size_t n) { return m_file.Write(p, n) != 0; }
  bool Write(const mstring& str)   { return m_file.Write(str); }

  // different ways to close the file
    // validate changes and delete the old file of name m_strName
  bool Commit();
    // discard changes
  void Discard();

  // dtor calls Discard() if file is still opened
 ~wxTempFile();

private:
  // no copy ctor/assignment operator
  wxTempFile(const wxTempFile&);
  wxTempFile& operator=(const wxTempFile&);

  mstring  m_strName,  // name of the file to replace in Commit()
            m_strTemp;  // temporary file name
  wxFile    m_file;     // the temporary file
};

class wxFileInputStream: public wxInputStream {
 public:
  wxFileInputStream(const mstring& ifileName);
  wxFileInputStream(wxFile& file);
  wxFileInputStream(int fd);
  ~wxFileInputStream();

  char Peek();
  size_t StreamSize() const;

  bool Ok() const { return m_file->IsOpened(); }

 protected:
  wxFileInputStream();

  size_t OnSysRead(void *buffer, size_t size);
  off_t OnSysSeek(off_t pos, wxSeekMode mode);
  off_t OnSysTell() const;

 protected:
  wxFile *m_file;
  bool m_file_destroy;
};

class wxFileOutputStream: public wxOutputStream {
 public:
  wxFileOutputStream(const mstring& fileName);
  wxFileOutputStream(wxFile& file);
  wxFileOutputStream(int fd);
  virtual ~wxFileOutputStream();

  // To solve an ambiguity on GCC
  inline wxOutputStream& Write(const void *buffer, size_t size)
     { return wxOutputStream::Write(buffer, size); }

  void Sync();
  size_t StreamSize() const;

  bool Ok() const { return m_file->IsOpened(); }

 protected:
  wxFileOutputStream();

  size_t OnSysWrite(const void *buffer, size_t size);
  off_t OnSysSeek(off_t pos, wxSeekMode mode);
  off_t OnSysTell() const;

 protected:
  wxFile *m_file;
  bool m_file_destroy;
};

class wxFileStream: public wxFileInputStream, public wxFileOutputStream {
 public:
  wxFileStream(const mstring& fileName);
};


#endif