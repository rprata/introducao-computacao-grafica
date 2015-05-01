#include "utils.h"
#include "noise.h"

#define		RESOLUTION 256

static GLuint	texture;

unsigned char total_texture[4 * 256 * 256];
unsigned char alpha_texture[256 * 256];
unsigned char caustic_texture[3 * 256 * 256];
unsigned char image_texture[3 * 256 * 256];
unsigned int i;
unsigned int j;
unsigned int length;
unsigned int indice;
unsigned int preindice;

static int	left_click = GLUT_UP;
static int	right_click = GLUT_UP;
static int	wire_frame = 0;
static int	normals = 0;
static int	xold = 0;
static int	yold = 0;
static float	rotate_x = 30;
static float	rotate_y = 15;
static float	translate_z = 4;

static float	surface[6 * RESOLUTION * (RESOLUTION + 1)];
static float	normal[6 * RESOLUTION * (RESOLUTION + 1)];


static float z (const float x, const float y, const float t)
{
  const float x2 = x - 3;
  const float y2 = y + 1;
  const float xx = x2 * x2;
  const float yy = y2 * y2;
  return ((2 * sinf (20 * sqrtf (xx + yy) - 4 * t) +
	   Noise (10 * x, 10 * y, t, 0)) / 200);
}

void init() 
{
  glClearColor(0.529f, 0.808f, 0.8f, 0.980f);
}

void initTexture(char * filename) {

  load_texture (filename, image_texture, GL_RGB, 256);

  GLuint textureID;
  glBindTexture (GL_TEXTURE_2D, textureID);
  gluBuild2DMipmaps (GL_TEXTURE_2D, GL_RGB, 256, 256, GL_RGB, GL_UNSIGNED_BYTE,  image_texture);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glEnable (GL_TEXTURE_GEN_S);
  glEnable (GL_TEXTURE_GEN_T);
  glTexGeni (GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
  glTexGeni (GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
}

void light()
{
  // Ilumination
  GLfloat ambientIntensity[4] = {1.0f, 1.0f, 1.0f, 1.0f};             // Ambient light color
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientIntensity);
  glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 1.0f);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_NORMALIZE);
  glEnable(GL_LIGHTING);                                          // set up light 0 properties
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_LIGHT0);
  GLfloat lt0Intensity[4] = {1.5, 1.5, 1.5, 1.0};         // white
  glLightfv(GL_LIGHT0, GL_DIFFUSE, lt0Intensity);
  glLightfv(GL_LIGHT0, GL_SPECULAR, lt0Intensity);
  GLfloat lt0Position[4] = {-2.0f, 10.0f, 2.0f, 1.0f};           // location
  glLightfv(GL_LIGHT0, GL_POSITION, lt0Position);
  glLightf (GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.1f);         // attenuation params (a,b,c)
  // glLightf (GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.0);
  // glLightf (GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0);
  //glFrontFace(GL_CCW);
}


void drawGround() 
{
  initTexture("sand.jpg");
  glEnable (GL_TEXTURE_2D);
  glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
  glColor3f (1, 1, 1);
  glBegin (GL_QUADS);

  glNormal3f(0, -1, 0);
  
  glTexCoord2f(-1.0f, -1.0f);
  glTexCoord2f(-1.0f, 1.0f); 
  glTexCoord2f(1.0f, -1.0f); 
  glTexCoord2f(1.0f, 1.0f);

  glVertex3f (-1, 0, -1); 
  glVertex3f (-1, 0,  1);
  glVertex3f ( 1, 0,  1);
  glVertex3f ( 1, 0, -1);

  glVertex3f (-1, 0, -1);
  glVertex3f ( 1, 0, -1);
  glVertex3f ( 1, 0.1, -1);
  glVertex3f (-1, 0.1, -1);

  glVertex3f (-1, 0, 1);
  glVertex3f ( 1, 0, 1);
  glVertex3f ( 1, 0.1, 1);
  glVertex3f (-1, 0.1, 1);
  
  glVertex3f (1, 0, -1);
  glVertex3f (1, 0, 1);
  glVertex3f (1, 0.1, 1);
  glVertex3f (1, 0.1, -1);
    
  glVertex3f (-1, 0, -1);
  glVertex3f (-1, 0, 1);
  glVertex3f (-1, 0.1, 1);
  glVertex3f (-1, 0.1, -1);
  glEnd();

  glTranslatef (0, 0.1, 0);
}

