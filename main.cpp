#define _USE_MATH_DEFINES
#include <GL/glut.h>
#include <cmath>
#include <iostream>

void drawText(const char* text, float x, float y, float z) {
    glRasterPos3f(x, y, z);
    for (const unsigned char* p = reinterpret_cast<const unsigned char*>(text); *p; ++p) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *p);
    }
}

void drawGlyphAtPoint(float x, float y, float z, char glyph, float r, float g, float b,
                      float dx, float dy,
                      const GLdouble* model, const GLdouble* proj, const GLint* viewport) {
    GLdouble sx, sy, sz;

    if (gluProject(x, y, z, model, proj, viewport, &sx, &sy, &sz) == GL_TRUE) {
        glColor3f(r, g, b);
        glRasterPos2f(static_cast<GLfloat>(sx + dx), static_cast<GLfloat>(viewport[3] - sy + dy));
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, static_cast<unsigned char>(glyph));
    }
}

const int screenWidth = 900;
const int screenHeight = 700;
const float pi = 3.14159265358979323846f;
const char* shadowChars = " .:-=+*#%@";

enum DisplayMode { MODE_POINTS, MODE_LINES, MODE_SHADOW };
DisplayMode displayMode = MODE_POINTS;

float majorRadius = 0.75f;
float minorRadius = 0.25f;
float rotationSpeed = 0.4f;
float xRotation = 20.0f;
float yRotation = 0.0f;
float zoom = -3.2f;

char shadowCharacterFor(float x, float y, float z, float bias) {
    float depth = std::fabs(x) + std::fabs(y) + std::fabs(z) + bias;
    int index = static_cast<int>(std::floor(depth * 2.2f)) % 10;
    if (index < 0) index = 0;
    if (index >= 10) index = 9;
    return shadowChars[index];
}

void display() {
    static float angle = 0.0f;
    angle += rotationSpeed;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, zoom);
    glRotatef(xRotation, 1.0f, 0.0f, 0.0f);
    glRotatef(yRotation + angle, 0.0f, 1.0f, 0.0f);

    if (displayMode == MODE_SHADOW) {
        GLdouble model[16];
        GLdouble proj[16];
        GLint viewport[4];

        glGetDoublev(GL_MODELVIEW_MATRIX, model);
        glGetDoublev(GL_PROJECTION_MATRIX, proj);
        glGetIntegerv(GL_VIEWPORT, viewport);

        glDisable(GL_DEPTH_TEST);

        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        gluOrtho2D(0.0, screenWidth, 0.0, screenHeight);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();

        for (float theta = 0.0f; theta < 2.0f * pi; theta += 0.08f) {
            for (float phi = 0.0f; phi < 2.0f * pi; phi += 0.08f) {
                float x = (majorRadius + minorRadius * cos(theta)) * cos(phi);
                float y = minorRadius * sin(theta);
                float z = (majorRadius + minorRadius * cos(theta)) * sin(phi);

                char shadowChar = shadowCharacterFor(x, y, z, 0.0f);
                char frontChar = shadowCharacterFor(x, y, z, 0.35f);

                drawGlyphAtPoint(x, y, z, shadowChar, 0.0f, 0.0f, 0.0f, -2.0f, -1.0f, model, proj, viewport);
                drawGlyphAtPoint(x, y, z, frontChar, 0.10f, 0.55f, 1.0f, 0.0f, 0.0f, model, proj, viewport);
            }
        }

        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);

        glEnable(GL_DEPTH_TEST);
    } else {
        glPointSize(2.0f);
        glColor3f(0.2f, 0.9f, 1.0f);

        glBegin(displayMode == MODE_POINTS ? GL_POINTS : GL_LINES);
        for (float theta = 0.0f; theta < 2.0f * pi; theta += 0.08f) {
            for (float phi = 0.0f; phi < 2.0f * pi; phi += 0.08f) {
                float x = (majorRadius + minorRadius * cos(theta)) * cos(phi);
                float y = minorRadius * sin(theta);
                float z = (majorRadius + minorRadius * cos(theta)) * sin(phi);

                if (displayMode == MODE_LINES) {
                    float x2 = (majorRadius + minorRadius * cos(theta + 0.08f)) * cos(phi + 0.08f);
                    float y2 = minorRadius * sin(theta + 0.08f);
                    float z2 = (majorRadius + minorRadius * cos(theta + 0.08f)) * sin(phi + 0.08f);
                    glVertex3f(x, y, z);
                    glVertex3f(x2, y2, z2);
                } else {
                    glVertex3f(x, y, z);
                }
            }
        }
        glEnd();
    }

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0.0, screenWidth, 0.0, screenHeight);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glColor3f(1.0f, 1.0f, 1.0f);
    drawText("Controls:", 10.0f, screenHeight - 12.0f, 0.0f);
    drawText("P dots  L lines  H shadow", 10.0f, screenHeight - 28.0f, 0.0f);
    drawText("W/S tilt  A/D rotate", 10.0f, screenHeight - 44.0f, 0.0f);
    drawText("+/ - size  Up/Down speed", 10.0f, screenHeight - 60.0f, 0.0f);
    drawText("Z/X zoom  R reset  Esc exit", 10.0f, screenHeight - 76.0f, 0.0f);

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    glutSwapBuffers();
    glutPostRedisplay();
}

