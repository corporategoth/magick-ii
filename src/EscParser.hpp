#ifndef INC_EscParser_hpp_
#define INC_EscParser_hpp_

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

#include "antlr/Tokenizer.hpp"
#include "antlr/TokenBuffer.hpp"
#include "antlr/LLkParser.hpp"

class EscParser : public LLkParser
 {

	mstring retstring;
public:
protected:
	EscParser(TokenBuffer& tokenBuf, int k);
public:
	EscParser(TokenBuffer& tokenBuf);
protected:
	EscParser(Tokenizer& lexer, int k);
public:
	EscParser(Tokenizer& lexer);
	public: void expr();
	public: void slashexpr();
private:
	static const char* _tokenNames[];
	
	static const unsigned long _tokenSet_0_data_[];
	static const BitSet _tokenSet_0;
	static const unsigned long _tokenSet_1_data_[];
	static const BitSet _tokenSet_1;
};

#endif /*INC_EscParser_hpp_*/
