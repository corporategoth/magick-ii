#ifndef _MSTREAM_H
#define _MSTREAM_H

#include <ace/OS.h>

class mStreamBase;
class mInputStream;
class mOutputStream;

typedef mInputStream& (*__mInputManip)(mInputStream&);
typedef mOutputStream& (*__mOutputManip)(mOutputStream&);

mOutputStream& mEndL(mOutputStream& o_stream);

class mStreamBuffer {
 public:
  typedef enum {
    read = 0, write, read_write
  } BufMode;
  // -----------
  // ctor & dtor
  // -----------
  mStreamBuffer(mStreamBase& stream, BufMode mode);
  mStreamBuffer(BufMode mode);
  mStreamBuffer(const mStreamBuffer& buf);
  ~mStreamBuffer();


#endif