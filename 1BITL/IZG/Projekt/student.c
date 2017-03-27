/******************************************************************************
 * Projekt - Zaklady pocitacove grafiky - IZG
 * spanel@fit.vutbr.cz
 *
 * $Id:$
 */

#include "student.h"
#include "transform.h"
#include "fragment.h"

#include <memory.h>
#include <math.h>

/*****************************************************************************
 * Globalni promenne a konstanty
 */
/* Typ/ID rendereru (nemenit) */
const int           STUDENT_RENDERER = 1;
float				frame = 0;
const S_Material    MAT_WHITE_AMBIENT = { 1.0, 1.0, 1.0, 1.0 };
const S_Material    MAT_WHITE_DIFFUSE = { 1.0, 1.0, 1.0, 1.0 };
const S_Material    MAT_WHITE_SPECULAR = { 1.0, 1.0, 1.0, 1.0 };

/*****************************************************************************
 * Funkce vytvori vas renderer a nainicializuje jej
 */

S_Renderer *studrenCreate() {
	S_StudentRenderer *renderer = (S_StudentRenderer *)malloc(sizeof(S_StudentRenderer));
	IZG_CHECK(renderer, "Cannot allocate enough memory");

	/* inicializace default rendereru */
	renderer->base.type = STUDENT_RENDERER;
	renInit(&renderer->base);
	/* nastaveni ukazatelu na upravene funkce */
	/* napr. renderer->base.releaseFunc = studrenRelease; */

	renderer->base.releaseFunc = studrenRelease;
	/* prepneme na studentsky renderer */
	renderer->base.projectTriangleFunc = studrenProjectTriangle;
	/* prepneme texturu z proceduralnej na texturu z bmp */
	renderer->texture = loadBitmap(TEXTURE_FILENAME, &renderer->width, &renderer->height);

	/* inicializace nove pridanych casti */

	return (S_Renderer *)renderer;
}

/*****************************************************************************
 * Funkce korektne zrusi renderer a uvolni pamet
 */

void studrenRelease(S_Renderer **ppRenderer) {
	S_StudentRenderer *renderer;

	if (ppRenderer && *ppRenderer) {
		/* ukazatel na studentsky renderer */
		renderer = (S_StudentRenderer *)(*ppRenderer);

		/* pripadne uvolneni pameti */
		/* dealokujeme texturu z loadBitmap funkcie */
		free(renderer->texture);

		/* fce default rendereru */
		renRelease(ppRenderer);
	}
}

/******************************************************************************
 * Nova fce pro rasterizaci trojuhelniku s podporou texturovani
 * Upravte tak, aby se trojuhelnik kreslil s texturami
 * (doplnte i potrebne parametry funkce - texturovaci souradnice, ...)
 * v1, v2, v3 - ukazatele na vrcholy trojuhelniku ve 3D pred projekci
 * n1, n2, n3 - ukazatele na normaly ve vrcholech ve 3D pred projekci
 * x1, y1, ... - vrcholy trojuhelniku po projekci do roviny obrazovky
 */

