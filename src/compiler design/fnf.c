#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
#define MAX 100

char prod[MAX][MAX];
int n;
char first[MAX][MAX];
char follow[MAX][MAX];
char nt[MAX];
int ntCount = 0;

int ntIdx(char c){
    for(int i=0;i<ntCount;i++){
        if(nt[i] == c){
            return i;
        }
    }
    return -1;
}

void getNonTerminals(){
    for(int i=0;i<n;i++){
        if(ntIdx(prod[i][0]) == -1){
            nt[ntCount++] = prod[i][0];
        }
    }
}

int contains(char set[],char c){
    for(int i=0;set[i];i++){
        if(set[i] == c){
            return 1;
        }
    }
    return 0;
}

int add(char set[],char c){
    if(!contains(set,c)){
        int len = strlen(set);
        set[len] = c;
        set[len+1] = '\0';
        return 1;
    }
    return 0;
}

void computeFirst(){
    for(int i=0;i<n;i++){
        char A = prod[i][0];
        char B = prod[i][3];

        if(!isupper(B)){
            int Aidx = ntIdx(A);
            add(first[Aidx],B);
        }
    }
    int changed = 1;
    while(changed){
        changed = 0;
        for(int i=0;i<n;i++){
            char A = prod[i][0];
            int Aidx = ntIdx(A);

            char *rhs = prod[i]+3;
            int hasEps = 1;
            for(int m=0;rhs[m] && hasEps;m++){
                hasEps = 0;
                char X = rhs[m];
                int Xidx = ntIdx(X);
                if(isupper(X)){
                    for(int l=0;first[Xidx][l];l++){
                        if(first[Xidx][l] == '#'){
                            hasEps = 1;
                        }else{
                            if(add(first[Aidx],first[Xidx][l])){
                                changed = 1;
                            }
                        }
                    }
                }else{
                    if(X == '#'){
                        hasEps = 1;
                    }else{
                        if(add(first[Aidx],X)){
                            changed = 1;
                        }
                    }
                }
            }
            if(hasEps){
                if(add(first[Aidx],'#')){
                    changed = 1;
                }
            }
        }
    }
}

void printFirst(){
    for(int i=0;i<ntCount;i++){
        printf("first(%c) = { ",nt[i]);
        for(int j=0;first[i][j];j++){
            printf("%c ",first[i][j]);
        }
        printf("}\n");
    }
}

void computeFollow(){
    add(follow[ntIdx(prod[0][0])],'$');
    int changed = 1;
    while(changed){
        changed = 0;
        for(int i=0;i<n;i++){
            char A = prod[i][0];
            int Aidx = ntIdx(A);

            for(int j=3;prod[i][j];j++){
                char X = prod[i][j];
                if(isupper(X)){
                    int Xidx = ntIdx(X);
                    int hasEps = 1;
                    int k = j+1;
                    while(prod[i][k] != '\0' && hasEps){
                        hasEps = 0;
                        char B = prod[i][k];
                        if(isupper(B)){
                            int Bidx = ntIdx(B);
                            for(int l=0;first[Bidx][l];l++){
                                if(first[Bidx][l] == '#'){
                                    hasEps = 1;
                                }else{
                                    if(add(follow[Xidx],first[Bidx][l])){
                                        changed = 1;
                                    }
                                }
                            }
                        }else{
                            if(B == '#'){
                                hasEps = 1;
                            }else{
                                if(add(follow[Xidx],B)){
                                    changed = 1;
                                }
                            }
                        }
                        k++;
                    }
                    if(hasEps){
                        for(int f=0;follow[Aidx][f];f++){
                            if(add(follow[Xidx],follow[Aidx][f])){
                                changed = 1;
                            }
                        }
                    }
                }
            }
        }
    }
}

void printFollow(){
    for(int i=0;i<ntCount;i++){
        printf("follow(%c) = { ",nt[i]);
        for(int j=0;follow[i][j];j++){
            printf("%c ",follow[i][j]);
        }
        printf("}\n");
    }
}

int main(){
    printf("Enter number of productions: ");
    scanf("%d",&n);

    for(int i=0;i<n;i++){
        scanf("%s",prod[i]);
    }

    getNonTerminals();

    computeFirst();
    printFirst();

    computeFollow();
    printFollow();
    return 0;
}




