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
   * サーバ側で"5678"という名前をつけられた共有メモリセグメントを得る.
   */
  key = 5678;

  /*
   * 共有メモリセグメントにアクセスする
   */
  if ((shmid = shmget(key, SHMSZ, 0666)) < 0) {
    perror("shmget");
    return 1;
  }

  /*
   * 共有メモリセグメントを本プログラムのデータスペースにアタッチする.
   */
  if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
    perror("shmat");
    return 1;
  }

  /*
   * サーバが書き込んだデータを読み込む
   */
  for (s = shm; *s != 0; s++)
    putchar(*s);
  putchar('\n');

  /*
   * 読み終えた合図として共有メモリに'*'を書き込む．
   */
  *shm = '*';

  /*
   * 共有メモリセグメントを取り外す
   */
  shmdt(shm);

  return 0;
}
