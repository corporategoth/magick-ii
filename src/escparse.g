
header {
/*  Magick IRC Services
**
** (c) 1997-2000 Preston Elder <prez@magick.tm>
** (c) 1998-2000 William King <ungod@magick.tm>
**
** The above copywright may not be removed under any
** circumstances, however it may be added to if any
** modifications are made to this file.  All modified
** code must be clearly documented and labelled.
**
** ========================================================== */
#pragma ident "$Id$"
/* ==========================================================
**
** Third Party Changes (please include e-mail address):
**
** N/A
**
** Changes by Magick Development Team <magick-devel@magick.tm>:
**
** $Log$
** Revision 1.5  2000/02/15 10:37:49  prez
** Added standardized headers to ALL Magick source files, including
** a #pragma ident, and history log.  ALL revisions of files from
** now on should include what changes were made to the files involved.
**
**
** ========================================================== */

#include <string>
using namespace std;
#include "mstring.h"
}

options {
language="Cpp";
}

class EscLexer extends Lexer;
options{
k=3;
//filter=Mignore;
charVocabulary = '\3'..'\377';
}
{
public:
	mstring retstring;
}

Slashn: "\\n"
	{ retstring=retstring+mstring("\n"); }
	;
Slasht: "\\t"
	{ retstring=retstring+mstring("\t"); }
	;
Slashv: "\\v"
	/*{ retstring=retstring+mstring("\v"); }*/
	{ retstring=retstring+mstring("\\v"); }
	;
Slashb: "\\b"
	{ retstring=retstring+mstring("\b"); }
	;
Slashr: "\\r"
	{ retstring=retstring+mstring("\r"); }
	;
Slashf: "\\f"
	{ retstring=retstring+mstring("\f"); }
	;
Slasha: "\\a"
	/*{ retstring=retstring+mstring("\a"); }*/
	{ retstring=retstring+mstring("\\a"); }
	;
Slashslash: "\\\\"
	{ retstring=retstring+mstring("\\"); }
	;
Slashquestion: "\\?"
	/*{ retstring=retstring+mstring("\?"); }*/
	{ retstring=retstring+mstring("\\?"); }
	;
Slashsinglequote: "\\'"
	{ retstring=retstring+mstring("\'"); }
	;
Slashdoublequote: "\\\""
	{ retstring=retstring+mstring("\""); }
	;
Slashhexidecimal: "\\"! b:HEXDIGIT
	{ int ival; sscanf(b->getText().c_str(),"%x",&ival); retstring=retstring+mstring((char)ival); }
	;
Slashoctal: "\\"! c:OCTALDIGIT
	{ int ival; sscanf(c->getText().c_str(),"%o",&ival); retstring=retstring+mstring((char)ival); }
	;
Slashtrash: "\\"! d:('c'..'e'|'g'..'m'|'o'..'q'|'u'|'w'|'y'|'z'|'A'..'W'|'Y'|'Z')
	{ retstring=retstring+mstring(d->getText()); }
	;
NewLine: 
	("\r\n" | '\r' | '\n') 
		{ newline(); retstring=retstring+mstring("\n"); }
	;

HEXDIGIT: ('x'|'X') ('0')* ('1'..'9'|'a'..'f'|'A'..'F') ('0'..'9'|'a'..'f'|'A'..'F')?;

OCTALDIGIT: ('0'..'7') | ('0'..'7')('0'..'7') | ('0'..'7')('0'..'7')('0'..'7');

protected
Mignore: a:.
	{ retstring=retstring+mstring(a); }
;

class EscParser extends Parser;

expr: (expr1)*;

expr1: slashexpr 
	| NewLine;

slashexpr:
	(Slashn) => Slashn
	| Slasht
	| Slashv
	| Slashb
	| Slashr
	| Slashf
	| Slasha
	| Slashslash
	| Slashquestion
	| Slashsinglequote
	| Slashdoublequote
	| Slashhexidecimal
	| Slashoctal
	| Slashtrash
	;
