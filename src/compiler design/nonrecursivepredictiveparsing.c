#include<stdio.h>//ye header file input output ke liye use hota hai
#include<string.h>//ye header file string handling ke liye use hota hai jaise ki strcpy, strcat, strcmp functions ke liye
#include<ctype.h>//ye header file character handling ke liye use hota hai jaise ki isupper function ke liye

#define MAX 100

char prod[MAX][MAX];//stores productions jaise S->AB, 
char table[MAX][MAX][MAX];//store parsing table
char nt[MAX];//store non-terminals like E R T Y F
char terms[MAX];//store terminals like + * ( ) id
int ntCount = 0;//it store how many non-terminal is found in grammer
int termCount = 0;//it stores how many terminal is found in grammer
int n;//it use to store number of productions

int ntIdx(char c){//find index of non-terminal
    for(int i=0;i<ntCount;i++){
        if(nt[i] == c){
            return i;
        }
    }
    return -1;//agar nahi mila to -1 return kar dega
}

int termIdx(char c){//use to find index of terminal
    for(int i=0;i<termCount;i++){
        if(terms[i] == c){
            return i;
        }
    }
    return -1;//agar nahi mila to -1 return kar dega
}

void getNonTerminals(){

    for(int i=0;i<n;i++){

        char c = prod[i][0];//it takes left hand side of production

        if(ntIdx(c) == -1){//yaha humlog check kar rahe ki c non-terminal list me pehle se hai ya nahi
            nt[ntCount++] = c;//agar nahi hai to usko list me add kar do and ntCount ko increment kar do
        }
    }
}

void getTerminals(){//yaha humlog terminal find kar rahe hai same way me jaise non-terminal find kiya tha

    for(int i=0;i<n;i++){

        for(int j=2;prod[i][j];j++){

            char c = prod[i][j];

            if(!isupper(c) && c != '#'){//terminal wo hota hai jo uppercase nahi hota and wo epsilon bhi nahi hota

                if(termIdx(c) == -1){//check if terminal is already in list or not

                    terms[termCount++] = c;//if not found add to list and increment count
                }
            }
        }
    }

    /* Add $ */
    if(termIdx('$') == -1){//check if $ is already in list or not

        terms[termCount++] = '$';//if not found add to list and increment count
    }
}

/* Input parsing table manually */
void inputTable(){//yaha humlog manually parsing table ke entries le rahe hai user se

    printf("\nEnter Parsing Table Entries:\n");
    printf("Use # for epsilon and - for empty\n\n");

    for(int i=0;i<ntCount;i++){//yaha humlog har non-terminal ke liye table ke entries le rahe hai

        for(int j=0;j<termCount;j++){//yaha humlog har terminal ke liye table ke entries le rahe hai

            char temp[MAX];//yaha humlog temporary variable me user se input le rahe hai

            printf("M[%c,%c] = ",nt[i],terms[j]);//yaha humlog user se input le rahe hai for M[non-terminal,terminal] entry aur yaha i ko fix rakh kar j ko change kar rahe hai taki har terminal ke liye entry le sake

            scanf("%s",temp);//yaha humlog user se input le rahe hai

            if(strcmp(temp,"-") == 0){//agar user ne - diya hai to iska matlab hai ki table entry empty hai to humlog usko empty string se replace kar denge

                table[i][j][0] = '\0';//empty string

            } else {

                strcpy(table[i][j],temp);//agar user ne - nahi diya hai to iska matlab hai ki table entry me production hai to humlog usko table me copy kar denge
            }
        }
    }
}

