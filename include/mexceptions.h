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
    E_NickServ_Stored(E_where where = W_Other, E_type type = T_Other, char *reason = "")
	: i_where(where), i_type(type)
    {
	ACE_OS::strncpy(i_reason, reason, 1024);
    }
    E_NickServ_Stored(char *reason)
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
    E_NickServ_Live(E_where where = W_Other, E_type type = T_Other, char *reason = "")
	: i_where(where), i_type(type)
    {
	ACE_OS::strncpy(i_reason, reason, 1024);
    }
    E_NickServ_Live(char *reason)
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
    E_NickServ_Recovered(E_where where = W_Other, E_type type = T_Other, char *reason = "")
	: i_where(where), i_type(type)
    {
	ACE_OS::strncpy(i_reason, reason, 1024);
    }
    E_NickServ_Recovered(char *reason)
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
    E_ChanServ_Stored(E_where where = W_Other, E_type type = T_Other, char *reason = "")
	: i_where(where), i_type(type)
    {
	ACE_OS::strncpy(i_reason, reason, 1024);
    }
    E_ChanServ_Stored(char *reason)
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
    E_ChanServ_Live(E_where where = W_Other, E_type type = T_Other, char *reason = "")
	: i_where(where), i_type(type)
    {
	ACE_OS::strncpy(i_reason, reason, 1024);
    }
    E_ChanServ_Live(char *reason)
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
    E_CommServ_List(E_where where = W_Other, E_type type = T_Other, char *reason = "")
	: i_where(where), i_type(type)
    {
	ACE_OS::strncpy(i_reason, reason, 1024);
    }
    E_CommServ_List(char *reason)
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
    E_Server_List(E_where where = W_Other, E_type type = T_Other, char *reason = "")
	: i_where(where), i_type(type)
    {
	ACE_OS::strncpy(i_reason, reason, 1024);
    }
    E_Server_List(char *reason)
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

public:
    E_MemoServ_Nick(E_where where = W_Other, E_type type = T_Other, char *reason = "")
	: i_where(where), i_type(type)
    {
	ACE_OS::strncpy(i_reason, reason, 1024);
    }
    E_MemoServ_Nick(char *reason)
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

class E_MemoServ_Channel : public exception
{
public:
    enum E_where { W_Add, W_Get, W_Rem, W_Use, W_Other };
    enum E_type { T_NotFound, T_Found, T_Invalid, T_Blank, T_Other };

private:
    E_where i_where;
    E_type i_type;
    char i_reason[1024];

public:
    E_MemoServ_Channel(E_where where = W_Other, E_type type = T_Other, char *reason = "")
	: i_where(where), i_type(type)
    {
	ACE_OS::strncpy(i_reason, reason, 1024);
    }
    E_MemoServ_Channel(char *reason)
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

#endif

#endif
