// frontend/token/token.hpp

#pragma once

#include "frontend/source/span.hpp"

#include "token_kind.hpp"

struct Token {
	TokenKind kind;
	Span span;

	[[nodiscard]] constexpr Bool is(TokenKind kind) const noexcept {
		return this->kind == kind;
	}

	template <typename... Kinds>
	[[nodiscard]] constexpr Bool is_one_of(Kinds... kinds) const noexcept {
		return (is(kinds) || ...);
	}
};