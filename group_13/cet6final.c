/********************************************************************/
/*                           CET6.C                                 */
/* 六级背单词程序                                                   */
/* Designed by: 34160301  Liu Ruojin                                */
/*              34160317  Zou Rong                                  */
/*              34160319  Huang Jialing                             */
/* (C)Copyright all rights reserved. Jun 2018                       */
/********************************************************************/
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <conio.h>
#include <string.h>

#define MAXLIST 100    //学生/教师最大人数
#define MAXID 11	   //ID最大长度
#define MAXSEX 11	   //性别字符串最大长度
#define MAXCODE 11	 
#define TELSIZE 12	   //电话号码最大长度
#define MAXPWD 19	   //用户密码最大长度
#define TIMESIZE 19    
#define MAXNAME 101	   //姓名最大长度
#define MAXPROBLEM 500 //测验题内容最大长度
#define MAXANSWER 500  //测验题答案最大长度
#define MAXSTART 255 
#define MAXCLASS 100   //最多班级数
#define HOW_MANY_QUESTION_IN_TEST 10  //测验中题数
#define PROBLEM_AMOUNT 400    //测验题数


void LoadStudentFile( FILE *StFp );
void LoadTeacherFile( FILE *TeFp );
void LoadAdminFile( FILE *AdFp );
void LoadProblemFile( FILE *PrFp );
void LoadRankFile( FILE *RaFp );
void fprintf_st(FILE *STFP,int line);
void fprintf_te(FILE *TEFP,int line);
void fprintf_ra(FILE *RAFP,int line);
void swapstr(char str1[],char str2[],int size);
void swap(int *p1,int *p2);
void swapfloat(float *p1,float *p2);
int myGets(char chr[],int charsize,int type);

int getID(char[]);			//获取ID，并判断是否超过长度 
int getName(char []);		//获取姓名 
int getPwd(char []);		//获取密码，密文输入 
int getSex(char []);
void Homepage();
void Student();
void Administrator();
void STchoose(char[]);
void StGoRank(char[]);
void Teacher();
void TeLogin();
void ADLogin();
void TEchoose(char[]);
void DisplayTextPicture(const char startFileName[]);
int FindLineNum(const char[]);
void TEsearch(char[]);
void StGoShop(char[]);
void SystemData();
void TeMakeComment(char []);

/* 学生信息数据结构 */
struct STData
{
	char STid[MAXID];     //id
	char STcode[MAXPWD];  //学号
	char STname[MAXNAME]; //学生姓名
	char STphone[TELSIZE];//联系电话
	char STsex[MAXSEX];   //性别
	int  TEclass;         //班级
	int  STbestscore;     //最高得分
	float STbestfinishtime; //最高完成时间
	int  STmoney;           //金额
	int tips;               
	int pikaqiu;
	char TEcomment[20];     //教师评价
	char degree[PROBLEM_AMOUNT+1];  //单词掌握成熟度信息
};

/*管理员信息*/
struct ADData
{
	char ADid[MAXID];
	char ADcode[MAXPWD];
};

/* 教师信息 */
struct TEData
{
	char TEid[MAXID];       //id
	char TEcode[MAXPWD];    //教师编号
	int  TEclass;           //负责班级
	char TEsex[MAXSEX];     //性别
	char TEname[MAXNAME];   //姓名
	char TEphone[TELSIZE];  //联系电话
};

/* 问题数据 */
struct PRData
{
	char Problem[MAXPROBLEM];
	char Answer[MAXANSWER];
};

/* 排名信息 */
struct RANDData
{
	int rank; 
	char rank_id[MAXID];
	char rank_name[MAXNAME];
	int Class;
	int rank_score;
	float finish_time;
};

struct STData 	stlist[MAXLIST];   //学生
struct TEData 	telist[MAXLIST];   //教师
struct PRData 	prlist[MAXPROBLEM];//问题
struct RANDData ralist[MAXLIST];   //排名
struct ADData   adlist[MAXLIST];   //管理员

FILE *stdata,*stdata2,*tedata,*tedata2,
	 *prdata,*prdata2,*radata,*radata2,
	 *addata,*addata2;


/* 主程序 */
int main()
{
	system("mode CON:COLS=151 LINES=50"); //设置命令行窗口大小
    system("color F0"); //设置命令行窗口背景颜色

	/* 显示启动画面 */
	DisplayTextPicture("MainStart.txt");
	_getch();
	system("cls");
	
	/* 显示主菜单画面，并根据用户输入执行相应功能 */
	Homepage();

	return 0;
}/*main*/


/* 显示主菜单，等待用户输入，执行指定的功能 */
void Homepage()
{
	char choice;

	/* 显示主菜单画面 */
	DisplayTextPicture("Homepage.txt");
	
	/* 读取用户输入 */
	choice = _getch();
	switch ( choice )
	{
		case '1': /* 管理员模块 */
			Administrator();
			break;
		case '2': /* 学生模块 */
			Student();
			break;
		case '3': /* 教师模块*/
			Teacher();
			break;
		default:
			exit(0);
	}
}


/* 从文件中读取学生数据 */
void LoadStudentFile( FILE *StFp )
{
	int j, k, lineNum;

	lineNum = FindLineNum( "stdata.txt" );
	for( j = 0; j < lineNum; j++ )
	{
		fscanf(StFp,"%s %s %s %s %s %d %d %f %d %d %d %s %s",stlist[j].STid,stlist[j].STcode,
		stlist[j].STname,stlist[j].STphone,stlist[j].STsex,
		&(stlist[j].TEclass),&(stlist[j].STbestscore),&(stlist[j].STbestfinishtime),
		&(stlist[j].STmoney),&(stlist[j].tips),&(stlist[j].pikaqiu),stlist[j].TEcomment,stlist[j].degree);
	
		/* 检查并校准单词掌握熟练度：如果是无效的熟练度值，将其初始化为初级'0' */
		for ( k = 0; k < PROBLEM_AMOUNT; k++ )
			if ( stlist[j].degree[k] < '0' || stlist[j].degree[k] > '3' )
				stlist[j].degree[k] = '0';
        stlist[j].degree[k] = '\0';
	}/*for j*/
}

/* 从文件中读取教师数据 */
void LoadTeacherFile( FILE *TeFp )
{
	int j, lineNum;

	lineNum = FindLineNum( "tedata.txt" );
	for( j = 0; j < lineNum; j++ )
		fscanf(TeFp,"%s %s %d %s %s %s",telist[j].TEid,telist[j].TEcode,&telist[j].TEclass,
		telist[j].TEsex,telist[j].TEname,telist[j].TEphone);
}

/* 从文件中读取管理员数据 */
void LoadAdminFile( FILE *AdFp )
{
	int j, lineNum;
	lineNum = FindLineNum( "addata.txt" );
	for( j = 0; j < lineNum; j++ )
		fscanf(AdFp,"%s %s",adlist[j].ADid,adlist[j].ADcode);
}

/* 从文件中读取问题数据 */
void LoadProblemFile( FILE *PrFp )
{
	int j, lineNum;

	lineNum = FindLineNum( "prdata.txt" );
	for( j = 0; j < lineNum; j++ )
		fscanf(PrFp,"%s %s",prlist[j].Problem,prlist[j].Answer);
}

/* 从文件中读取排名数据 */
void LoadRankFile( FILE *RaFp )
{
	int j, lineNum;

	lineNum = FindLineNum( "radata.txt" );
	for( j = 0; j < lineNum; j++ )
		fscanf(RaFp,"%d %s %s %d %d %f",&ralist[j].rank,ralist[j].rank_id,ralist[j].rank_name,
		&ralist[j].Class,&ralist[j].rank_score,&ralist[j].finish_time);
}

