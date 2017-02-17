/******************************************************************************
 * Laborator 04 Krivky - Zaklady pocitacove grafiky - IZG
 * ihulik@fit.vutbr.cz
 *
 * 
 * Popis: Hlavicky funkci pro funkce studentu
 *
 * Opravy a modifikace:
 * - isvoboda@fit.vutbr.cz
 *
 */

#include "student.h"
#include "globals.h"

#include <time.h>

//Viz hlavicka vector.h
USE_VECTOR_OF(Point2d, point2d_vec)
#define point2d_vecGet(pVec, i) (*point2d_vecGetPtr((pVec), (i)))

/* Secte dva body Point2d a vysledek vrati v result (musi byt inicializovan a alokovan)*/
void	addPoint2d(const Point2d *a, const Point2d *b, Point2d *result)
{
	IZG_ASSERT(result);
	result->x = a->x+b->x;
	result->y = a->y+b->y;
}

/* Vynasobi bod Point2d skalarni hodnotou typu double a vysledek vrati v result (musi byt inicializovan a alokovan)*/
void	mullPoint2d(double val, const Point2d *p, Point2d *result)
{
	IZG_ASSERT(result);
	result->x = p->x*val;
	result->y = p->y*val;
}

/* Mocnina */
double	Power(double a, int b)
{
  double result = 1.0;
  for (int i = 0; i < b; ++i)
  {
  	result *= a;
  }
  return result;
}

/* Vypocet faktorialu */
double	Factorial(int n)
{
  if (n <= 0)
  {
    return 1.0;
  }
  else
  {
    return n * Factorial(n - 1);
  }
}

/* Vypocet kombinacniho cisla 'n' nad 'k' */
double	BinomialCoefficient(int n, int k)
{
  if (n < k)
  {
      return 0.0;
  }
  else
  {
      return Factorial(n) / (Factorial(k) * Factorial(n - k));
  }
}

/* Vypocet Bernsteinova polynomu */
double	BernsteinPolynom(int n, int i, double t)
{
  return BinomialCoefficient(n, i) * Power(t, i) * Power(1.0-t, n-i);
}

//------------------------Zde dopiste Vas kod-------------------------

// quality: pocet bodu na krivce - 1
// const S_Vector* points: ridici body
// S_Vector* line_points: zde ukladate vysledek - vypoctene body lezici na krivce

// Vypocet Bezierovy krivky pomoci Bernsteinovych polynomu
void    bezierBernsteinPolynom(int quality, const S_Vector *points, S_Vector *line_points)
{
  // Toto musi byt na zacatku funkce, nemazat.
  point2d_vecClean(line_points);

}

/* Implementace algoritmu de Casteljau*/
void	bezierDeCasteljau(int quality, const S_Vector *points, S_Vector *line_points)
{
  // Toto musi byt na zacatku funkce, nemazat.
  point2d_vecClean(line_points);

  int n = point2d_vecSize(points) - 1;
  double t = 0.0;
  double t_step = 1.0 / quality;
  S_Vector * vector = point2d_vecCreateEmpty();
  Point2d mul_tmp1, mul_tmp2, add_tmp;

  if(n < 1)
	return;


	for(int q = 0; q <= quality; ++q, t+=t_step)
	{
	//Copy of input vector in to local vector of points
		vector = vecDeepCopy(points);
	

		for(int j = 0; j < n; ++j)
			for(int i = 0; i < n-j; ++i)
			{
				//Výpoèet bodu i v úrovni j
				mullPoint2d((1-t), point2d_vecGetPtr(vector, i), &mul_tmp1);
				mullPoint2d(t, point2d_vecGetPtr(vector, i+1), &mul_tmp2);
				addPoint2d(&mul_tmp1, &mul_tmp2, &add_tmp);
				point2d_vecSet(vector, i, add_tmp);
			}

			point2d_vecPushBack(line_points, add_tmp);
	}

}