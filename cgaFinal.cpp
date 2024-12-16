#include <GL/glut.h>
#include <string>
#include <iostream>
#include <chrono>

// Parámetros de animación
// Y para el tiempo
float timeStep = 0.01f;
float runtime = 0.0f;
std::chrono::steady_clock::time_point lastTime;


void timer(int value) {
	// Tiempo actual
	auto currentTime = std::chrono::steady_clock::now();

	// Tiempo que ha pasado en segundos
	float elapsed = std::chrono::duration<float>(currentTime - lastTime).count();

	// Actualizar el último tiempo grabado
	lastTime = currentTime;

	// Update runtime
	runtime += elapsed;

	// Mostrar en la CLI
	// std::cout << "runtime: " << runtime << std::endl;

	
	glutPostRedisplay();
	glutTimerFunc(16, timer, 0); // ~60 FPS
}


int selectedBox = 0;
// 0, topLevel
// 1, Academico
// 2, Social
// 3, Relajacion
// 4, Personalizar
// 5, Informes
int level = 0;

// Siempre es número de elementos + 2, por:
// el título
// botón de regresar o salir
const int numBoxesTopLevel = 7;
std::string boxLabelsTopLevel[numBoxesTopLevel] = { "Proyecto Ansiedad", "Diagnostico Academico", "Diagnostico Social", "Relajacion", "Personalizacion?", "Informes", "Salir"};
const int numBoxesAcademicLevel = 4;
std::string boxLabelsAcademicLevel[numBoxesAcademicLevel] = { "Proyecto Ansiedad", "Examen Escrito", "Examen Oral", "Regresar" };
const int numBoxesSocialLevel = 4;
std::string boxLabelsSocialLevel[numBoxesSocialLevel] = { "Proyecto Ansiedad", "Interaccion Grupal", "Presentacion Grupal", "Regresar" };
const int numBoxesRelaxLevel = 4;
std::string boxLabelsRelaxLevel[numBoxesRelaxLevel] = { "Proyecto Ansiedad", "Respiracion Profunda", "Visualizacion de Paisajes", "Regresar" };
const int numBoxesCustomizeLevel = 4;
std::string boxLabelsCustomizeLevel[numBoxesCustomizeLevel] = { "Proyecto Ansiedad", "Duracion", "Dificultad", "Regresar" };
const int numBoxesReportLevel = 3;
std::string boxLabelsReportLevel[numBoxesReportLevel] = { "Proyecto Ansiedad", "Ver", "Regresar" };

void drawText(float x, float y, const std::string &text) {
	glRasterPos2f(x, y);
	for (char c : text) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
	}
}

void showAxis(){
	glColor3i(0,0,0);
	// the dots
	float dotX = 0;
	float dotY = 0;
	float dotZ = 0;
	float limit = 100;
	float step = 0.1;

		do {
			// Every 10 steps -> bigger point
			if ( !((int) (dotX * 10) % 10)) {
				glPointSize(5.0f);
				glColor3f(0,0,1);
				glBegin(GL_POINTS);
					glVertex2f(dotX,dotY);
				glEnd();
			}
			else {
				glPointSize(1.0f);
				glColor3f(0,0,1);
				glBegin(GL_POINTS);
					glVertex2f(dotX,dotY);
				glEnd();
			}
			dotX += step;

			if (dotX >= limit) {
				dotX = -limit;
				limit = 0;
			}
		} while (dotX <= limit && dotX != 0);
	dotX = 0;
	limit = 100;
	// improve later
		 do {
			// Every 10 steps -> bigger point
			if ( !((int) (dotY * 10) % 10)) {
				glPointSize(5.0f);
				glColor3f(1,0,0);
				glBegin(GL_POINTS);
					glVertex2f(dotX,dotY);
				glEnd();
			}
			else {
				glPointSize(1.0f);
				glColor3f(1,0,0);
				glBegin(GL_POINTS);
					glVertex2f(dotX,dotY);
				glEnd();
			}
			dotY += step;
			if (dotY >= limit) {
				dotY = -limit;
				limit = 0;
			}
		} while (dotY <= limit && dotY != 0);

	dotX = 0;
	limit = 100;
	// improve later
		 do {
			// Every 10 steps -> bigger point
			if ( !((int) (dotZ * 10) % 10)) {
				glPointSize(5.0f);
				glColor3f(0,1,0);
				glBegin(GL_POINTS);
					glVertex3f(dotX,dotY,dotZ);
				glEnd();
			}
			else {
				glPointSize(1.0f);
				glColor3f(0,1,0);
				glBegin(GL_POINTS);
					glVertex3f(dotX,dotY,dotZ);
				glEnd();
			}
			dotZ += step;
			if (dotZ >= limit) {
				dotZ = -limit;
				limit = 0;
			}
		} while (dotZ <= limit && dotZ != 0);
}

const int numRows = 4;
const int numCols = 4;

