#ifndef INC_EscParser_hpp_
#define INC_EscParser_hpp_

#include "antlr/config.hpp"
/*
 * ANTLR-generated file resulting from grammar escparse.g
 * 
 * Terence Parr, MageLang Institute
 * with John Lilley, Empathy Software
 * ANTLR Version 2.6.1; 1996-1999
 */

#include "antlr/TokenStream.hpp"
#include "antlr/TokenBuffer.hpp"
#include "EscLexerTokenTypes.hpp"
#include "antlr/LLkParser.hpp"


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
#include <string>
using namespace std;
#include "mstring.h"

class EscParser : public LLkParser, public EscLexerTokenTypes
 {
protected:
	EscParser(TokenBuffer& tokenBuf, int k);
public:
	EscParser(TokenBuffer& tokenBuf);
protected:
	EscParser(TokenStream& lexer, int k);
public:
	EscParser(TokenStream& lexer);
	EscParser(const ParserSharedInputState& state);
	public: void expr();
	public: void expr1();
	public: void slashexpr();
private:
	static const char* _tokenNames[];
	
	static const unsigned long _tokenSet_0_data_[];
	static const BitSet _tokenSet_0;
	static const unsigned long _tokenSet_1_data_[];
	static const BitSet _tokenSet_1;
};

#endif /*INC_EscParser_hpp_*/
