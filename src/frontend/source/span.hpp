// frontend/source/span.hpp

#pragma once

#include "core/types.hpp"

struct Span {
	SourceId source_id;
	Pos start;
	Pos end;

	[[nodiscard]] constexpr Size size() const noexcept {
		return static_cast<Size>(end - start);
	}
};