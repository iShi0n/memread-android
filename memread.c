#include <sys/ptrace.h>
#include <sys/wait.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[ ]){;
    if(argc == 4){
        int pid = strtol(argv[1], NULL, 10);
        unsigned int address = strtoul(argv[2], NULL, 0);
        int size = strtol(argv[3], NULL, 10);

        if(ptrace(PTRACE_ATTACH, pid, NULL, NULL) == -1){
            printf("error: could not attach to pid %d\n", pid);
            return 1;
        }

        int status;
        if(waitpid(pid, &status, 0) == -1 || !WIFSTOPPED(status)){
            printf("error: unknown\n");
            return 1;
        }
        
        char* proc_mem = malloc(50);
        sprintf(proc_mem, "/proc/%d/mem", pid);
        
        int fd_proc_mem = open(proc_mem, O_RDONLY);
        if(fd_proc_mem == -1){
            printf("error\n");
            return 1;
        }

        char *buf = malloc(size);
        char *string;
        
        lseek(fd_proc_mem, address, SEEK_SET);
        read(fd_proc_mem, buf, size);
        ptrace(PTRACE_DETACH, pid, NULL, NULL);
        sprintf(&string, "%s", buf);
        printf("string: %s\n", buf);
        printf("hex: ");
        for(int i=0; i<size; i++){
            printf("%#x ", buf[i]);
        }
        printf("\n");
        printf("dec: %d\n", string);
        close(fd_proc_mem);
        free(proc_mem);
        free(buf);
    }
    else{
        printf("usage: %s pid address(0x123) size\n", argv[0]);
        return 1;
    }
    
    return 0;
}
