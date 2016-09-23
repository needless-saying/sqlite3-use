#include <stdio.h>
#include <stdlib.h>
#include "sqlite3.h"
#include <string.h>
#include <time.h>
#include <thread>
#include <math.h>
#include <windows.h>
/*


int main()
{
	const char *sqlr = "create table t(id int primary key,msg varchar(128),msg1 varchar(128),msg2 varchar(128),msg3 varchar(128),msg4 varchar(128),msg5 varchar(128), \
		msg6 varchar(128),msg7 varchar(128),msg8 varchar(128),msg9 varchar(128),msg10 varchar(128),msg11 varchar(128),msg12 varchar(128),msg13 varchar(128))"; 
	char *errmsg = 0; 
	int ret = 0; 
	time_t timen; 
	
	sqlite3 *db = 0; 
	ret = sqlite3_open("./sqlite3-demo.db",&db); 

	if(ret != SQLITE_OK){
        fprintf(stderr,"Cannot open db: %s\n",sqlite3_errmsg(db));
        return 1;
    }

	printf("open database success\n"); 

	ret = sqlite3_exec(db,sqlr,NULL,NULL,&errmsg); 
	if(ret != SQLITE_OK){
        fprintf(stderr,"create table fail: %s\n",errmsg);
    }

	
	char sqlr2[3500]; 
	char aaa[135]={0}; 
	aaa[0] = ','; 
	aaa[1]= '\"'; 
	memset(aaa+2,'q',128); 
	aaa[130] = '\"'; 
	//aaa[131] = ','; 

	memset(sqlr2,0,3500); 

	for(int i=0;i<14;i++)
		strcat(sqlr2,aaa); 

	//int len =strlen(sqlr2); 
	//sqlr2[len-1] = 0; 

	 time(&timen); 
	 char sqll[4000] = {0}; 
	 sqlite3_exec(db,"begin transaction",NULL,NULL,&errmsg);

	for(int i = 0; i < 2400; i++)
	{
	   
		sprintf(sqll, "insert into t values (%d%s)",i,sqlr2);
		ret = sqlite3_exec(db,sqll,NULL,NULL,&errmsg);

	if(ret != SQLITE_OK){
        fprintf(stderr,"insert table fail: %s\n",errmsg);
    }

	}

	sqlite3_exec(db,"commit transaction",NULL,NULL,&errmsg);
	printf("%d",time(NULL) - timen);	
    if(ret != SQLITE_OK){
        fprintf(stderr,"create table fail: %s\n",errmsg);
    }

	sqlite3_free(errmsg); 
	sqlite3_close(db); 

	printf("Close DateBase\n"); 
	getchar(); 


	return 0;
} */


char * err; 
std::thread *threadArray[16]; 
DWORD opTime[16][20]; 
char sqll[4000]; 
double max=0,min=0; 
int GetRandomIndex()
{
	return rand()*3; 
}

void OpenDataBase(sqlite3 **db)
{
	int ret = sqlite3_open("./sqlite3-demo.db",db); 

	if(ret != SQLITE_OK){
        fprintf(stderr,"Cannot open db: %s\n",sqlite3_errmsg(*db));
		getchar(); 
        exit(1);
    }
	
}


int GetRec(void *params,int n_column,char **column_value,char **column_name)
{
	char buf[130]; 
	for(int i = 1; i < n_column; i++)
	{
		memcpy(buf,column_value[i],strlen(column_value[i])); 
	}

	return 0; 
}

void WaitEnd()
{
	for(int i = 0; i < 16; i++)
	{
		threadArray[i]->join(); 
	}

	for(int i = 0; i < 16; i++)
	{
		free(threadArray[i]); 
	}
}

void ReadOperation(int threadIndex)
{
	sqlite3 *dbHandle; 

	char *sqlc = "select * from t where id = "; 

	

	char sqlExp[50] = {0}; 
	int ret; 
	for(int i = 0; i < 10; i++)
	{
		memset(sqlExp,0,sizeof(sqlExp));
		sprintf(sqlExp,"%s%d",sqlc,GetRandomIndex()); 
		OpenDataBase(&dbHandle); 
		ret = 0xf; 
		DWORD startTime = GetTickCount(); 
		while(ret != SQLITE_OK)
		{
			ret = sqlite3_exec(dbHandle,sqlExp,GetRec,NULL,&err); 
			if(ret != SQLITE_OK)
			{
				//printf("read error\n%s\n",err); 
			}
		}
		opTime[threadIndex][i] = GetTickCount() - startTime; 
		sqlite3_free(err); 
		sqlite3_close(dbHandle);
	}
	

}

