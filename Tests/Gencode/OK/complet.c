//Declaration de variables globales
int gINT1, gINT2 = 20, gINT3=5, gINT4 = 0x0FFFFFFF, gINT5=0xA;
bool gBOO1, gBOO2 = true, gBOO3=false;
int context1 = 1, context2 = 2;
bool context3 = true, context4 = false;

void main(){
	//Declaration de variables locales
	int entier1, entier2 = 0x20, a=1, b=2, c=3, d=16;
	bool bool1, bool2 = true, bool3 = false;
	int i, e=5;
	int context1 = 3;
	bool context3 = false;

	gINT1 = context1;//3 et non 1
	gINT1 = context2;//2
	gBOO1 = context3;//false et non true
	gBOO1 = context4;//false

	{
		int context2 = 4;
		bool context4 = true;

		gINT1 = context1; //3 et non 1
		gINT1 = context2; //4 et non 2
		gBOO1 = context3; //false et non true
		gBOO1 = context4; //true et non false
	}

	//test boucle for avec entier
	for(i=0; i<e; i=i+1){
		print("boucle for\n");
	}

	//test boucle for avec booleen
	for(bool2=true; bool2!=bool3; i=i+1){
		bool2 = false;
	}

	//test condition if
	if(!bool2){
		bool2 = true;
	}
	else if(bool2 && (a==1 || b==1)){
		entier1 = 0;
	}
	else{
		entier1 = 1;
	}

	//test boucle while
	while(bool2){
		bool2 = false;
	}

	//test boucle dowhile
	do{
		bool2 = true;
	}while(bool2==false);

	//test sur le type de retour
	entier1 = b - a + (-32 + entier2) / d * 0x2;
	print(entier1);
	entier1 = -entier2;
	print(entier1);
	entier1 = entier2 % ((d + a) % c);
	print(entier1);
	entier1 = a << (b << 1);
	print(entier1);
	entier1 =  a << 0x2;
	print(entier1);
	entier1 = entier2 >> 1;
	print(entier1);
	entier1 = entier2 >> 0x1;
	print(entier1);
	entier1 = entier2 >> a;
	print(entier1);
	entier1 = entier2 >>> 1;
	print(entier1);
	entier1 = entier2 >>> 0x1;
	print(entier1);
	entier1 = entier2 >>> a;
	print(entier1);
	bool1 = a < b;
	print(bool1);
	bool1 = a < 1;
	print(bool1);
	bool1 = 1 < b;
	print(bool1);
	bool1 = a < 0x1;
	print(bool1);
	bool1 = 0x1 < b;
	print(bool1);
	bool1 = a > b;
	print(bool1);
	bool1 = a > 1;
	print(bool1);
	bool1 = 1 > b;
	print(bool1);
	bool1 = a > 0x1;
	print(bool1);
	bool1 = 0x1 > b;
	print(bool1);
	bool1 = a <= b;
	print(bool1);
	bool1 = a <= 1;
	print(bool1);
	bool1 = 1 <= b;
	print(bool1);
	bool1 = a <= 0x1;
	print(bool1);
	bool1 = 0x1 <= b;
	print(bool1);
	bool1 = a >= b;
	print(bool1);
	bool1 = a >= 1;
	print(bool1);
	bool1 = 1 >= b;
	print(bool1);
	bool1 = a >= 0x1;
	print(bool1);
	bool1 = 0x1 >= b;
	print(bool1);
	entier1 = a & b;
	print(entier1);
	entier1 = a & 1;
	print(entier1);
	entier1 = 1 & b;
	print(entier1);
	entier1 = a & 0x1;
	print(entier1);
	entier1 = 0x1 & b;
	print(entier1);
	entier1 = a | b;
	print(entier1);
	entier1 = a | 1;
	print(entier1);
	entier1 = 1 | b;
	print(entier1);
	entier1 = a | 0x1;
	print(entier1);
	entier1 = 0x1 | b;
	print(entier1);
	entier1 = a ^ b;
	print(entier1);
	entier1 = a ^ 1;
	print(entier1);
	entier1 = 1 ^ b;
	print(entier1);
	entier1 = a ^ 0x1;
	print(entier1);
	entier1 = 0x1 ^ b;
	print(entier1);
	bool1 = bool2==bool3;
	print(bool1);
	bool1 = a==b;
	print(bool1);
	bool1 = a==1;
	print(bool1);
	bool1 = a==0x1;
	print(bool1);
	bool1 = 1==b;
	print(bool1);
	bool1 = 0x1==b;
	print(bool1);
	bool1 = bool2!=bool3;
	print(bool1);
	bool1 = bool2!=true;
	print(bool1);
	bool1 = bool2!=false;
	print(bool1);
	bool1 = true!=bool3;
	print(bool1);
	bool1 = false!=bool3;
	print(bool1);
	bool1 = a!=b;
	print(bool1);
	bool1 = a!=1;
	print(bool1);
	bool1 = a!=0x1;
	print(bool1);
	bool1 = 1!=b;
	print(bool1);
	bool1 = 0x1!=b;
	print(bool1);
	entier1 = ~entier2;
	print(entier1);
	entier1 = ~1;
	print(entier1);
	entier1 = ~0x1;
	print(entier1);
	bool1 = !bool2;
	print(bool1);
	bool1 = !true;
	print(bool1);
	bool1 = !false;
	print(bool1);
	print("Coucou, entier 1 = ", entier1, " et bool 1 = ", bool1, "\nFin test\n");
}