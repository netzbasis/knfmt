#include "config.h"

#ifdef HAVE_QUEUE
#  include <sys/queue.h>
#else
#  include "compat-queue.h"
#endif

/* Annotate the argument as unused. */
#define UNUSED(x)	__##x __attribute__((__unused__))
#define MAYBE_UNUSED(x)	x __attribute__((__unused__))

#define UNLIKELY(x)	__builtin_expect((x), 0)

/*
 * config ----------------------------------------------------------------------
 */

struct config {
	unsigned int	cf_flags;
#define CONFIG_FLAG_DIFF		0x00000001u
#define CONFIG_FLAG_INPLACE		0x00000002u
#define CONFIG_FLAG_TEST		0x80000000u

	unsigned int	cf_verbose;
	unsigned int	cf_mw;		/* max width per line */
	unsigned int	cf_tw;		/* tab width */
	unsigned int	cf_sw;		/* soft width */
};

void	config_init(struct config *);

/*
 * buffer ----------------------------------------------------------------------
 */

struct buffer {
	char	*bf_ptr;
	size_t	 bf_siz;
	size_t	 bf_len;
};

struct buffer	*buffer_alloc(size_t);
struct buffer	*buffer_read(const char *);
void		 buffer_free(struct buffer *);
void		 buffer_append(struct buffer *, const char *, size_t);
void		 buffer_appendc(struct buffer *, char);
void		 buffer_appendv(struct buffer *, const char *, ...)
	__attribute__((__format__(printf, 2, 3)));
void		 buffer_reset(struct buffer *);
int		 buffer_cmp(const struct buffer *, const struct buffer *);

/*
 * error -----------------------------------------------------------------------
 */

struct error {
	const struct config	*er_cf;
	struct buffer		*er_bf;
};

void		 error_init(struct error *, const struct config *);
void		 error_close(struct error *);
void		 error_reset(struct error *);
void		 error_flush(struct error *);
struct buffer	*error_get_buffer(struct error *);

#define error_write(er, fmt, ...) do {					\
	buffer_appendv(error_get_buffer((er)), (fmt), __VA_ARGS__);	\
	if (UNLIKELY((er)->er_cf->cf_verbose) >= 2)			\
		error_flush((er));					\
} while (0)

/*
 * token -----------------------------------------------------------------------
 */

enum token_type {
#define T(t, s, f) t,
#include "token.h"
};

TAILQ_HEAD(token_list, token);

struct token {
	enum token_type	tk_type;
	unsigned int	tk_lno;
	unsigned int	tk_cno;
	unsigned int	tk_markers;
	unsigned int	tk_flags;
#define TOKEN_FLAG_TYPE		0x00000001u
#define TOKEN_FLAG_QUALIFIER	0x00000002u
#define TOKEN_FLAG_STORAGE	0x00000004u
#define TOKEN_FLAG_IDENT	0x00000008u	/* optionally following ident */
#define TOKEN_FLAG_DANGLING	0x00000010u
#define TOKEN_FLAG_ASSIGN	0x00000020u
#define TOKEN_FLAG_AMBIGUOUS	0x00000040u
#define TOKEN_FLAG_BINARY	0x00000080u
#define TOKEN_FLAG_DISCARD	0x00000100u
#define TOKEN_FLAG_UNMUTE	0x00000200u
#define TOKEN_FLAG_NEWLINE	0x00000400u
#define TOKEN_FLAG_FAKE		0x00000800u
#define TOKEN_FLAG_FREE		0x00001000u
#define TOKEN_FLAG_TYPE_ARGS	0x08000000u
#define TOKEN_FLAG_TYPE_FUNC	0x10000000u

	size_t		tk_off;

	const char	*tk_str;
	size_t		 tk_len;

	union {
		struct token	*tk_token;
		int		 tk_int;
	};

	struct {
		struct token	*br_pv;
		struct token	*br_nx;
	} tk_branch;

