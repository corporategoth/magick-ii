// $Id$
//
// Magick IRC Services
// (c) 1997-1999 Preston A. Elder <prez@magick.tm>
// (c) 1998-1999 W. King <ungod@magick.tm>
//
// The above copywright may not be removed under any
// circumstances, however it may be added to if any
// modifications are made to this file.  All modified
// code must be clearly documented and labelled.
//
// ===================================================
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

#include "EscLexer.hpp"
#include "EscLexerTokenTypes.hpp"
#include "antlr/ScannerException.hpp"

EscLexer::EscLexer(std::istream& in)
	: CharScanner(new CharBuffer(in))
{
        FT("EscLexer::EscLexer", ("(std::istream) in"));
	setCaseSensitive(true);
	initLiterals();
}

EscLexer::EscLexer(CharBuffer& cb)
	: CharScanner(cb)
{
        FT("EscLexer::EscLexer", ("(CharBuffer) cb"));
	setCaseSensitive(true);
	initLiterals();
}

void EscLexer::initLiterals()
{
}
bool EscLexer::getCaseSensitiveLiterals() const
{
        NFT("EscLexer::getCaseSensitiveLiterals");
	RET(true);
}

RefToken EscLexer::nextToken()
{
        NFT("EscLexer::nextToken");
	RefToken _rettoken;
	for (;;) {
		RefToken _rettoken;
		int _ttype = Token::INVALID_TYPE;
		resetText();
		try {   // for error handling
			if ((LA(1)==static_cast<unsigned char>('\\')) && (LA(2)==static_cast<unsigned char>('n'))) {
				mSlashn(true);
				_rettoken=_returnToken;
			}
			else if ((LA(1)==static_cast<unsigned char>('\\')) && (LA(2)==static_cast<unsigned char>('t'))) {
				mSlasht(true);
				_rettoken=_returnToken;
			}
			else if ((LA(1)==static_cast<unsigned char>('\\')) && (LA(2)==static_cast<unsigned char>('v'))) {
				mSlashv(true);
				_rettoken=_returnToken;
			}
			else if ((LA(1)==static_cast<unsigned char>('\\')) && (LA(2)==static_cast<unsigned char>('b'))) {
				mSlashb(true);
				_rettoken=_returnToken;
			}
			else if ((LA(1)==static_cast<unsigned char>('\\')) && (LA(2)==static_cast<unsigned char>('r'))) {
				mSlashr(true);
				_rettoken=_returnToken;
			}
			else if ((LA(1)==static_cast<unsigned char>('\\')) && (LA(2)==static_cast<unsigned char>('f'))) {
				mSlashf(true);
				_rettoken=_returnToken;
			}
			else if ((LA(1)==static_cast<unsigned char>('\\')) && (LA(2)==static_cast<unsigned char>('a'))) {
				mSlasha(true);
				_rettoken=_returnToken;
			}
			else if ((LA(1)==static_cast<unsigned char>('\\')) && (LA(2)==static_cast<unsigned char>('\\'))) {
				mSlashslash(true);
				_rettoken=_returnToken;
			}
			else if ((LA(1)==static_cast<unsigned char>('\\')) && (LA(2)==static_cast<unsigned char>('?'))) {
				mSlashquestion(true);
				_rettoken=_returnToken;
			}
			else if ((LA(1)==static_cast<unsigned char>('\\')) && (LA(2)==static_cast<unsigned char>('\''))) {
				mSlashsinglequote(true);
				_rettoken=_returnToken;
			}
			else if ((LA(1)==static_cast<unsigned char>('\\')) && (LA(2)==static_cast<unsigned char>('"'))) {
				mSlashdoublequote(true);
				_rettoken=_returnToken;
			}
			else if ((LA(1)==static_cast<unsigned char>('\\')) && (LA(2)==static_cast<unsigned char>('X')||LA(2)==static_cast<unsigned char>('x'))) {
				mSlashhexidecimal(true);
				_rettoken=_returnToken;
			}
			else if ((LA(1)==static_cast<unsigned char>('\\')) && ((LA(2) >= static_cast<unsigned char>('0') && LA(2) <= static_cast<unsigned char>('7')))) {
				mSlashoctal(true);
				_rettoken=_returnToken;
			}
			else if ((LA(1)==static_cast<unsigned char>('\\')) && (_tokenSet_0.member(LA(2)))) {
				mSlashtrash(true);
				_rettoken=_returnToken;
			}
			else if ((LA(1)==static_cast<unsigned char>('\n')||LA(1)==static_cast<unsigned char>('\r'))) {
				mNewLine(true);
				_rettoken=_returnToken;
			}
			else {
				if (LA(1)!=EOF) throw ScannerException(std::string("no viable alt for char: ")+charName(LA(1)),getLine());_returnToken = makeToken(Token::EOF_TYPE);
			}
			
			_ttype = _returnToken->getType();
			_ttype = testLiteralsTable(_ttype);
			if ( _ttype!=Token::SKIP ) {
				_returnToken->setType(_ttype);
				return _returnToken;
			}
		}
		catch (ScannerException& e) {
			consume();
			reportError(e);
		}
	}
}

