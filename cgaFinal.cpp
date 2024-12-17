#include <GL/glut.h>
#include <string>
#include <iostream>
#include <chrono>
#include <cmath>
#include <ctime>

// Parámetros de animación
// Y para el tiempo
float timeStep = 0.01f;
float runtime = 0.0f;
std::chrono::steady_clock::time_point lastTime;

int difficulty = 2;
int duration = 5;

int selectedBox = 0;
// 0, topLevel
// 1, Academico
// 2, Social
// 3, Relajacion
// 4, Personalizar
	// 6, duracion
	// 7, dificultad
// 5, Informes
	// 8, ver
int level = 0;

// 1 = examen escrito
// 2 = examen oral
// 3 = interaccion grupal
// 4 = presentacion grupal
// 5 = respiracion profunda
// 6 = visualizacion de paisaje
int play = 0;

// Siempre es número de elementos + 2, por:
// el título
// botón de regresar o salir
const int numBoxesTopLevel = 7;
std::string boxLabelsTopLevel[numBoxesTopLevel] = { "Proyecto Ansiedad", "Diagnostico Academico", "Diagnostico Social", "Relajacion", "Personalizacion", "Informes", "Salir"};
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

// dificultades
const int numBoxesDurationLevel = 5;
std::string boxLabelsDurationLevel[numBoxesDurationLevel] = { "Proyecto Ansiedad", "Corto", "Normal", "Largo", "Regresar" };


// dificultades
const int numBoxesDifficultiesLevel = 5;
std::string boxLabelsDifficultiesLevel[numBoxesDifficultiesLevel] = { "Proyecto Ansiedad", "Facil", "Normal", "Dificil", "Regresar" };

// ver reportes
int numTherapies = 0;
const int numBoxesShowReportLevel = 4;
std::string boxLabelsShowReportLevel[numBoxesShowReportLevel] = { "Proyecto Ansiedad", "Numero de terapias", std::to_string(numTherapies), "Regresar" };

float treeAngle = 0.0f; // Angle for swaying palm tree
float sunY = 0.8f;      // Vertical position of the sun

bool inAnimation = false;
std::chrono::steady_clock::time_point activityStartTime;
bool activityTimeSet = false;
// normal y normal (5) al comienzo

// Tiempo actual
std::chrono::_V2::steady_clock::time_point currentTime;
// Tiempo que ha pasado en segundos
float elapsed = 0;

// Animation parameters
float radius = 0.1f; // Initial radius
float targetRadius = 0.5f; // Final target radius
float elasticAmplitude = 0.02f; // Subtle elastic amplitude (gentler oscillation)
float animationProgress = 0.0f; // Progress of the animation (0.0f to 1.0f)
float elasticProgress = 0.0f; // Progress of the elastic animation (0.0f to 1.0f)
bool isElastic = false; // Flag to switch to elastic animation
bool isShrinking = false; // Flag to shrink the circle
int animationPhase = 0; // 0 = Expand, 1 = Elastic, 2 = Shrink

// Time variables
std::chrono::steady_clock::time_point breathingStartTime;
std::chrono::steady_clock::time_point breathingCurrentTime;

