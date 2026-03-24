# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

Mercurial is a compiled programming language implemented in C++23. The compiler is named `merc` and currently consists of a lexer (complete) and early-stage AST/parser stubs. The language spec is written in Russian and lives in `specs/`.

## Build Commands

```bash
# Configure (Debug by default)
cmake -B build -DCMAKE_BUILD_TYPE=Debug

# Build
cmake --build build --parallel

# Run
./build/merc <source_file>

# Release build
cmake -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build --parallel

# Clean
cmake --build build --target clean
```

The binary is `build/merc`. It accepts a single source file path as argument and currently tokenizes it, printing token kinds and spans to stdout.

## Architecture

The compiler pipeline: **Source → Lexer → (Parser → AST)** — parser is stubbed out but not yet wired into `main.cpp`.

### Source Layer (`src/frontend/source/`)
- `Source` — owns the raw file buffer, provides indexed char access and string views
- `SourceManager` — maps `SourceId` (u32) to `(filename, Source)` pairs; IDs are assigned incrementally
- `Span` — `{ source_id, start: Pos, end: Pos }` — the universal source location type used throughout

### Lexer (`src/frontend/lexer/`)
- `Lexer::tokenize(source_id, source)` — static entry point, returns `LexerResult { tokens, errors }`
- `Cursor` — wraps `Source`, tracks `start`/`cur` positions; `sync()` resets `start = cur` between tokens
- Errors (`LexicalError`) are collected rather than aborting — `LexerResult::ok()` checks if empty
- `keywords.hpp` — `lookup_keyword(lexeme)` maps string views to `TokenKind`, defaults to `Identifier`
- `predicates.hpp` — character predicates conforming to `CharPredicate` concept (used with `Cursor::check/match`)

### Token Layer (`src/frontend/token/`)
- `Token { kind: TokenKind, span: Span }` — tokens store only their kind and source span, not lexeme text
- To recover lexeme text, use `SourceManager::source(id)` + `Span` offsets

### AST (`src/frontend/ast/`)
- `ASTNode` — base with `Span span`; hierarchy: `DeclNode`, `StmtNode`, `ExprNode`, `TypeNode`, `BindingNode`
- All AST nodes are heap-allocated via `UniquePtr<T>` aliases (`DeclPtr`, `ExprPtr`, etc.)
- `Module = DeclList = Vector<DeclPtr>` — top-level unit
- Declarations: `VarDecl`, `FnDecl`, `RecordDecl`, `VariantDecl` (TBD), `NamespaceDecl`, `AliasDecl`
- Statements: `IfStmt`, `WhileStmt`, `LoopStmt`, `ReturnStmt`, `LeaveStmt`, `AssignStmt`, `ExprStmt`, `DeclStmt`, `PassStmt`, `TodoStmt`, `BreakStmt`, `ContinueStmt`
- Expressions: `BinaryExpr`, `UnaryExpr`, `CallExpr`, `SubscriptExpr`, `MemberExpr`, `BlockExpr`, `ParenExpr`, `TupleExpr`, `ArrayExpr`, `LambdaExpr`, `IdentExpr`, `LitExpr`

### Parser (`src/frontend/parser/`)
- `Parser::parse(tokens)` — stub returning a `Module`; not yet called from `main.cpp`
- `parse_error.hpp` — exists but parser error types are not yet implemented

### Core Utilities (`src/core/`)
- `types.hpp` — project-wide type aliases: `u8`/`i8`/etc., `Bool`, `Char`, `String`, `StringView`, `Option<T>` (`std::optional`), `Result<T,E>` (`std::expected`), `Vector<T>`, `UniquePtr<T>`
- `contract.hpp` — assertion macros: `ASSERT` (debug-only), `REQUIRE`/`ENSURE`/`INVARIANT` (always-checked), `UNREACHABLE`

## Conventions

- C++23, all warnings enabled (`-Wall -Wextra -Wpedantic -Werror`)
- Header-only implementation (all logic in `.hpp` files); only `main.cpp` is a `.cpp` file
- `[[nodiscard]]` is used extensively — don't ignore return values
- `noexcept` is marked on functions that cannot throw
- `src/` is the include root: use `#include "frontend/lexer/lexer.hpp"` style (no `src/` prefix)
- Spans are byte offsets (not line/column); line/column mapping is not yet implemented