void WriteOperation(int threadIndex)
{
	sqlite3 *dbHandle; 

	char *sqlc = "insert into t values() "; 


	char sqlExp[50] = {0}; 
	int ret = 0xf; 
	for(int i = 0; i < 10; i++)
	{

		OpenDataBase(&dbHandle); 
		
		ret = 0xf; 
		DWORD startTime = GetTickCount(); 
		while(ret != SQLITE_OK)
		{
			ret = sqlite3_exec(dbHandle,sqll,NULL,NULL,&err); 
			if(ret != SQLITE_OK)
			{
				//printf("insert error!!!press enter continue\n%s\n",err); 
				//getchar(); 
			}
		}
		opTime[threadIndex][i] = GetTickCount() - startTime; 
		sqlite3_free(err); 
		sqlite3_close(dbHandle); 
	}



}


void RWOperation(int threadIndex)
{
	sqlite3 *dbHandle; 

	char *sqlc = "select * from t where id = "; 


	char sqlExp[50] = {0}; 
	int ret = 0xf; 
	for(int i = 0; i < 10; i++)
	{

		if(threadIndex % 2 != 0)
		{
			memset(sqlExp,0,sizeof(sqlExp));
			sprintf(sqlExp,"%s%d",sqlc,GetRandomIndex()); 
			ret = 0xf;
			DWORD startTime = GetTickCount(); 
			OpenDataBase(&dbHandle);
			while(ret != SQLITE_OK)
			{
				ret = sqlite3_exec(dbHandle,sqlExp,GetRec,NULL,&err); 
				if(ret != SQLITE_OK)
				{
					//printf("RW:select error!!!press enter continue\n%s\n",err); 
					//getchar(); 
				}
			}
			opTime[threadIndex][i] = GetTickCount() - startTime; 
			sqlite3_free(err); 
			sqlite3_close(dbHandle);

			OpenDataBase(&dbHandle);
			ret = 0xf;
			startTime = GetTickCount(); 
			while(ret != SQLITE_OK)
			{
				ret = sqlite3_exec(dbHandle,sqll,NULL,NULL,&err); 
				if(ret != SQLITE_OK)
				{
					//printf("RW:insert error!!!press enter continue\n%s\n",err); 
					//getchar(); 
				}
			}
			opTime[threadIndex][i+10] = GetTickCount() - startTime; 
			sqlite3_close(dbHandle);
		}else{


			OpenDataBase(&dbHandle);
			ret = 0xf;
			DWORD startTime = GetTickCount(); 
			while(ret != SQLITE_OK)
			{
				ret = sqlite3_exec(dbHandle,sqll,NULL,NULL,&err); 
				if(ret != SQLITE_OK)
				{
					//printf("RW:insert error!!!press enter continue\n%s\n",err); 
					//getchar(); 
				}
			}
			opTime[threadIndex][i+10] = GetTickCount() - startTime; 
			sqlite3_close(dbHandle);

			memset(sqlExp,0,sizeof(sqlExp));
			sprintf(sqlExp,"%s%d",sqlc,GetRandomIndex()); 
			ret = 0xf;
			startTime = GetTickCount(); 
			OpenDataBase(&dbHandle);
			while(ret != SQLITE_OK)
			{
				ret = sqlite3_exec(dbHandle,sqlExp,GetRec,NULL,&err); 
				if(ret != SQLITE_OK)
				{
					//printf("RW:select error!!!press enter continue\n%s\n",err); 
					//getchar(); 
				}
			}
			opTime[threadIndex][i] = GetTickCount() - startTime; 
			sqlite3_free(err); 
			sqlite3_close(dbHandle);
		}
	}

	
}


