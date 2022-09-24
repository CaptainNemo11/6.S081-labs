#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
void find(char const *path,char const *name);
int
main(int argc, char const *argv[])
{
    if (argc < 3){
        fprintf(2,"Usage:find path name");   
        exit(1);
    }
    else{
        find(argv[1],argv[2]);
        exit(0);
    }
}

void 
find (char const *path,char const *name) 
{
   struct stat st;
   struct dirent de;
   int fd;
   char buf[512];  
   char *p; 
   if ((fd=open(path,0))<0)
   {
        fprintf(2,"cannot open %s\n",path);
        exit(1);
   }

   if (fstat(fd,&st)<0)
   {
        fprintf(2,"cannot stat %s\n",path);
        close(fd);
        exit(1);
   }

    switch(st.type){
        case T_FILE:
        fprintf(2,"Usage:find dir name");
        exit(1);
        break;
        case T_DIR:
        while(read(fd,&de,sizeof(de))==sizeof(de)){
            if (de.inum==0 || !strcmp(de.name,"." )|| !strcmp(de.name,"..")){
                continue;
            }
            if(strlen(path)+1+DIRSIZ+1>sizeof(buf)){
                fprintf(2,"find: path too long");
                exit(1);
            }
            strcpy(buf,path);
            p=buf+strlen(buf);
            *p++ = '/';
            memmove(p,de.name,DIRSIZ);
            p[DIRSIZ]=0;
            if (stat(buf,&st) < 0){
                fprintf(2,"cannot stat %s\n",buf);
                continue;
            }
            switch(st.type){
                case T_FILE:
                if (!strcmp(de.name,name)){
                    printf("%s\n",buf);   
                }
                break;
                case T_DIR:
                find(buf,name);
                break;
            }
        }
        break;

    }
    close(fd);
   
   
   
}