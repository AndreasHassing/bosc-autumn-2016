#include <stdio.h>

/* print Fahrenheit-Celsius table
 * for fahr = 0, 20, ..., 300
 * exercise 1-3: add a header to the table
 * exercise 1-4: print the corresponding
 *               Celsius-Fahrenheit table
 */
int main() {
  float fahr, celsius;
  float lower, upper, step;

  lower = -20;   // lower limit of temperature scale
  upper = 200; // upper limit
  step = 10;   // step size

  printf("%10s  %s\n", "Fahrenheit", "Celsius");

  celsius = lower;
  while (celsius <= upper) {
    fahr = celsius / (5.0/9.0) + 32.0;
    printf("%10.1f  %.1f\n", celsius, fahr);
    celsius = celsius + step;
  }

  return 0;
}
