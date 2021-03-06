#include "9cc.h"

void error(char *fmt, ...){
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    exit(1);
}

void error_at(char *loc, char *fmt, ...){
    va_list ap;
    va_start(ap, fmt);
    
    int pos = loc - user_input;
    fprintf(stderr, "%s\n", user_input);
    fprintf(stderr, "%*s", pos, " ");
    fprintf(stderr, "^ ");
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    exit(1);
}


//if next token is "op", put token forward and return true
bool consume(char *op){
    if(token->kind != TK_RESERVED || strlen(op) != token->len || memcmp(token->str, op, token->len))
        return false;
    token = token->next;
    return true;
}

//if next token is op, put token forward.
void expect(char *op){
    if(token->kind != TK_RESERVED || strlen(op) != token->len || memcmp(token->str, op, token->len))
        error_at(token->str, "expected \"%s\"", op);
    token = token->next;
}


//return the value if the token is number. and move the token forward.
int expect_number(){
    if(token->kind != TK_NUM) error_at(token->str, "not a number");
    int val = token->val;
    token = token->next;
    return val;
}

bool at_eof(){
    return token->kind == TK_EOF;
}    

//create new token "tok" and connect to current token "cur"
Token *new_token(TokenKind kind, Token *cur, char *str, int len){
    Token *tok = calloc(1, sizeof(Token));
    tok->kind = kind;
    tok->str = str;
    tok->len = len;
    cur->next = tok;
    return tok;
}

bool startswith(char *p, char *q){
    return memcmp(p, q, strlen(q)) == 0;
}

//argv[1] will be tokenized by this funciton
Token *tokenize(char *p){
//define the first token named "head"
    Token head;
    head.next = NULL;
    //cur is adress of current token
    Token *cur = &head;
    
    while(*p){
        if(isspace(*p)){
            p++;
            continue;
        }
        
        if(startswith(p, "==") || startswith(p, "!=") || startswith(p, "<=") || startswith(p, ">=")){
            cur = new_token(TK_RESERVED, cur, p, 2);
            p += 2;
            continue;
        }
        if(strchr("+-*/()<>", *p)){
            cur = new_token(TK_RESERVED, cur, p++, 1);
            continue;
        }
        if(isdigit(*p)){
            cur = new_token(TK_NUM, cur, p, 0);
            char *q = p;
            cur->val = strtol(p, &p, 10);
            cur->len = p - q;
            continue;
        }
        if('a' <= *p && *p <= 'z'){
            cur = new_token(TK_IDENT, cur, p++, 1);
            cur->len = 1;
            continue;
          }
        //error("couldnt talknize");
        error_at(p, "invalid token");
    }
    new_token(TK_EOF, cur, p, 0);
    //head is empty token. so
    return head.next;
}




Node *new_node(NodeKind kind){
    Node *node = calloc(1, sizeof(Node));
    node->kind = kind;
    return node;
}

Node *new_binary(NodeKind kind, Node *lhs, Node *rhs){
    Node *node = new_node(kind);
    node->lhs = lhs;
    node->rhs = rhs;
    return node;
}

Node *new_node_num(int val){
    Node *node = calloc(1, sizeof(Node));
    node->kind = ND_NUM;
    node->val = val;
    return node;
}


//??????????????????
//primary?????????(expr?????????)????????????: ()
//unary????????????: ???????????????(+, -)
//mul????????????: *, /
//add????????????: +, -
//relational????????????: <, >, <=. >=
//equality????????????: ==, !=
//????????????????????????????????????????????????????????????


Node *expr(){
    return equality();
}

Node *equality(){
    Node *node = relational();
    for(;;){
        if(consume("=="))
            node = new_binary(ND_EQ, node, relational());
        else if(consume("!="))
            node = new_binary(ND_NE, node, relational());
        else
            return node;
    }
}

Node *relational(){
    Node *node = add();

    for(;;){
        if(consume("<"))
            node = new_binary(ND_LT, node, add());
        else if(consume("<="))
            node = new_binary(ND_LE, node, add());
        else if(consume(">"))
            node = new_binary(ND_LT, add(), node);
        else if(consume(">="))
            node = new_binary(ND_LE, add(), node);
        else
            return node;
    }
}

Node *add(){
    Node *node = mul();
    for(;;){
        if(consume("+")) 
            node = new_binary(ND_ADD, node, mul());
        else if(consume("-"))
            node = new_binary(ND_SUB, node, mul());
        else 
            return node;
    }
}

Node *mul(){
    Node *node = unary();
    for(;;){
        if(consume("*")) 
            node = new_binary(ND_MUL, node, unary());
        else if(consume("/")) 
            node = new_binary(ND_DIV, node, unary());
        else 
            return node;
    }
}

Node *unary(){
    if(consume("+"))
        return primary();
    if(consume("-"))
        return new_binary(ND_SUB, new_node_num(0), primary());
    return primary();
}

Node *primary(){
    if(consume("(")){
        Node *node = expr();
        expect(")");
        return node;
    }
    return new_node_num(expect_number());
}




