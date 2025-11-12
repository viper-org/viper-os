#include "lexer.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

int ispath(int ch)
{
    return isgraph(ch) && ch != '|'
        && ch != '>' && ch != '<';
}

char lexer_current(struct lexer *l)
{
    return l->l[l->position];
}

char lexer_consume(struct lexer *l)
{
    return l->l[l->position++];
}

char lexer_peek(struct lexer *l, int off)
{
    return l->l[l->position + off];
}

struct token lexer_next(struct lexer *l)
{
    if (ispath(lexer_current(l)))
    {
        char *buf = malloc(32);
        buf[0] = lexer_current(l);
        int n = 1, c = 32;
        while (ispath(lexer_peek(l, 1)))
        {
            if (n >= c)
            {
                buf = realloc(buf, c * 2);
                c *= 2;
            }
            lexer_consume(l);
            buf[n++] = lexer_current(l);
        }
        buf[n] = 0;
        return (struct token) {
            .type = TOK_STR,
            .text = buf
        };
    }
    if (isspace(lexer_current(l)))
    {
        lexer_consume(l);
        return lexer_next(l);
    }
    switch (lexer_current(l))
    {
        case '>':
            if (lexer_peek(l, 1) == '>')
            {
                lexer_consume(l);
                return (struct token) {
                    .type = TOK_PIPER2,
                    .text = NULL
                };
            }
            return (struct token) {
                .type = TOK_PIPER,
                .text = NULL
            };

        case 0:
            return (struct token) {
                .type = TOK_EOF,
                .text = NULL
            };
    }
    return (struct token) { // todo: return TOK_BAD
        .type = TOK_EOF,
        .text = NULL
    };
}

void lex(struct lexer *l)
{
    l->len = strlen(l->l);
    l->position = 0;
    l->toks = malloc(sizeof (struct token) * 32);
    l->ntok = 0;
    l->ctok = 32;
    
    while (l->position < l->len)
    {
        struct token tok = lexer_next(l);
        if (tok.type == TOK_EOF) return;
        if (l->ntok >= l->ctok)
        {
            l->toks = realloc(l->toks, l->ctok * 2);
            l->ctok *= 2;
        }
        l->toks[l->ntok++] = tok;
        lexer_consume(l);
    }
}

void lexer_cleanup(struct lexer *l)
{
    for (int i = 0; i < l->ntok; ++i)
    {
        if (l->toks[i].text)
        {
            free(l->toks[i].text);
        }
    }
    free(l->toks);
}
