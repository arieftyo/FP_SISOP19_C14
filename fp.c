#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <pthread.h>
#include<sys/wait.h>

pthread_t tid[100];
int urutan;
char cob[100];
int val[100];

void* execution()
{
  time_t rawtime;
  struct tm *timeinfo;
  time(&rawtime);
  timeinfo = localtime(&rawtime);
  pthread_t id = pthread_self();
  int time = 0;
  if(urutan > 0){
    time = (urutan * 2) + 1; 
  }
  if(pthread_equal(id,tid[urutan])) 
  {
    if(val[time]==0)
      val[time]==timeinfo->tm_min;
    if(val[time+1]==0)
      val[time+1] = timeinfo->tm_min;
    if(val[time+2]==0)
      val[time+2] = timeinfo->tm_mday;
    if(val[time+3]==0)
      val[time +3] = timeinfo->tm_mon+1;
    if(val[time + 4]==0)
      val[time+4]=timeinfo->tm_wday;
    if((val[time] == timeinfo->tm_min)
      && (val[time+1] == timeinfo->tm_min)
      && (val[time+2] == timeinfo-> tm_mday)
      && (val[time+3] == (timeinfo-> tm_mon + 1))
      && (val[time+4] == timeinfo-> tm_wday))
      system(cob);
    pthread_exit(NULL);
  }
}

int main() {
  pid_t pid, sid;
  int urutan=0;

  pid = fork();

  if (pid < 0) {
    exit(EXIT_FAILURE);
  }

  if (pid > 0) {
    exit(EXIT_SUCCESS);
  }

  umask(0);

  sid = setsid();

  if (sid < 0) {
    exit(EXIT_FAILURE);
  }

  if ((chdir("/")) < 0) {
    exit(EXIT_FAILURE);
  }

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  while(1){
    urutan = 0;
    
    FILE *file =fopen("/home/ariefp/fp/crontab.data", "r");
    int i= 0;
    int j = 0;
    int err;
    // char r;
    // char d ;
    // cob[0] =fgetc(file);
    // printf("%c\n", cob[0]);
    // cob[1] = fgetc(file);
    // printf("%c\n", cob[1]);

    while(1){
      char c[100] ;
      if (fgets(c, 100, file)==NULL)
      {
        break;
      }
      int k=0;
      int hitung_spasi=0;
      while(c[k]!='\n' && c[k]!='\0'){
        if(c[k]==' '){
          if (hitung_spasi==5)
          {
            k++;
            memset(cob, 0 , sizeof cob);
            i =0;
            break;
          }
          else{
            val[j] = atoi(cob);
            printf("%d\n", val[j]);
            j++;
            memset(cob, 0, sizeof cob);
            i = 0;
            hitung_spasi++;
          }
        }
        else{
          cob[i] = c[k];
          i++;
        }
        k++;
      }
      int l=k;
      while(c[l]!='\n' && c[l]!='\0'){
        cob[i] = c[l];
      }
      err = pthread_create(&(tid[urutan]), NULL, &execution, NULL);
      if (err != 0)
        printf("\ncan't create thread :[%s]", strerror(err));
      urutan++;
    }
  int iter = 0;
  while(iter <= urutan){
    pthread_join(tid[iter],NULL);
    iter++;
  }
  sleep(60);
 }
 return 0;
}