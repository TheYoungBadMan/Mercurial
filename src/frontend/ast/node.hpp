// frontend/ast/node.hpp

#pragma once

#include "frontend/source/span.hpp"

// --------------------- Nodes --------------------- //

class Visitor;

struct Node {
	Span span;

	explicit Node(Span span) noexcept
		: span{span} {}

	virtual ~Node() noexcept = default;
	virtual void accept(Visitor& visitor) = 0;
};

struct DeclNode : Node { using Node::Node; };
struct StmtNode : Node { using Node::Node; };
struct ExprNode : Node { using Node::Node; };
struct TypeNode : Node { using Node::Node; };
struct PatternNode : Node { using Node::Node; };

// -------------------- Node aliases -------------------- //

using DeclPtr = UniquePtr<DeclNode>;
using StmtPtr = UniquePtr<StmtNode>;
using ExprPtr = UniquePtr<ExprNode>;
using TypePtr = UniquePtr<TypeNode>;
using PatternPtr = UniquePtr<PatternNode>;

using DeclList = Vector<DeclPtr>;
using StmtList = Vector<StmtPtr>;
using ExprList = Vector<ExprPtr>;
using TypeList = Vector<TypePtr>;
using PatternList = Vector<PatternPtr>;

// -------------------- Helpers -------------------- //

using Ident = Span;

enum class Literal : u8 {
	Integer,
	Float,
	Boolean,
	Character,
	String,
	Pass,
	Todo,
	Default,
};

struct Binding {
	Span span;
	PatternPtr pattern;
	TypePtr type;
	ExprPtr init = nullptr; // optional
};
using BindingList = Vector<Binding>;

using BlockItem = Variant<DeclPtr, StmtPtr>;
using BlockItemList = Vector<BlockItem>;
struct Block {
	Span span;
	BlockItemList items;
};
using BlockPtr = UniquePtr<Block>;

using Module = DeclList;