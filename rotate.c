/*Write a C program that writes, to standard output, the names of the 88 piano keys and their frequencies. 
The program needs to actually compute the values; you cannot hardcode them. 
Display all frequencies with four places after the decimal point. */

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>


char* rotate(char* s, signed int n) {
  signed int move;
  if (move < 0) {
    printf("%s\n", "Negative numbers not allowed");
  } else {
    if (n < strlen(s)) {
      move = n;
    } else {
      move = abs(strlen(s) - n);
    }


    char* output = "";
    output = malloc(sizeof(s));



    for (int i = 0; i < strlen(s); i++) {
      output[i] = s[i + move];
    }

    int j = 0;
    for (int i = strlen(s) - move; i < strlen(s); i++) {
        output[i] = s[j];
        j++;
    }
    return output;
  }
}


int main() {

}