#include<stdio.h>
#include<string.h>
#define MAX 100

char prod[MAX][MAX];
int n;
int ntCount = 0;

void printGrammer(){
    for(int i=0;i<n;i++){
        printf("%s\n",prod[i]);
    }
}

int isNonTerminal(char c){
    return c>='A' && c<='Z';
}

void substitute(char Ai,char Aj){
    char newProd[MAX][MAX];
    int newCount = 0;

    for(int i=0;i<n;i++){
        if(prod[i][0] != Ai){
            strcpy(newProd[newCount++],prod[i]);
            continue;
        }
        char *rhs = prod[i]+2;
        if(rhs[0] == Aj){
            char rem[MAX];
            strcpy(rem,rhs+1);
            for(int j=0;j<n;j++){
                if(prod[j][0] == Aj){
                    char temp[MAX];
                    strcpy(temp,prod[j]+2);
                    strcat(temp,rem);
                    sprintf(newProd[newCount++],"%c=%s",Ai,temp);
                }
            }
        }else{ 
            strcpy(newProd[newCount++],prod[i]);
        }
    }
    n = newCount;
    for(int i=0;i<n;i++){
        strcpy(prod[i],newProd[i]);
    }
}

void removeDirect(char A){
    char alpha[MAX][MAX];
    char beta[MAX][MAX];
    int ac=0,bc=0;

    for(int i=0;i<n;i++){
        if(prod[i][0] != A){
            continue;
        }
        char *rhs = prod[i]+2;
        if(rhs[0] == A){
            strcpy(alpha[ac++],rhs+1);
        }else{
            strcpy(beta[bc++],rhs);
        }
    }
    if(ac == 0){
        printf("%c -> ",A);
        for(int j=0;j<bc;j++){
            printf("%s",beta[j]);
            if(j!=bc-1) printf(" | ");
        }
        printf("\n");
    }else{
    // printf("Removing Left Recursion for %c\n",A);
        printf("%c -> ",A);
        if(bc == 0){
            printf("%c'",A);
        }else{
            for(int i=0;i<bc;i++){
                printf("%s%c'",beta[i],A);
                if(i != bc-1){
                    printf(" | ");
                }
            }
            printf("\n");
        }
        printf("%c' -> ",A);
        for(int i=0;i<ac;i++){
            printf("%s%c'",alpha[i],A);
            if(i != bc-1){
                printf(" | ");
            }
        }
        printf(" # \n");
    }
}

int main(){
    printf("Enter number of productions: ");
    scanf("%d",&n);

    for(int i=0;i<n;i++){
        scanf("%s",prod[i]);
    }

    char nt[26];
    for(int i=0;i<n;i++){
        int found = 0;
        for(int j=0;j<ntCount;j++){
            if(nt[j] == prod[i][0]){
                found = 1;
                break;
            }
        }
        if(!found){
            nt[ntCount++] = prod[i][0];
        }
    }

    for(int i=0;i<ntCount;i++){
        char Ai = nt[i];
        for(int j=0;j<i;j++){
            char Aj = nt[j];
            substitute(Ai,Aj);
        }
        removeDirect(Ai);
    }
    // printGrammer();
    return 0;
}


#include<stdio.h>
#include<ctype.h>
#include<string.h>
#define MAX 100

int n;
char prod[MAX][MAX];
char first[MAX][MAX];
char follow[MAX][MAX];
char table[MAX][MAX][MAX];
char nt[MAX];
int ntCount = 0;
char terms[MAX];
int termCount = 0;

int ntIdx(char c){
    for(int i=0;nt[i];i++){
        if(nt[i] == c){
            return i;
        }
    }
    return -1;
}

int termIdx(char c){
    for(int i=0;terms[i];i++){
        if(terms[i] == c){
            return i;
        }
    }
    return -1;
}

void getNonTerminals(){
    for(int i=0;i<n;i++){
        char c = prod[i][0];
        if(ntIdx(c) == -1){
            nt[ntCount++] = c;
        }
    }
}

