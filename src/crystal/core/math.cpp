#include <cmath>
#include "core/math.h"

// TODO: Refactor this file into separate files and classes

double lerp(double a, double b, double r) {
    return a * (1.0 - r) + (b * r);
}

double ease_in_sine(double x) {
    return 1.0 - cos((x * CRYSTAL_PI) / 2.0);
}

double ease_out_sine(double x) {
    return sin((x * CRYSTAL_PI) / 2.0);
}