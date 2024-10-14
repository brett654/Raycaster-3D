#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <math.h>

#define PI 3.1415926535
#define DR 0.0174533 // one degreee in radians

float playerXPos, playerYPos, playerDirY, playerDirX, playerAngle;

void drawPlayer() {
    glColor3f(1.0f, 1.0f, 0.0f); // Set color to yellow
    glPointSize(8.0f);           // Set point size
    glBegin(GL_POINTS);          // Begin drawing points
    glVertex2f(playerXPos, playerYPos);          // Use floating-point vertex
    glEnd();                     // End drawing

    glLineWidth(3);
    glBegin(GL_LINES);
    glVertex2f(playerXPos, playerYPos);                       // Start point of the line
    glVertex2f(playerXPos + playerDirX * 5, playerYPos + playerDirY * 5);   // End point of the line
    glEnd();

}

float degToRad(int a) { return a * PI/180.0;}
int FixAng(int a){ if(a>359){ a-=360;} if(a<0){ a+=360;} return a;}
float distance(float ax, float ay, float bx, float by, float ang){ return cos(degToRad(ang))*(bx-ax)-sin(degToRad(ang))*(by-ay);}
void castRay(float& rayXCoord, float& rayYCoord, float& rayDist, float& rayAngle, bool isVertical);

int mapX = 8, mapY = 8, mapS = 64;

int mapW[] = {
    1, 1, 1, 1, 1, 2, 1, 1,
    1, 0, 1, 0, 0, 0, 0, 1,
    1, 0, 1, 0, 0, 0, 0, 1,
    1, 0, 1, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 1, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
};

int All_Textures[]=               //all 32x32 textures
{
 //Checkerboard
 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,
 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,
 0,0,0,0,0,1,1,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,
 0,0,1,1,1,1,1,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,
 0,0,1,1,1,1,1,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,
 0,0,0,0,0,1,1,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,
 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,
 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,

 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 
 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 
 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 
 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 
 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 
 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 
 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 
 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0,

 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,
 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,
 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,
 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,
 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,
 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,
 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,
 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,

 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 
 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 
 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 
 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 
 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 
 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 
 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 
 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 
 //Brick
 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1,
 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1,
 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1,
 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1,
 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1,
 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1,
 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,

 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,
 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,
 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,
 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,
 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,
 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,
 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,

 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1,
 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1,
 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1,
 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1,
 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1,
 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1,
 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,

 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,
 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,
 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,
 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,
 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,
 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,
 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
 //Window
 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 
 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,  
 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,  
 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,  
 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 
 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 
 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,    
       
 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,  
 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,  
 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,  
 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,  
 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 
 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 
 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 

 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,  
 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,  
 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,  
 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,  
 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 
 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 
 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,   
       
 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,  
 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,  
 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,  
 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,  
 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 
 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 
 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 
 //Door
 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,  
 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,  
 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,    
 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,    
 0,0,0,1,1,1,1,1, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 1,1,1,1,1,0,0,0,  
 0,0,0,1,0,0,0,1, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 1,0,0,0,1,0,0,0,  
 0,0,0,1,0,0,0,1, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 1,0,0,0,1,0,0,0,   
 0,0,0,1,0,0,0,1, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 1,0,0,0,1,0,0,0,     

 0,0,0,1,0,0,0,1, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 1,0,0,0,1,0,0,0,  
 0,0,0,1,0,0,0,1, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 1,0,0,0,1,0,0,0,    
 0,0,0,1,0,0,0,1, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 1,0,0,0,1,0,0,0,    
 0,0,0,1,0,0,0,1, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 1,0,0,0,1,0,0,0,   
 0,0,0,1,0,0,0,1, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 1,0,0,0,1,0,0,0,  
 0,0,0,1,0,0,0,1, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 1,0,0,0,1,0,0,0,  
 0,0,0,1,0,0,0,1, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 1,0,0,0,1,0,0,0,  
 0,0,0,1,1,1,1,1, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 1,1,1,1,1,0,0,0,  

 0,0,0,0,0,0,0,0, 0,0,0,0,0,1,0,1, 1,0,1,0,0,0,0,0, 0,0,0,0,0,0,0,0,  
 0,0,0,0,0,0,0,0, 0,0,1,1,1,1,0,1, 1,0,1,1,1,1,0,0, 0,0,0,0,0,0,0,0,   
 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,    
 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,    
 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,  
 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,  
 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,   
 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 
 
 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,  
 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,     
 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,   
 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,   
 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,   
 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,  
 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,   
 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,         
};

