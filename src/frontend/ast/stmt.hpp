// frontend/ast/stmt.hpp

#pragma once

#include "node.hpp"

// -------------------- Helpers ---------------------- //

enum class AssignOp : u8 {
	Assign,     // =
	AddAssign,  // +=
	SubAssign,  // -=
	MulAssign,  // *=
	DivAssign,  // /=
	ModAssign,  // %=
	AndAssign,  // &=
	OrAssign,   // |=
	XorAssign,  // ^=
	ShlAssign,  // <<=
	ShrAssign,  // >>=
};

// -------------------- Statements -------------------- //

struct IfStmt : StmtNode {
	ExprPtr cond;
	BlockPtr if_block;
	BlockPtr else_block; // optional

	IfStmt(Span span, ExprPtr cond, BlockPtr if_block, BlockPtr else_block = nullptr) noexcept
		: StmtNode{span},
		  cond{std::move(cond)},
		  if_block{std::move(if_block)},
		  else_block{std::move(else_block)}
		  {}

	void accept(Visitor& visitor) override;
};

struct WhileStmt : StmtNode {
	ExprPtr cond;
	BlockPtr block;

	WhileStmt(Span span, ExprPtr cond, BlockPtr block) noexcept
		: StmtNode{span},
		  cond{std::move(cond)},
		  block{std::move(block)}
		  {}

	void accept(Visitor& visitor) override;
};

struct LoopStmt : StmtNode {
	BlockPtr block;

	LoopStmt(Span span, BlockPtr block) noexcept
		: StmtNode{span},
		  block{std::move(block)}
		  {}

	void accept(Visitor& visitor) override;
};

struct ReturnStmt : StmtNode {
	ExprPtr expr; // unit if null

	ReturnStmt(Span span, ExprPtr expr = nullptr) noexcept
		: StmtNode{span},
		  expr{std::move(expr)}
		  {}

	void accept(Visitor& visitor) override;
};

struct BreakStmt : StmtNode {
	using StmtNode::StmtNode;

	void accept(Visitor& visitor) override;
};

struct ContinueStmt : StmtNode {
	using StmtNode::StmtNode;

	void accept(Visitor& visitor) override;
};

struct AssignStmt : StmtNode {
	ExprPtr lhs;
	AssignOp op;
	ExprPtr rhs;

	AssignStmt(Span span, ExprPtr lhs, AssignOp op, ExprPtr rhs) noexcept
		: StmtNode{span},
		  lhs{std::move(lhs)},
		  op{op},
		  rhs{std::move(rhs)}
		  {}

	void accept(Visitor& visitor) override;
};

struct ExprStmt : StmtNode {
	ExprPtr expr;

	ExprStmt(Span span, ExprPtr expr) noexcept
		: StmtNode{span},
		  expr{std::move(expr)}
		  {}

	void accept(Visitor& visitor) override;
};