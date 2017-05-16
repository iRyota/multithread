#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdio.h>
#include <unistd.h>


#define SHMSZ     27

int main()
{
  int shmid, semid;
  key_t shmkey, semkey;
  char *shm, *s;
  struct sembuf sbuf;

  /*
   * サーバ側で"5678"という名前をつけられた共有メモリセグメントを得る.
   */
  shmkey = 5678;

  /*
   * 共有メモリセグメントにアクセスする
   */
  if ((shmid = shmget(shmkey, SHMSZ, 0666)) < 0) {
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
   * セマフォに"1234"という名前をつける．
   */
  semkey = 1234;

  /*
   * セマフォを得る
   */
  if ((semid=semget(semkey, 1, 0666)) < 0){
    perror("semget");
    return 1;
  }

  /*
   * セマフォでロックする
   */
  sbuf.sem_num=0;
  sbuf.sem_op=-1;
  sbuf.sem_flg=0;
  if (semop(semid,&sbuf,1) < 0){
    perror("semop");
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
   * セマフォをアンロックする
   */
  sbuf.sem_num=0;
  sbuf.sem_op=1;
  sbuf.sem_flg=0;
  if (semop(semid,&sbuf,1) < 0){
    perror("semop");
    return 1;
  }

  /*
   * 共有メモリセグメントを取り外す
   */
  shmdt(shm);

  return 0;
}