void drawMap2D() {
    int x, y, xo, yo;
    for (y = 0; y < mapY; y++) {
        for (x = 0; x < mapX; x++) {

            if (mapW[y * mapX + x] > 0) {
                glColor3f(1.0f, 1.0f, 1.0f); // Set color to white
            }
            else {
                glColor3f(0.0f, 0.0f, 0.0f); // Set color to black
            }

            xo = x * mapS; 
            yo = y * mapS;

            glBegin(GL_TRIANGLES);
            glVertex2i(xo + 1, yo + 1);               // Triangle 1
            glVertex2i(xo + mapS - 1, yo + 1);
            glVertex2i(xo + 1, yo + mapS - 1);

            glVertex2i(xo + mapS - 1, yo + 1);        // Triangle 2
            glVertex2i(xo + mapS - 1, yo + mapS - 1);
            glVertex2i(xo + 1, yo + mapS - 1);
            glEnd();
        }
    }
}    

float dist(float ax, float ay, float bx, float by, float ang) {
    return ( sqrt((bx - ax) * (bx - ax) + (by - ay) * (by - ay)) );
}

void castRay(float& rayXCoord, float& rayYCoord, float& rayDist, float& rayAngle, bool isVertical, int& textureIndex) {
    int depthOfField = 0;
    float xOffset, yOffset;
    float tanValue = (isVertical) ? -tan(rayAngle) : -1 / tan(rayAngle);
    rayDist = 1000000; // Large number to simulate infinity
    textureIndex = 0;  // Initialize texture index
    
    // Horizontal or Vertical ray direction
    if (!isVertical) { // Horizontal check
        if (rayAngle > PI) { // Facing upwards
            rayYCoord = (((int)playerYPos >> 6) << 6) - 0.0001;
            rayXCoord = (playerYPos - rayYCoord) * tanValue + playerXPos;
            yOffset = -64;
            xOffset = -yOffset * tanValue;
        } else if (rayAngle < PI) { // Facing downwards
            rayYCoord = (((int)playerYPos >> 6) << 6) + 64;
            rayXCoord = (playerYPos - rayYCoord) * tanValue + playerXPos;
            yOffset = 64;
            xOffset = -yOffset * tanValue;
        } else { // Ray parallel to X axis
            rayXCoord = playerXPos;
            rayYCoord = playerYPos;
            depthOfField = 8;
        }
    } else { // Vertical check
        if (rayAngle > PI / 2 && rayAngle < 3 * PI / 2) { // Facing left
            rayXCoord = (((int)playerXPos >> 6) << 6) - 0.0001;
            rayYCoord = (playerXPos - rayXCoord) * tanValue + playerYPos;
            xOffset = -64;
            yOffset = -xOffset * tanValue;
        } else if (rayAngle < PI / 2 || rayAngle > 3 * PI / 2) { // Facing right
            rayXCoord = (((int)playerXPos >> 6) << 6) + 64;
            rayYCoord = (playerXPos - rayXCoord) * tanValue + playerYPos;
            xOffset = 64;
            yOffset = -xOffset * tanValue;
        } else { // Ray parallel to Y axis
            rayXCoord = playerXPos;
            rayYCoord = playerYPos;
            depthOfField = 8;
        }
    }

    // Raycasting loop
    while (depthOfField < 8) {
        int mapXPos = (int)(rayXCoord) >> 6; // Convert to grid position
        int mapYPos = (int)(rayYCoord) >> 6;
        int mapIndex = mapYPos * mapX + mapXPos;

        if (mapIndex > 0 && mapIndex < mapX * mapY && mapW[mapIndex] > 0) { // Wall hit
            depthOfField = 8;
            rayDist = dist(playerXPos, playerYPos, rayXCoord, rayYCoord, rayAngle);

            // Assign texture index based on wall hit
            textureIndex = mapW[mapIndex] - 1;  // Adjust for zero-indexing (similar to vmt/hmt logic)
        } else {
            rayXCoord += xOffset;
            rayYCoord += yOffset;
            depthOfField++;
        }
    }
}

