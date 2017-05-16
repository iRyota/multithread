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
   * 共有メモリセグメントに"5678"という名前をつける．
   */
  shmkey = 5678;

  /*
   * 共有メモリセグメントを作成する．
   */
  if ((shmid = shmget(shmkey, SHMSZ, IPC_CREAT | 0666)) < 0) {
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
   * セマフォを作成する．
   */
  if ((semid=semget(semkey, 1, IPC_CREAT | 0666)) < 0){
    perror("semget");
    return 1;
  }

  /*
   * セマフォを初期化する
   */
  if (semctl(semid, 0, SETVAL, 1) < 0){
    perror("semctl");
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
   * 共有メモリに他のプロセスが読むようになんらかのデータを書き込む
   */
  s = shm;
  for (c = 'a'; c <= 'z'; c++)
    *s++ = c;
  *s = 0;

  /*
   * キーボーから何らかの文字が入力されるまでロックされる
   */
  printf("press any keys to unlock.\n");
  getchar();

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
   * 他のプロセスが共有メモリに読み込み終了の合図として'*'に書き込む
   */
  while (*shm != '*')
    sleep(1);

  /*
   * 共有メモリセグメントを取り外す
   */
  shmdt(shm);


  /*
   * セマフォを削除する
   */
  if (semctl(semid, 0, IPC_RMID) < 0){
    perror("semctl");
    return 1;
  }

  return 0;
}

