#version 400
// vstupni promenne
in vec3 pozicia;
in vec3 normala;
in vec2 texcoord;


// uniformni promenne
uniform vec3 lightPosition; // pozice svetla
uniform int flagType; // pro pripadne prepinani ruznych vlajek

// vysledna barva
out vec4 fragColor;

#define WHITE vec3(1,1,1)
#define RED vec3(1,0,0)
#define BLUE vec3(0,0,1)
#define GREEN vec3(0,1,0)
#define BLACK vec3(0,0,0)
#define YELLOW vec3(1,1,0)

void main(){
	vec3 color=WHITE;	

	/*** STUDENT DOPLNI - cast 4 - promenne z VS ***/
	/* deklarujeme 3 vstupni promenne stejneho typu a jmena jako ve VS
	 */

 	/*** STUDENT DOPLNI - cast 5 - osvetlovaci model ***/
	/* pomoci lambertova osvetlovaciho modelu vystinujeme vlajku
	 * vyuzijeme pozice svetla, pozice fragmentu a normaly
	 */	 


 	/*** STUDENT DOPLNI - cast 6 - obarveni vlajky ***/
	/* v zavislosti na texturovacis souradnicich obarvime vlajku
	 * zvolte si svoji oblibenou vlajku
	 */
	float diffuse = max(dot(normala, lightPosition), 1);
	diffuse = diffuse * (1.0 / (1.0 + (0.25 * lightPosition.z * lightPosition.z)));

	if(pozicia.y > 0.5) 
		fragColor = vec4(WHITE,1);
	else 
		fragColor = vec4(RED,1);

	fragColor *= dot(normala, normalize(lightPosition - pozicia));
	//fragColor *= diffuse;
	
}
