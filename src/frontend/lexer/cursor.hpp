// frontend/lexer/cursor.hpp

#pragma once

#include "frontend/source/source.hpp"

#include "predicates.hpp"

class Cursor {
public:
	Cursor(SourceId source_id, const Source& source) noexcept
		: source_id{source_id}, source{source} {}

	void sync() noexcept {
		start = cur;
	}

	[[nodiscard]] Span span() const noexcept {
		return Span{
			.source_id = source_id,
			.start = start,
			.end = cur,
		};
	}

	[[nodiscard]] StringView slice() const noexcept {
		return source.view(start, cur - start);
	}

	void advance() noexcept {
		++cur;
	}

	void advance(Size offset) noexcept {
		ASSERT(cur + offset <= source.size(), "Advance out of bounds");
		cur += offset;
	}

	[[nodiscard]] Char consume() noexcept {
		return source[cur++];
	}

	[[nodiscard]] Char peek() const noexcept {
		return source[cur];
	}

	[[nodiscard]] Bool eof() const noexcept {
		return peek() == '\0';
	}

	[[nodiscard]] Bool check(Char expected) const noexcept {
		return peek() == expected;
	}

	[[nodiscard]] Bool check(StringView expected) const noexcept {
		return source.view(cur, expected.size()) == expected;
	}

	template<CharPredicate Predicate>
	[[nodiscard]] Bool check(Predicate predicate) const noexcept {
		return predicate(peek());
	}

	[[nodiscard]] Bool match(Char expected) noexcept {
		if (!check(expected)) return false;
		advance();
		return true;
	}

	[[nodiscard]] Bool match(StringView expected) noexcept {
		if (!check(expected)) return false;
		advance(expected.size());
		return true;
	}

	template <CharPredicate Predicate>
	[[nodiscard]] Bool match(Predicate predicate) noexcept {
		if (!check(predicate)) return false;
		advance();
		return true;
	}

private:
	SourceId source_id;
	const Source& source;
	Pos start = 0;
	Pos cur = 0;
};