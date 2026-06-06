#include<stdio.h>//ye header file input output ke liye use hota hai
#include<ctype.h>//ye header file character handling ke liye use hota hai jaise ki isupper function ke liye
#include<string.h>//ye header file string handling ke liye use hota hai jaise ki strcpy, strcat, strcmp functions ke liye
#define MAX 100//ye constant define kar raha hai MAX ko 100 ke barabar, iska use humlog array ke size ke liye kar rahe hai

int n;//ye variable n use kar raha hai number of productions ko store karne ke liye
char prod[MAX][MAX];//ye 2D array prod use kar raha hai productions ko store karne ke liye, jisme MAX number of productions aur MAX length of each production ko represent karta hai
char first[MAX][MAX];//ye 2D array first use kar raha hai first sets ko store karne ke liye, jisme MAX number of non-terminals aur MAX number of symbols in first set ko represent karta hai
char follow[MAX][MAX];//ye 2D array follow use kar raha hai follow sets ko store karne ke liye, jisme MAX number of non-terminals aur MAX number of symbols in follow set ko represent karta hai
char table[MAX][MAX][MAX];//ye 3D array table use kar raha hai parsing table ko store karne ke liye, jisme MAX number of non-terminals, MAX number of terminals aur MAX length of production ko represent karta hai
char nt[MAX];//ye array nt use kar raha hai non-terminals ko store karne ke liye
int ntCount = 0;//ye variable ntCount use kar raha hai number of non-terminals ko store karne ke liye
char terms[MAX];//ye array terms use kar raha hai terminals ko store karne ke liye
int termCount = 0;//ye variable termCount use kar raha hai number of terminals ko store karne ke liye

int ntIdx(char c){//ye function ntIdx use kar raha hai non-terminal ka index find karne ke liye nt array me, agar nahi mila to -1 return karega
    for(int i=0;nt[i];i++){//ye loop nt array me iterate kar raha hai jab tak null character nahi milta, iska matlab hai ki jitne non-terminals hai unke liye loop chalega
        if(nt[i] == c){//agar nt array me c non-terminal milta hai to uska index return kar dega
            return i;//agar nt array me c non-terminal milta hai to uska index return kar dega
        }
    }
    return -1;//agar nt array me c non-terminal nahi milta hai to -1 return kar dega
}

int termIdx(char c){//ye function termIdx use kar raha hai terminal ka index find karne ke liye terms array me, agar nahi mila to -1 return karega
    for(int i=0;terms[i];i++){//ye loop terms array me iterate kar raha hai jab tak null character nahi milta, iska matlab hai ki jitne terminals hai unke liye loop chalega
        if(terms[i] == c){//agar terms array me c terminal milta hai to uska index return kar dega
            return i;//agar terms array me c terminal milta hai to uska index return kar dega
        }
    }
    return -1;//agar terms array me c terminal nahi milta hai to -1 return kar dega
}

