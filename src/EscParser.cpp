/*
 * ANTLR-generated file resulting from grammar escparse.g
 * 
 * Terence Parr, MageLang Institute
 * with John Lilley, Empathy Software
 * ANTLR Version 2.6.1; 1996-1999
 */

#include "EscParser.hpp"
#include "antlr/NoViableAltException.hpp"
#include "antlr/SemanticException.hpp"
EscParser::EscParser(TokenBuffer& tokenBuf, int k)
: LLkParser(tokenBuf,k)
{
	setTokenNames(_tokenNames);
}

EscParser::EscParser(TokenBuffer& tokenBuf)
: LLkParser(tokenBuf,1)
{
	setTokenNames(_tokenNames);
}

EscParser::EscParser(TokenStream& lexer, int k)
: LLkParser(lexer,k)
{
	setTokenNames(_tokenNames);
}

EscParser::EscParser(TokenStream& lexer)
: LLkParser(lexer,1)
{
	setTokenNames(_tokenNames);
}

EscParser::EscParser(const ParserSharedInputState& state)
: LLkParser(state,1)
{
	setTokenNames(_tokenNames);
}

void EscParser::expr() {
	
	
	try {      // for error handling
		{
		for (;;) {
			if (((LA(1) >= Slashn && LA(1) <= NewLine))) {
				expr1();
			}
			else {
				goto _loop34;
			}
			
		}
		_loop34:;
		}
	}
	catch (ParserException& ex) {
		if (inputState->guessing==0) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_0);
		} else {
			throw ex;
		}
	}
}

void EscParser::expr1() {
	
	
	try {      // for error handling
		switch ( LA(1)) {
		case Slashn:
		case Slasht:
		case Slashv:
		case Slashb:
		case Slashr:
		case Slashf:
		case Slasha:
		case Slashslash:
		case Slashquestion:
		case Slashsinglequote:
		case Slashdoublequote:
		case Slashhexidecimal:
		case Slashoctal:
		case Slashtrash:
		{
			slashexpr();
			break;
		}
		case NewLine:
		{
			match(NewLine);
			break;
		}
		default:
		{
			throw NoViableAltException(LT(1));
		}
		}
	}
	catch (ParserException& ex) {
		if (inputState->guessing==0) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_1);
		} else {
			throw ex;
		}
	}
}

void EscParser::slashexpr() {
	
	
	try {      // for error handling
		switch ( LA(1)) {
		case Slashn:
		{
			match(Slashn);
			break;
		}
		case Slasht:
		{
			match(Slasht);
			break;
		}
		case Slashv:
		{
			match(Slashv);
			break;
		}
		case Slashb:
		{
			match(Slashb);
			break;
		}
		case Slashr:
		{
			match(Slashr);
			break;
		}
		case Slashf:
		{
			match(Slashf);
			break;
		}
		case Slasha:
		{
			match(Slasha);
			break;
		}
		case Slashslash:
		{
			match(Slashslash);
			break;
		}
		case Slashquestion:
		{
			match(Slashquestion);
			break;
		}
		case Slashsinglequote:
		{
			match(Slashsinglequote);
			break;
		}
		case Slashdoublequote:
		{
			match(Slashdoublequote);
			break;
		}
		case Slashhexidecimal:
		{
			match(Slashhexidecimal);
			break;
		}
		case Slashoctal:
		{
			match(Slashoctal);
			break;
		}
		case Slashtrash:
		{
			match(Slashtrash);
			break;
		}
		default:
		{
			throw NoViableAltException(LT(1));
		}
		}
	}
	catch (ParserException& ex) {
		if (inputState->guessing==0) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_1);
		} else {
			throw ex;
		}
	}
}

const char* EscParser::_tokenNames[] = {
	"<0>",
	"EOF",
	"<2>",
	"NULL_TREE_LOOKAHEAD",
	"Slashn",
	"Slasht",
	"Slashv",
	"Slashb",
	"Slashr",
	"Slashf",
	"Slasha",
	"Slashslash",
	"Slashquestion",
	"Slashsinglequote",
	"Slashdoublequote",
	"Slashhexidecimal",
	"Slashoctal",
	"Slashtrash",
	"NewLine",
	"HEXDIGIT",
	"OCTALDIGIT",
	"Mignore",
	0
};

const unsigned long EscParser::_tokenSet_0_data_[] = { 2UL, 0UL, 0UL, 0UL };
const BitSet EscParser::_tokenSet_0(_tokenSet_0_data_,4);
const unsigned long EscParser::_tokenSet_1_data_[] = { 524274UL, 0UL, 0UL, 0UL };
const BitSet EscParser::_tokenSet_1(_tokenSet_1_data_,4);