void drawStudent(float x, float y) {
	 // Randomize colors for body and head
	 float bodyR = static_cast<float>(rand()) / RAND_MAX;
	 float bodyG = static_cast<float>(rand()) / RAND_MAX;
	 float bodyB = static_cast<float>(rand()) / RAND_MAX;

	 float headR = static_cast<float>(rand()) / RAND_MAX;
	 float headG = static_cast<float>(rand()) / RAND_MAX;
	 float headB = static_cast<float>(rand()) / RAND_MAX;

	 // Draw the body (rectangle)
	 glColor3f(bodyR, bodyG, bodyB);
	 glBegin(GL_QUADS);
	 glVertex2f(x - 0.05f, y - 0.15f);
	 glVertex2f(x + 0.05f, y - 0.15f);
	 glVertex2f(x + 0.05f, y);
	 glVertex2f(x - 0.05f, y);
	 glEnd();

	 // Draw the head (sphere)
	 glColor3f(headR, headG, headB);
	 glPushMatrix();
	 glTranslatef(x, y + 0.05f, 0.0f);
	 glutSolidSphere(0.03f, 20, 20);
	 glPopMatrix();
}

void drawClassroom() {

	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, 800.0 / 600.0, 1.0, 10.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -2.5f); // Pull the camera back

	float startX = -0.6f;
	float startY = 0.6f;
	float seatSpacingX = 0.3f;
	float seatSpacingY = 0.3f;
	// Draw seats as simple rectangles
	for (int row = 0; row < numRows; ++row) {
		for (int col = 0; col < numCols; ++col) {
				float x = startX + col * seatSpacingX;
				float y = startY - row * seatSpacingY;

				// Draw seat
				glColor3f(0.5f, 0.3f, 0.1f); // Brown color for seats
				glBegin(GL_QUADS);
				glVertex2f(x - 0.1f, y - 0.05f);
				glVertex2f(x + 0.1f, y - 0.05f);
				glVertex2f(x + 0.1f, y + 0.05f);
				glVertex2f(x - 0.1f, y + 0.05f);
				glEnd();

				// Draw student on the seat
				drawStudent(x, y);
		}
	}
}


void drawMenu() {
	float boxWidth = 0.6f;
	float boxHeight = 0.2f;

	// float startX = -0.8f;
	// float startY = 0.6f;

	float startX = -0.2f;
	float startY = 0.8f;
	
	int numBoxes = 0;
	std::string *boxLabels;
	

	switch (level){
		case 0:
			numBoxes = numBoxesTopLevel;
			boxLabels = boxLabelsTopLevel;
		break;
		case 1:
			numBoxes = numBoxesAcademicLevel;
			boxLabels = boxLabelsAcademicLevel;
		break;
		case 2:
			numBoxes = numBoxesSocialLevel;
			boxLabels = boxLabelsSocialLevel;
		break;
		case 3:
			numBoxes = numBoxesRelaxLevel;
			boxLabels = boxLabelsRelaxLevel;
		break;
		case 4:
			numBoxes = numBoxesCustomizeLevel;
			boxLabels = boxLabelsCustomizeLevel;
		break;
		case 5:
			numBoxes = numBoxesReportLevel;
			boxLabels = boxLabelsReportLevel;
		break;
	}

	for (int i = 0; i < numBoxes; i++) {
		if (numBoxes == 1){
			startX = -0.8f;
			startY = 0.6f;
		}

		// Draw the box
		float x = startX;
		float y = startY - i * (boxHeight + 0.1f);

		glColor3f(0.0f, 0.0f, 0.0f); // Black contour
		glBegin(GL_LINE_LOOP);
		glVertex2f(x, y);
		glVertex2f(x + boxWidth, y);
		glVertex2f(x + boxWidth, y - boxHeight);
		glVertex2f(x, y - boxHeight);
		glEnd();

		glColor3f(1.0f, 1.0f, 1.0f); // White fill
		glBegin(GL_QUADS);
		glVertex2f(x, y);
		glVertex2f(x + boxWidth, y);
		glVertex2f(x + boxWidth, y - boxHeight);
		glVertex2f(x, y - boxHeight);
		glEnd();

		// Draw the text
		glColor3f(0.0f, 0.0f, 0.0f); // Black text
		drawText(x + 0.1f, y - 0.12f, boxLabels[i]);
	}

	// Draw the red cube to indicate selection
	float cubeSize = 0.1f;
	float cubeX = startX - 0.125f;
	float cubeY = -0.1f + startY - (selectedBox + 1) * (boxHeight + 0.1f) - cubeSize / 2;
	

	glColor3f(1.0f, 0.0f, 0.0f); // Red cube
	glBegin(GL_QUADS);
	glVertex2f(cubeX, cubeY + cubeSize);
	glVertex2f(cubeX + cubeSize * 0.75, cubeY + cubeSize);
	glVertex2f(cubeX + cubeSize * 0.75, cubeY);
	glVertex2f(cubeX, cubeY);
	//std::cout << "cubeX: " << cubeX << std::endl;
	//std::cout << "cubeY: " << cubeY << std::endl; 
	//std::cout << "cubeX + cubeSize: " << cubeX + cubeSize << std::endl;
	//std::cout << "cubeY + cubeSize: " << cubeY + cubeSize << std::endl;
	glEnd();
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	showAxis();
	drawMenu();
	glutSwapBuffers();
}

