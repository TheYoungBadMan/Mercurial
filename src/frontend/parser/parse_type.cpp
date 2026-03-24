// frontend/parser/parse_type.cpp

#include "parser.hpp"

#include "frontend/ast/type.hpp"

Parser::ParseResult<TypePtr> Parser::parse_type() {
	return parse_postfix_type();
}

Parser::ParseResult<TypePtr> Parser::parse_postfix_type() {
	auto type = PARSE_ATTEMPT(parse_primary_type());

	for (auto start = type->span.start; ;) {
		if (stream.match(LeftBracket)) {
			constexpr auto ctx = Context::ArrayType;
			auto size = PARSE_ATTEMPT(parse_expr());
			EXPECT(RightBracket);
			type = std::make_unique<ArrayType>(
				stream.span_from(start),
				std::move(type),
				std::move(size)
			);
		} else
			break;
	}

	return type;
}

Parser::ParseResult<TypePtr> Parser::parse_primary_type() {
	constexpr auto ctx = Context::Type;

	if (stream.check(TokenKind::LeftParen))
		return parse_paren_type();
	else if (stream.check(TokenKind::Fn))
		return parse_function_type();
	else if (stream.check(TokenKind::Identifier))
		return parse_named_type();
	else
		UNEXPECTED(Expected::Type);
}

Parser::ParseResult<TypePtr> Parser::parse_paren_type() {
	constexpr auto ctx = Context::ParenType;
	auto start = stream.start();

	ASSERT(stream.check(LeftParen), "Expected '(' token");
	stream.advance(); // consume '('
	auto type = PARSE_ATTEMPT(parse_type());

	if (stream.match(RightParen))
		return std::make_unique<ParenType>(
			stream.span_from(start),
			std::move(type)
		);
	
	EXPECT(Comma);

	TypeList element_types{std::move(type)};

	do {
		auto type = PARSE_ATTEMPT(parse_type());
		element_types.push_back(std::move(type));
	} while (stream.match(Comma) && !stream.check(RightParen));

	EXPECT(RightParen);

	return std::make_unique<TupleType>(
		stream.span_from(start),
		std::move(element_types)
	);
}

Parser::ParseResult<TypePtr> Parser::parse_function_type() {
	constexpr auto ctx = Context::FunctionType;
	auto start = stream.start();

	ASSERT(stream.check(Fn), "Expected 'fn' token");
	stream.advance(); // consume 'fn'
	EXPECT(LeftParen);

	TypeList param_types;
	if (!stream.check(RightParen)) {
		do {
			auto type = PARSE_ATTEMPT(parse_type());
			param_types.push_back(std::move(type));
		} while (stream.match(Comma) && !stream.check(RightParen));
	}

	EXPECT(RightParen);

	TypePtr return_type = nullptr;
	if (stream.match(Arrow))
		return_type = PARSE_ATTEMPT(parse_type());

	return std::make_unique<FunctionType>(
		stream.span_from(start),
		std::move(param_types),
		std::move(return_type)
	);
}

Parser::ParseResult<TypePtr> Parser::parse_named_type() {
	ASSERT(stream.check(Identifier), "Expected identifier token");
	auto name = stream.consume(); // consume identifier
	return std::make_unique<NamedType>(name.span);
}