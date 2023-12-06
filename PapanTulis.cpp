#define _USE_MATH_DEFINES 



#include <cmath>
#include <cstdlib>
#include <iostream>
#include <fstream>

#include <GL/glew.h>
#include <GL/freeglut.h>

//#include "stb_image.h"

#include "getBMP.h"

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

static unsigned int texture[1]; // Array of texture ids.
static int id = 0;

bool isRGB = 0;
bool isSpinY = 0;
bool moveUp = 0;
bool turnTexture = 0;

GLfloat lightPosition[] = { 0.0f, 0.0f, -10.0f, 1.0f }; // Posisi awal lampu



int brushSize = 3, screenHeight = 800;
float red = 0.0, green = 0.0, blue = 0.0;

void loadTextures()
{
    // Local storage for image data.
    imageFile* image[1];

    // Load the image.
    image[0] = getBMP("textures/josh.bmp");

    // Create texture object texture[0]. 
    glBindTexture(GL_TEXTURE_2D, texture[0]);

    // Specify image data for currently active texture object.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[0]->width, image[0]->height, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, image[0]->data);

    // Set texture parameters for wrapping.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Set texture parameters for filtering.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

static void drawBox(GLfloat size, GLenum type)
{
    // Enable texture mapping
    glEnable(GL_TEXTURE_2D);

    // Bind the loaded texture
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    static GLfloat n[6][3] =
    {
      {-1.0, 0.0, 0.0},
      {0.0, 1.0, 0.0},
      {1.0, 0.0, 0.0},
      {0.0, -1.0, 0.0},
      {0.0, 0.0, 1.0},
      {0.0, 0.0, -1.0}
    };
    static GLint faces[6][4] =
    {
      {0, 1, 2, 3},
      {3, 2, 6, 7},
      {7, 6, 5, 4},
      {4, 5, 1, 0},
      {5, 6, 2, 1},
      {7, 4, 0, 3}
    };

    GLfloat v[8][3];
    GLint i;
    GLfloat texCoords[8][2] = {
    {0.0, 0.0}, // v0
    {1.0, 0.0}, // v1
    {1.0, 1.0}, // v2
    {0.0, 1.0}, // v3
    {0.0, 0.0}, // v4
    {1.0, 0.0}, // v5
    {1.0, 1.0}, // v6
    {0.0, 1.0}  // v7
    };

    v[0][0] = v[1][0] = v[2][0] = v[3][0] = -size / 2;
    v[4][0] = v[5][0] = v[6][0] = v[7][0] = size / 2;
    v[0][1] = v[1][1] = v[4][1] = v[5][1] = -size / 2;
    v[2][1] = v[3][1] = v[6][1] = v[7][1] = size / 2;
    v[0][2] = v[3][2] = v[4][2] = v[7][2] = -size / 2;
    v[1][2] = v[2][2] = v[5][2] = v[6][2] = size / 2;

    for (i = 5; i >= 0; i--) {
        glBegin(type);
        glNormal3fv(&n[i][0]);

        // Loop through each vertex of the face
        for (int j = 0; j < 4; j++) {
            glTexCoord2fv(&texCoords[faces[i][j]][0]); // Set texture coordinates
            glVertex3fv(&v[faces[i][j]][0]);
        }

        glEnd();
    }

    glDisable(GL_TEXTURE_2D);
}  



void wrapTextures(GLfloat normalX, GLfloat normalY, GLfloat normalZ, float texMin, float texMax) {
    glBegin(GL_QUADS);
    GLfloat vertX = boardWidth / 2.0f;
    GLfloat vertY = boardHeight / 2.0f;
    GLfloat vertZ = -(boardDepth / 2.0f);

    glNormal3f(normalX, normalY, normalZ);
    glTexCoord2f(texMin, 0.0); glVertex3f(-vertX, -vertY, vertZ * 2);
    glTexCoord2f(texMax, 0.0); glVertex3f(vertX, -vertY, vertZ * 2);
    glTexCoord2f(texMax, texMax); glVertex3f(vertX, vertY, vertZ * 2);
    glTexCoord2f(texMin, texMax); glVertex3f(-vertX, vertY, vertZ * 2);

    glEnd();
}

// Function untuk menggambar Papan tulis
void drawMonitor() {
    glShadeModel(GL_SMOOTH);


    glPushMatrix();
    glTranslatef(TransX, TransY, TransZ);
    glRotatef(boardYRotation, 0.0f, 1.0f, 0.0f); // Apply Y-axis rotation
    glRotatef(boardXRotation, 1.0f, 0.0f, 0.0f); // Apply X-axis rotation
    glRotatef(boardZRotation, 0.0f, 0.0f, 1.0f);

    
    // Papan tulis
    if (isRGB) glColor4f((float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, 1.0f);
    else glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glPushMatrix();

    glTranslatef(0.0f, 0.0f, -(boardDepth / 2));
    glScalef(boardWidth, boardHeight, boardDepth);
    //glutSolidCube(1.0f);
    drawBox(1.0f, GL_QUADS);
    glPopMatrix();

    // Pinggiran bawah
    if (isRGB) glColor4f((float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, 1.0f);
    else glColor4f(0.7f, 0.7f, 0.7f, 1.0f); // 
    glPushMatrix();
    glTranslatef(0.0f, -boardHeight / 2, -(boardDepth / 2)); // 
    glScalef(boardWidth, 0.5f, boardDepth); // 
    glutSolidCube(1.0f); // 
    glPopMatrix();

    // Pinggiran atas
    if (isRGB) glColor4f((float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, 1.0f);
    else glColor4f(0.7f, 0.7f, 0.7f, 1.0f);
    glPushMatrix();

    glTranslatef(0.0f, boardHeight / 2, -(boardDepth / 2)); // 
    glScalef(boardWidth, 0.5f, boardDepth); // 
    glutSolidCube(1.0f);
    glPopMatrix();

    //Pinggiran kanan
    if (isRGB) glColor4f((float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, 1.0f);
    else glColor4f(0.7f, 0.7f, 0.7f, 1.0f);
    glPushMatrix();

    glTranslatef(boardWidth / 2, 0.0f, -(boardDepth / 2));
    glScalef(0.5f, boardHeight + 0.5f, boardDepth);
    glutSolidCube(1.0f);
    glPopMatrix();

    //Pinggiran kiri
    if (isRGB) glColor4f((float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, 1.0f);
    else glColor4f(0.7f, 0.7f, 0.7f, 1.0f);
    glPushMatrix();

    glTranslatef(-boardWidth / 2, 0.0f, -(boardDepth / 2)); // 
    glScalef(0.5f, boardHeight + 0.5f, boardDepth); // 
    glutSolidCube(1.0f);
    glPopMatrix();

    //tempat spidol
    if (isRGB) glColor4f((float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, 1.0f);
    else glColor4f(0.5f, 0.5f, 0.5f, 1.0f); // 
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
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0.0, 0.0, -20.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    // glEnable(GL_DEPTH_TEST);

     // Set the background color to black
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // Set the camera position and view



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

void setup(void)
{
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glEnable(GL_DEPTH_TEST); // Enable depth testing.

    // Turn on OpenGL lighting.
    glEnable(GL_LIGHTING);


    // Light property vectors.
    float lightAmb[] = { 0.0, 0.0, 0.0, 0.4 };
    float lightDifAndSpec[] = { 0.5, 0.5, 0.5, 0.5 };
    float lightPos[] = { 0.0, 1.0, 0.0, 0.0 }; // Overhead directional light source (e.g., sun).
    float globAmb[] = { 0.4, 0.4, 0.4, 1.0 };

    // Light properties.
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDifAndSpec);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightDifAndSpec);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    glEnable(GL_LIGHT0); // Enable particular light source.
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb); // Global ambient light.

    // Material property vectors.
    float matSpec[] = { 0.0, 0.0, 0.0, 0.4 };
    float matShine[] = { 1.0 };

    // Material properties.
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShine);

    // Enable color material mode.
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    // Cull back faces.
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glGenTextures(1, texture);

    // Load external texture. 
    loadTextures();

    // Turn on OpenGL texturing.
    glEnable(GL_TEXTURE_2D);

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
    case't':
        if (turnTexture) {
            turnTexture = 0;
        }
        else
        {
            turnTexture = 1;
        }

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

void rightMenu(int id) {
    //switch (id) {
    //case 1:
   // };
}

void makeMenu(void) {
    glutCreateMenu(rightMenu);
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

    //glutInitContextVersion(4, 3);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Papan Tulis");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(SpecialKey);


    glewExperimental = GL_TRUE;
    glewInit();

    setup();

    glutMainLoop();
    return 0;
}