	struct token_list	tk_prefixes;
	struct token_list	tk_suffixes;

	TAILQ_ENTRY(token)	tk_entry;
};

int	 token_cmp(const struct token *, const struct token *);
int	 token_has_dangling(const struct token *);
int	 token_has_line(const struct token *);
int	 token_is_branch(const struct token *);
int	 token_is_decl(const struct token *, enum token_type);
void	 token_trim(struct token *);
char	*token_sprintf(const struct token *);

/*
 * lexer -----------------------------------------------------------------------
 */

struct lexer_state {
	struct token	*st_tok;
	unsigned int	 st_lno;
	unsigned int	 st_cno;
	unsigned int	 st_err;
	size_t		 st_off;
};

struct lexer_recover_markers {
#define NMARKERS	2
	struct token	*lm_markers[NMARKERS];
};

void		 lexer_init(void);
void		 lexer_shutdown(void);
struct lexer	*lexer_alloc(const char *, struct error *,
    const struct config *);
void		 lexer_free(struct lexer *);

const struct buffer	*lexer_get_buffer(const struct lexer *);
int			 lexer_get_error(const struct lexer *);

void	lexer_recover_enter(struct lexer_recover_markers *);
void	lexer_recover_leave(struct lexer_recover_markers *);
void	lexer_recover_mark(struct lexer *, struct lexer_recover_markers *);
void	lexer_recover_purge(struct lexer_recover_markers *);

#define lexer_recover(a, b)						\
	__lexer_recover((a), (b), __func__, __LINE__)
int	__lexer_recover(struct lexer *, struct lexer_recover_markers *,
    const char *, int);

#define lexer_branch(a, b, c)						\
	__lexer_branch((a), (b), (c), __func__, __LINE__)
int	__lexer_branch(struct lexer *, struct token **, const struct token *,
    const char *, int);

int	lexer_is_branch(const struct lexer *);
int	lexer_is_branch_end(const struct lexer *);

int	lexer_pop(struct lexer *, struct token **);
int	lexer_back(const struct lexer *, struct token **);

#define lexer_expect(a, b, c) \
	__lexer_expect((a), (b), (c), __func__, __LINE__)
int	__lexer_expect(struct lexer *, enum token_type, struct token **,
    const char *, int);

void	lexer_peek_enter(struct lexer *, struct lexer_state *);
void	lexer_peek_leave(struct lexer *, struct lexer_state *);

int	lexer_peek(struct lexer *, struct token **);

int	lexer_peek_if_type(struct lexer *, struct token **);
int	lexer_if_type(struct lexer *, struct token **);

int	lexer_peek_if(struct lexer *, enum token_type, struct token **);
int	lexer_if(struct lexer *, enum token_type, struct token **);

int	lexer_peek_if_flags(struct lexer *, unsigned int, struct token **);
int	lexer_if_flags(struct lexer *, unsigned int, struct token **);

int	lexer_peek_if_pair(struct lexer *, enum token_type, enum token_type,
    struct token **);
int	lexer_if_pair(struct lexer *, enum token_type, enum token_type,
    struct token **);

int	lexer_peek_until(struct lexer *, enum token_type, struct token **);
int	lexer_peek_until_loose(struct lexer *, enum token_type,
    const struct token *, struct token **);
int	lexer_peek_until_stop(struct lexer *, enum token_type,
    const struct token *, struct token **);
#define lexer_until(a, b, c) \
	__lexer_until((a), (b), NULL, (c), __func__, __LINE__)
int	__lexer_until(struct lexer *, enum token_type, const struct token *,
    struct token **, const char *, int);

void	lexer_trim_enter(struct lexer *);
void	lexer_trim_leave(struct lexer *);

void	lexer_dump(const struct lexer *);

/*
 * parser ----------------------------------------------------------------------
 */

struct parser		*parser_alloc(const char *, struct error *,
    const struct config *);
