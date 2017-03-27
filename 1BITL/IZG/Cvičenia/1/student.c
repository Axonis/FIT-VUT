/******************************************************************************
 * Laborator 01 - Zaklady pocitacove grafiky - IZG
 * ihulik@fit.vutbr.cz
 *
 * $Id: $
 * 
 * Popis: Hlavicky funkci pro funkce studentu
 *
 * Opravy a modifikace:
 * -
 */

#include "student.h"
#include "globals.h"

#include <time.h>

/******************************************************************************
 ******************************************************************************
 Funkce vraci pixel z pozice x, y. Je nutne hlidat frame_bufferu, pokud 
 je dana souradnice mimo hranice, funkce vraci barvu (0, 0, 0).
 Ukol za 0.5 bodu */
S_RGBA getPixel(int x, int y)
{
	if(x < width   && 
	   y < height  &&
	   x >= 0      &&
	   y >= 0)
	{
		return frame_buffer[y * width + x];

	}

	return makeBlackColor();
}

/******************************************************************************
 ******************************************************************************
 Funkce vlozi pixel na pozici x, y. Je nutne hlidat frame_bufferu, pokud ý
 je dana souradnice mimo hranice, funkce neprovadi zadnou zmenu.
 Ukol za 0.5 bodu */
void putPixel(int x, int y, S_RGBA color)
{
	if(x < width   && 
	   y < height  &&
	   x >= 0      &&
	   y >= 0)
	{
		frame_buffer[y * width + x] = color;

	}

}

/******************************************************************************
 ******************************************************************************
 Funkce prevadi obrazek na odstiny sedi. Vyuziva funkce GetPixel a PutPixel.
 Ukol za 1 bod. */
void grayScale()
{
	float intensity = 0.0;
	int x,y;

	for (y = 0; y < height; ++y)
		for (x = 0; x < width; ++x)
		{
			/* Nacteme soucasnou barvu */
			S_RGBA color = getPixel(x, y);

			intensity = 0.299*color.red + 0.587*color.green + 0.114*color.blue;
			color.green = color.blue = color.red = ROUND(intensity);

			putPixel(x, y, color);
		}
	
}

/******************************************************************************
 ******************************************************************************
 Funkce prevadi obrazek na cernobily pomoci algoritmu distribuce chyby.
 Ukol za 1 bod */
void errorDistribution()
{
	int y, x;
	int err = 0; 
	int sucet;

	/* Prevedeme obrazek na grayscale */
	grayScale();

	/* Projdeme vsechny pixely obrazku */
	for (y = 0; y < height; ++y)
		for (x = 0; x < width; ++x)
		{
			/* Nacteme soucasnou barvu */
			S_RGBA color = getPixel(x, y);

			/* Porovname hodnotu cervene barevne slozky s prahem.
			   Muzeme vyuzit jakoukoli slozku (R, G, B), protoze
			   obrazek je sedotonovy, takze R=G=B */
			if (color.red > 120){

				err = color.red - COLOR_WHITE.red;
				putPixel(x, y, COLOR_WHITE);
			}
			else {
				err = color.red;
				putPixel(x, y, COLOR_BLACK);
			}

			color = getPixel(x+1,y);

			sucet = color.red + ROUND(3.0/8.0 * err);
			if (sucet < 0) sucet = 0;
			else if(sucet > 255) sucet = 255;
			
			color.red = color.blue = color.green = sucet;

			putPixel(x+1, y, color); 




			color = getPixel(x,y+1);

			sucet = color.red + ROUND(3.0/8.0 * err);
			if (sucet < 0) sucet = 0;
			else if(sucet > 255) sucet = 255;
			
			color.red = color.blue = color.green = sucet;

			putPixel(x, y+1, color); 




			color = getPixel(x+1,y+1);

			sucet = color.red + ROUND(1.0/4.0 * err);
			if (sucet < 0) sucet = 0;
			else if(sucet > 255) sucet = 255;
			
			color.red = color.blue = color.green = sucet;

			putPixel(x+1, y+1, color); 
			
		}
}

/******************************************************************************
 ******************************************************************************
 Funkce prevadi obrazek na cernobily pomoci metody prahovani.
 Demonstracni funkce */
void thresholding(int Threshold)
{
	int y, x;

	/* Prevedeme obrazek na grayscale */
	grayScale();

	/* Projdeme vsechny pixely obrazku */
	for (y = 0; y < height; ++y)
		for (x = 0; x < width; ++x)
		{
			/* Nacteme soucasnou barvu */
			S_RGBA color = getPixel(x, y);

			/* Porovname hodnotu cervene barevne slozky s prahem.
			   Muzeme vyuzit jakoukoli slozku (R, G, B), protoze
			   obrazek je sedotonovy, takze R=G=B */
			if (color.red > Threshold)
				putPixel(x, y, COLOR_WHITE);
			else
				putPixel(x, y, COLOR_BLACK);
		}
}

/******************************************************************************
 ******************************************************************************
 Funkce prevadi obrazek na cernobily pomoci nahodneho rozptyleni. 
 Vyuziva funkce GetPixel, PutPixel a GrayScale.
 Demonstracni funkce. */
void randomDithering()
{
	int y, x;

	/* Prevedeme obrazek na grayscale */
	grayScale();

	/* Inicializace generatoru pseudonahodnych cisel */
	srand((unsigned int)time(NULL));

	/* Projdeme vsechny pixely obrazku */
	for (y = 0; y < height; ++y)
		for (x = 0; x < width; ++x)
		{
			/* Nacteme soucasnou barvu */
			S_RGBA color = getPixel(x, y);
			
			/* Porovname hodnotu cervene barevne slozky s nahodnym prahem.
			   Muzeme vyuzit jakoukoli slozku (R, G, B), protoze
			   obrazek je sedotonovy, takze R=G=B */
			if (color.red > rand()%255)
			{
				putPixel(x, y, COLOR_WHITE);
			}
			else
				putPixel(x, y, COLOR_BLACK);
		}
}
/*****************************************************************************/
/*****************************************************************************/