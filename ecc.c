#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int G=2400, PA, PB,cpt[50],secretKA;
int A();
int B(int,int[],int);
int main(){
    printf("eool");
	int kG=A();
	B(kG,cpt,secretKA);
}

int A()
{
	int n;
	PA=n*G;
	int secretk=n*PB;
	secretKA=secretk;
	char *msg="Hello Dear";
	printf("%s",msg);
	int k=rand()%10;
	for(int i=0;i<((int)(strlen(msg)));i++){
		cpt[i]=(int)msg[i]+k*PB;
	}
  return k*G;
}

int B(int kG,int *cpt,int secretKA)
{
	int n;
	PB=n*G;
	char *ogmsg;
	int secretk=n*PA;
	if (secretk!=secretKA)
	  return 0;
	int i=0;
	printf("Cipher text is:");
	while (cpt[i]){
		
		printf("%d",cpt[i]);
		ogmsg[i]=(char)(cpt[i]-kG*n);
		i++;
	}
	ogmsg[i]='\0';
	printf("\n%s",ogmsg);
  return 0;
}
