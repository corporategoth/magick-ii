#ifndef INC_EscLexerTokenTypes_hpp_
#define INC_EscLexerTokenTypes_hpp_

#include "antlr/config.hpp"
/*
 * ANTLR-generated file resulting from grammar escparse.g
 * 
 * Terence Parr, MageLang Institute
 * with John Lilley, Empathy Software
 * ANTLR Version 2.5.0; 1996,1997,1998
 */


/* Escape parsing subsystem
 *
 * Magick IRC Services are copyright (c) 1996-1998 Preston A. Elder, W. King
 *     E-mail: <prez@magick.tm>   IRC: PreZ@RelicNet
 * This program is free but copyrighted software; see the file COPYING for
 * details.
 */
#include "mstring.h"

enum EscLexerTokenTypes {
	EOF_ = 1,
	NULL_TREE_LOOKAHEAD = 3,
	Slashn = 4,
	Slasht = 5,
	Slashv = 6,
	Slashb = 7,
	Slashr = 8,
	Slashf = 9,
	Slasha = 10,
	Slashslash = 11,
	Slashquestion = 12,
	Slashsinglequote = 13,
	Slashdoublequote = 14,
	Slashhexidecimal = 15,
	Slashoctal = 16,
	Slashtrash = 17,
	NewLine = 18,
	IGNORE = 19,
	HEXDIGIT = 20,
	OCTALDIGIT = 21,
};
#endif /*INC_EscLexerTokenTypes_hpp_*/
