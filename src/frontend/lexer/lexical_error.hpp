// frontend/lexer/lexical_error.hpp

#pragma once

#include "core/types.hpp"

#include "frontend/source/span.hpp"
#include "frontend/token/token.hpp"

struct LexicalError {
	String message;
	Span span;
};

using LexicalResult = Result<Token, LexicalError>;
