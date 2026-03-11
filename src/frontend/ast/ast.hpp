#pragma once

#include "core/types.hpp"

#include "source/span.hpp"

struct ASTNode {
	Span span;

	ASTNode(Span span) noexcept
		: span{span} {}

	virtual ~ASTNode() noexcept = default;
};

enum class Visibility : u8 {
	Public,
	Private,
};

struct DeclNode : ASTNode {
	Visibility visibility;

	DeclNode(Span span, Visibility visibility) noexcept
		: ASTNode{span}, visibility{visibility} {}
};

struct StmtNode : ASTNode {
	using ASTNode::ASTNode;
};

struct ExprNode : ASTNode {
	using ASTNode::ASTNode;
};

struct BindingNode : ASTNode {
	using ASTNode::ASTNode;
};

struct TypeNode : ASTNode {
	using ASTNode::ASTNode;
};

using DeclPtr = UniquePtr<DeclNode>;
using StmtPtr = UniquePtr<StmtNode>;
using ExprPtr = UniquePtr<ExprNode>;
using TypePtr = UniquePtr<TypeNode>;
using BindingPtr = UniquePtr<BindingNode>;

using DeclList = Vector<DeclPtr>;
using StmtList = Vector<StmtPtr>;
using ExprList = Vector<ExprPtr>;
using TypeList = Vector<TypePtr>;
using BindingList = Vector<BindingPtr>;

struct BlockNode : ASTNode {
	StmtList stmts;

	BlockNode(Span span, StmtList stmts) noexcept
		: ASTNode{span}, stmts{std::move(stmts)} {}
};

using BlockPtr = UniquePtr<BlockNode>;

using Ident = Span;
using Literal = Span;

using Module = DeclList;