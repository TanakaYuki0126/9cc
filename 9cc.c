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
};

Token *token;
char *user_input;

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
bool consume(char op){
    if(token->kind != TK_RESERVED || token->str[0] != op)
        return false;
    token = token->next;
    return true;
}

//if next token is op, put token forward.
void expect(char op){
    if(token->kind != TK_RESERVED || token->str[0] != op)
      error("not '%c'", op);
      
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
Token *new_token(TokenKind kind, Token *cur, char *str){
    Token *tok = calloc(1, sizeof(Token));
    tok->kind = kind;
    tok->str = str;
    cur->next = tok;
    return tok;
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
        
        if(*p == '+' || *p == '-'){
            cur = new_token(TK_RESERVED, cur, p++);
            continue;
        }
        
        if(isdigit(*p)){
            cur = new_token(TK_NUM, cur, p);
            cur->val = strtol(p, &p, 10);
            continue;
        }
        //error("couldnt talknize");
        error_at(token->str, "couldnt talknize");
    }
    new_token(TK_EOF, cur, p);
    //head is empty token. so
    return head.next;
}


    
int main(int argc, char **argv){
    if(argc!= 2){
        fprintf(stderr, "input 2 argument");
        return 1;
    }
    user_input = argv[1];
    //token: result of tokenized argv[1]
    token = tokenize(argv[1]);
   
    
    printf(".intel_syntax noprefix\n");
    printf(".global main\n");
    printf("main:\n");
    printf("    mov rax, %d\n", expect_number());
    while(!at_eof()){
    	if(consume('+')){
    		printf("    add rax, %d\n", expect_number());
    		continue;
    	}
    	
    	expect('-');
    	printf("    sub rax, %d\n", expect_number());
    	
    }
    printf("    ret\n");
    return 0;
}
