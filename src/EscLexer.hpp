#ifndef INC_EscLexer_hpp_
#define INC_EscLexer_hpp_

#include "antlr/config.hpp"
/*
 * ANTLR-generated file resulting from grammar escparse.g
 * 
 * Terence Parr, MageLang Institute
 * with John Lilley, Empathy Software
 * ANTLR Version 2.6.1; 1996-1999
 */

#include "antlr/CommonToken.hpp"
#include "antlr/InputBuffer.hpp"
#include "antlr/BitSet.hpp"
#include "EscLexerTokenTypes.hpp"
#include "antlr/CharScanner.hpp"

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

class EscLexer : public CharScanner, public EscLexerTokenTypes
 {

public:
	mstring retstring;
private:
	void initLiterals();
public:
	bool getCaseSensitiveLiterals() const;
public:
	EscLexer(std::istream& in);
	EscLexer(InputBuffer& ib);
	EscLexer(const LexerSharedInputState& state);
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
	public: void mHEXDIGIT(bool _createToken);
	public: void mSlashoctal(bool _createToken);
	public: void mOCTALDIGIT(bool _createToken);
	public: void mSlashtrash(bool _createToken);
	public: void mNewLine(bool _createToken);
	protected: void mMignore(bool _createToken);
private:
	
	static const unsigned long _tokenSet_0_data_[];
	static const BitSet _tokenSet_0;
};

#endif /*INC_EscLexer_hpp_*/
