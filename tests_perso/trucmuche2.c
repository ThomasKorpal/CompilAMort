int global1 = 15;
int global4;
bool global2 = false;
bool global3 = true;

void main()
{
    int b = 2;
    int a = global1 + 2;
    print("Bonjour\n");
    print("printf(",a,"+",b,"=",a,"\n");
    {
        int global1 = 1;
        int a = 2;
        int z = 5;
        for(a; a<z; a=a+1){
            if(global2 || global3){
                print("Coucou\n");
            }
        }
    }
}
