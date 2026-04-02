// frontend/ast/type.hpp

#pragma once

#include "node.hpp"

// -------------------- Types -------------------- //

struct ArrayType : TypeNode {
	TypePtr elem;
	ExprPtr size;

	ArrayType(Span span, TypePtr elem, ExprPtr size) noexcept
		: TypeNode{span},
		  elem{std::move(elem)},
		  size{std::move(size)}
		  {}

	void accept(Visitor& visitor) override;
};

struct ParenType : TypeNode {
	TypePtr inner; // unit type if null

	ParenType(Span span, TypePtr inner = nullptr) noexcept
		: TypeNode{span},
		  inner{std::move(inner)}
		  {}

	void accept(Visitor& visitor) override;
};

struct TupleType : TypeNode {
	TypeList elems;

	TupleType(Span span, TypeList elems) noexcept
		: TypeNode{span},
		  elems{std::move(elems)}
		  {}

	void accept(Visitor& visitor) override;
};

struct FunctionType : TypeNode {
	TypeList params;
	TypePtr ret_type; // auto if null

	FunctionType(Span span, TypeList params, TypePtr ret_type = nullptr) noexcept
		: TypeNode{span},
		  params{std::move(params)},
		  ret_type{std::move(ret_type)}
		  {}

	void accept(Visitor& visitor) override;
};

struct NamedType : TypeNode {
	using TypeNode::TypeNode;

	void accept(Visitor& visitor) override;
};