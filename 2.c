#include <stdio.h>
#include <string.h>
int main()
{	
	int a;
	char *temp1="123,23,43  ";
	char *temp=(char*)malloc(strlen(temp1)+1);
	memset(temp,'\0',sizeof(temp));
	memcpy(temp,temp1,strlen(temp1)+1);
	char *n;
	n=strsep(&temp,",");
	while(n!=NULL)
	{
	a=atoi(n);
	printf("%d ",a);
	n=strsep(&temp,",");
	}
	printf("\n");
	return 0;
}

