#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

int readline(char *newarg[],int xarg_argc);

int 
main(int argc ,char const *argv[])
{
   if (argc < 2){
    fprintf(2,"Usage: xargs command arg...");
    exit(1);
   }
   char *newarg[MAXARG];
   for (int i=1;i<argc;i++)
   { newarg[i-1]=(char*)malloc(strlen(argv[i])+1);
     strcpy(newarg[i-1],argv[i]);
   }
   char * command=(char*)malloc(strlen(argv[1])+1);
   strcpy(command,argv[1]);
   while(readline(newarg,argc))
   {
        
        if(fork()==0){
            if (exec(command,newarg) == -1){
                fprintf(2,"exec ERROR");
                exit(1);
            }
            
        }else{
            wait(0);
            
        }
   }
   exit(0);
}

int
readline(char * newarg[],int xarg_argc)
{
    
    char buf[512];
    int n = 0;
    while( read(0, buf+n ,1)){
        if (n== 511){
            fprintf(2,"argument too long");
            exit(1);
        }
        if (buf[n] == '\n')break;
        ++n;
    }
    if (n==0) return 0;
    buf[n]=0;
    int i=0;
    char *new=buf;
    int j = xarg_argc-1;
    while (i <n){
        new = buf+i;
        newarg[j++] = new;
        while(buf[i] != ' '&& i<n){
            i++;
        }
        while(buf[i] == ' '&& i<n ){
            buf[i++] = 0;
        }
        
    }
    newarg[j]=0;
    return 1;
}
