
/* Magick IRC Services
**
** (c) 1997-2002 Preston Elder <prez@magick.tm>
** (c) 1998-2002 William King <ungod@magick.tm>
**
** The above copywright may not be removed under any circumstances,
** however it may be added to if any modifications are made to this
** file.  All modified code must be clearly documented and labelled.
**
** This code is released under the GNU General Public License, which
** means (in short), it may be distributed freely, and may not be sold
** or used as part of any closed-source product.  Please check the
** COPYING file for full rights and restrictions of this software.
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
		      "") throw() : i_where(p_where), i_type(p_type)
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
		    "") throw() : i_where(p_where), i_type(p_type)
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
			 "") throw() : i_where(p_where), i_type(p_type)
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
		      "") throw() : i_where(p_where), i_type(p_type)
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
		    "") throw() : i_where(p_where), i_type(p_type)
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
		    "") throw() : i_where(p_where), i_type(p_type)
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
		  "") throw() : i_where(p_where), i_type(p_type)
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
    E_MemoServ_Nick(const E_where p_where = W_Other, const E_type p_type = T_Other, const char *p_reason = "", size_t p_memo =
		    0) throw() : i_where(p_where), i_type(p_type), i_memo(p_memo)
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
		       "", size_t p_news = 0) throw() : i_where(p_where), i_type(p_type), i_news(p_news)
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
		   "") throw() : i_where(p_where), i_type(p_type)
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

class E_Thread : public exception
{
public:
    enum E_type
    { T_NotProcessing, T_Other };

private:
    E_type i_type;
    char i_reason[1024];

public:
    E_Thread(const E_type p_type = T_Other, const char *p_reason = "") throw() : i_type(p_type)
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
    E_Magick(const E_type p_type = T_Other, const char *p_reason = "") throw() : i_type(p_type)
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
	   "") throw() : i_locktype(p_locktype), i_type(p_type)
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
