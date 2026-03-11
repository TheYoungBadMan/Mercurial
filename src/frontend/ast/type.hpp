// frontend/ast/type.hpp

#pragma once

#include "ast.hpp"

struct NamedType : TypeNode {
	using TypeNode::TypeNode;
};

struct GenericType : TypeNode {
	TypeList type_args;
	
	GenericType(Span span, TypeList type_args) noexcept
		: TypeNode{span}, type_args{std::move(type_args)} {}
};

struct ArrayType : TypeNode {
	TypePtr element_type;
	ExprPtr size;

	ArrayType(Span span, TypePtr element_type, ExprPtr size) noexcept
		: TypeNode{span}, element_type{std::move(element_type)}, size{std::move(size)} {}
};

struct TupleType : TypeNode {
	TypeList types;

	TupleType(Span span, TypeList types) noexcept
		: TypeNode{span}, types{std::move(types)} {}
};

struct VariantType : TypeNode {
	TypeList types;

	VariantType(Span span, TypeList types) noexcept
		: TypeNode{span}, types{std::move(types)} {}
};

struct FunctionType : TypeNode {
	TypeList param_types;
	TypePtr return_type;

	FunctionType(Span span, TypeList param_types, TypePtr return_type) noexcept
		: TypeNode{span}, param_types{std::move(param_types)}, return_type{std::move(return_type)} {}
};