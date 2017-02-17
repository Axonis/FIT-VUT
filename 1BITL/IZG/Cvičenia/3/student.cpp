/******************************************************************************
 * Laborator 03 - Zaklady pocitacove grafiky - IZG
 * ibehun@fit.vutbr.cz
 *
 * $Id:$

 * Popis:
 *    soubor obsahuje definice barevnych datovych typu + pomocne funkce pro
 *    praci s nimi 
 *
 * Opravy a modifikace:
 * -
 */


#include "base.h"
#include "student.h"
#include "globals.h"
#ifdef __linux__
#include <limits.h>
#endif
#include <vector>
#include <time.h>
#include <stdio.h>

/******************************************************************************
 ******************************************************************************
 Funkce vraci pixel z pozice x, y. Pozor: nehlida velkost frame_bufferu, pokud 
 je dana souradnice mimo hranice. */
S_RGBA getPixel(int x, int y)
{
		return frame_buffer[y * width + x];
}

/******************************************************************************
 ******************************************************************************
 Funkce vlozi pixel na pozici x, y. Pozor: nehlida velkost frame_bufferu, pokud 
 je dana souradnice mimo hranice. */
void putPixel(int x, int y, S_RGBA color)
{
		frame_buffer[y * width + x] = color;
}

/******************************************************************************
******************************************************************************
Funkce pro vykresleni usecky se suradnicemi [x1,y1] [x2,y2] */
void drawLine(int x1, int y1, int x2, int y2, S_RGBA col)
{

	bool steep = abs(y2 - y1) > abs(x2 - x1);
	if (steep)
	{
		SWAP(x1, y1);
		SWAP(x2, y2);
	}
	if (x1 > x2)
	{
		SWAP(x1, x2);
		SWAP(y1, y2);
	}

	const int dx = x2 - x1, dy = abs(y2 - y1);
	const int P1 = 2 * dy, P2 = P1 - 2 * dx;
	int P = 2 * dy - dx;
	int y = y1;
	int ystep = 1;
	if (y1 > y2) ystep = -1;

	for (int x = x1; x <= x2; x++)
	{
		if (steep) {
			if (y >= 0 && y < width && x >= 0 && x < height) {
				putPixel(y, x, col);
			}
		}
		else {
			if (x >= 0 && x < width && y >= 0 && y < height) {
				putPixel(x, y, col);
			}
		}
		if (P >= 0)
		{
			P += P2;
			y += ystep;
		}
		else
			P += P1;
	}


	return;
}


/************************** Ukoly na cviceni *********************************/

/******************************************************************************
 ******************************************************************************/
void pinedaTriangle(const S_Point & v1, const S_Point & v2, const S_Point & v3, const S_RGBA & color1, const S_RGBA & color2)
{
	int      minx, miny, maxx, maxy;
    int      e1_minx, e2_minx, e3_minx;
	int		 a1, a2, a3, b1, b2, b3, c1, c2, c3;
	int		 x, y, e1, e2, e3;
	/* nalezeni obalky (minx, maxx, miny, maxy) trojuhleniku */

	minx = MIN(v1.x, MIN(v2.x, v3.x)); 
	maxx = MAX(v1.x, MAX(v2.x, v3.x)); 

    miny = MIN(v1.y, MIN(v2.y, v3.y)); 
	maxy = MAX(v1.y, MAX(v2.y, v3.y)); 


	/* oriznuti obalky (minx, maxx, miny, maxy) trojuhleniku podle rozmeru okna */

	minx = MAX(minx, 0);
	maxx = MIN (maxx, width -1);

	miny = MAX(miny, 0);
	maxy = MIN (maxy, height -1);
	

	/* Spocitani parametru hranove funkce pro kazdou hranu
	hranova fce je obecna rovnice primky Ax + By + C = 0
	primku prochazejici body (x1, y1) a (x2, y2) urcime jako
	(y1 - y2)x + (x2 - x1)y + x1y2 - x2y1 = 0 */

	a1 = v1.y - v2.y;
	a2 = v2.y - v3.y;
	a3 = v3.y - v1.y;

	b1 = v2.x -v1.x;
	b2 = v3.x - v2.x;
	b3 = v1.x - v3.x;

	c1 = v1.x * v2.y - v2.x * v1.y;
	c2 = v2.x * v3.y - v3.x * v2.y;
	c3 = v3.x * v1.y - v1.x * v3.y;


	/* Vyplnovani: Cyklus pres vsechny body (x,y) v obdelniku (minx,miny), (maxx,maxy)
	   Pro aktualizaci hodnot hranove funkce v bode P (x +/- 1, y) a P (x, y +/- 1) vyuzijte hodnoty hranove funkce E (x, y) z bodu P (x, y) */
	
	e1_minx = a1 * minx + b1 * miny + c1;
	e2_minx = a2 * minx + b2 * miny + c2;
	e3_minx = a3 * minx + b3 * miny + c3;

	for (y = miny; y <= maxy; ++y)
	{
		e1 = e1_minx; 
		e2 = e2_minx; 
		e3 = e3_minx;

		for (x = minx; x <= maxx; ++x)
		{
			if (e1 >= 0 && e2 >= 0 && e3 >= 0)
				putPixel(x, y, color1);

			e1 += a1;
			e2 += a2;
			e3 += a3;
		}
		e1_minx += b1; e2_minx+= b2; e3_minx += b3;
	}


	/* prekrosleni hranic trojuholniku barvou color2*/
	drawLine(v1.x, v1.y, v2.x, v2.y, color2);
	drawLine(v2.x, v2.y, v3.x, v3.y, color2);
	drawLine(v3.x, v3.y, v1.x, v1.y, color2);

}


