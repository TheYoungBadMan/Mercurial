// frontend/ast/node.hpp

#pragma once

#include "frontend/source/span.hpp"

// -------------------- FORWARD DECLARATIONS -------------------- //

struct Node;
struct PatternNode;
struct TypeNode;
struct DeclNode;
struct StmtNode;
struct ExprNode;

// -------------------- TYPE ALIASES -------------------- //

using PatternPtr = UniquePtr<PatternNode>;
using TypePtr = UniquePtr<TypeNode>;
using DeclPtr = UniquePtr<DeclNode>;
using StmtPtr = UniquePtr<StmtNode>;
using ExprPtr = UniquePtr<ExprNode>;

using PatternList = Vector<PatternPtr>;
using TypeList = Vector<TypePtr>;
using DeclList = Vector<DeclPtr>;
using StmtList = Vector<StmtPtr>;
using ExprList = Vector<ExprPtr>;

// -------------------- HELPER TYPES -------------------- //

using Ident = Span;

enum class Literal : u8 {
	Integer,
	Float,
	Boolean,
	Character,
	String,
	Unit,
	Pass,
	Todo,
	Default,
};

using Module = DeclList;

using BlockItem = Variant<DeclPtr, StmtPtr>;

struct Block {
	Span span;
	Vector<BlockItem> items;
};

using BlockPtr = UniquePtr<Block>;

struct Binding {
	Span span;
	PatternPtr pattern;
	TypePtr type;
	ExprPtr init = nullptr; // optional
};

using BindingList = Vector<Binding>;

// -------------------- NODES -------------------- //

class Visitor;

struct Node {
	Span span;

	explicit Node(Span span) noexcept
		: span{span} {}

	virtual ~Node() noexcept = default;
	virtual void accept(Visitor& visitor) = 0;
};

struct PatternNode : Node { using Node::Node; };
struct TypeNode : Node { using Node::Node; };
struct DeclNode : Node { using Node::Node; };
struct StmtNode : Node { using Node::Node; };
struct ExprNode : Node { using Node::Node; };