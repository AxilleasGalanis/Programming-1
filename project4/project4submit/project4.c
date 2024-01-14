#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

int max(int num1, int num2){
    return (num1 > num2 ) ? num1 : num2;
}

int find_arguments(char *argv[],char *argv_name,char ***args){
    int i=1,j=1,k=0,res,counter=1;
    char *str=NULL,*p=NULL; //str=the string that reads the args one by one
    FILE *f;

    str=calloc(2,sizeof(char)); // single character plus end byte=2
    f=fopen(argv[2],"r");
    while(1){ //find the number of arguments
        res=fread(&str[0],sizeof(char),1,f); //read one character at a time
        if(res==0){
            break;
        }

        if(strpbrk(str," ")!=NULL||strpbrk(str,"\n")!=NULL||strpbrk(str,"\r")!=NULL){ //searches for specific chars
            res=fread(&str[0],sizeof(char),1,f);
            if(strpbrk(str," ")!=NULL||strpbrk(str,"\n") !=NULL||strpbrk(str,"\r")!=NULL||res==0){ //searches for consecutive specific chars
                i++;
                break;
            }
            i++;
            fseek(f,-1,SEEK_CUR);
        }
    }
    free(str);
    rewind(f);
        
    str=calloc(2,sizeof(char)); // single character plus end byte=2
    (*args)=calloc(i,sizeof(char *));
    (*args)[0]=calloc(strlen(argv_name)+1,sizeof(char));
    strcpy((*args)[0],argv_name);
        
    while(1){
        while(1){ //inserts args to the vector
            res=fread(&str[k],sizeof(char),1,f);
            if(res==0){
                (*args)=realloc((*args),(i+1)*sizeof(char *));
                (*args)[i]=NULL;
                free(str);
                fclose(f);
                return i;
            }
            p=strpbrk(str,"  \n\r");
            if(p!=NULL){ //searches for specific chars
                p=NULL;
                str[k]='\0';
                (*args)[j]=calloc(strlen(str)+1,sizeof(char));
                strcpy((*args)[j],str);
                res=fread(&str[k],sizeof(char),1,f);
                p=strpbrk(str,"  \n\r");
                if(p!=NULL||res==0){ //searches for consecutive specific chars
                    p=NULL;
                    (*args)=realloc((*args),(i+1)*sizeof(char *));
                    (*args)[i]=NULL;
                    free(str);
                    fclose(f);
                    return i;
                }
                fseek(f,-1,SEEK_CUR);
                break;
            }
            k++;
            counter++;
            str=realloc(str,(counter+1)*sizeof(char));
            str[k+1]='\0';
        }
        free(str);
        str=calloc(2,sizeof(char)); // single character plus end byte=2
        j++;
        k=0;
        counter=1;
    }
    
    return i;
}

int main(int argc,char *argv[]){
    int i,pid1,fd1,err_flag=0,wrn_flag=0,status1,res,ret,fd[2],pid2,pid3,status2=0,status3=0,mem_err=0;
    char **args=NULL,*err_out,*name,str[64],*argv_name,*name2;

    if(argc!=5){
        fprintf(stderr,"Wrong arguments\n");
        return 2;
    }
    name=calloc(strlen(argv[1])+1,sizeof(char));
    err_out=calloc(strlen(argv[1])+4,sizeof(char));
    name2=calloc(30,sizeof(char));
    argv_name=calloc(strlen(argv[1])+1,sizeof(char));

    strcpy(err_out,argv[1]);
    strcpy(name2,argv[1]);
    strtok(err_out,".");
    strcat(err_out,".err");
    strtok(argv[1],".");
    strcpy(name,argv[1]);
    strcpy(argv_name,argv[1]);
    strtok(name,"\0");
    strtok(argv_name,"\0");
    strcat(name,"\0");
    strcat(argv_name,"\0");
    ret=find_arguments(argv,argv_name,&args);

    pid1=fork(); //start process p1
    if(pid1==0){
        fd1=open(err_out,O_RDWR|O_CREAT|O_TRUNC,S_IRWXU);
        dup2(fd1,2);  //redirect stderr to <progname>.err
        execl("/usr/bin/gcc","gcc","-Wall",name2,"-o",name,NULL);
        close(fd1);
    }

    waitpid(pid1,&status1,0); //end process p1
    fd1=open(err_out,O_RDONLY);
    while(1){
        res=read(fd1,str,63);
        str[res]='\0';
        if(strstr(str,"error")!=NULL){ //finds "error" string
            err_flag=1;
            break;
        }
        else if(strstr(str,"warning")!=NULL){ //finds "warning" string
            wrn_flag=1;
        }
        if(res==0){
            break;
        }
        for(i=0;i<64;i++){
            str[i]=0;
        }
    }
    close(fd1);

    if(err_flag==1){
        printf("Compilation: %d\n",-100);
        printf("Output: %d\n",0);
        printf("Memory access: %d\n",0);
        printf("Total: %d\n",0);
        free(name);
        free(name2);
        free(err_out);
        free(argv_name);
        for(i=0;i<ret;i++){
            free(args[i]);
        }
        free(args);
        return 0;
    }

    if(err_flag==0){
        pipe(fd);
        pid2=fork(); //start process 2
        if(pid2==0){
            fd1=open(argv[3],O_RDWR,S_IRWXU);
            dup2(fd1,0); //redirect stdin to <progname>.in
            dup2(fd[1],1); //redirect stdout to pipe's write end
            close(fd[0]);
            close(fd[1]);
            close(fd1);
            execv(name,args);
        }
        else{
            pid3=fork(); //start process 3
            if(pid3==0){
                dup2(fd[0],0); //redirect p4diff's input to pipe's read end
                close(fd[0]);
                close(fd[1]);
                execl("./p4diff","p4diff",argv[4],NULL);
            }
        }
        close(fd[0]);
        close(fd[1]);
        waitpid(pid2,&status2,0); //end process p2
        waitpid(pid3,&status3,0); //end process p3

        if(WIFEXITED(status2) ) {
            mem_err=0;
        }
        else if(WIFSIGNALED(status2)) {
            if(WTERMSIG(status2)==6||WTERMSIG(status2)==10||WTERMSIG(status2)==11){
                mem_err=1;
            }
        }

    }

    int x=0,y=0,z=0,total=0;
    if(wrn_flag==1){
        printf("Compilation: %d\n",-10);
        x=-10;
    }
    else{
        printf("Compilation: %d\n",0);
    }

    y=WEXITSTATUS(status3);
    printf("Output: %d\n",y);


    if(mem_err==1){
        printf("Memory access: %d\n",-15);
        z=-15;
    }
    else{
        printf("Memory access: %d\n",0);
        z=0;
    }

    total=max(x+y+z,0);
    printf("Total: %d\n",total);
    free(name);
    free(name2);
    free(err_out);
    for(i=0;i<ret;i++){
        free(args[i]);
    }
    free(argv_name);
    free(args);
    return 0;
}