/* sampleCodeModule.c */

char * v = (char*)0xB8000 + 79 * 2;

static int var1 = 0;
static int var2 = 0;



int main(){
    
    welcome();
    
    getCommands();

    return 0;
}