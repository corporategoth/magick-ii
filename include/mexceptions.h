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
#ifndef _MEXCEPTIONS_H
#define _MEXCEPTIONS_H
#include "pch.h"
RCSID(mexceptions_h, "@(#) $Id$");
/* ========================================================== **
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <devel@magick.tm>:
**
** $Log$
** Revision 1.4  2001/05/03 04:40:17  prez
** Fixed locking mechanism (now use recursive mutexes) ...
** Also now have a deadlock/nonprocessing detection mechanism.
**
** Revision 1.3  2001/04/05 05:59:50  prez
** Turned off -fno-default-inline, and split up server.cpp, it should
** compile again with no special options, and have default inlines :)
**
** Revision 1.2  2001/04/02 02:13:27  prez
** Added inlines, fixed more of the exception code.
**
** Revision 1.1  2001/03/27 07:12:07  prez
** Added exceptions
**
**
** ========================================================== */

 #ifdef MAGICK_HAS_EXCEPTIONS

class E_NickServ_Stored : public exception
{
public:
    enum E_where { W_Add, W_Get, W_Rem, W_Use, W_Other };
    enum E_type { T_NotFound, T_Found, T_Invalid, T_Blank, T_Other };

private:
    E_where i_where;
    E_type i_type;
    char i_reason[1024];

public:
    E_NickServ_Stored(const E_where where = W_Other, const E_type type = T_Other, const char *reason = "")
	: i_where(where), i_type(type)
    {
	ACE_OS::strncpy(i_reason, reason, 1024);
    }
    E_NickServ_Stored(const char *reason)
	: i_where(W_Other), i_type(T_Other)
    {
	ACE_OS::strncpy(i_reason, reason, 1024);
    }
    const E_where where() const
	{ return i_where; }
    const E_type type() const
	{ return i_type; }
    const char *what() const
	{ return i_reason; };
};

class E_NickServ_Live : public exception
{
public:
    enum E_where { W_Add, W_Get, W_Rem, W_Use, W_Other };
    enum E_type { T_NotFound, T_Found, T_Invalid, T_Blank, T_Other };

private:
    E_where i_where;
    E_type i_type;
    char i_reason[1024];

public:
    E_NickServ_Live(const E_where where = W_Other, const E_type type = T_Other, const char *reason = "")
	: i_where(where), i_type(type)
    {
	ACE_OS::strncpy(i_reason, reason, 1024);
    }
    E_NickServ_Live(const char *reason)
	: i_where(W_Other), i_type(T_Other)
    {
	ACE_OS::strncpy(i_reason, reason, 1024);
    }
    const E_where where() const
	{ return i_where; }
    const E_type type() const
	{ return i_type; }
    const char *what() const
	{ return i_reason; };
};

class E_NickServ_Recovered : public exception
{
public:
    enum E_where { W_Add, W_Get, W_Rem, W_Use, W_Other };
    enum E_type { T_NotFound, T_Found, T_Invalid, T_Blank, T_Other };

private:
    E_where i_where;
    E_type i_type;
    char i_reason[1024];

public:
    E_NickServ_Recovered(const E_where where = W_Other, const E_type type = T_Other, const char *reason = "")
	: i_where(where), i_type(type)
    {
	ACE_OS::strncpy(i_reason, reason, 1024);
    }
    E_NickServ_Recovered(const char *reason)
	: i_where(W_Other), i_type(T_Other)
    {
	ACE_OS::strncpy(i_reason, reason, 1024);
    }
    const E_where where() const
	{ return i_where; }
    const E_type type() const
	{ return i_type; }
    const char *what() const
	{ return i_reason; };
};

class E_ChanServ_Stored : public exception
{
public:
    enum E_where { W_Add, W_Get, W_Rem, W_Use, W_Other };
    enum E_type { T_NotFound, T_Found, T_Invalid, T_Blank, T_Other };

private:
    E_where i_where;
    E_type i_type;
    char i_reason[1024];

public:
    E_ChanServ_Stored(const E_where where = W_Other, const E_type type = T_Other, const char *reason = "")
	: i_where(where), i_type(type)
    {
	ACE_OS::strncpy(i_reason, reason, 1024);
    }
    E_ChanServ_Stored(const char *reason)
	: i_where(W_Other), i_type(T_Other)
    {
	ACE_OS::strncpy(i_reason, reason, 1024);
    }
    const E_where where() const
	{ return i_where; }
    const E_type type() const
	{ return i_type; }
    const char *what() const
	{ return i_reason; };
};

class E_ChanServ_Live : public exception
{
public:
    enum E_where { W_Add, W_Get, W_Rem, W_Use, W_Other };
    enum E_type { T_NotFound, T_Found, T_Invalid, T_Blank, T_Other };

private:
    E_where i_where;
    E_type i_type;
    char i_reason[1024];

public:
    E_ChanServ_Live(const E_where where = W_Other, const E_type type = T_Other, const char *reason = "")
	: i_where(where), i_type(type)
    {
	ACE_OS::strncpy(i_reason, reason, 1024);
    }
    E_ChanServ_Live(const char *reason)
	: i_where(W_Other), i_type(T_Other)
    {
	ACE_OS::strncpy(i_reason, reason, 1024);
    }
    const E_where where() const
	{ return i_where; }
    const E_type type() const
	{ return i_type; }
    const char *what() const
	{ return i_reason; };
};

