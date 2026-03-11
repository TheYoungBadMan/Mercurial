// frontend/lexer/keywords.hpp

#pragma once

#include "core/types.hpp"

#include <array>
#include <algorithm>

#include "frontend/token/token_kind.hpp"

struct DictEntry {
    TokenKind kind;
    StringView lexeme;
};

constexpr auto keywords = [] {
    auto arr = std::to_array<DictEntry>({
		{TokenKind::If,        "if"},
        {TokenKind::Else,      "else"},
        {TokenKind::Match,     "match"},
        {TokenKind::While,     "while"},
        {TokenKind::Loop,      "loop"},
        {TokenKind::Return,    "return"},
        {TokenKind::Leave,     "leave"},
        {TokenKind::Break,     "break"},
        {TokenKind::Continue,  "continue"},
        {TokenKind::Pass,      "pass"},
		{TokenKind::Todo,      "todo"},
        {TokenKind::Default,   "default"},
        {TokenKind::Uninit,    "uninit"},
        {TokenKind::Discard,   "discard"},
        {TokenKind::Var,       "var"},
        {TokenKind::Fn,        "fn"},
        {TokenKind::Record,    "record"},
        {TokenKind::Variant,   "variant"},
        {TokenKind::Enum,      "enum"},
        {TokenKind::Namespace, "namespace"},
        {TokenKind::Alias,     "alias"},
        {TokenKind::Import,    "import"},
        {TokenKind::Public,    "public"},
        {TokenKind::Private,   "private"},
		// Literals
		{TokenKind::Boolean,      "true"},
		{TokenKind::Boolean,     "false"},
		// Operators
		{TokenKind::AmpAmp,       "and"},
		{TokenKind::BarBar,        "or"},
		{TokenKind::Bang,       "not"},
    });

    std::sort(arr.begin(), arr.end(),
        [](const DictEntry& a, const DictEntry& b) {
            return a.lexeme < b.lexeme;
        });

    return arr;
} ();

[[nodiscard]] inline TokenKind lookup_keyword(StringView lexeme) noexcept {
    auto it = std::lower_bound(keywords.begin(), keywords.end(), lexeme,
        [](const DictEntry& e, StringView s) {
            return e.lexeme < s;
        });

    if (it != keywords.end() && it->lexeme == lexeme)
		return it->kind;

    return TokenKind::Identifier;
}