// Function to update animation progress
void updateAnimation() {
	// Get the current time using std::chrono
	breathingCurrentTime = std::chrono::steady_clock::now();
	auto elapsedTime = std::chrono::duration<float>(breathingCurrentTime - breathingStartTime).count(); // Elapsed time in seconds

	if (animationPhase == 0) { // Expanding phase
		animationProgress = elapsedTime / 4.0f; // 4 seconds to expand
		if (animationProgress >= 1.0f) {
			animationProgress = 1.0f; // Ensure it caps at 1
			animationPhase = 1; // Switch to elastic phase
			breathingStartTime = breathingCurrentTime; // Reset start time for the next phase
		}
		// Smooth expansion, but limit radius to 1.0 to prevent it from getting too big
		radius = 0.1f + (targetRadius - 0.1f) * animationProgress;
		if (radius > 1.0f) {
			radius = 1.0f; // Limit maximum size to 1.0
		}
	} else if (animationPhase == 1) { // Elastic animation phase
		elasticProgress = elapsedTime / 4.0f; // 4 seconds for elastic effect
		if (elasticProgress >= 1.0f) {
			elasticProgress = 1.0f;
			animationPhase = 2; // Switch to shrinking phase
			breathingStartTime = breathingCurrentTime; // Reset start time for shrinking phase
		}
		// Elastic effect (sin wave) with a subtle amplitude
		radius = targetRadius + elasticAmplitude * sin(elasticProgress * 6.28f); 
		// Limit elastic radius so it doesn't exceed the view
		if (radius > 1.0f) {
			radius = 1.0f; // Limit maximum size to 1.0
		}
	} else if (animationPhase == 2) { // Shrinking phase
		animationProgress = 1.0f - elapsedTime / 4.0f; // 4 seconds to shrink
		radius = targetRadius * animationProgress; // Shrink smoothly to original size
		if (radius < 0.1f) {
			radius = 0.1f; // Prevent shrinking below original size
			animationPhase = 0; // Reset to expanding phase
			breathingStartTime = breathingCurrentTime; // Reset start time for next animation loop
		}
	}
}
void timer(int value) {
	currentTime = std::chrono::steady_clock::now();
	elapsed = std::chrono::duration<float>(currentTime - lastTime).count();

	// Actualizar el último tiempo grabado
	lastTime = currentTime;

	// Update runtime
	runtime += elapsed;

	// 15 segundos es el tiempo base
	// se multiplica por la dificultad
	// std::cout << "what is this: " << std::chrono::duration_cast<std::chrono::seconds>(currentTime - activityStartTime).count() << std::endl;

	if (activityTimeSet){
		std::cout << "Temporizador de Actividad: " << std::chrono::duration_cast<std::chrono::seconds>(currentTime - activityStartTime).count() << std::endl;
		
		if (play == 5){
			//std::cout << "WE ARE RUNNING UPDATEANIMATION" << std::endl;
			updateAnimation();
		}

		if (std::chrono::duration_cast<std::chrono::seconds>(currentTime - activityStartTime).count() > difficulty * duration)
		{
			std::cout << "EXITING ANIMATION TIMER" << std::endl;
			inAnimation = false;
			// maybe needed
			play = 0;
			activityTimeSet = false;
			glutPostRedisplay();
			glutTimerFunc(16, timer, 0); // ~60 FPS
			return;
		}
	}
	// Mostrar en la CLI
	// std::cout << "runtime: " << runtime << std::endl;

	// Para la animación de la palmera
	treeAngle += elapsed * 0.5f;
	if (treeAngle > 2 * M_PI)
		treeAngle -= 2 * M_PI;

	
	// Para la animación del sol
	sunY = 0.8f + 0.05f * sin(runtime * 2.0f);

	
	if (inAnimation){
		// std::cout << "inAnim" << std::endl;
		glutPostRedisplay();
	}
	glutTimerFunc(16, timer, 0); // ~60 FPS
}

void drawText(float x, float y, const std::string &text) {
	glRasterPos2f(x, y);
	for (char c : text) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
	}
}

void showAxis(){
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.53f, 0.81f, 0.98f, 1.0f); // Light blue background
	
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, 800.0 / 600.0, 1.0, 10.0);
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

// Constants for the scene
const int numRows = 4;
const int numCols = 4;

