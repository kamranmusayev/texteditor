#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_NUM 30
struct node
{
	int statno; 	// statement number
	char stat[40];	//	max. 40 characters
	int next;		// points to the textbuffer[] index of the next statement
};
struct dfs
{
	int code; //1-insertion 2-deletion 0-otherwise
	int statno;
	char statement[40]; // only for insertion
};
struct dfs diffs[20];  //max. 20 changes
int version=0,diffcount=0,temp_count=0;
struct node textbuffer[MAX_NUM];
int head=0,count=0;
void Edit(char *filename, int inputversion);
void Insert(int number,char statement[40]);
void deleteLine(int number);
void save(char *filename);
void commit(char *filename);
void print();


void Edit(char *filename,int inputversion){
	if(inputversion==0)
		inputversion=version;
	count=0,diffcount=0;
    FILE *fp;
    fp = fopen(filename,"r");
    int next=0;
    int position;
    int statenumber=0,i=0,tempversion,command=1;
    if(fp==NULL)
    	printf("Could not open file!");
    while(getc(fp)!=EOF){
    	fseek(fp,ftell(fp)-1,SEEK_SET);
    	fscanf(fp,"%d\t%s", &textbuffer[count].statno, textbuffer[count].stat);
        if(count != 0){
           	textbuffer[count-1].next=count;
       	}
       	count++;
	}
    fclose(fp);
    head=textbuffer[0].statno;
    textbuffer[count-1].next=-1;
    char name[40];
    struct node temp;
	i=0;
	strcpy(name,filename);
	while ((name[i] != '.') &&(name[i] !='\0'))  
      		i++;
	if (name[i] == '.')
   		strcpy(name+i, ".dif"); 
	else
    	strcat(name,".dif"); 
	fp = fopen(name,"r");
	if(fp==NULL)
    	printf("Could not open file!");
    while(1){
    	fscanf(fp, "%d", &tempversion);
    	if(tempversion==0)
    		break;
    	else if(tempversion==inputversion+1) 
    		break;
   		while(command!=-1) {
    		fscanf(fp,"%d", &command);
    		if(command==1){
    			fscanf(fp,"%d %s", &temp.statno,temp.stat);
			}
			else if(command==2)
				fscanf(fp,"%d", &temp.statno);
    		if(command==1){
    			Insert(temp.statno,temp.stat);
			}
			else if(command==2){
				deleteLine(temp.statno);
			}
		}
		if(tempversion==version){
			break;
		}
		command=1;
	}
	fclose(fp);
	for(int k=0;k<MAX_NUM;k++){
		if(textbuffer[k].statno< head && textbuffer[k].statno!=0)
			head=textbuffer[k].statno;
	}
}

void Insert(int number, char statement[40]){
	int smallest=MAX_NUM;
	for(int k=0;k<MAX_NUM;k++){
		if(textbuffer[k].statno< smallest && textbuffer[k].statno!=0)
			smallest=textbuffer[k].statno;
	}
	head=smallest;
	textbuffer[count].statno=number;
	for(int j=0;j<40;j++){
		textbuffer[count].stat[j]=statement[j];
	}
	for(int i=0;i<MAX_NUM;i++){
		if(number>textbuffer[i].statno && number<textbuffer[textbuffer[i].next].statno){
			textbuffer[count].next=textbuffer[i].next;
			textbuffer[i].next=count;
			break;
		}
		else if(number>textbuffer[i].statno && textbuffer[i].next==-1){
			textbuffer[i].next=count;
			textbuffer[count].next=-1;
			break;
		}
		else if(number<textbuffer[i].statno && textbuffer[i].statno==head){
			textbuffer[count].next=i;
			head=number;
			break;
		}
	}
	count++;
	diffs[diffcount].code=1;
	diffs[diffcount].statno=number;
	for(int j=0;j<40;j++){
		diffs[diffcount].statement[j]=statement[j];
	}
	diffcount++;
}

