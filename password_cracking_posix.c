/**
run with
gcc -o password_cracking_posix password_cracking_posix.c -lcrypt -pthread
./password_cracking_posix > PosixThreadresult.txt
**/
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <crypt.h>
#include <pthread.h>

int n_passwords = 4;
pthread_t thread_1, thread_2;  //thread variables 
//arrayy of hashed passowrd with salt
char *encrypted_passwords[] = {   
  "$6$KB$6SsUGf4Cq7/Oooym9WWQN3VKeo2lynKV9gXVyEG4HvYy1UFRx.XAye89TLp/OTcW7cGpf9UlU0F.cK/S9CfZn1",
  "$6$KB$Lsc5GJc6UWLEyVocbQBpezWOUtiLMkAO6MZ9vJY15o7P5jBXl4w7zxwolBkw5Z4jftUo.6aLeQ6gMUBrADh2q0",
  "$6$KB$GIGRUyhlF2hmevkShnr0ARkLpTiJx4UTiYuAdkuonWARwEKbpqltj/2bEhWD8x87V3QYhImocd//y03phj9m/0",
  "$6$KB$rUk1AWWJnutCqD6id7MtupeHvck78SduFKGelRIOCQ/9gBHAac3p02iBnbDzOZiraZQZFl31aeQiiPgd.Ko1O1"
};

void substr(char *dest, char *src, int start, int length){
  memcpy(dest, src + start, length);
  *(dest + length) = '\0';
}
//kernal function 1 that iterates overs password that starts from A to M
void *kernel_function_1(void *salt_and_encrypted){
  int i1, i2, i3;     // Loop counters
  char salt[7];    // String used in hashing the password. Need space
  char plain[7];   // The combination of letters currently being checked
  char *enc;       // Pointer to the encrypted password
  int count = 0;   // The number of combinations explored so far

  substr(salt, salt_and_encrypted, 0, 6);

  for(i1='A'; i1<='M'; i1++){   //iiterates overs password that starts from A to M
    for(i2='A'; i2<='Z'; i2++){
      for(i3=0; i3<=99; i3++){
        sprintf(plain, "%c%c%02d", i1, i2, i3); 
        enc = (char *) crypt(plain, salt);
        count++;
        if(strcmp(salt_and_encrypted, enc) == 0){
          printf("#%-8d%s %s\n", count, plain, enc);
        } else {
          //printf(" %-8d%s %s\n", count, plain, enc);
        }
      }
    }
  }
  printf("%d solutions explored\n", count);
}

//kernal function 2 that iterates overs password that starts from N to Z
void *kernel_function_2(void *salt_and_encrypted){
int i1, i2, i3;       // Loop counters
  char salt[7];    // String used in hashing the password. Need space
  char plain[7];   // The combination of letters currently being checked
  char *enc;       // Pointer to the encrypted password
  int count = 0;   // The number of combinations explored so far

  substr(salt, salt_and_encrypted, 0, 6);
  for(i1='N'; i1<='Z'; i1++){  //iterates overs password that starts from N to Z
   for(i2='A'; i2<='Z'; i2++){
     for(i3=0; i3<=99; i3++){
      sprintf(plain, "%c%c%02d", i1, i2, i3);
      enc = (char *) crypt(plain, salt);
      count++;
      if(strcmp(salt_and_encrypted, enc) == 0){
        printf("#%-8d%s %s\n", count, plain, enc);
      } else {
       // printf(" %-8d%s %s\n", count, plain, enc);
      }
    }
  }
}
printf("%d solutions explored\n", count);

}
///time difference code
int time_difference(struct timespec *start, 
  struct timespec *finish, 
  long long int *difference) {
  long long int ds =  finish->tv_sec - start->tv_sec; 
  long long int dn =  finish->tv_nsec - start->tv_nsec; 
  if(dn < 0 ) {
    ds--;
    dn += 1000000000; 
  } 
  *difference = ds * 1000000000 + dn;
  return !(*difference > 0);
}
int main(int argc, char *argv[]){
  int i;
  struct  timespec start, finish;
  long long int time_elapsed;
  clock_gettime(CLOCK_MONOTONIC, &start);

   //pthread create and pass kernal function to each thread 
  for(i=0;i<n_passwords;i<i++) {
    pthread_create(&thread_1, NULL, kernel_function_1, encrypted_passwords[i]);
    pthread_create(&thread_2, NULL, kernel_function_2, encrypted_passwords[i]);
    pthread_join(thread_1, NULL);
    pthread_join(thread_2, NULL);   
  }

  clock_gettime(CLOCK_MONOTONIC, &finish);
  time_difference(&start, &finish, &time_elapsed);
  printf("Time elapsed was %lldns or %0.9lfs\n", time_elapsed,
   (time_elapsed/1.0e9)); 

  return 0;
}

