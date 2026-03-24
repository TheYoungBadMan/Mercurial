// frontend/lexer/predicates.hpp

#pragma once

#include <concepts>

#include "core/types.hpp"

template<typename Predicate>
concept CharPredicate = std::is_invocable_v<Predicate, Char> &&
						std::same_as<std::invoke_result_t<Predicate, Char>, Bool>;

constexpr Bool is_newline(Char c) noexcept {
	return c == '\n' || c == '\r';
}

constexpr Bool is_whitespace(Char c) noexcept {
	return c == ' ' || c == '\t' || is_newline(c);
}

constexpr Bool is_alpha(Char c) noexcept {
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

constexpr Bool is_digit(Char c) noexcept {
	return c >= '0' && c <= '9';
}

constexpr Bool is_alnum(Char c) noexcept {
	return is_alpha(c) || is_digit(c);
}

constexpr Bool is_hex_digit(Char c) noexcept {
	return is_digit(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}

constexpr Bool is_oct_digit(Char c) noexcept {
	return c >= '0' && c <= '7';
}

constexpr Bool is_bin_digit(Char c) noexcept {
	return c == '0' || c == '1';
}

constexpr Bool is_exponent(Char c) noexcept {
	return c == 'e' || c == 'E';
}

constexpr Bool is_sign(Char c) noexcept {
	return c == '+' || c == '-';
}

constexpr Bool is_ident_start(Char c) noexcept {
	return is_alpha(c) || c == '_';
}

constexpr Bool is_ident_continue(Char c) noexcept {
	return is_alnum(c) || c == '_';
}

constexpr Bool is_quote(Char c) noexcept {
	return c == '"' || c == '\'';
}

constexpr Bool is_escapable(Char c) noexcept {
	return c == '0' ||
			c == 'n' ||
			c == 't' ||
			c == 'r' ||
			c == '\\' ||
			c == '\'' ||
			c == '"';
}