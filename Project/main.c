#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <io.h>
#include <time.h>

typedef struct _finddata_t FILE_SEARCH;

typedef struct __Filedata {
	int rank;
	char * Filename;
}Filedata;

int compare(void * first, void * second) {
	if (((Filedata *)first) -> rank < ((Filedata *)second)->rank)
		return 1;
	else if (((Filedata *)first)->rank > ((Filedata *)second)->rank)
		return -1;
	else
		return 0; 
}

int main(int argc, char * argv[])
{
	FILE_SEARCH folderdata;
	FILE *fin;
	Filedata *fd;
	clock_t a=clock();
	long handle;
	int result = 1;
	int filelength, cnt = 0, fnamelength;
	char * keywordplacepointer;
	char * filestr;

	handle = _findfirst("./*.txt", &folderdata);
	if (handle == -1) return 0;
	while (result != -1) {
		result = _findnext(handle, &folderdata);
		cnt++;
	}

	fd = malloc(cnt * sizeof(Filedata));

	handle = _findfirst("./*.txt", &folderdata);
	
	for (int i = 0; i < cnt; i++) {

		fd[i].rank = 0;
		fnamelength = strlen(folderdata.name);
		fd[i].Filename = (char*)malloc((fnamelength+1) * sizeof(char));
		strcpy(fd[i].Filename, folderdata.name);
		result = _findnext(handle, &folderdata);
		
		fin = fopen(folderdata.name, "r");
		fseek(fin, 0, SEEK_END);
		filelength = ftell(fin);
		fseek(fin, 0, SEEK_SET);
		filestr = (char*)malloc((filelength+1) * sizeof(char));
		fread(filestr, 1, filelength, fin);
		filestr[filelength] = '\0';
		keywordplacepointer = filestr;
		while (keywordplacepointer != 0) {
			keywordplacepointer = strstr(keywordplacepointer, argv[1]);
			if (keywordplacepointer != 0) 
			{
				if (keywordplacepointer == filestr) 
				{
					fd[i].rank++;
					keywordplacepointer = keywordplacepointer + strlen(argv[1]);
				}
				else 
				{
					if (*(keywordplacepointer + strlen(argv[1])) == ' ' && *(keywordplacepointer - sizeof(char)) == ' ') 
					{
						fd[i].rank++;
						keywordplacepointer = keywordplacepointer + strlen(argv[1]);
					}
					else {
						keywordplacepointer = keywordplacepointer + strlen(argv[1]);
					}
				}
			}
		}
		fclose(fin);
	}
	_findclose(handle);

	qsort(fd, cnt, sizeof(Filedata), compare);
	char b[50000]="";
	for (int i = 0; i < cnt; i++) {
		sprintf(b,"%s Rank : %d / ���ϸ� : %s \n",b, fd[i].rank, fd[i].Filename);
	}
	printf("%s", b);
	printf("%2.5lf", ((double)clock() - a) / CLOCKS_PER_SEC);
	system("pause");
	return 0;
}