void			 parser_free(struct parser *);
const struct buffer	*parser_exec(struct parser *);
struct doc		*parser_exec_expr_recover(void *);

struct lexer	*parser_get_lexer(struct parser *);

/*
 * expr ------------------------------------------------------------------------
 */

struct expr_exec_arg {
	const struct config	*ea_cf;
	struct lexer		*ea_lx;
	struct doc		*ea_dc;
	const struct token	*ea_stop;

	/*
	 * Callback invoked when an invalid expression is encountered. If the
	 * same callback returns a document implies that the expression parser
	 * can continue.
	 */
	struct doc	*(*ea_recover)(void *);
	void		*ea_arg;
};

struct doc	*expr_exec(const struct expr_exec_arg *);
int		 expr_peek(const struct expr_exec_arg *);

/*
 * doc -------------------------------------------------------------------------
 */

/* Keep in sync with DESIGN. */
enum doc_type {
	DOC_CONCAT,
	DOC_GROUP,
	DOC_INDENT,
	DOC_DEDENT,
	DOC_ALIGN,
	DOC_LITERAL,
	DOC_VERBATIM,
	DOC_LINE,
	DOC_SOFTLINE,
	DOC_HARDLINE,
	DOC_NEWLINE,
	DOC_MUTE,
};

void		doc_exec(const struct doc *, struct buffer *,
    const struct config *);
unsigned int	doc_width(const struct doc *, struct buffer *,
    const struct config *);
void		doc_free(struct doc *);
void		doc_append(struct doc *, struct doc *);
void		doc_remove(struct doc *, struct doc *);
void		doc_remove_tail(struct doc *);
void		doc_set_indent(struct doc *, int);

#define doc_alloc(a, b) \
	__doc_alloc((a), (b), __func__, __LINE__)
struct doc	*__doc_alloc(enum doc_type, struct doc *, const char *, int);

/*
 * Sentinels honored by doc_alloc_dedent() and doc_alloc_indent(). The numbers
 * are something arbitrary large enough to never conflict with any actual
 * indentation. Since the numbers are compared with signed integers, favor
 * integer literals over hexadecimal ones.
 */
#define DOC_DEDENT_NONE		512	/* remove all indentation */
#define DOC_INDENT_PARENS	1024	/* entering parenthesis */
#define DOC_INDENT_FORCE	2048	/* force indentation */

#define doc_alloc_indent(a, b) \
	__doc_alloc_indent(DOC_INDENT, (a), (b), __func__, __LINE__)
#define doc_alloc_dedent(a, b) \
	__doc_alloc_indent(DOC_DEDENT, (a), (b), __func__, __LINE__)
struct doc	*__doc_alloc_indent(enum doc_type, int, struct doc *,
    const char *, int);

#define doc_literal(a, b) \
	__doc_literal((a), (b), __func__, __LINE__)
struct doc	*__doc_literal(const char *, struct doc *, const char *, int);

#define doc_token(a, b) \
	__doc_token((a), (b), DOC_LITERAL, __func__, __LINE__)
struct doc	*__doc_token(const struct token *, struct doc *, enum doc_type,
    const char *, int);

/*
 * ruler -----------------------------------------------------------------------
 */

struct ruler {
	struct {
		struct ruler_column	*b_ptr;
		size_t			 b_len;
		size_t			 b_siz;
	} rl_columns;

	unsigned int	rl_align;
};

struct ruler_column {
	struct {
		struct ruler_datum	*b_ptr;
		size_t			 b_len;
		size_t			 b_siz;
	} rc_datums;

	size_t	rc_len;
	size_t	rc_nspaces;
};

void	ruler_init(struct ruler *, unsigned int);
void	ruler_free(struct ruler *);
void	ruler_insert(struct ruler *, const struct token *, struct doc *,
    unsigned int, unsigned int, unsigned int);
void	ruler_exec(struct ruler *);

/*
 * util ------------------------------------------------------------------------
 */

char	*strnice(const char *, size_t);
