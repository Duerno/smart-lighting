/* Code to convert voltage to resistance for the current circuit */

#include <stdio.h>

int main () {
  double x, y, dy;
  double r = 220.0, vin = 5.1;
  while(scanf("%lf%lf%lf\n", &x,&y,&dy) == 3)
    printf("%lf\t%lf\t%lf\n", x, r*vin/(vin - y), dy);
  return 0;
}
