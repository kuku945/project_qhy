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
	char* times;
	int time[6];       //年、月、日、时、分、秒。
        int start;         //确认是否需要启动  0:不开启 1:进提那开启 2：本小时开启 3：本分钟 开启，立即执行 4：成功 5：失败
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
int timechange(struct PROJECT *pro)             //转换时间类型 
{
	int i=0;
	char *temp=(char*)malloc(strlen(pro->times)+1);
	memset(temp,'\0',sizeof(temp));
	memcpy(temp,pro->times,strlen(pro->times)+1);
	char *n;
	n=strsep(&temp,".");
	while(n!=NULL)
	{
	pro->time[i++]=atoi(n);
	n=strsep(&temp,".");
	}
	return 0;
	
}
void DeleteScheduler(struct PROJECT *project)
{
	char *buf;
        unsigned long len;
        FILE *file;
        if((file=fopen("scheduler.xml","r"))==NULL){
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
                printf("doc == null\n");
                
        }
        root=xmlDocGetRootElement(doc);
        for(node=root->children;node;node=node->next){
                if(xmlStrcasecmp(node->name,BAD_CAST"content")==0)
                        break;
        }
        if(node==NULL){
                printf("no node = content\n");
                
        }
        for(node=node->children;node;node=node->next){
               if(xmlStrcasecmp(node->name,BAD_CAST"schedulers")==0){  
			
                        for(detail=node->children;detail;detail=detail->next){  
                                if(xmlStrcasecmp(detail->name,BAD_CAST"scheduler")==0){
                                        name=xmlGetProp(detail,BAD_CAST"name");
                                        value=xmlNodeGetContent(detail);
                                        if(strlen((char*)value)!=0){
						if(xmlStrcasecmp(value,(xmlChar*)project->proj)==0)
						{
							xmlNodePtr tempNode;
							tempNode=node->next;
							xmlUnlinkNode(node);
							xmlFreeNode(node);
							node=tempNode;
						}
						
                                         }else{
                                  //              printf("%s has no value\n",(char*)name);
                                        }
                                        xmlFree(name);
                                        xmlFree(value);
                                }
                        }
                }
        }
        xmlKeepBlanksDefault(0);
	xmlIndentTreeOutput=1;
	xmlSaveFile("scheduler.xml",doc);
	xmlFreeDoc(doc);
	
}
Update(struct PROJECT *project)
{
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
	xmlNodePtr node_schedulers=xmlNewNode(NULL,BAD_CAST"schedulers");
	xmlAddChild(node,node_schedulers);
	xmlNodePtr node_scheduler1=xmlNewNode(NULL,BAD_CAST"scheduler");
	xmlNodePtr content1=xmlNewText((xmlChar*)project->proj);
	xmlAddChild(node_schedulers,node_scheduler1);
	xmlAddChild(node_scheduler1,content1);
	xmlNewProp(node_scheduler1,BAD_CAST"name",BAD_CAST"point");

	xmlNodePtr node_scheduler2=xmlNewNode(NULL,BAD_CAST"scheduler");
	xmlNodePtr content2=xmlNewText((xmlChar*)project->times);
	xmlAddChild(node_schedulers,node_scheduler2);
	xmlAddChild(node_scheduler2,content2);
	xmlNewProp(node_scheduler2,BAD_CAST"name",BAD_CAST"time");

	xmlNodePtr node_scheduler3=xmlNewNode(NULL,BAD_CAST"scheduler");
	xmlNodePtr content3;
	if (project->start == 4)
	{
		content3 = xmlNewText(BAD_CAST"调度成功");
	}
	else if (project->start == 5)
	{
		content3 = xmlNewText(BAD_CAST"调度失败");
	}
	else if (project->start == 6)
	{
		content3 = xmlNewText(BAD_CAST"过期调度度计划");
	}
	xmlAddChild(node_schedulers,node_scheduler3);
	xmlAddChild(node_scheduler3,content3);
	xmlNewProp(node_scheduler3,BAD_CAST"name",BAD_CAST"condition");
	xmlKeepBlanksDefault(0);
	xmlIndentTreeOutput=1;
	xmlSaveFile("history.xml",doc);
	xmlFreeDoc(doc);
}
Run_project(struct PROJECT *project) //每秒判断需要进行修改
{
	struct PROJECT *temp=project;
	struct PROJECT *head=project;
	do{
		Gettime();
	}
	while(project->time[5]<p->tm_sec);
	while(temp->next!=NULL)
	{	
		temp=temp->next;
		if((p->tm_sec==temp->time[5])&&(temp->start==3))
		{
			char * a=(char*)malloc(strlen("gnome-terminal -e")+strlen(project->proj)+1);
			a="gnome-terminal -e ";
			strcat(a,project->proj);
			system(a);  
			//记录并打开监视程序//



			//	
			temp->start++;
			Update(temp);
			DeleteScheduler(temp);	
		}
		head=head->next;
		
	}
}
int infine(struct PROJECT *project,int i)                   //     确认当前时间内是否有计划
{
	int a=0;
	struct PROJECT *temp=project;
	Gettime();
	while(temp->next!=NULL)
	{
		temp=temp->next;
		switch(i)
		{
			case 0:              //第一次解析
				if((temp->time[0]== (1900+p->tm_year))&&(temp->time[1]==( 1+p->tm_mon))&&(temp->time[2]==p->tm_mday))
				{
					temp->start++;	
					a=1;
				}else if ((p->tm_year * 365 + p->tm_mon * 31 + p->tm_mday)>(temp->time[0] * 365 + temp->time[1] * 31 + temp->time[2]))
				{
					temp->start = 6;
					DeleteScheduler(temp);
				}
				break;
			case 3:              //按小时判断
				if(temp->time[3]== p->tm_hour)
				{
					temp->start++;
					a=1;
				}
				break;
			case 4:              //按分钟判断
				if(temp->time[4]== p->tm_min)
				{
					temp->start++;
					a=1;
				}
				break;
		}
		
	}	

	return a;
}

