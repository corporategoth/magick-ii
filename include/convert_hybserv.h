#ifndef WIN32
#pragma interface
#endif

/* Magick IRC Services
**
** (c) 1997-2002 Preston Elder <prez@magick.tm>
** (c) 1998-2002 William King <ungod@magick.tm>
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
#ifndef _CONVERT_HYBSERV_H
#define _CONVERT_HYBSERV_H
#include "pch.h"
RCSID(convert_hybserv_h, "@(#) $Id$");

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

/* Please note, that if you use encrypted passwords, then ALL passwords
** converted using this conversion process will be utterly useless.
**
** Some of the code in this (and the accompanying .cpp file) was written
** or based code by the HybServ2 Coding Team.
**/

#ifdef CONVERT

#include "mstring.h"

/* NickServ flags */
#define HYB_NS_IDENTIFIED   0x00000001 /* nick has IDENTIFY'd */
#define HYB_NS_PROTECTED    0x00000002 /* kill to regain nick */
#define HYB_NS_NOEXPIRE     0x00000004 /* nickname which doesn't expire */
#define HYB_NS_AUTOMASK     0x00000008 /* auto-add new hostmasks */
#define HYB_NS_PRIVATE      0x00000010 /* nick won't show up in a LIST */
#define HYB_NS_COLLIDE      0x00000020 /* stole a nick - kill them */
#define HYB_NS_RELEASE      0x00000040 /* to release an enforced nick */
#define HYB_NS_FORBID       0x00000080 /* nick is forbidden */
#define HYB_NS_SECURE       0x00000100 /* nick is secure */
#define HYB_NS_DELETE       0x00000200 /* delete after a RELOAD */
#define HYB_NS_UNSECURE     0x00000400 /* don't need to IDENTIFY */
#define HYB_NS_MEMOSIGNON   0x00000800 /* tell about memos on signon */
#define HYB_NS_MEMONOTIFY   0x00001000 /* tell about new memos */
#define HYB_NS_MEMOS        0x00002000 /* allow others to send memos */
#define HYB_NS_HIDEALL      0x00004000 /* hide everything from INFO */
#define HYB_NS_HIDEEMAIL    0x00008000 /* hide email from INFO */
#define HYB_NS_HIDEURL      0x00010000 /* hide url from INFO */
#define HYB_NS_HIDEQUIT     0x00020000 /* hide quitmsg from INFO */
#define HYB_NS_HIDEADDR     0x00040000 /* hide last addy from INFO */
#define HYB_NS_KILLIMMED    0x00080000 /* kill immediately to regain nick */
#define HYB_NS_NOREGISTER   0x00100000 /* cannot register channels */
#define HYB_NS_NOCHANOPS    0x00200000 /* not allowed to be opped */
#define HYB_NS_NUMERIC      0x00400000 /* ignores 432 numeric */
#define HYB_NS_PRIVMSG      0x00800000 /* PRIVMSG or NOTICE */

/* structure for registered nicknames */
struct HYB_NickInfo
{
  mstring nick;                /* registered nickname */
  mstring password;            /* password */
  vector<mstring> hosts;    /* recognized hosts for this nick */
  time_t created;            /* timestamp when it was registered */
  time_t lastseen;           /* for expiration purposes */
  long flags;                /* nick flags */

  mstring email;               /* email address */
  mstring url;                 /* url */

  mstring gsm;                 /* GSM number */
  mstring phone;               /* Phone */
  mstring UIN;                 /* ICQ UIN */

  mstring lastu;               /* last seen username */
  mstring lasth;               /* last seen hostname */
  mstring lastqmsg;            /* last quit message */

  /*
   * If this is the "hub" nickname for a list of linked nicknames,
   * master will be NULL. If this nickname is a leaf, master will
   * point to the "hub" nickname of this list
   */
  mstring master;
};

/*************************************************************************/

#define HYB_BAN_EXPIRE_TIME 10800

/* ChanServ flags */
#define HYB_CS_PRIVATE      0x00000001 /* channel won't show up in LIST */
#define HYB_CS_TOPICLOCK    0x00000002 /* must be changed via SET TOPIC */
#define HYB_CS_SECURE       0x00000004 /* channel is secure */
#define HYB_CS_SECUREOPS    0x00000008 /* only aop/sop/founders can be opped */
#define HYB_CS_SUSPENDED    0x00000010 /* channel is suspended */
#define HYB_CS_FORBID       0x00000020 /* channel is forbidden */
#define HYB_CS_RESTRICTED   0x00000040 /* channel is restricted */
#define HYB_CS_FORGET       0x00000080 /* channel is forgotten */
#define HYB_CS_DELETE       0x00000100 /* delete after a RELOAD */
#define HYB_CS_NOEXPIRE     0x00000200 /* never expires */
#define HYB_CS_GUARD        0x00000400 /* have ChanServ join the channel */
#define HYB_CS_SPLITOPS     0x00000800 /* let people keep ops from splits */
#define HYB_CS_VERBOSE      0x00001000 /* notify chanops for access changes */
#define HYB_CS_EXPIREBANS   0x00002000 /* expire bans after EXPIRETIME */

/* access_lvl[] indices */
/* We will happily FUBAR old databases by changing this. However, it had
 * to be done -kre && Janos
 * PS, I have added upgrade-chan target in Makefile for fixing this
 * properly - it relies on awk and DefaultAccess as well as ALVL in
 * chan.db -kre */
