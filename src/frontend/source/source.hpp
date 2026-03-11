// frontend/source/source.hpp

#pragma once

#include "core/types.hpp"
#include "core/contract.hpp"

class Source {
public:
	explicit Source(String buffer) noexcept : buffer{std::move(buffer)} {}
	~Source() noexcept = default;

	Source(const Source&) = delete;
	Source& operator=(const Source&) = delete;

	Source(Source&&) noexcept = default;
	Source& operator=(Source&&) noexcept = default;

	[[nodiscard]] Char operator[](Size index) const noexcept {
		ASSERT(index < buffer.size(), "Index out of bounds");
		return buffer[index];
	}

	[[nodiscard]] Size size() const noexcept {
		return buffer.size();
	}

	[[nodiscard]] StringView view() const noexcept {
		return StringView(buffer);
	}
	
	[[nodiscard]] StringView view(Size offset, Size length) const noexcept { 
		return StringView(buffer.data() + offset, length);
	}

private:
	String buffer;
};