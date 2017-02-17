#include "IZG06.h"

#include <algorithm>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

using namespace std;

#define PI 3.1415926535897932384626433832795

IZG06::IZG06() {
	// mys
	rmbPressed = false;
	lmbPressed = false;
	posx = 0, posy = 0;
	//kamera
	rotx = 90, roty = 0, zoom = 1;
	center = glm::vec3(0,0,0);
	//animace
	animationEnabled = true;
	time = 0;
	lastFrameTics = 0;
	//debug
	debug = false;
	wireframe = false;
	//svetlo
	lightPosition = glm::vec3(200, 500, 500);
	//vlajka
	flagType = 0;
}

void IZG06::init() {
	/* Muzete zmenit podle potreby (linux/mingw)*/
	string prefix = "../shaders/";
	//string prefix = "./shaders/";

	// Kompilace shaderu
	GLuint vert = compileShader(GL_VERTEX_SHADER, loadFile(prefix + "IZG06.vert"));
	GLuint frag = compileShader(GL_FRAGMENT_SHADER, loadFile(prefix + "IZG06.frag"));
	program = linkShader(2, vert, frag);

	// ziskani lokaci uniformnich promennych
	modelLocation = glGetUniformLocation(program, "model");
	viewLocation = glGetUniformLocation(program, "view");
	projectionLocation = glGetUniformLocation(program, "projection");
	lightPositionLocation = glGetUniformLocation(program, "lightPosition");
	/*** STUDENT DOPLNI - cast 3 - animace ***/
	// ziskani lokace pro cas
	timeLocation = glGetUniformLocation(program, "time");

	// priprava dat
	std::vector<Vertex> vertexArray;
	createGeometry(0.6, 0.4, 60, 40, vertexArray);
	vertexCount = vertexArray.size();

	//vector.data() - vytiahne data z vektoru
	/*** STUDENT DOPLNI - cast 1 - vertex array ***/
	/* Vytvoreni a aktivace VAO 
	* glCreateVertexArrays, glBindVertexArray
	*/
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	/*** STUDENT DOPLNI - cast 1 - buffer ***/
	/* Vytvoreni bufferu pro vertexy a nahrani dat z pole vertexArray 
	 * glCreateBuffers, glBindBuffer, glBufferData
	 */

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertexCount*sizeof(Vertex) , vertexArray.data() , GL_STATIC_DRAW);
	
	/*** STUDENT DOPLNI - cast 1 - atributy ***/
	/* Povoleni obou atributu a nastaveni pointeru 
	 * glEnableVertexAttribArray, glVertexAttribPointer
	 */
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, 0, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, 0, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
}

void IZG06::createGeometry(float width, float height, int xcount, int ycount, std::vector<Vertex>& vertexArray) {
	int vertexCount = 6*(ycount-1)*(xcount-1);
	vertexArray.reserve(vertexCount);
	for (int y = 0; y < ycount-1; ++y) {
		for (int x = 0; x < xcount-1; ++x) {
			float w0 = float(x) / float(xcount - 1);
			float h0 = float(y) / float(ycount - 1);
			float w1 = float(x+1) / float(xcount - 1);
			float h1 = float(y+1) / float(ycount - 1);
			vertexArray.emplace_back(Vertex(glm::vec3(width*(w0 - 0.5), height*(h0 - 0.5), 0), glm::vec2(w0, h0)));
			vertexArray.emplace_back(Vertex(glm::vec3(width*(w1 - 0.5), height*(h0 - 0.5), 0), glm::vec2(w1, h0)));
			vertexArray.emplace_back(Vertex(glm::vec3(width*(w0 - 0.5), height*(h1 - 0.5), 0), glm::vec2(w0, h1)));
			vertexArray.emplace_back(Vertex(glm::vec3(width*(w1 - 0.5), height*(h0 - 0.5), 0), glm::vec2(w1, h0)));
			vertexArray.emplace_back(Vertex(glm::vec3(width*(w1 - 0.5), height*(h1 - 0.5), 0), glm::vec2(w1, h1)));
			vertexArray.emplace_back(Vertex(glm::vec3(width*(w0 - 0.5), height*(h1 - 0.5), 0), glm::vec2(w0, h1)));
		}
	}
}

