/**
 * Project No.2 - Iteracne vypocty
 * Author : Jozef Urbanovsky, xurban66@stud.fit.vutbr.cz
 * Group : 39 (1BIB)
 * Assistant : Ing. Gabriela Necasova
 * Date : 18/11/2015
 * File name : proj2.c
 * Note : Binary search implemented (Accuracy +/-1)
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_EPS 1e-13
#define WRONG_ARG 5
#define WRONG_NUM 10
#define WRONG_EPS 15
#define WRONG_INT 20

/**
 * Function prototypes
 */

double taylor_log(double x, unsigned int n);
double cfrac_log(double x, unsigned int n);
void log_output(double x, unsigned int n);
void iter_output(double min, double max, double eps);
void iter_binary(double min, double max, double eps);
void error_log(int error);
void help();

/**
 * Function main
 */

int main(int argc, char *argv[])
{
	double number = 0.0;
	unsigned int iterations = 0;

	double min = 0.0;
	double max = 0.0;
	double eps = 0.0;

	if (strcmp(argv[1], "--help")) // if the 1st argument is help, calls function help
	{
		if (argc == 4 || argc == 5)
		{
			if (argc == 4 && !strcmp(argv[1], "--log")) // if there are 4 arguments and 2nd argument is --log,
			{				            // stores values and uses them for calculations
				number = strtod(argv[2], NULL);
				iterations = strtoul(argv[3], NULL, 10);
				if (iterations == UINT_MAX) // if negative number is given, function STRTOUL transforms it to UINT_MAX
				{
					error_log(WRONG_EPS);
					return EXIT_FAILURE;
				}

				log_output(number,iterations);

			}
			else if (argc == 5 && !strcmp(argv[1], "--iter"))
			{
				min = strtod(argv[2], NULL);
				max = strtod(argv[3], NULL);
				eps = strtod(argv[4], NULL);

				if (min > max) // if minimum is higher than maximum therefore no interval exists
				{
					error_log(WRONG_INT);
					return EXIT_FAILURE;
				}
				else if (eps < MAX_EPS) // if desired eps is higher than MAX_EPS termiantes program
				{
					error_log(WRONG_EPS);
					return EXIT_FAILURE;
				}
				else
				{
					//iter_output(min,max,eps); // use this function for output if binary wont me accurate enough
					iter_binary(min,max,eps);
				}
			}
			else
			{
				error_log(WRONG_ARG);
				return EXIT_FAILURE;
			}
		}
		else
		{
			error_log(WRONG_ARG);
			return EXIT_FAILURE;
		}
	}
	else
	{
		help();
		return EXIT_SUCCESS;
	}

	return EXIT_SUCCESS;
}

/**
 * Function : error_log
 * Checks given error number and depending on it prints error log
 */

void error_log(int error)
{
	switch (error)
	{
		case WRONG_ARG : fprintf(stderr,"Invalid arguments, type --help for info\n");
			break;
		case WRONG_NUM : fprintf(stderr,"Invalid input numbers, only positive\n");
			break;
		case WRONG_EPS : fprintf(stderr,"Invalid EPS, span (1e-12, UINT_MAX)\n");
			break;
		case WRONG_INT : fprintf(stderr,"Invalid interval, min has to be lower than max\n");
			break;

	}
	return;
}

/**
 * Function : help
 * Prints help
 */

void help()
{
		printf("Program works as approximation of natural logarithm\n"
		"implemented through Taylor series and Continued fraction\n\n"
		"--log NUMBER ITERATIONS\n"
		"--iter MIN MAX EPS\n");
		return;
}

/**
 * Function : my_abs
 * Checks if number is negative, if so returns positive value
 * @param num = number with positive or negative value
 * @return num or -num
 */

