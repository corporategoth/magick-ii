
header {
/* Escape parsing subsystem
 *
 * Magick IRC Services are copyright (c) 1996-1998 Preston A. Elder, W. King
 *     E-mail: <prez@magick.tm>   IRC: PreZ@RelicNet
 * This program is free but copyrighted software; see the file COPYING for
 * details.
 */
#include "mstring.h"
}

options {
language="Cpp";
}

class EscLexer extends Lexer;
options{
k=3;
filter=IGNORE;
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

protected
IGNORE: 
	a:. {retstring=retstring+mstring(a); }
	;

protected
HEXDIGIT: ('x'|'X') ('0')* ('1'..'9'|'a'..'f'|'A'..'F') ('0'..'9'|'a'..'f'|'A'..'F')?;

protected
OCTALDIGIT: ('0'..'7') | ('0'..'7')('0'..'7') | ('0'..'7')('0'..'7')('0'..'7');

class EscParser extends Parser;
{
	mstring retstring;
}

expr: ( slashexpr | IGNORE | NewLine)*;

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
