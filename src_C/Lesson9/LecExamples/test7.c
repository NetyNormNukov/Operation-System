#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <sys/signal.h> 
 
/* Максимальное число сигналов, которые можно принять */ 
#define MAX_SENDED_SIGS 10 
 
/* Массив, в который записываются номера принятых сигналов */ 
int rec_sig[MAX_SENDED_SIGS]; 
/* Следующий свободный для записи элемент в массиве */ 
unsigned int nfree_elem = 0; 
 
/* Пишем обработчик сигнала как можно проще! 
 * 
 * В данном случае обработчик сигнала лишь заносит номер принятого  
 * сигнала в массив  
 */ 
void sighandler(int signo) { 
 rec_sig[nfree_elem++] = signo; 
} 
 
int main(int argc, char * argv[]) { 
 sigset_t mask; 
 static struct sigaction act;
 int i; 
 
 //memset(&act, 0, sizeof(act)); 
 sigemptyset(&mask); 
 
 /* Добавляем в маску те сигналы, которые мы будем  
 * отправлять/принимать 
 */ 
 sigaddset(&mask, SIGRTMIN); 
 sigaddset(&mask, SIGRTMIN + 1); 
 sigaddset(&mask, SIGUSR1); 
 
 /* Устанавливаем обработчик для наших сигналов. 
 * Добавляем наши сигналы в список блокируемых при вызове  
 * обработчика сигналов – во избежание гонок. 
 */ 
 act.sa_handler = sighandler; 
 act.sa_mask = mask; 
 sigaction(SIGRTMIN, &act, NULL); 
 sigaction(SIGRTMIN + 1, &act, NULL); 
 sigaction(SIGUSR1, &act, NULL); 
 
 /* Блокируем доставку наших сигналов, чтобы избежать их  
 * немедленной доставки 
 */ 
 sigprocmask(SIG_BLOCK, &mask, NULL); 
 
 /* Отправляем сигналы (не забываем о блокировке строчкой выше) */ 
 raise(SIGRTMIN); 
 raise(SIGRTMIN + 1); 
 raise(SIGRTMIN); 
 raise(SIGRTMIN + 1); 
 raise(SIGRTMIN); 
 raise(SIGRTMIN); 
 raise(SIGUSR1); 
 raise(SIGUSR1); 
 
 /* Разблокируем сигналы – все сигналы приходят одновременно */ 
 sigprocmask(SIG_UNBLOCK, &mask, NULL); 
 
 /* Выводим на экран список принятых сигналов в порядке их поступления */ 
 for(i = 0; i < nfree_elem; ++i) { 
  if (rec_sig[i] == SIGUSR1) { 
   printf("We get SIGUSR1\n"); 
  } else if (rec_sig[i] == SIGRTMIN) { 
   printf("We get SIGRTMIN\n"); 
  } else if (rec_sig[i] == SIGRTMIN + 1) { 
   printf("We get SIGRTMIN + 1\n"); 
  } 
 } 
 
 return 0; 
}