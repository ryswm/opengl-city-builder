/*******************************************************************
           Multi-Part Model Construction and Manipulation
********************************************************************/

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include "Vector3D.h"
#include "QuadMesh.h"
#include "CubeMesh.h"
#include "Building.h"

static Building tower;
static CubeMesh block;
static unsigned char currentKey;
bool isBuilt = false, scaleH = false, scaleS = false, transBuild = false, isExtruded = false;
float sizeH = 1.0, sizeX = 1.0, sizeZ = 1.0, forback = 0.0, lefrig = 0.0;
float towerX, towerY, towerZ;


const int meshSize = 20;    // Default Mesh Size
const int vWidth = 650;     // Viewport width in pixels
const int vHeight = 500;    // Viewport height in pixels
//Add glEnable(GL_NORMALIZE)

static int currentButton;
static unsigned char currentKey;

// Lighting/shading and material properties for drone - upcoming lecture - just copy for now

// Light properties
static GLfloat light_position0[] = { -6.0F, 12.0F, 0.0F, 1.0F };
static GLfloat light_position1[] = { 6.0F, 12.0F, 0.0F, 1.0F };
static GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
static GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
static GLfloat light_ambient[] = { 0.2F, 0.2F, 0.2F, 1.0F };

// Material properties
static GLfloat drone_mat_ambient[] = { 0.4F, 0.2F, 0.0F, 1.0F };
static GLfloat drone_mat_specular[] = { 0.1F, 0.1F, 0.0F, 1.0F };
static GLfloat drone_mat_diffuse[] = { 0.9F, 0.5F, 0.0F, 1.0F };
static GLfloat drone_mat_shininess[] = { 0.0F };

// A quad mesh representing the ground
static QuadMesh groundMesh;

// Structure defining a bounding box, currently unused
//struct BoundingBox {
//    Vector3D min;
//    Vector3D max;
//} BBox;

// Prototypes for functions in this module
void initOpenGL(int w, int h);
void display(void);
void reshape(int w, int h);
void mouse(int button, int state, int x, int y);
void mouseMotionHandler(int xMouse, int yMouse);
void keyboard(unsigned char key, int x, int y);
void functionKeys(int key, int x, int y);
Vector3D ScreenToWorld(int x, int y);



int main(int argc, char **argv)
{
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(vWidth, vHeight);
    glutInitWindowPosition(200, 30);
    glutCreateWindow("City Builder");

    // Initialize GL
    initOpenGL(vWidth, vHeight);

    // Register callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotionHandler);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(functionKeys);

    // Start event loop, never returns
    glutMainLoop();

    return 0;
}


// Set up OpenGL. For viewport and projection setup see reshape(). */
void initOpenGL(int w, int h)
{
    // Set up and enable lighting
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);

    glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
    glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    //glEnable(GL_LIGHT1);   // This light is currently off

    // Other OpenGL setup
    glEnable(GL_DEPTH_TEST);   // Remove hidded surfaces
    glShadeModel(GL_SMOOTH);   // Use smooth shading, makes boundaries between polygons harder to see 
    glClearColor(0.6F, 0.6F, 0.6F, 0.0F);  // Color and depth for glClear
    glClearDepth(1.0f);
    glEnable(GL_NORMALIZE);    // Renormalize normal vectors 
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);   // Nicer perspective

    // Set up ground quad mesh
    Vector3D origin = NewVector3D(-10.0f, 0.0f, 10.0f);
    Vector3D dir1v = NewVector3D(1.0f, 0.0f, 0.0f);
    Vector3D dir2v = NewVector3D(0.0f, 0.0f, -1.0f);
    groundMesh = NewQuadMesh(meshSize);
    InitMeshQM(&groundMesh, meshSize, origin, 20.0, 20.0, dir1v, dir2v);

    Vector3D ambient = NewVector3D(0.0f, 0.05f, 0.0f);
    Vector3D diffuse = NewVector3D(0.4f, 0.8f, 0.4f);
    Vector3D specular = NewVector3D(0.04f, 0.04f, 0.04f);
    SetMaterialQM(&groundMesh, ambient, diffuse, specular, 0.2);
    
    
    block = newCube();
    
    
    
    

    // Set up the bounding box of the scene
    // Currently unused. You could set up bounding boxes for your objects eventually.
    //Set(&BBox.min, -8.0f, 0.0, -8.0);
    //Set(&BBox.max, 8.0f, 6.0,  8.0);
}