class E_CommServ_List : public exception
{
public:
    enum E_where { W_Add, W_Get, W_Rem, W_Use, W_Other };
    enum E_type { T_NotFound, T_Found, T_Invalid, T_Blank, T_Other };

private:
    E_where i_where;
    E_type i_type;
    char i_reason[1024];

public:
    E_CommServ_List(const E_where where = W_Other, const E_type type = T_Other, const char *reason = "")
	: i_where(where), i_type(type)
    {
	ACE_OS::strncpy(i_reason, reason, 1024);
    }
    E_CommServ_List(const char *reason)
	: i_where(W_Other), i_type(T_Other)
    {
	ACE_OS::strncpy(i_reason, reason, 1024);
    }
    const E_where where() const
	{ return i_where; }
    const E_type type() const
	{ return i_type; }
    const char *what() const
	{ return i_reason; };
};

class E_Server_List : public exception
{
public:
    enum E_where { W_Add, W_Get, W_Rem, W_Use, W_Other };
    enum E_type { T_NotFound, T_Found, T_Invalid, T_Blank, T_Other };

private:
    E_where i_where;
    E_type i_type;
    char i_reason[1024];

public:
    E_Server_List(const E_where where = W_Other, const E_type type = T_Other, const char *reason = "")
	: i_where(where), i_type(type)
    {
	ACE_OS::strncpy(i_reason, reason, 1024);
    }
    E_Server_List(const char *reason)
	: i_where(W_Other), i_type(T_Other)
    {
	ACE_OS::strncpy(i_reason, reason, 1024);
    }
    const E_where where() const
	{ return i_where; }
    const E_type type() const
	{ return i_type; }
    const char *what() const
	{ return i_reason; };
};

class E_MemoServ_Nick : public exception
{
public:
    enum E_where { W_Add, W_Get, W_Rem, W_Use, W_Other };
    enum E_type { T_NotFound, T_Found, T_Invalid, T_Blank, T_Other };

private:
    E_where i_where;
    E_type i_type;
    char i_reason[1024];
    size_t i_memo;

public:
    E_MemoServ_Nick(const E_where where = W_Other, const E_type type = T_Other, const char *reason = "", size_t memo = 0)
	: i_where(where), i_type(type), i_memo(memo)
    {
	ACE_OS::strncpy(i_reason, reason, 1024);
    }
    E_MemoServ_Nick(const char *reason, size_t memo = 0)
	: i_where(W_Other), i_type(T_Other), i_memo(memo)
    {
	ACE_OS::strncpy(i_reason, reason, 1024);
    }
    const E_where where() const
	{ return i_where; }
    const E_type type() const
	{ return i_type; }
    const char *what() const
	{ return i_reason; };
    const size_t memo() const
	{ return i_memo; }
};

class E_MemoServ_Channel : public exception
{
public:
    enum E_where { W_Add, W_Get, W_Rem, W_Use, W_Other };
    enum E_type { T_NotFound, T_Found, T_Invalid, T_Blank, T_Other };

private:
    E_where i_where;
    E_type i_type;
    char i_reason[1024];
    size_t i_news;

public:
    E_MemoServ_Channel(const E_where where = W_Other, const E_type type = T_Other, const char *reason = "", size_t news = 0)
	: i_where(where), i_type(type), i_news(news)
    {
	ACE_OS::strncpy(i_reason, reason, 1024);
    }
    E_MemoServ_Channel(const char *reason, size_t news = 0)
	: i_where(W_Other), i_type(T_Other), i_news(news)
    {
	ACE_OS::strncpy(i_reason, reason, 1024);
    }
    const E_where where() const
	{ return i_where; }
    const E_type type() const
	{ return i_type; }
    const char *what() const
	{ return i_reason; };
    const size_t news() const
	{ return i_news; }
};

class E_DccMap_Xfers : public exception
{
public:
    enum E_where { W_Add, W_Get, W_Rem, W_Use, W_Other };
    enum E_type { T_NotFound, T_Found, T_Invalid, T_Blank, T_Other };

private:
    E_where i_where;
    E_type i_type;
    char i_reason[1024];

public:
    E_DccMap_Xfers(const E_where where = W_Other, const E_type type = T_Other, const char *reason = "")
	: i_where(where), i_type(type)
    {
	ACE_OS::strncpy(i_reason, reason, 1024);
    }
    E_DccMap_Xfers(const char *reason)
	: i_where(W_Other), i_type(T_Other)
    {
	ACE_OS::strncpy(i_reason, reason, 1024);
    }
    const E_where where() const
	{ return i_where; }
    const E_type type() const
	{ return i_type; }
    const char *what() const
	{ return i_reason; };
};

class E_Thread : public exception
{
public:
    enum E_type { T_NotProcessing, T_Other };

private:
    E_type i_type;
    char i_reason[1024];

public:
    E_Thread(const E_type type = T_Other, const char *reason = "")
	: i_type(type)
    {
	ACE_OS::strncpy(i_reason, reason, 1024);
    }
    E_Thread(const char *reason)
	: i_type(T_Other)
    {
	ACE_OS::strncpy(i_reason, reason, 1024);
    }

    const E_type type() const
	{ return i_type; }
    const char *what() const
	{ return i_reason; };
};

#endif

#endif
