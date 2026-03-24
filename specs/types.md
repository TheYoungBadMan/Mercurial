# Mercurial type system v0.1

## Introduction

Mercurial's type system has the following properties:
* **Static typing** — all types are resolved and checked at compile time
* **Strong typing** — no implicit conversions between incompatible types
* **Nominal typing** — type compatibility is determined by name, not by structure
* **Value semantics** — all types behave as values (copied on assignment)

## Primitive types

Mercurial has the following primitive types:
* Integer types: `i8`, `i16`, `i32`, `i64`, `i128`
* Unsigned integer types: `u8`, `u16`, `u32`, `u64`, `u128`
* Floating point types: `f32`, `f64`, `f128`
* Byte types: `b8`, `b16`, `b32`, `b64`, `b128`
* Boolean: `bool`
* Character: `char`
* String: `string`
* Unit: `unit`
* Never: `never`

### Integer types

* Variants: `i8`, `i16`, `i32`, `i64`, `i128`
* Alias: `int` = `i64`
* Size: 8, 16, 32, 64, 128 bits
* Range: -2^(N-1) to 2^(N-1)-1 for `iN`
* Default: `0`
* Literals:
  * Decimal (`42`)
  * Hexadecimal (`0x2A`)
  * Octal (`0o52`)
  * Binary (`0b101010`)
* Operations:
  * Arithmetic: `+`, `-`, `*`, `/`, `%`
  * Bitwise: `&`, `|`, `^`, `~`, `<<`, `>>`
  * Comparison: `==`, `!=`, `<`, `>`, `<=`, `>=`
* Overflow: runtime panic by default
* Conversions:
  * Widening (within family): implicit
  * Narrowing (within family): explicit cast required
  * Compatibility (cross-family): `u*`, `f*`, `b*`, `bool` (explicit cast required) 
* Notes:
  * `i128`: native if platform supports, otherwise soft-emulated

### Unsigned integer types

* Variants: `u8`, `u16`, `u32`, `u64`, `u128`
* Alias: `uint` = `u64`
* Size: 8, 16, 32, 64, 128 bits
* Range: 0 to 2^N-1 for `uN`
* Default: `0`
* Literals: 
  * Decimal (`42`)
  * Hexadecimal (`0x2A`)
  * Octal (`0o52`)
  * Binary (`0b101010`)
* Operations:
  * Arithmetic: `+`, `-`, `*`, `/`, `%`
  * Bitwise: `&`, `|`, `^`, `~`, `<<`, `>>`
  * Comparison: `==`, `!=`, `<`, `>`, `<=`, `>=`
* Overflow: runtime panic by default
* Conversions:
  * Widening (within family): implicit
  * Narrowing (within family): explicit cast required
  * Compatibility (cross-family): `i*`, `f*`, `b*`, `bool` (explicit cast required)
* Notes:
  * `u128`: native if platform supports, otherwise soft-emulated

### Floating point types

* Variants: `f32`, `f64`, `f128`
* Alias: `float` = `f64`
* Size: 32, 64, 128 bits
* Range: IEEE 754 based
* Special values: `NaN`, `Inf`, `-Inf`
* Default: `0.0`
* Literals: 
  * Decimal (`3.14`)
  * Exponential (`1e-9`)
* Operations:
  * Arithmetic: `+`, `-`, `*`, `/`
  * Comparison: `==`, `!=`, `<`, `>`, `<=`, `>=`
* Overflow: `Inf` or `-Inf`
* Conversions:
  * Widening (within family): implicit
  * Narrowing (within family): explicit cast required
  * Compatibility (cross-family): `i*`, `u*`, `b*`, `bool` (explicit cast required)
* Notes:
  * `f128`: native if platform supports, otherwise soft-emulated

### Byte types

* Variants: `b8`, `b16`, `b32`, `b64`, `b128`
* Alias: `byte` = `b8`
* Size: 8, 16, 32, 64, 128 bits
* Range: 0 to 2^N-1 for `bN`
* Default: `0`
* Literals: 
  * Decimal (`42`)
  * Hexadecimal (`0x2A`)
  * Octal (`0o52`)
  * Binary (`0b101010`)
