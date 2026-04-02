# Mercurial language grammar v0.1

## Alphabet

Char = [\u0000-\u007F]

```ebnf

LETTER ::= [A-Za-z]
UNDERSCORE ::= '_'
DIGIT ::= [0-9]
HEX-DIGIT ::= DIGIT | [A-Fa-f]
OCT-DIGIT ::= [0-7]
BIN-DIGIT ::= '0' | '1'
SIGN ::= '+' | '-'
EXPONENT ::= 'e' | 'E'

NEWLINE ::= '\n' | '\r\n' | '\r'
WHITESPACE ::= ' ' | '\t' | NEWLINE

SINGLE-QUOTE ::= '\''
DOUBLE-QUOTE ::= '"'
ESCAPE ::= '\\'

ESCAPABLE ::= ESCAPE | SINGLE-QUOTE | DOUBLE-QUOTE | 'n' | 'r' | 't' | '0'
```


## Lexical grammar

### Tokens

```ebnf
token ::= identifier | keyword | literal | operator | punctuation
trivia ::= (WHITESPACE | comment)*
```

### Identifiers

```ebnf
identifier ::= ident-start ident-continue*
ident-start ::= LETTER | UNDERSCORE
ident-continue ::= LETTER | DIGIT | UNDERSCORE
```

### Keywords

```ebnf
keyword ::= stmt-keyword | decl-keyword | literal-keyword | operator-keyword | special-keyword

stmt-keyword ::= 'if' | 'else' | 'while' | 'loop' |
				'return' | 'break' | 'continue'

decl-keyword ::= 'var' | 'alias' | 'import' | 'fn' | 'record' | 'namespace'

literal-keyword ::= 'true' | 'false' | 'pass' | 'todo' | 'default'

operator-keyword ::= 'and' | 'or' | 'not'

special-keyword ::= 'uninit'
```

### Literals

```ebnf
literal ::= integer | float | char | string

integer ::= hex-integer | oct-integer | bin-integer | decimal-integer
hex-integer ::= '0x' HEX-DIGIT+
oct-integer ::= '0o' OCT-DIGIT+
bin-integer ::= '0b' BIN-DIGIT+

decimal-integer ::= DIGIT+

float ::= DIGIT+ '.' DIGIT* exponent? | DIGIT+ exponent
exponent ::= EXPONENT SIGN? DIGIT+

char ::= SINGLE-QUOTE (escape-seq | single-quote-char) SINGLE-QUOTE
single-quote-char ::= ~[ESCAPE | SINGLE-QUOTE | NEWLINE]

string ::= DOUBLE-QUOTE (escape-seq | double-quote-char)* DOUBLE-QUOTE
double-quote-char ::= ~[ESCAPE | DOUBLE-QUOTE | NEWLINE]

escape-seq ::= ESCAPE ESCAPABLE
```

### Operators

```ebnf
operator ::= one-char-operator | multi-char-operator
one-char-operator ::= '+' | '-' | '*' | '/' | '%' | '=' | '!' | '<' | '>' | '&' | '|' | '^' | '~'
multi-char-operator ::= '+=' | '-=' | '*=' | '/=' | '%=' |
						   '==' | '!=' | '<=' | '<<' | '<<=' | '>=' | '>>' | '>>=' |
						   '&&' | '&=' | '||' | '|=' | '^='
```

### Punctuation

```ebnf
punctuation ::= one-char-punctuation | multi-char-punctuation
one-char-punctuation ::= '(' | ')' | '{' | '}' | '[' | ']' | ':' | ';' | ',' | '.'
multi-char-punctuation ::= '->'
```

### Comments

```ebnf

comment ::= line-comment | block-comment
line-comment ::= '//' ~[NEWLINE]*

block-comment ::= '/*' (~['*'] | '*' ~['/'])* '*/'
```

## Syntax grammar

### Module

```ebnf
module ::= decl* EOF
```

### Block

```ebnf
block ::= '{' block-item-list? '}'
block-item-list ::= block-item (block-item)*
block-item ::= block-decl | stmt
```

### Bindings

```ebnf
binding ::= pattern ':' type ('=' init-expr)? |
			pattern '=' init-expr


init-expr ::= expr | 'uninit'

trailing-binding-list ::=  binding-list ','?
binding-list ::= binding (',' binding)*
```

