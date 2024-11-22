#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(){
    int pipeLine[2];
    char receiveBuff[5];

    pipe(pipeLine);
    int pid = fork(); //Tạo tiến trình con

    if(pid == 0){
        //Tiến trình con
        read(pipeLine[0], receiveBuff, 5);
        printf("%d:  received %s\n", getpid(), receiveBuff);
        close(pipeLine[0]);

        write(pipeLine[1], "pong", 5);
        close(pipeLine[1]);
    }else{
        //Tiến trình cha
        write(pipeLine[1], "ping", 5);
        wait(0);
        close(pipeLine[1]); 

        read(pipeLine[0], receiveBuff, 5);
        printf("%d:  received %s\n", getpid(), receiveBuff);
        close(pipeLine[0]);
    }
    exit(0);
}