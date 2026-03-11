// frontend/ast/expr.hpp

#pragma once

#include "ast.hpp"

enum class BinaryOp : u8 {
	Add, Sub, Mul, Div, Mod,
	And, Or,
	Eq, Neq, Lt, Gt, Leq, Geq,
	BitAnd, BitOr, BitXor, Shl, Shr,
};

struct BinaryExpr : ExprNode {
	ExprPtr lhs;
	ExprPtr rhs;
	BinaryOp op;

	BinaryExpr(Span span, ExprPtr lhs, ExprPtr rhs, BinaryOp op) noexcept
		: ExprNode{span}, lhs{std::move(lhs)}, rhs{std::move(rhs)}, op{op} {}
};

enum class UnaryOp : u8 {
	Pos, Neg,
	Not, Inv,
};

struct UnaryExpr : ExprNode {
	ExprPtr base;
	UnaryOp op;

	UnaryExpr(Span span, ExprPtr base, UnaryOp op) noexcept
		: ExprNode{span}, base{std::move(base)}, op{op} {}
};

struct CallExpr : ExprNode {
	ExprPtr base;
	ExprList args;

	CallExpr(Span span, ExprPtr base, ExprList args) noexcept
		: ExprNode{span}, base{std::move(base)}, args{std::move(args)} {}
};

struct SubscriptExpr : ExprNode {
	ExprPtr base;
	ExprPtr index;

	SubscriptExpr(Span span, ExprPtr base, ExprPtr index) noexcept
		: ExprNode{span}, base{std::move(base)}, index{std::move(index)} {}
};

struct MemberExpr : ExprNode {
	ExprPtr base;
	Ident member;

	MemberExpr(Span span, ExprPtr base, Ident member) noexcept
		: ExprNode{span}, base{std::move(base)}, member{member} {}
};

struct BlockExpr : ExprNode {
	BlockPtr block;
	ExprPtr tail;

	BlockExpr(Span span, BlockPtr block, ExprPtr tail = nullptr) noexcept
		: ExprNode{span}, block{std::move(block)}, tail{std::move(tail)} {}
};

struct ParenExpr : ExprNode {
	ExprPtr base;

	ParenExpr(Span span, ExprPtr base) noexcept
		: ExprNode{span}, base{std::move(base)} {}
};

struct TupleExpr : ExprNode {
	ExprList elements;

	TupleExpr(Span span, ExprList elements) noexcept
		: ExprNode{span}, elements{std::move(elements)} {}
};

struct ArrayExpr : ExprNode {
	ExprList elements;

	ArrayExpr(Span span, ExprList elements) noexcept
		: ExprNode{span}, elements{std::move(elements)} {}
};

struct LambdaExpr : ExprNode {
	BindingList params;
	TypePtr ret_type = nullptr;
	BlockPtr body;

	LambdaExpr(Span span, BindingList params, BlockPtr body) noexcept
		: ExprNode{span}, params{std::move(params)}, body{std::move(body)} {}

	LambdaExpr(Span span, BindingList params, TypePtr ret_type, BlockPtr body) noexcept
		: ExprNode{span},
		params{std::move(params)},
		ret_type{std::move(ret_type)},
		body{std::move(body)} {}
};

struct IdentExpr : ExprNode {
	IdentExpr(Span span) noexcept
		: ExprNode{span} {}
};

enum class LiteralKind : u8 {
	Integer,
	Float,
	Bool,
	Character,
	String,
	Unit,
};

struct LitExpr : ExprNode {
	LiteralKind kind;

	LitExpr(Span span, LiteralKind kind) noexcept
		: ExprNode{span}, kind{kind} {}
};