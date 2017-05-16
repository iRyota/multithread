#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdio.h>
#include <unistd.h>

#define SHMSZ     27

int main()
{
  char c;
  int shmid, semid;
  key_t shmkey, semkey;
  char *shm, *s;
  struct sembuf sbuf;

  /*
   * $B6&M-%a%b%j%;%0%a%s%H$K(B"5678"$B$H$$$&L>A0$r$D$1$k!%(B
   */
  shmkey = 5678;

  /*
   * $B6&M-%a%b%j%;%0%a%s%H$r:n@.$9$k!%(B
   */
  if ((shmid = shmget(shmkey, SHMSZ, IPC_CREAT | 0666)) < 0) {
    perror("shmget");
    return 1;
  }

  /*
   * $B6&M-%a%b%j%;%0%a%s%H$rK\%W%m%0%i%`$N%G!<%?%9%Z!<%9$K%"%?%C%A$9$k(B.
   */
  if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
    perror("shmat");
    return 1;
  }

  /*
   * $B%;%^%U%)$K(B"1234"$B$H$$$&L>A0$r$D$1$k!%(B
   */
  semkey = 1234;

  /*
   * $B%;%^%U%)$r:n@.$9$k!%(B
   */
  if ((semid=semget(semkey, 1, IPC_CREAT | 0666)) < 0){
    perror("semget");
    return 1;
  }

  /*
   * $B%;%^%U%)$r=i4|2=$9$k(B
   */
  if (semctl(semid, 0, SETVAL, 1) < 0){
    perror("semctl");
    return 1;
  }

  /*
   * $B%;%^%U%)$G%m%C%/$9$k(B
   */
  sbuf.sem_num=0;
  sbuf.sem_op=-1;
  sbuf.sem_flg=0;
  if (semop(semid,&sbuf,1) < 0){
    perror("semop");
    return 1;
  }

  /*
   * $B6&M-%a%b%j$KB>$N%W%m%;%9$,FI$`$h$&$K$J$s$i$+$N%G!<%?$r=q$-9~$`(B
   */
  s = shm;
  for (c = 'a'; c <= 'z'; c++)
    *s++ = c;
  *s = 0;

  /*
   * $B%-!<%\!<$+$i2?$i$+$NJ8;z$,F~NO$5$l$k$^$G%m%C%/$5$l$k(B
   */
  printf("press any keys to unlock.\n");
  getchar();

  /*
   * $B%;%^%U%)$r%"%s%m%C%/$9$k(B
   */
  sbuf.sem_num=0;
  sbuf.sem_op=1;
  sbuf.sem_flg=0;
  if (semop(semid,&sbuf,1) < 0){
    perror("semop");
    return 1;
  }

  /*
   * $BB>$N%W%m%;%9$,6&M-%a%b%j$KFI$_9~$_=*N;$N9g?^$H$7$F(B'*'$B$K=q$-9~$`(B
   */
  while (*shm != '*')
    sleep(1);

  /*
   * $B6&M-%a%b%j%;%0%a%s%H$r<h$j30$9(B
   */
  shmdt(shm);


  /*
   * $B%;%^%U%)$r:o=|$9$k(B
   */
  if (semctl(semid, 0, IPC_RMID) < 0){
    perror("semctl");
    return 1;
  }

  return 0;
}

