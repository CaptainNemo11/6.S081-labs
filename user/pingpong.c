#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int 
main(int argc, char const * argv[])
{   int p[2];
    pipe(p);
    char buf[2];

    if (argc > 1){
        fprintf(2,"Usage: pingpong\n");
        exit(1);
    }
    int pid=fork();
    if (pid < 0){
        fprintf(2,"ERROR fork()\n");
        exit(1);

    }
    else if (pid > 0){
        //parent
        pid=getpid();
        char info[2]="a";
        if (write(p[1],info,1) != 1)fprintf(2,"parent failed to send");
        close(p[1]);
        if (read(p[0],buf,1) !=1)fprintf(2,"parent failed to recieve");
        fprintf(1,"%d: received pong\n",pid);
        close(p[0]);
        exit(0);
    }
    else{//child
        wait(0);
        pid=getpid();
        if (read (p[0],buf,1) != 1)fprintf(2,"child failed to receive");
        fprintf(1,"%d: received ping\n",pid);
        close(p[0]);
        if (write(p[1],buf,1) != 1) fprintf(2,"child failed to send");
        close(p[1]);
        exit(0);
    }
}
