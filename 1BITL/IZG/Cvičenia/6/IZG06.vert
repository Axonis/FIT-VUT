#version 400

// atributy
layout(location=0) in vec3 aPosition;
layout(location=1) in vec2 aTexCoord;

// uniformni promenne - matice
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform int time;


// vystupni promenne
out vec3 pozicia;
out vec3 normala;
out vec2 texcoord;


void main(){
	/*** STUDENT DOPLNI - cast 2 - transformace ***/
	/* transformace vertexu pomoci matic */

	/*** STUDENT DOPLNI - cast 3 - animace ***/
	/* animace vlajky - posuneme Z souradnici vertexu v zavislosti na X a casu
	 * deklarujeme celociselnou uniformni promennou a v cpp do ni zapiseme aktualni cas
	 * pouzijeme funkci sinus z+=A*sin(time*B+x*C)
	 */

	vec3 temp;
	temp = aPosition;
	
	temp.z += 0.05 * sin(-time*0.005 + temp.x * 12);
	temp.z *= aTexCoord.x; 

	gl_Position = projection * view * model * vec4(temp,1);
	



	
	
	
	/*** STUDENT DOPLNI - cast 4 - poslani promennych do FS ***/
	/* pro vypocet osvetleni je nutne poslat pozici a normalu do FS
	 * navic budou potreba texturovaci souradnice
	 *
	 * deklarujeme 3 vystupni promenne spravneho typu a zapiseme do nich
	 * normalu vypocitame jako vec3(x*-cos(arg),0,1) viz krok 3
	 */
	 
	//normala.x = aTexCoord.x * -cos(aTexCoord.y);
	//normala.y = 0;
	//normala.z = 1;

	normala = normalize(vec3( (aTexCoord.x * -cos(temp.x)) ,0 ,1 ) );

	pozicia=vec3(aTexCoord,1);
	
}
