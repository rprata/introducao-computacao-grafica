#include <GL/glut.h>
#include <GL/glu.h>
#include <cmath>
#include <cstdio>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#define PI 3.1415926f

void drawCircle(float cx, float cy, float r, int num_segments) 
{ 
    glColor3f(0.0, 0.0, 0.0); 
    glBegin(GL_POLYGON); 
    for(int i = 0; i < num_segments; i++) 
    { 
        float theta = 2.0f * PI * (float )i / (float) num_segments;

        float x = r * cosf(theta);
        float y = r * sinf(theta);

        glVertex2f(x + cx, y + cy);

    } 
    glEnd(); 
}

void myReshape(int w, int h) 
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w >= h) //permite que a circunferencia fique no centro do circulo, independente do formato da window
        gluOrtho2D ((float) -w/h, (float) w/h, -1.0, 1.0);
    else
        gluOrtho2D (-1.0, 1.0,(float) -h/w, (float) h/w);    
}


void myDisplay(void)
{
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    drawCircle(0.0, 0.0, 1, 100);
    glutSwapBuffers(); 

}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    glutInitWindowPosition(0, 0);
    
    glutCreateWindow(argv[0]);
    glutDisplayFunc(myDisplay);
    glutReshapeFunc(myReshape);

    glutMainLoop();
    
    return 0;
}