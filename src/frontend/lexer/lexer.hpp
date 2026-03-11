// frontend/lexer/lexer.hpp

#pragma once

#include <format>

#include "core/types.hpp"
#include "core/contract.hpp"

#include "frontend/token/token.hpp"
#include "frontend/source/source.hpp"

#include "cursor.hpp"
#include "keywords.hpp"
#include "predicates.hpp"
#include "lexical_error.hpp"

struct LexerResult {
	Vector<Token> tokens;
	Vector<LexicalError> errors;

	[[nodiscard]] Bool ok() const noexcept {
		return errors.empty();
	}
};

class Lexer {

	Cursor cursor;

	Lexer(SourceId source_id, const Source& source) noexcept
		: cursor{source_id, source} {}

public:

	[[nodiscard]] static LexerResult tokenize(SourceId source_id, const Source& source) {
		return Lexer{source_id, source}.tokenize();
	}

private:
	[[nodiscard]] LexerResult tokenize() {
		LexerResult result;

		for (;;) {
			if (!skip()) {
				result.errors.push_back(make_error("Unterminated block comment"));
				cursor.sync();
			}

			if (cursor.eof())
				break;

			if (auto extracted = extract(); extracted.has_value())
				result.tokens.push_back(extracted.value());
			else
				result.errors.push_back(extracted.error());

			cursor.sync();
		}

		result.tokens.push_back(make_token(TokenKind::Eof));
		return result;
	}

	[[nodiscard]] Bool skip() noexcept {
		for (;;) {
			cursor.sync();
			if (cursor.match(is_whitespace)) {
				skip_whitespaces();
			} else if (cursor.match("//")) {
				skip_line_comment();
			} else if (cursor.match("/*")) {
				if (!skip_block_comment()) return false;
			} else {
				break;
			}
		}

		return true;
	}

	void skip_whitespaces() noexcept {
		while (cursor.match(is_whitespace)) {}
	}

	void skip_line_comment() noexcept {
		while (!cursor.eof() && !cursor.match('\n')) cursor.advance();
	}

	[[nodiscard]] Bool skip_block_comment() noexcept {
		while (!cursor.eof()) {
			if (cursor.match("*/"))
				return true;
			cursor.advance();
		}

		return false;
	}

	[[nodiscard]] LexicalResult extract() {
		if (cursor.match(is_ident_start))
			return extract_identifier();

		if (cursor.check(is_digit))
			return extract_number();

		if (cursor.check(is_quote))
			return extract_quote();

		return extract_operator();
	}

	[[nodiscard]] Token extract_identifier() noexcept {
		while (cursor.match(is_ident_continue)) {}

		auto lexeme = cursor.slice();
		return make_token(lookup_keyword(lexeme));
	}
	
	[[nodiscard]] LexicalResult extract_number() {
		if (cursor.match("0b")) {
			if (!cursor.match(is_bin_digit))
				return make_unexpected("Expected binary digit after '0b' prefix");
			return extract_radix_number(is_bin_digit);
		}

		if (cursor.match("0o")) {
			if (!cursor.match(is_oct_digit))
				return make_unexpected("Expected octal digit after '0o' prefix");
			return extract_radix_number(is_oct_digit);
		}

		if (cursor.match("0x")) {
			if (!cursor.match(is_hex_digit))
				return make_unexpected("Expected hexadecimal digit after '0x' prefix");
			return extract_radix_number(is_hex_digit);
		}

		return extract_decimal_number();
	}

	template <CharPredicate Predicate>
	[[nodiscard]] LexicalResult extract_radix_number(Predicate predicate) noexcept {
		while (cursor.match(predicate)) {}

		return make_token(TokenKind::Integer);
	}

	[[nodiscard]] LexicalResult extract_decimal_number() {
		// TODO: Handle _ as separator
		while (cursor.match(is_digit)) {}

		if (cursor.match('e') || cursor.match('E'))
			return extract_exponent();

		if (cursor.match('.')) {
			while (cursor.match(is_digit)) {}

			if (cursor.match('e') || cursor.match('E'))
				return extract_exponent();

			return make_token(TokenKind::Float);
		}

		return make_token(TokenKind::Integer);
	}

