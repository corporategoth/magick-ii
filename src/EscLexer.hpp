#ifndef INC_EscLexer_hpp_
#define INC_EscLexer_hpp_

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

#include "antlr/CommonToken.hpp"
#include "antlr/CharBuffer.hpp"
#include "antlr/BitSet.hpp"
#include "antlr/CharScanner.hpp"
class EscLexer : public CharScanner
 {

public:
	mstring retstring;
private:
	void initLiterals();
public:
	bool getCaseSensitiveLiterals() const;
public:
	EscLexer(std::istream& in);
	EscLexer(CharBuffer& cb);
	RefToken nextToken();
	public: void mSlashn(bool _createToken);
	public: void mSlasht(bool _createToken);
	public: void mSlashv(bool _createToken);
	public: void mSlashb(bool _createToken);
	public: void mSlashr(bool _createToken);
	public: void mSlashf(bool _createToken);
	public: void mSlasha(bool _createToken);
	public: void mSlashslash(bool _createToken);
	public: void mSlashquestion(bool _createToken);
	public: void mSlashsinglequote(bool _createToken);
	public: void mSlashdoublequote(bool _createToken);
	public: void mSlashhexidecimal(bool _createToken);
	protected: void mHEXDIGIT(bool _createToken);
	public: void mSlashoctal(bool _createToken);
	protected: void mOCTALDIGIT(bool _createToken);
	public: void mSlashtrash(bool _createToken);
	public: void mNewLine(bool _createToken);
	protected: void mIGNORE(bool _createToken);
private:
	
	static const unsigned long _tokenSet_0_data_[];
	static const BitSet _tokenSet_0;
};

#endif /*INC_EscLexer_hpp_*/