/* 将学生数据写入到文件中 */
void fprintf_st(FILE *STFP,int line)
{
	int j;
	for(j=0;j<line;j++)
	{
		fprintf(STFP,"%s %s %s %s %s %d %d %f %d %d %d %s %s\n",stlist[j].STid,stlist[j].STcode,
		stlist[j].STname,stlist[j].STphone,stlist[j].STsex,stlist[j].TEclass,
		stlist[j].STbestscore,stlist[j].STbestfinishtime,stlist[j].STmoney,stlist[j].tips,stlist[j].pikaqiu,stlist[j].TEcomment,stlist[j].degree);
	}
}

/* 将教师数据写入到文件中 */
void fprintf_te(FILE *TEFP,int line)
{
	int j;
	for(j=0;j<line;j++)
	{
		fprintf(TEFP,"%s %s %d %s %s %s\n",telist[j].TEid,telist[j].TEcode,telist[j].TEclass,
		telist[j].TEsex,telist[j].TEname,telist[j].TEphone);
	}
}

/* 将排名数据写入到文件中 */
void fprintf_ra(FILE *RAFP,int line)
{
	int j;
	for(j=0;j<line;j++)
	{
		fprintf(RAFP,"%d %s %s %d %d %f\n",ralist[j].rank,ralist[j].rank_id,ralist[j].rank_name,
		ralist[j].Class,ralist[j].rank_score,ralist[j].finish_time);
	}
}

/* 交换两个字符串 */
void swapstr(char str1[],char str2[],int size)
{
	char str3[10];
	strcpy(str3,str1);
	strcpy(str1,str2);
	strcpy(str2,str3);	
}

/* 交换两个整数 */
void swap(int *p1,int *p2)
{  
	int temp=*p1;  
	*p1=*p2;  
	*p2=temp;
}    

/* 交换两个浮点数 */
void swapfloat(float *p1,float *p2)
{  
	float temp=*p1;  
	*p1=*p2;  
	*p2=temp;
}

/* 读取用户输入的一行内容 */
int myGets(char chr[],int charsize,int type)
{
	char temp,ch;
	int i=1;
	memset(chr,0,charsize);
	
	temp = _getch();
	if(temp==0x1B)
	{
		printf("\n");
		Homepage();
	}
	else
	{
		chr[0]=temp;
		if (type==0)
			printf("%c",chr[0]);
		else
			printf("*");
		while((ch = _getch())!='\r')
	    {
	        if(i>=charsize)     //当字符串超过字数限制 
	        {   
	        	if(ch == '\b')
	        	{
	        		i--;
	                printf("\b \b");
	                chr[i] = 0;
	                continue;
				}
				else
				{
	            memset(chr,0,charsize);
	            chr[0]=temp;
	            i = 0;
	            printf("\n");
	            printf("\t* Warning! Out of bonds! Enter again: ");
	            continue;					
				}
	        }    //当键入退格键时
	        if(ch == '\b')
	        { 
	            if(i>0)
	            {
	                i--;
	                printf("\b \b");
	                chr[i] = 0;
	                continue;
	            }
	            else
	            {
	                printf("\a");     //没有内容的时候 
	                continue;
	            }
	        }
	        else
	        {
	        	if (type==0)
	        	{
	        		chr[i] = ch;
	            	printf("%c",chr[i]);
				}
	            else if (type==1)
	            {
	            	chr[i] = ch;
	            	printf("*");	
				}              
	        }
	        i++;
    	}
    	printf("\n");
	}
	return 1;
}

/* 从指定文件中读取文本画面并显示 */
void DisplayTextPicture(const char startFileName[])
{
	char temp[MAXSTART]="";
	FILE *rFile;
	
	if((rFile=fopen( startFileName,"r"))==NULL)//打开文件打不开报错 
	{
		printf("\nFailed to open the data file.\n");
		exit(1); 
	}
	
	while(fgets(temp,MAXSTART,rFile)!= NULL)
	{
		printf("%s",temp);
	}
	fclose(rFile);
}

/* 读取文件的行数 */
int FindLineNum(const char name[])
{
	/*input the file name
		output the lines*/
	int linenum=0,c;
	FILE *fp;
	
	if((fp=fopen(name ,"r"))==NULL){
		printf("\nFailed to open the data file.\n");
		exit(1); 
	} 	
	
	if (fp!=NULL)
	{
		while ((c=fgetc(fp))!=EOF)
		{ 
			if(c=='\n')
				linenum++;
		}
	}
	fclose(fp);
	return linenum;
}

/* 显示学生数据的记事本所有内容，然后返回主界面 */
void StudentData()
{
	char st;
	if((stdata=fopen("stdata.txt","r"))==NULL)//stdata??
	{
		printf("\nFailed to open the data file.\n");
		exit(1); 
	}
	st=fgetc(stdata);

	while(st!=EOF)
	{
		putchar(st);
		st=fgetc(stdata);
	}
	
	fclose(stdata);
	printf("\t---------------------------------------------------\n");
	printf("\t* Press any key to open other data files.         *\n");
	printf("\t---------------------------------------------------\n\n");
	_getch();
	SystemData();
}

/* 显示教师数据 */
void TeacherData()
{
	char te;
	if((tedata=fopen("tedata.txt","r"))==NULL)
	{
		printf("\nFailed to open the data file.\n");
		exit(1); 
	}
	te=fgetc(tedata);

	while(te!=EOF)
	{
		putchar(te);
		te=fgetc(tedata);
	}
	
	fclose(tedata);
	printf("\t---------------------------------------------------\n");
	printf("\t* Press any key to open other data files.         *\n");
	printf("\t---------------------------------------------------\n\n");
	_getch();
	SystemData();
}

/* 显示所有测验题 */
void ProblemData()
{
	char pr;
	if((prdata=fopen("prdata.txt","r"))==NULL)
	{
		printf("\nFailed to open the data file.\n");
		exit(1); 
	}
	pr=fgetc(prdata);
	
	while(pr!=EOF)
	{
		putchar(pr);
		pr=fgetc(prdata);
	}
	
	fclose(prdata);
	printf("\t---------------------------------------------------\n");
	printf("\t* Press any key to open other data files.         *\n");
	printf("\t---------------------------------------------------\n\n");
	_getch();
	SystemData();
}

void SystemData()
{
	char choice;

	system("cls");
	printf("\t---------------------------------------------------\n");
	printf("\t*         Please choose your operation.           *\n");
	printf("\t---------------------------------------------------\n");
	printf("		1.Student Data.\n");
	printf("		2.Teacher Data.\n");
	printf("		3.Problem Data.\n");
	printf("\t*         Press other key to HomePage.           *\n");
	printf("\t===================================================\n");
	printf("\t* Enter:");
	choice = _getch();
	_putch( choice );
	printf("\t===================================================\n\n");
	
	switch ( choice )
	{//看记事本的文件
		case '1':
			StudentData();
			break;
		case '2':
			TeacherData();
			break;
		case '3':
			ProblemData();
			break;
		default:
			Homepage();
			break;
	}
}

