#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>
#include<string.h>

#define INPUT_LENGTH 1000

#define INT 0
#define OPERATOR 1
#define ERR -1
#define INT_STR "INT"
#define OPERATOR_STR "OPERATOR"

typedef struct{
    int type; //either int or operator
    char operator_value;
    int number_value;
} Token;

size_t curr_pos = 0;
Token curr_token;

bool isDigit(char c){
    return c >= '0' && c <= '9';
}

bool is_operator(char c){
    return c == '+' || c == '-';
}

bool is_whitespace(char c){
    return c == ' ';
}

//sets the current token and advances the curr pos pointer
void get_next_token(char *text){
    char c = text[curr_pos];

    //ignoring whitespaces
    while(is_whitespace(c)){
        curr_pos += 1;
        c = text[curr_pos];
    }
    
    //is this char a digit? -> number token, else it's an operator
    if(isDigit(c)){
        int curr_number = c - '0';
        curr_pos += 1;
        while(isDigit(text[curr_pos])){
            curr_number *= 10;
            curr_number += (text[curr_pos] - '0');
            curr_pos += 1;
        }
        curr_token.type = INT;
        curr_token.number_value = curr_number;
        curr_token.operator_value = ' ';
    }
    else if(is_operator(c)){
        curr_token.type = OPERATOR;
        curr_token.number_value = 0;
        curr_token.operator_value = c;
    }
    else{
        printf("Invalid token recieved at pos %lld: %c\n", curr_pos, c);
        exit(-1);
    }
    curr_pos += 1;
}

//represents the token as string in given string s
void token_str(Token token, char *s){

    sprintf(s, "Token (type=%s, number_value=%d, operator_value=%c)", token.type == INT ? INT_STR : OPERATOR_STR, token.number_value, token.operator_value);
}

void parse(int type){
    
    if(curr_token.type != type){
        char s[100];
        token_str(curr_token, s);
        printf("Syntax Error at pos %lld: Expected token of type %s, but recieved token %s\n", curr_pos, type == INT ? INT_STR : OPERATOR_STR, s);
        exit(-1);
    }
}

int interpret(char *text){
    int result;
    //from the ip text, create tokens -> tokenizer (reads from texts to tokens)
    get_next_token(text);   //expected to be an integer
    parse(INT);
    result = curr_token.number_value;

    size_t len = strlen(text) - 1;
    while(curr_pos < len){
        get_next_token(text);
        parse(OPERATOR);
        char operator = curr_token.operator_value;
        get_next_token(text);
        parse(INT);
        int operand = curr_token.number_value;

        if(operator == '+')
            result += operand;
        else
            result -= operand;
    }
    return result;
}

int main(){
    char s[INPUT_LENGTH];
    memset(s, 0, INPUT_LENGTH);
    printf("Interpreter running...\n");
    int result;
    while(true){
        curr_pos = 0;
        printf(">>> ");
        fgets(s, INPUT_LENGTH, stdin);
        result = interpret(s);
        printf("%d\n", result);
    }
    return 0;
}