// Function to draw a blackboard
void drawBlackboard() {
	// Blackboard
	glColor3f(0.0f, 0.3f, 0.0f); // Dark green color
	glBegin(GL_QUADS);
		glVertex3f(-1.5f, 0.7f, -3.0f);
		glVertex3f(1.5f, 0.7f, -3.0f);
		glVertex3f(1.5f, -0.3f, -3.0f);
		glVertex3f(-1.5f, -0.3f, -3.0f);
	glEnd();

	// Blackboard frame
	glColor3f(0.5f, 0.2f, 0.1f); // Brown color
	glLineWidth(4.0f);
	glBegin(GL_LINE_LOOP);
		glVertex3f(-1.5f, 0.7f, -3.0f);
		glVertex3f(1.5f, 0.7f, -3.0f);
		glVertex3f(1.5f, -0.3f, -3.0f);
		glVertex3f(-1.5f, -0.3f, -3.0f);
	glEnd();

	// Clock
	glColor3f(1.0f, 1.0f, 1.0f); // White clock face
	glBegin(GL_QUADS);
		glVertex3f(-0.1f, 1.0f, -3.01f);
		glVertex3f(0.1f, 1.0f, -3.01f);
		glVertex3f(0.1f, 0.8f, -3.01f);
		glVertex3f(-0.1f, 0.8f, -3.01f);
	glEnd();

	// Clock hands (simplified as lines)
	glColor3f(0.0f, 0.0f, 0.0f);
	glLineWidth(2.0f);
	glBegin(GL_LINES);
		glVertex3f(0.0f, 0.9f, -3.01f); // Center of the clock
		glVertex3f(0.0f, 0.95f, -3.01f); // Hour hand
		glVertex3f(0.0f, 0.9f, -3.01f);
		glVertex3f(0.05f, 0.9f, -3.01f); // Minute hand
	glEnd();
}

// Function to draw a student
void drawStudent(float x, float y, float z) {
	// Randomize colors
	float bodyR = static_cast<float>(rand()) / RAND_MAX;
	float bodyG = static_cast<float>(rand()) / RAND_MAX;
	float bodyB = static_cast<float>(rand()) / RAND_MAX;

	float headR = static_cast<float>(rand()) / RAND_MAX;
	float headG = static_cast<float>(rand()) / RAND_MAX;
	float headB = static_cast<float>(rand()) / RAND_MAX;

	// Draw body
	glColor3f(bodyR, bodyG, bodyB);
	glPushMatrix();
	glTranslatef(x, y, z);
	glScalef(0.1f, 0.2f, 0.1f);
	glutSolidCube(1.0);
	glPopMatrix();

	// Draw head
	glColor3f(headR, headG, headB);
	glPushMatrix();
	glTranslatef(x, y + 0.15f, z);
	glutSolidSphere(0.05, 20, 20);
	glPopMatrix();
}

// Function to draw desks and students
// Function to draw desks and students
void drawClassroom() {
	

	float startX = -0.9f;
	float startZ = -1.5f;
	float spacingX = 0.6f;
	float spacingZ = 0.8f;

	for (int row = 0; row < numRows; ++row) {
		for (int col = 0; col < numCols; ++col) {
			float x = startX + col * spacingX;
			float z = startZ + row * spacingZ;

			// Draw desk
			glColor3f(0.7f, 0.5f, 0.3f); // Wood-like desk
			glPushMatrix();
			glTranslatef(x, -0.1f, z);
			glScalef(0.4f, 0.05f, 0.4f);
			glutSolidCube(1.0);
			glPopMatrix();

			// Draw student
			drawStudent(x, 0.0f, z);
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

	glClearColor(0.8f, 0.8f, 0.8f, 1.0f); // Light gray background

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-2.0, 2.0, -2.0, 2.0);
	//gluLookAt(0,0,0, 0, 0, -1, 0, 1, 0);

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
		case 6:
			numBoxes = numBoxesDurationLevel;
			boxLabels = boxLabelsDurationLevel;
		break;
		case 7:
			numBoxes = numBoxesDifficultiesLevel;
			boxLabels = boxLabelsDifficultiesLevel;
		break;

		case 8:
			numBoxes = numBoxesShowReportLevel;
			boxLabels = boxLabelsShowReportLevel;
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

	
	glutSwapBuffers();
}
void prepare3D(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, 800.0 / 600.0, 1.0, 10.0);

	glMatrixMode(GL_MODELVIEW);
}
void examenEscrito(){
	std::cout << "Examen Escrito" << std::endl;

	// Guarda el tiempo de inicio
	if (!activityTimeSet){
		activityTimeSet = true;
		activityStartTime = lastTime;
	}
	
	// No estamos en el menú
	inAnimation = true;
	prepare3D();
	// Set camera position
	gluLookAt(0.0f, 1.0f, 3.0f,  // Camera position (eye)
				 0.0f, 0.0f, -3.0f, // Look-at point
				 0.0f, 1.0f, 0.0f); // Up direction
	drawBlackboard();
	drawClassroom();
	glutSwapBuffers();
}

