// frontend/lexer/cursor.hpp

#pragma once

#include "frontend/source/source.hpp"

#include "predicates.hpp"

class Cursor {
public:

	explicit Cursor(const Source& source) noexcept
		: source{source} {}

	[[nodiscard]] Span span() const noexcept {
		return Span{
			.start = start,
			.end = current,
		};
	}

	[[nodiscard]] StringView slice() const noexcept {
		return source.view(start, current - start);
	}

	void sync() noexcept {
		start = current;
	}

	[[nodiscard]] Char peek() const noexcept {
		return source[current];
	}

	void advance(Size offset = 1) noexcept {
		ASSERT(current + offset <= source.size(), "Advance out of bounds");
		current += offset;
	}

	void advance_if(CharPredicate auto predicate) noexcept {
		if (check(predicate))
			advance();
	}

	void advance_while(CharPredicate auto predicate) noexcept {
		while (check(predicate))
			advance();
	}

	void advance_until(CharPredicate auto predicate) noexcept {
		while (!eof() && !check(predicate))
			advance();
	}

	[[nodiscard]] Char consume() noexcept {
		ASSERT(!eof(), "Consume out of bounds");
		return source[current++];
	}

	[[nodiscard]] Bool check(Char expected) const noexcept {
		return peek() == expected;
	}

	[[nodiscard]] Bool eof() const noexcept {
		return check('\0');
	}

	[[nodiscard]] Bool check(StringView expected) const noexcept {
		if (current + expected.size() > source.size())
			return false;
		return source.view(current, expected.size()) == expected;
	}

	[[nodiscard]] Bool check(CharPredicate auto predicate) const noexcept {
		return predicate(peek());
	}

	[[nodiscard]] Bool match(Char expected) noexcept {
		if (!check(expected))
			return false;
		advance();
		return true;
	}

	[[nodiscard]] Bool match(StringView expected) noexcept {
		if (!check(expected))
			return false;
		advance(expected.size());
		return true;
	}

	[[nodiscard]] Bool match(CharPredicate auto predicate) noexcept {
		if (!check(predicate))
			return false;
		advance();
		return true;
	}

private:

	const Source& source;
	Pos start = 0;
	Pos current = 0;

};