
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
#ifndef _MEXCEPTIONS_H
#define _MEXCEPTIONS_H
#include "pch.h"
RCSID(mexceptions_h, "@(#) $Id$");

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

#ifdef MAGICK_HAS_EXCEPTIONS

enum locktype_enum
{ L_Invalid = 0, L_Read, L_Write, L_WriteUpgrade, L_Mutex };

class E_NickServ_Stored : public exception
{
public:
    enum E_where
    { W_Add, W_Get, W_Rem, W_Use, W_Other };
    enum E_type
    { T_NotFound, T_Found, T_Invalid, T_Blank, T_Other };

private:
    E_where i_where;
    E_type i_type;
    char i_reason[1024];

public:
    E_NickServ_Stored(const E_where p_where = W_Other, const E_type p_type = T_Other, const char *p_reason =
		      "NickServ Stored") throw() : i_where(p_where), i_type(p_type)
    {
	ACE_OS::strncpy(i_reason, p_reason, 1024);
    }
    E_NickServ_Stored(const char *p_reason) throw() : i_where(W_Other), i_type(T_Other)
    {
	ACE_OS::strncpy(i_reason, p_reason, 1024);
    }
    const E_where where() const throw()
    {
	return i_where;
    }
    const E_type type() const throw()
    {
	return i_type;
    }
    const char *what() const throw()
    {
	return i_reason;
    };
};

class E_NickServ_Live : public exception
{
public:
    enum E_where
    { W_Add, W_Get, W_Rem, W_Use, W_Other };
    enum E_type
    { T_NotFound, T_Found, T_Invalid, T_Blank, T_Other };

private:
    E_where i_where;
    E_type i_type;
    char i_reason[1024];

public:
    E_NickServ_Live(const E_where p_where = W_Other, const E_type p_type = T_Other, const char *p_reason =
		    "NickServ Live") throw() : i_where(p_where), i_type(p_type)
    {
	ACE_OS::strncpy(i_reason, p_reason, 1024);
    }
    E_NickServ_Live(const char *p_reason) throw() : i_where(W_Other), i_type(T_Other)
    {
	ACE_OS::strncpy(i_reason, p_reason, 1024);
    }
    const E_where where() const throw()
    {
	return i_where;
    }
    const E_type type() const throw()
    {
	return i_type;
    }
    const char *what() const throw()
    {
	return i_reason;
    };
};

class E_NickServ_Recovered : public exception
{
public:
    enum E_where
    { W_Add, W_Get, W_Rem, W_Use, W_Other };
    enum E_type
    { T_NotFound, T_Found, T_Invalid, T_Blank, T_Other };

private:
    E_where i_where;
    E_type i_type;
    char i_reason[1024];

public:
    E_NickServ_Recovered(const E_where p_where = W_Other, const E_type p_type = T_Other, const char *p_reason =
			 "NickServ Recovered") throw() : i_where(p_where), i_type(p_type)
    {
	ACE_OS::strncpy(i_reason, p_reason, 1024);
    }
    E_NickServ_Recovered(const char *p_reason) throw() : i_where(W_Other), i_type(T_Other)
    {
	ACE_OS::strncpy(i_reason, p_reason, 1024);
    }
    const E_where where() const throw()
    {
	return i_where;
    }
    const E_type type() const throw()
    {
	return i_type;
    }
    const char *what() const throw()
    {
	return i_reason;
    };
};

class E_ChanServ_Stored : public exception
{
public:
    enum E_where
    { W_Add, W_Get, W_Rem, W_Use, W_Other };
    enum E_type
    { T_NotFound, T_Found, T_Invalid, T_Blank, T_Other };

private:
    E_where i_where;
    E_type i_type;
    char i_reason[1024];

public:
    E_ChanServ_Stored(const E_where p_where = W_Other, const E_type p_type = T_Other, const char *p_reason =
		      "ChanServ Stored") throw() : i_where(p_where), i_type(p_type)
    {
	ACE_OS::strncpy(i_reason, p_reason, 1024);
    }
    E_ChanServ_Stored(const char *p_reason) throw() : i_where(W_Other), i_type(T_Other)
    {
	ACE_OS::strncpy(i_reason, p_reason, 1024);
    }
    const E_where where() const throw()
    {
	return i_where;
    }
    const E_type type() const throw()
    {
	return i_type;
    }
    const char *what() const throw()
    {
	return i_reason;
    };
};