void deleteLine(int number){
	int NumToDelete;
	for(int i=0;i<MAX_NUM;i++){	// find the line to delete
		if(number==textbuffer[i].statno){
			NumToDelete=i;
			break;
		}
	}
	for(int i=0;i<MAX_NUM;i++){	// find the previous line and change its next
		if(textbuffer[i].next==NumToDelete && number!=head){
			textbuffer[i].next=textbuffer[NumToDelete].next;
			break;
		}
	}
	struct node tmp;
  	if(NumToDelete == MAX_NUM){//Shift textbuffer array
        textbuffer[NumToDelete].next=0;
        textbuffer[NumToDelete].stat[0]='\0';
        textbuffer[NumToDelete].statno=0;
    }
 	else{
       for(int i=0;i <MAX_NUM;i++){
           if(textbuffer[i].next>NumToDelete){
               textbuffer[i].next--;
           }
           if(i>NumToDelete){
                tmp=textbuffer[i];
                textbuffer[i-1]=tmp;
                textbuffer[i].next=0;
                textbuffer[i].stat[0]='\0';
                textbuffer[i].statno=0;
            }
        }
    }
    int smallest=MAX_NUM;
	for(int k=0;k<MAX_NUM;k++){
		if(textbuffer[k].statno< smallest && textbuffer[k].statno!=0)
			smallest=textbuffer[k].statno;
	}
	head=smallest;
	count--;
	diffs[diffcount].code=2;
	diffs[diffcount].statno=number;
	diffcount++;
}
void print(){
	int j=0;
	int firstnum=MAX_NUM;
	for (int i=0;i<30 ;i++){
		if(textbuffer[i].statno != 0 && textbuffer[i].statno <= firstnum ){
			firstnum=textbuffer[i].statno;
			j=i;
		}
	}
	while(1){
		printf("%d\t%s \n",textbuffer[j].statno,textbuffer[j].stat);	
		if(textbuffer[j].next==-1){
			break;
		}
		j=textbuffer[j].next;
	}
}
void commit(char *filename){
	FILE *fp;
	fp=fopen(filename,"w");
	int j=0;
	int firstnum=MAX_NUM;
	for (int i=0;i<30 ;i++){
		if(textbuffer[i].statno != 0 && textbuffer[i].statno <= firstnum ){
			firstnum=textbuffer[i].statno;
			j=i;
		}
	}
	while(1){
		fprintf(fp,"%d\t%s \n",textbuffer[j].statno,textbuffer[j].stat);	
		if(textbuffer[j].next==-1){
			break;
		}
		j=textbuffer[j].next;
	}
	fclose(fp);
	for(int k=0;k<20;k++){
		diffs[k].code=0;
		diffs[k].statno=0;
		for(int j=0;j<40;j++){
			diffs[k].statement[j]=0;
		}
	}
	diffcount=0;
	filename[strlen(filename)-3]=0;
	strcat(filename,"dif");
    fp=fopen(filename,"w");
    fprintf(fp,"%d",0);
    fclose(fp);
}
void save(char *filename){
	filename[strlen(filename)-3]=0;
	strcat(filename,"dif");
	version++;
	FILE *fp;
	if(version==1)
		fp=fopen(filename,"w");
	else
		fp=fopen(filename,"a");
	fprintf(fp,"%d\n",version);
	while(diffs[temp_count].statno!=0){
		if(diffs[temp_count].statement!=0)
			fprintf(fp,"%d\t%d\t%s\n",diffs[temp_count].code,diffs[temp_count].statno,diffs[temp_count].statement);
		else
			fprintf(fp,"%d\t%d\n",diffs[temp_count].code,diffs[temp_count].statno);
		temp_count++;
	}
	fprintf(fp,"-1\n");
	fclose(fp);
	for(int k=0;k<20;k++){
		textbuffer[k].next=0;
		textbuffer[k].statno=0;
		for(int j=0;j<40;j++){
			textbuffer[k].stat[j]=0;
		}
	}
	for(int k=0;k<20;k++){
		diffs[k].code=0;
		diffs[k].statno=0;
		for(int j=0;j<40;j++){
			diffs[k].statement[j]=0;
		}
	}
	diffcount=0;
}