double my_abs(double num)
{
	return num < 0 ? -num : num; // ternary operator for simple condition
}
/**
 * Function : taylor_log
 * Taylor series for natural logarithm are centered at 1,
 * so 2 implementation is chosen if number is higher or lower
 * than 1, to make calculation more precise.
 *
 *  0 < x < 2
 *                 x^2   x^3   x^4          n   (x-1)^n
 *  log(1-x)= -x - ___ - ___ - ___ - ... or E = _____
 *                  2     3     4           1     n
 *
 *  x > 1/2
 *
 *         (x-1) +   1     x-1          1     x-1             n   ((x-1) / x)^n
 *  log(x)=_____ + ( _ * ( ___ )^2) + ( _ * ( ___ )^3) ... or E = ______________
 *           x       2      x           3      x              1         n
 *
 * @param x - number to be converted to log
 * @param n - number of iterations
 * @return result after n iterations
 */

double taylor_log(double x, unsigned int n)
{
	double result = 0.0;
	double tmp = 1.0;

	if (x == 0)
		return -INFINITY;

	else if (x < 0)
		return NAN;

	else if (isinf(x))
		return INFINITY;

	else if (isnan(x))
		return NAN;

	else if (x > 1)
	{
		x = (x-1) / x;
		for (unsigned int i = 1; i <= n; i++)
		{
			tmp *= x;
			result += tmp / i ;
		}
	}
	else
	{
		x = 1 - x; // for easier calculation and implementation
		for (unsigned int i = 1; i <=n; i++)
		{
			tmp *= x;
			result -= tmp / i;
		}
	}

	return result;
}

/**
 * Function : crafc_log
 * Input is customized 	to a required form, to make whole calculation easier
 * Function uses auxiliary function my_power
 *
 *     (1+z)              2z
 * log _____ = _______________________ n
 *     (1-z)    1+ Nn(-k^2 * z^2, 2k+1)
 *                                     1
 *
 * @param x - number to be converted to log
 * @param n - number of iterations
 * @return result after n iterations
 */

double cfrac_log(double x, unsigned int n)
{
					// 1+z                                 y-1                                1+  (z-1/1+z)   2z/1+z  2z(1+z)
					// ___ = y, 1+z=y(1-z), z+y*z=y-1, z = ___, back to start - substitution, _____________ = _____ = _______ = z
					// 1-z                                 1+y                                1-  (z-1/1+z)   2/1+z   2(1+z)

	if (x == 0)
		return -INFINITY;

	else if (x < 0)
		return NAN;

	else if (isinf(x))
		return INFINITY;

	else if (isnan(x))
		return NAN;

	else if (n == 0)
		return 0;

	x = (x-1)/(x+1);

	double tmp_den = ((n*2)-1);
	double tmp_frac = ((n-1)*(n-1)*(x*x)) / tmp_den;

		for (unsigned int i = (n-1); i > 1; i--)
			tmp_frac = ((i-1)*x)*(x*(i-1)) / ((i*2)-1-(tmp_frac));

	return (2*x)/(1-tmp_frac); // predchazdajuca bola o 1 iteraciu na pred, pretoze uz prvy denom bol 2*2-1

	/*x = (x-1)/(x+1);
	for (unsigned int i = n; i > 0; i--) // going from last to first iteration
	{
		tmp = -(x*x*i*i);
		result = tmp / ((2*i+1) + result);
	}

  return 2*x/(1+result); // 2 * specified x divided by continued fraction result + 1 (because of first iteration as denominator)*/
}


/**
 * Function : log_output
 * Required format of output data written to the function
 * Every printf calculates for number x and n iterations value of each implementation
 * @param x - number to be converted to log
 * @param n - number of iterations
 */

void log_output(double x, unsigned int n)
{
	printf("       log(%g) = %.12g\n",x, log(x));
	printf("    cf_log(%g) = %.12g\n",x, cfrac_log(x,n));
	printf("taylor_log(%g) = %.12g\n",x, taylor_log(x,n));
	return;
}

