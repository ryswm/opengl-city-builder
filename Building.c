#include <GLUT/glut.h>
#include "Building.h"
float v0 = 0, v1 = 1, v2 = 2, v3 = 3, v4 = 4, v5 = 5, v6 = 6, v7 = 7;

static GLfloat vertices[][3] = {
    {-1.0, 0.0, 1.0},
    {1.0, 0.0, 1.0},
    {1.0, 0.0, -1.0},
    {-1.0, 0.0, -1.0},
    {-1.0, 0.20, 1.0},
    {1.0, 0.20, 1.0},
    {1.0, 0.20, -1.0},
    {-1.0, 0.20, -1.0}
};

static GLubyte quads[] = {
    0, 3, 2, 1,         // Bottom
    4, 7, 6, 5,         // Top
    0, 4, 5, 1,        // Front
    2, 6, 7, 3,        // Back
    3, 7, 4, 0,        // Left
    1, 5, 6, 2          // Right
    
};

static GLfloat normals[][3] = {
    {0.0, -1.0, 0.0},       //Bottom
    {0.0, 1.0, 0.0},        //Top
    {0.0, 0.0, 1.0},        //Front
    {0.0, 0.0, -1.0},       //Back
    {-1.0, 0.0, 0.0},       //L
    {1.0, 0.0, 0.0}         //R
};

Building newBuilding(float scaleX, float scaleY, float scaleZ)
{
    Building tower;
    
    tower.sfx = scaleX;
    tower.sfy = scaleY*2.0;
    tower.sfz = scaleZ;
    tower.floors = (scaleY*2.0)/0.20;
    
    tower.mat_ambient[0] = 0.0;
    tower.mat_ambient[1] = 0.05f;
    tower.mat_ambient[2] = 0.0;
    tower.mat_ambient[3] = 1.0;
    tower.mat_specular[0] = 0.0;
    tower.mat_specular[1] = 0.0;
    tower.mat_specular[2] = 0.004f;
    tower.mat_specular[3] = 1.0;
    tower.mat_diffuse[0] = 0.5;
    tower.mat_diffuse[1] = 0.5;
    tower.mat_diffuse[2] = 0.5;
    tower.mat_diffuse[3] = 1.0;
    tower.mat_shininess[0] = 0;
    
    tower.highlightMat_ambient[0] = 0.0;
    tower.highlightMat_ambient[1] = 0.00;
    tower.highlightMat_ambient[2] = 0.0;
    tower.highlightMat_ambient[3] = 1.0;
    tower.highlightMat_specular[0] = 0.0;
    tower.highlightMat_specular[1] = 0.0;
    tower.highlightMat_specular[2] = 0.0;
    tower.highlightMat_specular[3] = 1.0;
    tower.highlightMat_diffuse[0] = 1.0;
    tower.highlightMat_diffuse[1] = 0.0;
    tower.highlightMat_diffuse[2] = 0.0;
    tower.highlightMat_diffuse[3] = 1.0;
    tower.highlightMat_shininess[0] = 0.0;
    
    return tower;
}

void drawBuilding(Building *tower)
{
    
    glMaterialfv(GL_FRONT, GL_AMBIENT, tower->mat_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, tower->mat_specular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, tower->mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SHININESS, tower->mat_shininess);
    
    glPushMatrix();
    //Transformations
    
    glScalef(tower->sfx, 1.0, tower->sfz);
    
    glBegin(GL_QUADS);
    
    //Top Face
    glNormal3f(normals[1][0], normals[1][1], normals[1][2]);
    glVertex3f(vertices[quads[4]][0], tower->sfy, vertices[quads[4]][2]);
    glVertex3f(vertices[quads[5]][0], tower->sfy, vertices[quads[5]][2]);
    glVertex3f(vertices[quads[6]][0], tower->sfy, vertices[quads[6]][2]);
    glVertex3f(vertices[quads[7]][0], tower->sfy, vertices[quads[7]][2]);
    
    //Bottom Face
    glNormal3f(normals[0][0], normals[0][1], normals[0][2]);
    glVertex3f(vertices[quads[0]][0], vertices[quads[0]][1], vertices[quads[0]][2]);
    glVertex3f(vertices[quads[1]][0], vertices[quads[1]][1], vertices[quads[1]][2]);
    glVertex3f(vertices[quads[2]][0], vertices[quads[2]][1], vertices[quads[2]][2]);
    glVertex3f(vertices[quads[3]][0], vertices[quads[3]][1], vertices[quads[3]][2]);
    
    

      for(float i = 0.0; i < tower->floors; i++){
        //Front
        glNormal3f(normals[2][0], normals[2][1], normals[2][2]);
        glVertex3f(vertices[quads[8]][0], vertices[quads[8]][1], vertices[quads[8]][2]);
        glVertex3f(vertices[quads[9]][0], vertices[quads[9]][1], vertices[quads[9]][2]);
        glVertex3f(vertices[quads[10]][0], vertices[quads[10]][1], vertices[quads[10]][2]);
        glVertex3f(vertices[quads[11]][0], vertices[quads[11]][1], vertices[quads[11]][2]);
    
        //Right
        glNormal3f(normals[5][0], normals[5][1], normals[5][2]);
        glVertex3f(vertices[quads[20]][0], vertices[quads[20]][1], vertices[quads[20]][2]);
        glVertex3f(vertices[quads[21]][0], vertices[quads[21]][1], vertices[quads[21]][2]);
        glVertex3f(vertices[quads[22]][0], vertices[quads[22]][1], vertices[quads[22]][2]);
        glVertex3f(vertices[quads[23]][0], vertices[quads[23]][1], vertices[quads[23]][2]);
    
        //Back
        glNormal3f(normals[3][0], normals[3][1], normals[3][2]);
        glVertex3f(vertices[quads[12]][0], vertices[quads[12]][1], vertices[quads[12]][2]);
        glVertex3f(vertices[quads[13]][0], vertices[quads[13]][1], vertices[quads[13]][2]);
        glVertex3f(vertices[quads[14]][0], vertices[quads[14]][1], vertices[quads[14]][2]);
        glVertex3f(vertices[quads[15]][0], vertices[quads[15]][1], vertices[quads[15]][2]);
        
        //Left
        glNormal3f(normals[4][0], normals[4][1], normals[4][2]);
        glVertex3f(vertices[quads[16]][0], vertices[quads[16]][1], vertices[quads[16]][2]);
        glVertex3f(vertices[quads[17]][0], vertices[quads[17]][1], vertices[quads[17]][2]);
        glVertex3f(vertices[quads[18]][0], vertices[quads[18]][1], vertices[quads[18]][2]);
        glVertex3f(vertices[quads[19]][0], vertices[quads[19]][1], vertices[quads[19]][2]);
    
        //Add Floor
        for(int j = 0; j < 8; j++){
            vertices[j][1] = vertices[j][1] + 0.20;
        }
    }
   
    
    glEnd();
    glPopMatrix();
}
