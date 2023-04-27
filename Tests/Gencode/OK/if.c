int a=2;

void main(){
    int b=2, c=3;
    bool d;
    if(a<b){
        print("a<b\n");
    }
    else if(a<c){
        print("a<c\n");
    }
    else{
        print(a);
    }

    if(d=((a=b+0)==(c=(c-1)))){
        print("Coucou");
    }
    else{
        print("pas coucou");
    }
}