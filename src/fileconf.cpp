///////////////////////////////////////////////////////////////////////////////
// Name:        fileconf.cpp
// Purpose:     wxFileConfig derivation of wxConfigBase
// Author:      Vadim Zeitlin
// Modified by: Ungod for magick2
// Created:     07.04.98 (adapted from appconf.cpp)
// RCS-ID:      $Id$
// Copyright:   (c) 1997 Karsten Ballnder   &  Vadim Zeitlin
//                       Ballueder@usa.net     <zeitlin@dptmaths.ens-cachan.fr>
// Licence:     wxWindows license
///////////////////////////////////////////////////////////////////////////////
#include "fileconf.h"
#include "utils.h"
#include <ctype.h>
#ifdef _MSC_VER
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