void getTerminals(){
    for(int i=0;i<n;i++){
        for(int j=2;prod[i][j];j++){
            char c = prod[i][j];
            if(!isupper(c) && c != '#'){
                int found = 0;
                for(int k=0;terms[k];k++){
                    if(terms[k] == c){
                        found = 1;
                        break;
                    }
                }
                if(!found) terms[termCount++] = c;
            }
        }
    }
    int found = 0;
    for(int k=0;terms[k];k++){
        if(terms[k] == '$'){
            found = 1;
            break;
        }
    }
    if(!found){
        terms[termCount++] = '$';
    }
}

void buildTable(){
    // Empty table
    for(int i=0;i<ntCount;i++){
        for(int j=0;j<termCount;j++){
            table[i][j][0] = '\0';
        }
    }

    // Reach to every non terminal
    for(int i=0;i<ntCount;i++){
        // pick each non terminal
        char A = nt[i];
        // go to each production
        for(int j=0;j<n;j++){
            // check if production non termnal matches
            if(prod[j][0] != A) continue;

            // take complete rhs
            char *rhs = prod[j] + 2;
            // to take first of rhs

            // assume
            int rhsCanBeEps = 1;

            // check every rhs symbol
            for(int m=0;rhs[m] && rhsCanBeEps;m++){
                char X = rhs[m]; 
                rhsCanBeEps = 0;
                
                if(isupper(X)){
                    int Xi = ntIdx(X);
                    int hasEps = 0;
                    for(int p=0;first[Xi][p];p++){
                        char firstSym = first[Xi][p];
                        // If first is not eps
                        if(firstSym != '#'){
                            int ti = termIdx(firstSym);
                            if(table[i][ti][0] == '\0'){
                                strcpy(table[i][ti],rhs);
                            }else{
                                strcat(table[i][ti],"/");
                                strcat(table[i][ti],rhs);
                            }
                        }
                        if(firstSym == '#'){
                            hasEps = 1;
                        }
                    }
                    if(hasEps){
                        rhsCanBeEps = 1;
                    }
                } else{
                    if(X == '#'){
                        rhsCanBeEps = 1;
                    }else{
                        int ti = termIdx(X);
                        if(table[i][ti][0] == '\0'){
                            strcpy(table[i][ti],rhs);
                        } else{
                            strcat(table[i][ti],"/");
                            strcat(table[i][ti],rhs);
                        }
                    }
                }
            }
            
            if(rhsCanBeEps){
                for(int f=0;follow[i][f];f++){
                    char b = follow[i][f];
                    int ti = termIdx(b);

                    if(table[i][ti][0] == '\0'){
                        strcpy(table[i][ti],rhs);
                    }else{
                        strcat(table[i][ti],"/");
                        strcat(table[i][ti],rhs);
                    }
                }
            }
        }
    }
}

void printTable(){
    printf("%-8s","NT");
    for(int i=0;i<termCount;i++){
        printf("%-12c",terms[i]);
    }
    printf("\n");

    for(int i=0;i<ntCount;i++){
        printf("%-8c",nt[i]);
        for(int j=0;j<termCount;j++){
            if(table[i][j][0] == '\0'){
                printf("%-14s","-");
            }else{
                printf("%c->%-11s",nt[i],table[i][j]);
            }
        }
        printf("\n");
    }
}

int main(){
    printf("Enter number of productions: ");
    scanf("%d",&n);

    for(int i=0;i<n;i++){
        scanf("%s",prod[i]);
    }

    getNonTerminals();
    getTerminals();

    printf("Enter first sets: \n");
    for(int i=0;i<ntCount;i++){
        printf("first(%c) = ",nt[i]);
        scanf("%s",first[i]);
    }

    printf("Enter follow sets: \n");
    for(int i=0;i<ntCount;i++){
        printf("follow(%c) = ",nt[i]);
        scanf("%s",follow[i]);
    }

    buildTable();
    printTable();
    return 0;
}




#include<stdio.h>
#include<string.h>
#include<ctype.h>

#define MAX 100

char prod[MAX][MAX];
char table[MAX][MAX][MAX];

char nt[MAX];
char terms[MAX];

int ntCount = 0;
int termCount = 0;

int n;

int ntIdx(char c){
    for(int i=0;i<ntCount;i++){
        if(nt[i] == c){
            return i;
        }
    }
    return -1;
}

int termIdx(char c){
    for(int i=0;i<termCount;i++){
        if(terms[i] == c){
            return i;
        }
    }
    return -1;
}