void EscLexer::mSlashn(bool _createToken) {
	FT("EscLexer::mSlashn", (_createToken));
	int _ttype; RefToken _token; int _begin=text.length();
	_ttype = Slashn;
	int _saveIndex;
	
	match("\\n");
	retstring=retstring+mstring("\n");
	if ( _createToken && _token==0 ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
}

void EscLexer::mSlasht(bool _createToken) {
	FT("EscLexer::mSlasht", (_createToken));
	int _ttype; RefToken _token; int _begin=text.length();
	_ttype = Slasht;
	int _saveIndex;
	
	match("\\t");
	retstring=retstring+mstring("\t");
	if ( _createToken && _token==0 ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
}

void EscLexer::mSlashv(bool _createToken) {
	FT("EscLexer::mSlashv", (_createToken));
	int _ttype; RefToken _token; int _begin=text.length();
	_ttype = Slashv;
	int _saveIndex;
	
	match("\\v");
	retstring=retstring+mstring("\\v");
	if ( _createToken && _token==0 ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
}

void EscLexer::mSlashb(bool _createToken) {
	FT("EscLexer::mSlashb", (_createToken));
	int _ttype; RefToken _token; int _begin=text.length();
	_ttype = Slashb;
	int _saveIndex;
	
	match("\\b");
	retstring=retstring+mstring("\b");
	if ( _createToken && _token==0 ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
}

void EscLexer::mSlashr(bool _createToken) {
	FT("EscLexer::mSlashr", (_createToken));
	int _ttype; RefToken _token; int _begin=text.length();
	_ttype = Slashr;
	int _saveIndex;
	
	match("\\r");
	retstring=retstring+mstring("\r");
	if ( _createToken && _token==0 ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
}

void EscLexer::mSlashf(bool _createToken) {
	FT("EscLexer::mSlashf", (_createToken));
	int _ttype; RefToken _token; int _begin=text.length();
	_ttype = Slashf;
	int _saveIndex;
	
	match("\\f");
	retstring=retstring+mstring("\f");
	if ( _createToken && _token==0 ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
}

void EscLexer::mSlasha(bool _createToken) {
	FT("EscLexer::mSlasha", (_createToken));
	int _ttype; RefToken _token; int _begin=text.length();
	_ttype = Slasha;
	int _saveIndex;
	
	match("\\a");
	retstring=retstring+mstring("\\a");
	if ( _createToken && _token==0 ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
}

void EscLexer::mSlashslash(bool _createToken) {
	FT("EscLexer::mSlashslash", (_createToken));
	int _ttype; RefToken _token; int _begin=text.length();
	_ttype = Slashslash;
	int _saveIndex;
	
	match("\\\\");
	retstring=retstring+mstring("\\");
	if ( _createToken && _token==0 ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
}

void EscLexer::mSlashquestion(bool _createToken) {
	FT("EscLexer::mSlashquestion", (_createToken));
	int _ttype; RefToken _token; int _begin=text.length();
	_ttype = Slashquestion;
	int _saveIndex;
	
	match("\\?");
	retstring=retstring+mstring("\\?");
	if ( _createToken && _token==0 ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
}

void EscLexer::mSlashsinglequote(bool _createToken) {
	FT("EscLexer::mSlashsinglequote", (_createToken));
	int _ttype; RefToken _token; int _begin=text.length();
	_ttype = Slashsinglequote;
	int _saveIndex;
	
	match("\\'");
	retstring=retstring+mstring("\'");
	if ( _createToken && _token==0 ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
}

void EscLexer::mSlashdoublequote(bool _createToken) {
	FT("EscLexer::mSlashdoublequote", (_createToken));
	int _ttype; RefToken _token; int _begin=text.length();
	_ttype = Slashdoublequote;
	int _saveIndex;
	
	match("\\\"");
	retstring=retstring+mstring("\"");
	if ( _createToken && _token==0 ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
}

void EscLexer::mSlashhexidecimal(bool _createToken) {
	FT("EscLexer::mSlashhexidecimal", (_createToken));
	int _ttype; RefToken _token; int _begin=text.length();
	_ttype = Slashhexidecimal;
	int _saveIndex;
	RefToken b(0);
	
	_saveIndex=text.length();
	match("\\");
	text.erase(_saveIndex);
	mHEXDIGIT(true);
	b=_returnToken;
	int ival; sscanf(b->getText().c_str(),"%x",&ival); retstring=retstring+mstring((char)ival);
	if ( _createToken && _token==0 ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
}

void EscLexer::mHEXDIGIT(bool _createToken) {
	FT("EscLexer::mHEXDIGIT", (_createToken));
	int _ttype; RefToken _token; int _begin=text.length();
	_ttype = HEXDIGIT;
	int _saveIndex;
	
	{
	switch ( LA(1)) {
	case static_cast<unsigned char>('x'):
	{
		match('x');
		break;
	}
	case static_cast<unsigned char>('X'):
	{
		match('X');
		break;
	}
	default:
	{
		throw ScannerException(std::string("no viable alt for char: ")+charName(LA(1)),getLine());
	}
	}
	}
	{
	do {
		if ((LA(1)==static_cast<unsigned char>('0'))) {
			match('0');
		}
		else {
			goto _loop22;
		}
		
	} while (true);
	_loop22:;
	}
	{
	switch ( LA(1)) {
	case static_cast<unsigned char>('1'):  case static_cast<unsigned char>('2'):  case static_cast<unsigned char>('3'):  case static_cast<unsigned char>('4'):
	case static_cast<unsigned char>('5'):  case static_cast<unsigned char>('6'):  case static_cast<unsigned char>('7'):  case static_cast<unsigned char>('8'):
	case static_cast<unsigned char>('9'):
	{
		matchRange('1','9');
		break;
	}
	case static_cast<unsigned char>('a'):  case static_cast<unsigned char>('b'):  case static_cast<unsigned char>('c'):  case static_cast<unsigned char>('d'):
	case static_cast<unsigned char>('e'):  case static_cast<unsigned char>('f'):
	{
		matchRange('a','f');
		break;
	}
	case static_cast<unsigned char>('A'):  case static_cast<unsigned char>('B'):  case static_cast<unsigned char>('C'):  case static_cast<unsigned char>('D'):
	case static_cast<unsigned char>('E'):  case static_cast<unsigned char>('F'):
	{
		matchRange('A','F');
		break;
	}
	default:
	{
		throw ScannerException(std::string("no viable alt for char: ")+charName(LA(1)),getLine());
	}
	}
	}
	{
	switch ( LA(1)) {
	case static_cast<unsigned char>('0'):  case static_cast<unsigned char>('1'):  case static_cast<unsigned char>('2'):  case static_cast<unsigned char>('3'):
	case static_cast<unsigned char>('4'):  case static_cast<unsigned char>('5'):  case static_cast<unsigned char>('6'):  case static_cast<unsigned char>('7'):
	case static_cast<unsigned char>('8'):  case static_cast<unsigned char>('9'):
	{
		matchRange('0','9');
		break;
	}
	case static_cast<unsigned char>('a'):  case static_cast<unsigned char>('b'):  case static_cast<unsigned char>('c'):  case static_cast<unsigned char>('d'):
	case static_cast<unsigned char>('e'):  case static_cast<unsigned char>('f'):
	{
		matchRange('a','f');
		break;
	}
	case static_cast<unsigned char>('A'):  case static_cast<unsigned char>('B'):  case static_cast<unsigned char>('C'):  case static_cast<unsigned char>('D'):
	case static_cast<unsigned char>('E'):  case static_cast<unsigned char>('F'):
	{
		matchRange('A','F');
		break;
	}
	default:
		{
		}
	}
	}
	if ( _createToken && _token==0 ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
}

void EscLexer::mSlashoctal(bool _createToken) {
	FT("EscLexer::mSlashoctal", (_createToken));
	int _ttype; RefToken _token; int _begin=text.length();
	_ttype = Slashoctal;
	int _saveIndex;
	RefToken c(0);
	
	_saveIndex=text.length();
	match("\\");
	text.erase(_saveIndex);
	mOCTALDIGIT(true);
	c=_returnToken;
	int ival; sscanf(c->getText().c_str(),"%o",&ival); retstring=retstring+mstring((char)ival);
	if ( _createToken && _token==0 ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
}

void EscLexer::mOCTALDIGIT(bool _createToken) {
	FT("EscLexer::mOCTALDIGIT", (_createToken));
	int _ttype; RefToken _token; int _begin=text.length();
	_ttype = OCTALDIGIT;
	int _saveIndex;
	
	if (((LA(1) >= static_cast<unsigned char>('0') && LA(1) <= static_cast<unsigned char>('7'))) && ((LA(2) >= static_cast<unsigned char>('0') && LA(2) <= static_cast<unsigned char>('7'))) && ((LA(3) >= static_cast<unsigned char>('0') && LA(3) <= static_cast<unsigned char>('7')))) {
		{
		matchRange('0','7');
		}
		{
		matchRange('0','7');
		}
		{
		matchRange('0','7');
		}
	}
	else if (((LA(1) >= static_cast<unsigned char>('0') && LA(1) <= static_cast<unsigned char>('7'))) && ((LA(2) >= static_cast<unsigned char>('0') && LA(2) <= static_cast<unsigned char>('7')))) {
		{
		matchRange('0','7');
		}
		{
		matchRange('0','7');
		}
	}
	else if (((LA(1) >= static_cast<unsigned char>('0') && LA(1) <= static_cast<unsigned char>('7')))) {
		{
		matchRange('0','7');
		}
	}
	else {
		throw ScannerException(std::string("no viable alt for char: ")+charName(LA(1)),getLine());
	}
	
	if ( _createToken && _token==0 ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
}

void EscLexer::mSlashtrash(bool _createToken) {
	FT("EscLexer::mSlashtrash", (_createToken));
	int _ttype; RefToken _token; int _begin=text.length();
	_ttype = Slashtrash;
	int _saveIndex;
	RefToken d(0);
	
	_saveIndex=text.length();
	match("\\");
	text.erase(_saveIndex);
	{
	switch ( LA(1)) {
	case static_cast<unsigned char>('c'):  case static_cast<unsigned char>('d'):  case static_cast<unsigned char>('e'):
	{
		matchRange('c','e');
		break;
	}
	case static_cast<unsigned char>('g'):  case static_cast<unsigned char>('h'):  case static_cast<unsigned char>('i'):  case static_cast<unsigned char>('j'):
	case static_cast<unsigned char>('k'):  case static_cast<unsigned char>('l'):  case static_cast<unsigned char>('m'):
	{
		matchRange('g','m');
		break;
	}
	case static_cast<unsigned char>('o'):  case static_cast<unsigned char>('p'):  case static_cast<unsigned char>('q'):
	{
		matchRange('o','q');
		break;
	}
	case static_cast<unsigned char>('u'):
	{
		match('u');
		break;
	}
	case static_cast<unsigned char>('w'):
	{
		match('w');
		break;
	}
	case static_cast<unsigned char>('y'):
	{
		match('y');
		break;
	}
	case static_cast<unsigned char>('z'):
	{
		match('z');
		break;
	}
	case static_cast<unsigned char>('A'):  case static_cast<unsigned char>('B'):  case static_cast<unsigned char>('C'):  case static_cast<unsigned char>('D'):
	case static_cast<unsigned char>('E'):  case static_cast<unsigned char>('F'):  case static_cast<unsigned char>('G'):  case static_cast<unsigned char>('H'):
	case static_cast<unsigned char>('I'):  case static_cast<unsigned char>('J'):  case static_cast<unsigned char>('K'):  case static_cast<unsigned char>('L'):
	case static_cast<unsigned char>('M'):  case static_cast<unsigned char>('N'):  case static_cast<unsigned char>('O'):  case static_cast<unsigned char>('P'):
	case static_cast<unsigned char>('Q'):  case static_cast<unsigned char>('R'):  case static_cast<unsigned char>('S'):  case static_cast<unsigned char>('T'):
	case static_cast<unsigned char>('U'):  case static_cast<unsigned char>('V'):  case static_cast<unsigned char>('W'):
	{
		matchRange('A','W');
		break;
	}
	case static_cast<unsigned char>('Y'):
	{
		match('Y');
		break;
	}
	case static_cast<unsigned char>('Z'):
	{
		match('Z');
		break;
	}
	default:
	{
		throw ScannerException(std::string("no viable alt for char: ")+charName(LA(1)),getLine());
	}
	}
	}
	retstring=retstring+mstring(d->getText());
	if ( _createToken && _token==0 ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
}

void EscLexer::mNewLine(bool _createToken) {
	FT("EscLexer::mNewLine", (_createToken));
	int _ttype; RefToken _token; int _begin=text.length();
	_ttype = NewLine;
	int _saveIndex;
	
	{
	if ((LA(1)==static_cast<unsigned char>('\r')) && (LA(2)==static_cast<unsigned char>('\n'))) {
		match("\r\n");
	}
	else if ((LA(1)==static_cast<unsigned char>('\r'))) {
		match('\r');
	}
	else if ((LA(1)==static_cast<unsigned char>('\n'))) {
		match('\n');
	}
	else {
		throw ScannerException(std::string("no viable alt for char: ")+charName(LA(1)),getLine());
	}
	
	}
	newline(); retstring=retstring+mstring("\n");
	if ( _createToken && _token==0 ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
}

void EscLexer::mIGNORE(bool _createToken) {
	FT("EscLexer::mIGNORE", (_createToken));
	int _ttype; RefToken _token; int _begin=text.length();
	_ttype = IGNORE;
	int _saveIndex;
	char  a = '\0';
	
	a = LA(1);
	matchNot(EOF);
	retstring=retstring+mstring(a);
	if ( _createToken && _token==0 ) {
	   _token = makeToken(_ttype);
	   _token->setText(text.substr(_begin, text.length()-_begin));
	}
	_returnToken = _token;
}


const unsigned long EscLexer::_tokenSet_0_data_[] = { 0UL, 0UL, 117440510UL, 111394744UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL };
const BitSet EscLexer::_tokenSet_0(_tokenSet_0_data_,10);

