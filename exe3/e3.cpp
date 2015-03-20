#include <GL/glut.h>
#include <GL/glu.h>
#include <cmath>
#include <cstdio>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#define NUM_SEG 50
#define PI 3.1415926f

#define X_MAX 50

float ratio = 0.5;

int window_width;
int window_height;
float screen_ratio;

float vertex_array[2 * NUM_SEG];

void createVertexArray()
{
    for (int i = 0; i < NUM_SEG; i++)
    {
        float theta = 2.0f * PI * (float )i / (float) NUM_SEG;
        vertex_array[2 * i] = cosf(theta);
        vertex_array[2 * i + 1] = sinf(theta);
    }
}

void drawCircle(float cx, float cy, float r) 
{
    glEnableClientState(GL_VERTEX_ARRAY);
    glPushMatrix();
    glVertexPointer(2, GL_FLOAT, 2 * sizeof(float), vertex_array);
    glTranslatef(cx, cy, 0);
    glScalef(r, r, 1);
    glDrawArrays(GL_TRIANGLE_FAN, 0, NUM_SEG);
    glPopMatrix();
    glDisableClientState(GL_VERTEX_ARRAY);
}

void drawMultipleCircles(float cx, float cy, float r)
{
    drawCircle(cx, cy, r);

    if (window_width >= window_height)
    {
        if ((cx - 4*r >= -screen_ratio) && (cx + 4*r <= screen_ratio))
        {
            if (cx == 0)
            {
                drawMultipleCircles(cx - 3*r, cy, r);
                drawMultipleCircles(cx + 3*r, cy, r); 
            }
            else if (cx < 0)
                drawMultipleCircles(cx - 3*r, cy, r);
            else if (cx > 0)
                drawMultipleCircles(cx + 3*r, cy, r);
        }

        if ((cy - 4*r >= -1.0f) && (cy + 4*r <= 1.0f))
        {
            if (cy == 0)
            {
                drawMultipleCircles(cx, cy - 3*r, r);
                drawMultipleCircles(cx, cy + 3*r, r);
            }
            else if (cy < 0)
                drawMultipleCircles(cx, cy - 3*r, r);
            else if (cy > 0)
                drawMultipleCircles(cx, cy + 3*r, r);
        }
    }
    else
    {
        if ((cx - 4*r >= -1.0f) && (cx + 4*r <= 1.0f))
        {
            if (cx == 0)
            {
                drawMultipleCircles(cx - 3*r, cy, r);
                drawMultipleCircles(cx + 3*r, cy, r); 
            }
            else if (cx < 0)
                drawMultipleCircles(cx - 3*r, cy, r);
            else if (cx > 0)
                drawMultipleCircles(cx + 3*r, cy, r);
        }

        if ((cy - 4*r >= -screen_ratio) && (cy + 4*r <= screen_ratio))
        {
            if (cy == 0)
            {
                drawMultipleCircles(cx, cy - 3*r, r);
                drawMultipleCircles(cx, cy + 3*r, r);
            }
            else if (cy < 0)
                drawMultipleCircles(cx, cy - 3*r, r);
            else if (cy > 0)
                drawMultipleCircles(cx, cy + 3*r, r);
        }   
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
    glColor3f(0.0, 0.0, 0.0);  
    drawMultipleCircles(0.0, 0.0, ratio);
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
    createVertexArray();
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