// frontend/ast/decl.hpp

#pragma once

#include "node.hpp"

struct VarDecl : DeclNode {
	BindingList bindings;

	explicit VarDecl(Span span, BindingList bindings) noexcept
		: DeclNode{span},
		  bindings{std::move(bindings)} {}

	void accept(Visitor& visitor) override;
};

struct AliasBinding {
	Span span;
	Ident name;
	TypePtr type;
};

using AliasBindingList = Vector<AliasBinding>;

struct AliasDecl : DeclNode {
	AliasBindingList bindings;

	AliasDecl(Span span, AliasBindingList bindings) noexcept
		: DeclNode{span},
		  bindings{std::move(bindings)} {}

	void accept(Visitor& visitor) override;
};

using ImportItem = Ident;

using ImportItemList = Vector<ImportItem>;

struct ImportDecl : DeclNode {
	ImportItemList items;

	ImportDecl(Span span, ImportItemList items) noexcept
		: DeclNode{span},
		  items{std::move(items)} {}

	void accept(Visitor& visitor) override;
};

struct FnDecl : DeclNode {
	Ident name;
	BindingList params;
	TypePtr ret_type; // optional
	BlockPtr body;

	FnDecl(Span span, Ident name, BindingList params, TypePtr ret_type, BlockPtr body) noexcept
		: DeclNode{span},
		  name{name},
		  params{std::move(params)},
		  ret_type{std::move(ret_type)},
		  body{std::move(body)} {}

	void accept(Visitor& visitor) override;
};

struct RecordDecl : DeclNode {
	Ident name;
	BindingList fields;

	RecordDecl(Span span, Ident name, BindingList fields) noexcept
		: DeclNode{span},
		  name{name},
		  fields{std::move(fields)} {}

	void accept(Visitor& visitor) override;
};

struct NamespaceDecl : DeclNode {
	Ident name;
	DeclList decls;

	NamespaceDecl(Span span, Ident name, DeclList decls) noexcept
		: DeclNode{span},
		  name{name},
		  decls{std::move(decls)} {}

	void accept(Visitor& visitor) override;
};