// frontend/parser/parse_pattern.cpp

#include "parser.hpp"

#include "frontend/ast/pattern.hpp"

Parser::ParseResult<PatternPtr> Parser::parse_pattern() {
	constexpr auto ctx = Context::Pattern;

	if (stream.check(TokenKind::LeftParen))
		return parse_paren_pattern();
	else if (stream.check(TokenKind::Identifier))
		return parse_named_pattern();
	else
		UNEXPECTED(Expected::Pattern);
}

Parser::ParseResult<PatternPtr> Parser::parse_paren_pattern() {
	constexpr auto ctx = Context::ParenPattern;
	auto start = stream.start();

	ASSERT(stream.check(LeftParen), "Expected '(' token");
	stream.advance(); // consume '('

	if (stream.match(RightParen))
		return std::make_unique<ParenPattern>(
			stream.span_from(start)
		);

	auto pattern = PARSE_ATTEMPT(parse_pattern());

	if (stream.match(RightParen))
		return std::make_unique<ParenPattern>(
			stream.span_from(start),
			std::move(pattern)
		);

	PatternList patterns{std::move(pattern)};

	if (!stream.match(RightParen)) {
		do {
			auto pattern = PARSE_ATTEMPT(parse_pattern());
			patterns.push_back(std::move(pattern));
		} while (stream.match(Comma) && !stream.check(RightParen));
		EXPECT(RightParen);
	}

	return std::make_unique<ListPattern>(
		stream.span_from(start),
		std::move(patterns)
	);
}

Parser::ParseResult<PatternPtr> Parser::parse_named_pattern() {
	auto name = stream.consume(); // consume identifier
	return std::make_unique<NamedPattern>(name.span);
}