* Operations:
  * Bitwise: `&`, `|`, `^`, `~`
  * Shift: `<<`, `>>`
  * Equality: `==`, `!=`
* Conversions:
  * Widening (within family): implicit
  * Narrowing (within family): explicit cast required
  * Compatibility (cross-family): `i*`, `u*`, `f*`, `bool` (explicit cast required)
* Notes:
  * `b128`: native if platform supports, otherwise soft-emulated

### Boolean

* Variants: `bool`
* Size: 8 bits
* Values: `true`, `false`
* Default: `false`
* Operations:
  * Logical: `&&`, `||`, `!`
  * Equality: `==`, `!=`
* Conversions:
  * To `i*`, `u*`, `b*`: `true` = 1, `false` = 0 (explicit cast required)
  * To `f*`: `true` = 1.0, `false` = 0.0 (explicit cast required)
  * From `i*`, `u*`, `b*`: 0 = `false`, non-zero = `true` (explicit cast required)
  * From `f*`: 0.0 = `false`, non-zero = `true` (explicit cast required)

### Character

* Variants: `char`
* Size: 32 bits
* Values: Unicode codepoints (U+0000 to U+10FFFF)
* Default: `'\0'`
* Literals: character in single quotes (`'a'`)
* Escapes: `\n`, `\r`, `\t`, `\\`, `\'`, `\"`, `\0`
* Operations:
  * Equality: `==`, `!=`

### String

* Variants: `string`
* Size: variable
* Values: UTF-8 encoded text
* Default: `""`
* Literals: text in double quotes (`"Hello, world!"`)
* Escapes: `\n`, `\r`, `\t`, `\\`, `\"`, `\0`
* Operations:
  * Equality: `==`, `!=`

### Unit

* Variants: `unit`
* Size: 0 bytes
* Values: `()`

### Never

* Variants: `never`
* Conversions: implicitly coerces to any type

## Aggregate types
	
Mercurial has the following aggregate types:

* Record
* Tuple
* Array

### Record

* Syntax: `record Name { field1: Type1, field2: Type2, ... }`
* Description: heterogeneous collection of named fields
* Size: sum of field sizes + padding
* Literals: `Name { field1: value1, field2: value2, ... }`
* Default: all fields initialized to their default values
* Operations:
  * Field access (e.g., `.field1`, `.field2`, etc.)
  * Equality: field-wise equality
* Compatibility: nominally typed, so `Name` is only compatible with itself

### Tuple

* Syntax: `(Type1, Type2, ..., TypeN)`
* Description: heterogeneous fixed-size positional collection (minimum 2 elements)
* Size: sum of element sizes + padding
* Literals: `(value1, value2, ..., valueN)`
* Default: all elements initialized to their default values
* Operations:
  * Positional access (e.g., `.0`, `.1`, etc.)
  * Equality: element-wise equality
* Compatibility: `(T1, T2, ..., TN)` is compatible with `(U1, U2, ..., UN)` if each `Ti` is `Ui`

### Array

* Syntax: `T[N]`
* Description: homogeneous fixed-size positional collection
* Size: element size * number of elements
* Literals: `[value1, value2, ..., valueN]`
* Default: all elements initialized to their default values
* Operations:
  * Subscript access (e.g., `[0]`, `[1]`, etc.)
  * Equality: element-wise equality
* Compatibility: `T[N]` is compatible with `U[M]` if `T` is `U` and `N` is `M`

## Function Type

Mercurial has one function type:

* Syntax: `fn(T1, T2, ..., TN) -> R`
* Description: represents callable value with a specific signature
* Size: implementation-defined
* Operations:
  * Invocation (e.g., `f(arg1, arg2, ..., argN)`)

## Type Aliases

* Syntax: `alias Name = Type`
* Description: defines an alternative name for an existing type
* Interchangeability: `Name` and `Type` are interchangeable in all contexts
* Built-in aliases:
  * `int` = `i64`
  * `uint` = `u64`
  * `float` = `f64`
  * `byte` = `b8`