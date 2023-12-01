#define _USE_MATH_DEFINES 
#define Spidol 2
#define Penghapus 3

#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>

// Global variables for adjusting the monitor board
GLfloat boardWidth = 10.0f;  // Initial width of the board
GLfloat boardHeight = 6.0f; // Initial height of the board
GLfloat boardDepth = 0.1f;  // Initial depth of the board
GLfloat boardXRotation = 0.0f; // Initial rotation around the X-axis
GLfloat boardYRotation = 0.0f; // Initial rotation around the Y-axis
GLfloat boardZRotation = 0.0f;
GLfloat TransX = 0.0f;
GLfloat TransY = 0.0f;
GLfloat TransZ = 0.0f;

bool isRGB = 0;
bool isSpinY = 0;
bool moveUp = 0;


int brushSize = 3, screenHeight = 800;
float red = 0.0, green = 0.0, blue = 0.0;

// Function to draw the 3D monitor and stand
void drawMonitor() {
    glPushMatrix();
    glTranslatef(TransX, TransY, TransZ);
    glRotatef(boardYRotation, 0.0f, 1.0f, 0.0f); // Apply Y-axis rotation
    glRotatef(boardXRotation, 1.0f, 0.0f, 0.0f); // Apply X-axis rotation
    glRotatef(boardZRotation, 0.0f, 0.0f, 1.0f);

    // Papan tulis
   if (isRGB) glColor3f((float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX);
   else glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -(boardDepth / 2));
    glScalef(boardWidth, boardHeight, boardDepth);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Pinggiran bawah
    if (isRGB) glColor3f((float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX);
    else glColor3f(0.7f, 0.7f, 0.7f); // 
    glPushMatrix();
    glTranslatef(0.0f, -boardHeight / 2, -(boardDepth / 2)); // 
    glScalef(boardWidth, 0.5f, boardDepth); // 
    glutSolidCube(1.0f); // 
    glPopMatrix();

    // Pinggiran atas
    if (isRGB) glColor3f((float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX);
    else glColor3f(0.7f, 0.7f, 0.7f);
    glPushMatrix();

    glTranslatef(0.0f, boardHeight / 2, -(boardDepth / 2)); // 
    glScalef(boardWidth, 0.5f, boardDepth); // 
    glutSolidCube(1.0f);
    glPopMatrix();

    //Pinggiran kanan
    if (isRGB) glColor3f((float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX);
    else glColor3f(0.7f, 0.7f, 0.7f);
    glPushMatrix();

    glTranslatef(boardWidth / 2, 0.0f, -(boardDepth / 2));
    glScalef(0.5f, boardHeight + 0.5f, boardDepth);
    glutSolidCube(1.0f);
    glPopMatrix();

    //Pinggiran kiri
    if (isRGB) glColor3f((float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX);
    else glColor3f(0.7f, 0.7f, 0.7f);
    glPushMatrix();

    glTranslatef(-boardWidth / 2, 0.0f, -(boardDepth / 2)); // 
    glScalef(0.5f, boardHeight + 0.5f, boardDepth); // 
    glutSolidCube(1.0f);
    glPopMatrix();

    //tempat spidol
    if (isRGB) glColor3f((float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX);
    else glColor3f(0.5f, 0.5f, 0.5f); // 
    glPushMatrix();


    glTranslatef(0.0f, -boardHeight / 2, -0.5f); // 
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    glScalef(boardWidth, 1.0f, boardDepth); // 
    glutSolidCube(1.0f); // 
    glPopMatrix();

    glPopMatrix();
}

void SpinYAround() {
    if (moveUp) {
        TransY += 0.01;
        TransX += 0.01;
        if (TransY > 5.0) moveUp = 0;
    }
    if (!moveUp) {
        TransY -= 0.01;
        TransX -= 0.01;
        if (TransY < -5.0) moveUp = 1;
    }

    boardYRotation += 0.1;
   if (boardYRotation > 360.0) boardYRotation -= 360.0;
   glutPostRedisplay();
}

// Display callback function
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Set the background color to black
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // Set the camera position and view
    gluLookAt(0.0, 0.0, -20.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
   
   // SpinAround();

    drawMonitor();

    glutSwapBuffers();
}

// Reshape callback function
void reshape(int width, int height) {
    if (height == 0) height = 1;
    float aspect = (float)width / (float)height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, aspect, 0.1, 100.0);
   
    glMatrixMode(GL_MODELVIEW);
}



// Keyboard callback function for adjusting parameters
void keyboard(unsigned char key, int x, int y) {
    switch (key) {

    case 'q':
        if (!isRGB)isRGB = 1;
        else isRGB = 0;
        break;
    case 'z':
        boardZRotation += 5.0f; // Rotate board upward
        break;
    case 'x':
        boardZRotation -= 5.0f; // Rotate board upward
        break;
    case ' ':
        if (isSpinY)
        {
            isSpinY = 0;
            glutIdleFunc(NULL);
        }
        else
        {
            isSpinY = 1;
            glutIdleFunc(SpinYAround);
        }
        break;

    case 27: // Escape key
        exit(0);
        break;
    }

    glutPostRedisplay(); // Request a redraw
}

void SpecialKey(int key, int x, int y) {

    switch (key) {
    case GLUT_KEY_UP:
        boardXRotation += 5.0f; // Rotate board upward
        break;
    case GLUT_KEY_LEFT:
        boardYRotation -= 5.0f; // Rotate board left
        break;
    case GLUT_KEY_RIGHT:
        boardYRotation += 5.0f; // Rotate board right
        break;
    case GLUT_KEY_DOWN:
        boardXRotation -= 5.0f; // Rotate board downward
        break;
    }
    glutPostRedisplay();
}

void printInteraction(void)
{
    std::cout << "Interaction:" << std::endl;
    std::cout << "Tekan tombol panah untuk memutar papan tulis" << std::endl
        << "Tekan tombol Z atau X untuk memutar di sumbu z" << std::endl
        << "Tekan tombol Q atau E untuk on/off efek warna-warni" << std::endl;

}


int main(int argc, char** argv) {
    printInteraction();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(800, screenHeight);
    glutCreateWindow("Papan Tulis");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(SpecialKey);


    glewExperimental = GL_TRUE;
    glewInit();


    glEnable(GL_DEPTH_TEST);

    glutMainLoop();
    return 0;
}