void IZG06::draw() {
	/* Aktualizace casu animace - klavesa A vypne */
	Uint32 tics = SDL_GetTicks();
	Uint32 dt = tics - lastFrameTics; //ms
	lastFrameTics = tics;
	if (animationEnabled) {
		time += dt;
	}

	// matice
	updateMatrix();
	
	// Vycisteni hloubky a vyplneni pozadi
	glClearColor(0.68, 0.88, 0.93, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// zapnuti depth testu
	glEnable(GL_DEPTH_TEST);

	// Aktivace programu
	glUseProgram(program);

	// nastaveni uniformnich promennych
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
	glUniform3fv(lightPositionLocation, 1, glm::value_ptr(lightPosition));	
	glUniform1i(glGetUniformLocation(program, "flagType"), flagType);
	/*** STUDENT DOPLNI - cast 3 - animace ***/
	/* zapiseme cas do uniformni promene, kterou deklarujeme v shaderu */
	glUniform1i(timeLocation,time);

	// dratovy model - klaveda W
	glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE:GL_FILL);

	/*** STUDENT DOPLNI - cast 1 - kresleni ***/
	/* Vykresleni trojuhelniku
	 * glDrawArrays
	 */
	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}

void IZG06::onMouseMove(int dx, int dy, int x, int y) {
	if (rmbPressed || lmbPressed) {
		rotx += dx;
		roty += dy;
		roty = max(min(roty, 89), -89);
	}
	posx = x;
	posy = y;
}

void IZG06::onMousePress(Uint8 button, int x, int y) {
	switch (button) {
	case SDL_BUTTON_LEFT:
		lmbPressed = true;
		break;
	case SDL_BUTTON_MIDDLE:
		break;
	case SDL_BUTTON_RIGHT:
		rmbPressed = true;
		break;
	}
}

void IZG06::onMouseRelease(Uint8 button, int x, int y) {
	switch (button) {
	case SDL_BUTTON_LEFT:
		lmbPressed = false;
		break;
	case SDL_BUTTON_MIDDLE:
		break;
	case SDL_BUTTON_RIGHT:
		rmbPressed = false;
		break;
	}
}

void IZG06::onMouseWheel(int delta) {
	if (delta > 0) {
		zoom /= 1.1;
	} else {
		zoom *= 1.1;
	}
}

void IZG06::onKeyPress(SDL_Keycode key, Uint16 mod) {
	switch (key) {
	case SDLK_ESCAPE:
		quit();
	case SDLK_a:
		animationEnabled = !animationEnabled;
		if (animationEnabled) time = 0;
		break;
	case SDLK_d:
		debug = !debug;
		break;
	case SDLK_w:
		wireframe = !wireframe;
		break;
	case SDLK_1:
	case SDLK_2:
	case SDLK_3:
	case SDLK_4:
	case SDLK_5:
	case SDLK_6:
		flagType = key-SDLK_1;
		break;
	}
}

void IZG06::updateMatrix() {
	model = glm::mat4(1.0);

	float radx = glm::radians((float)rotx);
	float rady = glm::radians((float)roty);
	float x = zoom * cos(rady) * cos(radx);
	float y = zoom * sin(rady);
	float z = zoom * cos(rady) * sin(radx);

	glm::vec3 eye(x, y, z);
	eye += center;
	glm::vec3 up(0, 1, 0);
	view = glm::lookAt(eye, center, up);
	projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 1000.0f);
}



int main(int /*argc*/, char ** /*argv*/) {
	IZG06 app;
	return app.run();
}