// frontend/ast/decl.hpp

#pragma once

#include "ast.hpp"

struct VarDecl : DeclNode {
	BindingList bindings;

	VarDecl(Span span, Visibility visibility, BindingList bindings) noexcept
		: DeclNode{span, visibility}, bindings{std::move(bindings)} {}

};

struct FnDecl : DeclNode {
	Ident name;
	BindingList params;
	TypePtr ret_type = nullptr;
	BlockPtr body;

	FnDecl(Span span, Visibility visibility, Ident name, BindingList params, BlockPtr body) noexcept
		: DeclNode{span, visibility},
		  name{name},
		  params{std::move(params)},
		  body{std::move(body)} {}

	FnDecl(Span span, Visibility visibility, Ident name, BindingList params, TypePtr ret_type, BlockPtr body) noexcept
		: DeclNode{span, visibility},
		  name{name},
		  params{std::move(params)},
		  ret_type{std::move(ret_type)},
		  body{std::move(body)} {}

};

struct RecordDecl : DeclNode {
	Ident name;
	BindingList fields;

	RecordDecl(Span span, Visibility visibility, Ident name, BindingList fields) noexcept
		: DeclNode{span, visibility}, name{name}, fields{std::move(fields)} {}
};

struct VariantDecl : DeclNode {
	// TBD

	using DeclNode::DeclNode;
};

struct NamespaceDecl : DeclNode {
	Ident name;
	DeclList decls;

	NamespaceDecl(Span span, Visibility visibility, Ident name, DeclList decls) noexcept
		: DeclNode{span, visibility}, name{name}, decls{std::move(decls)} {}
};

struct AliasDecl : DeclNode {
	Ident name;
	TypePtr aliased_type;

	AliasDecl(Span span, Visibility visibility, Ident name, TypePtr aliased_type) noexcept
		: DeclNode{span, visibility}, name{name}, aliased_type{std::move(aliased_type)} {}
};