/**
 * Function : iter_output
 * Calculated difference between given number and real log value,
 * increments counter each time, difference is higher than eps, else
 * terminates calculation and choses higher counter from both calculations,
 * then proceeds to output part. Required format of output data is printed to screen
 * Function taylor needs 1 more iterations, because it does not count first iteration.
 * @param min - minimum number from given interval
 * @param max - maximum number from given interval
 * @param eps - precision of calculation
 */

void iter_output(double min, double max, double eps)
{
	unsigned int cf_min = 1;
	unsigned int cf_max = 1	;
	unsigned int cf_iter = 1;
	unsigned int taylor_min = 1;
	unsigned int taylor_max = 1;
	unsigned int taylor_iter = 1;

	printf("                      log(%g) = %.12g\n", min, log(min));
	printf("                      log(%g) = %.12g\n", max, log(max));

	while (my_abs(cfrac_log(min,cf_min) - log(min)) > eps) // if continued fraction of min number on cf_min iterations - real value of log of min is lower
		cf_min++;

	while(my_abs(cfrac_log(max,cf_max) - log(max)) > eps)
		cf_max++;

	while (my_abs(taylor_log(min,taylor_min) - log(min)) > eps)
		taylor_min++;


	while(my_abs(taylor_log(max,taylor_max) - log(max)) >eps)
		taylor_max++;


	cf_iter = cf_max > cf_min ? cf_max : cf_min;

	printf("continued fraction iterations = %d\n", cf_iter);
	printf("                   cf_log(%g) = %.12g\n", min, cfrac_log(min, cf_iter));
	printf("                   cf_log(%g) = %.12g\n", max, cfrac_log(max, cf_iter));

	taylor_iter = taylor_max > taylor_min ? taylor_max : taylor_min;

	printf(" taylor polynomial iterations = %d\n", taylor_iter);
	printf("               taylor_log(%g) = %.12g\n", min, taylor_log(min, taylor_iter));
	printf("               taylor_log(%g) = %.12g\n", max, taylor_log(max, taylor_iter));

	return;
}

/**
 * Function : iter_binary
 * At start finds median, every iteration that is not successful at finding number of iterations that is lesser than
 * difference between math log and taylor/cf log divides interval to 2 same intervals and continues same process.
 * @param min - minimum number from given interval
 * @param max - maximum number from given interval
 * @param eps - precision of calculation
 */

void iter_binary(double min, double max, double eps)
{
	int n = 10000; // first assumption
	int imax = n-1;
	int imin = 0;
	int imid;
	double logarithm = log(min); // for faster calculation

	printf("                      log(%g) = %.12g\n", min, log(min));
	printf("                      log(%g) = %.12g\n", max, log(max));

	while (imin <= imax)
	{
		imid = (imin + imax)/2; // if imin is lower than imax finds median of each iteration

        if (fabs(cfrac_log(min, imid)-logarithm) > eps)
          imin = imid+1; // shoves it one iteration to the right is eps is lower

			else if (fabs(cfrac_log(min, imid)-logarithm) < eps)
				imax = imid-1; // shoves it one iteration to the left if eps is higher

			else
				break;
	}
	printf("continued fraction iterations = %d\n", imid+1);
	printf("                   cf_log(%g) = %.12g\n", min, cfrac_log(min, imid+1));
	printf("                   cf_log(%g) = %.12g\n", max, cfrac_log(max, imid+1));

	n = 1000000;
	imax = n-1;
	imin = 0;

	while (imin < imax)
	{
		imid = (imin + imax)/2;

        if (fabs(taylor_log(min, imid)-logarithm) > eps)
          imin = imid+1;

			else if (fabs(taylor_log(min, imid)-logarithm) < eps)
				imax = imid-1;

			else
				break;
	}
	printf(" taylor polynomial iterations = %d\n", imid+1);
	printf("               taylor_log(%g) = %.12g\n", min, taylor_log(min, imid+1));
	printf("               taylor_log(%g) = %.12g\n", max, taylor_log(max, imid+1));
}
