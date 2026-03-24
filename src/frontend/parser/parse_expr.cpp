// frontend/parser/parse_expr.cpp

#include "parser.hpp"

#include "frontend/ast/expr.hpp"
	
Parser::ParseResult<ExprPtr> Parser::parse_expr() {
	return parse_binary_expr();
}

struct BinaryOpInfo {
	BinaryOp op;
	Int precedence;
};

static Option<BinaryOpInfo> to_binary_op_info(TokenKind kind) noexcept {
	switch (kind) {
		case TokenKind::BarBar:
			return BinaryOpInfo{BinaryOp::Or, 1}; // ||
		case TokenKind::AmpAmp:
			return BinaryOpInfo{BinaryOp::And, 2}; // &&
		case TokenKind::EqualEqual:
			return BinaryOpInfo{BinaryOp::Eq, 3}; // ==
		case TokenKind::BangEqual:
			return BinaryOpInfo{BinaryOp::Neq, 3}; // !=
		case TokenKind::Less:
			return BinaryOpInfo{BinaryOp::Lt, 4}; // <
		case TokenKind::LessEqual:
			return BinaryOpInfo{BinaryOp::Leq, 4}; // <=
		case TokenKind::Greater:
			return BinaryOpInfo{BinaryOp::Gt, 4}; // >
		case TokenKind::GreaterEqual:
			return BinaryOpInfo{BinaryOp::Geq, 4}; // >=
		case TokenKind::Bar:
			return BinaryOpInfo{BinaryOp::BitOr, 5}; // |
		case TokenKind::Caret:
			return BinaryOpInfo{BinaryOp::BitXor, 6}; // ^
		case TokenKind::Amp:
			return BinaryOpInfo{BinaryOp::BitAnd, 7}; // &
		case TokenKind::LessLess:
			return BinaryOpInfo{BinaryOp::Shl, 8}; // <<
		case TokenKind::GreaterGreater:
			return BinaryOpInfo{BinaryOp::Shr, 8}; // >>
		case TokenKind::Plus:
			return BinaryOpInfo{BinaryOp::Add, 9}; // +
		case TokenKind::Minus:
			return BinaryOpInfo{BinaryOp::Sub, 9}; // -
		case TokenKind::Star:
			return BinaryOpInfo{BinaryOp::Mul, 10}; // *
		case TokenKind::Slash:
			return BinaryOpInfo{BinaryOp::Div, 10}; // /
		case TokenKind::Percent:
			return BinaryOpInfo{BinaryOp::Mod, 10}; // %
		default:
			return std::nullopt; // not a binary operator
	}
}

Parser::ParseResult<ExprPtr> Parser::parse_binary_expr(Int min_precedence) {
	auto start = stream.start();

	auto left = PARSE_ATTEMPT(parse_unary_expr());

	while (auto op_info = to_binary_op_info(stream.peek().kind)) {
		if (op_info->precedence < min_precedence)
			break;

		stream.advance(); // consume operator

		auto right = PARSE_ATTEMPT(parse_binary_expr(op_info->precedence + 1));

		left = std::make_unique<BinaryExpr>(
			stream.span_from(start),
			std::move(left),
			op_info->op,
			std::move(right)
		);
	}

	return left;
}

static Option<UnaryOp> to_unary_op(TokenKind kind) noexcept {
	switch (kind) {
		case TokenKind::Plus: return UnaryOp::Pos; // +
		case TokenKind::Minus: return UnaryOp::Neg; // -
		case TokenKind::Bang: return UnaryOp::Not; // !
		case TokenKind::Tilde: return UnaryOp::BitNot; // ~
		default: return std::nullopt; // not a unary operator
	}
}

Parser::ParseResult<ExprPtr> Parser::parse_unary_expr() {
	if (auto op = to_unary_op(stream.peek().kind); op.has_value()) {
		auto start = stream.start();
		stream.advance(); // consume operator
		auto operand = PARSE_ATTEMPT(parse_unary_expr());
		return std::make_unique<UnaryExpr>(
			stream.span_from(start),
			*op,
			std::move(operand)
		);
	}

	return parse_postfix_expr();
}

Parser::ParseResult<ExprPtr> Parser::parse_postfix_expr() {
	auto expr = PARSE_ATTEMPT(parse_primary_expr());
	for (auto start = expr->span.start;;) {
		if (stream.match(LeftParen)) {
			constexpr auto ctx = Context::CallExpr;
			ExprList args;
			if (!stream.check(RightParen)) {
				do {
					auto arg = PARSE_ATTEMPT(parse_expr());
					args.push_back(std::move(arg));
				} while (stream.match(Comma) && !stream.check(RightParen));
			}
			EXPECT(RightParen);
			expr = std::make_unique<CallExpr>(
				stream.span_from(start),
				std::move(expr),
				std::move(args)
			);
		} else if (stream.match(LeftBracket)) {
			constexpr auto ctx = Context::SubscriptExpr;
			
			auto index = PARSE_ATTEMPT(parse_expr());
			EXPECT(RightBracket);
			expr = std::make_unique<SubscriptExpr>(
				stream.span_from(start),
				std::move(expr),
				std::move(index)
			);

		} else if (stream.match(Dot)) {
			constexpr auto ctx = Context::MemberExpr;

			if (stream.check(Identifier) || stream.check(Integer)) {
				auto is_index = stream.check(Integer);
				auto member = stream.consume(); // consume member name or index
				expr = std::make_unique<MemberExpr>(
					stream.span_from(start),
					std::move(expr),
					member.span,
					is_index
				);
			} else
				UNEXPECTED(Expected::Member);
		} else
			break;
	}

	return expr;
}