	[[nodiscard]] LexicalResult extract_exponent() {
		if (cursor.eof())
			return make_unexpected("Unexpected end of exponent");

		if (cursor.match('+') || cursor.match('-')) {}

		if (!cursor.match(is_digit))
			return make_unexpected("Expected digit in exponent");

		while (cursor.match(is_digit)) {}

		return make_token(TokenKind::Float);
	}

	[[nodiscard]] LexicalResult extract_quote() {
		auto quote = cursor.consume();

		while (!cursor.eof() && !cursor.match(quote)) {
			if (cursor.match('\\')) {
				if (cursor.eof())
					break;
			}
			cursor.advance();
		}

		if (cursor.eof()) {
			if (quote == '"')
				return make_unexpected("Unterminated string literal");
			else
				return make_unexpected("Unterminated character literal");
		}

		if (quote == '"')
			return make_token(TokenKind::String);
		else
			return make_token(TokenKind::Character);
	}

	[[nodiscard]] LexicalResult extract_operator() {
		switch (auto ch = cursor.consume(); ch) {
			case '+': return make_token(TokenKind::Plus);
			case '-':
				if (cursor.match('>'))
					return make_token(TokenKind::Arrow);
				else
					return make_token(TokenKind::Minus);
			case '*': return make_token(TokenKind::Star);
			case '/': return make_token(TokenKind::Slash);
			case '%': return make_token(TokenKind::Percent);
			case '=':
				if (cursor.match('='))
					return make_token(TokenKind::EqualEqual);
				else if (cursor.match('>'))
					return make_token(TokenKind::FatArrow);
				else 
					return make_token(TokenKind::Equal);
			case '!':
				if (cursor.match('='))
					return make_token(TokenKind::BangEqual);
				else
					return make_token(TokenKind::Bang);
			case '<':
				if (cursor.match('='))
					return make_token(TokenKind::LessEqual);
				else if (cursor.match('<'))
					return make_token(TokenKind::LessLess);
				else
					return make_token(TokenKind::Less);
			case '>':
				if (cursor.match('='))
					return make_token(TokenKind::GreaterEqual);
				else if (cursor.match('>'))
					return make_token(TokenKind::GreaterGreater);
				else
					return make_token(TokenKind::Greater);
			case '&':
				if (cursor.match('&'))
					return make_token(TokenKind::AmpAmp);
				else
					return make_token(TokenKind::Amp);
			case '|':
				if (cursor.match('|'))
					return make_token(TokenKind::BarBar);
				else
					return make_token(TokenKind::Bar);
			case '^': return make_token(TokenKind::Caret);
			case '~': return make_token(TokenKind::Tilde);
			case ':': return make_token(TokenKind::Colon);
			case ';': return make_token(TokenKind::Semicolon);
			case ',': return make_token(TokenKind::Comma);
			case '.': return make_token(TokenKind::Dot);
			case '(': return make_token(TokenKind::LeftParen);
			case ')': return make_token(TokenKind::RightParen);
			case '{': return make_token(TokenKind::LeftBrace);
			case '}': return make_token(TokenKind::RightBrace);
			case '[': return make_token(TokenKind::LeftBracket);
			case ']': return make_token(TokenKind::RightBracket);
			default:
				return make_unexpected(std::format("Unexpected character '{}'", ch));
		}
	}

	[[nodiscard]] Token make_token(TokenKind kind) const noexcept {
		return Token {
			.kind = kind,
			.span = cursor.span(),
		};
	}

	[[nodiscard]] LexicalError make_error(String message) const {
		return LexicalError{
			.message = std::move(message),
			.span = cursor.span(),
		};
	}

	[[nodiscard]] LexicalResult make_unexpected(String message) const {
		return std::unexpected(make_error(std::move(message)));
	}
};

