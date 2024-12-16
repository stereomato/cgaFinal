#include <GL/glut.h>
#include <cstdlib>
#include <ctime>

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

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw the classroom
    drawClassroom();

    glutSwapBuffers();
}

int main(int argc, char** argv) {
    srand(static_cast<unsigned>(time(0))); // Seed for random colors

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Classroom with Students");

    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 800.0 / 600.0, 1.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -2.5f); // Pull the camera back

    glClearColor(0.8f, 0.8f, 0.8f, 1.0f); // Light gray background

    glutDisplayFunc(display);

    glutMainLoop();
    return 0;
}