// Function to draw a more detailed desk with legs, surface, and a drawer
void drawDetailedDesk() {
	 glPushMatrix();
	 glColor3f(0.55f, 0.27f, 0.07f); // Main desk color (wood)

	 // Draw the main desk surface
	 glTranslatef(0.0f, -0.25f, 0.0f); // Adjust the surface to be centered
	 glScalef(2.0f, 0.1f, 1.0f); // Surface is wider and thinner
	 glutSolidCube(1.0f); // Desk surface
	 glPopMatrix();

	 // Add desk legs (4 legs)

	 // Front-left leg
	 glPushMatrix();
	 glColor3f(0.39f, 0.19f, 0.05f); // Darker wood color for the legs
	 glTranslatef(-0.85f, -0.65f, 0.45f);
	 glScalef(0.1f, 0.7f, 0.1f); // Leg size and position
	 glutSolidCube(1.0f);
	 glPopMatrix();

	 // Front-right leg
	 glPushMatrix();
	 glTranslatef(0.85f, -0.65f, 0.45f);
	 glScalef(0.1f, 0.7f, 0.1f);
	 glutSolidCube(1.0f);
	 glPopMatrix();

	 // Back-left leg
	 glPushMatrix();
	 glTranslatef(-0.85f, -0.65f, -0.45f);
	 glScalef(0.1f, 0.7f, 0.1f);
	 glutSolidCube(1.0f);
	 glPopMatrix();

	 // Back-right leg
	 glPushMatrix();
	 glTranslatef(0.85f, -0.65f, -0.45f);
	 glScalef(0.1f, 0.7f, 0.1f);
	 glutSolidCube(1.0f);
	 glPopMatrix();

	 // Optional: Add a drawer (front of the desk)
	 glPushMatrix();
	 glColor3f(0.3f, 0.15f, 0.05f); // Darker wood for the drawer
	 glTranslatef(0.0f, -0.25f, 0.55f); // Positioning the drawer
	 glScalef(0.6f, 0.2f, 0.1f); // Drawer size
	 glutSolidCube(1.0f); // Drawer shape
	 glPopMatrix();
}

void examenOral(){
	std::cout << "Examen Oral" << std::endl;

	// Guarda el tiempo de inicio
	activityTimeSet = true;
	activityStartTime = lastTime;

	// No estamos en el menú
	inAnimation = true;

	prepare3D();
	glClearColor(0.53f, 0.81f, 0.98f, 1.0f); // Light blue background

	 // Set up the camera
	gluLookAt(0.0, 1.0, 4.0, // Eye position
				 0.0, 0.0, 0.0, // Look-at point
				 0.0, 1.0, 0.0); // Up direction

	 // Draw the blackboard
	 drawBlackboard();

	 // Draw the detailed desk
	 drawDetailedDesk();

	 // Draw the teacher (on the left side, larger size)
	 glPushMatrix();
	 glTranslatef(-1.0f, -0.3f, 0.0f); // Position teacher on the left
	 glScalef(5, 5, 5);       // Scale up the teacher
	 drawStudent(0.0f, 0.0f, 0.0f);    // Teacher centered at this transformation
	 glPopMatrix();

	 // Draw the student (on the right side, normal size)
	 glPushMatrix();
	 glTranslatef(1.0f, -0.3f, 0.0f); // Position student on the right
	 glScalef(3, 3, 3);      // Normal size for the student
	 drawStudent(0.0f, 0.0f, 0.0f);   // Student centered at this transformation
	 glPopMatrix();

	 glutSwapBuffers();
}

