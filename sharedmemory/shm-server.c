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
   * 共有メモリセグメントに"5678"という名前をつける．
   */
  key = 5678;

  /*
   * 共有メモリセグメントを作成する．
   */
  if ((shmid = shmget(key, SHMSZ, IPC_CREAT | 0666)) < 0) {
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
   * 共有メモリに他のプロセスが読むようになんらかのデータを書き込む
   */
  s = shm;
  for (c = 'a'; c <= 'z'; c++)
    *s++ = c;
  *s = 0;

  /*
   * 他のプロセスが共有メモリに読み込み終了の合図として'*'に書き込む
   */
  while (*shm != '*')
    sleep(1);

  /*
   * 共有メモリセグメントを取り外す
   */
  shmdt(shm);

  return 0;
}

