#define F(f) TOKEN_FLAG_ ## f

/* keywords */
X(TOKEN_ATTRIBUTE,	"__attribute__",	0)
X(TOKEN_BREAK,		"break",		0)
X(TOKEN_CASE,		"case",			0)
X(TOKEN_CHAR,		"char",			F(TYPE))
X(TOKEN_CONST,		"const",		F(QUALIFIER))
X(TOKEN_CONTINUE,	"continue",		0)
X(TOKEN_DEFAULT,	"default",		0)
X(TOKEN_DO,		"do",			0)
X(TOKEN_DOUBLE,		"double",		F(TYPE))
X(TOKEN_ELSE,		"else",			0)
X(TOKEN_ENUM,		"enum",			F(TYPE) | F(IDENT))
X(TOKEN_EXTERN,		"extern",		F(STORAGE))
X(TOKEN_FLOAT,		"float",		F(TYPE))
X(TOKEN_FOR,		"for",			0)
X(TOKEN_GOTO,		"goto",			0)
X(TOKEN_IF,		"if",			0)
X(TOKEN_INLINE,		"inline",		F(STORAGE))
X(TOKEN_INT,		"int",			F(TYPE))
X(TOKEN_LONG,		"long",			F(TYPE))
X(TOKEN_REGISTER,	"register",		F(STORAGE))
X(TOKEN_RETURN,		"return",		0)
X(TOKEN_SHORT,		"short",		F(TYPE))
X(TOKEN_SIGNED,		"signed",		F(TYPE))
X(TOKEN_SIZEOF,		"sizeof",		0)
X(TOKEN_STATIC,		"static",		F(STORAGE))
X(TOKEN_STRUCT,		"struct",		F(TYPE) | F(IDENT))
X(TOKEN_SWITCH,		"switch",		0)
X(TOKEN_TYPEDEF,	"typedef",		F(TYPE))
X(TOKEN_UNION,		"union",		F(TYPE) | F(IDENT))
X(TOKEN_UNSIGNED,	"unsigned",		F(TYPE))
X(TOKEN_VOID,		"void",			F(TYPE))
X(TOKEN_VOLATILE,	"volatile",		F(QUALIFIER))
X(TOKEN_WHILE,		"while",		0)

/* punctuators */
X(TOKEN_LSQUARE,		"[",	0)
X(TOKEN_RSQUARE,		"]",	0)
X(TOKEN_LPAREN,			"(",	0)
X(TOKEN_RPAREN,			")",	0)
X(TOKEN_LBRACE,			"{",	0)
X(TOKEN_RBRACE,			"}",	0)
X(TOKEN_PERIOD,			".",	F(AMBIGUOUS))
X(TOKEN_ELLIPSIS,		"...",	F(TYPE))
X(TOKEN_AMP,			"&",	F(AMBIGUOUS) | F(BINARY))
X(TOKEN_AMPAMP,			"&&",	F(BINARY))
X(TOKEN_AMPEQUAL,		"&=",	F(ASSIGN))
X(TOKEN_STAR,			"*",	F(AMBIGUOUS) | F(BINARY))
X(TOKEN_STAREQUAL,		"*=",	F(ASSIGN))
X(TOKEN_PLUS,			"+",	F(AMBIGUOUS) | F(BINARY))
X(TOKEN_PLUSPLUS,		"++",	0)
X(TOKEN_PLUSEQUAL,		"+=",	F(ASSIGN))
X(TOKEN_MINUS,			"-",	F(AMBIGUOUS) | F(BINARY))
X(TOKEN_ARROW,			"->",	0)
X(TOKEN_MINUSMINUS,		"--",	0)
X(TOKEN_MINUSEQUAL,		"-=",	F(ASSIGN))
X(TOKEN_TILDE,			"~",	0)
X(TOKEN_EXCLAIM,		"!",	F(AMBIGUOUS))
X(TOKEN_EXCLAIMEQUAL,		"!=",	F(BINARY))
X(TOKEN_SLASH,			"/",	F(AMBIGUOUS) | F(BINARY))
X(TOKEN_SLASHEQUAL,		"/=",	F(ASSIGN))
X(TOKEN_PERCENT,		"%",	F(AMBIGUOUS) | F(BINARY))
X(TOKEN_PERCENTEQUAL,		"%=",	F(ASSIGN))
X(TOKEN_LESS,			"<",	F(AMBIGUOUS) | F(BINARY))
X(TOKEN_LESSLESS,		"<<",	F(AMBIGUOUS) | F(BINARY))
X(TOKEN_LESSEQUAL,		"<=",	F(BINARY))
X(TOKEN_LESSLESSEQUAL,		"<<=",	F(ASSIGN))
X(TOKEN_GREATER,		">",	F(AMBIGUOUS) | F(BINARY))
X(TOKEN_GREATERGREATER,		">>",	F(AMBIGUOUS) | F(BINARY))
X(TOKEN_GREATEREQUAL,		">=",	F(ASSIGN))
X(TOKEN_GREATERGREATEREQUAL,	">>=",	F(ASSIGN))
X(TOKEN_CARET,			"^",	F(AMBIGUOUS))
X(TOKEN_CARETEQUAL,		"^=",	F(ASSIGN))
X(TOKEN_PIPE,			"|",	F(AMBIGUOUS) | F(BINARY))
X(TOKEN_PIPEPIPE,		"||",	F(BINARY))
X(TOKEN_PIPEEQUAL,		"|=",	F(ASSIGN))
X(TOKEN_QUESTION,		"?",	0)
X(TOKEN_COLON,			":",	0)
X(TOKEN_SEMI,			";",	0)
X(TOKEN_EQUAL,			"=",	F(AMBIGUOUS) | F(ASSIGN))
X(TOKEN_EQUALEQUAL,		"==",	F(BINARY))
X(TOKEN_COMMA,			",",	0)
X(TOKEN_BACKSLASH,		"\\",	F(DISCARD))

/* sentinels */
X(TOKEN_COMMENT,	"",	0)
X(TOKEN_CPP,		"",	0)
X(TOKEN_EOF,		"",	0)
X(TOKEN_ERROR,		"",	0)
X(TOKEN_IDENT,		"",	0)
X(TOKEN_LITERAL,	"",	0)
X(TOKEN_SPACE,		"",	0)
X(TOKEN_STRING,		"",	0)
X(TOKEN_UNKNOWN,	"",	0)
X(TOKEN_NONE,		"",	0)

#undef F