/* Print parsing table */
/*void printTable(){//yaha humlog parsing table print kar rahe hai

    printf("\n\n--- LL(1) Parsing Table ---\n\n");

    printf("%-10s","");//print header for terminals

    for(int i=0;i<termCount;i++){//print terminal headers

        printf("%-15c",terms[i]);//print terminal header with some spacing
    }

    printf("\n");//new line after header

    for(int i=0;i<ntCount;i++){//yaha humlog har non-terminal ke liye table ke entries print kar rahe hai

        printf("%-10c",nt[i]);//print non-terminal header with some spacing

        for(int j=0;j<termCount;j++){//yaha humlog har terminal ke liye table ke entries print kar rahe hai

            if(table[i][j][0] == '\0'){//agar table entry empty hai to humlog - print karenge

                printf("%-15s","-");//print - for empty entry

            } else {

                char cell[MAX];//yaha humlog cell variable me M[non-terminal,terminal] = production format me string banayenge taki print karne me asani ho

                sprintf(cell,"%c->%s",nt[i],table[i][j]);//yaha humlog cell variable me M[non-terminal,terminal] = production format me string banayenge taki print karne me asani ho

                printf("%-15s",cell);//print the cell content with some spacing
                // printf("%c->%-11s",nt[i],table[i][j]);
            }
        }

        printf("\n");//new line after each non-terminal row
    }
}
*/
/* Print stack */
void printStack(char stack[],int top){//yaha humlog stack print kar rahe hai

    for(int i=top;i>=0;i--){//yaha humlog stack ke top se bottom tak print kar rahe hai

        printf("%c",stack[i]);//print stack element
    }
}

