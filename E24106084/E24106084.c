#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include<time.h>
struct FunctionalBoard{
	int id;
	int index[5];
	int connectedNum;
	bool valid;
	bool bridged;
};
//typedef struct FunctionalBoard checker;

void readboard(char* filename,int* board);
int readstep(char* filename,char* content);
void writeboard(char* filename,int* board);
//void writestep(char* filename,char* content);
//void renew(int* board,char* sL,char* content,int round,int* pos);

void initializeFB(struct FunctionalBoard* fb,int* chess,int* bridge,bool black);
void wherecanset(struct FunctionalBoard* fb);

int liberty(struct FunctionalBoard* fb,int pos);
void move(struct FunctionalBoard* fb,int* cb,int* bb,char* sL,int round,bool black);
bool setchess(struct FunctionalBoard* fb,int* cb,int* bb,char* sL,int round,bool black);
bool setbridge(struct FunctionalBoard* fb,int* cb,int* bb,char* sL,int round,bool black);
int defense(struct FunctionalBoard* fb,int* chess,int* bb,char* sL,int round,bool black,int* tried);

void printFB(struct FunctionalBoard* fb);
void split(char **arr, char *str, const char *del);

int main(int argc,char *argv[]){
        char color=argv[1][0],VN=argv[2];
        char *chessfile=argv[3];
        char *bridgefile=argv[4];
        char *stepLogfile=argv[5];

        int chessboard[10][10];
        int *cb=chessboard;
        int bridgeboard[10][10];
        int *bb=bridgeboard;
        char stepLog[100][20];
        char *sL=stepLog;
	int round=0;
	bool black;
	
	struct FunctionalBoard functionalboard[10][10];
	struct FunctionalBoard *fb=functionalboard;
	
        int i=0,j=0,k=0;
	if(strcmp(argv[1],"B")==0){
		black=true;
	}else{
		black=false;
	}
        readboard(chessfile,cb);
	
        readboard(bridgefile,bb);
	round=readstep(stepLogfile,sL);
        printf("chessboard is below\n");
        for(i=0;i<100;i++){
                printf("%d",*(cb+i));
                if(i%10 != 9){
                        printf(",");
                }else{
                        printf("\n");
                }
        }
	i=0;
	printf("stepNum is = %d\n",round);
	while(i<round){
		printf("%s",stepLog[i]);
		i++;
	}
	initializeFB(fb,cb,bb,black);
	printFB(fb);
	move(fb,cb,bb,sL,round,black);		
	writeboard(chessfile,cb);
	writeboard(bridgefile,bb);
	//writestep(stepLogfile,stepLog[round]);
        return 0;
}
void readboard(char* filename,int* board){
        FILE *fpr;
        char str[100];
        int i=0,j=0,k=0;
        if(fpr=fopen(filename,"r")){
                for(i=0;i<10;i++){
                        fscanf(fpr,"%s",str);
                        for(j=0;j<10;j++){
                                char *word;
                                if(j==0){
                                        word=strtok(str,",");
                                }else{
                                        word=strtok(NULL,",");
                                }
                                for(k=0;word[k]!='\0';k++){
                                        if(k!=0){
                                                *(board+10*i+j)=10*((int)word[0]-'0')+(int)word[1]-'0';
                                        }else{
                                        	*(board+10*i+j)=(int)word[k]-'0';
					}
                                }
				
                        }
                }
        }else{
                printf("Failed to open the file\n");
        }
        fclose(fpr);
}
int readstep(char* filename,char* content){
        FILE *fpr;
        char str[20];
	char str1[20];
	char *temp=str1;
	char *arr[7];
        int i=0,j=0;
        if(fpr=fopen(filename,"r")){
                while(fgets(str,20,fpr)!=NULL){
			/*for(j=0;str[j]!='\0';j++){
				*(content+20*i+j)=str[j];
			}*/
			strcpy((content+20*i),str);
			split(arr,str,"_");
			if(i<9){
				i=(int)arr[1][0]-'0';
			}else{
				i=((int)arr[1][0]-'0')*10+(int)arr[1][1]-'0';
			}
			printf("i=%d\n",i);
			
		}
        }else{
                printf("Failed to open the file\n");
	}
	fclose(fpr);
	return i;
}
void writeboard(char* filename,int* board){
	FILE *fpr;
	fpr=fopen(filename,"w");
	fclose(fpr);
	fpr=fopen(filename,"a");
	int i=0;
	for(i=0;i<100;i++){
                fprintf(fpr,"%d",*(board+i));
                if(i%10 != 9){
                        fprintf(fpr,",");
                }else{
                        fprintf(fpr,"\n");
                }
        }
	fclose(fpr);
}
/*void writestep(char* filename,char* content){
	FILE *fpr;
	fpr=fopen(filename,"a");
	int i=0,j=0;
	fprintf(fpr,"%s",content);
	fclose(fpr);
}*/
/*void renew(int* board,char* sL,char* content,int round,int* pos){
	if(round%2==1){
		strcpy((sL+round),"B_");
	}else{
		strcpy((sL+round),"W_");
	}
	char str[3];
	char* temp=str;
	int i=0;
	if(round>9){
		str[0]=round/10+'0';
		str[1]=round%10+'0';
	}else{
		str[0]=round%10+'0';
	}
	strcat((sL+round),temp);
	if(strcmp(content,"chess")==0){
		strcat((sL+round),"_chess");
		for(i=0;i<2;i++){
                        str[0]=pos[i]/10+'0';
                        str[1]=',';
                        str[2]=pos[i]%10+'0';
                        strcat((sL+round),"_");
                        strcat((sL+round),temp);
			if(round%2){
				*(board+pos[i])=2;
			}else{
				*(board+pos[i])=1;
			}
                }	
	}else{
		for(i=0;i<2;i++){
                        str[0]=pos[i]/10+'0';
                        str[1]=',';
                        str[2]=pos[i]%10+'0';
                        strcat((sL+round),"_");
                        strcat((sL+round),temp);
                }
		int num=0;
		for(i=0;i<99;i++){
			if(*(board+i)>num){
				num=*(board+i);
			}
		}
		*(board+pos[0])=num;
		*(board+pos[1])=num;	
	}
	printf("stepLog content is %s\n",(sL+round));
}*/