// Callback, called whenever GLUT determines that the window should be redisplayed
// or glutPostRedisplay() has been called.
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set drone material properties
    glMaterialfv(GL_FRONT, GL_AMBIENT, drone_mat_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, drone_mat_specular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, drone_mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SHININESS, drone_mat_shininess);

    
    glPushMatrix();
    tower = newBuilding(sizeX, sizeH, sizeZ);
    if(isExtruded){
        drawBuilding(&tower);
    }
    glPopMatrix();
    
    glPushMatrix(); //Cube
    if(isBuilt){
        glTranslatef(lefrig, 0.0, forback);
        glScalef(sizeX, sizeH, sizeZ);
        glTranslatef(0.0, 1.0, 0.0);
        drawCube(&block);
    }
    glPopMatrix(); //Cube
    
    // Draw ground mesh
    DrawMeshQM(&groundMesh, meshSize);

    glutSwapBuffers();   // Double buffering, swap buffers
}


// Callback, called at initialization and whenever user resizes the window.
void reshape(int w, int h)
{
    // Set up viewport, projection, then change to modelview matrix mode - 
    // display function will then set up camera and do modeling transforms.
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.0, (GLdouble)w / h, 0.2, 40.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Set up the camera at position (0, 6, 22) looking at the origin, up along positive y axis
    gluLookAt(0.0, 25.0, 15.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

// Callback, handles input from the keyboard, non-arrow keys
void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 't':
        currentKey = 't';
        transBuild = true;
        scaleH = false;
        scaleS = false;
        break;
    case 's':
        currentKey = 's';
        scaleS = true;
        transBuild = false;
        scaleH = false;
        break;
    case 'p':
        currentKey = 'p';
        isBuilt = true;
        break;
    case 'h':
        currentKey = 'h';
        scaleH = true;
        transBuild = false;
        scaleS = false;
        break;
    case 'e':
        currentKey = 'e';
        isExtruded = true;
        isBuilt = false;
        transBuild = false;
        scaleS = false;
        scaleH = false;
        break;
    }

    glutPostRedisplay();   // Trigger a window redisplay
}

// Callback, handles input from the keyboard, function and arrow keys
void functionKeys(int key, int x, int y)
{
    
    if (key == GLUT_KEY_F1)
    {
        
    }
    else if (key == GLUT_KEY_LEFT)
    {
        if(transBuild){
            lefrig -= 0.1;
        }
        else if(scaleS){
            sizeX -= 0.1;
            //block.sfx = sizeX;
        }
    }
    else if (key == GLUT_KEY_RIGHT)
    {
        if(transBuild){
            lefrig += 0.1;
        }
        else if(scaleS){
            sizeX += 0.1;
            
        }
    }
    else if (key == GLUT_KEY_UP){
        if(scaleH){
            sizeH += 0.20;
        }
        else if(scaleS){
            sizeZ += 0.1;
        }
        else if(transBuild){
            forback -= 0.1;
        }
    }
    else if (key == GLUT_KEY_DOWN){
        if(scaleH){
            sizeH -= 0.20;
        }
        else if(scaleS){
            sizeZ -= 0.1;
        }
        else if(transBuild){
            forback += 0.1;
        }
    }
    
    // Do transformations with arrow keys
    //else if (...)   // GLUT_KEY_DOWN, GLUT_KEY_UP, GLUT_KEY_RIGHT, GLUT_KEY_LEFT
    //{
    //}

    glutPostRedisplay();   // Trigger a window redisplay
}


// Mouse button callback - use only if you want to 
void mouse(int button, int state, int x, int y)
{
    currentButton = button;

    switch (button)
    {
    case GLUT_LEFT_BUTTON:
        if (state == GLUT_DOWN)
        {
            ;

        }
        break;
    case GLUT_RIGHT_BUTTON:
        if (state == GLUT_DOWN)
        {
            ;
        }
        break;
    default:
        break;
    }

    glutPostRedisplay();   // Trigger a window redisplay
}


// Mouse motion callback - use only if you want to 
void mouseMotionHandler(int xMouse, int yMouse)
{
    if (currentButton == GLUT_LEFT_BUTTON)
    {
        ;
    }

    glutPostRedisplay();   // Trigger a window redisplay
}


Vector3D ScreenToWorld(int x, int y)
{
    // you will need to finish this if you use the mouse
    return NewVector3D(0, 0, 0);
}



