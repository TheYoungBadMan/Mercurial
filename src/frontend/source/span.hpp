// frontend/source/span.hpp

#pragma once

#include "core/types.hpp"
#include "core/contract.hpp"

struct Span {
	Pos start;
	Pos end;

	[[nodiscard]] constexpr Size length() const noexcept {
		ASSERT(end >= start, "Invalid span");
		return end - start;
	}
};