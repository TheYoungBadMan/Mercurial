// frontend/source/manager.hpp

#pragma once

#include "core/types.hpp"
#include "core/contract.hpp"

#include "source/source.hpp"

class SourceManager {
public:

	SourceManager() noexcept = default;
	~SourceManager() noexcept = default;

	SourceManager(const SourceManager&) = delete;
	SourceManager& operator=(const SourceManager&) = delete;

	SourceManager(SourceManager&&) noexcept = default;
	SourceManager& operator=(SourceManager&&) noexcept = default;

	[[nodiscard]]
	SourceId add(String filename, String buffer) {
		SourceId id = sources.size();
		sources.emplace_back(std::move(filename), Source{std::move(buffer)});
		return id;
	}

	[[nodiscard]]
	StringView filename(SourceId id) const noexcept {
		ASSERT(id < sources.size(), "Invalid source ID");
		return sources[id].first;
	}

	[[nodiscard]]
	const Source& source(SourceId id) const noexcept {
		ASSERT(id < sources.size(), "Invalid source ID");
		return sources[id].second;
	}

	[[nodiscard]]
	Size size() const noexcept {
		return sources.size();
	}

private:
	Vector<Pair<String, Source>> sources;
};
