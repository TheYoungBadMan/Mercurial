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
keyword ::= stmt-keyword | decl-keyword | literal-keyword | special-keyword | operator-keyword

stmt-keyword ::= 'if' | 'else' | 'while' | 'loop' |
				'break' | 'continue' | 'return'

decl-keyword ::= 'var' | 'alias' | 'fn' | 'record' | 'namespace' | 'import'

literal-keyword ::= 'true' | 'false' | 'pass' | 'todo' | 'default'

special-keyword ::= 'uninit'

operator-keyword ::= 'and' | 'or' | 'not'
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
block ::= '{' (stmt | block-decl)+ '}'
```

### Declarations

```ebnf
decl ::= block-decl     |
				fn-decl        |
				record-decl    |
				namespace-decl |
				import-decl

block-decl ::= var-decl | alias-decl

var-decl ::= 'var' binding-list ';'

alias-decl ::= 'alias' alias-list ';'
alias-list ::= alias-item (',' alias-item)*
alias-item ::= identifier '=' type

fn-decl ::= 'fn' identifier '(' trailing-binding-list? ')' ('->' type)? block

record-decl ::= 'record' identifier '{' trailing-binding-list? '}'

namespace-decl ::= 'namespace' identifier '{' decl* '}'

import-decl ::= 'import' import-list ';'
import-list ::= import-item (',' import-item)*
import-item ::= identifier
```

### Statements

```ebnf
stmt ::= if-stmt       |
		 while-stmt    |
		 loop-stmt     |
		 break-stmt    |
		 continue-stmt |
		 return-stmt   |
		 decl-stmt     |
		 expr-stmt

if-stmt ::= if-clause else-clause?
if-clause ::= 'if' '(' expr ')' block
else-clause ::= 'else' block

while-stmt ::= 'while' '(' expr ')' block

loop-stmt ::= 'loop' block

break-stmt ::= 'break' ';'

continue-stmt ::= 'continue' ';'

return-stmt ::= 'return' expr? ';'

decl-stmt ::= block-decl

expr-stmt ::= (expr | assignment) ';'

assignment ::= expr assign-op expr
assign-op ::= '=' | '+=' | '-=' | '*=' | '/=' | '%=' | '&=' | '|=' | '^=' | '<<=' | '>>='
```

### Expressions

```ebnf
expr ::= binary-expr

binary-expr ::= unary-expr (binary-op unary-expr)*
binary-op ::= '||' | '&&' |
			  '==' | '!=' | '<' | '>' | '<=' | '>=' |
			  '|' | '^' | '&' |
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

primary-expr ::= paren-expr   |
				 array-expr   |
				 lambda-expr  |
				 ident-expr   |
				 literal-expr
		
paren-expr ::= unit-expr | group-expr | tuple-expr
unit-expr ::= '(' ')'
group-expr ::= '(' expr ')'
tuple-expr ::= '(' expr ',' expr-list? ')'

array-expr ::= '[' expr-list? ']'

ident-expr ::= identifier | record-expr
record-expr ::= identifier '{' field-init-list? '}'
field-init-list ::= field-init (',' field-init)* ','?
field-init ::= identifier '=' expr

lambda-expr ::= 'fn' '(' trailing-binding-list? ')' ('->' type)? block

literal-expr ::= literal | literal-keyword

expr-list ::= expr (',' expr)* ','?
```

### Types

```ebnf
type ::= primary-type (type-suffix)?
type-suffix ::= '[' expr ']'

primary-type ::= paren-type |
				 fn-type    |
				 identifier

paren-type ::= group-type | tuple-type
group-type ::= '(' type ')'
tuple-type ::= '(' type (',' type)+ ','? ')'

fn-type ::= 'fn' '(' type-list? ')' ('->' type)?

type-list ::= type (',' type)* ','?
```

### Bindings

```ebnf
binding ::= binding-target ':' type ('=' init-expr)? |
			binding-target '=' init-expr

binding-target ::= identifier

init-expr ::= expr | 'uninit'

trailing-binding-list ::=  binding-list ','?
binding-list ::= binding (',' binding)*
```