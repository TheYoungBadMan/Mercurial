# Mercurial Roadmap

## v0.1

### Language

#### Types

* Primitive: `iN`, `uN`, `fN`, `bN`, `bool`, `char`, `string`, `unit`, `never`
* `record`, tuple, fixed array
* `fn`

#### Statements

* `if`, `while`, `loop`
* `break`, `continue`, `return`
* Expression statement, assignment, declaration

#### Declarations

* `var`, `alias`, `namespace`, `record`, `fn`

#### Expressions

* Binary: arithmetic, comparison, logical, bitwise
* Unary: logical, bitwise, arithmetic
* Postfix: call, subscript, member access
* Primary: identifier, literal, array, record, tuple, lambda, grouping

## v0.2

### Language

#### Types

* Variant types:
  * `variant` / `variant class`
  * `enum` / `enum class`
  * Anonymous variant `(T | T | ...)`
* `visit` for anonymous variant
* `impls` for records and variants
* Numeric conversions: widening, narrowing, cross-family, reinterpret

#### Expressions

* Block expression `{ ... }`
* `cast`, `convert`, `reinterpret`

#### Control Flow

* `match`:
  * Patterns: literal, negative literal, constructor, binding, wildcard, or-pattern
  * Guards
  * Nested match
* `_` discard

#### Modules

* Import: `import math.sin;` / `import math.{ sin, cos };`
* Visibility (`public` / `private`)

## v0.3

### Language

#### Types

* Generics (full — constraints, bounds)
* Traits
* `result[T, E]` / `option[T]` methods (`map`, `unwrap`, `?` operator)
* References (`&T`, `&mut T`)

#### Expressions

* Statement to expression refactoring (`if` / `while` / `loop` as expressions)

#### Control Flow

* Labels (`break :label`, `continue :label`)

### Standard Library

#### IO

* `print` / `read` (replaces builtins)