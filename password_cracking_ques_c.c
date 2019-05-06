#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <crypt.h>
#include <time.h>

/***********************************************************************
*******
The variable names and the function names of this program is same as provided by the university.
The added variable and function are the only changes made to this program.

  Compile with:
    cc -o password_cracking_ques_c password_cracking_ques_c.c -lcrypt 
    ./password_cracking_ques_c

************************************************************************
******/
int n_passwords = 1;
int time_difference(struct timespec *start, struct timespec *finish,
                    long long int *difference) {
  long long int ds =  finish->tv_sec - start->tv_sec; 
  long long int dn =  finish->tv_nsec - start->tv_nsec; 

  if(dn < 0 ) {
    ds--;
    dn +=            1000000000; 
  } 
  *difference = ds * 1000000000 + dn;
  
  return !(*difference > 0);
}


char *encrypted_passwords[] = {
//Note:Following is the hash value of ART23
"$6$KB$b16LWjJdi7ZpiFk7d3FObZJtaMgv2Ks3ST3FdvMWnrU.plsAfMeOSFLMs/yF1uQP1kJigHIDTsaPnZtujGKPA/",


};
void substr(char *dest, char *src, int start, int length){
  memcpy(dest, src + start, length);
  *(dest + length) = '\0';
}
void pwdcrack(char *salt_and_encrypted){
  int a,b,c,d;     // Loop counters
  char salt[7];    // String used in hashing the password. Need space

  char plain[7];   // The combination of letters currently being checked
  char *enc;       // Pointer to the encrypted password
  int count = 0;   // The number of combinations explored so far

  substr(salt, salt_and_encrypted, 0, 6);

  for(a='A'; a<='Z'; a++){  
	for(b='A'; b<='Z'; b++){
	    for(c='A'; c<='Z'; c++){
	      for(d=0; d<=99; d++){
		sprintf(plain, "%c%c%c%02d",a,b,c,d);
		enc = (char *) crypt(plain, salt);
		count++;
		if(strcmp(salt_and_encrypted, enc) == 0){
		  printf("#%-8d%s %s\n", count, plain, enc);
		} else {
		  printf(" %-8d%s %s\n", count, plain, enc);
		}
	      }
	    }
	  }
	}
  printf("%d solutions explored\n", count);
}

int main(int argc, char *argv[]){
  int i;
struct timespec start, finish;
  long long int difference;   
  int account = 0;
  clock_gettime(CLOCK_MONOTONIC, &start);

  for(i=0;i<n_passwords;i<i++) {
    pwdcrack(encrypted_passwords[i]);
  }

  clock_gettime(CLOCK_MONOTONIC, &finish);
  time_difference(&start, &finish, &difference);
 printf("Time elapsed was %lldns or %0.9lfs\n", difference, 
         (difference/1.0e9));
  return 0;
}