/* 修改学生信息 */
void StModifyInfo(char STid[])
{ 
	int i=0;
	int line=FindLineNum("stdata.txt");
	char newcode[MAXCODE],newname[MAXNAME],newgender[MAXSEX],newphone[TELSIZE];
	int newclass;

	system("cls");
	if((stdata=fopen("stdata.txt","r"))==NULL)
	{
		printf("\nFailed to open the file\n");
		exit(1);
	}
	
    LoadStudentFile( stdata );
	fclose(stdata);
	
	for(i=0;i<FindLineNum("stdata.txt");i++)
		if(strcmp(stlist[i].STid,STid)==0)
			break;
			
	printf("\tThis is your information.\n");
	printf("\t| ID:%s | code:%s | name:%s | gender:%s | telephone:%s | class:%d |\n\n",stlist[i].STid,stlist[i].STcode,stlist[i].STname,stlist[i].STsex,stlist[i].STphone,stlist[i].TEclass);
	printf("\tPlease input your new information:\n");
	
	getPwd(newcode);
	getName(newname);	
	getSex(newgender);
	
	printf("\t===================================================\n");
	printf("\t* Enter telephone: ");
	scanf("%s",newphone);
	printf("\t===================================================\n\n");
	
	printf("\t===================================================\n");
	printf("\t* Enter classnumber: ");
	scanf("%d",&newclass);
	printf("\t===================================================\n\n");
	
	strcpy(stlist[i].STcode,newcode);
	strcpy(stlist[i].STname,newname);
	strcpy(stlist[i].STphone,newphone);
	strcpy(stlist[i].STsex,newgender);
	stlist[i].TEclass=newclass; 


	if((stdata2=fopen("stdata.txt","w"))==NULL)
	{
		printf("\n fail open\n");
		exit(1);
	}
	fprintf_st(stdata2,line);
	fclose(stdata2);		
	
	printf("\t---------------------------------------------------\n");				
	printf("\t* Change successfully, press any key to back. *\n");
	printf("\t---------------------------------------------------\n\n");
	_getch();
	STchoose(STid);	
}

/* 单词测验 */
void StGoStudy(char ID[])
{
	char a[MAXANSWER];
	int i,n,k;
	int score=0;
	float finish_time;
	//int choose;
	int line=0;
	int num=0;
	float best_finish_time=10000;
	char nowtime[20];
	char degree[PROBLEM_AMOUNT];
	int tmp;
	struct tm * lt;
	clock_t start,finish; 
	time_t t;

	start=clock();	
	
	
	system("cls");
	if((prdata=fopen("prdata.txt","r"))==NULL)
	{
		printf("\nFailed to open the file\n");
		exit(1);
	}
	LoadProblemFile(prdata);
	fclose(prdata);
	
	if((stdata=fopen("stdata.txt","r"))==NULL)
	{
		printf("\nFailed to open the file\n");
		exit(1);
	}
    LoadStudentFile( stdata );
	fclose(stdata);
	
	
	/*找到学生k*/
	for(k=0;k<FindLineNum("stdata.txt");k++)
		if(!strcmp(stlist[k].STid,ID))
			break;
	printf("\t---------------------------------------------------\n");
	printf("If you need tips, please press 'tips' to replace the answer\n");
	printf("\t---------------------------------------------------\n");
	/*做题和判断*/ 
	
	/* 由学生输入希望做的单词测验题数目 */
	printf("\t===================================================\n");
	while(1)
	{
		printf("\t* Enter number of question(less than 200): ");
		scanf("%d",&num);
		if (0<num && num <=PROBLEM_AMOUNT ) break;
		else
		{
			printf( "\tWrong input!Please enter again!\n\n" );
		}
	}

	printf("\t===================================================\n\n");
    /* 根据先前输入的测验题数目，逐题随机选择题号，显示题目内容，读取学生答题选择，根据结果更新单词熟悉度 */
	strcpy( degree, stlist[k].degree );
	for ( n = 0; n <= num; n++ )
	{
		i = rand() % 100;
		if ( degree[i] != '3' )
		{
			printf("\t---------------------------------------------------\n");
			printf("\n\t%d: %s\n",i,prlist[i].Problem);

			printf("\n\tPlease choose your answer:");
			scanf("%s",a);
            if(strcmp(a,"tips")==0) 
            {
                char wrongAnswer[1];
			do
			{
				tmp = rand() % 4;
				if(tmp==0) strcpy(wrongAnswer,"a");
				else if(tmp==1) strcpy(wrongAnswer,"b");
				else if(tmp==2) strcpy(wrongAnswer,"c");
				else if(tmp==3) strcpy(wrongAnswer,"d");
			}while(strcmp(wrongAnswer,prlist[i].Answer)==0);	
			printf("\tThe statement %s is wrong.\n",wrongAnswer);	
			printf("\t===================================================\n");
			printf("\tNow please choose your answer:");
			scanf("%s",a);
			printf("\t===================================================\n\n");			
			}
			if (strcmp(prlist[i].Answer,a)==0)//answer is correct
			{
				printf("\n\t\tCorrect!\n\n");
				switch ( degree[i] )
				{
					case '0': degree[i] = '1'; break;
					case '1': degree[i] = '2'; break;
					case '2': degree[i] = '3'; break;
				}
				score += 10;
				stlist[k].STmoney += 2;
        	}
			else//answer is wrong
			{
				printf("\tWrong!\n");
				printf("\tThe right answer is %s.\n",prlist[i].Answer);
			}
		}
	}	
	strcpy(stlist[k].degree,degree);
	/* 给钱 */
	stlist[k].STmoney+=10;
	/*获取现在时间*/	
	time( &t );		 
	lt = localtime( (time_t *)&t );

	sprintf(nowtime,"%d:%.2d:%.2d:%.2d:%.2d",
	lt->tm_year+1900,lt->tm_mon+1,lt->tm_mday,lt->tm_hour,lt->tm_min);
	
	/*获取做题时间*/
	finish=clock();			 
	finish_time=(float)(finish-start)/CLOCKS_PER_SEC;	

	/*输出信息基本结束*/   
	printf("\t\tEnd.\n\n");
	printf("\t---------------------------------------------------\n");
	printf("\t*Your final score is %d.\n\n",score);
	printf("\t*Now time: %s.\n\n",nowtime);
	printf("\t*Time: %.2f seconds\n",finish_time);
	printf("\t*Your score: %d in %f seconds\n\n",score,finish_time);
	printf("\t---------------------------------------------------\n");

	/*写回，基本结束*/ 
	line=FindLineNum("stdata.txt");
	stdata2=fopen("stdata.txt","w");
	fprintf_st(stdata2,line);
	fclose(stdata2);
	
	printf("\t---------------------------------------------------\n");
	printf("\tPress any key to continue...\n");
	printf("\t---------------------------------------------------\n");
	_getch();
	STchoose(ID);
}

/* 显示排名数据 */
void StGoRank(char ID[])
{
	char ra;
	int start=0;
	int i=0, j, studentNum;
	system("cls");
	
	if((radata=fopen("radata.txt","r"))==NULL)
	{
		printf("\nFailed to open the file\n");
		exit(1);
	}
	ra=fgetc(radata);
	
	
	printf("\t---------------------------------------------------\n");
	printf("\t--------------------  -Rank-   --------------------\n");
	printf("\t---------------------------------------------------\n");
	printf("\n");

	while(ra!=EOF)
	{
		if(start==0)
			printf("		");
		start=1;
		putchar(ra);
		if(ra=='\n')
			printf("		");
		ra=fgetc(radata);//把radata里的所有字符逐个输出 
	}
	fclose(radata);
	
	if((radata2=fopen("radata.txt","r"))==NULL)
	{
		printf("\nFailed to open the file.\n");
		exit(1);
	}
	LoadRankFile(radata2);
	
    LoadStudentFile( stdata );
	fclose(stdata);
	
	printf("\n");
	studentNum = FindLineNum( "radata.txt" );//排行榜中学生的个数 
	for( i = 0; i < studentNum; i++ )
	{
		if(!strcmp(ID,ralist[i].rank_id))//在排行榜中找到该学生 
		{
			printf("\t---------------------------------------------------\n");
			printf("		Hello %s\n",ralist[i].rank_name);
			printf("\t---------------------------------------------------\n");
			printf("		Your best score is %d\n",ralist[i].rank_score);
			printf("		Your rank is %d\n",ralist[i].rank);	
			for ( j = 0; j < FindLineNum( "stdata.txt" ); j++ )
			{
				if ( !strcmp( ID, stlist[j].STid ) )
			        printf("        Here's the comment from your teacher: %s\n",stlist[j].TEcomment); 
			}
			printf("\t---------------------------------------------------\n");
		}
	}
	fclose(radata2);
	printf("		Press any key to continue...\n");
	_getch();
	STchoose(ID);
}