void initializeFB(struct FunctionalBoard* fb,int* chess,int* bridge,bool black){
	int i=0,j=0,k=0;
	int square[16]={-22,-21,-20,-19,-18,-12,-8,-2,2,8,12,18,19,20,21,22};
	for(i=0;i<100;i++){
		(fb+i)->connectedNum=0;
		(fb+i)->bridged=false;
		(fb+i)->valid=true;
		(fb+i)->id=0;
	}
	for(i=0;i<100;i++){
		if(*(chess+i)!=0){
			int right=i+1,down=i+10;
			bool R=false,D=false;
			if((fb+i)->connectedNum==0){
				(fb+i)->index[0]=i;
				(fb+i)->connectedNum=1;
			}
			if(right%10!=0){
				if(*(chess+i)==*(chess+right)){
					if((fb+right)->connectedNum!=0){
						for(j=0;j<(fb+right)->connectedNum;j++){
							bool fine=true;
							for(k=0;k<(fb+i)->connectedNum;k++){
								if((fb+i)->index[k]==(fb+right)->index[j]){
									fine=false;
								}
							}
							if(fine){
								(fb+i)->index[((fb+i)->connectedNum)]=(fb+right)->index[j];
                                                                ((fb+i)->connectedNum)++;
							}
							R=true;
						}
					}else{
						(fb+i)->index[((fb+i)->connectedNum)]=right;
                                                ((fb+i)->connectedNum)++;
                                                R=true;
					}
				}
			}
			if(down<100){
                                 if(*(chess+i)==*(chess+down)){

                                        (fb+i)->index[((fb+i)->connectedNum)]=down;
                                         ((fb+i)->connectedNum)++;
                                        D=true;
                                }
                        }
			for(j=0;j<(fb+i)->connectedNum;j++){
				for(k=0;k<(fb+i)->connectedNum;k++){
					if((fb+i)->index[j]!=i){
						(fb+((fb+i)->index[j]))->index[k]=(fb+i)->index[k];
						(fb+((fb+i)->index[j]))->connectedNum=k+1;
					}
				}
			}
		}
	}
	printf("bridge dealing\n");
	for(i=0;i<100;i++){
		if(*(bridge+i)!=0){
			for(j=0;j<16;j++){
				if((i+square[j]>=0&&(i+square[j]<100))){
					if(*(bridge+i+square[j])==*(bridge+i)){
						int middle=i+square[j]/2;
						if((square[j]%2)*(square[j]%2)==1){
							if(square[j]<0){
								(fb+middle-1)->id=7;
							}else{
								(fb+middle+1)->id=7;
							}
							//(fb+middle+1)->bridged=true;
							(fb+middle)->id=7;
                                                        //(fb+middle)->bridged=true;
                                                        printf("middle=%d\n",middle);
						}else if(j==5||j==6||j==9||j==10){	
							printf("middle=%d,middle-5=%d,middle+5=%d\n",middle,middle-5,middle+5);
							(fb+(middle-5))->id=7;
							//(fb+middle+5)->bridged=true;
							(fb+(middle+5))->id=7;
							//(fb+middle-5)->bridged=true;
							//printf("middle=%d,middle-5=%d,middle+5=%d\n",middle,middle-5,middle+5);
						}else{
							(fb+middle)->id=7;
							 printf("middle=%d\n",middle);
							//(fb+middle)->bridged=true;
						}
						if((fb+i)->connectedNum==4){
							for(k=0;k<4;k++){
								(fb+((fb+i)->index[k]))->bridged=true;
							}
						}
						if((fb+i+square[j])->connectedNum==4){
                                                        for(k=0;k<4;k++){
                                                                (fb+((fb+i+square[j])->index[k]))->bridged=true;
                                                        }
                                                }
						
					}
				}	
                	}
		}
		if((fb+i)->connectedNum!=0){
			if(*(chess+i)==1){
				if(black){
					(fb+i)->id=-1*((fb+i)->connectedNum);
				}else{
					(fb+i)->id=((fb+i)->connectedNum);
				}
			}else{
				if(black){
					(fb+i)->id=((fb+i)->connectedNum);
				}else{
					(fb+i)->id=-1*((fb+i)->connectedNum);
				}
			}
		}
		/*if(i==17||i==29){
			printf("i=%d->connectedNum=%d,id=%d",i,(fb+i)->connectedNum,(fb+i)->id);
		}*/
	}
	wherecanset(fb);
}
void wherecanset(struct FunctionalBoard* fb){
	int i=0,j=0;
	int location[8];
	int locate=0;
	for(i=0;i<10;i++){
                for(j=0;j<10;j++){
                        int x,y,z;
                        if((fb+10*i+j)->id!=0){
				(fb+10*i+j)->valid=false;
			}
			if((fb+10*i+j)->id==3){
				bool limit=false;
				for(y=-1;y<2;y+=2){
					for(x=-1;x<2;x+=2){
						if((i+y>=0)&&(i+y<10)&&(j+x>=0)&&(j+x<10)){
                                                        if(((fb+10*(i+y)+(j+x))->id>=1&&(fb+10*(i+y)+(j+x))->id<=4)){
								limit=true;
								for(z=0;z<(fb+10*(i+y)+(j+x))->connectedNum;z++){
									if((fb+10*(i+y)+(j+x))->index[z]==10*(i)+(j)){
										limit=false;
									}
								}
							}
                                                }
					}
				}
				if(limit){
					for(z=0;z<(fb+10*i+j)->connectedNum;z++){
						//for(y=-1;y<2;y++){
                               			    	//for(x=-1;x<2;x++){
								int temp1=((fb+10*i+j)->index[z]%10);
								int temp10=((fb+10*i+j)->index[z]/10);
                                                		if((temp10-1>=0)){
                                                        		(fb+10*(temp10-1)+temp1)->valid=false;
                                                		}
								if((temp10+1<10)){
                                                                        (fb+10*(temp10+1)+temp1)->valid=false;
                                                                }
								if((temp10%10!=0)){
                                                                        (fb+10*(temp10)+temp1-1)->valid=false;
                                                                }
								if((temp10%10!=9)){
                                                                        (fb+10*(temp10-1)+temp1+1)->valid=false;
                                                                }
                                        		//}
                                		//}
					}
				}
			}
			if((fb+10*i+j)->id==4){
				for(y=-1;y<2;y++){
   	                        	for(x=-1;x<2;x++){
        	                                if((i+y>=0)&&(i+y<10)&&(j+x>=0)&&(j+x<10)){
                        	                	(fb+10*(i+y)+(j+x))->valid=false;
                                	        }
                               		}
                        	}
			}
                        
                }
        }
	printFB(fb);
}
int liberty(struct FunctionalBoard* fb,int pos){
	int num=0;
	bool space=true;
	bool left=false,right=false,up=false,down=false;
	bool l=false,r=false,u=false,d=false;
	if((fb+pos)->id>0&&(fb+pos)->id<=4){
		num+=(fb+pos)->id;
		num--;
		space=false;
	}
	if(pos>9&&(fb+pos-10)->valid){
		num++;
		u=true;
	}
	if(pos>9&&(fb+pos-10)->id>0&&(fb+pos-10)->id<4){
		up=true;
		if(space){
			num+=(fb+pos-10)->connectedNum;
		}
	}
	if(pos<90&&(fb+pos+10)->valid){
                num++;
		d=true;
        }
	if(pos<90&&(fb+pos+10)->id>0&&(fb+pos+10)->id<4){
		down=true;
                if(space){
			num+=(fb+pos+10)->connectedNum;
		}
        }
	if(pos%10!=0&&(fb+pos-1)->valid){
                num++;
		l=true;
        }
	if(pos%10!=0&&(fb+pos-1)->id>0&&(fb+pos-1)->id<4){
		left=true;
		if(space){
			num+=(fb+pos-1)->connectedNum;
		}
	}
	if(pos%10!=9&&(fb+pos+1)->valid){
                num++;
		r=true;
        }
	if(pos%10!=9&&(fb+pos+1)->id>0&&(fb+pos+1)->id<4){
		right=true;
		if(space){
                	num+=(fb+pos+1)->connectedNum;
		}
        }
	/////////////////////////////////////////////////////////////////////////
	if(pos%10>0&&pos>9&&(fb+pos-11)->valid&&(u||l||up||left)){
		num++;
		if(pos%10>0&&pos>19&&(fb+pos-21)->valid){
			num++;
		}
		if(pos%10>1&&pos>9&&(fb+pos-12)->valid){
			num++;
		}
	}
	if(pos%10<9&&pos>9&&(fb+pos-9)->valid&&(u||r||up||right)){
                num++;
		if(pos%10>0&&pos>19&&(fb+pos-19)->valid){
                        num++;
                }
                if(pos%10<8&&pos>9&&(fb+pos-8)->valid){
                        num++;
                }
        }
	if(pos%10>0&&pos<90&&(fb+pos+9)->valid&&(d||l||down||left)){
                num++;
		if(pos%10>0&&pos<80&&(fb+pos+19)->valid){
                        num++;
                }
                if(pos%10>1&&pos<90&&(fb+pos+8)->valid){
                        num++;
                }
        }
	if(pos%10<0&&pos<90&&(fb+pos+11)->valid&&(d||r||down||right)){
		if(pos%10>0&&pos<80&&(fb+pos+21)->valid){
                        num++;
                }
                if(pos%10<8&&pos<90&&(fb+pos+12)->valid){
                        num++;
                }
                num++;
        }
	//////////////////////////////////////////////////////////////////////////
	if(pos>=20&&((fb+pos-20)->valid||((fb+pos-20)->id>0&&(fb+pos-20)->id<4))&&(u||up)){
		if((fb+pos-20)->valid){
			num++;
		}
		if(pos>=30&&(fb+pos-30)->valid){
			num++;
		}
		if(pos%10>0&&(fb+pos-21)->valid){
			num++;
		}
		if(pos%10<9&&(fb+pos-19)->valid){
			num++;
		}
	}
	if(pos<80&&((fb+pos+20)->valid||((fb+pos+20)->id>0&&(fb+pos+20)->id<4))&&(d||down)){
		if((fb+pos+20)->valid){
                        num++;
                }
                if(pos<70&&(fb+pos+30)->valid){
                        num++;
                }
                if(pos%10>0&&(fb+pos+19)->valid){
                        num++;
                }
                if(pos%10<9&&(fb+pos+21)->valid){
                        num++;
                }
	}
	if(pos%10>1&&((fb+pos-2)->valid||((fb+pos-2)->id>0&&(fb+pos-2)->id<4))&&(l||left)){
		if((fb+pos-2)->valid){
                        num++;
                }
                if(pos%10>2&&(fb+pos-3)->valid){
                        num++;
                }
                if(pos>9&&(fb+pos-12)->valid){
                        num++;
                }
                if(pos<90&&(fb+pos+8)->valid){
                        num++;
                }
	}
	if(pos%10<8&&((fb+pos+2)->valid||((fb+pos+2)->id>0&&(fb+pos+2)->id<4))&&(r||right)){
		if((fb+pos+2)->valid){
                        num++;
                }
                if(pos%10<7&&(fb+pos+3)->valid){
                        num++;
                }
                if(pos>9&&(fb+pos-8)->valid){
                        num++;
                }
                if(pos<90&&(fb+pos+12)->valid){
			num++;
		}         
	}
	////////////////////////////////////////////////////////////////////////////
	if(pos>9&&pos%10>0&&(fb+pos-11)->id>0&&(fb+pos-11)->id<4){
		if(!(fb+pos-10)->valid&&!(fb+pos-1)->valid&&!(u||l||up||left)){
			num=-1;
		}
	}
	if(pos>9&&pos%10<9&&(fb+pos-9)->id>0&&(fb+pos-9)->id<4){
                if(!(fb+pos-10)->valid&&!(fb+pos+1)->valid&&!(u||r||up||right)){
                        num=-1;
                }
        }
	if(pos<90&&pos%10>0&&(fb+pos+9)->id>0&&(fb+pos+9)->id<4){
                if(!(fb+pos+10)->valid&&!(fb+pos-1)->valid&&!(d||l||down||left)){
                        num=-1;
                }
        }
	if(pos<90&&pos%10<9&&(fb+pos+11)->id>0&&(fb+pos+11)->id<4){
                if(!(fb+pos+10)->valid&&!(fb+pos+1)->valid&&!(d||r||down||right)){
                        num=-1;
                }
        }
	printf("	pos=%d,liberty=%d\n",pos,num);
	return num;
}
void move(struct FunctionalBoard* fb,int* cb,int* bb,char* sL,int round,bool black){
	char *arr[7];
        char array[]="_";
        const char *del = array;
	char lastmove[20];
	char *str=lastmove;
	int turn=round;
	bool sc;
	if(round<2){
		sc=setchess(fb,cb,bb,sL,round,black);
	}else if(true||round<43){
		strcpy(str,(sL+(round-2)*20));
		printf("str = %s\n",str);
                split(arr,str,del);
		//printf("id is %d\n",(fb+((int)(arr[3][2])-'0')*10+(int)(arr[3][0])-'0')->id);
		if(strcmp(arr[2],"Chess")==0/*&&(fb+((int)*(arr[3]+2)-'0')*10+(int)*(arr[3]+0)-'0')->id==2*/){
			printf("ready to build bridge\n");
			if(!setbridge(fb,cb,bb,sL,round,black)){
				printf("It seems to only set chessQQ\n");
				sc=setchess(fb,cb,bb,sL,round,black);
			}
		}else{
			printf("find chess first\n");
			if(!setchess(fb,cb,bb,sL,round,black)){
				printf("It seems to only set bridgeQQ\n");
				sc=setbridge(fb,cb,bb,sL,round,black);
			}
			
		}
	}else{
		if(!setbridge(fb,cb,bb,sL,round,black)){
			sc=setchess(fb,cb,bb,sL,round,black);
		}
	}
	//sc=setchess(fb,cb,sL,round);	
	printFB(fb);
}
bool setchess(struct FunctionalBoard* fb,int* chess,int* bb,char* sL,int round,bool black){
	
	int location;
	char lastmove[20];
	char *str=lastmove;
	int i=0,j=0;
	int num=0;
	char *arr[7];
	char array[]="_";
	const char *del = array;
	int turn=round/2+1;
	
	int save[2]={-1,-1};
	int lookback=-1;
	int wired[8];
	int wiredNum=0;
	
	bool hinder=false;
	int spot=-1;
	bool unstoptable=false;
	int tryarray[24];
        for(i=0;i<24;i++){
                tryarray[i]=-1;
        }
        int* tried=tryarray;
        if(round>=2){
		strcpy(str,(sL+1*20));
                split(arr,str,del);
                if(!black&&strcmp(arr[2],"Change")==0){
                        turn--;
                }
		strcpy(str,(sL+(round-2)*20));
		split(arr,str,del);
		if(turn==6){
			strcpy(str,(sL+(round-10)*20));
			printf("This is turn 6\n");
                        printf("look for %s",str);
                	split(arr,str,del);
		}
		if(strcmp(arr[2],"Bridge")==0){
			strcpy(str,(sL+(round-4)*20));
			split(arr,str,del);
		}
		//printf("(sL+round-2)=%s\n",str);
        }
	/*if(turn==1){
		int locate;
		time_t t;
              	srand((unsigned) time(&t));
                locate=(rand()%4);
		while(true){
                	time_t t;
                	srand((unsigned) time(&t));
                	location=(rand()%4)+4;
			int start[]={4,5,6,7,4,5,6,7};
			location=start[locate];
			if(black){
				strcpy(str,(sL));
				split(arr,str,del);
				int first=10*((int)*(arr[3]+2)-'0')+((int)*(arr[3]+0)-'0');
				int second=10*((int)*(arr[4]+2)-'0')+((int)*(arr[4]+0)-'0');
				if(first-second==1||first-second==-1||first-second==10||first-second==-10){
					if((first%10==5&&first/10<7&&first/10>3)||(second%10==5&&second/10<7&&second/10>3)){
						*(chess+first)=2;
						*(chess+second)=2;
						return true;
						break;
					}
				}
			}
			if((fb+location*10+5)->valid&&(fb+(location+1)*10+5)->valid&&(fb+location*10+8)->valid){
				(fb+location*10+5)->id=2;
				(fb+(location+1)*10+5)->id=2;
				if(black){
					*(chess+location*10+5)=2;
					*(chess+(location+1)*10+5)=2;
				}else{
					*(chess+location*10+5)=1;
					*(chess+(location+1)*10+5)=1;
				}
				return true;
				break;
			}
			locate++;
		}
        }*/
	if(turn==1){
		location=60;
		while(true){
			if((fb+location+5)->valid&&(fb+location+4)->valid&&((fb+location-20+7)->valid||(fb+location+7)->valid||(fb+location+20+7)->valid)){
				if((fb+location-20+7)->valid&&(fb+location-10+6)->id==0&&(fb+location-20+8)->valid&&(fb+location-20+9)->valid){
					if(black){
						*(chess+location+5)=2;
						*(chess+location-20+7)=2;
					}else{
						*(chess+location+5)=1;
                                                *(chess+location-20+7)=1;
					}
					return true;
				}
				if((fb+location+7)->valid&&(fb+location+6)->id==0&&(fb+location+8)->valid&&(fb+location+9)->valid){
                                        if(black){
                                                *(chess+location+5)=2;
                                                *(chess+location+7)=2;
                                        }else{
                                                *(chess+location+5)=1;
                                                *(chess+location+7)=1;
                                        }
					return true;
                                }
				if(location<80&&(fb+location+20+7)->valid&&(fb+location+10+6)->id==0&&(fb+location+20+8)->valid&&(fb+location+20+9)->valid){
                                        if(black){
                                                *(chess+location+5)=2;
                                                *(chess+location+20+7)=2;
                                        }else{
                                                *(chess+location+5)=1;
                                                *(chess+location+20+7)=1;
                                        }
                                        return true;
                                }
			}
			location+=10;
		}
	}
	if(turn==3){
		bool fine=false;
		int record=0;
                while(true&&!fine){
                        for(i=4;i<10;i++){
				if((fb+10*i+5)->id>0&&(fb+10*i+5)->id<3){
					location=i*10+5;
				}
			}
			int sequence[3]={-1,10,-10};
			fine=false;
			for(i=record;i<3&&!fine;i++){
				if((fb+location+sequence[i])->valid){
                                	location+=sequence[i];
					record=i;
					fine=true;
                        	}
			}
                        if((fb+location)->valid&&((fb+location-22)->valid||(fb+location-2)->valid||(fb+location+18)->valid)){
                                if((fb+location-22)->valid&&(fb+location-11)->id==0&&(fb+10*(location/10-2))->valid&&(fb+10*(location/10-2)+1)->valid){
                                        if(black){
                                                *(chess+location)=2;
                                                *(chess+location-22)=2;
                                        }else{
                                                *(chess+location)=1;
                                                *(chess+location-22)=1;
                                        }
                                        return true;
                                }
                                if((fb+location-2)->valid&&(fb+location-1)->id==0&&(fb+location-4)->valid&&(fb+location-3)->valid){
                                        if(black){
                                                *(chess+location)=2;
                                                *(chess+location-2)=2;
                                        }else{
                                                *(chess+location)=1;
                                                *(chess+location-2)=1;
                                        }
                                        return true;
                                }
				if((fb+location+18)->valid&&(fb+location+9)->id==0&&(fb+10*(location/10+2))->valid&&(fb+10*(location/10+2)+1)->valid){
                                        if(black){
                                                *(chess+location)=2;
                                                *(chess+location+18)=2;
                                        }else{
                                                *(chess+location)=1;
                                                *(chess+location+18)=1;
                                        }
                                        return true;
                                }

                        }
                }
        }
	int loop=40;
	printf("Start to find chessplace\n");
	while(num<2){
		bool finding=true;
		if(true||round<43){
			int targetpos=10*((int)*(arr[3]+2)-'0')+((int)*(arr[3]+0)-'0');
			if(turn==6){
				targetpos=10*((int)*(arr[4]+2)-'0')+((int)*(arr[4]+0)-'0');
			}
			for(i=0;i<2;i++){
				if(save[i]!=-1){
					targetpos=save[i];
				}
			}
			if(turn<=9){
				bool suit=false;
				for(i=0;i<100&&!suit;i++){
					if((fb+i)->id>0&&(fb+i)->id<4&&liberty(fb,(i))>=3){
						targetpos=i;
						suit=true;
					}
				}
			}
			if(turn==8){
				if((targetpos%10==4||targetpos%10==5)&&(fb+targetpos)->id<4&&(fb+targetpos)->id>0){
					for(i=0;i<(fb+targetpos)->connectedNum;i++){
						int temp=(fb+targetpos)->index[i];
						if(temp>=10&&(fb+temp-10)->valid&&temp<90&&(fb+temp+10)->valid){
							if(black){
                                                                *(chess+temp-10)=2;
								*(chess+temp+10)=2;
                                                        }else{
                                                                *(chess+temp-10)=1;
								*(chess+temp+10)=1;
                                                        }
							return true;
						}
					}
				}
			}
			if(turn==9&&num==0){
				bool cut=false;
				for(i=0;i<10;i++){
					if((fb+i*10+9)->id>0&&(fb+i*10+9)->id<=4){	
						//cut=true;
						for(i=0;i<10;i++){
                                        		if((fb+i*10)->id>0&&(fb+i*10)->id<=4){
                                                		cut=true;
                                        		}
                                		}
					}
				}
				/*for(i=0;i<10;i++){
					if((fb+i*10)->id>0&&(fb+i*10)->id<=4){
						cut=true;
					}
				}*/
				if((fb+targetpos)->id==4){
					if(cut){
						for(i=0;i<100&&cut;i++){
							if((fb+i)->id==4){
								targetpos=i;
								cut=false;
							}
						}
					}else{
						if(targetpos<80&&(fb+targetpos+10)->id==0&&(fb+targetpos+20)->valid){
							/*if(black){
								*(chess+targetpos+20)=2;
							}else{
								*(chess+targetpos+20)=1;
							}*/
							targetpos+=20;
							save[0]=targetpos;
							location=targetpos;
							num=0;
							finding=false;
						}
					}
				}
			}
			printf("targetpos is = %d\n",targetpos);
			int y,x;
			//bool finding=true;
			bool maybe=false;
			if((fb+targetpos)->id<4){
				for(i=0;i<(fb+targetpos)->connectedNum;i++){
					if(liberty(fb,(fb+targetpos)->index[i])>=3){
						maybe=true;
					}
				}
				for(i=0;i<(fb+targetpos)->connectedNum&&finding&&turn<9&&targetpos%10>6;i++){
					if((fb+targetpos)->index[i]%10!=9&&(fb+(fb+targetpos)->index[i]+1)->valid&&finding&&((liberty(fb,(fb+targetpos)->index[i]+1)>=3)||maybe)){
						if((liberty(fb,(fb+targetpos)->index[i]+1))!=-1){
                                                	location=(fb+targetpos)->index[i]+1;
                                                	finding=false;
						}
                                        }
				}
				for(i=0;i<(fb+targetpos)->connectedNum&&finding&&turn<20&&targetpos/10>=6&&targetpos%10>=4&&num==0;i++){
					if((fb+targetpos)->index[i]<90&&(fb+(fb+targetpos)->index[i]+10)->valid&&finding&&((liberty(fb,(fb+targetpos)->index[i]+10)>=3)||maybe)){
						if((liberty(fb,(fb+targetpos)->index[i]+10))!=-1){
                                                	location=(fb+targetpos)->index[i]+10;
                                                	finding=false;
						}
                                        }	
				}
				for(i=0;i<(fb+targetpos)->connectedNum&&finding&&targetpos%10==8&&num==0;i++){
					 if((fb+targetpos)->index[i]%10!=9&&(fb+(fb+targetpos)->index[i]+1)->valid&&finding&&((liberty(fb,(fb+targetpos)->index[i]+1)>=3)||maybe)){
                                                if((liberty(fb,(fb+targetpos)->index[i]+1))!=-1){
							location=(fb+targetpos)->index[i]+1;
                                                	finding=false;
						}
                                         }
				}
				for(i=0;i<(fb+targetpos)->connectedNum&&finding;i++){
					printf("check at %d\n",(fb+targetpos)->index[i]);
					if(turn<9&&targetpos%10>6){
						if((fb+targetpos)->index[i]%10!=9&&(fb+(fb+targetpos)->index[i]+1)->valid&&finding&&((liberty(fb,(fb+targetpos)->index[i]+1)>=3)||maybe)){
                                         	       if((liberty(fb,(fb+targetpos)->index[i]+1))!=-1){
								location=(fb+targetpos)->index[i]+1;
                                                		finding=false;
							}
						}
					}
					if(targetpos/10==8){
						if((fb+targetpos)->index[i]<90&&(fb+(fb+targetpos)->index[i]+10)->valid&&finding&&((liberty(fb,(fb+targetpos)->index[i]+10)>=3)||maybe)){
                                                	if((liberty(fb,(fb+targetpos)->index[i]+10))!=-1){
                                                                location=(fb+targetpos)->index[i]+10;
                                                                finding=false;
                                                        }
                                        	}
					}
					if(targetpos%10==8){
						if((fb+targetpos)->index[i]%10!=9&&(fb+(fb+targetpos)->index[i]+1)->valid&&finding&&((liberty(fb,(fb+targetpos)->index[i]+1)>=3)||maybe)){
                                                	if((liberty(fb,(fb+targetpos)->index[i]+1))!=-1){
								location=(fb+targetpos)->index[i]+1;
                                                		finding=false;
							}
                                                }
					}
					if((fb+targetpos)->index[i]%10!=0&&(fb+(fb+targetpos)->index[i]-1)->valid&&finding&&((liberty(fb,(fb+targetpos)->index[i]-1)>=3)||maybe)){
						if((liberty(fb,(fb+targetpos)->index[i]-1))!=-1){
                                                        location=(fb+targetpos)->index[i]-1;
                                                        finding=false;
                                                }
					}
					if(true){
						if((fb+targetpos)->index[i]%10!=9&&(fb+(fb+targetpos)->index[i]+1)->valid&&finding&&((liberty(fb,(fb+targetpos)->index[i]+1)>=3)||maybe)){
                                                	if((liberty(fb,(fb+targetpos)->index[i]+1))!=-1){
                                                                location=(fb+targetpos)->index[i]+1;
                                                                finding=false;
                                                        }
                                        	}
					}
					if((fb+targetpos)->index[i]>=10&&(fb+(fb+targetpos)->index[i]-10)->valid&&finding&&((liberty(fb,(fb+targetpos)->index[i]-10)>=3)||maybe)){
                                                if((liberty(fb,(fb+targetpos)->index[i]-10))!=-1){
                                                        location=(fb+targetpos)->index[i]-10;
                                                        finding=false;
                                                }
                                        }if((fb+targetpos)->index[i]<90&&(fb+(fb+targetpos)->index[i]+10)->valid&&finding&&((liberty(fb,(fb+targetpos)->index[i]+10)>=3)||maybe)){
                                                if((liberty(fb,(fb+targetpos)->index[i]+10))!=-1){
                                                        location=(fb+targetpos)->index[i]+10;
                                                        finding=false;
                                                }
                                        }
					/*if(turn<=6){
						if((fb+targetpos)->index[i]%10!=9&&(fb+(fb+targetpos)->index[i]+1)->valid&&finding&&((liberty(fb,(fb+targetpos)->index[i]+1)>=3)||maybe)){
                                                	location=(fb+targetpos)->index[i]+1;
                                                	finding=false;
                                        	}
					}*/
						
				}
			}else if((fb+targetpos)->id==4){
				int wall[3]={0,-2,2};
				printf("connectedNum is = %d\n",(fb+targetpos)->connectedNum);
				for(i=0;i<4&&finding;i++){
					printf("index=%d\n",(fb+targetpos)->index[i]);
                                        if((fb+targetpos)->index[i]%10>1&&*(bb+(fb+targetpos)->index[i])==0){
						for(j=0;j<3&&finding;j++){
							int pos=(fb+targetpos)->index[i]-2+10*wall[j];
							//bool feasible=true;
							if(turn==6&&(j+i)!=0){
                                                                continue;
                                                        }
							if(pos>=0&&pos<100&&(fb+pos)->valid&&(fb+((fb+targetpos)->index[i]+pos)/2)->id==0&&(liberty(fb,pos)>=3)){
									int direction[4]={1,10,-1,-10};
                                                                        int correspond[4]={-22,-22,22,22};
                                                                        int k;
                                                                        //printf("%d-%d/%d-%d",i/10,(i+square[j])/10,i%10,(i+square[j])%10);
                                                                        if((((fb+targetpos)->index[i]/10-pos/10)/((fb+targetpos)->index[i]%10-pos%10))==1){

                                                                                direction[0]=-1;
                                                                                direction[2]=1;

                                                                                for(k=0;k<4;k++){
                                                                                        if(k<2){
                                                                                                correspond[k]+=4;
                                                                                        }else{
                                                                                                correspond[k]-=4;
                                                                                        }
                                                                                }
                                                                        }
                                                                        bool cross=false;
                                                                        int middle=((fb+targetpos)->index[i]+pos)/2;
                                                                        for(k=0;k<4&&!cross;k++){
                                                                                printf("Test %d on %d,%d\n",middle,middle+direction[k],middle+direction[k]+correspond[k]);
                                                                                if(*(bb+middle+direction[k])!=0&&*(bb+middle+direction[k])==*(bb+middle+direction[k]+correspond[k])){
                                                                                        printf("Due to the bridge set on %d,%d\n",middle+direction[k],middle+direction[k]+correspond[k]);
                                                                                        cross=true;

                                                                                }
                                                                        }
								if(i==0||!cross){
									location=pos;
									finding=false;
									printf("index=%d,j=%d,left\n",(fb+targetpos)->index[i],j);
								}
							}
						}
                                        }
				}
				for(i=0;i<4&&finding;i++){	
					if((fb+targetpos)->index[i]%10<8&&*(bb+(fb+targetpos)->index[i])==0&&finding){
                                                for(j=0;j<3&&finding;j++){
							if(turn==6&&i<2){
								continue;
							}
                                                        int pos=(fb+targetpos)->index[i]+2+10*wall[j];
                                                        if(pos>=0&&pos<100&&(fb+pos)->valid&&(fb+((fb+targetpos)->index[i]+pos)/2)->id==0&&(liberty(fb,pos)>=3)){
                                                                	int direction[4]={1,10,-1,-10};
                                                                        int correspond[4]={-22,-22,22,22};
                                                                        int k;
                                                                        //printf("%d-%d/%d-%d",i/10,(i+square[j])/10,i%10,(i+square[j])%10);
                                                                        if((((fb+targetpos)->index[i]/10-pos/10)/((fb+targetpos)->index[i]%10-pos%10))==1){

                                                                                direction[0]=-1;
                                                                                direction[2]=1;

                                                                                for(k=0;k<4;k++){
                                                                                        if(k<2){
                                                                                                correspond[k]+=4;
                                                                                        }else{
                                                                                                correspond[k]-=4;
                                                                                        }
                                                                                }
                                                                        }
                                                                        bool cross=false;
                                                                        int middle=((fb+targetpos)->index[i]+pos)/2;
                                                                        for(k=0;k<4&&!cross;k++){
                                                                                printf("Test %d on %d,%d\n",middle,middle+direction[k],middle+direction[k]+correspond[k]);
                                                                                if(*(bb+middle+direction[k])!=0&&*(bb+middle+direction[k])==*(bb+middle+direction[k]+correspond[k])){
                                                                                        printf("Due to the bridge set on %d,%d\n",middle+direction[k],middle+direction[k]+correspond[k]);
                                                                                        cross=true;

                                                                                }
                                                                        }
								if(j==0||!cross){
									location=pos;
                                                                	finding=false;
									printf("index=%d,j=%d,right\n",(fb+targetpos)->index[i],j);
								}
                                                        }
                                                }
                                        }
				}for(i=0;i<4&&finding;i++){
						if((fb+targetpos)->index[i]>=20&&*(bb+(fb+targetpos)->index[i])==0&&finding){
                                                for(j=0;j<1&&finding;j++){
                                                        int pos=(fb+targetpos)->index[i]-20+wall[j];
							bool inside=true;
							if((j==1&&pos%10==9)||((j==2)&&pos%10==0)){
								inside=false;
							}
                                                        if(pos>=0&&pos<100&&(fb+pos)->valid&&inside&&(fb+((fb+targetpos)->index[i]+pos)/2)->id==0&&(liberty(fb,pos)>=3)){
                                                                location=pos;
                                                                finding=false;
								printf("index=%d,j=%d,up\n",(fb+targetpos)->index[i],j);
                                                        }
                                              	}
                                        }
				}for(i=0;i<4&&finding;i++){
					if((fb+targetpos)->index[i]<100&&*(bb+(fb+targetpos)->index[i])==0&&finding){
                                                for(j=0;j<1&&finding;j++){
                                                        int pos=(fb+targetpos)->index[i]+20+wall[j];
							bool inside=true;
                                                        if((j==1&&pos%10==9)||((j==2)&&pos%10==0)){
                                                                inside=false;
                                                        }
                                                        if(pos>=0&&pos<100&&(fb+pos)->valid&&inside&&(fb+((fb+targetpos)->index[i]+pos)/2)->id==0&&(liberty(fb,pos)>=3)){
                                                                location=pos;
                                                                finding=false;
								printf("index=%d,j=%d,down\n",(fb+targetpos)->index[i],j);
                                                        }
                                                }
                                        }
					if(!finding){
						save[num]=location;
					}
                                }
				if(!finding){
                                                save[num]=location;
                                }
				if(finding){
					for(i=lookback+1;i<100&&finding;i++){
						if((fb+i)->id==4){
							//strcpy(str,(sL+(round-7*2)*20));
                                			//split(arr,str,del);
                                			finding=false;
                                			location=i;
							save[num]=i;
                                			lookback=i;
                        			}
					}
					if(finding){
						save[num]=-1;
					}
					for(i=save[num]+1;i<100&&finding;i++){
						if((fb+i)->id<4&&((fb+i)->id>0&&(liberty(fb,i)>=3))){
							save[num]=i;
							location=i;
							finding=false;
						}
					}
				}
			}
		}
		if(finding){
			if(!unstoptable&&finding){
                                printf("start to defense\n");
                                i=defense(fb,chess,bb,sL,round,black,tried);
                                if(i==-1){
                                        unstoptable==true;
                                        i=0;
                                }else{
                                        printf("Defense location=%d",i);
                                        location=i;
                                        save[num]=i;
                                        finding=false;
                                }
                        }
			for(i=save[num]+1;i<100;i++){
				if((fb+i)->id>0&&(fb+i)->id<4&&liberty(fb,i)>=3){
					location=i;
					save[num]=i;
					finding=false;
				}
			}
			
			for(i=save[num]+1;i<100&&finding;i++){
				printf("i=%d\n",i);
				if(i==97&&(fb+i)->valid){
					printf("97->valid==ture\n");
				}
				if((fb+i)->valid){
					if(liberty(fb,i)>=3){
						location=i;
						save[num]=i;
						printf("save=%d\n",save[num]);
						finding=false;
					}
				}
			}
			/*if(!unstoptable&&finding){
				printf("start to defense\n");
				i=defense(fb,chess,bb,sL,round,black,tried);
				if(i==-1){
					unstoptable==true;
					i=0;
				}else{
					printf("Defense location=%d",i);
					location=i;
					save[num]=i;
					finding=false;
				}
			}*/
			for(i=save[num]+1;i<100&&finding;i++){
				if((fb+i)->valid){
					location=i;
					save[num]=i;
					finding=false;
				}
			}
		}
		if(finding){
			printf("Nowhere to set chess\n");
			if(num==1){
				*(chess+save[0])=0;
				wired[wiredNum]=save[0];
				save[num]=-1;
				initializeFB(fb,chess,bb,black);
				for(i=0;i<=wiredNum;i++){
					(fb+wired[i])->valid=false;
				}
				wiredNum++;
				num--;
			}
			else{
				printf("chess retrun false\n");
				return false;
			}
		}
		if((fb+location)->valid){
			printf("Set chess at %d\n",location);
			(fb+location)->connectedNum=1;
			(fb+location)->index[0]=location;
			bool block=false;
			if(location>=10){
				if((fb+location-10)->id>0&&(fb+location-10)->id!=7){
					for(i=0;i<(fb+location-10)->connectedNum;i++){
						(fb+location)->index[(fb+location)->connectedNum]=(fb+location-10)->index[i];
						(fb+location)->connectedNum++;
						if((location%10!=0&&(fb+location-10)->index[i]==location-1)||(location%10!=9&&(fb+location-10)->index[i]==location+1)){
							block=true;
						}
					}
					
				}
			}
			if(location%10!=0&&!block){
                                if((fb+location-1)->id>0&&(fb+location-1)->id!=7){
                                        for(i=0;i<(fb+location-1)->connectedNum;i++){
                                                (fb+location)->index[(fb+location)->connectedNum]=(fb+location-1)->index[i];
                                                (fb+location)->connectedNum++;
						if((location>9&&(fb+location-1)->index[i]==location-10)||(location<90&&(fb+location-1)->index[i]==location+10)){
                                                        block=true;
                                                }
                                        }

                                }
                        }
			if(location%10!=9&&!block){
                                if((fb+location+1)->id>0&&(fb+location+1)->id!=7){
                                        for(i=0;i<(fb+location+1)->connectedNum;i++){
                                                (fb+location)->index[(fb+location)->connectedNum]=(fb+location+1)->index[i];
                                                (fb+location)->connectedNum++;
						 if((location>9&&(fb+location+1)->index[i]==location-10)||(location<90&&(fb+location+1)->index[i]==location+10)){
                                                        block=true;
                                                }
                                        }

                                }
                        }
			if(location<90&&!block){
                                if((fb+location+10)->id>0&&(fb+location+10)->id!=7){
                                        for(i=0;i<(fb+location+10)->connectedNum;i++){
                                                (fb+location)->index[(fb+location)->connectedNum]=(fb+location+10)->index[i];
                                                (fb+location)->connectedNum++;
						if((location%10!=0&&(fb+location+10)->index[i]==location-1)||(location%10!=9&&(fb+location+10)->index[i]==location+1)){
                                                        block=true;
                                                }

					}
                                }
                        }
			if((fb+location)->connectedNum==4){
				for(i=0;i<4;i++){
					bool limit=false;
					int dot=(fb+location)->index[i];
					int x,y,z;
					printf("location[%d]=%d\n",i,dot);
                                	for(y=-1;y<2;y+=2){
                                        	for(x=-1;x<2;x+=2){
							int pos10=dot/10+y,pos1=dot%10+x;
                                                	if((pos10>=0)&&(pos10<10)&&(pos1>=0)&&(pos1<10)){
                                                        	if(((fb+10*(pos10)+(pos1))->id>=1&&(fb+10*(pos10)+(pos1))->id<=4)){
                                                                	limit=true;
									if(i<4){
										for(z=0;z<4&&limit;z++){
											if((10*pos10+pos1)==(fb+location)->index[z]){
												printf("%d=index[%d]\n",10*pos10+pos1,z);
												limit=false;
											}
										}
										
									}
									else{
                                                                		for(z=0;z<(fb+10*(pos10)+(pos1))->connectedNum&&limit;z++){	
                                                                        		if((fb+10*(pos10)+(pos1))->index[z]==dot){
                                                                                		limit=false;
												printf("It's me\n");
                                                                        		}
                                                                		}
									}
									if(limit){
										printf("location=%d,pos10=%d,pos1=%d,will let next step no where to set\n",location,pos10,pos1);
										(fb+location)->connectedNum=5;
									}
                                                        	}
                                                	}
                                        	}
                               		}
				}
			}
			if(((fb+location)->connectedNum)>4){
                                (fb+location)->id=0;
                                (fb+location)->valid=false;
				printf("location=%d,connectedNum=%d",location,(fb+location)->connectedNum);
                                save[num]=-1;
                                continue;
                        }
			for(i=1;i<(fb+location)->connectedNum;i++){
				if(i>0){
					for(j=0;j<(fb+location)->connectedNum;j++){
						(fb+((fb+location)->index[i]))->index[j]=(fb+location)->index[j];
					}
					(fb+((fb+location)->index[i]))->connectedNum=(fb+location)->connectedNum;
					(fb+((fb+location)->index[i]))->id=(fb+location)->connectedNum;
				}
			}
			(fb+location)->id=(fb+location)->connectedNum;
			if(black){
                                *(chess+location)=2;
                        }else{
                                *(chess+location)=1;
                        }
			wherecanset(fb);
			printf("%d->%d\n",num,location);
                	num++;
		}
	}
	return true;
}
bool setbridge(struct FunctionalBoard* fb,int* chess,int* bb,char* sL,int round,bool black){
	char *arr[7];
	char array[]="_";
	const char *del = array;
	int turn=round/2+1;
	char lastmove[20];
	char *str=lastmove;
	//int square[16]={-22,-21,-20,-19,-18,-12,-8,-2,2,8,12,18,19,20,21,22};
	int wall[5]={0,1,-1,2,-2};
	int i,j;
	int num=0;
	bool success=false;
	for(i=0;i<100;i++){
		if(*(bb+i)>num){
			num=*(bb+i);
		}
	}
	num++;
	strcpy(str,(sL+1*20));
        split(arr,str,del);
	if(!black&&strcmp(arr[2],"Change")==0){
                turn--;
        }
	if(true||round<43){
		int head[2],tail[2];
		
		strcpy(str,sL+(round-2)*20);
		split(arr,str,del);
		head[0]=((int)arr[4][2]-'0')*10+(int)arr[4][0]-'0';
		head[1]=((int)arr[3][2]-'0')*10+(int)arr[3][0]-'0';
		tail[0]=head[0];
		tail[1]=head[1];
		strcpy(str,sL+(round-4)*20);
		split(arr,str,del);
		if(round>=4){
			tail[0]=((int)arr[4][2]-'0')*10+(int)arr[4][0]-'0';
                	tail[1]=((int)arr[3][2]-'0')*10+(int)arr[3][0]-'0';
		}
		bool begin=false;
		if(turn==2||turn==4){
                        tail[0]=head[1];
			begin=true;
                }
		if(head[0]==tail[0]&&head[1]==tail[1]){
			return false;
		}
		printf("head[0] is = %d\n",head[0]);
		printf("head[1] is = %d\n",head[1]);
		printf("tail[0] is = %d\n",tail[0]);
		printf("tail[1] is = %d\n",tail[1]);
		printf("(fb+tail[0])->id is = %d\n",(fb+tail[1])->connectedNum);
		if(strcmp(arr[2],"Chess")==0&&((fb+tail[0])->id==4)||begin){
			//printf("Here\n");
			for(i=0;i<2&&!success;i++){
				for(j=0;j<2&&!success;j++){
					int temp=(head[i]/10-tail[j]/10)*(head[i]/10-tail[j]/10);
					int distance=temp+((head[i]%10-tail[j]%10)*(head[i]%10-tail[j]%10));
					printf("tail[j]=%d\n",tail[j]);
					switch(distance){
						case 4:
							if((fb+(head[i]+tail[j])/2)->id==0&&*(bb+head[i])==0&&*(bb+tail[j])==0){
								*(bb+head[i])=num;
								*(bb+tail[j])=num;
								(fb+(head[i]+tail[j])/2)->id=7;
								success=true;
							}
							break;
						case 5:
							if(temp==1){
								if((fb+(head[i]+tail[j])/2-5)->id==0&&(fb+(head[i]+tail[j])/2+5)->id==0&&*(bb+head[i])==0&&*(bb+tail[j])==0){
									*(bb+head[i])=num;
									*(bb+tail[j])=num;
									(fb+(head[i]+tail[j])/2-5)->id=7;
									(fb+(head[i]+tail[j])/2+5)->id=7;
									success=true;
								}
							}else{
								if((fb+(head[i]+tail[j])/2)->id==0&&(fb+(head[i]+tail[j])/2+1)->id==0&&*(bb+head[i])==0&&*(bb+tail[j])==0){
                                                                        *(bb+head[i])=num;
                                                                        *(bb+tail[j])=num;
                                                                        (fb+(head[i]+tail[j])/2)->id=7;
                                                                        (fb+(head[i]+tail[j])/2+1)->id=7;
                                                                        success=true;
                                                                }
							}
							break;
						case 8:
							if((fb+(head[i]+tail[j])/2)->id==0&&*(bb+head[i])==0&&*(bb+tail[j])==0){
								printf("In the distance 8\n");
								int direction[4]={1,10,-1,-10};
                                                                int correspond[4]={-22,-22,22,22};
								int k;
								printf("%d-%d/%d-%d",head[i]/10,tail[j]/10,head[i]%10,tail[j]%10);
								if(((head[i]/10-tail[j]/10)/(head[i]%10-tail[j]%10))==1){
									
									direction[0]=-1;
									direction[2]=1;
									
									for(k=0;k<4;k++){
										if(k<2){
											correspond[k]+=4;
										}else{
											correspond[k]-=4;
										}
									}
								}
                                                		bool cross=false;
								int middle=(head[i]+tail[j])/2;
                                                		for(k=0;k<4&&!cross;k++){
                                                        		printf("Test %d on %d,%d\n",middle,middle+direction[k],middle+direction[k]+correspond[k]);
                                                        		if(*(bb+middle+direction[k])!=0&&*(bb+middle+direction[k])==*(bb+middle+direction[k]+correspond[k])){
                                                                		printf("Due to the bridge set on %d,%d\n",middle+direction[k],middle+direction[k]+correspond[k]);
                                                                		cross=true;
										
                                                        		}
                                                		}
								printf("head[i]=%d,tail[j]=%d,keep=%d\n",head[i],tail[j]);
                                                		if(!cross){
                                                        		*(bb+head[i])=num;
                                                        		*(bb+tail[j])=num;
									(fb+middle)->id=7;
                                                        		printf("setbridge on %d,%d\n",head[i],tail[j]);
                                                        		success=true;
                                               	 		}
							}
							break;
						default:
							
							break;
					}
				}
			}		
		}
		
		if(!success){
			for(i=0;i<100&&!success;i++){
				int specify=0;
				if(i==-2){
					i=head[0]*10+head[1];
					specify=-2;
				}
				if(i==-1){
					i=tail[0]*10+tail[1];
					specify=-1;
				}
				if(((fb+i)->id==4||((fb+i)->id==2)&&liberty(fb,i)>=3)&&!(fb+i)->bridged&&*(bb+i)==0){
					int square[]={-20,20,-2,2,-22,-18,18,22,-21,-19,19,21,-12,-8,8,12};
					for(j=0;j<16&&!success;j++){
						if((fb+i+square[j])->id==4&&*(bb+i+square[j])==0){
							if(((j==0&&i>19)||(j==1&&i<80))){
								if((fb+i+square[j]/2)->id==0){
									printf("Find isolated island which can be connected at%d,%d,j=%d\n1\n",i,i+square[j],j);
									*(bb+i)=num;
									*(bb+i+square[j])=num;
									(fb+i+square[j]/2)->id=7;
									success=true;
								}
							}else if(((j==2&&i%10>1)||(j==3&&i%10<8))){
								printf("i%10=%d\n",i%10);
								printf("Why\n");
								if((fb+i+square[j])->id==4&&*(bb+i+square[j])==0&&(fb+i+square[j]/2)->id==0){
                                                                        printf("Find isolated island which can be connected at%d,%d,j=%d\nHere2\n",i,i+square[j],j);
                                                                        *(bb+i)=num;
                                                                        *(bb+i+square[j])=num;
                                                                        (fb+i+square[j]/2)->id=7;
                                                                        success=true;
                                                                }
							}else if((i>19&&((j==4&&i%10>2)||(j==5&&i%10<8)))||(i<80&&((j==6&&i%10>=2)||(j==7&&i%10<8)))){
								if((fb+i+square[j]/2)->id==0){
									int direction[4]={1,10,-1,-10};
                                                                	int correspond[4]={-22,-22,22,22};
                                                                	int k;
                                                                	printf("%d-%d/%d-%d",i/10,(i+square[j])/10,i%10,(i+square[j])%10);
                                                                	if(((i/10-(i+square[j])/10)/(i%10-(i+square[j])%10))==1){

                                                                        	direction[0]=-1;
                                                                        	direction[2]=1;

                                                                        	for(k=0;k<4;k++){
                                                                                	if(k<2){
                                                                                        	correspond[k]+=4;
                                                                                	}else{
                                                                                        	correspond[k]-=4;
                                                                                	}
                                                                        	}
                                                                	}
                                                                	bool cross=false;
                                                                	int middle=i+(square[j])/2;
                                                                	for(k=0;k<4&&!cross;k++){
                                                                        	printf("Test %d on %d,%d\n",middle,middle+direction[k],middle+direction[k]+correspond[k]);
                                                                        	if(*(bb+middle+direction[k])!=0&&*(bb+middle+direction[k])==*(bb+middle+direction[k]+correspond[k])){
                                                                                	printf("Due to the bridge set on %d,%d\n",middle+direction[k],middle+direction[k]+correspond[k]);
                                                                                	cross=true;

                                                                        	}
                                                                	}
                                                                	if(!cross){
										printf("Find isolated island which can be connected at%d,%d,j=%d\n3\n",i,i+square[j],j);
                                                                        	*(bb+i)=num;
                                                                        	*(bb+i+square[j])=num;
                                                                        	(fb+middle)->id=7;
                                                                        	success=true;
                                                                	}
								}
							}else if(((j==8&&i>19&&i%10>0)||(j==9&&i>19&&i%10<9))){
								if((fb+i-10)->id==0&&(fb+i+square[j]+10)->id==0){
									printf("Find isolated island which can be connected at%d,%d,j=%d\n4\n",i,i+square[j],j);
									*(bb+i)=num;
									*(bb+i+square[j])=num;
									(fb+i-10)->id=7;
									(fb+i+square[j]+10)->id=7;
									success=true;
								}
							}else if(((j==10&&i<80&&i%10>0)||(j==11&&i<80&&i%10<9))){
								if((fb+i+10)->id==0&&(fb+i+square[j]-10)->id==0){
                                                                        printf("Find isolated island which can be connected at%d,%d,j=%d\n5\n",i,i+square[j],j);
                                                                        *(bb+i)=num;
                                                                        *(bb+i+square[j])=num;
                                                                        (fb+i+10)->id=7;
                                                                        (fb+i+square[j]-10)->id=7;
                                                                        success=true;
                                                                }
							}else if((i>9&&((j==12&&i%10>2)||(j==13&&i%10<8)))||(i<90&&((j==14&&i%10>2)||(j==15&&i%10<8)))){
								printf("i%10\n",i%10);
								if((fb+i+square[j]/2-5)->id==0&&(fb+i+square[j]/2+5)->id==0){
									printf("Find isolated island which can be connected at%d,%d,j=%d\n6\n",i,i+square[j],j);
									*(bb+i)=num;
									*(bb+i+square[j])=num;
									(fb+i+square[j]/2-5)->id=7;
									(fb+i+square[j]/2+5)->id=7;
									success=true;
								}
							}
						}
					}
				}
				if(specify<0){
					i=specify+1;
				}
			}
		}
	}
	if(!success){
		printf("\n");
		if(true){
			printf("go to seek chess\n");
			success=setchess(fb,chess,bb,sL,round,black);
			printf("mission on\n");
		}
		printf("Find the place to bulid bridge desperately\n");
		int color;
		//int direction[4];
		//int correspond[4];
		//bool cross;
		bool unstoptable=false;
		int tryarray[24];
		for(i=0;i<24;i++){
			tryarray[i]=-1;
		}
		int* tried=tryarray;
		if(black){
			color=2;
		}else{
			color=1;
		}
		for(i=0;i<100&&!success;i++){
			if(!unstoptable){
				i=defense(fb,chess,bb,sL,round,black,tried);
				if(i==-1){
					i=0;
					unstoptable=true;
				}
			}
			if(i>=10&&i<90&&i%10!=9&&(fb+i)->id==0&&(fb+i+1)->id==0){
				for(j=0;j<2&&!success;j++){
					if(*(chess+i-10+j)==color&&*(chess+i+11-j)==color&&*(bb+i-10+j)==0&&*(bb+i+11-j)==0){
						*(bb+i-10+j)=num;
						*(bb+i+11-j)=num;
						 printf("setbridge on %d,%d\n",i-10+j,i+11-j);
						success=true;
					}
				}
			}
			if(!success&&(i%10!=0)&&(i%10!=9)&&(i<90)&&(fb+i)->id==0&&(fb+i+10)->id==0){
				for(j=0;j<2&&!success;j++){
                                        if(*(chess+i-1+10*j)==color&&*(chess+i+11-10*j)==color&&*(bb+i-1+10*j)==0&&*(bb+i+11-10*j)==0){
                                                *(bb+i-1+10*j)=num;
                                                *(bb+i+11-10*j)=num;
                                                 printf("setbridge on %d,%d\n",i-1+10*j,i+11-10*j);
                                                success=true;
                                        }
                                }
			}
			if(!success&&(fb+i)->id==0){
				if(i>=10&&i<90){
					if(*(chess+i-10)==color&&*(chess+i+10)==color&&*(bb+i-10)==0&&*(bb+i+10)==0){
						*(bb+i-10)=num;
						*(bb+i+10)=num;
						printf("setbridge on %d,%d\n",i-10,i+10);
						success=true;
					}
				}
				if((i%10!=0)&&(i%10!=9)&&!success){
					if(*(chess+i-1)==color&&*(chess+i+1)==color&&*(bb+i-1)==0&&*(bb+i+1)==0){
                                                *(bb+i-1)=num;
                                                *(bb+i+1)=num;
                                                printf("setbridge on %d,%d\n",i-1,i+1);
						success=true;
                                        }
				}
				if(i>=10&&i<90&&(i%10!=0)&&(i%10!=9)&&!success){
					if(*(chess+i-11)==color&&*(chess+i+11)==color&&*(bb+i-11)==0&&*(bb+i+11)==0){
						int direction[4]={-1,10,-10,1};
						int correspond[4]={-18,-18,18,18};
						bool cross=false;
						for(j=0;j<4&&!cross;j++){
							if(*(bb+i+direction[j])!=0&&*(bb+i+direction[j])==*(bb+i+direction[j]+correspond[j])){
								cross=true;
							}
						}
						if(!cross){
                                                	*(bb+i-11)=num;
                                                	*(bb+i+11)=num;
                                                	printf("setbridge on %d,%d\n",i-11,i+11);
                                                	success=true;
						}
                                        }
					if(*(chess+i-9)==color&&*(chess+i+9)==color&&*(bb+i-9)==0&&*(bb+i+9)==0&&!success){
						int direction[4]={1,10,-1,-10};
						int correspond[4]={-22,-22,22,22};
						bool cross=false;
						for(j=0;j<4&&!cross;j++){
							printf("Test %d on %d,%d\n",i,i+direction[j],i+direction[j]+correspond[j]);
							if(*(bb+i+direction[j])!=0&&*(bb+i+direction[j])==*(bb+i+direction[j]+correspond[j])){
								printf("Due to the bridge set on %d,%d\nThis place can't not bridge\n",i+direction[j],i+direction[j]+correspond[j]);
								cross=true;
							}
						}
						if(!cross){
							*(bb+i-9)=num;
							*(bb+i+9)=num;
							printf("setbridge on %d,%d\n",i-9,i+9);
							success=true;
						}
					}
				}
			}
		}
	}
	return success;
}
int defense(struct FunctionalBoard* fb,int* chess,int* bb,char* sL,int round,bool black,int* tried){
	char *arr[7];
        char array[]="_";
        const char *del = array;
        char lastmove[20];
        char *str=lastmove;
	strcpy(str,(sL+(round-1)*20));
	split(arr,str,del);
	int i,j,k,t;
	bool finding=true;
	int block[]={-10,10,-1,1,-11,-9,9,11};
	int save=-1;
	/*for(i=0;i<24&&first;i++){
		tried[i]=-1;
	}*/
	if((black&&(int)arr[0][0]=='W')||(!black&&(int)arr[0][0]=='B')){
		int enemy[2];
		enemy[0]=((int)arr[4][2]-'0')*10+(int)arr[4][0]-'0';
		enemy[1]=((int)arr[3][2]-'0')*10+(int)arr[3][0]-'0';
		for(i=0;i<2&&finding;i++){
			if((fb+enemy[i])->id==-4){
				for(j=0;j<((fb+enemy[i])->connectedNum)&&finding;j++){
					int pos=((fb+enemy[i])->index[j]);
					if(*(bb+pos)!=0){
						continue;
					}
					for(k=0;k<8&&finding;k++){
						if(k==0&&pos>19&&(fb+pos+block[k])->id==0&&(fb+pos+2*block[k])->id<0){
							bool new=true;
							int count=0;
							for(t=0;*(tried+t)!=-1;t++){
								if(*(tried+t)==pos+block[k]){
									new=false;
								}
							}
							if(new){
								save=pos+block[k];
								*(tried+t)=save;
								finding=false;
							}
						}
						if(k==1&&pos<80&&(fb+pos+block[k])->id==0&&(fb+pos+2*block[k])->id<0){
                                                        bool new=true;
                                                        for(t=0;*(tried+t)!=-1;t++){
                                                                if(*(tried+t)==pos+block[k]){
                                                                        new=false;
                                                                }
                                                        }
                                                        if(new){
                                                                save=pos+block[k];
								*(tried+t)=save;
                                                                finding=false;
                                                        }
                                                }
						if(k==2&&pos%10>1&&(fb+pos+block[k])->id==0&&(fb+pos+2*block[k])->id<0){
                                                        bool new=true;
                                                        for(t=0;*(tried+t)!=-1;t++){
                                                                if(*(tried+t)==pos+block[k]){
                                                                        new=false;
                                                                }
                                                        }
                                                        if(new){
                                                                save=pos+block[k];
								*(tried+t)=save;
                                                                finding=false;
                                                        }
                                                }
						if(k==3&&pos%10<8&&(fb+pos+block[k])->id==0&&(fb+pos+2*block[k])->id<0){
                                                        bool new=true;
                                                        for(t=0;*(tried+t)!=-1;t++){
                                                                if(*(tried+t)==pos+block[k]){
                                                                        new=false;
                                                                }
                                                        }
                                                        if(new){
                                                                save=pos+block[k];
								*(tried+t)=save;
                                                                finding=false;
                                                        }
                                                }
						if(k==4&&pos>19&&pos%10>1&&(fb+pos+block[k])->id==0&&(fb+pos+2*block[k])->id<0){
                                                        bool new=true;
                                                        for(t=0;*(tried+t)!=-1;t++){
                                                                if(*(tried+t)==pos+block[k]){
                                                                        new=false;
                                                                }
                                                        }
                                                        if(new){
                                                                save=pos+block[k];
								*(tried+t)=save;
                                                                finding=false;
                                                        }
                                                }
						if(k==5&&pos>19&&pos%10<8&&(fb+pos+block[k])->id==0&&(fb+pos+2*block[k])->id<0){
                                                        bool new=true;
                                                        for(t=0;*(tried+t)!=-1;t++){
                                                                if(*(tried+t)==pos+block[k]){
                                                                        new=false;
                                                                }
                                                        }
                                                        if(new){
                                                                save=pos+block[k];
								*(tried+t)=save;
                                                                finding=false;
                                                        }
                                                }
						if(k==6&&pos<80&&pos%10>1&&(fb+pos+block[k])->id==0&&(fb+pos+2*block[k])->id<0){
                                                        bool new=true;
                                                        for(t=0;*(tried+t)!=-1;t++){
                                                                if(*(tried+t)==pos+block[k]){
                                                                        new=false;
                                                                }
                                                        }
                                                        if(new){
                                                                save=pos+block[k];
								*(tried+t)=save;
                                                                finding=false;
                                                        }
                                                }
						if(k==7&&pos<80&&pos%10<8&&(fb+pos+block[k])->id==0&&(fb+pos+2*block[k])->id<0){
                                                        bool new=true;
                                                        for(t=0;*(tried+t)!=-1;t++){
                                                                if(*(tried+t)==pos+block[k]){
                                                                        new=false;
                                                                }
                                                        }
                                                        if(new){
                                                                save=pos+block[k];
								*(tried+t)=save;
                                                                finding=false;
                                                        }
                                                }
						
					}
				}
			}
		}
		for(i=0;i<2&&finding;i++){
			if((fb+enemy[i])->id<0&&(fb+enemy[i])->id!=-4){
				for(j=0;j<(fb+enemy[i])->connectedNum&&finding;j++){
					int pos=(fb+enemy[i])->index[j];
					for(k=0;k<4;k++){
						if(k==0&&pos>9&&(fb+pos+block[k])->valid){
							save=pos+block[k];
							finding=false;
						}
						if(k==1&&pos<90&&(fb+pos+block[k])->valid){
                                                        save=pos+block[k];
                                                        finding=false;
                                                }
						if(k==2&&pos%10>0&&(fb+pos+block[k])->valid){
                                                        save=pos+block[k];
                                                        finding=false;
                                                }
						if(k==3&&pos%10<9&&(fb+pos+block[k])->valid){
                                                        save=pos+block[k];
                                                        finding=false;
                                                }
					}
				}
			}
		}
	}
	return save;
}
void printFB(struct FunctionalBoard* fb){
	int i=0;
	printf("functionalboard is below\n");
        for(i=0;i<100;i++){
		if((fb+i)->id<0){
                	printf("%d",(fb+i)->id);
		}else{
			printf(" %d",(fb+i)->id);	
		}
                if(i%10 != 9){
                        printf(",");
                }else{
                        printf("\n");
                }
		
        }
	printf("Valid location is below\n");
	for(i=0;i<100;i++){
		if((fb+i)->valid){
			printf(" 1");
		}else{
			printf(" 0");
		}
		 if(i%10 != 9){
                        printf(",");
                }else{
                        printf("\n");
                }
	}
}
void split(char **arr, char *str, const char *del) {
	char c;
   char *s = strtok(str, del);
  
   while(s != NULL) {
     *arr++ = s;
     s = strtok(NULL, del);
   }
 }