class E_ChanServ_Live : public exception
{
public:
    enum E_where
    { W_Add, W_Get, W_Rem, W_Use, W_Other };
    enum E_type
    { T_NotFound, T_Found, T_Invalid, T_Blank, T_Other };

private:
    E_where i_where;
    E_type i_type;
    char i_reason[1024];

public:
    E_ChanServ_Live(const E_where p_where = W_Other, const E_type p_type = T_Other, const char *p_reason =
		    "ChanServ Live") throw() : i_where(p_where), i_type(p_type)
    {
	ACE_OS::strncpy(i_reason, p_reason, 1024);
    }
    E_ChanServ_Live(const char *p_reason) throw() : i_where(W_Other), i_type(T_Other)
    {
	ACE_OS::strncpy(i_reason, p_reason, 1024);
    }
    const E_where where() const throw()
    {
	return i_where;
    }
    const E_type type() const throw()
    {
	return i_type;
    }
    const char *what() const throw()
    {
	return i_reason;
    };
};

class E_CommServ_List : public exception
{
public:
    enum E_where
    { W_Add, W_Get, W_Rem, W_Use, W_Other };
    enum E_type
    { T_NotFound, T_Found, T_Invalid, T_Blank, T_Other };

private:
    E_where i_where;
    E_type i_type;
    char i_reason[1024];

public:
    E_CommServ_List(const E_where p_where = W_Other, const E_type p_type = T_Other, const char *p_reason =
		    "CommServ Live") throw() : i_where(p_where), i_type(p_type)
    {
	ACE_OS::strncpy(i_reason, p_reason, 1024);
    }
    E_CommServ_List(const char *p_reason) throw() : i_where(W_Other), i_type(T_Other)
    {
	ACE_OS::strncpy(i_reason, p_reason, 1024);
    }
    const E_where where() const throw()
    {
	return i_where;
    }
    const E_type type() const throw()
    {
	return i_type;
    }
    const char *what() const throw()
    {
	return i_reason;
    };
};

class E_Server_List : public exception
{
public:
    enum E_where
    { W_Add, W_Get, W_Rem, W_Use, W_Other };
    enum E_type
    { T_NotFound, T_Found, T_Invalid, T_Blank, T_Other };

private:
    E_where i_where;
    E_type i_type;
    char i_reason[1024];

public:
    E_Server_List(const E_where p_where = W_Other, const E_type p_type = T_Other, const char *p_reason =
		  "Server List") throw() : i_where(p_where), i_type(p_type)
    {
	ACE_OS::strncpy(i_reason, p_reason, 1024);
    }
    E_Server_List(const char *p_reason) throw() : i_where(W_Other), i_type(T_Other)
    {
	ACE_OS::strncpy(i_reason, p_reason, 1024);
    }
    const E_where where() const throw()
    {
	return i_where;
    }
    const E_type type() const throw()
    {
	return i_type;
    }
    const char *what() const throw()
    {
	return i_reason;
    };
};

class E_MemoServ_Nick : public exception
{
public:
    enum E_where
    { W_Add, W_Get, W_Rem, W_Use, W_Other };
    enum E_type
    { T_NotFound, T_Found, T_Invalid, T_Blank, T_Other };

private:
    E_where i_where;
    E_type i_type;
    char i_reason[1024];
    size_t i_memo;

public:
    E_MemoServ_Nick(const E_where p_where = W_Other, const E_type p_type = T_Other, const char *p_reason =
		    "MemoServ Nick", size_t p_memo = 0) throw() : i_where(p_where), i_type(p_type), i_memo(p_memo)
    {
	ACE_OS::strncpy(i_reason, p_reason, 1024);
    }
    E_MemoServ_Nick(const char *p_reason, size_t p_memo = 0) throw() : i_where(W_Other), i_type(T_Other), i_memo(p_memo)
    {
	ACE_OS::strncpy(i_reason, p_reason, 1024);
    }
    const E_where where() const throw()
    {
	return i_where;
    }
    const E_type type() const throw()
    {
	return i_type;
    }
    const char *what() const throw()
    {
	return i_reason;
    };
    const size_t memo() const throw()
    {
	return i_memo;
    }
};

class E_MemoServ_Channel : public exception
{
public:
    enum E_where
    { W_Add, W_Get, W_Rem, W_Use, W_Other };
    enum E_type
    { T_NotFound, T_Found, T_Invalid, T_Blank, T_Other };

private:
    E_where i_where;
    E_type i_type;
    char i_reason[1024];
    size_t i_news;

public:
    E_MemoServ_Channel(const E_where p_where = W_Other, const E_type p_type = T_Other, const char *p_reason =
		       "MemoServ Channel", size_t p_news = 0) throw() : i_where(p_where), i_type(p_type), i_news(p_news)
    {
	ACE_OS::strncpy(i_reason, p_reason, 1024);
    }
    E_MemoServ_Channel(const char *p_reason, size_t p_news = 0) throw() : i_where(W_Other), i_type(T_Other), i_news(p_news)
    {
	ACE_OS::strncpy(i_reason, p_reason, 1024);
    }
    const E_where where() const throw()
    {
	return i_where;
    }
    const E_type type() const throw()
    {
	return i_type;
    }
    const char *what() const throw()
    {
	return i_reason;
    };
    const size_t news() const throw()
    {
	return i_news;
    }
};

