// frontend/lexer/lexer.hpp

#pragma once

#include "frontend/token/token.hpp"

#include "cursor.hpp"
#include "keywords.hpp"
#include "lexical_error.hpp"

#define LEXER_ATTEMPT(action) \
	do { \
		if (auto result = action; !result) \
			return result; \
	} while (false)

#define LEXER_EXPECT(condition, error_kind) \
	do { \
		if (!(condition)) \
			return make_unexpected(error_kind); \
	} while (false)

struct LexerResult {
	Vector<Token> tokens;
	Vector<LexicalError> errors;
	
	[[nodiscard]] Bool ok() const noexcept {
		return errors.empty();
	}

	void add_token(Token token) {
		tokens.push_back(std::move(token));
	}

	void add_error(LexicalError error) {
		errors.push_back(std::move(error));
	}
};

class Lexer {
public:

	[[nodiscard]] static LexerResult tokenize(const Source& source) {
		return Lexer{source}.tokenize();
	}

private:

	// -------------------- FIELDS -------------------- //

	Cursor cursor;

	Lexer(const Source& source) noexcept
		: cursor{source} {}


	// -------------------- RESULT --------------------- //


	using ExtractResult = Result<Token, LexicalError>;
	using SkipResult = Result<void, LexicalError>;

	[[nodiscard]] Token make_token(TokenKind kind) const noexcept {
		return Token {
			.kind = kind,
			.span = cursor.span(),
		};
	}

	[[nodiscard]] LexicalError make_error(LexicalErrorKind kind) const noexcept {
		return LexicalError{
			.kind = kind,
			.span = cursor.span(),
		};
	}

	[[nodiscard]] auto make_unexpected(LexicalErrorKind kind) const noexcept {
		return std::unexpected(make_error(kind));
	}

	// -------------------- MAIN LOOP -------------------- //

	[[nodiscard]] LexerResult tokenize() {
		LexerResult result;

		for (;;) {
			if (auto skip_result = skip_trivia(); !skip_result)
				result.add_error(skip_result.error());

			if (cursor.eof())
				break;

			if (auto extracted = extract(); extracted)
				result.add_token(extracted.value());
			else
				result.add_error(extracted.error());
		}

		cursor.sync();
		result.add_token(make_token(TokenKind::Eof));
		return result;
	}

	// -------------------- SKIPPING -------------------- //

	[[nodiscard]] SkipResult skip_trivia() noexcept {
		for (;;) {
			cursor.sync();
			if (cursor.match(is_whitespace))
				skip_whitespaces();
			else if (cursor.match("//"))
				skip_line_comment();
			else if (cursor.match("/*"))
				LEXER_ATTEMPT(skip_block_comment());
			else
				break;
		}

		return {};
	}

	void skip_whitespaces() noexcept {
		cursor.advance_while(is_whitespace);
	}

	void skip_line_comment() noexcept {
		cursor.advance_until(is_newline);
	}

	[[nodiscard]] SkipResult skip_block_comment() noexcept {
		Size depth = 1;

		while (!cursor.eof()) {
			if (cursor.match("/*"))
				++depth;
			else if (cursor.match("*/")) {
				if (--depth == 0)
					break;
			} else {
				cursor.advance();
			}
		}

		LEXER_EXPECT(depth == 0, LexicalErrorKind::UnterminatedComment);
		return {};
	}

	// -------------------- EXTRACTION -------------------- //

	[[nodiscard]] ExtractResult extract() noexcept {
		cursor.sync();

		if (cursor.match(is_ident_start))
			return extract_identifier();

		if (cursor.check(is_digit))
			return extract_number();

		if (cursor.check(is_quote))
			return extract_quote();

		return extract_operator();
	}

	[[nodiscard]] Token extract_identifier() noexcept {
		cursor.advance_while(is_ident_continue);

		auto lexeme = cursor.slice();
		return make_token(lookup_keyword(lexeme));
	}
	
	[[nodiscard]] ExtractResult extract_number() noexcept {
		if (cursor.match("0b")) {
			LEXER_EXPECT(cursor.match(is_bin_digit), LexicalErrorKind::InvalidBinLiteral);
			return extract_radix_number(is_bin_digit);
		}

		if (cursor.match("0o")) {
			LEXER_EXPECT(cursor.match(is_oct_digit), LexicalErrorKind::InvalidOctLiteral);
			return extract_radix_number(is_oct_digit);
		}

		if (cursor.match("0x")) {
			LEXER_EXPECT(cursor.match(is_hex_digit), LexicalErrorKind::InvalidHexLiteral);
			return extract_radix_number(is_hex_digit);
		}

		return extract_decimal_number();
	}

