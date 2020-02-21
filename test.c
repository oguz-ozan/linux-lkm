#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#define BUFFER_LENGTH 256               ///< The buffer length (crude but fine)
static char receive[BUFFER_LENGTH];     ///< The receive buffer from the LKM

int main(int argc, char *argv[]){
   FILE * fp;
   int fd;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    int arglen = strlen(*argv);
    int ret;
    int receivetracker = 0;
 if(!argv[1]){
        //print only cpu information.. /proc/cpu
        printf("no parameter\n");
        fp = fopen("/proc/cpuinfo","r");
        if(fp == NULL){exit(EXIT_FAILURE);}
        int ctr = 0;
        while ((read = getline(&line, &len, fp)) != -1) {
        if(ctr>0 && ctr<5){
        strcpy(receive+receivetracker, line);
        receivetracker += read;
        }
        ctr++;
        }
        printf("receive is: %s", receive);
        fclose(fp);
        if (line)
        free(line);
        }
        else{
        if(strcmp(argv[1],"-all")==0 && !argv[2]){
        //print only cpu information.. /proc/cpu
        printf("no parameter\n");
        fp = fopen("/proc/cpuinfo","r");
        if(fp == NULL){exit(EXIT_FAILURE);}
        int ctr = 0;
        while ((read = getline(&line, &len, fp)) != -1) {
        if(ctr>0 && ctr<5){
        printf("Retrieved line of length %zu:\n", read);
        printf("%s", line);
        strcpy(receive+receivetracker, line);
        receivetracker += read;
        }
        ctr++;
        }
        printf("receive is: %s", receive);
        fclose(fp);
        if (line)
        free(line);
        // cpu + system statistics..  /proc/cpu ve /proc/loadavg
        printf("-all parameter is given\n");
        fp = fopen("/proc/loadavg","r");
        if(fp == NULL){exit(EXIT_FAILURE);}
        int ctr2 = 0;
        while ((read = getline(&line, &len, fp)) != -1){
        printf("line is: %s",line);
        }
        char* token = strtok(line, " ");
        //strcpy(receive+receivetracker, token);
        //receivetracker+=strlen(token);
        //strcpy(receive+receivetracker++, " ");
        //printf("token len is: %d \n",strlen(token));
        strcpy(receive+receivetracker++,"\n");
        for(int i=0;i<3;i++){
        token = strtok(NULL," ");
        if(i==2){
        strcpy(receive+receivetracker, token);
        receivetracker += strlen(token);
        strcpy(receive+receivetracker++," ");
        strcpy(receive+receivetracker, ": This is the total number of processes.\n");
        }
        }
        printf("CPU information is \n%s",receive);
        }

        else if(strcmp(argv[1],"-p") == 0){
        //argv[2] burada process id.. hem /proc/cpu hem de /proc/ID/status
        //print only cpu information.. /proc/cpu
        fp = fopen("/proc/cpuinfo","r");
        if(fp == NULL){exit(EXIT_FAILURE);}
        int ctr3 = 0;
        while ((read = getline(&line, &len, fp)) != -1) {
        if(ctr3>0 && ctr3<5){
        strcpy(receive+receivetracker, line);
        receivetracker += read;
        }
        ctr3++;
        }
        printf("receive is: %s", receive);
        fclose(fp);
        if (line)
        free(line);
        // ended cpu part
        //print process information.. /proc/$ID/status

        fp = fopen("/proc/1/status","r");
        if(fp == NULL){exit(EXIT_FAILURE);}
        int ctr4 = 0;
        while ((read = getline(&line, &len, fp)) != -1) {
        if(ctr4>0 && ctr4<5){
        strcpy(receive+receivetracker, line);
        receivetracker += read;
        }
        ctr4++;
        }
        printf("receive is: %s", receive);
        fclose(fp);
        if (line)
        free(line);


        }else if(strcmp(argv[1],"-all")== 0){
        if(argv[2] && strcmp(argv[2],"-p")==0){
        //argv[3] burada process id. hem /proc/cpu hem /proc/ID/status hem de /proc/loadavg
        printf("all ve process parametreleri girilmis\n");
        }
        }
}

   fd = open("/dev/lab2", O_RDWR);
   ret = write(fd, receive, strlen(receive)); // Send the string to the LKM
   close(fd);
if (ret < 0){
      perror("Failed to write the message to the device.");
      return errno;
   }
 return 0;
}
