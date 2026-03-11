// frontend/ast/stmt.hpp

#pragma once

#include "ast.hpp"

struct PassStmt : StmtNode {
	using StmtNode::StmtNode;
};

struct TodoStmt : StmtNode {
	using StmtNode::StmtNode;
};

enum class AssignOp : u8 {
	Assign,     // =
	AddAssign,  // +=
	SubAssign,  // -=
	MulAssign,  // *=
	DivAssign,  // /=
	ModAssign,  // %=
	BitAndAssign, // &=
	BitOrAssign,  // |=
	BitXorAssign, // ^=
	ShlAssign,   // <<=
	ShrAssign,   // >>=
};

struct AssignStmt : StmtNode {
	ExprPtr lhs;
	AssignOp op;
	ExprPtr rhs;

	AssignStmt(Span span, ExprPtr lhs, AssignOp op, ExprPtr rhs) noexcept
		: StmtNode{span}, lhs{std::move(lhs)}, op{op}, rhs{std::move(rhs)} {}
};

struct ExprStmt : StmtNode {
	ExprPtr expr;

	ExprStmt(Span span, ExprPtr expr) noexcept
		: StmtNode{span}, expr{std::move(expr)} {}
};

struct DeclStmt : StmtNode {
	DeclPtr decl;

	DeclStmt(Span span, DeclPtr decl) noexcept
		: StmtNode{span}, decl{std::move(decl)} {}
};

struct IfStmt : StmtNode {
	DeclPtr init = nullptr;
	ExprPtr cond;
	BlockPtr if_body;
	BlockPtr else_body = nullptr;

	IfStmt(Span span, ExprPtr cond, BlockPtr if_body, BlockPtr else_body = nullptr) noexcept
		: StmtNode{span},
		  cond{std::move(cond)},
		  if_body{std::move(if_body)},
		  else_body{std::move(else_body)} {}

	IfStmt(Span span, DeclPtr init, ExprPtr cond, BlockPtr if_body, BlockPtr else_body = nullptr) noexcept
		: StmtNode{span},
		  init{std::move(init)},
		  cond{std::move(cond)},
		  if_body{std::move(if_body)},
		  else_body{std::move(else_body)} {}
};

struct LoopStmt : StmtNode {
	DeclPtr init = nullptr;
	BlockPtr body;

	LoopStmt(Span span, BlockPtr body) noexcept
		: StmtNode{span}, body{std::move(body)} {}

	LoopStmt(Span span, DeclPtr init, BlockPtr body) noexcept
		: StmtNode{span}, init{std::move(init)}, body{std::move(body)} {}
};

struct WhileStmt : StmtNode {
	DeclPtr init = nullptr;
	ExprPtr cond;
	BlockPtr body;

	WhileStmt(Span span, DeclPtr init, ExprPtr cond, BlockPtr body) noexcept
		: StmtNode{span},
		  init{std::move(init)},
		  cond{std::move(cond)},
		  body{std::move(body)} {}

	WhileStmt(Span span, ExprPtr cond, BlockPtr body) noexcept
		: StmtNode{span}, cond{std::move(cond)}, body{std::move(body)} {}
};

struct ReturnStmt : StmtNode {
	ExprPtr expr = nullptr;

	ReturnStmt(Span span, ExprPtr expr) noexcept
		: StmtNode{span}, expr{std::move(expr)} {}
};

struct LeaveStmt : StmtNode {
	ExprPtr expr = nullptr;

	LeaveStmt(Span span, ExprPtr expr) noexcept
		: StmtNode{span}, expr{std::move(expr)} {}
};

struct BreakStmt : StmtNode {
	using StmtNode::StmtNode;
};

struct ContinueStmt : StmtNode {
	using StmtNode::StmtNode;
};