class E_DccMap_Xfers : public exception
{
public:
    enum E_where
    { W_Add, W_Get, W_Rem, W_Use, W_Other };
    enum E_type
    { T_NotFound, T_Found, T_Invalid, T_Blank, T_Other };

private:
    E_where i_where;
    E_type i_type;
    char i_reason[1024];

public:
    E_DccMap_Xfers(const E_where p_where = W_Other, const E_type p_type = T_Other, const char *p_reason =
		   "DccMap Xfers") throw() : i_where(p_where), i_type(p_type)
    {
	ACE_OS::strncpy(i_reason, p_reason, 1024);
    }
    E_DccMap_Xfers(const char *p_reason) throw() : i_where(W_Other), i_type(T_Other)
    {
	ACE_OS::strncpy(i_reason, p_reason, 1024);
    }
    const E_where where() const throw()
    {
	return i_where;
    }
    const E_type type() const throw()
    {
	return i_type;
    }
    const char *what() const throw()
    {
	return i_reason;
    };
};

class E_IrcSvcHandler_Message : public exception
{
public:
    enum E_where
    { W_Add, W_Get, W_Rem, W_Use, W_Other };
    enum E_type
    { T_NotFound, T_Found, T_Invalid, T_Blank, T_Other };

private:
    E_where i_where;
    E_type i_type;
    char i_reason[1024];

public:
    E_IrcSvcHandler_Message(const E_where p_where = W_Other, const E_type p_type = T_Other, unsigned long msgid = 0) throw()
			: i_where(p_where), i_type(p_type)
    {
	ACE_OS::memcpy(i_reason, reinterpret_cast<void *>(msgid), sizeof(msgid));
	i_reason[sizeof(msgid)] = 0;
    }
    E_IrcSvcHandler_Message(unsigned long msgid) throw() : i_where(W_Other), i_type(T_Other)
    {
	ACE_OS::memcpy(i_reason, reinterpret_cast<void *>(msgid), sizeof(msgid));
	i_reason[sizeof(msgid)] = 0;
    }
    const E_where where() const throw()
    {
	return i_where;
    }
    const E_type type() const throw()
    {
	return i_type;
    }
    const char *what() const throw()
    {
	return i_reason;
    };
};


class E_Thread : public exception
{
public:
    enum E_type
    { T_NotProcessing, T_Other };

private:
    E_type i_type;
    char i_reason[1024];

public:
    E_Thread(const E_type p_type = T_Other, const char *p_reason = "Thread") throw() : i_type(p_type)
    {
	ACE_OS::strncpy(i_reason, p_reason, 1024);
    }
    E_Thread(const char *p_reason) throw() : i_type(T_Other)
    {
	ACE_OS::strncpy(i_reason, p_reason, 1024);
    }

    const E_type type() const throw()
    {
	return i_type;
    }
    const char *what() const throw()
    {
	return i_reason;
    };
};

class E_Magick : public exception
{
public:
    enum E_type
    { T_NotFound, T_Invalid, T_Other };

private:
    E_type i_type;
    char i_reason[1024];

public:
    E_Magick(const E_type p_type = T_Other, const char *p_reason = "Magick") throw() : i_type(p_type)
    {
	ACE_OS::strncpy(i_reason, p_reason, 1024);
    }
    E_Magick(const char *p_reason) throw() : i_type(T_Other)
    {
	ACE_OS::strncpy(i_reason, p_reason, 1024);
    }

    const E_type type() const throw()
    {
	return i_type;
    }
    const char *what() const throw()
    {
	return i_reason;
    };
};

class E_Lock : public exception
{
public:
    enum E_type
    { T_Create, T_Acquire, T_Release, T_Duplicate, T_Other };

private:
    locktype_enum i_locktype;
    E_type i_type;
    char i_reason[1024];

public:
    E_Lock(const locktype_enum p_locktype = L_Invalid, const E_type p_type = T_Other, const char *p_reason =
	   "Lock") throw() : i_locktype(p_locktype), i_type(p_type)
    {
	ACE_OS::strncpy(i_reason, p_reason, 1024);
    }
    E_Lock(const char *p_reason) throw() : i_locktype(L_Invalid), i_type(T_Other)
    {
	ACE_OS::strncpy(i_reason, p_reason, 1024);
    }

    const locktype_enum locktype() const throw()
    {
	return i_locktype;
    }
    const E_type type() const throw()
    {
	return i_type;
    }
    const char *what() const throw()
    {
	return i_reason;
    };
};

#endif

#endif
