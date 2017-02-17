/**
 * Project No.1 - práca s textom
 * Author : Jozef Urbanovský, xurban66@stud.fit.vutbr.cz
 * Group : 39 (1BIB)
 * Assistant : Ing. Gabriela Nečasová
 * Date : 26/10/2015
 * File name : proj1.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <ctype.h>

#define MAX_INPUT 101

/**
 *Function prototypes
 */
int		read(char *s);
int		numberCond(char *s);
int		removeZero(char *s, int len);
int		printCond(char *s);
int		prime(char *s);
int		palindrome(char *s, int len);
const char * 	date(char *s, int len);

/**
 *Function main
 */

int main(int argc, char *argv[])
{
	char		buffer[MAX_INPUT];
	char		length = 0;
	int 		zeroCounter = 0;
	const char * 	weekday = NULL;

	(void)argv;

	if (argc != 1)
	{
		printf("Program detects if input text is a number, prime number, palindrome or a date.\n"
					 "Prime number span = 1 to INT_MAX (2147483647).\n"
					 "Palindrome works on words and is case-sensivite.\n"
					 "Date works with format YYYY-MM-DD.\n");
		exit(1);
	}

	while ((length = read(buffer)) > 0)
	{
		if (printCond(buffer))
		{
		if ((weekday = date(buffer, length)) != NULL)
			printf("date: %s %s\n", weekday, buffer);
		else if (numberCond(buffer))
		{
			zeroCounter = removeZero(buffer, length);
			if (prime(buffer))
				printf("number: %s (prime)\n", buffer + zeroCounter);
			else
				printf("number: %s\n", buffer + zeroCounter);

		}

		else if (palindrome(buffer, length))
			printf("word: %s (palindrome)\n", buffer);
		 else
			printf("word: %s\n", buffer);
		}
	}
	if (length == -1)
		fprintf(stderr, "Error : Input overflow\n");
	exit(1);
}


/**
 * Function: Read
 * Loads stdin as whole, cuts it, if array of char is
 * longer than 100 bytes. Also counts length of the given
 * input.
 * @param *s = pointer to stdin.
 * @return 0 - failed to read, [1,100] - length of stdin.
*/

int read(char *s)
{
	int len=0;

	if (scanf("%100s", s) != EOF )
	{
	    while (s[len] != '\0')
			len++;

   	 if (len < 100) // program terminates if input is longer than 100 chars
		return len;
    	else
		return EOF;
	}
 	 return 0;
}

/**
 * Function: RemoveZero
 * Checks if number begins with 0, if so function counts number
 * of zeroes found before different number. If string consists of
 * 0s there is special condition.
 * @param *s = pointer to stdin.
 * @return zeroCounter = number of zeroes.
*/

int removeZero(char *s, int len)
{
	int zeroCounter = 0;

	if (s[len-1] == '0' && len == 1)
		return len-1;

	while (*s == '0')
	{
		s++; // increments counter inside of buffer
		zeroCounter++; // counts every single zero found
	}

	if (zeroCounter-len == 0)
		return len-1;
	else return zeroCounter;
}

/**
 * Function: NumberCond
 * While string is different from null terminator, function
 * checks if every single character is digit.
 * @param *s = pointer to stdin.
 * @return 0 - false, 1 - true = string is printable
 */

int numberCond(char *s)
{
  while (*s != '\0')
	{
	if (!isdigit(*s++)) // if any char given is not digit then terminates function
		 return 0;
	}
  return 1;
}

/**
 * Function: PrintCond
 * While string is different from null terminator, function
 * checks if every single character is printable.
 * @param *s = pointer to stdin.
 * @return 0 - false, 1 - true = string is printable
 */

int printCond(char *s)
{
  while (*s != '\0')
	{
		if (!isprint(*s++)) // if any char given is not printable then terminates function
		{
		fprintf(stderr,"Error: Not printable character\n");
		return 0;
		}
	}
  return 1;
}

/**
 * Function: Prime
 * Converts given string to long integer and
 * checks if number has only 1 divisor.
 * @param *s = pointer to stdin.
 * @return 0 - false, 1 - true = prime
*/

int prime(char *s)
{

	unsigned long num = strtoul(s, NULL, 10);
	unsigned long i = 0;

	if (num > INT_MAX || num <= 1)
	{
   	fprintf(stderr,"Error: Invalid number for function prime (num > INT_MAX || num < 1\n)");
	return 0;
	}
	for (i = 2; i < num/2+1; i++)
	{
	if (num % i == 0) // if divisor for num was found function ends
		return 0;

	}
	return 1;
}

/**
 * Function: Palindrome
 * Checks if given word is the same from forward or
 * backward. Loop compares if character sequence
 * left-to-right is equal as right-to-left.
 * @param *s = pointer to stdin.
 * @param len = length of stdin.
 * @return 0 - false, 1 - true = palindrome
 */

int palindrome(char *s, int len)
{
	int normal = 0;
  	int reverse = len - 1; // without '\0'

  while (reverse > normal)
  {
	if (s[normal++] != s[reverse--]) // left-to-right(inc) is equal to right-to-left(dec)
		return 0;
	}
	return 1;
}

/**
 * Function: Date
 * Checks if length of given string is exactly 10, and if
 * there can be found '-' at 5th and 8th place. Then parse
 * string separately to year, month and day. Mktime
 * structured is filled and compared.
 * @param *s = pointer to stdin.
 * @param len = length of stdin.
 * @return weekday = string literal containing shortcut of day.
*/

const char * date(char *s, int len)
{
  time_t 	curTime;
  struct tm * 	timeInfo;
  int 		year, month ,day; // YYYY-DD-MM
  const char * 	weekday[] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"}; // declared array of string literals (Monday is first because of US dates)

  if (len != 10 || s[4] != '-' || s[7] != '-') // date length is 10 with '-' and they have to be found on 5th a 8th place in order for date to be in valid format
	return NULL;

	sscanf(s, "%d-%d-%d;", &year, &month, &day);

	if (month > 12 || month < 1)
		return NULL;

	if (day > 31 || day < 1)
		return NULL;

	time (&curTime);
	timeInfo = localtime (&curTime);

	timeInfo->tm_year = year - 1900; // fills time structure with year - 1900 (format of structure)
	timeInfo->tm_mon = month - 1; // month -1 is cause by indexing in structure (0-11)
	timeInfo->tm_mday = day;

	if (mktime(timeInfo) == -1) // if there is problem with mktime function terminates program
        {
  	fprintf(stderr, "Error: Mktime\n");
		exit(1);
	}
  return weekday[timeInfo->tm_wday]; // returns string literal containing day of week
}

