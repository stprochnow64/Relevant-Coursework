#include <stdio.h>
#include <math.h>


int main() {
  char* KEY_NAMES[] = {"A", "A#", "B", "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#"};
  const int ARRAY_SIZE = (sizeof(KEY_NAMES)/sizeof(KEY_NAMES[0]));
  const int NUM_OF_KEYS = 88;
  double INITIAL_FREQUENCY = 27.5;

  for (int i = 0; i < NUM_OF_KEYS; i++) {
    double exponentI = i;
    double exponentSize = ARRAY_SIZE;
    char* keyName = KEY_NAMES[i % ARRAY_SIZE];
    double exponent = (exponentI / exponentSize);

    double frequency = INITIAL_FREQUENCY * pow(2.0, exponent);
    printf("%s\t", keyName);
    printf("%10.4lf\n", frequency);
  }
  return 0;
}
