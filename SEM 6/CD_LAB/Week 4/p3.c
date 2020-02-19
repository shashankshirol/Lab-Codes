#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "new_lexan.c"

#define MAX 200

char str[30];
Token lookahead;
Token tkns[MAX];
int tkn_count = 0;
int start = 0;

char first_E[20][5] = {"(", "id" ,"\0"};
char follow_E[20][5] = {"$", ")", "\0"};

char first_Ed[20][5] = {"+", "@", "\0"};
char follow_Ed[20][5] = {"$", ")", "\0"};

char first_T[20][5] = {"(", "id", "\0"};
char follow_T[20][5] = {"+", ")", "$", "\0"};

char first_Td[20][5] = {"*", "@", "\0"};
char follow_Td[20][5] = {"+", ")", "$", "\0"};

char first_F[20][5] = {"(", "id", "\0"};
char follow_F[20][5] = {"*", "+", ")", "$", "\0"};

Token genToken(){
    return tkns[start++];
}

int inFirst(char *str, char t){
    int i = 0;
    if(t == 'E'){
        while(strcmp(first_E[i], "\0") != 0){
            if(strcmp(str, first_E[i]) == 0)
                return 1;
            i++;
        }
        return 0;
    }
    else if(t == 'T'){
        while(strcmp(first_T[i], "\0") != 0){
            if(strcmp(str, first_T[i]) == 0)
                return 1;
            i++;
        }
        return 0;
    }
    else if(t == 'F'){
        while(strcmp(first_F[i], "\0") != 0){
            if(strcmp(str, first_F[i]) == 0)
                return 1;
            i++;
        }
        return 0;
    }
    else if(t == '-'){
        while(strcmp(first_Ed[i], "\0") != 0){
            if(strcmp(str, first_Ed[i]) == 0)
                return 1;
            i++;
        }
        return 0;
    }
    else{
        while(strcmp(first_Td[i], "\0") != 0){
            if(strcmp(str, first_Td[i]) == 0)
                return 1;
            i++;
        }
        return 0;
    }
}

int inFollow(char *str, char t){
    int i = 0;
    if(t == 'E'){
        while(strcmp(follow_E[i], "\0") != 0){
            if(strcmp(str, follow_E[i]) == 0)
                return 1;
            i++;
        }
        return 0;
    }
    else if(t == 'T'){
        while(strcmp(follow_T[i], "\0") != 0){
            if(strcmp(str, follow_T[i]) == 0)
                return 1;
            i++;
        }
        return 0;
    }
    else if(t == 'F'){
        while(strcmp(follow_F[i], "\0") != 0){
            if(strcmp(str, follow_F[i]) == 0)
                return 1;
            i++;
        }
        return 0;
    }
    else if(t == '-'){
        while(strcmp(follow_Ed[i], "\0") != 0){
            if(strcmp(str, follow_Ed[i]) == 0)
                return 1;
            i++;
        }
        return 0;  
    }
    else{
        while(strcmp(follow_Td[i], "\0") != 0){
            if(strcmp(str, follow_Td[i]) == 0)
                return 1;
            i++;
        }
        return 0;
    }
}

void E();
void Ed();
void T();
void Td();
void F();

void E(){
    lookahead = genToken();
    if(inFirst(lookahead.lexname, 'E')){
        start--;
        T();
        Ed();
    }
    else{
        printf("Error in E -- Expected (, id\n");
        exit(1);
    }
}

void Ed(){
    lookahead = genToken();
    if(inFirst(lookahead.lexname, '-')){
        T();
        Ed();
    }
    else if(inFollow(lookahead.lexname, '-')){
        //do nothing
        start--;
    }
    else{
        printf("Error in Ed -- Expected +\n");
        exit(1);
    }
}

void T(){
    lookahead = genToken();
    if(inFirst(lookahead.lexname, 'T')){
        start--;
        F();
        Td();
    }
    else{
        printf("Error in T -- Expected (, id\n");
        exit(1);
    }
}

void Td(){
    lookahead = genToken();
    if(inFirst(lookahead.lexname, '?')){
        F();
        Td();
    }
    else if(inFollow(lookahead.lexname, '?')){
        //do nothing
        start--;
    }
    else{
        printf("Error in Td -- Expected *, current: %s\n", lookahead.lexname);
        exit(1);
    }
}

void F(){
    lookahead = genToken();
    if(inFirst(lookahead.lexname, 'F')){
        if(strcmp(lookahead.lexname, "(") == 0){
            E();
            lookahead = genToken();
            if(strcmp(lookahead.lexname, ")") != 0){
                printf("Error in F -- Expected )\n");
                exit(1);
            }
        }
    }
    else{
        printf("Error in F -- Expected (, id\n");
        exit(1);
    }
}

int main(){
    char c;
    FILE *fp = fopen("h.c", "r");
    //Ignores preprop
    c = fgetc(fp);

    while(c != EOF){
        char buff[30];
        int i = 0;

        while (c == ' ' || c == '\t')
            c = fgetc(fp);

        while (delimiter(c) == 0 && c != EOF)
        {
            buff[i++] = c;
            c = fgetc(fp);
        }
        
        buff[i] = '\0';
        Token t = getNextToken(buff, 0, 0);
        
        if(t.row != -1)
            tkns[tkn_count++] = t;

        if (delimiter(c) && c != ' ' && c != '\n')
        {
            char tempb[2];
            tempb[0] = c;
            tempb[1] = '\0';
            Token temp = getNextToken(tempb, 0, 0);
            tkns[tkn_count++] = temp;
        }

        c = fgetc(fp);
    }
    for(int i = 0; i<tkn_count; i++)
        printf("%s\t", tkns[i].lexname);
    printf("\n");

    E();
    lookahead = genToken();
    if(strcmp(lookahead.lexname, "$") == 0){
            printf("Success\n");
            exit(0);
    }
    else{
        printf("Error -- Expected $\n");
        exit(1);
    }
}