static Option<Literal> to_literal_kind(TokenKind kind) noexcept {
	switch (kind) {
		case TokenKind::Integer: return Literal::Integer;
		case TokenKind::Float: return Literal::Float;
		case TokenKind::String: return Literal::String;
		case TokenKind::Character: return Literal::Character;
		case TokenKind::Boolean: return Literal::Boolean;
		case TokenKind::Pass: return Literal::Pass;
		case TokenKind::Todo: return Literal::Todo;
		case TokenKind::Default: return Literal::Default;
		default: return std::nullopt; // not a literal
	}
}

Parser::ParseResult<ExprPtr> Parser::parse_primary_expr() {
	constexpr auto ctx = Context::Expr;

	if (stream.check(TokenKind::LeftParen))
		return parse_paren_expr();
	if (stream.check(TokenKind::LeftBracket))
		return parse_array_expr();
	if (stream.check(TokenKind::Fn))
		return parse_lambda_expr();
	if (stream.check(TokenKind::Identifier))
		return parse_ident_expr();

	if (auto literal_kind = to_literal_kind(stream.peek().kind); literal_kind.has_value()) {
		auto literal = stream.consume(); // consume literal
		return std::make_unique<LitExpr>(
			literal.span,
			*literal_kind
		);
	}

	UNEXPECTED(Expected::Expression);
}

Parser::ParseResult<ExprPtr> Parser::parse_paren_expr() {
	constexpr auto ctx = Context::ParenExpr;
	auto start = stream.start();

	ASSERT(stream.check(LeftParen), "Expected '(' token");
	stream.advance(); // consume '('

	if (stream.match(RightParen))
		return std::make_unique<LitExpr>(
			stream.span_from(start),
			Literal::Unit
		); // unit literal

	auto first_expr = PARSE_ATTEMPT(parse_expr());

	if (stream.match(RightParen))
		return std::make_unique<ParenExpr>(
			stream.span_from(start),
			std::move(first_expr)
		); // parenthesized expression

	EXPECT(Comma);

	ExprList elements = {std::move(first_expr)};
	do {
		auto element = PARSE_ATTEMPT(parse_expr());
		elements.push_back(std::move(element));
	} while (stream.match(Comma) && !stream.check(RightParen));

	EXPECT(RightParen);

	return std::make_unique<TupleExpr>(
		stream.span_from(start),
		std::move(elements)
	); // tuple literal
}

Parser::ParseResult<ExprPtr> Parser::parse_array_expr() {
	constexpr auto ctx = Context::ArrayExpr;
	auto start = stream.start();

	ASSERT(stream.check(TokenKind::LeftBracket), "Expected '[' token");
	stream.advance(); // consume '['
	
	ExprList elements;
	if (!stream.check(RightBracket)) {
		do {
			auto element = PARSE_ATTEMPT(parse_expr());
			elements.push_back(std::move(element));
		} while (stream.match(Comma) && !stream.check(RightBracket));
	}

	EXPECT(RightBracket);

	return std::make_unique<ArrayExpr>(
		stream.span_from(start),
		std::move(elements)
	);
}

Parser::ParseResult<ExprPtr> Parser::parse_ident_expr() {
	auto start = stream.start();

	ASSERT(stream.check(Identifier), "Expected identifier token");
	auto name = stream.consume(); // consume identifier

	if (!stream.match(LeftBrace))
		return std::make_unique<IdentExpr>(name.span); // simple identifier

	constexpr auto ctx = Context::RecordExpr;

	auto parse_field_init = [this]() -> ParseResult<FieldInit> {
			auto start = stream.start();
			auto field_name = CONSUME(Identifier);
			EXPECT(Equal);
			auto field_value = PARSE_ATTEMPT(parse_expr());
			return FieldInit{
				.name = field_name.span,
				.value = std::move(field_value),
			};
	};

	Vector<FieldInit> field_inits;
	if (!stream.match(RightBrace)) {
		do {
			auto field_init = PARSE_ATTEMPT(parse_field_init());
			field_inits.push_back(std::move(field_init));
		} while (stream.match(Comma) && !stream.check(RightBrace));
		EXPECT(RightBrace);
	}

	return std::make_unique<RecordExpr>(
		stream.span_from(start),
		name.span,
		std::move(field_inits)
	); // record literal
}

Parser::ParseResult<ExprPtr> Parser::parse_lambda_expr() {
	constexpr auto ctx = Context::LambdaExpr;
	auto start = stream.start();

	ASSERT(stream.check(Fn), "Expected 'fn' token");
	stream.advance(); // consume 'fn'
	EXPECT(LeftParen);

	BindingList params;
	if (!stream.check(RightParen)) {
		do {
			auto param = PARSE_ATTEMPT(parse_binding());
			params.push_back(std::move(param));
		} while (stream.match(Comma) && !stream.check(RightParen));
	}

	EXPECT(RightParen);

	TypePtr return_type = nullptr;
	if (stream.match(Arrow))
		return_type = PARSE_ATTEMPT(parse_type());

	auto body = PARSE_ATTEMPT(parse_block(ctx));

	return std::make_unique<LambdaExpr>(
		stream.span_from(start),
		std::move(params),
		std::move(return_type),
		std::move(body)
	);
}