void interaccionGrupal(){
	std::cout << "Interaccion Grupal" << std::endl;
	// Guarda el tiempo de inicio
	activityTimeSet = true;
	activityStartTime = lastTime;

	// No estamos en el menú
	inAnimation = true;

	prepare3D();
	glClearColor(0.53f, 0.81f, 0.98f, 1.0f); // Light blue background

	// Set up the camera
	gluLookAt(0.0, 1.0, 4.0, // Eye position
			0.0, 0.0, 0.0, // Look-at point
			0.0, 1.0, 0.0); // Up direction

	// Draw the blackboard
	drawBlackboard();

	// Draw the more detailed desk
	drawDetailedDesk();

	// Draw the teacher (left side, larger)
	//glPushMatrix();
	//glTranslatef(-1.5f, 0.0f, 0.0f);
	//drawStudent(0.0f, 0.0f, 0.0f); // Teacher
	//glPopMatrix();

	// Draw 4 students forming a circle in front of the blackboard
	float radius = 0.5f; // Radius of the circle
	for (int i = 0; i < 4; i++) {
		float angle = i * (360.0f / 4) * (3.14159f / 180.0f); // Angle in radians
		float x = radius * cos(angle); // X position
		float z = radius * sin(angle); // Z position

		glPushMatrix();
		glTranslatef(x, -0.25f, -2.0f + z); // Position student

		// Rotate the head to face inward (toward the center of the circle)
		float headRotation = -90.0f + (i * 90.0f); // Adjust rotation based on placement
		glRotatef(headRotation, 0.0f, 1.0f, 0.0f); // Rotate around Y-axis

		glScaled(3,3,3);
		drawStudent(0.0f, 0.0f, 0.0f);
		glPopMatrix();
	}

	glutSwapBuffers();

}

void presentacionGrupal(){

	std::cout << "Presentacion Grupal" << std::endl;
	// Guarda el tiempo de inicio
	activityTimeSet = true;
	activityStartTime = lastTime;

	// No estamos en el menú
	inAnimation = true;

	prepare3D();
	glClearColor(0.53f, 0.81f, 0.98f, 1.0f); // Light blue background

	 // Set up the camera
	gluLookAt(0.0, 1.0, 4.0, // Eye position
				 0.0, 0.0, 0.0, // Look-at point
				 0.0, 1.0, 0.0); // Up direction

	// Draw the blackboard
	drawBlackboard();

	// Draw the more detailed desk
	drawDetailedDesk();

	// Draw the teacher (left side, larger)
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 1.5f);
	glScaled(2,2,2);
	drawStudent(0.0f, 0.0f, 0.0f); // Using drawStudent to replace drawPerson
	glPopMatrix();

	// Draw three students in front of the blackboard (presentation mode)
	float studentSpacing = 0.5f; // Spacing between students
	glPushMatrix();
	glTranslatef(-0.5f, -0.25f, -2.5f); // Start at leftmost position
	glScaled(3,3,3);
	drawStudent(0.0f, 0.0f, 0.0f); // Left student
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0f, -0.25f, -2.5f); // Center student
	glScaled(3,3,3);
	drawStudent(0.0f, 0.0f, 0.0f);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.5f, -0.25f, -2.5f); // Right student
	glScaled(3,3,3);
	drawStudent(0.0f, 0.0f, 0.0f);
	glPopMatrix();

	glutSwapBuffers();

}


void drawCircle(float x, float y, float radius, int segments = 100) {
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x, y);
	for (int i = 0; i <= segments; i++) {
		float theta = 2.0f * M_PI * i / segments;
		float cx = radius * cosf(theta);
		float cy = radius * sinf(theta);
		glVertex2f(x + cx, y + cy);
	}
	glEnd();
}

void respiracionProfunda(){

	//std::cout << "Respiracion Profunda" << std::endl;
	
	glDisable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
	//gluLookAt(0,0,0, 0, 0, -1, 0, 1, 0);
	
	// Draw circle with current radius at the center (0, 0)
	glColor3f(1,1,1);
	drawCircle(0.0f, 0.0f, radius);

	glutSwapBuffers();
}