void drawRays2D() {
    glColor3f(0,1,1); glBegin(GL_QUADS); glVertex2i(526,  0); glVertex2i(1006,  0); glVertex2i(1006,160); glVertex2i(526,160); glEnd();
    glColor3f(0,0,1); glBegin(GL_QUADS); glVertex2i(526,160); glVertex2i(1006,160); glVertex2i(1006,320); glVertex2i(526,320); glEnd();	 	

    int r;                   // Loop counter for raycasting (possibly for iterating over rays)
    int mapXPos, mapYPos;     // X and Y position of the ray in the map grid
    int mp;                   // Index in the map array, used to check if there's a wall at mapXPos, mapYPos
    int depthOfField;         // Counter for the raycasting depth (how many grid spaces the ray travels)

    float rayXCoord, rayYCoord; // X and Y coordinates of the current ray in the world space
    float rayAngle;           // Angle of the ray in radians (the direction it's cast)
    float xOffset, yOffset;   // X and Y offsets for stepping the ray to the next grid cell
    float rayDistance;        // Distance from the player's position to the point where the ray hits an object (like a wall)


    int numRays = 60;

    rayAngle = playerAngle - DR * 30;
    if (rayAngle < 0) {
        rayAngle += 2 * PI;
    }
    if (rayAngle > 2 * PI) {
        rayAngle -= 2 * PI;
    }

    for (r = 0; r < numRays; r++) {
        int verticalWallTexture = 0, horizontalWallTexture = 0; // Store the texture index for vertical and horizontal wall hits

        if (rayAngle < 0) rayAngle += 2 * PI;
        if (rayAngle > 2 * PI) rayAngle -= 2 * PI; 

        float horizontalRayX, horizontalRayY, horizontalDist;
        castRay(horizontalRayX, horizontalRayY, horizontalDist, rayAngle, false, horizontalWallTexture);

        float verticalRayX, verticalRayY, verticalDist;
        castRay(verticalRayX, verticalRayY, verticalDist, rayAngle, true, verticalWallTexture);

        float shade = 1;
        float textureX, textureY;
        glColor3f(0, 0.8, 0);

        //std::cout << "rayAngle: " << rayAngle << std::endl;

        if (verticalDist < horizontalDist) {
            horizontalWallTexture = verticalWallTexture;
            shade = 0.5f;
            rayXCoord = verticalRayX;
            rayYCoord = verticalRayY;
            rayDistance = verticalDist;
            textureX = (int)(rayYCoord / 2.0) % 32;

            if (rayAngle > PI / 2 && rayAngle < 3 * PI / 2) {
               textureX = 31 - textureX; 
               //std::cout << "Looking left" << std::endl;
            }
            glColor3f(0.9, 0.0, 0.0);
        } else {
            rayXCoord = horizontalRayX;
            rayYCoord = horizontalRayY;
            rayDistance = horizontalDist;
            textureX = (int)(rayXCoord / 2.0) % 32;

            if (rayAngle < PI) {
                textureX = 31 - textureX;
                //std::cout << "Looking down" << std::endl;
            }
            glColor3f(0.7, 0.0, 0.0);
        }

        glLineWidth(2);
        glBegin(GL_LINES);
        glVertex2f(playerXPos, playerYPos);
        glVertex2f(rayXCoord, rayYCoord);
        glEnd();

        //Draw 3D Walls
        float correctedAngle = playerAngle - rayAngle; //represents the difference between the player’s angle and the ray angle.
        if (correctedAngle < 0) {
            correctedAngle += 2 * PI;
        } else if (correctedAngle > 2 * PI) {
            correctedAngle -= 2 * PI;
        }

        rayDistance = rayDistance * cos(correctedAngle); // fixes fisheyes

        int lineHeight = (mapS * 320) / rayDistance; //lines height
        if (lineHeight <= 0) lineHeight = 1;

        float textureStepY = 32.0 / (float)lineHeight;
        float textureYOffset = 0;

        if (lineHeight > 320) {
            textureYOffset = (lineHeight - 320) / 2.0;
            lineHeight = 320;
        }

        float lineOff = 160 - (lineHeight / 2.0f);

        textureY = textureYOffset * textureStepY + horizontalWallTexture * 32;

        for (int y = 0; y < lineHeight; y++) {
            float textureColor = All_Textures[(int)(textureY) * 32 + (int)(textureX)] * shade;

            glPointSize(8);
            glColor3f(textureColor, textureColor, textureColor);
            glBegin(GL_POINTS);
            glVertex2f(r * 8 + 530, y + lineOff);
            glEnd();

            textureY += textureStepY;
        }

        rayAngle += DR;
        if (rayAngle < 0) {
            rayAngle += 2 * PI;
        }
        if (rayAngle > 2 * PI) {
            rayAngle -= 2 * PI;
        }
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
    drawMap2D();
    drawPlayer(); // Draw the player
    drawRays2D();
}

// Key state management
bool keys[1024] = { false }; // Array to track key states
float movementSpeed = 50.0f;
float rotationSpeed = 2.0f;

bool checkCollision(float playerPosX, float playerPosY) {
    int mapTileX = static_cast<int>(playerPosX / mapS);
    int mapTileY = static_cast<int>(playerPosY / mapS);

    // Check if the player is within the bounds of the map and if the tile is walkable (0 means walkable)
    return (mapTileX < 0 || mapTileX >= mapX || mapTileY < 0 || mapTileY >= mapY || mapW[mapTileY * mapX + mapTileX] != 0);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    // Update key state based on the action
    if (action == GLFW_PRESS) {
        keys[key] = true;
    } else if (action == GLFW_RELEASE) {
        keys[key] = false;
    }
}

void update_player_position(float deltaTime) {
    // Update rotation based on key states
    if (keys[GLFW_KEY_A]) {
        playerAngle -= rotationSpeed * deltaTime;
        if (playerAngle < 0) {
            playerAngle += 2 * PI;  // Wrap around
        }
    }
    if (keys[GLFW_KEY_D]) {
        playerAngle += rotationSpeed * deltaTime;
        if (playerAngle > 2 * PI) {
            playerAngle -= 2 * PI;
        }
    }

    // Calculate direction based on the updated angle
    playerDirX = cos(playerAngle);
    playerDirY = sin(playerAngle);

    float nextPlayerPosX = playerXPos;
    float nextPlayerPosY = playerYPos;

    // Move forward or backward
    if (keys[GLFW_KEY_W]) {
        nextPlayerPosX += playerDirX * movementSpeed * deltaTime; // Move forward
        nextPlayerPosY += playerDirY * movementSpeed * deltaTime;
    }
    if (keys[GLFW_KEY_S]) {
        nextPlayerPosX -= playerDirX * movementSpeed * deltaTime; // Move backward
        nextPlayerPosY -= playerDirY * movementSpeed * deltaTime;
    }

    //std::cout << "FPS: " << 1 / deltaTime << std::endl;

    // Check collision and update position
    if (!checkCollision(nextPlayerPosX, playerYPos)) {
        playerXPos = nextPlayerPosX; // Update x position if no collision
    }
    if (!checkCollision(playerXPos, nextPlayerPosY)) {
        playerYPos = nextPlayerPosY; // Update y position if no collision
    }

    //std::cout << "Player angle (pa): " << playerAngle << std::endl;

    if (keys[GLFW_KEY_E]) {
        int xOffset = 0;
        int yOffset = 0;

        if (playerDirX < 0) {
            xOffset = -25;
        } else {
            xOffset = 25;
        }
        if (playerDirY < 0) {
            yOffset = -25;
        } else {
            yOffset = 25;
        }

        
    }
}

void init() {
    glClearColor(0.3f, 0.3f, 0.3f, 0.0f); // Set clear color
    glOrtho(0, 1024, 512, 0, -1, 1);      // Set up orthographic projection
    playerXPos = 300.0f; playerYPos = 300.0f;              // Set player position
    playerAngle = 0.0f;
    playerDirX = cos(playerAngle) * 5;
    playerDirY = sin(playerAngle) * 5;
} 

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        printf("Failed to initialize GLFW\n");
        return -1;
    }

    // Set window hints for OpenGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE); // Use compatibility profile

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(1024, 512, "OpenGL Window", nullptr, nullptr);
    if (!window) {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Enable V-Sync
    glfwSwapInterval(1); // Wait for vertical refresh to prevent tearing

    // Load OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to initialize GLAD\n");
        return -1;
    }

    // Initialize OpenGL settings
    init();

    // Set the key callback function
    glfwSetKeyCallback(window, key_callback);

    // Variables for timing
    float lastFrameTime = 0.0f;

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Calculate delta time
        float currentFrameTime = glfwGetTime();
        float deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;

        // Update player position based on key states and delta time
        update_player_position(deltaTime);

        // Render the scene
        display(); // Call the display function to render your objects

        // Swap front and back buffers (double buffering)
        glfwSwapBuffers(window); // Swap buffers to show the rendered frame

        // Poll for and process events
        glfwPollEvents(); // Check for user input and events
    }

    // Clean up and terminate GLFW
    glfwTerminate();
    return 0;
}