void getNonTerminals(){

    for(int i=0;i<n;i++){

        char c = prod[i][0];

        if(ntIdx(c) == -1){
            nt[ntCount++] = c;
        }
    }
}

void getTerminals(){

    for(int i=0;i<n;i++){

        for(int j=2;prod[i][j];j++){

            char c = prod[i][j];

            if(!isupper(c) && c != '#'){

                if(termIdx(c) == -1){

                    terms[termCount++] = c;
                }
            }
        }
    }

    /* Add $ */
    if(termIdx('$') == -1){

        terms[termCount++] = '$';
    }
}

/* Input parsing table manually */
void inputTable(){

    printf("\nEnter Parsing Table Entries:\n");
    printf("Use # for epsilon and - for empty\n\n");

    for(int i=0;i<ntCount;i++){

        for(int j=0;j<termCount;j++){

            char temp[MAX];

            printf("M[%c,%c] = ",nt[i],terms[j]);

            scanf("%s",temp);

            if(strcmp(temp,"-") == 0){

                table[i][j][0] = '\0';

            } else {

                strcpy(table[i][j],temp);
            }
        }
    }
}

/* Print parsing table */
void printTable(){

    printf("\n\n--- LL(1) Parsing Table ---\n\n");

    printf("%-10s","");

    for(int i=0;i<termCount;i++){

        printf("%-15c",terms[i]);
    }

    printf("\n");

    for(int i=0;i<ntCount;i++){

        printf("%-10c",nt[i]);

        for(int j=0;j<termCount;j++){

            if(table[i][j][0] == '\0'){

                printf("%-15s","-");

            } else {

                char cell[MAX];

                sprintf(cell,"%c->%s",nt[i],table[i][j]);

                printf("%-15s",cell);
                // printf("%c->%-11s",nt[i],table[i][j]);
            }
        }

        printf("\n");
    }
}

/* Print stack */
void printStack(char stack[],int top){

    for(int i=top;i>=0;i--){

        printf("%c",stack[i]);
    }
}

/* Predictive Parsing */
void predictiveParse(){
    char input[MAX];
    printf("\nEnter input string: ");
    scanf("%s",input);
    strcat(input,"$");
    char stack[MAX];
    int top = -1;
    /* Push $ */
    stack[++top] = '$';
    /* Push start symbol */
    stack[++top] = nt[0];
    int ip = 0;
    printf("\n\n%-25s %-25s %-25s\n",
           "STACK",
           "INPUT",
           "ACTION");
    printf("--------------------------------------------------------------------------\n");
    while(top >= 0){
        char stackTop = stack[top];
        char currentInput = input[ip];
        /* Print stack */
        printStack(stack,top);
        printf("\t\t");
        /* Print remaining input */
        printf("%s",input + ip);
        printf("\t\t");
        /*
            ACCEPT
        */
        if(stackTop == '$' && currentInput == '$'){
            printf("ACCEPT\n");
            break;
        }
        /*
            TERMINAL
        */
        if(!isupper(stackTop)){
            if(stackTop == currentInput){
                printf("Match %c\n",currentInput);
                top--;
                ip++;
            } else {
                printf("ERROR\n");
                break;
            }
        }
        /*
            NON-TERMINAL
        */
        else {
            int nti = ntIdx(stackTop);
            int ti = termIdx(currentInput);
            if(nti == -1 || ti == -1){
                printf("ERROR\n");
                break;
            }
            /*
                Empty table entry
            */
            if(table[nti][ti][0] == '\0'){
                printf("ERROR\n");
                break;
            }
            char production[MAX];
            strcpy(production,table[nti][ti]);
            printf("%c->%s\n",stackTop,production);
            /* Pop NT */
            top--;
            /*
                If epsilon,
                push nothing
            */
            if(strcmp(production,"#") != 0){
                /*
                    Push RHS in reverse
                */
                for(int i=strlen(production)-1;i>=0;i--){
                    stack[++top] = production[i];
                }
            }
        }
    }
}

int main(){

    printf("Enter number of productions: ");

    scanf("%d",&n);

    printf("\nEnter productions (Example: E=TR)\n\n");

    for(int i=0;i<n;i++){

        scanf("%s",prod[i]);
    }

    getNonTerminals();

    getTerminals();

    inputTable();

    printTable();

    predictiveParse();

    return 0;
}
