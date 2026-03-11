// frontend/lexer/lexical_error.hpp

#pragma once

#include "core/types.hpp"

#include "frontend/source/span.hpp"

enum class LexicalErrorKind : u8 {
	UnknownChar,
	
	UnterminatedChar,
	UnterminatedString,
	UnterminatedComment,
	
	InvalidBinLiteral,
	InvalidOctLiteral,
	InvalidHexLiteral,
	InvalidExponent,
	InvalidEscape,
};

struct LexicalError {
	LexicalErrorKind kind;
	Span span;
};