void drawWater()
{
  /* Render wireframe? */
  if (wire_frame != 0)
    glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);

  /* The water */
  load_texture ("alpha.jpg", alpha_texture, GL_ALPHA, 256);
  load_texture ("reflection.jpg", caustic_texture, GL_RGB, 256);

  for (i = 0; i < 256 * 256; i++)
  {
    total_texture[4 * i] = caustic_texture[3 * i];
    total_texture[4 * i + 1] = caustic_texture[3 * i + 1];
    total_texture[4 * i + 2] = caustic_texture[3 * i + 2];
    total_texture[4 * i + 3] = alpha_texture[i];
  }

  glBindTexture (GL_TEXTURE_2D, texture);
  gluBuild2DMipmaps (GL_TEXTURE_2D, GL_RGBA, 256, 256, GL_RGBA, GL_UNSIGNED_BYTE,  total_texture);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glEnable (GL_TEXTURE_GEN_S);
  glEnable (GL_TEXTURE_GEN_T);
  glTexGeni (GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
  glTexGeni (GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);

  glEnable (GL_TEXTURE_2D);
  glColor3f (1, 1, 1);
  glEnableClientState (GL_NORMAL_ARRAY);
  glEnableClientState (GL_VERTEX_ARRAY);
  glNormalPointer (GL_FLOAT, 0, normal);
  glVertexPointer (3, GL_FLOAT, 0, surface);
  for (i = 0; i < RESOLUTION; i++)
    glDrawArrays (GL_TRIANGLE_STRIP, i * length, length);

  /* Draw normals? */
  if (normals != 0)
    {
      glDisable (GL_TEXTURE_2D);
      glColor3f (1, 0, 0);
      glBegin (GL_LINES);
      for (j = 0; j < RESOLUTION; j++)
  for (i = 0; i <= RESOLUTION; i++)
    {
      indice = 6 * (i + j * (RESOLUTION + 1));
      glVertex3fv (&(surface[indice]));
      glVertex3f (surface[indice] + normal[indice] / 50,
      surface[indice + 1] + normal[indice + 1] / 50,
      surface[indice + 2] + normal[indice + 2] / 50);
    }

      glEnd ();
    }
}

void DisplayFunc (void)
{
  const float t = glutGet (GLUT_ELAPSED_TIME) / 1000.;
  const float delta = 2. / RESOLUTION;
  length = 2 * (RESOLUTION + 1);
  const float xn = (RESOLUTION + 1) * delta + 1;
  unsigned int i;
  unsigned int j;
  float x;
  float y;

  /* Yes, I know, this is quite ugly... */
  float v1x;
  float v1y;
  float v1z;

  float v2x;
  float v2y;
  float v2z;

  float v3x;
  float v3y;
  float v3z;

  float vax;
  float vay;
  float vaz;

  float vbx;
  float vby;
  float vbz;

  float nx;
  float ny;
  float nz;

  float l;

  // Ilumination
  light();

  init();

  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glLoadIdentity ();
  glTranslatef (0, 0, -translate_z);
  glRotatef (rotate_y, 1, 0, 0);
  glRotatef (rotate_x, 0, 1, 0);

  for (j = 0; j < RESOLUTION; j++)
    {
      y = (j + 1) * delta - 1;
      for (i = 0; i <= RESOLUTION; i++)
	{
	  indice = 6 * (i + j * (RESOLUTION + 1));

	  x = i * delta - 1;
	  surface[indice + 3] = x;
	  surface[indice + 4] = z (x, y, t);
	  surface[indice + 5] = y;
	  if (j != 0)
	    {
	      preindice = 6 * (i + (j - 1) * (RESOLUTION + 1));
	      surface[indice] = surface[preindice + 3];
	      surface[indice + 1] = surface[preindice + 4];
	      surface[indice + 2] = surface[preindice + 5];
	    }
	  else
	    {
	      surface[indice] = x;
	      surface[indice + 1] = z (x, -1, t);
	      surface[indice + 2] = -1;
	    }
	}
    }

  /* Normals */
  for (j = 0; j < RESOLUTION; j++)
    for (i = 0; i <= RESOLUTION; i++)
      {
	indice = 6 * (i + j * (RESOLUTION + 1));

	v1x = surface[indice + 3];
	v1y = surface[indice + 4];
	v1z = surface[indice + 5];

	v2x = v1x;
	v2y = surface[indice + 1];
	v2z = surface[indice + 2];

	if (i < RESOLUTION)
	  {
	    v3x = surface[indice + 9];
	    v3y = surface[indice + 10];
	    v3z = v1z;
	  }
	else
	  {
	    v3x = xn;
	    v3y = z (xn, v1z, t);
	    v3z = v1z;
	  }

	vax =  v2x - v1x;
	vay =  v2y - v1y;
	vaz =  v2z - v1z;

	vbx = v3x - v1x;
	vby = v3y - v1y;
	vbz = v3z - v1z;

	nx = (vby * vaz) - (vbz * vay);
	ny = (vbz * vax) - (vbx * vaz);
	nz = (vbx * vay) - (vby * vax);

	l = sqrtf (nx * nx + ny * ny + nz * nz);
	if (l != 0)
	  {
	    l = 1 / l;
	    normal[indice + 3] = nx * l;
	    normal[indice + 4] = ny * l;
	    normal[indice + 5] = nz * l;
	  }
	else
	  {
	    normal[indice + 3] = 0;
	    normal[indice + 4] = 1;
	    normal[indice + 5] = 0;
	  }


	if (j != 0)
  {
    /* Values were computed during the previous loop */
    preindice = 6 * (i + (j - 1) * (RESOLUTION + 1));
    normal[indice] = normal[preindice + 3];
    normal[indice + 1] = normal[preindice + 4];
    normal[indice + 2] = normal[preindice + 5];
  }
	else
	  {
/* 	    v1x = v1x; */
	    v1y = z (v1x, (j - 1) * delta - 1, t);
	    v1z = (j - 1) * delta - 1;

/* 	    v3x = v3x; */
	    v3y = z (v3x, v2z, t);
	    v3z = v2z;

	    vax = v1x - v2x;
	    vay = v1y - v2y;
	    vaz = v1z - v2z;

	    vbx = v3x - v2x;
	    vby = v3y - v2y;
	    vbz = v3z - v2z;

	    nx = (vby * vaz) - (vbz * vay);
	    ny = (vbz * vax) - (vbx * vaz);
	    nz = (vbx * vay) - (vby * vax);

	    l = sqrtf (nx * nx + ny * ny + nz * nz);
	    if (l != 0)
	      {
		l = 1 / l;
		normal[indice] = nx * l;
		normal[indice + 1] = ny * l;
		normal[indice + 2] = nz * l;
	      }
	    else
	      {
		normal[indice] = 0;
		normal[indice + 1] = 1;
		normal[indice + 2] = 0;
	      }
	  }
      }

  /* The ground */
  drawGround();

  /* the water */
  drawWater();

  glFlush ();
  glutSwapBuffers ();
  glutPostRedisplay ();
}

