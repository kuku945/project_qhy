#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
struct Node
{
	int num;
	struct Node* next;
};
void main()
{
	struct Node* a;
	a=(struct Node*)malloc(sizeof(struct Node));
	a->num=1;
	a->next=NULL;
	struct Node* b;
	b=(struct Node*)malloc(sizeof(struct Node));
	b->num=2;
b->next=NULL;
	a->next=b;
	struct Node* c=a;
	c=c->next;
	c->num=3;
	printf("%d\n",a->num);
	printf("%d\n",a->next->num);
}
