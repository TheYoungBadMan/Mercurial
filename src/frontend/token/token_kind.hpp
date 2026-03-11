// frontend/token/token_kind.hpp

#pragma once

#include "core/types.hpp"

enum class TokenKind : u8 {
	// General
	Identifier,

	// Literals
	Integer, Float,
	Boolean, Character, String,

	// Keywords
	Pass, Todo,
	If, Else, Match, While, Loop, Return, Leave, Break, Continue,
	Default, Uninit, Discard,
	Var, Fn, Record, Variant, Enum, Alias, Namespace, Import,
	Public, Private,

	// Operators
	Plus, Minus, Star, Slash, Percent,
	Equal, Less, Greater,
	Amp, Bar, Caret, Tilde, Bang,
	EqualEqual, BangEqual, LessEqual, GreaterEqual,
	AmpAmp, BarBar,
	LessLess, GreaterGreater,
	PlusEqual, MinusEqual, StarEqual, SlashEqual, PercentEqual,
	AmpEqual, BarEqual, CaretEqual, LessLessEqual, GreaterGreaterEqual,

	// Delimiters
	LeftParen, RightParen,
	LeftBrace, RightBrace,
	LeftBracket, RightBracket,
	Comma, Semicolon, Colon, Dot, Arrow, FatArrow,

	// End of file
	Eof,
};