/* 更新排名数据 */
void UpdateRank(int k)
{
	int i;
	int line=FindLineNum("radata.txt");
	if((radata=fopen("radata.txt","r"))==NULL)
	{
		printf("\nFailed to open the file\n");
		exit(1);
	}
	LoadRankFile(radata);
	fclose(radata);
	
	for(i=k;i>0;i--)
	{
		if(ralist[i].rank_score>ralist[i-1].rank_score)
		{
			swapstr(ralist[i].rank_id,ralist[i-1].rank_id,MAXID);
			swapstr(ralist[i].rank_name,ralist[i-1].rank_name,MAXNAME);
			swap(&ralist[i].Class,&ralist[i-1].Class);
			swap(&ralist[i].rank_score,&ralist[i-1].rank_score);
			swapfloat(&ralist[i].finish_time,&ralist[i-1].finish_time);
		}
		else if(ralist[i].rank_score == ralist[i-1].rank_score)
		{
			if(ralist[i].finish_time < ralist[i-1].finish_time)
			{
			swapfloat(&ralist[i].finish_time,&ralist[i-1].finish_time);
			swapstr(ralist[i].rank_id,ralist[i-1].rank_id,MAXID);
			swapstr(ralist[i].rank_name,ralist[i-1].rank_name,MAXNAME);
			swap(&ralist[i].Class,&ralist[i-1].Class);
			swap(&ralist[i].rank_score,&ralist[i-1].rank_score);
		    }
		}
	}

	radata2=fopen("radata.txt","w");
	fprintf_ra(radata2,line);
	fclose(radata2);
}

void StGoTest(char ID[])
{
	char a[MAXANSWER];
	int i,n,k;
	int score=0;
	float finish_time;
	//int choose;
	char choice;
	int line=0;
	int lastScore=0;
	float best_finish_time=10000;
	int num=0;
	struct tm * lt;
	char nowtime[20];
	clock_t start,finish; 
	time_t t;
	char wrongAnswer[1];

	start=clock();		
	system("cls");
	if((prdata=fopen("prdata.txt","r"))==NULL)
	{
		printf("\nFailed to open the file\n");
		exit(1);
	}
	LoadProblemFile(prdata);
	fclose(prdata);
	
	if((stdata=fopen("stdata.txt","r"))==NULL)
	{
		printf("\nFailed to open the file\n");
		exit(1);
	}
    LoadStudentFile( stdata );
	fclose(stdata);
	
	if((radata=fopen("radata.txt","r"))==NULL)
	{
		printf("\nFailed to open the file\n");
		exit(1);
	}
	LoadRankFile(radata);
	fclose(radata);
	
	/*找到学生k*/
	for(k=0;k<FindLineNum("stdata.txt");k++)
		if(!strcmp(stlist[k].STid,ID))
			break;
	printf("\t---------------------------------------------------\n");
	printf("\tIf you need tips, please press 'tips' to replace the answer\n");
	printf("\t---------------------------------------------------\n");
	/*做题和判断*/ 
	for(n=0;n<HOW_MANY_QUESTION_IN_TEST;n++)
	{
		i = rand() % 100;
		printf("\t---------------------------------------------------\n");
		printf("\n\t%d: %s\n",i,prlist[i].Problem);

		printf("\n\tPlease choose your answer:");
		scanf("%s",a);
		if(strcmp(a,"tips")==0)
		{
			if(stlist[k].tips==0)
				printf("\n\tSorry you don't have tips anymore.\n");
			else
			{
				
				stlist[k].tips--;
				do
				{
					int tmp=rand()%4;
					if(tmp==0) strcpy(wrongAnswer,"a");
					else if(tmp==1) strcpy(wrongAnswer,"b");
					else if(tmp==2) strcpy(wrongAnswer,"c");
					else if(tmp==3) strcpy(wrongAnswer,"d");
				}while(strcmp(wrongAnswer,prlist[i].Answer)==0);
				
				
				printf("\tThe statement %s is wrong.\n",wrongAnswer);
				printf("\tNow you have %d tips available.\n",stlist[k].tips);	
			}			
			printf("\n\tNow please choose your answer:");
			scanf("%s",a);
		}
		if(strcmp(prlist[i].Answer,a)==0)
		{	
			printf("\n\t\tCorrect!\n\n");
			score+=10;
			stlist[k].STmoney+=10;
		}
		else
		{
			printf("\tWrong!\n");
			printf("\tThe right answer is %s.\n",prlist[i].Answer);
		}
		
	}	
	/* 给钱 */
	stlist[k].STmoney+=100;
	/*获取现在时间*/	
	time( (time_t*)&t );		
	lt = localtime( (time_t*)&t );

	sprintf(nowtime,"%d:%.2d:%.2d:%.2d:%.2d",lt->tm_year+1900,lt->tm_mon+1,lt->tm_mday,lt->tm_hour,lt->tm_min);
	
	/*获取做题时间*/
	finish=clock();			 
	finish_time=(float)(finish-start)/CLOCKS_PER_SEC;
	
	/*时间分数权重算综合分*/ 
	lastScore = (int)(score*1.0+finish_time*0.0);
	
	/*检验排行榜*/ 
	if(stlist[k].STbestscore==0)
	{
		printf("welcome first test!");
		stlist[k].STbestscore=lastScore;
		stlist[k].STbestfinishtime=best_finish_time;
		
		line=FindLineNum("radata.txt");
		radata2=fopen("radata.txt","a");		
		fprintf(radata2,"%d %s %s %d %d %f\n",line+1,ID,stlist[k].STname,stlist[k].TEclass,lastScore,finish_time);
		fclose(radata2);
		
		UpdateRank(FindLineNum("radata.txt"));
	}
	else if(stlist[k].STbestscore<lastScore)
	{
		printf("\tExcellent!\n");
		stlist[k].STbestscore=lastScore;
		stlist[k].STbestfinishtime=finish_time;

		for(num=0;num<FindLineNum("radata.txt");num++)
			if(!strcmp(ralist[num].rank_id,stlist[k].STid))
				break;
		strcpy(ralist[num].rank_name,stlist[k].STname);//name 
		ralist[num].rank_score=lastScore;
		ralist[num].finish_time=finish_time;
		
		line=FindLineNum("radata.txt");
		radata2=fopen("radata.txt","w");
		fprintf_ra(radata2,line);
		fclose(radata2);
		
		UpdateRank(num);
	}
	else if(stlist[k].STbestscore == lastScore)
	{
		printf("\tGOOD!\n");
		if(stlist[k].STbestfinishtime > finish_time)
		{
			stlist[k].STbestfinishtime=finish_time;
			for(num=0;num<FindLineNum("radata.txt");num++)
			if(!strcmp(ralist[num].rank_id,stlist[k].STid))
				break;
			strcpy(ralist[num].rank_name,stlist[k].STname);
		    ralist[num].finish_time=finish_time;
		}
		
		line=FindLineNum("radata.txt");
		radata2=fopen("radata.txt","w");
		fprintf_ra(radata2,line);
		fclose(radata2);
		
		UpdateRank(num);
	}
	else
	{
		printf("\tKeep trying!\n");
		for(num=0;num<FindLineNum("radata.txt");num++)
			if(!strcmp(ralist[num].rank_id,stlist[k].STid))
				break;
		strcpy(ralist[num].rank_name,stlist[k].STname);
	}
	
	/*输出信息基本结束*/  
	printf("\n\t---------------------------------------------------\n");
	printf("\t\tEnd.\n\n");
	printf("\t---------------------------------------------------\n");
	printf("\tYour final score is %d.\n\n",score);
	printf("\tNow time: %s.\n\n",nowtime);
	printf("\tTime: %.2f seconds\n",finish_time);
	printf("\tYour score: %d in %f seconds\n\n",score,finish_time);
	printf("\t*Your best score: %d in %f seconds\n\n",stlist[k].STbestscore,stlist[k].STbestfinishtime);
	printf("\t$Your money:%d\n",stlist[k].STmoney);
	printf("\t---------------------------------------------------\n");

	/*写回，基本结束*/ 
	line=FindLineNum("stdata.txt");
	stdata2=fopen("stdata.txt","w");
	fprintf_st(stdata2,line);
	fclose(stdata2);
	
	/*去不去排行榜*/ 
	printf("\t---------------------------------------------------\n");
	printf("	See Rank? Press 1. Default to Back\n");
	printf("	So your choice:");
	choice = _getch();
	_putch( choice );
	if ( choice == '1' )
		StGoRank( ID );
	else
		STchoose( ID );
}

