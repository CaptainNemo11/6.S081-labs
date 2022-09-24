#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void neighbor(int p[2]);

int
main(int argc , char const *argv[])
{
    if (argc > 1){
        fprintf(2,"Usage:primes\n");
        exit(1);
    }
    int p[2];
    pipe(p);
    int pid =fork();
    
    if (pid < 0){
        fprintf(2,"fork ERROR\n");
        exit(1);
    }
    else if (pid > 0){
        close(p[0]);
        for (int i=2;i <= 35;i++){
            if (write(p[1],&i,4)!=4){
                fprintf(2,"failed to write %d to child",i );
                exit(1);
            }
        }
        close(p[1]);
        wait(0);
        
    }
    else{
        neighbor(p);
    
    }
    exit(0);
    

}

void
neighbor(int p[2]){

    close(p[1]);
    int prime;
    if (read(p[0],&prime,4) != 4){
            fprintf(2,"failed to read from parent\n");
            exit(1);
        }
    printf("prime %d\n",prime);
    int flag;
    int n;
    int newp[2];
    int pid;
    flag=read(p[0],&n,4);
    if (flag){
        if (flag != 4)fprintf(2,"failed to read from neighbor");
        pipe(newp);
        pid =fork();
        if (pid < 0)fprintf(2,"fork ERROR\n");
        else if (pid > 0){
            close(newp[0]);
            do{
                if (n%prime){
                    if(write(newp[1],&n,4)!=4)fprintf(2,"failed to write to neighbor");
                }
            }while(read(p[0],&n,4));
            close(p[0]);
            close(newp[1]);
            wait(0);
        }
        else{
            neighbor(newp);
        }
    }

    
    
}