/* Predictive Parsing */
void predictiveParse(){//yaha humlog predictive parsing kar rahe hai using the parsing table
    char input[MAX];//yaha humlog input string le rahe hai user se
    printf("\nEnter input string: ");//yaha humlog user se input string ke liye prompt kar rahe hai
    scanf("%s",input);//yaha humlog user se input string le rahe hai
    strcat(input,"$");//yaha humlog input string ke end me $ add kar rahe hai taki parsing me asani ho
    char stack[MAX];//yaha humlog stack declare kar rahe hai parsing ke liye
    int top = -1;//yaha humlog stack ke top ko initialize kar rahe hai -1 se taki jab stack me element add kare to top 0 se start ho jaye
    /* Push $ */
    stack[++top] = '$';//yaha humlog stack me sabse pehle $ push kar rahe hai taki parsing ke end me jab input aur stack dono me $ aaye to humlog accept kar sake
    /* Push start symbol */
    stack[++top] = nt[0];//yaha humlog stack me start symbol push kar rahe hai jo ki nt array ka pehla element hota hai yani ki leftmost non-terminal hota hai
    int ip = 0;//yaha humlog input pointer ko initialize kar rahe hai 0 se taki input string ke first symbol se parsing start ho jaye
    printf("\n\n%-25s %-25s %-25s\n",
           "STACK",
           "INPUT",
           "ACTION");//yaha humlog parsing process ke liye headers print kar rahe hai stack, input aur action ke liye
    printf("--------------------------------------------------------------------------\n");
    while(top >= 0){//yaha humlog tab tak parsing process chalate rahenge jab tak stack empty nahi ho jata
        char stackTop = stack[top];//yaha humlog stack ke top element ko variable me store kar rahe hai taki uske basis par action decide kar sake
        char currentInput = input[ip];//yaha humlog current input symbol ko variable me store kar rahe hai taki uske basis par action decide kar sake
        /* Print stack */
        printStack(stack,top);//yaha humlog current stack ko print kar rahe hai
        printf("\t\t");//spacing between stack and input
        /* Print remaining input */
        printf("%s",input + ip);//yaha humlog remaining input ko print kar rahe hai yani ki input string me se current input symbol se lekar end tak ka part print kar rahe hai
        printf("\t\t");//spacing between input and action
        /*
            ACCEPT
        */
        if(stackTop == '$' && currentInput == '$'){//agar stack ke top me $ hai aur current input bhi $ hai to iska matlab hai ki parsing successful hai aur humlog ACCEPT print karenge
            printf("ACCEPT\n");
            break;
        }
        /*
            TERMINAL
        */
        if(!isupper(stackTop)){//agar stack ke top me terminal symbol hai to humlog check karenge ki wo current input symbol ke barabar hai ya nahi
            if(stackTop == currentInput){//agar stack ke top me terminal symbol hai to humlog check karenge ki wo current input symbol ke barabar hai ya nahi
                printf("Match %c\n",currentInput);//agar stack ke top me terminal symbol hai to humlog check karenge ki wo current input symbol ke barabar hai ya nahi, agar match karta hai to humlog Match print karenge aur stack se us terminal ko pop kar denge aur input pointer ko next symbol pe move kar denge
                top--;//pop the terminal from stack
                ip++;//input pointer ko next symbol pe move kar denge
            } else {
                printf("ERROR\n");//agar stack ke top me terminal symbol hai to humlog check karenge ki wo current input symbol ke barabar hai ya nahi, agar match nahi karta hai to iska matlab hai ki parsing error hai aur humlog ERROR print karenge
                break;//isko humlog T-conflict bolte hai jisme terminal symbol stack ke top me hai lekin wo current input symbol se match nahi kar raha hai, is case me parsing fail ho jati hai
            }
        }
        /*
            NON-TERMINAL
        */
        else {
            int nti = ntIdx(stackTop);//yaha humlog stack ke top me jo non-terminal symbol hai uska index find kar rahe hai nt array me
            int ti = termIdx(currentInput);//yaha humlog current input symbol ka index find kar rahe hai terms array me
            if(nti == -1 || ti == -1){//agar non-terminal ya terminal ka index -1 hai to iska matlab hai ki wo symbol grammar me defined nahi hai to humlog ERROR print karenge
                printf("ERROR\n");//agar non-terminal ya terminal ka index -1 hai to iska matlab hai ki wo symbol grammar me defined nahi hai to humlog ERROR print karenge
                break;
            }
            /*
                Empty table entry
            */
            if(table[nti][ti][0] == '\0'){//agar parsing table me M[non-terminal,terminal] entry empty hai to iska matlab hai ki parsing error hai aur humlog ERROR print karenge
                printf("ERROR\n");//N-conflict bolte hai is case me jisme non-terminal symbol stack ke top me hai lekin parsing table me us non-terminal aur current input terminal ke liye entry empty hai, is case me parsing fail ho jati hai
                break;
            }
            char production[MAX];//yaha humlog production variable me parsing table se M[non-terminal,terminal] entry ko copy kar rahe hai taki usko use karke stack me push kar sake
            strcpy(production,table[nti][ti]);//yaha humlog production variable me parsing table se M[non-terminal,terminal] entry ko copy kar rahe hai taki usko use karke stack me push kar sake
            printf("%c->%s\n",stackTop,production);//yaha humlog action ke part me M[non-terminal,terminal] = production format me action print kar rahe hai taki parsing process ko samajhne me asani ho
            /* Pop NT */
            top--;//yaha humlog stack se non-terminal ko pop kar rahe hai kyunki ab humlog usko expand karne wale hai using the production we got from parsing table
            /*
                If epsilon,
                push nothing
            */
            if(strcmp(production,"#") != 0){//agar production epsilon nahi hai to hi humlog usko stack me push karenge, agar production epsilon hai to iska matlab hai ki humlog stack me kuch bhi push nahi karenge aur simply non-terminal ko pop kar denge
                /*
                    Push RHS in reverse
                */
                for(int i=strlen(production)-1;i>=0;i--){//yaha humlog production ke right hand side ko reverse order me stack me push kar rahe hai taki parsing process me leftmost symbol top pe aaye
                    stack[++top] = production[i];//yaha humlog production ke right hand side ko reverse order me stack me push kar rahe hai taki parsing process me leftmost symbol top pe aaye
                }
            }
        }
    }
}

int main(){

    printf("Enter number of productions: ");//yaha humlog user se number of productions ke liye prompt kar rahe hai

    scanf("%d",&n);

    printf("\nEnter productions (Example: E=TR)\n\n");//yaha humlog user se productions ke liye prompt kar rahe hai aur user ko example bhi de rahe hai ki production ka format kaisa hona chahiye

    for(int i=0;i<n;i++){

        scanf("%s",prod[i]);//yaha humlog user se productions le rahe hai aur unko prod array me store kar rahe hai, prod[i] me i-th production store ho jayegi
    }

    getNonTerminals();//yaha humlog non-terminals find kar rahe hai using the productions we got from user input

    getTerminals();//yaha humlog terminals find kar rahe hai using the productions we got from user input

    inputTable();//yaha humlog manually parsing table ke entries le rahe hai user se

    //printTable();//yaha humlog parsing table print kar rahe hai taki user ko pata chal jaye ki usne jo entries di hai wo sahi hai ya nahi

    predictiveParse();//yaha humlog predictive parsing kar rahe hai using the parsing table

    return 0;
}