void STGOpikaqiu(char ID[])
{
	
	int i=0;
	system("cls");
	stdata = fopen("stdata.txt","r");	
    LoadStudentFile( stdata );
	fclose(stdata);

	for(i=0;i<FindLineNum("stdata.txt");i++)
		if(!strcmp(stlist[i].STid,ID))
			break;
	printf("Hello %s, Now you have %d pikaqiu!\n",stlist[i].STname,stlist[i].pikaqiu);
	printf("Keep moving!\n");
	
	if(stlist[i].pikaqiu==1)
		DisplayTextPicture("pikaqiu1.txt");
	else if(stlist[i].pikaqiu==2)
		DisplayTextPicture("pikaqiu2.txt");
	else if(stlist[i].pikaqiu==3)
		DisplayTextPicture("pikaqiu3.txt");
	else
		printf("You don't have any qikaqiu now!\n");
		
	printf("Press any key to continue...");
	_getch();
	STchoose(ID);
}

/* 学生登录 */
void StLogin()
{
	char student_id[MAXID];
	char student_code[MAXCODE];
//	int g;
	char id[MAXID];
	int find=0,i;
	char choice='0';
	
	system("cls");
	stdata = fopen("stdata.txt","r");	
    LoadStudentFile( stdata );
	fclose(stdata);			

	while(1)
	{
		printf("\t-------------------------------------------------------------------------\n");
		getID( student_id );
		strcpy( id, student_id );
		for( i = 0; i < FindLineNum("stdata.txt"); i++ )
		{
			if ( !strcmp(student_id,stlist[i].STid) )//存在该账号 
			{
				find=1;
				break;
			}
		}
		if ( find==1 ) break;
		else
		{
			printf("\t-------------------------------------------------------------------------\n");
			printf("		ID no found, press 1 to homepage,2 to print again\n");
			printf("\t-------------------------------------------------------------------------\n");
			printf("		So your choice:");
			choice = _getch();
			_putch( choice );
			switch ( choice )
			{
				case '1':
					Homepage();
					break;
			}
		}
	}	
	printf("\t-------------------------------------------------------------------------\n");
	getPwd(student_code);

	if(!strcmp(student_code,stlist[i].STcode))
	{
		printf("\t-------------------------------------------------------------------------\n");
		printf("\t		Success!\n");
		printf("\t-------------------------------------------------------------------------\n");
	 
	}	
	else
	{
		while(1)
		{
			printf("\t-------------------------------------------------------------------------\n");
			printf("\t*Wrong input, again!\n");
			getPwd(student_code);
			if(!strcmp(student_code,stlist[i].STcode))
			{
				printf("\t-------------------------------------------------------------------------\n");
				printf("		Success!\n");
				printf("\t-------------------------------------------------------------------------\n");
				break;
			}
		}
	}
	STchoose(id);
}

/* 学生模块功能菜单选择 */
void STchoose(char student_id[])
{
	char choice;

	system("cls");
	printf("\t---------------------------------------------------\n");
	printf("\t*         Please choose your operation.           *\n");
	printf("\t---------------------------------------------------\n");
	printf("		1.Modify your information.\n");
	printf("		2.Go to study.\n");
	printf("		3.Go to test.\n");
	printf("		4.Check the rank.\n");
	printf("		5.Go to SHOP.\n");
	printf("		6.Check my pikaqius.\n");
	printf("		So your choice:");
	choice = _getch();
	_putch( choice );
	switch ( choice )
	{
		case '1':
			StModifyInfo(student_id);
			break;
		case '2':
			StGoStudy(student_id);
			break;
		case '3':
			StGoTest(student_id);
			break;
		case '4':
			StGoRank(student_id);
			break;
		case '5':
			StGoShop(student_id);
			break;
		case '6':
			STGOpikaqiu(student_id);
			break;
		default:
			Homepage();
			break;
	}
	fclose(stdata);

}

void StGoShop(char ID[])
{
	int i=0;
	char choice;
    int line=FindLineNum("stdata.txt");
    int money;

	system("cls");

	if((stdata=fopen("stdata.txt","r"))==NULL)
	{
		printf("\nFailed to open the file\n");
		exit(1);
	}
    LoadStudentFile( stdata );
	fclose(stdata);
	
	for(i=0;i<FindLineNum("stdata.txt");i++)
	{
		if(strcmp(stlist[i].STid,ID)==0)
		{
			printf("\tThis is your information.\n");
			printf("\tID:%s code:%s name:%s moneys:%d\n",stlist[i].STid,stlist[i].STcode,stlist[i].STname,stlist[i].STmoney);
		    money = stlist[i].STmoney;
			printf("\tPlease choose what you want to buy:\n");
			printf("\t------------------------------------------------------------------------------------------------------\n");
			printf("\t1. A correct answer.(cost 100 points)\n");
			printf("\t2. A pikaqiu.(cost 1000 points)\n");
			printf("\t3. A chance for a game.(cost 200 points)\n");
			printf("\t4. I don't want to buy anything.\n'"); 
			printf("\t------------------------------------------------------------------------------------------------------\n");
			printf("\t------------------------------------------------------------------------------------------------------\n");
			printf("\tI would like to buy a:");
			choice = _getch();
			_putch( choice );
			printf("\n\t------------------------------------------------------------------------------------------------------\n");
			printf("\t------------------------------------------------------------------------------------------------------\n");
			
			switch ( choice )
			{
				case '1':
					if (money > 100)
					{
					printf("\n\t---------------------------------------------Done-----------------------------------------------------");
					stlist[i].tips++;
					stlist[i].STmoney-=100;
				    }
				    else
				    printf("\n\tSorry, you don't have enough money.\n");
					break;
				case '2':
				    if (money > 1000)
					{
					printf("\n\t---------------------------------------------Done-----------------------------------------------------");
					stlist[i].pikaqiu++;
					stlist[i].STmoney-=1000;
				    }
				    else
				    printf("\n\tSorry, you don't have enough money.\n");
					break;
				case '3':
                    if (money > 200)
					{
					printf("\n\t---------------------------------------------Done-----------------------------------------------------");
					stlist[i].STmoney-=200;
					getchar();	
	                ShellExecute(NULL,"open","12.html",NULL,NULL,SW_SHOWNORMAL);
				    }
				    else
				    printf("\n\tSorry, you don't have enough money.\n");
					break;
				case '4':
					STchoose(ID);
					break;
				default:
					break;
			} 
     	}
    }

	stdata2=fopen("stdata.txt","w");
	fprintf_st(stdata2,line);
	fclose(stdata2);
	
	printf("\n\t------------------------------------------------------------------------------------------------------\n");
	printf("\t------------------------------------------------------------------------------------------------------\n");
	printf("\tPlease press any key to go back to choose your operation.\n");
	_getch();
	STchoose(ID);
}