void studrenDrawTriangle(S_Renderer *pRenderer,
                         S_Coords *v1, S_Coords *v2, S_Coords *v3,
                         S_Coords *n1, S_Coords *n2, S_Coords *n3,
                         int x1, int y1,
                         int x2, int y2,
                         int x3, int y3,
						 S_Triangle *triangle,
						 double pers_aa, double pers_bb, double pers_cc
						 )
{
	int         minx, miny, maxx, maxy;
	int         a1, a2, a3, b1, b2, b3, c1, c2, c3;
	int         s1, s2, s3;
	int         x, y, e1, e2, e3;
	double      alpha, beta, gamma, w1, w2, w3, z;
	double		interx, intery;
	S_RGBA      col1, col2, col3, color;

	IZG_ASSERT(pRenderer && v1 && v2 && v3 && n1 && n2 && n3);

	/* vypocet barev ve vrcholech */
	col1 = pRenderer->calcReflectanceFunc(pRenderer, v1, n1);
	col2 = pRenderer->calcReflectanceFunc(pRenderer, v2, n2);
	col3 = pRenderer->calcReflectanceFunc(pRenderer, v3, n3);

	/* obalka trojuhleniku */
	minx = MIN(x1, MIN(x2, x3));
	maxx = MAX(x1, MAX(x2, x3));
	miny = MIN(y1, MIN(y2, y3));
	maxy = MAX(y1, MAX(y2, y3));

	/* oriznuti podle rozmeru okna */
	miny = MAX(miny, 0);
	maxy = MIN(maxy, pRenderer->frame_h - 1);
	minx = MAX(minx, 0);
	maxx = MIN(maxx, pRenderer->frame_w - 1);

	/* Pineduv alg. rasterizace troj.
	hranova fce je obecna rovnice primky Ax + By + C = 0
	primku prochazejici body (x1, y1) a (x2, y2) urcime jako
	(y1 - y2)x + (x2 - x1)y + x1y2 - x2y1 = 0 */

	/* normala primek - vektor kolmy k vektoru mezi dvema vrcholy, tedy (-dy, dx) */
	a1 = y1 - y2;
	a2 = y2 - y3;
	a3 = y3 - y1;
	b1 = x2 - x1;
	b2 = x3 - x2;
	b3 = x1 - x3;

	/* koeficient C */
	c1 = x1 * y2 - x2 * y1;
	c2 = x2 * y3 - x3 * y2;
	c3 = x3 * y1 - x1 * y3;

	/* vypocet hranove fce (vzdalenost od primky) pro protejsi body */
	s1 = a1 * x3 + b1 * y3 + c1;
	s2 = a2 * x1 + b2 * y1 + c2;
	s3 = a3 * x2 + b3 * y2 + c3;

	if (!s1 || !s2 || !s3)
	{
		return;
	}

	/* normalizace, aby vzdalenost od primky byla kladna uvnitr trojuhelniku */
	if (s1 < 0)
	{
		a1 *= -1;
		b1 *= -1;
		c1 *= -1;
	}
	if (s2 < 0)
	{
		a2 *= -1;
		b2 *= -1;
		c2 *= -1;
	}
	if (s3 < 0)
	{
		a3 *= -1;
		b3 *= -1;
		c3 *= -1;
	}

	/* koeficienty pro barycentricke souradnice */
	alpha = 1.0 / ABS(s2);
	beta = 1.0 / ABS(s3);
	gamma = 1.0 / ABS(s1);

	/* vyplnovani... */
	for (y = miny; y <= maxy; ++y)
	{
		/* inicilizace hranove fce v bode (minx, y) */
		e1 = a1 * minx + b1 * y + c1;
		e2 = a2 * minx + b2 * y + c2;
		e3 = a3 * minx + b3 * y + c3;

		for (x = minx; x <= maxx; ++x)
		{
			if (e1 >= 0 && e2 >= 0 && e3 >= 0)
			{
				/* interpolace pomoci barycentrickych souradnic
				e1, e2, e3 je aktualni vzdalenost bodu (x, y) od primek */
				w1 = alpha * e2;
				w2 = beta * e3;
				w3 = gamma * e1;

				/* interpolace z-souradnice */
				z = w1 * v1->z + w2 * v2->z + w3 * v3->z;

				/* interpolacia texturovanej x(u) suradnice */

				/* nasobenie je rychlejsie ako delenie a 1/cislo sa prevedie prekladacom na konstantu */
				double opta = 1 / pers_aa;
				double optb = 1 / pers_bb;
				double optc = 1 / pers_cc;

				//interx = w1 * (triangle->t[0].x) + w2 * (triangle->t[1].x) + w3 * (triangle->t[2].x);
				double denomx = 1 / (w1 * opta + w2 * optb + w3 * optc);
				interx = (w1 * (triangle->t[0].x * opta) + w2 * (triangle->t[1].x * optb) + w3 * (triangle->t[2].x * optc)) *  denomx;

				/* interpolacia texturovanej y(v) suradnice */
				//intery = w1 * (triangle->t[0].y) + w2 * (triangle->t[1].y) + w3 * (triangle->t[2].y);
				double denomy =  1 / (w1 * opta + w2 * optb + w3 * optc);
				intery = (w1 * (triangle->t[0].y * opta) + w2 * (triangle->t[1].y * optb) + w3 * (triangle->t[2].y * optc)) *  denomy;

				/* interpolace barvy */
				color = studrenTextureValue((S_StudentRenderer *)pRenderer, interx, intery);
				
				/* farba = (farba z textury * farba z osvetlovacieho modelu) / 255 aby nepretekalo  */

				color.red   = (color.red   *  ROUND2BYTE(w1 * col1.red + w2 * col2.red + w3 * col3.red)		  / 255);
				color.green = (color.green *  ROUND2BYTE(w1 * col1.green + w2 * col2.green + w3 * col3.green) / 255);
				color.blue  = (color.blue  *  ROUND2BYTE(w1 * col1.blue + w2 * col2.blue + w3 * col3.blue)    / 255);
				color.alpha = 255;


				/* vykresleni bodu */
				if (z < DEPTH(pRenderer, x, y))
				{
					PIXEL(pRenderer, x, y) = color;
					DEPTH(pRenderer, x, y) = z;
				}
			}

			/* hranova fce o pixel vedle */
			e1 += a1;
			e2 += a2;
			e3 += a3;
		}
	}
}

