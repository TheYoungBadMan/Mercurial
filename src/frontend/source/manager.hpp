// frontend/source/manager.hpp

#pragma once

#include "source.hpp"

class SourceManager {
public:
	SourceManager() = default;

	SourceManager(const SourceManager&) = delete;
	SourceManager& operator=(const SourceManager&) = delete;

	SourceManager(SourceManager&&) noexcept = default;
	SourceManager& operator=(SourceManager&&) noexcept = default;

	[[nodiscard]] Size size() const noexcept {
		return sources.size();
	}

	[[nodiscard]] SourceId add_source(String filename, String buffer) {
		sources.emplace_back(std::move(filename), Source(std::move(buffer)));
		return static_cast<SourceId>(sources.size() - 1);
	}

	[[nodiscard]] StringView filename(SourceId id) const noexcept {
		ASSERT(id < sources.size(), "Invalid source ID");
		return sources[id].filename;
	}

	[[nodiscard]] const Source& source(SourceId id) const noexcept {
		ASSERT(id < sources.size(), "Invalid source ID");
		return sources[id].source;
	}

private:
	struct SourceEntry {
		String filename;
		Source source;
	};

	Vector<SourceEntry> sources;
};
