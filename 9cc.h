#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum{
    TK_RESERVED,
    TK_NUM,
    TK_EOF,
} TokenKind;

typedef struct Token Token;

struct Token {
    TokenKind kind;
    Token *next;
    int val;
    char *str;
    int len;
};

Token *token;
char *user_input;

typedef enum{
    ND_ADD,
    ND_SUB,
    ND_MUL,
    ND_DIV,
    ND_EQ,
    ND_NE,
    ND_LT, // larger than
    ND_LE, // larger or equal
    ND_NUM,
} NodeKind;

typedef struct Node Node;

struct Node{
    NodeKind kind;
    Node *lhs;//ledt handed side
    Node *rhs;//right handed side
    int val;//use if "kind" = ND_NUM
};


//Nodeとは、(左辺) (演算子) (右辺)のペアのこと。
//NodeKindは、演算子の種類。
//単なる数値の時は、kindはND_NUMとし、Node->valにその値を代入。


//codegen.c
void gen(Node *node);


//parse.c
void error(char *fmt, ...);
void error_at(char *loc, char *fmt, ...);
bool consume(char *op);
void expect(char *op);
int expect_number();
bool at_eof();
Token *new_token(TokenKind kind, Token *cur, char *str, int len);
bool startswith(char *p, char *q);
Token *tokenize(char *p);

Node *primary();
Node *mul();
Node *expr();
Node *unary();
Node *equality();
Node *relational();
Node *add();

Node *new_node(NodeKind kind);
Node *new_binary(NodeKind kind, Node *lhs, Node *rhs);
Node *new_node_num(int val);
Node *expr();
Node *equality();
Node *relational();
Node *mul();
Node *unary();
Node *primary();