/******************************************************************************
 * Vykresli i-ty trojuhelnik n-teho klicoveho snimku modelu
 * pomoci nove fce studrenDrawTriangle()
 * Pred vykreslenim aplikuje na vrcholy a normaly trojuhelniku
 * aktualne nastavene transformacni matice!
 * Upravte tak, aby se model vykreslil interpolovane dle parametru n
 * (cela cast n udava klicovy snimek, desetinna cast n parametr interpolace
 * mezi snimkem n a n + 1)
 * i - index trojuhelniku
 * n - index klicoveho snimku (float pro pozdejsi interpolaci mezi snimky)
 */

void studrenProjectTriangle(S_Renderer *pRenderer, S_Model *pModel, int i, float n)
{
	S_Coords    aa, bb, cc;							/* souradnice vrcholu po transformaci */
	S_Coords	aa_n1, bb_n1, cc_n1;				/* suradnice vrcholu po transformacii pre n+1 snimok */
	S_Coords    naa, nbb, ncc;						/* normaly ve vrcholech po transformaci */
	S_Coords    naa_n1, nbb_n1, ncc_n1;				/* normaly vo vrcholoch po transformaci pre n+1 snimok */
	S_Coords    nn;									/* normala trojuhelniku po transformaci */
	S_Coords    nn_n1;								/* normala trojuholnika po transformaci pre n+1 snimok */
	int         u1, v1, u2, v2, u3, v3;				/* souradnice vrcholu po projekci do roviny obrazovky */
	S_Triangle  *triangle;							/* trojuholnik pre body ktore potrebujeme */
	int         vertexOffsetn, normalOffsetn;		/* offset pro vrcholy a normalove vektory trojuhelniku */
	int 		vertexOffset_n1, normalOffset_n1;	/* offset pre vrcholy a normalove vektory trojuholnika pre n+1 snimok */
	int         i0, i1, i2, in;						/* indexy vrcholu a normaly pro i-ty trojuhelnik n-teho snimku */
	int 		i0_n1, i1_n1, i2_n1, in_n1;			/* indexy vrcholov a normaly pre i-ty trojuholnik n+1-teho snimku */
	float		decpart =((float)n) - ((int)n);	    /* desatinna cast parametru n */
	double		pers_aa, pers_bb, pers_cc;			/* suradnice potrebne pre perspektivnu korekciu */

	IZG_ASSERT(pRenderer && pModel && i >= 0 && i < trivecSize(pModel->triangles) && n >= 0);

	/* z modelu si vytahneme i-ty trojuhelnik */
	triangle = trivecGetPtr(pModel->triangles, i);

	/* ziskame offset pro vrcholy n-teho snimku */
	vertexOffsetn = (((int)n) % pModel->frames) * pModel->verticesPerFrame;
	vertexOffset_n1 = (((int)(n+1)) % pModel->frames) * pModel->verticesPerFrame;

	/* ziskame offset pro normaly trojuhelniku n-teho snimku */
	normalOffsetn = (((int)n) % pModel->frames) * pModel->triangles->size;
	normalOffset_n1 = (((int)(n+1)) % pModel->frames) * pModel->triangles->size;
	
	/* indexy vrcholu pro i-ty trojuhelnik n-teho snimku - pricteni offsetu */
	i0 = triangle->v[0] + vertexOffsetn;
	i1 = triangle->v[1] + vertexOffsetn;
	i2 = triangle->v[2] + vertexOffsetn;

	i0_n1 = triangle->v[0] + vertexOffset_n1;
	i1_n1 = triangle->v[1] + vertexOffset_n1;
	i2_n1 = triangle->v[2] + vertexOffset_n1;

	/* index normaloveho vektoru pro i-ty trojuhelnik n-teho snimku - pricteni offsetu */
	in = triangle->n + normalOffsetn;

	in_n1 = triangle->n + normalOffset_n1;

	/* transformace vrcholu matici model */
	trTransformVertex(&aa, cvecGetPtr(pModel->vertices, i0));
	trTransformVertex(&bb, cvecGetPtr(pModel->vertices, i1));
	trTransformVertex(&cc, cvecGetPtr(pModel->vertices, i2));

	trTransformVertex(&aa_n1, cvecGetPtr(pModel->vertices, i0_n1));
	trTransformVertex(&bb_n1, cvecGetPtr(pModel->vertices, i1_n1));
	trTransformVertex(&cc_n1, cvecGetPtr(pModel->vertices, i2_n1));

	/* postupna interpolacia medzi bodami aan1 a aa */
	/* blizim sa dx krokmi k samotnemu bodu x v n+1 snimke */
	aa.x += (aa_n1.x - aa.x) * decpart;
	aa.y += (aa_n1.y - aa.y) * decpart;
	aa.z += (aa_n1.z - aa.z) * decpart;

	bb.x += (bb_n1.x - bb.x) * decpart;
	bb.y += (bb_n1.y - bb.y) * decpart;
	bb.z += (bb_n1.z - bb.z) * decpart;

	cc.x += (cc_n1.x - cc.x) * decpart;
	cc.y += (cc_n1.y - cc.y) * decpart;
	cc.z += (cc_n1.z - cc.z) * decpart;

	/* promitneme vrcholy trojuhelniku na obrazovku */
	/* ulozim si 4tu suradnicu pre perspektivnu korekciu*/
	pers_aa = trProjectVertex(&u1, &v1, &aa);
	pers_bb = trProjectVertex(&u2, &v2, &bb);
	pers_cc = trProjectVertex(&u3, &v3, &cc);

	/* pro osvetlovaci model transformujeme take normaly ve vrcholech */
	trTransformVector(&naa, cvecGetPtr(pModel->normals, i0));
	trTransformVector(&nbb, cvecGetPtr(pModel->normals, i1));
	trTransformVector(&ncc, cvecGetPtr(pModel->normals, i2));

	trTransformVector(&naa_n1, cvecGetPtr(pModel->normals, i0_n1));
	trTransformVector(&nbb_n1, cvecGetPtr(pModel->normals, i1_n1));
	trTransformVector(&ncc_n1, cvecGetPtr(pModel->normals, i2_n1));

	/* interpolujeme aj normaly kvoli osvetleniu a backface cullingu */
	naa.x += (naa_n1.x - naa.x) * decpart;
	naa.y += (naa_n1.y - naa.y) * decpart;
	naa.z += (naa_n1.z - naa.z) * decpart;

	nbb.x += (nbb_n1.x - nbb.x) * decpart;
	nbb.y += (nbb_n1.y - nbb.y) * decpart;
	nbb.z += (nbb_n1.z - nbb.z) * decpart;

	ncc.x += (ncc_n1.x - ncc.x) * decpart;
	ncc.y += (ncc_n1.y - ncc.y) * decpart;
	ncc.z += (ncc_n1.z - ncc.z) * decpart;

	/* normalizace normal */
	coordsNormalize(&naa);
	coordsNormalize(&nbb);
	coordsNormalize(&ncc);

	/* transformace normaly trojuhelniku matici model */
	trTransformVector(&nn, cvecGetPtr(pModel->trinormals, in));
	trTransformVector(&nn_n1, cvecGetPtr(pModel->trinormals, in_n1));

	
	nn.x += (nn_n1.x - nn.x) * decpart;
	nn.y += (nn_n1.y - nn.y) * decpart;
	nn.z += (nn_n1.z - nn.z) * decpart;

	/* normalizace normaly */
	coordsNormalize(&nn);

	/* je troj. privraceny ke kamere, tudiz viditelny? */
	if (!renCalcVisibility(pRenderer, &aa, &nn))
	{
		/* odvracene troj. vubec nekreslime */
		return;
	}

	/* rasterizace trojuhelniku */
	studrenDrawTriangle(pRenderer,
		&aa, &bb, &cc,
		&naa, &nbb, &ncc,
		u1, v1, u2, v2, u3, v3,
		triangle,
		pers_aa, pers_bb, pers_cc
		);
}