void examenEscrito(){
	std::cout << "Examen Escrito" << std::endl;
	drawClassroom();

}

void examenOral(){

	std::cout << "Examen Oral" << std::endl;
}

void interaccionGrupal(){
	std::cout << "Interaccion Grupal" << std::endl;
}

void presentacionGrupal(){

	std::cout << "Presentacion Grupal" << std::endl;
}

void respiracionProfunda(){

	std::cout << "Respiracion Profunda" << std::endl;
}

void visualizacionPaisajes(){
	
	std::cout << "Visualizacion de Paisajes" << std::endl;
}

void duracion(){

	std::cout << "duracion" << std::endl;
}

void dificultad(){

	std::cout << "dificultad" << std::endl;
}

void ver(){

	std::cout << "ver" << std::endl;
}
void handleKeyboard(unsigned char key, int x, int y) {
	int numBoxes = 0;
	

	switch (level){
		case 0:
			numBoxes = numBoxesTopLevel;
			// boxLabels = boxLabelsTopLevel;
		break;
		case 1:
			numBoxes = numBoxesAcademicLevel;
			//boxLabels = boxLabelsAcademicLevel;
		break;
		case 2:
			numBoxes = numBoxesSocialLevel;
			//boxLabels = boxLabelsSocialLevel;
		break;
		case 3:
			numBoxes = numBoxesRelaxLevel;
			//boxLabels = boxLabelsRelaxLevel;
		break;
		case 4:
			numBoxes = numBoxesCustomizeLevel;
			//boxLabels = boxLabelsCustomizeLevel;
		break;
		case 5:
			numBoxes = numBoxesReportLevel;
			//boxLabels = boxLabelsReportLevel;
		break;
	}


	switch (key)
	{
	case 'w':
	case 'W':
		selectedBox = (selectedBox - 1 + (numBoxes - 1) ) % (numBoxes - 1);
	break;
	
	case 's':
	case 'S':
		selectedBox = (selectedBox + 1) % (numBoxes - 1);
	break;

	case '\r':
		
		std::cout << "selectedBox: " << selectedBox << std::endl;
		std::cout << "numBoxes: " << numBoxes << std::endl;
		// para regresar o salir
		if (selectedBox == numBoxes - 2) {
			switch(level) {
				case 0:
					exit(EXIT_SUCCESS);
				break;
				case 1:
				case 2:
				case 3:
				case 4:
				case 5:
					selectedBox = level - 1;
					level = 0;
				break;
			}
		} else {
				// para entrar a submenu
				switch (level) {
				// topLevel
				case 0:
					
					switch(selectedBox) {
						case 0:
							level = 1;
							selectedBox = 0;
						break;

						case 1:
							level = 2;
							selectedBox = 0;
						break;

						case 2:
							level = 3;
							selectedBox = 0;
						break;

						case 3:
							level = 4;
							selectedBox = 0;
						break;

						case 4:
							level = 5;
							selectedBox = 0;
						break;
					}
					break;

				// Academico
				case 1:
					switch (selectedBox) {
					case 0:
						examenEscrito();
						break;
					case 1:
						examenOral();
						break;
					default:
						break;
					}
				break;

				// Social
				case 2:
					switch (selectedBox) {
					case 0:
						interaccionGrupal();
						break;
					case 1:
						presentacionGrupal();
						break;
					default:
						break;
					}
				break;

				// Relajacion
				case 3:
					switch (selectedBox) {
					case 0:
						respiracionProfunda();
						break;
					case 1:
						visualizacionPaisajes();
						break;
					default:
						break;
					}
				break;

				// Personalizacion
				case 4:
					switch (selectedBox) {
					case 0:
						duracion();
						break;
					case 1:
						dificultad();
						break;
					default:
						break;
					}
				break;

				// Informes
				case 5:
					switch (selectedBox) {
					case 0:
						ver();
						break;
					default:
						break;
					}
				break;

				default:
					break;
				}
				
				
			}
	break;

	default:
	break;
	}
	glutPostRedisplay();
}

int main(int argc, char** argv) {
	srand(static_cast<unsigned>(time(0))); // Seed for random colors
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(1200, 680);
	glutCreateWindow("Menu in XY Plane");

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-2.0, 2.0, -2.0, 2.0);

	glutDisplayFunc(display);
	glutKeyboardFunc(handleKeyboard);

	// Grabar el tiempo real durante la primera ejecución
	lastTime = std::chrono::steady_clock::now();
	glutTimerFunc(16, timer, 0);

	glClearColor(0.8f, 0.8f, 0.8f, 1.0f); // Light gray background

	glutMainLoop();
	return 0;
}
