#pragma once
/**
 * We are not testing values outside of one million seconds.
 * At one million second, and double precision, the smallest
 * resolvable number that can be added to one million and return
 * a new value one million + epsilon is 5.82077e-11.
 *
 * This was calculated by searching iteratively for epsilon
 * around 1,000,000, with epsilon starting from 1 and halved
 * at every iteration, until epsilon when added to 1,000,000
 * resulted in 1,000,000.
 */
double DBL_EPSILON = 5.82077e-11;
