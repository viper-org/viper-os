#ifndef VIPEROS_SH_LEXER
#define VIPEROS_SH_LEXER 1

#include <stddef.h>

// this is probably overkill

enum token_type
{
    TOK_STR,
    TOK_EOF
};

struct token
{
    enum token_type type;
    char *text;
};

struct lexer
{
    char *l;
    size_t position;
    size_t len;

    struct token *toks;
    int ntok;
    int ctok;
};

void lex(struct lexer *l);
void lexer_cleanup(struct lexer *l);

#endif // VIPEROS_SH_LEXER
