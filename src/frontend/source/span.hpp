// frontend/source/span.hpp

#pragma once

#include "core/types.hpp"

struct Span {
	Pos start;
	Pos end;

	[[nodiscard]] constexpr Size length() const noexcept {
		return end - start;
	}
};