/* 新学生用户注册 */
void StRegister()
{
	char student_id[MAXID];
	char id[MAXID];
	char student_code[MAXCODE];
	char student_name[MAXNAME];
	char student_sex[MAXSEX];
	char student_tel[TELSIZE];
	char student_degree[PROBLEM_AMOUNT]; 
	int line=FindLineNum("stdata.txt");
	int TEclass = 0;
	float time=100;
	char choice;
	int i;
	system("cls");
	stdata = fopen("stdata.txt","r");	
    LoadStudentFile( stdata );
	fclose(stdata);	
	memset(student_degree,0, sizeof(student_degree));
	stdata2 = fopen("stdata.txt","a+");
	
    printf("\t-------------------------------------------------------------------------\n");
	printf("\t*        			  Welcome!  		            *\n");
	printf("\t-------------------------------------------------------------------------\n");

	while(1)
	{
		int search=0;
		printf("\t-------------------------------------------------------------------------\n");
		getID(student_id);
		strcpy(id,student_id);
		for(i=0;i<line;i++)
		{
			if(!strcmp(student_id,stlist[i].STid))//相同 
			{
				search=1;
				printf("\t-------------------------------------------------------------------------\n");
				printf("		The account has been registered, pass 1 to homepage,2 to print again\n");
				printf("\t-------------------------------------------------------------------------\n");
				printf("		So your choice:");
				choice = _getch();
				_putch( choice );
				switch ( choice )
				{
					case '1':
						Homepage();
						break;
					case '2':
						StRegister();
						break;
				}
			}
		}
		if(search==0) break;	
	}

	getPwd(student_code);
	getName(student_name);	
	getSex(student_sex);
	
	printf("\t==========================================================================\n");
	printf("\t* Enter telephone: ");
	scanf("%s",student_tel);
	printf("\t==========================================================================\n\n");
	
	printf("\t==========================================================================\n");
	printf("\t* Enter classnumber: ");
	scanf("%d",&TEclass);
	printf("\t==========================================================================\n\n");
	
	fprintf(stdata2,"%s %s %s %s %s %d %d %d %d %d %d %s %s\n",student_id,student_code,
	student_name,student_tel,student_sex,TEclass,0,0,0,0,0,"0",student_degree);

	printf("\t-------------------------------------------------------------------------\n");
	printf("\t*        			  Success!  		            *\n");
	printf("\t-------------------------------------------------------------------------\n");

	fclose(stdata2);
	_getch(); 
	Student();
}

void Student()
{
	char choice;

	system("cls");
	DisplayTextPicture("STpage.txt"); 

	choice = _getch();
	_putch( choice );
	switch ( choice )
	{
		case '1':
			StLogin();
			break;
		case '2':
			StRegister();
			break;
		default:
			Homepage();
	}
}

void TeModify(char TEid[])
{
	int i=0;
	int line=FindLineNum("tedata.txt");
	char newcode[MAXCODE],newname[MAXNAME],newgender[MAXSEX],newphone[TELSIZE];
	int newclass;
	
	system("cls");
	if((tedata=fopen("tedata.txt","r"))==NULL)
	{
		printf("\nFailed to open the file\n");
		exit(1);
	}
	
	LoadTeacherFile(tedata);
	fclose(tedata);
	
	for(i=0;i<FindLineNum("tedata.txt");i++)
		if(strcmp(telist[i].TEid,TEid)==0)
			break;
			
	printf("\tThis is your information.\n");
	printf("\t| ID:%s | code:%s | name:%s | gender:%s | telephone:%s | class:%d |\n\n",telist[i].TEid,telist[i].TEcode,telist[i].TEname,telist[i].TEsex,telist[i].TEphone,telist[i].TEclass);
	printf("\tPlease input your new information:\n");
	
	getPwd(newcode);
	getName(newname);	
	getSex(newgender);
	
	printf("\t===================================================\n");
	printf("\t* Enter telephone: ");
	scanf("%s",newphone);
	printf("\t===================================================\n\n");
	
	printf("\t===================================================\n");
	printf("\t* Enter classnumber: ");
	scanf("%d",&newclass);
	printf("\t===================================================\n\n");
	
	strcpy(telist[i].TEcode,newcode);
	strcpy(telist[i].TEname,newname);
	strcpy(telist[i].TEphone,newphone);
	strcpy(telist[i].TEsex,newgender);
	telist[i].TEclass=newclass; 


	if((tedata2=fopen("tedata.txt","w"))==NULL)
	{
		printf("\n fail open\n");
		exit(1);
	}
	fprintf_te(tedata2,line);
	fclose(tedata2);		
	
	printf("\t---------------------------------------------------\n");				
	printf("\t* Change successfully, press any key to back. *\n");
	printf("\t---------------------------------------------------\n\n");
	_getch();
	TEchoose(TEid);
}

/* 教师对学生做出评价，并记录评价信息 */
void TeMakeComment(char TEid[])
{
	int find=0;
	char comment[20];
	char id[MAXID];
	int i;  //i,j;
	int line=FindLineNum("stdata.txt");
	char choice='0', choice_1='0', choice_2='0';
	int TEclass=0; //telist[j].TEclass;

	stdata = fopen("stdata.txt","r");	
    LoadStudentFile( stdata );
	fclose(stdata);
	
	tedata = fopen("tedata.txt","r");
	LoadTeacherFile(tedata);
	fclose(tedata);
	
	printf("\t---------------------------------------------------\n");	
	printf("\t---------------------------------------------------\n");	
	printf("\tPlease insert the student's id:");
	scanf("%s",&id);
	printf("\t---------------------------------------------------\n");	

	for(i=0;i<FindLineNum("stdata.txt");i++)
		{
			if(!strcmp(id,stlist[i].STid))						
			{
			    printf("\tID:%s name:%s gender:%s telephone:%s best score:%d finish time:%f\n",stlist[i].STid, 
				stlist[i].STname,stlist[i].STsex,stlist[i].STphone,stlist[i].STbestscore,stlist[i].STbestfinishtime);
		        find=1;			
				printf("\t---------------------------------------------------\n");	
				printf("\tPlease choose from these 4 choices:\n");
				printf("\t1.Brilliant!\n\t2.Good.\n\t3.Okay.\n\t4.Ewwwww.\n\n");
				printf("\t---------------------------------------------------\n\t");	
		        choice_1 = _getch();
				_putch( choice_1 );
		        switch ( choice_1 )
				{
					case '1':
						strcpy(comment,"Brilliant");
						break;
					case '2':
						strcpy(comment,"Good");
						break;
					case '3':
						strcpy(comment,"Okay");
						break;
					case '4':
						strcpy(comment,"Ewwwww.");
						break;
					default:
						printf("Wrong Input, again\n");
						find=0;
						TeMakeComment(TEid);
						break;
				}
				strcpy(stlist[i].TEcomment,comment);
				printf("\t\tOK!\n");
				printf("\tPress any key to return.\n");
				break;
			}
		}	
	if(find==0) 
	{
		printf("\tIt's an unregistered ID! Press again, please.\n");
		_getch();
		TeMakeComment(TEid);
	}

    line=FindLineNum("stdata.txt");
	stdata2=fopen("stdata.txt","w");
	fprintf_st(stdata2,line);
	fclose(stdata2);
	_getch();

    TEchoose(TEid);
}


