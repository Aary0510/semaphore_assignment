#include<stdio.h>
#include<semaphore.h>
#include <thread>

sem_t empty_count, full_count;
