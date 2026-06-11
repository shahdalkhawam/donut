#include <GL/glut.h>
#include <cmath>

const int screenWidth = 800;
const int screenHeight = 600;
const float donutSize = 0.5f;
const float rotationSpeed = 0.5f;

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -3.0f);

    static float angle = 0.0f;
    angle += rotationSpeed;

    glRotatef(angle, 0.0f, 1.0f, 0.0f); // Rotation around Y-axis

    glBegin(GL_POINTS);
    for (float theta = 0; theta < 2 * M_PI; theta += 0.1f) {
        for (float phi = 0; phi < 2 * M_PI; phi += 0.1f) {
            float x = (donutSize + donutSize * sin(theta)) * cos(phi);
            float y = donutSize * cos(theta);
            float z = (donutSize + donutSize * sin(theta)) * sin(phi);
            glVertex3f(x, y, z);
        }
    }
    glEnd();

    glutSwapBuffers();
    glutPostRedisplay(); // keep redrawing
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)w / h, 1.0f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(screenWidth, screenHeight);
    glutCreateWindow("3D Spinning Donut");

    glEnable(GL_DEPTH_TEST);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutMainLoop();
    return 0;
}
