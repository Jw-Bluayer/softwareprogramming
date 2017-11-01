#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <io.h>

typedef struct _finddata_t FILE_SEARCH;

typedef struct __Filedata {
	int rank;
	char * Filename;
}Filedata;

int compare(void * first, void * second) {
	/*if (((Filedata *)first)->rank < ((Filedata *)second)->rank)
		return 1;
	else if (((Filedata *)first)->rank > ((Filedata *)second)->rank)
		return -1;
	else
		return 0; */
	
	if (((Filedata *)first)-> rank == ((Filedata *)second)-> rank) {
		return (strcmp(((Filedata *)second)-> Filename, ((Filedata *)first)-> Filename));
	}
	
	return (((Filedata *)first)->rank < ((Filedata *)second)->rank);
}

int main(int argc, char * argv[])
{
	FILE_SEARCH folderdata;
	FILE *fin;
	Filedata *fd;

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
			if (keywordplacepointer != 0) {
				fd[i].rank++;
				keywordplacepointer = keywordplacepointer+strlen(argv[1]);
			}
		}
		fclose(fin);
	}
	_findclose(handle);

	qsort(fd, cnt, sizeof(Filedata), compare);

	for (int i = 0; i < cnt; i++) {
		printf("%d / ÆÄÀÏ¸í : %s \n", fd[i].rank, fd[i].Filename);
	}

		system("pause");
		return 0;
}