// frontend/parser/parse_stmt.cpp

#include "parser.hpp"

#include "core/contract.hpp"
#include "frontend/ast/stmt.hpp"

Parser::ParseResult<StmtPtr> Parser::parse_stmt() {
	if (stream.check(TokenKind::If))
		return parse_if_stmt();
	else if (stream.check(TokenKind::While))
		return parse_while_stmt();
	else if (stream.check(TokenKind::Loop))
		return parse_loop_stmt();
	else if (stream.check(TokenKind::Break))
		return parse_break_stmt();
	else if (stream.check(TokenKind::Continue))
		return parse_continue_stmt();
	else if (stream.check(TokenKind::Return))
		return parse_return_stmt();
	else
		return parse_expr_stmt();
}

Parser::ParseResult<StmtPtr> Parser::parse_if_stmt() {
	constexpr auto ctx = Context::IfStmt;
	auto start = stream.start();

	ASSERT(stream.check(TokenKind::If), "Expected 'if' token");
	stream.advance(); // consume 'if'
	EXPECT(LeftParen);
	auto condition = PARSE_ATTEMPT(parse_expr());
	EXPECT(RightParen);
	auto if_block = PARSE_ATTEMPT(parse_block(ctx));

	BlockPtr else_block = nullptr;
	if (stream.match(TokenKind::Else))
		else_block = PARSE_ATTEMPT(parse_block(ctx));
	
	return std::make_unique<IfStmt>(
		stream.span_from(start),
		std::move(condition),
		std::move(if_block),
		std::move(else_block)
	);
}

Parser::ParseResult<StmtPtr> Parser::parse_while_stmt() {
	constexpr auto ctx = Context::WhileStmt;
	auto start = stream.start();

	ASSERT(stream.check(TokenKind::While), "Expected 'while' token");
	stream.advance(); // consume 'while'
	EXPECT(LeftParen);
	auto condition = PARSE_ATTEMPT(parse_expr());
	EXPECT(RightParen);
	auto block = PARSE_ATTEMPT(parse_block(ctx));

	return std::make_unique<WhileStmt>(
		stream.span_from(start),
		std::move(condition),
		std::move(block)
	);
}


Parser::ParseResult<StmtPtr> Parser::parse_loop_stmt() {
	constexpr auto ctx = Context::LoopStmt;
	auto start = stream.start();

	ASSERT(stream.check(TokenKind::Loop), "Expected 'loop' token");
	stream.advance(); // consume 'loop'
	auto block = PARSE_ATTEMPT(parse_block(ctx));

	return std::make_unique<LoopStmt>(
		stream.span_from(start),
		std::move(block)
	);
}

Parser::ParseResult<StmtPtr> Parser::parse_return_stmt() {
	constexpr auto ctx = Context::ReturnStmt;
	auto start = stream.start();

	ASSERT(stream.check(TokenKind::Return), "Expected 'return' token");
	stream.advance(); // consume 'return'

	if (stream.match(TokenKind::Semicolon))
		return std::make_unique<ReturnStmt>(stream.span_from(start));

	auto expr = PARSE_ATTEMPT(parse_expr());
	EXPECT(Semicolon);

	return std::make_unique<ReturnStmt>(stream.span_from(start), std::move(expr));
}

Parser::ParseResult<StmtPtr> Parser::parse_break_stmt() {
	constexpr auto ctx = Context::BreakStmt;
	auto start = stream.start();

	ASSERT(stream.check(TokenKind::Break), "Expected 'break' token");
	stream.advance(); // consume 'break'
	EXPECT(Semicolon);

	return std::make_unique<BreakStmt>(stream.span_from(start));
}

Parser::ParseResult<StmtPtr> Parser::parse_continue_stmt() {
	constexpr auto ctx = Context::ContinueStmt;
	auto start = stream.start();

	ASSERT(stream.check(TokenKind::Continue), "Expected 'continue' token");
	stream.advance(); // consume 'continue'
	EXPECT(Semicolon);

	return std::make_unique<ContinueStmt>(stream.span_from(start));
}

static Option<AssignOp> to_assign_op(TokenKind kind) noexcept {
	switch (kind) {
		case TokenKind::Equal: return AssignOp::Assign;
		case TokenKind::PlusEqual: return AssignOp::AddAssign;
		case TokenKind::MinusEqual: return AssignOp::SubAssign;
		case TokenKind::StarEqual: return AssignOp::MulAssign;
		case TokenKind::SlashEqual: return AssignOp::DivAssign;
		case TokenKind::PercentEqual: return AssignOp::ModAssign;
		case TokenKind::AmpEqual: return AssignOp::BitAndAssign;
		case TokenKind::BarEqual: return AssignOp::BitOrAssign;
		case TokenKind::CaretEqual: return AssignOp::BitXorAssign;
		case TokenKind::LessLessEqual: return AssignOp::ShlAssign;
		case TokenKind::GreaterGreaterEqual: return AssignOp::ShrAssign;
		default: return std::nullopt;
	}
}

Parser::ParseResult<StmtPtr> Parser::parse_expr_stmt() {
	constexpr auto ctx = Context::ExprStmt;
	auto start = stream.start();
	
	auto expr = PARSE_ATTEMPT(parse_expr());

	if (auto assign_op = to_assign_op(stream.peek().kind); assign_op.has_value()) {
		constexpr auto ctx = Context::AssignStmt;
		stream.advance(); // consume assignment operator
		auto rhs = PARSE_ATTEMPT(parse_expr());
		EXPECT(Semicolon);
		return std::make_unique<AssignStmt>(
			stream.span_from(start),
			std::move(expr),
			*assign_op,
			std::move(rhs)
		);
	}

	EXPECT(Semicolon);

	return std::make_unique<ExprStmt>(
		stream.span_from(start),
		std::move(expr)
	);	
}
