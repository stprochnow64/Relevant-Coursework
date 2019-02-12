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