# define HYB7_CA_AUTODEOP     0
# define HYB7_CA_AUTOVOICE    1
# define HYB7_CA_CMDVOICE     2
# define HYB7_CA_ACCESS       3
# define HYB7_CA_CMDINVITE    4
# define HYB7_CA_AUTOHALFOP   5
# define HYB7_CA_CMDHALFOP    6
# define HYB7_CA_AUTOOP       7
# define HYB7_CA_CMDOP        8
# define HYB7_CA_CMDUNBAN     9
# define HYB7_CA_AKICK        10
# define HYB7_CA_CMDCLEAR     11
# define HYB7_CA_SET          12
# define HYB7_CA_SUPEROP      13
# define HYB7_CA_FOUNDER      14
# define HYB7_CA_SIZE         15 /* number of indices */
# define HYB6_CA_AUTODEOP     0
# define HYB6_CA_AUTOVOICE    1
# define HYB6_CA_CMDVOICE     2
# define HYB6_CA_ACCESS       3
# define HYB6_CA_CMDINVITE    4
# define HYB6_CA_AUTOOP       5
# define HYB6_CA_CMDOP        6
# define HYB6_CA_CMDUNBAN     7
# define HYB6_CA_AKICK        8
# define HYB6_CA_CMDCLEAR     9
# define HYB6_CA_SET          10
# define HYB6_CA_SUPEROP      11
# define HYB6_CA_FOUNDER      12
# define HYB6_CA_SIZE         13 /* number of indices */

#define HYB_CMODE_L          0x000010 /* channel is +l */
#define HYB_CMODE_K          0x000020 /* channel is +k */
#define HYB_CMODE_S          0x000040 /* channel is +s */
#define HYB_CMODE_P          0x000080 /* channel is +p */
#define HYB_CMODE_N          0x000100 /* channel is +n */
#define HYB_CMODE_T          0x000200 /* channel is +t */
#define HYB_CMODE_M          0x000400 /* channel is +m */
#define HYB_CMODE_I          0x000800 /* channel is +i */
#define HYB_CMODE_C          0x001000 /* channel is +c */
#define HYB_CMODE_F          0x002000 /* channel is +f */
#define HYB_CMODE_A          0x004000 /* channel is +a - Janos */


struct HYB_ChanAccess
{
  int level;               /* privs mask has */
  mstring entry;   /* nickname */

  /*
   * pointer to corresponding AccessChannel structure on nptr's
   * AccessChannels list - this way, when we delete a ChanAccess
   * structure, we don't have to loop through all of nptr's
   * access channels to find the corresponding pointer.
   */
  time_t created; /* time when this entry was added */
  time_t last_used; /* last time the person joined the channel while
                       identified */
};

struct HYB_AutoKick
{
  mstring hostmask; /* mask to autokick */
  mstring reason;   /* reason for autokick */
};

struct HYB_ChanInfo
{
  mstring name;                   /* channel name */
  mstring founder;                /* founder nick (must be registered) */
  time_t last_founder_active;   /* last time the founder joined/left */
  mstring successor;              /* successor nick (must be registered) */
  mstring password;               /* founder password */
  mstring topic;                  /* NULL if no topic lock */
  long limit;                   /* 0 if no limit */
  mstring key;                    /* NULL if no key */
  int modes_on,                 /* modes to enforce */
      modes_off;                /* modes to enforce off */
  vector<HYB_ChanAccess> access;    /* access list */
  vector<HYB_AutoKick> akick;       /* autokick list */

  mstring entrymsg;               /* msg to send to users upon entry to channel */
  mstring email;                  /* email address of channel */
  mstring url;                    /* url of channel */

  time_t created;               /* timestamp when it was registered */
  time_t lastused;              /* for expiration purposes */
  vector<int> access_lvl;              /* customized access levels for this channel */
  long flags;                   /* channel flags */
};


/*************************************************************************/

/* MemoServ flags */
#define HYB_MS_READ         0x00000001 /* memo has been read */
#define HYB_MS_DELETE       0x00000002 /* marked for deletion */
#define HYB_MS_RDELETE      0x00000004 /* delete after a RELOAD */
#define HYB_MS_REPLIED      0x00000008 /* has been replied */

struct HYB_Memo
{
  mstring sender;
  time_t sent;    /* time it was sent */
  mstring text;
  long flags;
};

struct HYB_MemoInfo
{
  mstring name;            /* who the memo was sent to */
  vector<HYB_Memo> memos;    /* the actual memos */
};


/*************************************************************************/

struct Ignore
{
  time_t expire;   /* 0 if permanent ignore */
  mstring hostmask;  /* hostmask to ignore */
};

/*************************************************************************/

void HYB_load_ns_dbase(void);
void HYB_load_cs_dbase(void);
void HYB_load_ms_dbase(void);
void HYB_load_ignore_dbase(void);

/*************************************************************************/

class Nick_Stored_t;
class Chan_Stored_t;
class Memo_t;
class News_t;

/* OK -- now for the magick conversions ... */
Nick_Stored_t *HYB_CreateNickEntry(HYB_NickInfo * ni);
Chan_Stored_t *HYB_CreateChanEntry(HYB_ChanInfo * ci);
list < Memo_t > HYB_CreateMemoEntry(HYB_MemoInfo *mi);

#endif /* CONVERT */
#endif /* _CONVERT_HYBSERV_H */