/******************************************************************************
* Vraci hodnotu v aktualne nastavene texture na zadanych
* texturovacich souradnicich u, v
* Pro urceni hodnoty pouziva bilinearni interpolaci
* Pro otestovani vraci ve vychozim stavu barevnou sachovnici dle uv souradnic
* u, v - texturovaci souradnice v intervalu 0..1, ktery odpovida sirce/vysce textury
*/

S_RGBA studrenTextureValue( S_StudentRenderer * pRenderer, double u, double v )
{
	/* bilinearna interpolacia */
	int x, y;
	S_RGBA color, colorst, colorx, colory, colorxy;

	x = (int)(u * pRenderer->width);
	y = (int)(v * pRenderer->height);

	colorst = pRenderer->texture[x * pRenderer->width + y];
	colorx = pRenderer->texture[(x+1) * pRenderer->width + y];
	colory = pRenderer->texture[x * pRenderer->width + (y+1)];
	colorxy = pRenderer->texture[(x+1) * pRenderer->width + (y+1)];
	
	color.red = (unsigned char)((colorst.red + colorx.red + colory.red + colorxy.red) / 4 );
	color.green = (unsigned char)((colorst.green + colorx.green + colory.green + colorxy.green) / 4 );
	color.blue = (unsigned char)((colorst.blue + colorx.blue + colory.blue + colorxy.blue) / 4 );

	return color;

    /*unsigned char c = ROUND2BYTE( ( ( fmod( u * 10.0, 1.0 ) > 0.5 ) ^ ( fmod( v * 10.0, 1.0 ) < 0.5 ) ) * 255 );
    return makeColor( c, 255 - c, 0 );*/
}

