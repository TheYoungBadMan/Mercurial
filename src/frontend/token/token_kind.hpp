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
	If, Else, While, Loop, Return, Break, Continue,
	Pass, Todo, Default, Uninit,
	Var, Fn, Record, Alias, Namespace, Import,

	// Operators
	Plus, Minus, Star, Slash, Percent, // +, -, *, /, %
	Equal, Less, Greater, // =, <, >
	Amp, Bar, Caret, Tilde, Bang, // &, |, ^, ~, !

	EqualEqual, BangEqual, LessEqual, GreaterEqual, // ==, !=, <=, >=
	LessLess, GreaterGreater, // <<, >>
	AmpAmp, BarBar, // &&, ||
	PlusEqual, MinusEqual, StarEqual, SlashEqual, PercentEqual, // +=, -=, *=, /=, %=
	AmpEqual, BarEqual, CaretEqual, LessLessEqual, GreaterGreaterEqual, // &=, |=, ^=, <<=, >>=

	// Delimiters
	LeftParen, RightParen, // (, )
	LeftBrace, RightBrace, // {, }
	LeftBracket, RightBracket, // [, ]
	Comma, Semicolon, Colon, Dot, // ,, ;, :, .
	Arrow, // ->

	// End of file
	Eof,
};