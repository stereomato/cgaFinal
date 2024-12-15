#include <GL/glut.h>
#include <string>

int selectedBox = 0;
const int numBoxes = 5;
std::string boxLabels[numBoxes] = {"a", "b", "c", "d", "e"};

void drawText(float x, float y, const std::string &text) {
	glRasterPos2f(x, y);
	for (char c : text) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
	}
}

void drawMenu() {
	float boxWidth = 0.3f;
	float boxHeight = 0.2f;
	float startX = -0.8f;
	float startY = 0.6f;

	for (int i = 0; i < numBoxes; i++) {
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
	float cubeX = startX - 0.15f;
	float cubeY = startY - selectedBox * (boxHeight + 0.1f) - cubeSize / 2;

	glColor3f(1.0f, 0.0f, 0.0f); // Red cube
	glBegin(GL_QUADS);
	glVertex2f(cubeX, cubeY + cubeSize);
	glVertex2f(cubeX + cubeSize, cubeY + cubeSize);
	glVertex2f(cubeX + cubeSize, cubeY);
	glVertex2f(cubeX, cubeY);
	glEnd();
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	drawMenu();
	glutSwapBuffers();
}

void handleKeyboard(unsigned char key, int x, int y) {
	if (key == 'w' || key == 'W') {
		selectedBox = (selectedBox - 1 + numBoxes) % numBoxes;
	} else if (key == 's' || key == 'S') {
		selectedBox = (selectedBox + 1) % numBoxes;
	}
	glutPostRedisplay();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Menu in XY Plane");

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-1.0, 1.0, -1.0, 1.0);

	glutDisplayFunc(display);
	glutKeyboardFunc(handleKeyboard);

	glClearColor(0.8f, 0.8f, 0.8f, 1.0f); // Light gray background

	glutMainLoop();
	return 0;
}