/******************************************************************************
 ******************************************************************************
 * Funkce pro vyrenderovani sceny, tj. vykresleni modelu
 * Upravte tak, aby se model vykreslil animovane
 * (volani renderModel s aktualizovanym parametrem n)
 */

void renderStudentScene(S_Renderer *pRenderer, S_Model *pModel)
{	
	/* test existence frame bufferu a modelu */
	IZG_ASSERT(pModel && pRenderer);

	/* nastavit projekcni matici */
	trProjectionPerspective(pRenderer->camera_dist, pRenderer->frame_w, pRenderer->frame_h);

	/* vycistit model matici */
	trLoadIdentity();

	/* nejprve nastavime posuv cele sceny od/ke kamere */
	trTranslate(0.0, 0.0, pRenderer->scene_move_z);

	/* nejprve nastavime posuv cele sceny v rovine XY */
	trTranslate(pRenderer->scene_move_x, pRenderer->scene_move_y, 0.0);

	/* natoceni cele sceny - jen ve dvou smerech - mys je jen 2D... :( */
	trRotateX(pRenderer->scene_rot_x);
	trRotateY(pRenderer->scene_rot_y);

	/* nastavime material */
	renMatAmbient(pRenderer, &MAT_WHITE_AMBIENT);
	renMatDiffuse(pRenderer, &MAT_WHITE_DIFFUSE);
	renMatSpecular(pRenderer, &MAT_WHITE_SPECULAR);

	/* a vykreslime nas model (ve vychozim stavu kreslime pouze snimek 0) */
	renderModel(pRenderer, pModel, frame);
}

/* Callback funkce volana pri tiknuti casovace
 * ticks - pocet milisekund od inicializace */
void onTimer( int ticks )
{
    /* uprava parametru pouzivaneho pro vyber klicoveho snimku a pro interpolaci mezi snimky */
 
	/* rychlost animacie podla cisla ktorym delim */
	frame = (float)ticks * ((float)1 / 100);

}

/*****************************************************************************
 *****************************************************************************/
