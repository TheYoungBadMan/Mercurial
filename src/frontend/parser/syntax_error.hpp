// frontend/parser/syntax_error.hpp

#pragma once

#include "frontend/source/span.hpp"

using Mismatch = TokenKind;
enum class Expected : u8 {
	Expression,
	Declaration,
	Type,
	Pattern,
	Member,
};

enum class Context : u8 {
	Binding,
	Pattern, ParenPattern,
	Type, ArrayType, ParenType, FunctionType,
	Decl, VarDecl, AliasDecl, ImportDecl, FnDecl, RecordDecl, NamespaceDecl,
	IfStmt, WhileStmt, LoopStmt, BreakStmt, ContinueStmt, ReturnStmt, ExprStmt, AssignStmt,
	Expr, CallExpr, SubscriptExpr, MemberExpr, ParenExpr, ArrayExpr, RecordExpr, LambdaExpr,
};

using SyntaxErrorKind = Variant<Mismatch, Expected>;

struct SyntaxError {
	Span span;
	SyntaxErrorKind kind;
	Context context;
};