### Declarations

```ebnf
decl ::= block-decl		|
		 fn-decl		|
		 record-decl	|
		 namespace-decl	|
		 import-decl

block-decl ::= var-decl | alias-decl

var-decl ::= 'var' binding-list ';'

alias-decl ::= 'alias' alias-binding-list ';'
alias-binding-list ::= alias-binding (',' alias-binding)*
alias-binding ::= identifier '=' type

fn-decl ::= 'fn' identifier '(' trailing-binding-list? ')' ('->' type)? block

record-decl ::= 'record' identifier '{' trailing-binding-list? '}'

namespace-decl ::= 'namespace' identifier '{' decl* '}'

import-decl ::= 'import' import-item-list ';'
import-item-list ::= import-item (',' import-item)*
import-item ::= identifier
```

### Statements

```ebnf
stmt ::= if-stmt       |
		 while-stmt    |
		 loop-stmt     |
		 return-stmt   |
		 break-stmt    |
		 continue-stmt |
		 expr-or-assignment-stmt

if-stmt ::= if-clause else-clause?
if-clause ::= 'if' '(' expr ')' block
else-clause ::= 'else' block

while-stmt ::= 'while' '(' expr ')' block

loop-stmt ::= 'loop' block

return-stmt ::= 'return' expr? ';'

break-stmt ::= 'break' ';'

continue-stmt ::= 'continue' ';'

expr-or-assignment-stmt ::= (expr | assignment) ';'

assignment ::= expr assign-op expr
assign-op ::= '=' | '+=' | '-=' | '*=' | '/=' | '%=' | '&=' | '|=' | '^=' | '<<=' | '>>='
```

### Expressions

```ebnf
expr ::= binary-expr

binary-expr ::= unary-expr (binary-op unary-expr)*
binary-op ::= '||' |
			  '&&' |
			  '==' | '!=' |
			  '<' | '>' | '<=' | '>=' |
			  '|' |
			  '^' |
			  '&' |
			  '<<' | '>>' |
			  '+' | '-' |
			  '*' | '/' | '%'

unary-expr ::= prefix-op unary-expr | postfix-expr
prefix-op ::= '+' | '-' | '!' | '~'

postfix-expr ::= primary-expr postfix-suffix*
postfix-suffix ::= call-suffix | subscript-suffix | member-suffix
call-suffix ::= '(' expr-list? ')'
subscript-suffix ::= '[' expr ']'
member-suffix ::= '.' (identifier | decimal-integer)

primary-expr ::= lambda-expr 		  |
				 array-expr			  |
				 paren-or-tuple-expr  |
				 ident-or-record-expr |
				 literal-expr

lambda-expr ::= 'fn' '(' trailing-binding-list? ')' ('->' type)? block

array-expr ::= '[' expr-list? ']'

paren-or-tuple-expr ::= unit-literal | group-expr | tuple-expr
unit-literal ::= '(' ')'
group-expr ::= '(' expr ')'
tuple-expr ::= '(' expr ',' expr-list? ')'

ident-or-record-expr ::= identifier | record-expr
record-expr ::= identifier '{' field-init-list? '}'
field-init-list ::= field-init (',' field-init)* ','?
field-init ::= identifier '=' expr

literal-expr ::= literal | literal-keyword

expr-list ::= expr (',' expr)* ','?
```

### Types

```ebnf
type ::= primary-type (type-suffix)?
type-suffix ::= '[' expr ']'

primary-type ::= fn-type			 |
				 paren-or-tuple-type |
				 identifier

paren-or-tuple-type ::= unit-type | paren-type | tuple-type
unit-type ::= '(' ')'
paren-type ::= '(' type ')'
tuple-type ::= '(' type ',' type-list? ')'

fn-type ::= 'fn' '(' type-list? ')' ('->' type)?

type-list ::= type (',' type)* ','?
```

### Patterns

```ebnf
pattern ::= named-pattern | paren-pattern
named-pattern ::= identifier
paren-pattern ::= group-pattern | list-pattern
group-pattern ::= '(' pattern ')'
list-pattern ::= '(' pattern ',' pattern-list? ')'

pattern-list ::= pattern (',' pattern)* ','?
```