void drawPalmTree(float x, float y) {
	glPushMatrix();
	glTranslatef(x, y, 0.0f);
	glRotatef(sin(treeAngle) * 5.0f, 0.0f, 0.0f, 1.0f); // Swaying effect

	// std::cout << treeAngle << " tree angle" << std::endl;
	// Draw trunk
	glColor3f(0.55f, 0.27f, 0.07f);
	glBegin(GL_QUADS);
		glVertex2f(-0.02f, 0.0f);
		glVertex2f(0.02f, 0.0f);
		glVertex2f(0.02f, 0.3f);
		glVertex2f(-0.02f, 0.3f);
	glEnd();

	// Draw leaves
	glColor3f(0.0f, 0.5f, 0.0f);
	glBegin(GL_TRIANGLES);
		glVertex2f(0.0f, 0.3f);
		glVertex2f(-0.15f, 0.4f);
		glVertex2f(0.15f, 0.4f);

		glVertex2f(0.0f, 0.3f);
		glVertex2f(-0.1f, 0.5f);
		glVertex2f(0.1f, 0.5f);
	glEnd();

	glPopMatrix();
}

void drawScene() {
	// Sky
	glColor3f(0.53f, 0.81f, 0.98f);
	glBegin(GL_QUADS);
		glVertex2f(-1.0f, 0.0f);
		glVertex2f(1.0f, 0.0f);
		glVertex2f(1.0f, 1.0f);
		glVertex2f(-1.0f, 1.0f);
	glEnd();

	// Sand
	glColor3f(0.94f, 0.87f, 0.73f);
	glBegin(GL_QUADS);
		glVertex2f(-1.0f, -1.0f);
		glVertex2f(1.0f, -1.0f);
		glVertex2f(1.0f, 0.0f);
		glVertex2f(-1.0f, 0.0f);
	glEnd();

	// Sun
	glColor3f(1.0f, 1.0f, 0.0f);
	drawCircle(-0.7f, sunY, 0.1f);

	// Small Island
	glColor3f(0.85f, 0.7f, 0.5f);
	glBegin(GL_TRIANGLE_FAN);
		glVertex2f(0.7f, -0.1f);
		for (int i = 0; i <= 50; i++) {
			float theta = 2.0f * M_PI * i / 50;
			glVertex2f(0.7f + 0.15f * cosf(theta), -0.1f + 0.05f * sinf(theta));
		}
	glEnd();

	// Palm Tree
	drawPalmTree(0.7f, -0.05f);
}

void visualizacionPaisajes(){
	// std::cout << "Visualizacion de Paisajes" << std::endl;
	glDisable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
	glClearColor(0.4f, 0.7f, 1.0f, 1.0f);
	drawScene();
	glutSwapBuffers();
}

void duracion(int value){

	std::cout << "duracion" << std::endl;
	switch (value)
	{
	case 0:
		duration = 2;
		break;
	
	case 1:
		duration = 5;
		break;

	case 2:
		duration = 10;
		break;
	}
	std::cout << "Duracion: " << duration << std::endl;

}

