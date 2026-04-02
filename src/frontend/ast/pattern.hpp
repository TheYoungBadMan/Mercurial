// frontend/ast/pattern.hpp

#pragma once

#include "frontend/ast/node.hpp"

struct ParenPattern : PatternNode {
	PatternPtr inner; // optional, if null then this is an empty pattern '()'

	ParenPattern(Span span, PatternPtr inner = nullptr) noexcept
		: PatternNode{span}, inner{std::move(inner)} {}

	void accept(Visitor& visitor) override;
};

struct ListPattern : PatternNode {
	PatternList patterns;

	ListPattern(Span span, PatternList patterns) noexcept
		: PatternNode{span}, patterns{std::move(patterns)} {}

	void accept(Visitor& visitor) override;
};

struct NamedPattern : PatternNode {
	using PatternNode::PatternNode;	

	void accept(Visitor& visitor) override;
};

struct LiteralPattern : PatternNode {
	Literal kind;

	LiteralPattern(Span span, Literal kind) noexcept
		: PatternNode{span}, kind{kind} {}

	void accept(Visitor& visitor) override;
};