void reshape(int w, int h) {
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)w / h, 1.0f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int, int) {
    switch (key) {
        case 'p': case 'P':
            displayMode = MODE_POINTS;
            break;
        case 'l': case 'L':
            displayMode = MODE_LINES;
            break;
        case 'h': case 'H':
            displayMode = MODE_SHADOW;
            break;
        case 'w': case 'W': xRotation += 5.0f; break;
        case 's': case 'S': xRotation -= 5.0f; break;
        case 'a': case 'A': yRotation -= 5.0f; break;
        case 'd': case 'D': yRotation += 5.0f; break;
        case '+': case '=': majorRadius += 0.03f; minorRadius += 0.01f; break;
        case '-': case '_':
            majorRadius = std::max(0.2f, majorRadius - 0.03f);
            minorRadius = std::max(0.05f, minorRadius - 0.01f);
            break;
        case 'z': case 'Z': zoom += 0.1f; break;
        case 'x': case 'X': zoom -= 0.1f; break;
        case 'r': case 'R':
            majorRadius = 0.75f;
            minorRadius = 0.25f;
            rotationSpeed = 0.4f;
            xRotation = 20.0f;
            yRotation = 0.0f;
            zoom = -3.2f;
            break;
        case 27:
            exit(0);
    }
}

void specialKeys(int key, int, int) {
    switch (key) {
        case GLUT_KEY_UP:    rotationSpeed += 0.02f; break;
        case GLUT_KEY_DOWN:  rotationSpeed = std::max(0.0f, rotationSpeed - 0.02f); break;
        case GLUT_KEY_LEFT:  majorRadius -= 0.02f; break;
        case GLUT_KEY_RIGHT: majorRadius += 0.02f; break;
    }
}

int main(int argc, char** argv) {
    std::cout << "Donut controls\n";
    std::cout << "  P - dots\n";
    std::cout << "  L - lines\n";
    std::cout << "  H - shadow mode\n";
    std::cout << "  W/S - tilt up/down\n";
    std::cout << "  A/D - rotate left/right\n";
    std::cout << "  + / - - bigger / smaller\n";
    std::cout << "  Up / Down - speed up / slow down\n";
    std::cout << "  Z / X - zoom in / out\n";
    std::cout << "  R - reset\n";
    std::cout << "  Esc - exit\n";

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(screenWidth, screenHeight);
    glutCreateWindow("3D Spinning Donut");

    glEnable(GL_DEPTH_TEST);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);

    glutMainLoop();
    return 0;
}