void dificultad(int value){
	std::cout << "dificultad" << std::endl;
	std::cout << "duracion" << std::endl;
	switch (value)
	{
	case 0:
		difficulty = 1;
		break;
	
	case 1:
		difficulty = 2;
		break;

	case 2:
		difficulty = 3;
		break;
	}

	std::cout << "Dificultad: " << difficulty << std::endl;
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
		case 6:
			numBoxes = numBoxesDurationLevel;
			//boxLabels = boxLabelsDurationLevel;
		break;
		case 7:
			numBoxes = numBoxesDifficultiesLevel;
			//boxLabels = boxLabelsDifficultiesLevel;
		break;
		case 8:
			numBoxes = numBoxesShowReportLevel;
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
		if (inAnimation)
		{
			inAnimation = false;
			std::cout << "We have set nIM to false and returned";
			play = 0;
			glutPostRedisplay();
			return;
		}
		
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
				case 6:
				case 7:
					selectedBox = 0;
					// probablemente hay que ajustar para niveles más altos
					// terribles
					if (level == 7) {
						selectedBox = 1;
					}
					level = 4;
				break;
				case 8:
					selectedBox = 0;
					level = 5;
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
						play = 1;
						break;
					case 1:
						examenOral();
						play = 2;
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
						play = 3;
						break;
					case 1:
						presentacionGrupal();
						play = 4;
						break;
					default:
						break;
					}
				break;

				// Relajacion
				case 3:
					switch (selectedBox) {
					case 0:
						//respiracionProfunda();
						activityTimeSet = true;
						activityStartTime = lastTime;

						// No estamos en el menú
						inAnimation = true;
						glEnable(GL_BLEND);
						glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

						// Actualizar el contador de terapias
						numTherapies++;
						boxLabelsShowReportLevel[2] = std::to_string(numTherapies);
						play = 5;
						break;
					case 1:
						// visualizacionPaisajes();
						// Guarda el tiempo de inicio
						activityTimeSet = true;
						activityStartTime = lastTime;

						// No estamos en el menú
						inAnimation = true;

						// Actualizar el contador de terapias
						numTherapies++;
						boxLabelsShowReportLevel[2] = std::to_string(numTherapies);
	
						play = 6;
						break;
					default:
						break;
					}
				break;

				// Personalizacion
				case 4:
					switch (selectedBox) {
					case 0:
						//duracion();
						level = 6;
						selectedBox = 0;

						break;
					case 1:
						//dificultad();
						level = 7;
						selectedBox = 0;
						break;
					default:
						break;
					}
				break;
					// Duracion
					case 6:
						switch(selectedBox) {
							case 0:
								duracion(0);
							break;
							case 1:
								duracion(1);
							break;
							case 2:
								duracion(2);
							break;
						}
					break;

					// Dificultad
					case 7:
						switch(selectedBox) {
							case 0:
								dificultad(0);
							break;
							case 1:
								dificultad(1);
							break;
							case 2:
								dificultad(2);
							break;
						}
					break;
				
				// Informes
				case 5:
					switch (selectedBox) {
					case 0:
						switch (selectedBox) {
							case 0:
							// ver();
							level = 8;
							selectedBox = 0;

						break;
						default:
							break;
						}
						
						play = 7;
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
	}
	glutPostRedisplay();
}

// Reshape function
void reshape(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, static_cast<double>(w) / h, 1.0, 10.0);
	std::cout << "Windows resized to: " << w << " x " << h << std::endl;

	glMatrixMode(GL_MODELVIEW);
	// redibujados extra?
	// examenEscrito();
	switch (play)
	{
	case 1:
		examenEscrito();
		break;
	
	case 2:
		examenOral();
		break;

	case 3:
		interaccionGrupal();
		break;

	case 4:
		presentacionGrupal();
		break;

	case 6:
		visualizacionPaisajes();
		break;
	}
	
}

void display() {
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glPushMatrix();
	showAxis();
	//examenEscrito();
	glPopMatrix();
	switch (play)
	{

	//case 0:
	//	examenEscrito();
	//	break;
	case 5:
		respiracionProfunda();
		break;
	case 6:
		visualizacionPaisajes();
		break;
	
	default:
		break;
	}

	if (!inAnimation){
		//std::cout << "help" << std::endl;
		drawMenu();
	}
	
	//glutSwapBuffers();
}

int main(int argc, char** argv) {
	srand(static_cast<unsigned>(time(0))); // Seed for random colors
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(1200, 680);
	glutCreateWindow("Menu in XY Plane");

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(handleKeyboard);

	// Grabar el tiempo real durante la primera ejecución
	lastTime = std::chrono::steady_clock::now();
	glutTimerFunc(25, timer, 0);

	glutMainLoop();
	return 0;
}
