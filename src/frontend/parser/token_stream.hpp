// frontend/parser/token_stream.hpp

#pragma once

#include "core/contract.hpp"

#include "frontend/token/token.hpp"

class TokenStream {
public:

	explicit TokenStream(const Vector<Token>& tokens) noexcept
		: tokens_(tokens) {
		ASSERT(!tokens.empty(), "Token stream cannot be empty");
		ASSERT(tokens.back().is(TokenKind::Eof), "Token stream must end with Eof token");
	}

	// -------------------- Positioning -------------------- //

	[[nodiscard]] Pos start() const noexcept {
		return peek().span.start;
	}

	[[nodiscard]] Span span_from(Pos start) const noexcept {
		return Span{
			.start = start,
			.end = prev().span.end,
		};
	}

	// -------------------- Navigation -------------------- //

	void advance() noexcept {
		ASSERT(!eof(), "Cannot advance past eof");
		++current_;
	}

	// -------------------- Accessors -------------------- //

	[[nodiscard]] const Token& peek() const noexcept {
		return tokens_[current_];
	}

	[[nodiscard]] const Token& prev() const noexcept {
		ASSERT(current_ > 0, "No previous token available");
		return tokens_[current_ - 1];
	}

	[[nodiscard]] const Token& consume() noexcept {
		ASSERT(!eof(), "Cannot consume past eof");
		return tokens_[current_++];
	}

	// -------------------- Checking -------------------- //

	[[nodiscard]] Bool check(TokenKind kind) const noexcept {
		return peek().is(kind);
	}

	[[nodiscard]] Bool eof() const noexcept {
		return check(TokenKind::Eof);
	}

	[[nodiscard]] Bool match(TokenKind kind) noexcept {
		if (!check(kind))
			return false;

		advance();
		return true;
	}

private:
	const Vector<Token>& tokens_;
	Size current_ = 0;
};