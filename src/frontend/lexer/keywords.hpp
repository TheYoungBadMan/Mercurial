// frontend/lexer/keywords.hpp

#pragma once

#include <array>
#include <algorithm>

#include "frontend/token/token_kind.hpp"

struct DictEntry {
    TokenKind kind;
    StringView lexeme;
};

constexpr auto keywords = [] noexcept {
    auto arr = std::to_array<DictEntry>({
		// Control flow
		{TokenKind::If,        "if"},
        {TokenKind::Else,      "else"},
        {TokenKind::While,     "while"},
        {TokenKind::Loop,      "loop"},
        {TokenKind::Return,    "return"},
        {TokenKind::Break,     "break"},
        {TokenKind::Continue,  "continue"},
		// Declarations
        {TokenKind::Var,       "var"},
        {TokenKind::Fn,        "fn"},
        {TokenKind::Record,    "record"},
        {TokenKind::Namespace, "namespace"},
        {TokenKind::Alias,     "alias"},
        {TokenKind::Import,    "import"},
		// Literals
		{TokenKind::Boolean,      "true"},
		{TokenKind::Boolean,     "false"},
        {TokenKind::Pass,      "pass"},
		{TokenKind::Todo,      "todo"},
        {TokenKind::Default,   "default"},
		// Operators
		{TokenKind::AmpAmp,       "and"},
		{TokenKind::BarBar,        "or"},
		{TokenKind::Bang,       "not"},
		// Special
        {TokenKind::Uninit,    "uninit"},
    });

    std::sort(arr.begin(), arr.end(),
        [](const DictEntry& a, const DictEntry& b) noexcept {
            return a.lexeme < b.lexeme;
        });

    return arr;
} ();

[[nodiscard]] inline TokenKind lookup_keyword(StringView lexeme) noexcept {
    auto it = std::lower_bound(keywords.begin(), keywords.end(), lexeme,
        [](const DictEntry& e, StringView s) noexcept {
            return e.lexeme < s;
        });

    if (it != keywords.end() && it->lexeme == lexeme)
		return it->kind;

    return TokenKind::Identifier;
}