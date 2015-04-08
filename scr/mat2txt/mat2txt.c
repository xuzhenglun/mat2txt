#include "mat.h"
#include <stdio.h>
#include "freegetopt-0.11\getopt.h"
#include <stdlib.h>
#include <string.h>
#pragma comment(lib,"libmat.lib")
#pragma comment(lib,"libmx.lib")
#define MAX 128

int main(int argc, char *argv[])
{
	char * path = NULL;
	char * target = NULL;
	char c;
	for (c = getopt(argc, argv, "p:t:h"); c != -1; c = getopt(argc, argv, "p:t:h"))
	{
		switch (c)
		{
		case 'p':
				path = optarg;
				break;
		case 't':
				target = optarg;
				break;
		case 'h':
				puts("-p:                     Path of Input files (*.mat)");
				puts("-t: (Optional)          Path of Output files(*.txt)");
				exit(-1);
		case '?':
				puts("wrong command");
				puts("use -h for more detail");
				exit(-1);
		}
	}

	if (argc == 2) path = argv[2];
	if (path == NULL) {
		puts("arguments error");
		exit(-1);
	}

	char name[MAX] = { '\0' };
	int t = 0;
	for (int i = 0; path[i] != '\0'; i++){
		if (path[i] == '.') break;
		name[t++] = path[i];
		if (path[i] == '\\') t = 0;
	}

	MATFile *pmat;
	const char **dir;
	int         ndir;
	mxArray *pa; 

	pmat = matOpen(path, "r");//打开文件，返回指向文件指针
	if (pmat == NULL)
	{
		printf("Input File is not exist");//cout<<"Error opening file:" <<file<<endl;
		exit(-1);
	}


	dir = (const char **)matGetDir(pmat, &ndir);
	//ndir 表示mat文件中含有矩阵数目
	if (dir == NULL)
	{
		printf("Error reading directory of file:");
		exit(-1);
	}
	else
	{
		printf("Directory of %s\n", path);
		for (int i = 0; i < ndir; i++)
			printf("No.%d=====>%s\n",ndir,dir[i]);//输出所含矩阵数目
	}

	int num;
	if (ndir != 1)
	{
		puts("There are more than one Matrix, Plz Input a number to choose the one need to be processd.");
		scanf("%d", &num);
		num--;
	}
	else
	{	
		puts("There is only one Matrix, Processing,Plz w8");
		num = 0;
	}


	char targetfile[MAX] = { '\0' };
	if (target == NULL)
	{
		strcat(strcpy(targetfile, name), ".txt");
		target = targetfile;
	}

	FILE *fp;
	fp=fopen(target, "w");//打开文件，返回指向文件指针
	if (fp == NULL)
	{
		printf("Output File is invalid");//cout<<"Error opening file:" <<file<<endl;
		exit(-1);
	}


	mxArray *p = matGetVariable(pmat, dir[num]);
	double * initA = (double*)mxGetData(p);
	int M = mxGetM(p);
	int N = mxGetN(p);
	int count = 0;
	for (int i = 0; i < M; i++){
		for (int j = 0; j < N; j++)
		{
			fprintf(fp, "   %1.7e", initA[M*j + i]);
		}
		fprintf(fp, "\n");
	}
	fclose(fp);
	matClose(pmat);
	return 0;
}