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
#ifndef _CONVERT_INTERFACE_H
#define _CONVERT_INTERFACE_H
#include "pch.h"
RCSID(convert_interface_h, "@(#) $Id$");

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

#ifdef CONVERT

enum convert_t { CONVERT_MAGICK, CONVERT_IRCSERVICES, CONVERT_EPONA,
	CONVERT_HYBSERV, CONVERT_AUSPICE, CONVERT_PTLINK, CONVERT_SIRV,
	CONVERT_WRECKED, CONVERT_TRIRCD, CONVERT_CYGNUS, CONVERT_SRVX,
	CONVERT_DAYLIGHT, CONVERT_IRCS, CONVERT_BOLIVIA };

struct convert_name_t
{
    convert_t id;
    const char *name;
};

const convert_name_t convert_names[] = {
    { CONVERT_MAGICK, "magick" },
    { CONVERT_IRCSERVICES, "ircservices" },
    { CONVERT_EPONA, "epona" },
    { CONVERT_HYBSERV, "hybserv" },
    { CONVERT_AUSPICE, "auspice" },
    { CONVERT_PTLINK, "ptlink" },
    { CONVERT_SIRV, "sirc" },
    { CONVERT_WRECKED, "wrecked" },
    { CONVERT_TRIRCD, "trircd" },
    { CONVERT_CYGNUS, "cygnus" },
    { CONVERT_SRVX, "srvx" },
    { CONVERT_DAYLIGHT, "daylight" },
    { CONVERT_IRCS, "ircs" },
    { CONVERT_BOLIVIA, "bolivia" },
    { (convert_t) -1, NULL }
};

void convert_db(convert_t c);

struct magick_NickInfo;
struct magick_ChanInfo;
struct magick_MemoList;
struct magick_NewsList;
struct hybserv_NickInfo;
struct hybserv_ChanInfo;
struct hybserv_MemoInfo;
struct epona_NickAlias;
struct epona_NickCore;
struct epona_ChanInfo;
struct epona_MemoInfo;
struct wrecked_NickInfo;
struct wrecked_ChanInfo;
struct wrecked_MemoList;
struct wrecked_NewsList;
struct cygnus_NickInfo;
struct cygnus_ChanInfo;
struct cygnus_MemoInfo;
struct auspice_NickInfo;
struct auspice_ChanInfo;
struct auspice_MemoList;
struct bolivia_NickInfo;
struct bolivia_ChanInfo;
struct bolivia_MemoList;
struct ptlink_NickInfo;
struct ptlink_ChanInfo;
struct ptlink_MemoInfo;

class Convert
{
public:
    static Nick_Stored_t *magick_CreateNickEntry(magick_NickInfo *ni);
    static Chan_Stored_t *magick_CreateChanEntry(magick_ChanInfo *ci);
    static MemoServ::nick_memo_t magick_CreateMemoEntry(magick_MemoList *ml);
    static MemoServ::channel_news_t magick_CreateNewsEntry(magick_NewsList *nl);

    static Nick_Stored_t *hybserv_CreateNickEntry(hybserv_NickInfo *ni);
    static Chan_Stored_t *hybserv_CreateChanEntry(hybserv_ChanInfo *ci);
    static MemoServ::nick_memo_t hybserv_CreateMemoEntry(hybserv_MemoInfo *mi);

    static Nick_Stored_t *epona_CreateNickEntry(epona_NickAlias *na, epona_NickCore *nc);
    static Chan_Stored_t *epona_CreateChanEntry(epona_ChanInfo *ci);
    static MemoServ::nick_memo_t epona_CreateMemoEntry(epona_MemoInfo *ml, const char *nick);
    static MemoServ::channel_news_t epona_CreateNewsEntry(epona_MemoInfo *nl, const char *chan);

    static Nick_Stored_t *wrecked_CreateNickEntry(wrecked_NickInfo *ni);
    static Chan_Stored_t *wrecked_CreateChanEntry(wrecked_ChanInfo *ci);
    static MemoServ::nick_memo_t wrecked_CreateMemoEntry(wrecked_MemoList *ml);
    static MemoServ::channel_news_t wrecked_CreateNewsEntry(wrecked_NewsList *nl);

    static Nick_Stored_t *cygnus_CreateNickEntry(cygnus_NickInfo *ni);
    static Chan_Stored_t *cygnus_CreateChanEntry(cygnus_ChanInfo *ci);
    static MemoServ::nick_memo_t cygnus_CreateMemoEntry(cygnus_MemoInfo *ml, const char *name);

    static Nick_Stored_t *auspice_CreateNickEntry(auspice_NickInfo *ni);
    static Chan_Stored_t *auspice_CreateChanEntry(auspice_ChanInfo *ci);
    static MemoServ::nick_memo_t auspice_CreateMemoEntry(auspice_MemoList *ml);
    static MemoServ::channel_news_t auspice_CreateNewsEntry(size_t count, char **list, const char *chan);

    static Nick_Stored_t *bolivia_CreateNickEntry(bolivia_NickInfo *ni);
    static Chan_Stored_t *bolivia_CreateChanEntry(bolivia_ChanInfo *ci);
    static MemoServ::nick_memo_t bolivia_CreateMemoEntry(bolivia_MemoList *ml);

    static Nick_Stored_t *ptlink_CreateNickEntry(ptlink_NickInfo *ni);
    static Chan_Stored_t *ptlink_CreateChanEntry(ptlink_ChanInfo *ci);
    static MemoServ::nick_memo_t ptlink_CreateMemoEntry(ptlink_MemoInfo *ml, const char *name);
    static MemoServ::channel_news_t ptlink_CreateNewsEntry(ptlink_MemoInfo *ml, const char *name);

};

#endif /* CONVERT */
#endif /* _CONVERT_INTERFACE */
