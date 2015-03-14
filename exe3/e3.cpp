#include <GL/glut.h>
#include <GL/glu.h>
#include <cmath>
#include <cstdio>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#define PI 3.1415926f

#define X_MAX 100

float ratio = 0.5;

int window_width;
int window_height;
float screen_ratio;

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

void drawMultipleCircles(float cx, float cy, float r, int num_segments)
{
    drawCircle(cx, cy, r, num_segments);
    if ((cx - 4*r >= -screen_ratio) && (cx + 4*r <= screen_ratio))
    {
        if (cx == 0)
        {
            drawMultipleCircles(cx - 3*r, cy, r, num_segments);
            drawMultipleCircles(cx + 3*r, cy, r, num_segments); 
        }
        else if (cx < 0)
            drawMultipleCircles(cx - 3*r, cy, r, num_segments);
        else if (cx > 0)
            drawMultipleCircles(cx + 3*r, cy, r, num_segments);
    }

    if ((cy - 4*r >= -1.0f) && (cy + 4*r <= 1.0f))
    {
        if (cy == 0)
        {
            drawMultipleCircles(cx, cy - 3*r, r, num_segments);
            drawMultipleCircles(cx, cy + 3*r, r, num_segments);
        }
        else if (cy < 0)
            drawMultipleCircles(cx, cy - 3*r, r, num_segments);
        else if (cy > 0)
            drawMultipleCircles(cx, cy + 3*r, r, num_segments);
    }
}

void myReshape(int w, int h) 
{
    window_width = w;
    window_height = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w >= h) //permite que a circunferencia fique no centro do circulo, independente do formato da window
    {
        screen_ratio = (float) w/h;
        gluOrtho2D (-screen_ratio, screen_ratio, -1.0, 1.0);
    }
    else
    {
        screen_ratio = (float) h/w;
        gluOrtho2D (-1.0, 1.0, -screen_ratio, screen_ratio);    
    }
}


void myDisplay(void)
{
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    drawMultipleCircles(0.0, 0.0, ratio, 50);
    glutSwapBuffers(); 

}

void myMouseMotion(int x, int y)
{
    if (x > X_MAX && x < window_width) 
    {
        ratio = (float) x / (float) window_width;
        glutPostRedisplay();
    }
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
    glutPassiveMotionFunc(myMouseMotion);

    glutMainLoop();
    
    return 0;
}