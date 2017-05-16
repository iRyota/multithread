#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>


#define SHMSZ     27

int main()
{
  int shmid;
  key_t key;
  char *shm, *s;

  /*
   * $B%5!<%PB&$G(B"5678"$B$H$$$&L>A0$r$D$1$i$l$?6&M-%a%b%j%;%0%a%s%H$rF@$k(B.
   */
  key = 5678;

  /*
   * $B6&M-%a%b%j%;%0%a%s%H$K%"%/%;%9$9$k(B
   */
  if ((shmid = shmget(key, SHMSZ, 0666)) < 0) {
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
   * $B%5!<%P$,=q$-9~$s$@%G!<%?$rFI$_9~$`(B
   */
  for (s = shm; *s != 0; s++)
    putchar(*s);
  putchar('\n');

  /*
   * $BFI$_=*$($?9g?^$H$7$F6&M-%a%b%j$K(B'*'$B$r=q$-9~$`!%(B
   */
  *shm = '*';

  /*
   * $B6&M-%a%b%j%;%0%a%s%H$r<h$j30$9(B
   */
  shmdt(shm);

  return 0;
}
