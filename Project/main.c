#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE

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

char * plural(char word[]) {
	int length;
	char * str;
	length = strlen(word);

	if ((length > 1 && word[length - 2] != 'a' && word[length - 2] != 'e' && word[length - 2] != 'i' && word[length - 2] != 'o'
		&& word[length - 2] != 'u' && word[length - 1] == 'y') ) {
		str = (char*)malloc(length + 3);
		strcpy(str, word);
		strcpy(str + length - 1, "ies");
	}

	else if (word[length - 1] == 's' || word[length - 1] == 'x' || word[length - 1] == 'z' || (length > 1 && word[length - 2] == 'c' && word[length - 1] == 'h') 
		|| (length > 1 && word[length - 2] == 's' && word[length - 1] == 'h')) {
		str = (char*)malloc(length + 3);
		strcpy(str, word);
		strcat(str, "es");
	}

	else {
		str = (char*)malloc(length + 2);
		strcpy(str, word);
		strcat(str, "s");
	}
	
	return str;

}

int compare(void * first, void * second) 
{
	if (((Filedata*)first)->rank == ((Filedata*)second)->rank) {
		return strcmp(((Filedata*)first)->Filename, ((Filedata*)second)->Filename);
	}
	return ((Filedata*)first)->rank > ((Filedata*)second)->rank ? -1 : 1;
}


int main(int argc, char * argv[])
{
	FILE_SEARCH folderdata;
	FILE *fin;
	Filedata *fd;
	clock_t a=clock();
	long handle;
	int result = 1;
	int filelength, cnt = 0, fnamelength, filecnt = 0;
	int j = 0;
	char * keywordplacepointer, *pkeywordplacepointer;
	char * filestr, * pluralargv = plural(argv[1]);
	char b[50000] = "";

	handle = _findfirst("./*.txt", &folderdata);
	if (handle == -1) {
		printf("There is no file");
		return 0;
	}
	while (result != -1) 
	{
		result = _findnext(handle, &folderdata);
		cnt++;
	}

	fd = malloc(cnt * sizeof(Filedata));

	handle = _findfirst("./*.txt", &folderdata);
	
	for (int i = 0; i < cnt; i++) 
	{
		fd[i].rank = 0;
		fnamelength = strlen(folderdata.name);
		fd[i].Filename = (char*)malloc((fnamelength+1) * sizeof(char));
		strcpy(fd[i].Filename, folderdata.name);
		
		fin = fopen(folderdata.name, "r");
		fseek(fin, 0, SEEK_END);
		filelength = ftell(fin);
		fseek(fin, 0, SEEK_SET);
		filestr = (char*)malloc((filelength+1) * sizeof(char));
		fread(filestr, 1, filelength, fin);
		filestr[filelength] = '\0';
		argv[1] = strlwr(argv[1]);
		filestr = strlwr(filestr);
		pluralargv = strlwr(pluralargv);
		keywordplacepointer = filestr;
		pkeywordplacepointer = filestr;
		while (pkeywordplacepointer != 0) {
			pkeywordplacepointer = strstr(pkeywordplacepointer, pluralargv);
			if (pkeywordplacepointer != 0)
			{
				if (pkeywordplacepointer == filestr)
				{
					fd[i].rank++;
					pkeywordplacepointer = pkeywordplacepointer + strlen(pluralargv);
				}
				else
				{
					if (isalpha(*(pkeywordplacepointer + strlen(pluralargv))) == 0 && isalpha(*(pkeywordplacepointer - sizeof(char))) == 0)
					{
						fd[i].rank++;
						pkeywordplacepointer = pkeywordplacepointer + strlen(pluralargv);
					}

					else {
						pkeywordplacepointer = pkeywordplacepointer + strlen(pluralargv);
					}
				}
			}
		}

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
					if (isalpha(*(keywordplacepointer + strlen(argv[1]))) == 0 && isalpha(*(keywordplacepointer - sizeof(char))) == 0)
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
		result = _findnext(handle, &folderdata);
	}
	_findclose(handle);
	
	qsort(fd, cnt, sizeof(Filedata), compare);

	for (int i = 0; i < cnt; i++) {
		if (fd[i].rank == 0) {
			filecnt++;
		}
	}
	
	cnt -= filecnt;

	if (cnt == 0) {
		printf("키워드가 들어 간 파일이 없습니다!");
	}
	else {
		for (int i = 0; i < cnt; i++) {
			sprintf(b, "%s Rank : %d / 파일명 : %s \n", b, fd[i].rank, fd[i].Filename);
		}
	}
	printf("%s", b);
	printf("%2.5lf", ((double)clock() - a) / CLOCKS_PER_SEC);
	return 0;
}