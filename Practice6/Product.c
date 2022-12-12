#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <fcntl.h>

void sem_change(int semaphore, int v, int index) {
	struct sembuf sem;
	sem.sem_num = index;
	sem.sem_op = v;
	sem.sem_flg = 0;
	semop(semaphore, &sem, 1);
}


int main() {
	key_t key;
	if ((key = ftok("key.txt", 'a')) == (key_t) - 1) {
	    printf("ftok error\n");
	    return 1;
	}
	int semaphore;
	if ((semaphore = semget(key, 0, 0)) == -1)
		if ((semaphore = semget(key, 4, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH)) == -1)
			return 1;
  	int i = 1;
	while (1) {
	    sem_change(semaphore, -1, 2);
	    sem_change(semaphore, -1, 3);
	    printf("Made: product %d\n", i);
	    i++;
  }
  return 0;
}
