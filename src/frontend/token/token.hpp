// frontend/token/token.hpp

#pragma once

#include "source/span.hpp"

#include "token_kind.hpp"

struct Token {
	TokenKind kind;
	Span span;

	[[nodiscard]] constexpr bool is(TokenKind kind) const noexcept {
		return this->kind == kind;
	}
};