void ReshapeFunc (int width, int height)
{
  glMatrixMode(GL_PROJECTION);

  glLoadIdentity ();
  gluPerspective (20, width / (float) height, 0.1, 15);
  glViewport (0, 0, width, height);

  glMatrixMode(GL_MODELVIEW);
  glutPostRedisplay();
}

void KeyboardFunc (unsigned char key, int x, int y)
{
  xold = x; /* Has no effect: just to avoid a warning */
  yold = y;
  switch (key)
    {
    case 'q':
    case 'Q':
    case 27:
      exit (0);
      break;
    case 'f':
    case 'F':
    case 'p':
    case 'P':
      wire_frame = 0;
      break;
    case 'l':
    case 'L':
      wire_frame = 1;
      break;
    case 'n':
    case 'N':
      if (0 == normals)
	normals = 1;
      else
	normals = 0;
      break;
    }
}

void MouseFunc (int button, int state, int x, int y)
{
  if (GLUT_LEFT_BUTTON == button)
    left_click = state;
  if (GLUT_RIGHT_BUTTON == button)
    right_click = state;
  xold = x;
  yold = y;
}

void		MotionFunc (int x, int y)
{
  if (GLUT_DOWN == left_click)
    {
      rotate_y = rotate_y + (y - yold) / 5.0;
      rotate_x = rotate_x + (x - xold) / 5.0;
      if (rotate_y > 90)
	rotate_y = 90;
      if (rotate_y < -90)
	rotate_y = -90;
      glutPostRedisplay ();
    }
  if (GLUT_DOWN == right_click)
    {
      rotate_x = rotate_x + (x - xold) / 5.;
      translate_z = translate_z +
	(yold - y) / 50.;
      if (translate_z < 0.5)
	translate_z = 0.5;
      if (translate_z > 10)
	translate_z = 10;
      glutPostRedisplay ();
    }
  xold = x;
  yold = y;
}


int		main (int narg, char ** args)
{
  InitNoise ();

  glutInit (&narg, args);
  glutInitDisplayMode (GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize (1280, 720);
  glutCreateWindow ("My Island");

  glClearColor (0, 0, 0, 0);
  glEnable (GL_DEPTH_TEST);
  glEnable (GL_BLEND);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

  glGenTextures (1, &texture);

  glutDisplayFunc (&DisplayFunc);
  glutReshapeFunc (&ReshapeFunc);
  glutKeyboardFunc (&KeyboardFunc);
  glutMouseFunc (&MouseFunc);
  glutMotionFunc (&MotionFunc);

  glutMainLoop ();

  return 0;
}
