// frontend/source/source.hpp

#pragma once

#include "core/contract.hpp"

#include "span.hpp"

class Source {
public:
	explicit Source(String buffer)
	: buffer{std::move(buffer)} {
		this->buffer.push_back('\0');
	}

	Source(const Source&) = delete;
	Source& operator=(const Source&) = delete;

	Source(Source&&) = default;
	Source& operator=(Source&&) = default;

	[[nodiscard]] Char operator[](Size index) const noexcept {
		ASSERT(index < buffer.size(), "Index out of bounds");
		return buffer[index];
	}

	[[nodiscard]] Size size() const noexcept {
		return buffer.size() - 1;
	}

	[[nodiscard]] StringView view() const noexcept {
		return view(0, size());
	}

	[[nodiscard]] StringView view(Span span) const noexcept {
		return view(span.start, span.length());
	}

	[[nodiscard]] StringView view(Size start, Size length) const noexcept {
		ASSERT(start + length <= size(), "View out of bounds");
		return StringView(buffer.data() + start, length);
	}

private:
	String buffer;
};