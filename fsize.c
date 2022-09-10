#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <locale.h>
#include <langinfo.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>


#define verde "\x1b[32m"//colores ansi
#define azul "\x1b[34m"
#define negro "\x1b[0m"

FILE *name;
int bandera = 0;

#define MAX_PATH 1024


void fsize (char *name){
	struct stat stbuf;
	if(stat(name, &stbuf) == -1){
		fprintf(stderr, "fsize: no puede abrir %s", name);
		return;
	}
	
	if(bandera==1){
			printf(negro " %s \n",  name);
	}
	
	if(bandera==2){
		dirwalk(name,fsize);
	}

	if(bandera==3){
	printf((S_ISDIR(stbuf.st_mode)) ? negro "d" : negro "-");
    printf( (stbuf.st_mode & S_IRUSR) ? "r" : "-");
    printf( (stbuf.st_mode & S_IWUSR) ? "w" : "-");
    printf( (stbuf.st_mode & S_IXUSR) ? "x" : "-");
    printf( (stbuf.st_mode & S_IRGRP) ? "r" : "-");
    printf( (stbuf.st_mode & S_IWGRP) ? "w" : "-");
    printf( (stbuf.st_mode & S_IXGRP) ? "x" : "-");
    printf( (stbuf.st_mode & S_IROTH) ? "r" : "-");
    printf( (stbuf.st_mode & S_IWOTH) ? "w" : "-");
    printf( (stbuf.st_mode & S_IXOTH) ? "x" : "-");

	
	struct passwd *pwd;
	struct group *grp;
	struct tm  *tm;
	char   datestring[256];
	struct dirent  *dp;	

/* Print out group name if it is found using getgrgid(). */
    if ((grp = getgrgid(stbuf.st_gid)) != NULL)
        printf(" %-8.8s", grp->gr_name);
   	 else
        printf(" %-8d", stbuf.st_gid);
	
	if ((pwd = getpwuid(stbuf.st_uid)) != NULL)
        printf(" %-8.8s", pwd->pw_name);
    else
        printf(" %-8d", stbuf.st_uid);

	printf("%8ld  ", stbuf.st_size);

	tm = localtime(&stbuf.st_mtime);
    
    strftime(datestring, sizeof(datestring), nl_langinfo(D_T_FMT), tm);

	printf(" %s ", datestring);

	if(S_ISDIR(stbuf.st_mode))
		printf(azul " %s \n",  name);
	else
		if(stbuf.st_mode & S_IXUSR||stbuf.st_mode & S_IXGRP||stbuf.st_mode & S_IXOTH)
			printf(verde" %s \n",  name);
		else
			printf(negro " %s \n",  name);
}

}

void dirwalk(char *dir, void (*fcn)(char *)){
    char name[MAX_PATH];
    struct dirent *dp;

    DIR *dfd;

    if ((dfd = opendir(dir)) == NULL) {
            fprintf(stderr, "dirwalk: can't open %s\n", dir);
            return;
    }

    while ((dp = readdir(dfd)) != NULL) {
            if (strcmp(dp->d_name, ".") == 0
                            || strcmp(dp->d_name, "..") == 0)
                    continue;
            if (strlen(dir)+strlen(dp->d_name)+2 > sizeof(name))
                    fprintf(stderr, "dirwalk: name %s/%s too long\n",
                                    dir, dp->d_name);
            else {
                    sprintf(name, "%s/%s", dir, dp->d_name);
                    (*fcn)(name);
            }
    }
    closedir(dfd);
}

/* Muestra el tamaño de los achivos */
int main (int argc, char **argv) {
	int c; 
	while((c= getopt(argc, argv, "flR:")) !=- 1){
//	printf("[%c]", c);
		switch(c){
			case 'f':
				bandera=1;
			break;

			case 'l':
				bandera=3;
			break;

			case'R':
				bandera=2;
			break;
	}
}
if(argc == 1)
					fsize(".");
				else
			while(--argc >0 )
				fsize(*++argv);

return 0;
}
