#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<curses.h>
#include<time.h>
#include<string.h>
#include<libxml/parser.h>
#include<libxml/tree.h>
#define menu_item_number 3
#define menu_left_margin 4 
char menu[menu_item_number][20]={"New Scheduler\0","history\0","exit\0"};
int men[menu_item_number][2]={{2,3},{2,4},{2,5}};
void initial()
{
	initscr();
	cbreak();
	nonl();
	echo();
	intrflush(stdscr,FALSE);
	keypad(stdscr,TRUE);
}
void menu_display(int selected_index)
{
	clear();
	int i;
	move(1,menu_left_margin);
	printw("JOB Scheduler --");
	move(2,menu_left_margin);
	printw("=====================================");
	move(6,menu_left_margin);
	printw("=====================================");
	for(i=0;i<menu_item_number;i++)
	{
		move(men[i][1],men[i][0]+menu_left_margin);
		if(i==selected_index)
		{
			attron(A_REVERSE);
			printw("%s",menu[i]);
			attroff(A_REVERSE);
		}else
			printw("%s",menu[i]);
	}

}
void New_Scheduler()
{
	char temp1[80];
	char temp2[80];
	echo();
	printw("Please input your program's address ( a/b/c.sh ) \n");
	scanw("%s",temp1);
	printw("PLease input your scheduler's time ( 2015.5.10.23.43.1 )\n");
	scanw("%s",temp2);
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
                printw("doc == null\n");
                
        }
        root=xmlDocGetRootElement(doc);
        for(node=root->children;node;node=node->next){
                if(xmlStrcasecmp(node->name,BAD_CAST"content")==0)
                        break;
        }
        if(node==NULL){
                printw("no node = content\n");
                
        }
	xmlNodePtr node_schedulers=xmlNewNode(NULL,BAD_CAST"schedulers");
	xmlAddChild(node,node_schedulers);
	xmlNodePtr node_scheduler1=xmlNewNode(NULL,BAD_CAST"scheduler");
	xmlNodePtr content1=xmlNewText((xmlChar*)temp1);
	xmlAddChild(node_schedulers,node_scheduler1);
	xmlAddChild(node_scheduler1,content1);
	xmlNewProp(node_scheduler1,BAD_CAST"name",BAD_CAST"point");

	xmlNodePtr node_scheduler2=xmlNewNode(NULL,BAD_CAST"scheduler");
	xmlNodePtr content2=xmlNewText((xmlChar*)temp2);
	xmlAddChild(node_schedulers,node_scheduler2);
	xmlAddChild(node_scheduler2,content2);
	xmlNewProp(node_scheduler2,BAD_CAST"name",BAD_CAST"time");
	xmlKeepBlanksDefault(0);
	xmlIndentTreeOutput=1;
	xmlSaveFile("scheduler.xml",doc);
        xmlFreeDoc(doc);
	printw("Add success,Please input to return");


	getch();
	return;
}
void History()
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
                printw("doc == null\n");
                
        }
        root=xmlDocGetRootElement(doc);
        for(node=root->children;node;node=node->next){
                if(xmlStrcasecmp(node->name,BAD_CAST"content")==0)
                        break;
        }
        if(node==NULL){
                printw("no node = content\n");
                
        }
        for(node=node->children;node;node=node->next){
               if(xmlStrcasecmp(node->name,BAD_CAST"schedulers")==0){  
                        for(detail=node->children;detail;detail=detail->next){  
                                if(xmlStrcasecmp(detail->name,BAD_CAST"scheduler")==0){
                                        name=xmlGetProp(detail,BAD_CAST"name");
                                        value=xmlNodeGetContent(detail);
                                        if(strlen((char*)value)!=0){
                                                printw("%s : %s\n",(char*)name,(char*)value);                                       
                                         }else{
                                                printw("%s has no value\n",(char*)name);
                                        }
                                        xmlFree(name);
                                        xmlFree(value);
                                }
                        }
                }
        }
        xmlFreeDoc(doc);
	printw("Please input to return");
	getch();
	return;
	
	
}
void main()
{
	char *content;
        unsigned long filesize;
	int selected_index=0;
	int ch;
	initial();
	menu_display(selected_index);
	while(1)
	{ 	
		ch=getch();
		switch(ch){
		case KEY_UP:
			--selected_index;
			if(selected_index<0)
			{
				selected_index=0;
			}
			menu_display(selected_index);
			break;
		case KEY_DOWN:
			selected_index++;
			if(selected_index>=menu_item_number)
			{
				selected_index=0;
			}
			menu_display(selected_index);
			break;
		case '\r':
			clear();
			switch(selected_index){
			case 0:
				New_Scheduler();
				menu_display(selected_index);
				break;
			case 1:
				History();
				menu_display(selected_index);
				break;
			case 2:
				printw("bye\n");
				sleep(1);
				endwin();
				exit(1);
			}
		}
	}
}
