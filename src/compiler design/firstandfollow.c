#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
#define MAX 100

char prod[MAX][MAX];//ye 2D array prod use kar raha hai productions ko store karne ke liye, jisme MAX number of productions aur MAX length of each production ko represent karta hai
char first[MAX][MAX];//ye 2D array first use kar raha hai first sets ko store karne ke liye, jisme MAX number of non-terminals aur MAX number of symbols in first set ko represent karta hai
char follow[MAX][MAX];//ye 2D array follow use kar raha hai follow sets ko store karne ke liye, jisme MAX number of non-terminals aur MAX number of symbols in follow set ko represent karta hai

int ntCount = 0;//ye variable ntCount use kar raha hai number of non-terminals ko store karne ke liye
int n;//ye variable n use kar raha hai number of productions ko store karne ke liye

char nt[MAX];//ye array nt use kar raha hai non-terminals ko store karne ke liye

int ntIdx(char c){//ye function ntIdx use kar raha hai non-terminal ka index find karne ke liye nt array me, agar nahi mila to -1 return karega
    for(int i=0;i<ntCount;i++){
        if(nt[i] == c){
            return i;
        }
    }
    return -1;
}

void getnonTerminals(){//ye function getnonTerminals use kar raha hai non-terminals ko find karne ke liye productions ke left hand side se, agar non-terminal pehle se nt array me nahi hai to usko add kar dega
    for(int i=0;i<n;i++){
        char c = prod[i][0];
        if(ntIdx(c) == -1){
            nt[ntCount++] = c;
        }
    }
}

int contains(char set[],char c){//ye function contains use kar raha hai check karne ke liye ki given set me c symbol already hai ya nahi, agar set me c symbol milta hai to 1 return karega, agar nahi milta hai to 0 return karega
    for(int i=0;set[i];i++){//ye loop set array me iterate kar raha hai jab tak null character nahi milta, iska matlab hai ki jitne symbols hai unke liye loop chalega
        if(set[i] == c){//agar set array me c symbol milta hai to 1 return kar dega
            return 1;
        }
    }
    return 0;
}

int add(char set[],char c){//ye function add use kar raha hai set me c symbol add karne ke liye, agar c symbol set me pehle se nahi hai to usko set me add kar dega aur 1 return karega, agar c symbol set me pehle se hai to usko add nahi karega aur 0 return karega
    if(!contains(set,c)){//agar set me c symbol pehle se nahi hai to usko set me add kar dega
        int l = strlen(set);//ye variable l me set array ki current length store kar raha hai, taki uske baad c symbol ko add kar sake
        set[l] = c;//ye set array ke end me c symbol add kar raha hai
        set[l+1] = '\0';//ye set array ke end me null character add kar raha hai taki set array ek valid string ban jaye
        return 1;
    }
    return 0;
}

