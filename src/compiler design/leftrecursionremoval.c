#include <stdio.h>
#include <string.h>

int main() {
    char nt;//ye variable nt use kar raha hai non-terminal ko store karne ke liye, jiske liye left recursion remove karna hai
    int n;//ye variable n use kar raha hai number of productions ko store karne ke liye, jise user input karega

    printf("Enter Non-Terminal: ");
    scanf(" %c", &nt);

    printf("Enter number of productions: ");
    scanf("%d", &n);

    char prod[20][50];
    char alpha[20][50];//ye array alpha use kar raha hai left recursive productions ke right hand side ko store karne ke liye, jisme se nt non-terminal ke baad ka part store hoga
    char beta[20][50];//ye array beta use kar raha hai non-left recursive productions ke right hand side ko store karne ke liye, jisme se nt non-terminal ke baad ka part store hoga

    int a = 0, b = 0;//

    printf("Enter productions (without %c->)\n", nt);

    for(int i = 0; i < n; i++) {
        scanf("%s", prod[i]);

        if(prod[i][0] == nt) {
            strcpy(alpha[a++], prod[i] + 1);
        } else {
            strcpy(beta[b++], prod[i]);
        }
    }

    if(a == 0) {
        printf("\nNo Left Recursion Found.\n");
        return 0;
    }

    printf("\nGrammar after removing Left Recursion:\n");

    printf("%c -> ", nt);
    for(int i = 0; i < b; i++) {
        printf("%s%c'", beta[i], nt);
        if(i != b - 1)
            printf(" | ");
    }

    printf("\n");

    printf("%c' -> ", nt);
    for(int i = 0; i < a; i++) {
        printf("%s%c'", alpha[i], nt);
        if(i != a - 1)
            printf(" | ");
    }

    printf(" | ε\n");

    return 0;
}