void GetScheduler(struct PROJECT *project)
{
	int i=0;
	struct PROJECT *temp=project;
	struct PROJECT *pro;
	pro=(struct PROJECT *)malloc(sizeof(project));
	char *buf;
        unsigned long len;
        FILE *file;
        if((file=fopen("scheduler.xml","r"))==NULL){
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
                printf("doc == null\n");
                
        }
        root=xmlDocGetRootElement(doc);
        for(node=root->children;node;node=node->next){
                if(xmlStrcasecmp(node->name,BAD_CAST"content")==0)
                        break;
        }
        if(node==NULL){
                printf("no node = content\n");
                
        }
        for(node=node->children;node;node=node->next){
               if(xmlStrcasecmp(node->name,BAD_CAST"schedulers")==0){  
			
                        for(detail=node->children;detail;detail=detail->next){  
                                if(xmlStrcasecmp(detail->name,BAD_CAST"scheduler")==0){
                                        name=xmlGetProp(detail,BAD_CAST"name");
                                        value=xmlNodeGetContent(detail);
                                        if(strlen((char*)value)!=0){
						if(i==0)
						{
							
							pro->proj=(char*)malloc(strlen(value)+1);
							memset(pro->proj,'\0',sizeof(value));
							memcpy(pro->proj,value,strlen(value)+1);
						}else if(i==1)
						{
							
							pro->times=(char*)malloc(strlen(value)+1);
							memset(pro->times,'\0',sizeof(value));
							memcpy(pro->times,value,strlen(value)+1);
							timechange(pro);  
						}
						i++;
						if(i==2)
						{
							i=0;
						}
						temp->next=pro;  
						temp=temp->next; 
                                         }else{
                                  //              printf("%s has no value\n",(char*)name);
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
	int sle=0;
	while(1)
	{
		if(infine(project,0)==0)
		{
			Gettime();
			sle=24*60*60-(p->tm_hour*60*60+p->tm_min*60+p->tm_sec);
			sleep(sle);	
		}else
		{
			while(1)
			{
				if(infine(project,3)==0)
				{	
					Gettime();
					sle=60*60-(p->tm_min*60+p->tm_sec);
					sleep(sle);
				}else
				{
					while(1)
					{
						if(infine(project,4)==0)
						{
							Gettime();
							sle=60-p->tm_sec;
							sleep(sle);
						}else
						{	
							
							Run_project(project);
							run(project);
						}
				
					}
				}
			}
		}
		struct PROJECT *temp;
		temp=project->next;
		project->next=NULL;
		free(temp);
		GetScheduler(project);
	}
}
void main()
{
	struct PROJECT *project;
	project=(struct PROJECT*)malloc(sizeof(PRO));
	project->proj=" ";
	project->next=NULL;
	GetScheduler(project);
	run(project);
			
}
