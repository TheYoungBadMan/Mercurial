#pragma once

#include "frontend/token/token.hpp"

#include "frontend/ast/node.hpp"

#include "token_stream.hpp"
#include "syntax_error.hpp"

#define PARSE_ATTEMPT(expr) ({ \
		auto result = (expr); \
		if (!result) \
			return std::unexpected(result.error()); \
		std::move(result.value()); \
	})

#define UNEXPECTED(expected) \
	return std::unexpected{ \
		SyntaxError{ \
			.span = stream.peek().span, \
			.kind = expected, \
			.context = ctx \
		} \
	}

#define CHECK(cond, expected) \
	do { \
		if (!(cond)) \
			return std::unexpected{ \
				SyntaxError{ \
					.span = stream.span_from(start), \
					.kind = expected, \
					.context = ctx \
				} \
			}; \
	} while (0)

#define EXPECT(token_kind) \
	CHECK(stream.match(token_kind), token_kind)

#define CONSUME(token_kind) ({ \
		EXPECT(token_kind); \
		stream.prev(); \
	})

struct ParserResult {
	// CURRENTLY UNUSED
};

class Parser {
public:

	template <typename T>
	using ParseResult = Result<T, SyntaxError>;

	[[nodiscard]] static ParseResult<Module> parse(const Vector<Token>& tokens) {
		return Parser{tokens}.parse_module();
	}

private:

	// -------------------- FIELDS -------------------- //

	TokenStream stream;

	explicit Parser(const Vector<Token>& tokens) noexcept
		: stream{tokens} {}

	// -------------------- RESULT --------------------- //

	// -------------------- PARSING FUNCTIONS -------------------- //

	using enum TokenKind;

	ParseResult<Module> parse_module() {
		Vector<DeclPtr> decls;

		while (!stream.eof()) {
			auto decl = PARSE_ATTEMPT(parse_decl());
			decls.push_back(std::move(decl));
		}

		return decls;
	}

	ParseResult<BlockPtr> parse_block(Context ctx) {
		auto start = stream.start();

		EXPECT(LeftBrace);

		Vector<BlockItem> items;
		while (!stream.check(RightBrace) && !stream.eof()) {
			if (stream.check(Var)) {
				auto decl = PARSE_ATTEMPT(parse_var_decl());
				items.push_back(std::move(decl));
			} else if (stream.check(Alias)) {
				auto decl = PARSE_ATTEMPT(parse_alias_decl());
				items.push_back(std::move(decl));
			} else {
				auto stmt = PARSE_ATTEMPT(parse_stmt());
				items.push_back(std::move(stmt));
			}
		}

		EXPECT(RightBrace);

		return std::make_unique<Block>(stream.span_from(start), std::move(items));
	}

	ParseResult<Binding> parse_binding() {
		constexpr auto ctx = Context::Binding;
		auto start = stream.start();

		auto name = CONSUME(Identifier);
		EXPECT(Colon);
		auto type = PARSE_ATTEMPT(parse_type());

		ExprPtr init = nullptr;
		if (stream.match(Equal))
			init = PARSE_ATTEMPT(parse_expr());

		return Binding {
			.span = stream.span_from(start),
			.name = name.span,
			.type = std::move(type),
			.init = std::move(init)
		};
	}

	// -------------------- Node parsing --------------------- //

	ParseResult<TypePtr> parse_type();
	ParseResult<TypePtr> parse_postfix_type();
	ParseResult<TypePtr> parse_primary_type();
	ParseResult<TypePtr> parse_paren_type();
	ParseResult<TypePtr> parse_function_type();
	ParseResult<TypePtr> parse_named_type();

	ParseResult<DeclPtr> parse_decl();
	ParseResult<DeclPtr> parse_var_decl();
	ParseResult<DeclPtr> parse_alias_decl();
	ParseResult<DeclPtr> parse_import_decl();
	ParseResult<DeclPtr> parse_fn_decl();
	ParseResult<DeclPtr> parse_record_decl();
	ParseResult<DeclPtr> parse_namespace_decl();

	ParseResult<StmtPtr> parse_stmt();
	ParseResult<StmtPtr> parse_if_stmt();
	ParseResult<StmtPtr> parse_while_stmt();
	ParseResult<StmtPtr> parse_loop_stmt();
	ParseResult<StmtPtr> parse_break_stmt();
	ParseResult<StmtPtr> parse_continue_stmt();
	ParseResult<StmtPtr> parse_return_stmt();
	ParseResult<StmtPtr> parse_expr_stmt();

	ParseResult<ExprPtr> parse_expr();
	ParseResult<ExprPtr> parse_binary_expr(Int min_precedence = 0);
	ParseResult<ExprPtr> parse_unary_expr();
	ParseResult<ExprPtr> parse_postfix_expr();
	ParseResult<ExprPtr> parse_primary_expr();
	ParseResult<ExprPtr> parse_paren_expr();
	ParseResult<ExprPtr> parse_array_expr();
	ParseResult<ExprPtr> parse_lambda_expr();
	ParseResult<ExprPtr> parse_ident_expr();
};