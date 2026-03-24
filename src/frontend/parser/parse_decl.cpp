// frontend/parser/parse_decl.cpp

#include "parser.hpp"

#include "core/contract.hpp"

#include "frontend/ast/decl.hpp"

Parser::ParseResult<DeclPtr> Parser::parse_decl() {
	constexpr auto ctx = Context::Decl;

	if (stream.check(Var))
		return parse_var_decl();
	else if (stream.check(Alias))
		return parse_alias_decl();
	else if (stream.check(Fn))
		return parse_fn_decl();
	else if (stream.check(Record))
		return parse_record_decl();
	else if (stream.check(Namespace))
		return parse_namespace_decl();
	else if (stream.check(Import))
		return parse_import_decl();
	else
		UNEXPECTED(Expected::Declaration);
}

Parser::ParseResult<DeclPtr> Parser::parse_var_decl() {
	constexpr auto ctx = Context::VarDecl;
	auto start = stream.start();

	ASSERT(stream.check(Var), "Expected 'var' token");
	stream.advance(); // consume 'var'
	
	BindingList bindings;
	do {
		auto binding = PARSE_ATTEMPT(parse_binding());
		bindings.push_back(std::move(binding));
	} while (stream.match(Comma));
	EXPECT(Semicolon);

	return std::make_unique<VarDecl>(
		stream.span_from(start),
		std::move(bindings)
	);
}

Parser::ParseResult<DeclPtr> Parser::parse_alias_decl() {
	constexpr auto ctx = Context::AliasDecl;
	auto start = stream.start();

	ASSERT(stream.check(Alias), "Expected 'alias' token");
	stream.advance(); // consume 'alias'

	auto parse_alias_binding = [this]() -> ParseResult<AliasBinding> {
		auto start = stream.start();

		auto name = CONSUME(Identifier);
		EXPECT(Equal);
		auto type = PARSE_ATTEMPT(parse_type());

		return AliasBinding{
			.span = stream.span_from(start),
			.name = name.span,
			.type = std::move(type)
		};
	};

	AliasBindingList bindings;
	do {
		auto binding = PARSE_ATTEMPT(parse_alias_binding());
		bindings.push_back(std::move(binding));
	} while (stream.match(Comma));
	EXPECT(Semicolon);


	return std::make_unique<AliasDecl>(
		stream.span_from(start),
		std::move(bindings)
	);
}

Parser::ParseResult<DeclPtr> Parser::parse_import_decl() {
	constexpr auto ctx = Context::ImportDecl;
	auto start = stream.start();

	ASSERT(stream.check(TokenKind::Import), "Expected 'import' token");
	stream.advance(); // consume 'import'
	
	auto parse_import_item = [this]() -> ParseResult<ImportItem> {
		auto start = stream.start();
		auto name = CONSUME(Identifier);
		return ImportItem{name.span};
	};

	ImportItemList items;
	do {
		auto item = PARSE_ATTEMPT(parse_import_item());
		items.push_back(std::move(item));
	} while (stream.match(Comma));
	EXPECT(Semicolon);

	return std::make_unique<ImportDecl>(
		stream.span_from(start),
		std::move(items)
	);
}

Parser::ParseResult<DeclPtr> Parser::parse_fn_decl() {
	constexpr auto ctx = Context::FnDecl;
	auto start = stream.start();

	ASSERT(stream.check(Fn), "Expected 'fn' token");
	stream.advance(); // consume 'fn'
	auto name = CONSUME(Identifier);

	EXPECT(LeftParen);
	BindingList params;
	if (!stream.match(RightParen)) {
		do {
			auto param = PARSE_ATTEMPT(parse_binding());
			params.push_back(std::move(param));
		} while (stream.match(Comma) && !stream.check(RightParen));
		EXPECT(RightParen);
	}

	TypePtr return_type = nullptr;
	if (stream.match(Arrow))
		return_type = PARSE_ATTEMPT(parse_type());

	auto body = PARSE_ATTEMPT(parse_block(ctx));
	
	return std::make_unique<FnDecl>(
		stream.span_from(start),
		name.span,
		std::move(params),
		std::move(return_type),
		std::move(body)
	);
}

Parser::ParseResult<DeclPtr> Parser::parse_record_decl() {
	constexpr auto ctx = Context::RecordDecl;
	auto start = stream.start();
	
	ASSERT(stream.check(Record), "Expected 'record' token");
	stream.advance(); // consume 'record'
	auto name = CONSUME(Identifier);
	
	EXPECT(LeftBrace);
	BindingList fields;
	if (!stream.match(RightBrace)) {
		do {
			auto field = PARSE_ATTEMPT(parse_binding());
			fields.push_back(std::move(field));
		} while (stream.match(Comma) && !stream.check(RightBrace));
		EXPECT(RightBrace);
	}

	return std::make_unique<RecordDecl>(
		stream.span_from(start),
		name.span,
		std::move(fields)
	);
}

Parser::ParseResult<DeclPtr> Parser::parse_namespace_decl() {
	constexpr auto ctx = Context::NamespaceDecl;
	auto start = stream.start();

	ASSERT(stream.check(Namespace), "Expected 'namespace' token");
	stream.advance(); // consume 'namespace'
	auto name = CONSUME(Identifier);
	
	EXPECT(LeftBrace);
	DeclList decls;
	while (!stream.check(RightBrace) && !stream.eof()) {
		auto decl = PARSE_ATTEMPT(parse_decl());
		decls.push_back(std::move(decl));
	}
	EXPECT(RightBrace);

	return std::make_unique<NamespaceDecl>(
		stream.span_from(start),
		name.span,
		std::move(decls)
	);
}