void calculateFirst(){//ye function calculateFirst use kar raha hai first sets ko calculate karne ke liye, sabse pehle un productions ke liye jinke right hand side ka first symbol terminal hai unke left hand side non-terminal ke first set me us terminal ko add kar diya jayega, phir baaki productions ke liye iteratively first sets calculate kiya jayega jab tak koi change nahi hota, is process me agar right hand side ka symbol non-terminal hai to uske first set ke symbols ko left hand side non-terminal ke first set me add kar diya jayega, agar right hand side ka symbol epsilon ho sakta hai to next symbol ko bhi check karna hoga, agar right hand side ke sabhi symbols epsilon ho sakte hai to left hand side non-terminal ke first set me epsilon add kar diya jayega
    for(int i=0;i<n;i++){//ye loop productions me iterate kar raha hai
        char A = prod[i][0];//ye variable A me production ke left hand side ka non-terminal store kar raha hai
        char X = prod[i][2];//ye variable X me production ke right hand side ka first symbol store kar raha hai, prod[i][2] se start kar raha hai kyunki prod[i][0] non-terminal hai aur prod[i][1] me '->' hai, to right hand side ka first symbol prod[i][2] se start hota hai

        if(!isupper(X)){//agar X terminal hai to usko A ke first set me add kar denge, A ke first set me X symbol add karne ke liye hume A non-terminal ka index Aidx find karna hoga nt array me, phir first[Aidx] me X symbol add karna hoga using add function, agar X symbol pehle se first[Aidx] me hai to add function 0 return karega aur agar X symbol successfully add hota hai to add function 1 return karega
            int Aidx = ntIdx(A);//ye variable Aidx me A non-terminal ka index store kar raha hai nt array me, taki uske first set ko access kar sake
            add(first[Aidx],X);//agar X terminal hai to usko A ke first set me add kar denge, A ke first set me X symbol add karne ke liye hume A non-terminal ka index Aidx find karna hoga nt array me, phir first[Aidx] me X symbol add karna hoga using add function, agar X symbol pehle se first[Aidx] me hai to add function 0 return karega aur agar X symbol successfully add hota hai to add function 1 return karega
        }
    }
    int changed = 1;//ye variable changed use kar raha hai check karne ke liye ki first sets me koi change hua hai ya nahi, initially assume kar raha hai ki change hua hai, jab tak change hota rahega tab tak first sets calculate karta rahega
    while(changed){//jab tak first sets me change hota rahega tab tak loop chalega
        changed = 0;//ye line changed ko 0 kar rahi hai, iska matlab hai ki abhi tak assume kar rahe the ki change hua hai, lekin ab humlog first sets calculate karne wale hai, agar first sets calculate karne ke baad koi change hota hai to is variable ko wapas 1 kar denge, lekin agar first sets calculate karne ke baad koi change nahi hota hai to is variable 0 hi rahega aur loop terminate ho jayega
        for(int i=0;i<n;i++){//ye loop productions me iterate kar raha hai
            char A = prod[i][0];//ye variable A me production ke left hand side ka non-terminal store kar raha hai
            int Aidx = ntIdx(A);//ye variable Aidx me A non-terminal ka index store kar raha hai nt array me, taki uske first set ko access kar sake
            int k = 2;
            int hasEps = 1;//ye variable hasEps use kar raha hai check karne ke liye ki right hand side ke symbols me se koi symbol epsilon ho sakta hai ya nahi, initially assume kar raha hai ki right hand side ke symbols me se koi symbol epsilon ho sakta hai, agar right hand side ke kisi symbol ke first set me epsilon nahi milta hai to is variable ko 0 kar dega, lekin agar right hand side ke kisi symbol ke first set me epsilon milta hai to is variable ko 1 hi rakhega aur next symbol ko bhi check karega

            while(prod[i][k] != '\0' && hasEps){//ye loop right hand side ke symbols me iterate kar raha hai jab tak null character nahi milta, iska matlab hai ki jitne symbols hai unke liye loop chalega, aur hasEps check kar raha hai ki right hand side ke symbols me se koi symbol epsilon ho sakta hai ya nahi
                char X = prod[i][k];//ye variable X me right hand side ka current symbol store kar raha hai
                hasEps = 0;//ye line hasEps ko 0 kar rahi hai, iska matlab hai ki abhi tak assume kar rahe the ki right hand side ke symbols me se koi symbol epsilon ho sakta hai, lekin ab humlog current symbol X ko check karne wale hai, agar X non-terminal hai to uske first set me epsilon milta hai to hasEps ko wapas 1 kar denge, lekin agar X non-terminal hai aur uske first set me epsilon nahi milta hai to hasEps 0 hi rahega, agar X terminal hai aur wo epsilon nahi hai to hasEps 0 hi rahega, lekin agar X terminal hai aur wo epsilon hai to hasEps wapas 1 kar denge
                if(isupper(X)){//agar X non-terminal hai to uske first set ke symbols ke hisab se A ke first set me symbols add karenge, agar X non-terminal ke first set me epsilon hai to hasEps ko 1 kar denge, iska matlab hai ki right hand side ke symbols me se koi symbol epsilon ho sakta hai, to next symbol ko bhi check karna hoga
                    int Xidx = ntIdx(X);//ye variable Xidx me X non-terminal ka index store kar raha hai nt array me, taki uske first set ko access kar sake
                    for(int j=0;first[Xidx][j];j++){//ye loop X non-terminal ke first set ke symbols me iterate kar raha hai jab tak null character nahi milta, iska matlab hai ki jitne symbols hai unke liye loop chalega
                        if(first[Xidx][j] == '#'){//agar X non-terminal ke first set me epsilon hai to hasEps ko 1 kar denge, iska matlab hai ki right hand side ke symbols me se koi symbol epsilon ho sakta hai, to next symbol ko bhi check karna hoga
                            hasEps = 1;//agar X non-terminal ke first set me epsilon hai to hasEps ko 1 kar denge, iska matlab hai ki right hand side ke symbols me se koi symbol epsilon ho sakta hai, to next symbol ko bhi check karna hoga
                        }else{
                            if(add(first[Aidx],first[Xidx][j])){//agar X non-terminal ke first set me epsilon nahi hai to uske first set ke symbols ko A ke first set me add karenge, A ke first set me X ke first set ke symbol add karne ke liye hume A non-terminal ka index Aidx find karna hoga nt array me, aur X non-terminal ka index Xidx find karna hoga nt array me, phir add function use karke first[Aidx] me first[Xidx][j] symbol add karna hoga, agar first[Xidx][j] symbol pehle se first[Aidx] me hai to add function 0 return karega aur agar first[Xidx][j] symbol successfully add hota hai to add function 1 return karega
                                changed = 1;
                            }
                        }
                    }
                }else{
                    if(X == '#'){
                        hasEps = 1;
                    }else{
                        if(add(first[Aidx],X)){
                            changed = 1;//agar X terminal hai aur wo epsilon nahi hai to usko A ke first set me add kar denge, A ke first set me X symbol add karne ke liye hume A non-terminal ka index Aidx find karna hoga nt array me, phir add function use karke first[Aidx] me X symbol add karna hoga, agar X symbol pehle se first[Aidx] me hai to add function 0 return karega aur agar X symbol successfully add hota hai to add function 1 return karega
                        }
                    }
                }
                k++;
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

void calculateFollow(){//ye function calculateFollow use kar raha hai follow sets ko calculate karne ke liye, sabse pehle start symbol ke follow set me $ symbol add kar diya jayega, phir baaki productions ke liye iteratively follow sets calculate kiya jayega jab tak koi change nahi hota, is process me agar production ke right hand side me non-terminal B milta hai to uske baad ke symbols ke first set ke symbols ko B ke follow set me add kar diya jayega, agar production ke right hand side me non-terminal B milta hai aur uske baad ke symbols epsilon ho sakte hai to production ke left hand side non-terminal A ke follow set ke symbols ko B ke follow set me add kar diya jayega
    add(follow[ntIdx(prod[0][0])],'$');//ye line start symbol ke follow set me $ symbol add kar rahi hai, prod[0][0] se start kar raha hai kyunki prod[0][0] start symbol hai, ntIdx function use karke start symbol ka index find kar raha hai nt array me, phir follow array me us index ke corresponding set me $ symbol add kar raha hai using add function, agar $ symbol pehle se follow set me hai to add function 0 return karega aur agar $ symbol successfully add hota hai to add function 1 return karega
    int followChanged = 1;//ye variable followChanged use kar raha hai check karne ke liye ki follow sets me koi change hua hai ya nahi, initially assume kar raha hai ki change hua hai, jab tak change hota rahega tab tak follow sets calculate karta rahega
    while(followChanged){//jab tak follow sets me change hota rahega tab tak loop chalega
        followChanged = 0;//ye line followChanged ko 0 kar rahi hai, iska matlab hai ki abhi tak assume kar rahe the ki change hua hai, lekin ab humlog follow sets calculate karne wale hai, agar follow sets calculate karne ke baad koi change hota hai to is variable ko wapas 1 kar denge, lekin agar follow sets calculate karne ke baad koi change nahi hota hai to is variable 0 hi rahega aur loop terminate ho jayega
        for(int i=0;i<n;i++){
            char A = prod[i][0];
            int Aidx = ntIdx(A);

            for(int j=2;prod[i][j]!='\0';j++){//ye loop production ke right hand side ke symbols me iterate kar raha hai jab tak null character nahi milta, iska matlab hai ki jitne symbols hai unke liye loop chalega
                char B = prod[i][j];//ye variable B me right hand side ka current symbol store kar raha hai
                int Bidx = ntIdx(B);
                if(isupper(B)){//agar B non-terminal hai to uske baad ke symbols ke first set ke symbols ko B ke follow set me add kar diya jayega, agar production ke right hand side me non-terminal B milta hai aur uske baad ke symbols epsilon ho sakte hai to production ke left hand side non-terminal A ke follow set ke symbols ko B ke follow set me add kar diya jayega
                    int k = j+1;//ye variable k me B ke baad ke symbols me iterate karne ke liye index store kar raha hai, initially k ko j+1 se initialize kar raha hai kyunki hume B ke baad ke symbols me iterate karna hai
                    int hasEps = 1;
                    while(prod[i][k] != '\0' && hasEps){//ye loop B ke baad ke symbols me iterate kar raha hai jab tak null character nahi milta, iska matlab hai ki jitne symbols hai unke liye loop chalega, aur hasEps check kar raha hai ki B ke baad ke symbols me se koi symbol epsilon ho sakta hai ya nahi
                        char X = prod[i][k];
                        hasEps = 0;
                        if(isupper(X)){//agar X non-terminal hai to uske first set ke symbols ke hisab se B ke follow set me symbols add karenge, agar X non-terminal ke first set me epsilon hai to hasEps ko 1 kar denge, iska matlab hai ki B ke baad ke symbols me se koi symbol epsilon ho sakta hai, to next symbol ko bhi check karna hoga
                            int Xidx = ntIdx(X);
                            for(int l=0;first[Xidx][l];l++){//ye loop X non-terminal ke first set ke symbols me iterate kar raha hai jab tak null character nahi milta, iska matlab hai ki jitne symbols hai unke liye loop chalega
                                if(first[Xidx][l] == '#'){//agar X non-terminal ke first set me epsilon hai to hasEps ko 1 kar denge, iska matlab hai ki B ke baad ke symbols me se koi symbol epsilon ho sakta hai, to next symbol ko bhi check karna hoga
                                    hasEps = 1;
                                }else{
                                    if(add(follow[Bidx],first[Xidx][l])){
                                        followChanged = 1;
                                    }
                                }
                            }
                        }else{
                            if(X == '#'){
                                hasEps = 1;
                            }else{
                                if(add(follow[Bidx],X)){//agar X terminal hai aur wo epsilon nahi hai to usko B ke follow set me add kar denge, B ke follow set me X symbol add karne ke liye hume B non-terminal ka index Bidx find karna hoga nt array me, phir add function use karke follow[Bidx] me X symbol add karna hoga, agar X symbol pehle se follow[Bidx] me hai to add function 0 return karega aur agar X symbol successfully add hota hai to add function 1 return karega
                                    followChanged = 1;
                                }
                            }
                        }
                        k++;
                    }
                    if(hasEps){//agar production ke right hand side me non-terminal B milta hai aur uske baad ke symbols epsilon ho sakte hai to production ke left hand side non-terminal A ke follow set ke symbols ko B ke follow set me add kar diya jayega
                        for(int l=0;follow[Aidx][l];l++){//ye loop A ke follow set ke symbols me iterate kar raha hai jab tak null character nahi milta, iska matlab hai ki jitne symbols hai unke liye loop chalega
                            if(add(follow[Bidx],follow[Aidx][l])){
                                followChanged = 1;
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

    getTerminals();

    calculateFirst();
    printFirst();

    calculateFollow();
    printFollow();

    return 0;
}