int main()
{
	sqlite3 *dbHandle; 

	double aversum = 0.0; 

	OpenDataBase(&dbHandle); 
	sqlite3_exec(dbHandle,"create table t(id int primary key,msg varchar(128),msg1 varchar(128),msg2 varchar(128),msg3 varchar(128),msg4 varchar(128),msg5 varchar(128), \
	msg6 varchar(128),msg7 varchar(128),msg8 varchar(128),msg9 varchar(128),msg10 varchar(128),msg11 varchar(128),msg12 varchar(128),msg13 varchar(128))",NULL,NULL,&err); 
	sqlite3_close(dbHandle); 


	double min1=0,max1=0; 
	char sqlr2[3500]; 
	char aaa[135]={0}; 
	aaa[0] = '\"'; 
	//aaa[1]= '\"'; 
	memset(aaa+1,'q',128); 
	aaa[129] = '\"'; 
	aaa[130] = ','; 
	
	memset(sqlr2,0,3500); 

	for(int i=0;i<14;i++)
		strcat(sqlr2,aaa); 
	int len = strlen(sqlr2); 
	sqlr2[len-1] = 0; 
	sprintf(sqll, "insert into t (msg,msg1,msg2,msg3,msg4,msg5,msg6,msg7,msg8,msg9,msg10,msg11,msg12,msg13) values (%s)",sqlr2);
	
	memset(opTime,0,sizeof(opTime));
	
	
	for(int i = 0; i < 16; i++)
	{
		threadArray[i] = new std::thread(ReadOperation,i); 
	}

	WaitEnd(); 
	 
	//ReadOperation(1); 
	printf("-------------read start----------------\n");
	
	
	
	for(int i = 0; i < 16; i++)
	{
		double sum = 0.0; 

		printf("thread %d result :",i+1);
		for(int j = 0; j < 10; j++)
		{
			printf(" %3.3f",(double)opTime[i][j]/1000); 
			sum += (double)opTime[i][j]; 
		}
		max = max > sum ? max : sum; 
		if(i == 0)
		  min = sum; 
		min = min < sum ? min:sum; 
		printf("\nsum:%3.3f\naverage:%3.3f\n\n",sum / 1000,sum / 10000); 
		aversum += sum; 
	}

	printf("per thread per operation:%3.3f\nmax:%3.3f\nmin:%3.3f\n",aversum / 160000,max/1000,min/1000); 
	printf("-------------read end----------------\n\n");
	getchar(); 
	memset(opTime,0,sizeof(opTime)); 

	min = 0; 
	max = 0; 

	aversum = 0.0; 
	for(int i = 0; i < 16; i++)
	{
		threadArray[i] = new std::thread(WriteOperation,i); 
	}

	WaitEnd();
	//WriteOperation(1); 
	printf("-------------write start----------------\n");
	for(int i = 0; i < 16; i++)
	{
		double sum = 0; 

		printf("thread %d result read :",i+1);
		for(int j = 0; j < 10; j++)
		{
			printf(" %3.3f",(double)opTime[i][j] / 1000); 
			sum += (double)opTime[i][j]; 
		}
		
		double s = sum / 10; 
		max = max > sum ? max : sum; 
		if(i == 0)
		  min = sum; 
		min = min < sum ? min:sum; 
		printf("\nsum:%3.3f\naverage:%3.3f\n\n",sum / 1000,s / 1000); 
		aversum += sum; 
	}

	printf("per thread per operation:%3.3f\nmax:%3.3f\nmin:%3.3f\n",aversum / 160000,max /1000,min/1000); 
	printf("-------------write end----------------\n\n");
	getchar(); 
	memset(opTime,0,sizeof(opTime)); 
	
	min = 0;
	max =0 ; 
	
	aversum = 0.0;
	for(int i = 0; i < 16; i++)
	{
		threadArray[i] = new std::thread(RWOperation,i); 
	}

	WaitEnd(); 
	double aversumx = 0.0; 
	aversum = 0.0; 
	printf("-------------RW start----------------\n");
	for(int i = 0; i < 16; i++)
	{
		double sum = 0; 

		printf("thread %d result read ",i+1);
		for(int j = 0; j < 10; j++)
		{
			printf(" %3.3f",(double)opTime[i][j] / 1000 ); 
			sum += (double)opTime[i][j]; 
		}
		max = max > sum ? max : sum; 
		if(i == 0)
		  min = sum; 
		min = min < sum ? min:sum; 
		printf("\nsum:%3.3f\naverrage:%3.3f\n\n",sum / 1000,sum / 10000); 
		aversumx += sum; 



		sum = 0.0; 

		printf("thread %d result write	 ",i+1);
		for(int j = 0; j < 10; j++)
		{
			printf(" %3.3f",(double)opTime[i][j+10] / 1000); 
			sum += (double)opTime[i][j+10]; 
		}
		max1 = max1 > sum ? max1 : sum; 
		if(i == 0)
		  min1 = sum; 
		min1 = min1 < sum ? min1:sum; 
		printf("\nsum:%3.3f\naverage:%3.3f\n\n",sum / 1000,sum / 10000); 
		aversum += sum; 

	}
	printf("per thread per operation:\nread:%3.3f\nwrite:%3.3f\nmax read :%3.3f\nmin read:%3.3f\nmax write:%3.3f\nmin write:%3.3f",aversumx / 160000,aversum / 160000,max / 1000,min / 1000, max1 / 1000, min1/ 1000); 
	printf("-------------RW end----------------\n\n");

	memset(opTime,0,sizeof(opTime)); 

	getchar();
}