	[[nodiscard]] ExtractResult extract_radix_number(CharPredicate auto predicate) noexcept {
		cursor.advance_while(predicate);

		return make_token(TokenKind::Integer);
	}

	[[nodiscard]] ExtractResult extract_decimal_number() noexcept {
		cursor.advance_while(is_digit);

		if (cursor.match(is_exponent))
			return extract_exponent();

		if (cursor.match('.')) {
			cursor.advance_while(is_digit);

			if (cursor.match(is_exponent))
				return extract_exponent();

			return make_token(TokenKind::Float);
		}

		return make_token(TokenKind::Integer);
	}

	[[nodiscard]] ExtractResult extract_exponent() noexcept {
		LEXER_EXPECT(!cursor.eof(), LexicalErrorKind::InvalidExponent);
		cursor.advance_if(is_sign);
		LEXER_EXPECT(cursor.match(is_digit), LexicalErrorKind::InvalidExponent);
		cursor.advance_while(is_digit);
		return make_token(TokenKind::Float);
	}

	[[nodiscard]] ExtractResult extract_quote() noexcept {
		auto quote = cursor.consume(); // consume opening quote

		while (!cursor.eof() && !cursor.check(quote)) {
			if (cursor.match('\\')) {
				if (cursor.eof())
					break;

				LEXER_EXPECT(cursor.match(is_escapable), LexicalErrorKind::InvalidEscape);
			} else {
				cursor.advance();
			}
		}

		LEXER_EXPECT(cursor.match(quote),
			quote == '"' ?
				LexicalErrorKind::UnterminatedString :
				LexicalErrorKind::UnterminatedChar
		);

		return make_token(
			quote == '"' ?
				TokenKind::String :
				TokenKind::Character
		);
	}

	[[nodiscard]] ExtractResult extract_operator() noexcept {
		switch (auto ch = cursor.consume(); ch) {
			case '+': 
				if (cursor.match('='))
					return make_token(TokenKind::PlusEqual);
				else
					return make_token(TokenKind::Plus);
			case '-':
				if (cursor.match('='))
					return make_token(TokenKind::MinusEqual);
				else if (cursor.match('>'))
					return make_token(TokenKind::Arrow);
				else
					return make_token(TokenKind::Minus);
			case '*':
				if (cursor.match('='))
					return make_token(TokenKind::StarEqual);
				else
					return make_token(TokenKind::Star);
			case '/':
				if (cursor.match('='))
					return make_token(TokenKind::SlashEqual);
				else
					return make_token(TokenKind::Slash);
			case '%':
				if (cursor.match('='))
					return make_token(TokenKind::PercentEqual);
				else
					return make_token(TokenKind::Percent);
			case '=':
				if (cursor.match('='))
					return make_token(TokenKind::EqualEqual);
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
				else if (cursor.match('<')) {
					if (cursor.match('='))
						return make_token(TokenKind::LessLessEqual);
					else
						return make_token(TokenKind::LessLess);
				} else
					return make_token(TokenKind::Less);
			case '>':
				if (cursor.match('='))
					return make_token(TokenKind::GreaterEqual);
				else if (cursor.match('>')) {
					if (cursor.match('='))
						return make_token(TokenKind::GreaterGreaterEqual);
					else
						return make_token(TokenKind::GreaterGreater);
				} else
					return make_token(TokenKind::Greater);
			case '&':
				if (cursor.match('='))
					return make_token(TokenKind::AmpEqual);
				else if (cursor.match('&'))
					return make_token(TokenKind::AmpAmp);
				else
					return make_token(TokenKind::Amp);
			case '|':
				if (cursor.match('='))
					return make_token(TokenKind::BarEqual);
				else if (cursor.match('|'))
					return make_token(TokenKind::BarBar);
				else
					return make_token(TokenKind::Bar);
			case '^':
				if (cursor.match('='))
					return make_token(TokenKind::CaretEqual);
				else
					return make_token(TokenKind::Caret);
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
				return make_unexpected(LexicalErrorKind::UnknownChar);
		}
	}
};