void pinedaPolygon(const S_Point * points, const int size, const S_RGBA & color1, const S_RGBA & color2)
{
	/************************************************************/
	/******************** Bodovany ukol *************************/
	/************************************************************/
	// ------ Pri praci muzete vyuzit pro vas predpripravene datove typy z base.h. Napriklad:  -----
	//
	// Pro ukladani parametru hranovych funkci muzete vyuzit prichystan zásobnik parametru hranovych funkci "EdgeParams":
	//
	// EdgeParams edge_params(size)                           --- vytvorite vektor (pole) "edge_params" parametu hranovych funkci o velikosti "size"


	EdgeParams edge_params(size);
	// edge_params[i].a, edge_params[i].b, edge_params[i].c   --- pristup k parametum (a,b,c) hranove funkce v poli "edge_params" na indexu "i"
	//
	// Pre ukladanie hodnot hranovych funkcii mozete vyuzit prichystany zasobnik hodnot hranovych funkcii "EdgeFncValues":
	//


	EdgeFncValues edge_vals(size); 
	EdgeFncValues edge_vals_c(size);// --- vytvorite vektor (pole) "edge_vals" hodnot hranovych funkcii o velikosti "size"
	// edge_vals[i]                                           --- pristup k hodnote hranove funkce v poli "edge_vals" na indexu "i"
	//
	
	int      minx, miny, maxx, maxy;
	int		 i,x,y;
	bool	 eval;

	/* nalezeni obalky (minx, maxx, miny, maxy) polygonu */
	maxx=minx=points[0].x;
	maxy=miny=points[0].y;

	for (i = 0; i < size; ++i)
	{
		minx = MIN(minx, points[i].x);
		miny = MIN(miny, points[i].y);
		maxx = MAX(maxx, points[i].x);
		maxy = MAX(maxy, points[i].y);
	}


	/* oriznuti obalky (minx, maxx, miny, maxy) polygonu podle rozmeru okna */

	minx = MAX(minx, 0);
	maxx = MIN (maxx, width -1);

	miny = MAX(miny, 0);
	maxy = MIN (maxy, height -1);


	/* Spocitani parametru hranove funkce pro kazdou hranu
	hranova fce je obecna rovnice primky Ax + By + C = 0
	primku prochazejici body (x1, y1) a (x2, y2) urcime jako
	(y1 - y2)x + (x2 - x1)y + x1y2 - x2y1 = 0 */

	for (i = 0; i < size-1; ++i)
	{
		if (i == size-1)
		{
			edge_params[size-1].a = points[size-1].y - points[0].y;
			edge_params[size-1].b = points[0].x - points[size-1].x;
			edge_params[size-1].c = points[size-1].x * points[0].y - points[0].x * points[size-1].y;
			edge_vals[size-1] = edge_params[size-1].a * minx + edge_params[size-1].b * miny + edge_params[size-1].c;
			break;
		}

		edge_params[i].a = points[i].y - points[i+1].y;
		edge_params[i].b = points[i+1].x - points[i].x;
		edge_params[i].c = points[i].x * points[i+1].y - points[i+1].x * points[i].y;
		edge_vals[i] = edge_params[i].a * minx + edge_params[i].b * miny + edge_params[i].c;

		
	}


	/* Test konvexnosti polygonu */

	//////// DOPLNTE KOD /////////


	/* Vyplnovani: Cyklus pres vsechny body (x,y) v obdelniku (minx,miny), (maxx,maxy)
	Pro aktualizaci hodnot hranove funkce v bode P (x +/- 1, y) a P (x, y +/- 1) vyuzijte hodnoty hranove funkce E (x, y) z bodu P (x, y) */

	//////// DOPLNTE KOD /////////

	for (y = miny; y <= maxy; ++y)
	{
		for (i = 0; i < size; ++i)
			edge_vals_c[i] = edge_vals[i]; 

		for (x = minx; x <= maxx; ++x)
		{
			
			eval = true;
			for (i = 0; i < size; ++i)
			{
				if (edge_vals_c[i] < 0)
				{
					eval = false;
					break;
				}
				else eval = true;
			}			

			if (eval)
					putPixel(x, y, color1);

			for (i = 0; i < size; ++i)
				edge_vals_c[i] += edge_params[i].a;
		}
		for (i = 0; i < size; ++i)
			edge_vals[i] += edge_params[i].b;
	}

	/*for (y = miny; y <= maxy; ++y)
	{
		e1 = e1_minx; 
		e2 = e2_minx; 
		e3 = e3_minx;

		for (x = minx; x <= maxx; ++x)
		{
			if (e1 >= 0 && e2 >= 0 && e3 >= 0)
				putPixel(x, y, color1);

			e1 += a1;
			e2 += a2;
			e3 += a3;
		}
		e1_minx += b1; e2_minx+= b2; e3_minx += b3;
	}*/





	/* prokresleni hranic polygonu barvou color2*/
	for (int i = 0; i < size; i++) {
		if (i == size - 1) {
			drawLine(points[i].x, points[i].y, points[0].x, points[0].y, color2);
		}
		else {
			drawLine(points[i].x, points[i].y, points[i + 1].x, points[i + 1].y, color2);
		}
	}
}