void TeSearch(char TEid[]) 
{
	int find=0,i;
	int j=0;
	int TEclass=telist[j].TEclass;
//	char teacher_id;
	
	tedata = fopen("tedata.txt","r");	
	LoadTeacherFile(tedata);
    fclose(tedata);
	stdata = fopen("stdata.txt","r");	
    LoadStudentFile( stdata );
	fclose(stdata);
	for(j;j<FindLineNum("tedata.txt");j++)
	{	
	    if(!strcmp(telist[j].TEid,TEid))
			break;
	} 
	
	printf("\t---------------------------------------------------\n");
	printf("		Here's all your students: \n"); 
	printf("\t---------------------------------------------------\n");
	printf("\t---------------------------------------------------\n");
	for(i=0;i<FindLineNum("stdata.txt");i++)
	{
		if(TEclass==stlist[i].TEclass)
		{
			printf("\tID:%s name:%s gender:%s telephone:%s best score:%d finish time:%f\n",stlist[i].STid, 
				    stlist[i].STname,stlist[i].STsex,stlist[i].STphone,stlist[i].STbestscore,stlist[i].STbestfinishtime);
		//	find=1;
		}
		else
		{
			find = 1;
		}
	}
	printf("\t---------------------------------------------------\n");
	if(find==1)  
		{
			printf("\t---------------------------------------------------\n");
			printf("        No found, pass any key to continue...\n");
			printf("\t---------------------------------------------------\n");
			_getch();
			//TeLogin();
		}

    TEchoose(TEid);
}

void TeGoRank(char TEid[])
{
    char ra;
	int start=0;
	int i=0;
	char choice;
	
	system("cls");
	if((radata=fopen("radata.txt","r"))==NULL)
	{
		printf("\nFailed to open the file\n");
		exit(1);
	}
	ra=fgetc(radata);
	
	
	printf("\t---------------------------------------------------\n");
	printf("\t--------------------  -Rank-   --------------------\n");
	printf("\t---------------------------------------------------\n");
	printf("\n");

	while(ra!=EOF)
	{
		if(start==0)
			printf("		");
		start=1;
		putchar(ra);
		if(ra=='\n')
			printf("		");
		ra=fgetc(radata);
	}
	fclose(radata);
	
	if((radata2=fopen("radata.txt","r"))==NULL)
	{
		printf("\nFailed to open the file\n");
		exit(1);
	}
	LoadRankFile(radata2);
	
	printf("\tPress 1 to see your own students.\n\tPress 2 to quit.");
	choice = _getch();
	_putch( choice );
	switch ( choice )
	{
		case '1':
			TeSearch(TEid);
			break;
		case '2':
			TEchoose(TEid);
			break;
	}
	
	
	printf("\n");
	
	fclose(radata2);
	printf("		Press any key to continue...\n");
	_getch();
	TEchoose(TEid);
}

/* 教师登录 */
void TeLogin()
{
	char teacher_id[MAXID];
	char teacher_code[MAXCODE];
//	int g;
	char id[MAXID];
	int find=0,i;
	char choice = '0';
	
	system("cls");
	tedata = fopen("tedata.txt","r");	
	LoadTeacherFile(tedata);
	fclose(tedata);			

	while(1)
	{
		printf("\t-------------------------------------------------------------------------\n");
		getID(teacher_id);
		strcpy(id,teacher_id);
		for(i=0;i<FindLineNum("tedata.txt");i++)
		{
			if(!strcmp(teacher_id,telist[i].TEid))
			{
				find=1;
				break;
			}
		}
		if(find==1) break;
		else
		{
			printf("\t-------------------------------------------------------------------------\n");
			printf("		ID no found, pass 1 to homepage,2 to print again\n");
			printf("\t-------------------------------------------------------------------------\n");
			printf("		So your choice:");
			choice = _getch();
			_putch( choice );
			switch ( choice )
			{
				case '1': Homepage(); break;
			}
		}
	}	
	printf("\t-------------------------------------------------------------------------\n");
	getPwd(teacher_code);

	if(!strcmp(teacher_code,telist[i].TEcode))
	{
		printf("\t-------------------------------------------------------------------------\n");
		printf("\t		Success!\n");
		printf("\t-------------------------------------------------------------------------\n");
	 
	}	
	else
	{
		while(1)
		{
			printf("\t-------------------------------------------------------------------------\n");
			printf("\t*Wrong input, again!\n");
			getPwd(teacher_code);
			if(!strcmp(teacher_code,telist[i].TEcode))
			{
				printf("\t-------------------------------------------------------------------------\n");
				printf("		Success!\n");
				printf("\t-------------------------------------------------------------------------\n");
				break;
			}
		}
	}
	TEchoose(id);
}

/* 教师功能菜单选择 */
void TEchoose(char teacher_id[])
{
	char choice;

	system("cls");
	printf("\t---------------------------------------------------\n");
	printf("\t*         Please choose your operation.           *\n");
	printf("\t---------------------------------------------------\n");
	
	printf("\t---------------------------------------------------\n");
	printf("\t*         1.Modify your information.              *\n");
	printf("\t*         2.Search your students.                 *\n");
	printf("\t*         3.Make commment.                        *\n");
	printf("\t*         4.Check the rank.           .           *\n");
	printf("\t*         Other key to Homepage.                  *\n");	
	printf("\t---------------------------------------------------\n");
	
	printf("\t===================================================\n");
	printf("\t* So your choice: ");
	choice = _getch();
	_putch( choice );
	printf("\n\t===================================================\n\n");
	
	switch ( choice )
	{
		case '1':
			TeModify(teacher_id);
			break;
		case '2':
			TeSearch(teacher_id);
			break;
		case '3':
			TeMakeComment(teacher_id);
			break;
		case '4':
		    TeGoRank(teacher_id);
			break;
		default:
			Homepage();
	}
	fclose(tedata);
}

/* 教师用户注册 */
void TeRegister()
{
	char teacher_id[MAXID];
	char id[MAXID];
	char teacher_code[MAXCODE];
	char teacher_name[MAXNAME];
	char teacher_sex[MAXSEX];
	char teacher_tel[TELSIZE];
	
	int line=FindLineNum("tedata.txt");
	int TEclass = 0;

	char choice;
	int i;

	system("cls");
	tedata = fopen("tedata.txt","r");	
	LoadTeacherFile(tedata);
	fclose(tedata);	
	tedata2 = fopen("tedata.txt","a+");
	
	printf("\t-------------------------------------------------------------------------\n");
	printf("\t*			         Welcome!  		                *\n");
	printf("\t-------------------------------------------------------------------------\n");

	while(1)
	{
		int search=0;
	    printf("\t-------------------------------------------------------------------------\n");
		getID(teacher_id);
		strcpy(id,teacher_id);
		for(i=0;i<line;i++)
		{
			if(!strcmp(teacher_id,telist[i].TEid))//相同 
			{
				search=1;
				printf("\t-------------------------------------------------------------------------\n");
				printf("		The account has been registered, pass 1 to homepage,2 to print again\n");
				printf("\t-------------------------------------------------------------------------\n");
				printf("		So your choice:");
				choice = _getch();
				_putch( choice );
				switch ( choice )
				{
					case '1': Homepage(); break;
					case '2': TeRegister(); break;
				}
			}
		}
		if(search==0) break;	
	}

	getPwd(teacher_code);
	getName(teacher_name);	
	getSex(teacher_sex);
	
	printf("\t==========================================================================\n");
	printf("\t* Enter telephone: ");
	scanf("%s",teacher_tel);
	printf("\t==========================================================================\n\n");
	
	printf("\t==========================================================================\n");
	printf("\t* Enter classnumber: ");
	scanf("%d",&TEclass);
	printf("\t==========================================================================\n\n");
	
	fprintf(tedata2,"%s %s %d %s %s %s\n",teacher_id,teacher_code,TEclass,teacher_sex,
	teacher_name,teacher_tel);

	printf("\t-------------------------------------------------------------------------\n");
	printf("\t*        			  Success!  		            *\n");
	printf("\t-------------------------------------------------------------------------\n");

	fclose(tedata2);
	_getch(); 
	Teacher();
}