void getNonTerminals(){//ye function getNonTerminals use kar raha hai non-terminals ko find karne ke liye productions ke left hand side se, agar non-terminal pehle se nt array me nahi hai to usko add kar dega
    for(int i=0;i<n;i++){//ye loop productions me iterate kar raha hai
        char c = prod[i][0];//ye variable c me production ke left hand side ka non-terminal store kar raha hai
        if(ntIdx(c) == -1){//agar c non-terminal nt array me pehle se nahi hai to usko nt array me add kar dega aur ntCount ko increment kar dega
            nt[ntCount++] = c;
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
void buildTable(){//ye function buildTable use kar raha hai parsing table ko build karne ke liye using the productions, first sets aur follow sets, sabse pehle table ko empty kar dega, phir har non-terminal ke liye uske productions ke right hand side ke first symbol ke hisab se table me entry karega, agar first symbol epsilon hai to follow set ke symbols ke hisab se table me entry karega
    // Empty table
    for(int i=0;i<ntCount;i++){//ye loop non-terminals ke liye iterate kar raha hai
        for(int j=0;j<termCount;j++){//ye loop terminals ke liye iterate kar raha hai
            table[i][j][0] = '\0'; //ye line table ke har entry ko empty string se initialize kar rahi hai, iska matlab hai ki initially parsing table me koi entry nahi hai
        }
    }

    // Reach to every non terminal
    for(int i=0;i<ntCount;i++){//ye loop non-terminals ke liye iterate kar raha hai
        // pick each non terminal
        char A = nt[i];//ye variable A me current non-terminal store kar raha hai
        // go to each production
        for(int j=0;j<n;j++){//ye loop productions ke liye iterate kar raha hai
            // check if production non termnal matches
            if(prod[j][0] != A) continue;//agar production ke left hand side ka non-terminal current non-terminal A se match nahi karta hai to us production ko skip kar dega aur ussi loop ke next iteration me chala jayega, iska matlab hai ki humlog us production ko consider nahi kar rahe hai kyunki wo current non-terminal A ke liye production nahi hai

            // take complete rhs
            char *rhs = prod[j] + 2;//ye pointer rhs me production ke right hand side ka string store kar raha hai, prod[j] + 2 se start kar raha hai kyunki prod[j][0] non-terminal hai aur prod[j][1] me '->' hai, to right hand side ka first symbol prod[j][2] se start hota hai
            // to take first of rhs

            // assume
            int rhsCanBeEps = 1;//ye variable rhsCanBeEps use kar raha hai check karne ke liye ki right hand side epsilon ho sakta hai ya nahi, initially assume kar raha hai ki right hand side epsilon ho sakta hai, agar right hand side me koi symbol aisa milta hai jo epsilon nahi ho sakta to is variable ko 0 kar dega

            // check every rhs symbol
            for(int m=0;rhs[m] && rhsCanBeEps;m++){//ye loop right hand side ke symbols ke liye iterate kar raha hai jab tak null character nahi milta, iska matlab hai ki jitne symbols hai unke liye loop chalega, aur rhsCanBeEps check kar raha hai ki abhi tak right hand side epsilon ho sakta hai ya nahi
                char X = rhs[m]; //ye variable X me current symbol store kar raha hai right hand side ka current symbol store kar raha hai right hand side ka
                rhsCanBeEps = 0;//ye line rhsCanBeEps ko 0 kar rahi hai, iska matlab hai ki abhi tak assume kar rahe the ki right hand side epsilon ho sakta hai, lekin ab humlog current symbol X ko check karne wale hai, agar X epsilon ho sakta hai to rhsCanBeEps ko wapas 1 kar denge, lekin agar X epsilon nahi ho sakta to rhsCanBeEps 0 hi rahega
                
                if(isupper(X)){//agar X non-terminal hai to uske first set ke symbols ke hisab se table me entry karenge, agar first set me epsilon hai to rhsCanBeEps ko 1 kar denge
                    int Xi = ntIdx(X);//ye variable Xi me X non-terminal ka index store kar raha hai nt array me, taki uske first set ko access kar sake
                    int hasEps = 0;//ye variable hasEps use kar raha hai check karne ke liye ki X non-terminal ke first set me epsilon hai ya nahi, initially assume kar raha hai ki epsilon nahi hai, agar first set me epsilon milta hai to is variable ko 1 kar dega
                    for(int p=0;first[Xi][p];p++){//ye loop X non-terminal ke first set ke symbols ke liye iterate kar raha hai jab tak null character nahi milta, iska matlab hai ki jitne symbols hai unke liye loop chalega
                        char firstSym = first[Xi][p];//ye variable firstSym me X non-terminal ke first set ka current symbol store kar raha hai
                        // If first is not eps
                        if(firstSym != '#'){//agar X non-terminal ke first set ka current symbol epsilon nahi hai to usko table me entry karenge, table me entry karne ke liye hume current non-terminal A ka index i aur current symbol firstSym ka index ti find karna hoga, phir table[i][ti] me rhs ko store karna hoga, agar pehle se koi entry hai to uske sath rhs ko concatenate kar denge using '/' as separator
                            int ti = termIdx(firstSym);//ye variable ti me firstSym terminal ka index store kar raha hai terms array me, taki usko table me entry kar sake
                            if(table[i][ti][0] == '\0'){//agar table me M[A,firstSym] entry empty hai to usme rhs ko copy kar denge, agar pehle se entry hai to uske sath rhs ko concatenate kar denge using '/' as separator
                                strcpy(table[i][ti],rhs);//agar table me M[A,firstSym] entry empty hai to usme rhs ko copy kar denge, agar pehle se entry hai to uske sath rhs ko concatenate kar denge using '/' as separator
                            }else{
                                strcat(table[i][ti],"/");//agar table me M[A,firstSym] entry empty hai to usme rhs ko copy kar denge, agar pehle se entry hai to uske sath rhs ko concatenate kar denge using '/' as separator
                                strcat(table[i][ti],rhs);//agar table me M[A,firstSym] entry empty hai to usme rhs ko copy kar denge, agar pehle se entry hai to uske sath rhs ko concatenate kar denge using '/' as separator
                            }
                        }
                        if(firstSym == '#'){//agar X non-terminal ke first set ka current symbol epsilon hai to hasEps ko 1 kar denge, iska matlab hai ki X non-terminal epsilon ho sakta hai, to hume right hand side ke next symbol ko bhi check karna hoga, agar right hand side ke next symbol ke first set me bhi epsilon hai to hume usko bhi consider karna hoga, isliye hasEps variable use kar rahe hai
                            hasEps = 1;
                        }
                    }
                    if(hasEps){
                        rhsCanBeEps = 1;//agar X non-terminal ke first set me epsilon hai to rhsCanBeEps ko 1 kar denge, iska matlab hai ki abhi tak right hand side epsilon ho sakta hai, to hume right hand side ke next symbol ko bhi check karna hoga, agar right hand side ke next symbol ke first set me bhi epsilon hai to hume usko bhi consider karna hoga, isliye rhsCanBeEps variable use kar rahe hai
                    }
                } else{
                    if(X == '#'){//agar X symbol ke andar epsilon hai to rhsCanBeEps ko 1 kar denge, iska matlab hai ki right hand side epsilon ho sakta hai, to hume follow set ke symbols ke hisab se table me entry karna hoga, isliye rhsCanBeEps variable use kar rahe hai
                        rhsCanBeEps = 1;//agar X symbol ke andar epsilon hai to rhsCanBeEps ko 1 kar denge, iska matlab hai ki right hand side epsilon ho sakta hai, to hume follow set ke symbols ke hisab se table me entry karna hoga, isliye rhsCanBeEps variable use kar rahe hai
                    }else{
                        int ti = termIdx(X);//agar X symbol terminal hai to usko table me entry karenge, table me entry karne ke liye hume current non-terminal A ka index i aur current symbol X ka index ti find karna hoga, phir table[i][ti] me rhs ko store karna hoga, agar pehle se koi entry hai to uske sath rhs ko concatenate kar denge using '/' as separator
                        if(table[i][ti][0] == '\0'){//
                            strcpy(table[i][ti],rhs);//agar table me M[A,X] entry empty hai to usme rhs ko copy kar denge, agar pehle se entry hai to uske sath rhs ko concatenate kar denge using '/' as separator
                        } else{
                            strcat(table[i][ti],"/");//agar table me M[A,X] entry empty hai to usme rhs ko copy kar denge, agar pehle se entry hai to uske sath rhs ko concatenate kar denge using '/' as separator
                            strcat(table[i][ti],rhs);//agar table me M[A,X] entry empty hai to usme rhs ko copy kar denge, agar pehle se entry hai to uske sath rhs ko concatenate kar denge using '/' as separator
                        }
                    }
                }
            }
            
            if(rhsCanBeEps){//agar right hand side epsilon ho sakta hai to follow set ke symbols ke hisab se table me entry karenge, follow set ke symbols ke hisab se table me entry karne ke liye hume current non-terminal A ka index i find karna hoga, phir follow[i] me jitne symbols hai unke liye table[i][ti] me rhs ko store karna hoga, agar pehle se koi entry hai to uske sath rhs ko concatenate kar denge using '/' as separator
                for(int f=0;follow[i][f];f++){//ye loop current non-terminal A ke follow set ke symbols ke liye iterate kar raha hai jab tak null character nahi milta, iska matlab hai ki jitne symbols hai unke liye loop chalega
                    char b = follow[i][f];//ye variable b me current non-terminal A ke follow set ka current symbol store kar raha hai
                    int ti = termIdx(b);//ye variable ti me b terminal ka index store kar raha hai terms array me, taki usko table me entry kar sake

                    if(table[i][ti][0] == '\0'){//agar table me M[A,b] entry empty hai to usme rhs ko copy kar denge, agar pehle se entry hai to uske sath rhs ko concatenate kar denge using '/' as separator
                        strcpy(table[i][ti],rhs);//agar table me M[A,b] entry empty hai to usme rhs ko copy kar denge, agar pehle se entry hai to uske sath rhs ko concatenate kar denge using '/' as separator
                    }else{
                        strcat(table[i][ti],"/");//agar table me M[A,b] entry empty hai to usme rhs ko copy kar denge, agar pehle se entry hai to uske sath rhs ko concatenate kar denge using '/' as separator
                        strcat(table[i][ti],rhs);
                    }
                }
            }
        }
    }
}

void printTable(){//ye function printTable use kar raha hai parsing table ko print karne ke liye, sabse pehle terminals ke header print karega, phir har non-terminal ke liye uske corresponding table entries print karega, agar entry empty hai to '-' print karega, agar entry me production hai to usko M[non-terminal,terminal] = production format me print karega
    printf("%-8s","NT");//print header for non-terminals
    for(int i=0;i<termCount;i++){//ye loop terminals ke header print kar raha hai
        printf("%-12c",terms[i]);//print terminal header with some spacing
    }
    printf("\n");//new line after header

    for(int i=0;i<ntCount;i++){//ye loop non-terminals ke liye iterate kar raha hai, har non-terminal ke liye uske corresponding table entries print kar raha hai
        printf("%-8c",nt[i]);//print non-terminal header with some spacing
        for(int j=0;j<termCount;j++){//ye loop terminals ke liye iterate kar raha hai, har terminal ke liye table entry print kar raha hai
            if(table[i][j][0] == '\0'){//agar table entry empty hai to '-' print karenge
                printf("%-14s","-");//print - for empty entry
            }else{
                printf("%c->%-11s",nt[i],table[i][j]);//agar table entry me production hai to usko M[non-terminal,terminal] = production format me print karenge
            }
        }
        printf("\n");
    }
}

int main(){
    printf("Enter number of productions: ");
    scanf("%d",&n);

    for(int i=0;i<n;i++){
        scanf("%s",prod[i]);//ye loop user se productions ke liye input le raha hai, har production ko prod array me store kar raha hai
    }

    getNonTerminals();//ye function call karke non-terminals find kar rahe hai productions ke left hand side se aur unko nt array me store kar rahe hai
    getTerminals();//ye function call karke terminals find kar rahe hai productions ke right hand side se aur unko terms array me store kar rahe hai

    printf("Enter first sets: \n");//ye line user ko prompt kar rahi hai ki wo first sets ke liye input de, user ko first sets ke format ke baare me bhi batana chahiye, jaise ki first(A) = {a,b,#} ke format me input dena chahiye
    for(int i=0;i<ntCount;i++){//ye loop user se first sets ke liye input le raha hai, har non-terminal ke first set ko first array me store kar raha hai
        printf("first(%c) = ",nt[i]);//ye line user ko prompt kar rahi hai ki wo current non-terminal ke first set ke liye input de, user ko first set ke format ke baare me bhi batana chahiye, jaise ki first(A) = {a,b,#} ke format me input dena chahiye
        scanf("%s",first[i]);//ye line user se current non-terminal ke first set ke liye input le raha hai aur usko first array me store kar raha hai, first[i] me i-th non-terminal ke first set ka string store ho jayega, jisme symbols comma se separated honge, jaise ki "a,b,#"
    }

    printf("Enter follow sets: \n");//ye line user ko prompt kar rahi hai ki wo follow sets ke liye input de, user ko follow sets ke format ke baare me bhi batana chahiye, jaise ki follow(A) = {a,b,$} ke format me input dena chahiye
    for(int i=0;i<ntCount;i++){//ye loop user se follow sets ke liye input le raha hai, har non-terminal ke follow set ko follow array me store kar raha hai
        printf("follow(%c) = ",nt[i]);//ye line user ko prompt kar rahi hai ki wo current non-terminal ke follow set ke liye input de, user ko follow set ke format ke baare me bhi batana chahiye, jaise ki follow(A) = {a,b,$} ke format me input dena chahiye
        scanf("%s",follow[i]);//ye line user se current non-terminal ke follow set ke liye input le raha hai aur usko follow array me store kar raha hai, follow[i] me i-th non-terminal ke follow set ka string store ho jayega, jisme symbols comma se separated honge, jaise ki "a,b,$"
    }

    buildTable();//ye function call karke parsing table ko build kar rahe hai using the productions, first sets aur follow sets, is function me sabse pehle table ko empty kar diya jayega, phir har non
    printTable();//ye function call karke parsing table ko print kar rahe hai, is function me terminals ke header print kiya jayega, phir har non-terminal ke liye uske corresponding table entries print kiya jayega, agar entry empty hai to '-' print kiya jayega, agar entry me production hai to usko M[non-terminal,terminal] = production format me print kiya jayega
    return 0;
}