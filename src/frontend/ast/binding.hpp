// frontend/ast/binding.hpp

#pragma once

#include "ast.hpp"

struct NamedBinding : BindingNode {
	using BindingNode::BindingNode;
};

// After SMVP
struct TupleBinding : BindingNode {
	BindingList elements;

	TupleBinding(Span span, BindingList elements) noexcept
		: BindingNode{span}, elements{std::move(elements)} {}
};

struct TypedBinding : BindingNode {
	BindingPtr binding;
	TypePtr type;

	TypedBinding(Span span, BindingPtr binding, TypePtr type) noexcept
		: BindingNode{span}, binding{std::move(binding)}, type{std::move(type)} {}
};

struct InitializedBinding : BindingNode {
	BindingPtr binding;
	ExprPtr init;

	InitializedBinding(Span span, BindingPtr binding, ExprPtr init) noexcept
		: BindingNode{span}, binding{std::move(binding)}, init{std::move(init)} {}
};