void Teacher()
{
	char choice;
	
	system("cls");
	DisplayTextPicture("TEpage.txt"); 
	choice = _getch();
	_putch( choice );

	switch ( choice )
	{
		case '1':
			TeLogin();
			break;
		case '2':
			TeRegister();
			break;
		case '3':
			Homepage();	
			break;
		default:
			Homepage();
	}
}

/* 读取用户输入的ID */
int getID(char ID[])
{
//	char temp;
	int ret=0;
	int inputright=2;
	int count=0;
	
//	在没按ESC且输入错误的情况下进行循环输入直到输入正确或者手动结束 
	while(!(ret==1&&inputright==1)) 
	{
		if(inputright==0) //输入错误，重新输入 
		{
			system("cls");
			printf("\t-------------------------------------------------------------------------\n");
			printf("\t* Warning! Not a valid ID, enter new one         *\n");
			printf("\t-------------------------------------------------------------------------\n\n");
		}
		
		printf("\t==========================================================================\n");
		printf("\t* Enter an ID: ");		
		ret=myGets(ID,MAXID-1,0);
		printf("\t==========================================================================\n\n");
		
		if(ret==0) //按了ESC之后结束跳出循环并结束该函数 
		{
			printf("\t-------------------------------------------------------------------------\n");
			printf("\t* Exit! Information not recorded!                 *\n");
			printf("\t------------------------------------------------------------------------\n\n");
			return 0;
		} 
		
		else if(ret==1)//没按ESC正常执行 
		{
			inputright=1;// 输入正确 
		}
	}
	return 1;
}

/* 读取用户输入的名称 */
int getName(char name[])
{
	int ret=0;
	
	while(ret==0)//除非按了ESC或者输入正确否则循环 
	{
		printf("\t==========================================================================\n");
		printf("\t* Enter name: ");
		ret=myGets(name,MAXNAME-1,0);
		printf("\t==========================================================================\n\n");
		if(ret==0)
		{
			printf("\t-------------------------------------------------------------------------\n");
			printf("\t* Exit! Information not recorded!                 *\n");
			printf("\t-------------------------------------------------------------------------\n\n");
			return 0;
		} 
	}
	return 1;
}

/* 读取用户输入的密码 */
int getPwd(char pwd[])
{
	int ret=0;
	
	while(ret==0) //输入密码，按了ESC退出 
	{
		printf("\t==========================================================================\n");
		printf("\t* Enter password: ");		
		ret=myGets(pwd,MAXPWD-1,1);
		printf("\t==========================================================================\n\n");
		if(ret==0)
		{
			printf("\t-------------------------------------------------------------------------\n");
			printf("\t* Exit! Information not recorded!                 *\n");
			printf("\t-------------------------------------------------------------------------\n\n");
			return 0;
		} 
	}
	return 1;
}

/* 读取用户选择的性别信息 */
int getSex(char sex[])
{
	int getsex=0;
	int inputright=2;
	
	while(inputright!=1)//如果输入错误允许重新输入 
	{
		if(inputright==0)
		{
			system("cls");
			printf("\t-------------------------------------------------------------------------\n");
			printf("\t* Warning! Not a valid enter, enter new one       *\n");
			printf("\t-------------------------------------------------------------------------\n\n");
		}
		
		printf("\t-------------------------------------------------------------------------\n");
		printf("\t* Select your gender:                             *\n");
		printf("\t* 1.Male                                          *\n");
		printf("\t* 2.Female                                        *\n");
		printf("\t* 3.Other                                         *\n");
		printf("\t* 4.Secret                                        *\n");
		printf("\t-------------------------------------------------------------------------\n\n");
		
		printf("\t==========================================================================\n");
		printf("\t* Choose one from the table: ");
		getsex = _getch();
		
		if(getsex==0x1B)//输入ESC退出 
			return 0;
		else
			printf("%c\n",getsex);
			
		printf("\t==========================================================================\n\n");
		
		switch (getsex)//以选择形式输入性别，保证性别内容一样 
		{
			case '1':
				strcpy(sex,"Male");
				inputright=1;
				break;
			case '2':
				strcpy(sex,"Female");
				inputright=1;
				break;
			case '3':
				strcpy(sex,"Other");
				inputright=1;
				break;
			case '4':
				strcpy(sex,"Secret");
				inputright=1;
				break;
			default:
				printf("\t* Not a valid enter, enter again: ");
				inputright=0;
				break;	
		}			
	}
	return 1;
}

/* 管理员登录 */
void ADlogin()
{
	char administrator_id[MAXID];
	char administrator_code[MAXCODE];
	//int choose;
	char choice;
	int find=0,i;
	system("cls");
	addata = fopen("addata.txt","r");	
	LoadAdminFile(addata);
	fclose(addata);	
		
	while(1)
	{
		printf("\t---------------------------------------------------\n");
		getID(administrator_id);
		for(i=0;i<FindLineNum("addata.txt");i++)
		{
			if(!strcmp(administrator_id,adlist[i].ADid))
			{
				find=1;
				break;
			}
		}
		if(find==1) break;
		else{
			printf("\t---------------------------------------------------\n");
			printf("		ID no found, pass 1 to homepage,2 to print again\n");
			printf("\t---------------------------------------------------\n");
			printf("		So your choice:");

			choice = _getch();
			_putch( choice );
			switch ( choice )
			{
				case '1': 
					Homepage();
					break;
			}
		}
	}	
	printf("\t---------------------------------------------------\n");	
	getPwd(administrator_code);
	
	if(!strcmp(administrator_code,adlist[i].ADcode))
	{
		printf("\t---------------------------------------------------\n");
		printf("		Success!\n");
		printf("\t---------------------------------------------------\n");
	}	
	else
	{
		while(1)
		{
			printf("\t---------------------------------------------------\n");
			printf("\t*Wrong input, again!\n");
			getPwd(administrator_code);
			if(!strcmp(administrator_code,adlist[i].ADcode)){
				printf("\t---------------------------------------------------\n");
				printf("\t		Success!\n");
				printf("\t---------------------------------------------------\n");
				break;
			}
		}
	}
	system("cls");
	printf("\t---------------------------------------------------\n");
	printf("\t*         Please choose your operation.           *\n");
	printf("\t---------------------------------------------------\n");
	
	printf("\t---------------------------------------------------\n");
	printf("\t*         1.Check the data            *\n");
	printf("\t*         Press other key to Homepage.                  *\n");	
	printf("\t---------------------------------------------------\n");
	
	printf("\t===================================================\n");
	printf("\t* So your choice: ");
	choice = _getch();
	_putch( choice );
	printf("\t===================================================\n\n");
	
	switch ( choice )
	{
		case '1':
			SystemData();
			break;
		default:
			Homepage();
			break;
	}
	fclose(addata);
}

/* 管理员功能选择 */
void Administrator()
{
	//int choose;
	char choice;

	system("cls");
	DisplayTextPicture("ADpage.txt"); 
	//scanf("%d",&choose);
	choice = _getch();
	_putch( choice );
	switch( choice )
	{
		case '1':
			ADlogin();
			break;
		case '2':
			Homepage();
			break;
		default:
			Homepage();
			break;
	}
}
