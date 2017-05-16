#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>

#define SHMSZ     27

int main()
{
  char c;
  int shmid;
  key_t key;
  char *shm, *s;

  /*
   * $B6&M-%a%b%j%;%0%a%s%H$K(B"5678"$B$H$$$&L>A0$r$D$1$k!%(B
   */
  key = 5678;

  /*
   * $B6&M-%a%b%j%;%0%a%s%H$r:n@.$9$k!%(B
   */
  if ((shmid = shmget(key, SHMSZ, IPC_CREAT | 0666)) < 0) {
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
   * $B6&M-%a%b%j$KB>$N%W%m%;%9$,FI$`$h$&$K$J$s$i$+$N%G!<%?$r=q$-9~$`(B
   */
  s = shm;
  for (c = 'a'; c <= 'z'; c++)
    *s++ = c;
  *s = 0;

  /*
   * $BB>$N%W%m%;%9$,6&M-%a%b%j$KFI$_9~$_=*N;$N9g?^$H$7$F(B'*'$B$K=q$-9~$`(B
   */
  while (*shm != '*')
    sleep(1);

  /*
   * $B6&M-%a%b%j%;%0%a%s%H$r<h$j30$9(B
   */
  shmdt(shm);

  return 0;
}

