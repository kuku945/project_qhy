#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<curses.h>
#include<fcntl.h>
#include<time.h>
#include<string.h>
#include<libxml/parser.h>
#include<libxml/tree.h>
#include<pthread.h>
#include<sys/stat.h>
#include<sys/wait.h>
#include<sys/types.h>
struct tm *p;
struct PROJECT
{
	char* proj;
	int time[6];       //年、月、日、时、分、秒。
        int start;         //确认是否需要启动 
	struct PROJECT *next;
}PRO;
void Gettime()                               //获取当前时间
{
	char *wday[]={"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
	time_t timep;
	time(&timep);
	p=localtime(&timep); /*取得当地时间*/
//	printf (“%d%d%d ”, (1900+p->tm_year),( 1+p->tm_mon), p->tm_mday);
//	printf(“%s%d:%d:%d\n”, wday[p->tm_wday],p->tm_hour, p->tm_min, p->tm_sec);
}
int timelink(struct PROJECT *pro,char *temp)             //转换时间类型 
{
	
}
Run_project(struct PROJECT *project)
{
	while(1)
{
	if()
	system("gnome-terminal -e"+project->proj);  
	//记录并打开监视程序//



	//	
}
}
int infine(struct PROJECT *project，int i,int time)                   //     确认当前时间内是否有计划
{
	struct PROJECT *pro;
	pro=project->next;
	Gettime();
	while(project->next!=NULL)
	{
		
	}	
}
void GetScheduler(struct PROJECT *project)
{
	struct PROJECT *pro=project;
	char *buf;
        unsigned long len;
        FILE *file;
        if((file=fopen("history.xml","r"))==NULL){
                perror("openf file error");
        }
        fseek(file,0,SEEK_END);
        len=ftell(file);
        rewind(file);
        buf=(char *)malloc(len+1);
        memset(buf,0,len+1);
        fread(buf,1,len,file);
        fclose(file);
	xmlDocPtr doc;
        xmlNodePtr root,node,detail;
        xmlChar *name,*value;
        doc=xmlParseMemory(buf,len);    
        if(doc==NULL){
                
        }
        root=xmlDocGetRootElement(doc);
        for(node=root->children;node;node=node->next){
                if(xmlStrcasecmp(node->name,BAD_CAST"content")==0)
                        break;
        }
        if(node==NULL){
                
        }
        for(node=node->children;node;node=node->next){
               if(xmlStrcasecmp(node->name,BAD_CAST"schedulers")==0){  
                        for(detail=node->children;detail;detail=detail->next)
			{  
                                if(xmlStrcasecmp(detail->name,BAD_CAST"scheduler")==0)
				{
                                        name=xmlGetProp(detail,BAD_CAST"name");
                                        value=xmlNodeGetContent(detail);
                                        if(strlen((char*)value)!=0)
					{
						struct PROJECT *temp;
						temp=(struct PROJECT *)malloc(sizeof(PRO));
					        if((char*)name=="name")
						{
							temp->proj=(char*)value;	
						}else if((char*)name=="time")
						{
							timelink(temp,(char*)value);
						}
						temp->next=NULL;
						pro->next=temp;
						pro=pro->next;
                                 
                                         }else{
   
                                  }
                                        xmlFree(name);
                                        xmlFree(value);
                           }
                        }
                }
        }
        xmlFreeDoc(doc);
}

void run(struct PROJECT *project)
{
	while(1)
	{
		if(infine(project,0)==0)
		{	
			sleep(60*60);
		}else
		{
			while(1)
			{
				if(infine(project,3)==0)
				{
					sleep(60);
				}else
				{
					while(1)
					{
						if(infine(project,4)!=0)
						{
							Run_project(project);
							return;
						}
					}			
				}
				
			}

		}
	}
}
void main()
{
	struct PROJECT *project;
	project=(struct PROJECT*)malloc(sizeof(PRO));
	project->proj=" ";
	project->next=NULL;
	GetScheduler(project);
	while(1)
	{
		if(infine(project)==0)
		{
			sleep(24*60*60);	
		}else
		{
			run(project);
		}
	}	
	

}
