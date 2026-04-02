// frontend/ast/expr.hpp

#pragma once

#include "node.hpp"

enum class BinaryOp : u8 {
	Add, Sub, Mul, Div, Mod,
	And, Or,
	Eq, Neq, Lt, Gt, Leq, Geq,
	BitAnd, BitOr, BitXor, Shl, Shr,
};

struct BinaryExpr : ExprNode {
	ExprPtr lhs;
	BinaryOp op;
	ExprPtr rhs;

	BinaryExpr(Span span, ExprPtr lhs, BinaryOp op, ExprPtr rhs) noexcept
		: ExprNode{span},
		  lhs{std::move(lhs)},
		  op{op},
		  rhs{std::move(rhs)} {}

	void accept(Visitor& visitor) override;
};

enum class UnaryOp : u8 {
	Pos, Neg,
	Not, BitNot,
};

struct UnaryExpr : ExprNode {
	UnaryOp op;
	ExprPtr operand;

	UnaryExpr(Span span, UnaryOp op, ExprPtr operand) noexcept
		: ExprNode{span},
		  op{op},
		  operand{std::move(operand)} {}

	void accept(Visitor& visitor) override;
};

struct CallExpr : ExprNode {
	ExprPtr callee;
	ExprList args;

	CallExpr(Span span, ExprPtr callee, ExprList args) noexcept
		: ExprNode{span},
		  callee{std::move(callee)},
		  args{std::move(args)} {}

	void accept(Visitor& visitor) override;
};

struct SubscriptExpr : ExprNode {
	ExprPtr object;
	ExprPtr index;

	SubscriptExpr(Span span, ExprPtr object, ExprPtr index) noexcept
		: ExprNode{span},
		  object{std::move(object)}, index{std::move(index)} {}

	void accept(Visitor& visitor) override;
};

struct MemberExpr : ExprNode {
	ExprPtr object;
	Ident member;
	Bool is_index; // false if .name, true if .index

	MemberExpr(Span span, ExprPtr object, Ident member, Bool is_index = false) noexcept
		: ExprNode{span},
		  object{std::move(object)}, member{member}, is_index{is_index} {}

	void accept(Visitor& visitor) override;
};

struct ParenExpr : ExprNode {
	ExprPtr expr; // optional, if null then it's a unit literal `()`

	ParenExpr(Span span, ExprPtr expr = nullptr) noexcept
		: ExprNode{span},
		  expr{std::move(expr)} {}

	void accept(Visitor& visitor) override;
};

struct TupleExpr : ExprNode {
	ExprList elems;

	TupleExpr(Span span, ExprList elems) noexcept
		: ExprNode{span},
		  elems{std::move(elems)} {}

	void accept(Visitor& visitor) override;
};

struct ArrayExpr : ExprNode {
	ExprList elems;

	ArrayExpr(Span span, ExprList elems) noexcept
		: ExprNode{span},
		  elems{std::move(elems)} {}

	void accept(Visitor& visitor) override;
};

struct IdentExpr : ExprNode {
	using ExprNode::ExprNode;

	void accept(Visitor& visitor) override;
};

struct FieldInit {
	Ident name;
	ExprPtr value;
};

struct RecordExpr : ExprNode {
	Ident name;
	Vector<FieldInit> fields;

	RecordExpr(Span span, Ident name, Vector<FieldInit> fields) noexcept
		: ExprNode{span},
		  name{name}, fields{std::move(fields)} {}

	void accept(Visitor& visitor) override;
};

struct LambdaExpr : ExprNode {
	BindingList params;
	TypePtr ret; // optional
	BlockPtr body;

	LambdaExpr(Span span, BindingList params, TypePtr ret, BlockPtr body) noexcept
		: ExprNode{span},
		  params{std::move(params)},
		  ret{std::move(ret)},
		  body{std::move(body)} {}

	void accept(Visitor& visitor) override;
};

struct LitExpr : ExprNode {
	Literal kind;

	LitExpr(Span span, Literal kind) noexcept
		: ExprNode{span},
		  kind{kind} {}

	void accept(Visitor& visitor) override;
};