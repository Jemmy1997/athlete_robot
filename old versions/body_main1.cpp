#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>
#include "glm.h"
#include "imageloader.h"

static int shoulder = 0, lshoulder2 = 0, rshoulder2 = 0, shoulder2 = 0, elbow = 0, elbow2 = 0, rfingerBase = 0, rfingerUp = 0;
static int lfingerBase = 0, lfingerUp = 0, rhip = 0, rhip2 = 0, rknee = 0, lknee = 0, lhip = 0, lhip2 = 0, trunk = 0;
double eye[] = {0, 0, -20};
double center[] = {0, 0, 1};
double up[] = {0, 1, 0};
double direction[] = {1, 0, 0};
double current[] = {0, 0, 0};
double speed = .02;
GLfloat angle = 180;  /* in degrees */
GLfloat angle2 = 90;  /* in degrees */
GLfloat angle3 = 180; /* in degrees */

// RGBA
GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 0.0 };
GLfloat light_diffuse[] = { 0.5, 0.5, 0.5,1.0 };
GLfloat light_specular[] = {0.5,0.5, 0.5, 1.0 };
// x , y, z, w
GLfloat light_position[] = {0.5,5.0, 0.0, 1.0 };
GLfloat lightPos1[] = {-10.5,-100.0,-20.0, 1.0 };
// Material Properties
GLfloat mat_amb_diff[] = {0.643, 0.753, 0.934, 1.0 };
GLfloat mat_specular[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat shininess[] = {100 };  

//Makes the image into a texture, and returns the id of the texture
GLuint loadTexture(Image* image) {
	GLuint textureId;
	glGenTextures(1, &textureId); //Make room for our texture
	glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit
	//Map the image to the texture
	glTexImage2D(GL_TEXTURE_2D,                //Always GL_TEXTURE_2D
		0,                            //0 for now
		GL_RGB,                       //Format OpenGL uses for image
		image->width, image->height,  //Width and height
		0,                            //The border of the image
		GL_RGB, //GL_RGB, because pixels are stored in RGB format
		GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
		//as unsigned numbers
		image->pixels);               //The actual pixel data
	return textureId; //Returns the id of the texture
}

GLuint _textureId_wood; //The id of the texture
GLuint _textureId_stone; //The id of the texture
GLuint _textureId_cement; //The id of the texture
GLuint _textureId_ceramics; //The id of the texture
GLuint _textureId;
void init(void)
{
	Image* image = loadBMP("Texture/wood.bmp");
	_textureId_wood = loadTexture(image);
	_textureId=_textureId_wood;
	delete image;
	image = loadBMP("Texture/stone.bmp");
	_textureId_stone = loadTexture(image);
	delete image;
	image = loadBMP("Texture/cement.bmp");
	_textureId_cement = loadTexture(image);
	delete image;
	image = loadBMP("Texture/ceramics.bmp");
	_textureId_ceramics = loadTexture(image);
	delete image;

	GLfloat lightColor1[] = {1.0f, 1.0f,  1.0f, 1.0f };
	// Turn on the power
	glEnable(GL_LIGHTING);
	// Flip light switch
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	// assign light parameters
	glEnable(GL_COLOR_MATERIAL);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
	// Material Properties         
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,mat_amb_diff);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor1);
	glEnable(GL_NORMALIZE);
	//Enable smooth shading
	glShadeModel(GL_SMOOTH);
	// Enable Depth buffer
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	gluPerspective(65.0, (GLfloat)1024 / (GLfloat)869, 1.0, 60.0);

}

void crossProduct(double a[], double b[], double c[])
{
	c[0] = a[1] * b[2] - a[2] * b[1];
	c[1] = a[2] * b[0] - a[0] * b[2];
	c[2] = a[0] * b[1] - a[1] * b[0];
}

void normalize(double a[])
{
	double norm;
	norm = a[0] * a[0] + a[1] * a[1] + a[2] * a[2];
	norm = sqrt(norm);
	a[0] /= norm;
	a[1] /= norm;
	a[2] /= norm;
}

void rotatePoint(double a[], double theta, double p[])
{

	double temp[3];
	temp[0] = p[0];
	temp[1] = p[1];
	temp[2] = p[2];

	temp[0] = -a[2] * p[1] + a[1] * p[2];
	temp[1] = a[2] * p[0] - a[0] * p[2];
	temp[2] = -a[1] * p[0] + a[0] * p[1];

	temp[0] *= sin(theta);
	temp[1] *= sin(theta);
	temp[2] *= sin(theta);

	temp[0] += (1 - cos(theta)) * (a[0] * a[0] * p[0] + a[0] * a[1] * p[1] + a[0] * a[2] * p[2]);
	temp[1] += (1 - cos(theta)) * (a[0] * a[1] * p[0] + a[1] * a[1] * p[1] + a[1] * a[2] * p[2]);
	temp[2] += (1 - cos(theta)) * (a[0] * a[2] * p[0] + a[1] * a[2] * p[1] + a[2] * a[2] * p[2]);

	temp[0] += cos(theta) * p[0];
	temp[1] += cos(theta) * p[1];
	temp[2] += cos(theta) * p[2];

	p[0] = temp[0];
	p[1] = temp[1];
	p[2] = temp[2];
}

void Left()
{
	rotatePoint(up, -.2, eye);
}

void Right()
{
	rotatePoint(up, .2, eye);
}

void Up()
{
	crossProduct(up, eye, current);
	normalize(current);
	rotatePoint(current, -.2, up);
	rotatePoint(current, -.2, eye);
}

void Down()
{
	crossProduct(up, eye, current);
	normalize(current);
	rotatePoint(current, .2, up);
	rotatePoint(current, .2, eye);
}

void moveForward()
{
	direction[0] = center[0] - eye[0];
	direction[1] = center[1] - eye[1];
	direction[2] = center[2] - eye[2];
	eye[0] += direction[0] * speed;
	eye[1] += direction[1] * speed;
	eye[2] += direction[2] * speed;

	center[0] += direction[0] * speed;
	center[1] += direction[1] * speed;
	center[2] += direction[2] * speed;
}

void moveBack()
{
	direction[0] = center[0] - eye[0];
	direction[1] = center[1] - eye[1];
	direction[2] = center[2] - eye[2];
	eye[0] -= direction[0] * speed;
	eye[1] -= direction[1] * speed;
	eye[2] -= direction[2] * speed;

	center[0] -= direction[0] * speed;
	center[1] -= direction[1] * speed;
	center[2] -= direction[2] * speed;
}

void draw_floor(void)
{
	glPushMatrix();
	glTranslatef(0, -9.5, 0.0);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBegin(GL_QUADS);

	glNormal3f(0.0,-1.0,0.0);
	glTexCoord2f(0.0f, 0.0f);
	glColor3f(1, 1, 1);
	glVertex3f(-100,0,-100);
	glTexCoord2f(4.0f,  0.0f);
	glVertex3f(100,0,-100);
	glTexCoord2f(4.0f, 4.0f);
	glVertex3f(100,0,100);
	glTexCoord2f(0.0f, 4.0f);
	glVertex3f(-100,0,100);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

}

void draw_right_arm(void)
{
	glPushMatrix();

	glTranslatef(-3, 2.5, 0.0);
	glRotatef(angle2, 0.0, 0.0, 1.0);
	glRotatef(angle, 0.0, 1.0, 0.0);
	glRotatef((GLfloat)rshoulder2, 0.0, -1, 0);
	glRotatef((GLfloat)shoulder2, 0.0, 0, 1);
	glTranslatef(1, 0, 0);
	glPushMatrix();
	glScalef(2.0, 0.8, 1.0);
	glColor3f(1, 0, 0.2);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -0.3, 0);
	glutSolidSphere(0.6, 50, 50);
	glPopMatrix();

	glTranslatef(1, 0, 0.0);
	glRotatef((GLfloat)elbow2, 0.0, 0.0, 1.0);
	glTranslatef(1.0, 0.0, 0.0);
	glPushMatrix();
	glScalef(2.0, 0.6, 1.0);
	glColor3f(0.9, 0.8, 1.0);
	glutSolidCube(1.0);
	glPopMatrix();

	//Draw finger flang c
	glPushMatrix();
	glTranslatef(1.0, 0.3, 0.0);
	glRotatef((GLfloat)rfingerBase, 0.0, 0.0, 1.0);
	glTranslatef(0.15, 0.0, 0.0);
	glPushMatrix();
	glScalef(0.3, 0.1, 0.1);
	glutSolidCube(1);
	glPopMatrix();

	//Draw finger flang d
	glTranslatef(0.15, 0.0, 0.0);
	glRotatef((GLfloat)rfingerUp, 0.0, 0.0, 1.0);
	glTranslatef(0.15, 0.0, 0.0);
	glPushMatrix();
	glScalef(0.3, 0.1, 0.1);
	glutSolidCube(1);
	glPopMatrix();
	glPopMatrix();

	//Draw finger flang a
	glPushMatrix();
	glTranslatef(1.0, 0.3, .2);
	glRotatef((GLfloat)rfingerBase, 0.0, 0.0, 1.0);
	glTranslatef(0.15, 0.0, 0.0);
	glPushMatrix();
	glScalef(0.3, 0.1, 0.1);
	glutSolidCube(1);
	glPopMatrix();

	//Draw finger flang b
	glTranslatef(0.15, 0.0, 0.0);
	glRotatef((GLfloat)rfingerUp, 0.0, 0.0, 1.0);
	glTranslatef(0.15, 0.0, 0.0);
	glPushMatrix();
	glScalef(0.3, 0.1, 0.1);
	glutSolidCube(1);
	glPopMatrix();
	glPopMatrix();

	//Draw finger flang e
	glPushMatrix();
	glTranslatef(1.0, 0.3, -.2);
	glRotatef((GLfloat)rfingerBase, 0.0, 0.0, 1.0);
	glTranslatef(0.15, 0.0, 0.0);
	glPushMatrix();
	glScalef(0.3, 0.1, 0.1);
	glutSolidCube(1);
	glPopMatrix();

	//Draw finger flang f
	glTranslatef(0.15, 0.0, 0.0);
	glRotatef((GLfloat)rfingerUp, 0.0, 0.0, 1.0);
	glTranslatef(0.15, 0.0, 0.0);
	glPushMatrix();
	glScalef(0.3, 0.1, 0.1);
	glutSolidCube(1);
	glPopMatrix();
	glPopMatrix();

	//Draw finger flang g
	glPushMatrix();
	glTranslatef(1.0, 0.3, -.4);
	glRotatef((GLfloat)rfingerBase, 0.0, 0.0, 1.0);
	glTranslatef(0.15, 0.0, 0.0);
	glPushMatrix();
	glScalef(0.3, 0.1, 0.1);
	glutSolidCube(1);
	glPopMatrix();

	//Draw finger flang h
	glTranslatef(0.15, 0.0, 0.0);
	glRotatef((GLfloat)rfingerUp, 0.0, 0.0, 1.0);
	glTranslatef(0.15, 0.0, 0.0);
	glPushMatrix();
	glScalef(0.3, 0.1, 0.1);
	glutSolidCube(1);
	glPopMatrix();
	glPopMatrix();

	glPopMatrix();
}

void draw_left_arm(void)
{
	glPushMatrix();

	glTranslatef(3, 2.5, 0.0);
	glRotatef(angle2, 0.0, 0.0, 1.0);
	glRotatef(angle, 0.0, 1.0, 0.0);
	glRotatef((GLfloat)lshoulder2, 0.0, 1, 0);
	glRotatef((GLfloat)shoulder, 0.0, 0, 1);
	glTranslatef(1, 0, 0);
	glPushMatrix();
	glScalef(2.0, 0.8, 1.0);
	glColor3f(1, 0, 0.2);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0.3, 0);
	glutSolidSphere(0.6, 50, 50);
	glPopMatrix();
	
	glTranslatef(1, 0, 0.0);
	glRotatef((GLfloat)elbow, 0.0, 0.0, 1.0);
	glTranslatef(1.0, 0.0, 0.0);
	glPushMatrix();
	glScalef(2.0, 0.6, 1.0);
	glColor3f(0.9, 0.8, 1.0);
	glColor3f(0.9, 0.8, 1.0);
	glutSolidCube(1.0);
	glPopMatrix();

	//Draw finger flang c
	glPushMatrix();
	glTranslatef(1.0, -0.3, 0.0);
	glRotatef((GLfloat)lfingerBase, 0.0, 0.0, 1.0);
	glTranslatef(0.15, 0.0, 0.0);
	glPushMatrix();
	glScalef(0.3, 0.1, 0.1);
	glutSolidCube(1);
	glPopMatrix();

	//Draw finger flang d
	glTranslatef(0.15, 0.0, 0.0);
	glRotatef((GLfloat)lfingerUp, 0.0, 0.0, 1.0);
	glTranslatef(0.15, 0.0, 0.0);
	glPushMatrix();
	glScalef(0.3, 0.1, 0.1);
	glutSolidCube(1);
	glPopMatrix();
	glPopMatrix();

	//Draw finger flang a
	glPushMatrix();
	glTranslatef(1.0, -0.3, .2);
	glRotatef((GLfloat)lfingerBase, 0.0, 0.0, 1.0);
	glTranslatef(0.15, 0.0, 0.0);
	glPushMatrix();
	glScalef(0.3, 0.1, 0.1);
	glutSolidCube(1);
	glPopMatrix();

	//Draw finger flang b
	glTranslatef(0.15, 0.0, 0.0);
	glRotatef((GLfloat)lfingerUp, 0.0, 0.0, 1.0);
	glTranslatef(0.15, 0.0, 0.0);
	glPushMatrix();
	glScalef(0.3, 0.1, 0.1);
	glutSolidCube(1);
	glPopMatrix();
	glPopMatrix();

	//Draw finger flang e
	glPushMatrix();
	glTranslatef(1.0, -0.3, -.2);
	glRotatef((GLfloat)lfingerBase, 0.0, 0.0, 1.0);
	glTranslatef(0.15, 0.0, 0.0);
	glPushMatrix();
	glScalef(0.3, 0.1, 0.1);
	glutSolidCube(1);
	glPopMatrix();

	//Draw finger flang f
	glTranslatef(0.15, 0.0, 0.0);
	glRotatef((GLfloat)lfingerUp, 0.0, 0.0, 1.0);
	glTranslatef(0.15, 0.0, 0.0);
	glPushMatrix();
	glScalef(0.3, 0.1, 0.1);
	glutSolidCube(1);
	glPopMatrix();
	glPopMatrix();

	//Draw finger flang g
	glPushMatrix();
	glTranslatef(1.0, -0.3, -.4);
	glRotatef((GLfloat)lfingerBase, 0.0, 0.0, 1.0);
	glTranslatef(0.15, 0.0, 0.0);
	glPushMatrix();
	glScalef(0.3, 0.1, 0.1);
	glutSolidCube(1);
	glPopMatrix();

	//Draw finger flang h
	glTranslatef(0.15, 0.0, 0.0);
	glRotatef((GLfloat)lfingerUp, 0.0, 0.0, 1.0);
	glTranslatef(0.15, 0.0, 0.0);
	glPushMatrix();
	glScalef(0.3, 0.1, 0.1);
	glutSolidCube(1);
	glPopMatrix();
	glPopMatrix();

	glPopMatrix();
}

void draw_righ_leg(void)
{
	glPushMatrix();

	glTranslatef(-1.8, -2.5, 0.0);
	glRotatef(angle2, 0.0, 0.0, 1.0);
	glRotatef(angle, 0.0, 1.0, 0.0);
	glRotatef((GLfloat)rhip2, 0.0, 1, 0);
	glRotatef((GLfloat)rhip, 0.0, 0, 1);
	glTranslatef(1.5, 0, 0);
	glPushMatrix();
	glScalef(3.0, 1.5, 1.0);
	glColor3f(0.2, 0.0, 1.0);
	glutSolidCube(1.0);
	glPopMatrix();

	glTranslatef(1.5, 0, 0.0);
	glRotatef((GLfloat)rknee, 0.0, 1.0, 0.0);
	glTranslatef(1.5, 0.0, 0.0);
	glPushMatrix();
	glScalef(3.0, 1., 1.0);
	glColor3f(0.9, 0.8, 1.0);
	glutSolidCube(1.0);
	glPopMatrix();
	glTranslatef(2, 0.0, 0.0);
	glPushMatrix();
	glScalef(1, 1, 2);
	glColor3f(0, 0.0, 0.0);
	glutSolidCube(1);
	glPopMatrix();

	glPopMatrix();
}

void draw_left_leg(void)
{
	glPushMatrix();

	glTranslatef(1.8, -2.5, 0.0);
	glRotatef(angle2, 0.0, 0.0, 1.0);
	glRotatef(angle, 0.0, 1.0, 0.0);
	glRotatef((GLfloat)lhip2, 0.0, 1, 0);
	glRotatef((GLfloat)lhip, 0.0, 0, 1);
	glTranslatef(1.5, 0, 0);
	glPushMatrix();
	glScalef(3.0, 1.5, 1.0);
	glColor3f(0.2, 0, 1.0);
	glutSolidCube(1.0);
	glPopMatrix();

	glTranslatef(1.5, 0, 0.0);
	glRotatef((GLfloat)lknee, 0.0, 1.0, 0.0);
	glTranslatef(1.5, 0.0, 0.0);
	glPushMatrix();
	glScalef(3.0, 1., 1.0);
	glColor3f(0.9, 0.8, 1.0);
	glutSolidCube(1.0);
	glPopMatrix();
	glTranslatef(2, 0.0, 0.0);
	glPushMatrix();
	glScalef(1, 1, 2);
	glColor3f(0.0, 0.0, 0.0);
	glutSolidCube(1);
	glPopMatrix();

	glPopMatrix();
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]);

	glPushMatrix();
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glPopMatrix();
	//materials properties
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,mat_amb_diff);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

	//draw floor
	draw_floor();
	// draw trunck
	glColor3f(1, 0, 0.2);
	glPushMatrix();
	glTranslatef(0, -2.5, 0);
	glRotatef((GLfloat)trunk, 1, 0, 0);
	glTranslatef(0, 2.5, 0);
	glPushMatrix();
	glScalef(5, 5, 2);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 4, 0);
	glColor3f(0.9, 0.8, 1.0);
	glutSolidSphere(1.0, 50, 50);
	glPopMatrix();

	draw_left_arm();
	draw_right_arm();
	glPopMatrix();
	draw_righ_leg();
	draw_left_leg();

	glutSwapBuffers();
}

void specialKeys(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		Left();
		break;
	case GLUT_KEY_RIGHT:
		Right();
		break;
	case GLUT_KEY_UP:
		Up();
		break;
	case GLUT_KEY_DOWN:
		Down();
		break;
	}

	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 's':
		if (shoulder > -90)
			shoulder = (shoulder - 5) % 360;
		glutPostRedisplay();
		break;
	case 'S':
		if (shoulder < 0)
			shoulder = (shoulder + 5) % 360;
		glutPostRedisplay();
		break;
	case 'a':
		if (shoulder2 < 90)
			shoulder2 = (shoulder2 + 5) % 360;
		glutPostRedisplay();
		break;
	case 'A':
		if (shoulder2 > 0)
			shoulder2 = (shoulder2 - 5) % 360;
		glutPostRedisplay();
		break;
	case 'd':
		if (elbow < 120)
			elbow = (elbow + 5) % 360;
		glutPostRedisplay();
		break;
	case 'D':
		if (elbow > 0)
			elbow = (elbow - 5) % 360;
		glutPostRedisplay();
		break;
	case 'c':
		if (elbow2 < 0)
			elbow2 = (elbow2 + 5) % 360;
		glutPostRedisplay();
		break;
	case 'C':
		if (elbow2 > -120)
			elbow2 = (elbow2 - 5) % 360;
		glutPostRedisplay();
		break;
	case 'z':
		if (rfingerBase < 0)
			rfingerBase = (rfingerBase + 5) % 360;
		glutPostRedisplay();
		break;
	case 'Z':
		if (rfingerBase > -45)
			rfingerBase = (rfingerBase - 5) % 360;
		glutPostRedisplay();
		break;
	case 'x':
		if (rfingerUp < 0)
			rfingerUp = (rfingerUp + 5);
		glutPostRedisplay();
		break;
	case 'X':
		if (rfingerUp > -45)
			rfingerUp = (rfingerUp - 5) % 360;
		glutPostRedisplay();
		break;
	case 'n':
		if (lfingerBase < 45)
			lfingerBase = (lfingerBase + 5) % 360;
		glutPostRedisplay();
		break;
	case 'N':
		if (lfingerBase > 0)
			lfingerBase = (lfingerBase - 5) % 360;
		glutPostRedisplay();
		break;
	case 'm':
		if (lfingerUp < 45)
			lfingerUp = (lfingerUp + 5) % 360;
		glutPostRedisplay();
		break;
	case 'M':
		if (lfingerUp > 0)
			lfingerUp = (lfingerUp - 5) % 360;
		glutPostRedisplay();
		break;
	case 'q':
		if (rhip < 45)
			rhip = (rhip + 5) % 360;
		glutPostRedisplay();
		break;
	case 'Q':
		if (rhip > 0)
			rhip = (rhip - 5) % 360;
		glutPostRedisplay();
		break;
	case 'w':
		if (rknee < 120)
			rknee = (rknee + 5) % 360;
		glutPostRedisplay();
		break;
	case 'W':
		if (rknee > 0)
			rknee = (rknee - 5) % 360;
		glutPostRedisplay();
		break;
	case 'e':
		if (rhip2 < 45)
			rhip2 = (rhip2 + 5) % 360;
		glutPostRedisplay();
		break;
	case 'E':
		if (rhip2 > -90)
			rhip2 = (rhip2 - 5) % 360;
		glutPostRedisplay();
		break;
	case 'r':
		if (lhip > -45)
			lhip = (lhip - 5) % 360;
		glutPostRedisplay();
		break;
	case 'R':
		if (lhip < 0)
			lhip = (lhip + 5) % 360;
		glutPostRedisplay();
		break;
	case 't':
		if (lknee < 120)
			lknee = (lknee + 5) % 360;
		glutPostRedisplay();
		break;
	case 'T':
		if (lknee > 0)
			lknee = (lknee - 5) % 360;
		glutPostRedisplay();
		break;
	case 'y':
		if (lhip2 < 45)
			lhip2 = (lhip2 + 5) % 360;
		glutPostRedisplay();
		break;
	case 'Y':
		if (lhip2 > -90)
			lhip2 = (lhip2 - 5) % 360;
		glutPostRedisplay();
		break;
	case 'l':
		if (rshoulder2 < 90)
			rshoulder2 = (rshoulder2 + 5) % 360;
		glutPostRedisplay();
		break;
	case 'L':
		if (rshoulder2 > 0)
			rshoulder2 = (rshoulder2 - 5) % 360;
		glutPostRedisplay();
		break;
	case 'u':
		if (lshoulder2 > -90)
			lshoulder2 = (lshoulder2 - 5) % 360;
		glutPostRedisplay();
		break;
	case 'U':
		if (lshoulder2 < 0)
			lshoulder2 = (lshoulder2 + 5) % 360;
		glutPostRedisplay();
		break;
	case 'o':
		if (trunk > -90)
			trunk = (trunk - 5) % 360;
		glutPostRedisplay();
		break;
	case 'O':
		if (trunk < 0)
			trunk = (trunk + 5) % 360;
		glutPostRedisplay();
		break;
	case 'F':
		moveForward();
		glutPostRedisplay();
		break;
	case 'B':
		moveBack();
		glutPostRedisplay();
		break;

	default:
		break;
	}
}
void texture_mode(int value)
{
	switch (value)
	{
	case 1:
		_textureId=_textureId_wood;
		break;
	case 2:
		_textureId=_textureId_stone;
		break;
	case 3:
		_textureId=_textureId_cement;
		break;
	case 4:
		_textureId=_textureId_ceramics;
		break;
	}
	glutPostRedisplay();
}
/*void timer(int value)
{
	int sp = 70 ;
 
    if (value == 1)
    {    
       
      if ( shoulder <= 90) 
      {
         shoulder = (shoulder + 10) % 360;
         glutPostRedisplay();
         glutTimerFunc(sp, timer, 1);
      }
       else 
       glutTimerFunc(sp, timer, 2);
    }
}*/
void timer(int value)
{
int l=300;
int l2=300;
    if (value == 1)
    {    
      if (shoulder2 < 90)
      {
         shoulder2 = (shoulder2 + 5) % 360;
		     glutPostRedisplay();
         glutTimerFunc(l, timer, 1);
      }
	  if  (rshoulder2 < 180)
	  {
		rshoulder2 = (rshoulder2 + 5) % 360;
		glutPostRedisplay();
		glutTimerFunc(l, timer, 1);
	  }
	  else if  (elbow2 > -120)
	  {
		 
		elbow2 = (elbow2 - 5) % 360;
		glutPostRedisplay();
		glutTimerFunc(l, timer, 1);
		
	  }
	  
    else 
	  glutTimerFunc(l, timer, 2);
	}
	
    if (value == 2)
    {

		if (elbow2 < 0)
	  {
		elbow2 = (elbow2 + 5) % 360;
		glutPostRedisplay();
		glutTimerFunc(l, timer, 2);
	  }
	 if (rshoulder2 > 0)
	  {
		rshoulder2 = (rshoulder2 - 5) % 360;
		glutPostRedisplay();
		glutTimerFunc(l2, timer, 2);
	  }
   else if (shoulder2 > 0)
	 {
		
		shoulder2 = (shoulder2 - 5) % 360;
		glutPostRedisplay();
		glutTimerFunc(l, timer, 2);
	 }
	else 
	glutTimerFunc(l, timer, 3);
    }
	if (value == 3 )
		{
		if  (rshoulder2 < 40)
	  {
		rshoulder2 = (rshoulder2 + 5) % 360;
		glutPostRedisplay();
		glutTimerFunc(l, timer, 3);
	  }
    else if (elbow2 > -80)
	  {
		 
		elbow2 = (elbow2 - 5) % 360;
		glutPostRedisplay();
		glutTimerFunc(l, timer, 3);
		
	  }
		else 
		glutTimerFunc(l, timer, 4);
		}
}
void timer2(int value)
{

   int l=300;
   int l2=300;
    if (value == 1)
    {    
      if ( shoulder > -90) 
      {
         shoulder = (shoulder - 5) % 360;
         glutPostRedisplay();
         glutTimerFunc(l, timer2, 1);
      }
	  if (lshoulder2 > -180)
	  {
		lshoulder2 = (lshoulder2 - 5) % 360;
		glutPostRedisplay();
		glutTimerFunc(l, timer2, 1);
	  }
	  else if (elbow < 120)
	  {
		 
		elbow = (elbow + 5) % 360;
		glutPostRedisplay();
		glutTimerFunc(l, timer2, 1);
		
	  }
	  
    else 
	  glutTimerFunc(l, timer2, 2);
	}
	
    if (value == 2)
    {

     if (elbow > 0)
	  {
		elbow = (elbow - 5) % 360;
		glutPostRedisplay();
		glutTimerFunc(l, timer2, 2);
	  }
	 if (lshoulder2 < 0)
	  {
		lshoulder2 = (lshoulder2 + 5) % 360;
		glutPostRedisplay();
		glutTimerFunc(l2, timer2, 2);
	  }
	else if (shoulder < 0)
	 {
		
		shoulder = (shoulder + 5) % 360;
		glutPostRedisplay();
		glutTimerFunc(l, timer2, 2);
	 }
  else
	  glutTimerFunc(l, timer2, 3);
  }
	if (value == 3 )
		{
		if (lshoulder2 > -40)
	  {
		lshoulder2 = (lshoulder2 - 5) % 360;
		glutPostRedisplay();
		glutTimerFunc(l, timer2, 3);
	  }
    else if (elbow < 80)
	  { 
		elbow = (elbow + 5) % 360;
		glutPostRedisplay();
		glutTimerFunc(l, timer2, 3);
		
	  }
		else 
		glutTimerFunc(l, timer2, 4);
		}
	}
void main_menu(int value)
{
	if (value == 666)
		exit(0);
}
int main(int argc, char **argv)
{
	int submenu;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB| GLUT_DEPTH);
	glutInitWindowSize(1000, 1000);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("body");
	init();
		//glutTimerFunc(2000, timer, 0);

	glutDisplayFunc(display);
	submenu = glutCreateMenu(texture_mode);
	glutAddMenuEntry("Wood", 1);
	glutAddMenuEntry("Stone", 2);
	glutAddMenuEntry("Cement", 3);
	glutAddMenuEntry("Ceramics", 4);
	glutSpecialFunc(specialKeys);
	glutCreateMenu(main_menu);
	glutAddMenuEntry("Quit", 666);
	glutAddSubMenu("Texture mode", submenu);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutKeyboardFunc(keyboard);
  glutTimerFunc(0, timer, 2);
	glutTimerFunc(0, timer, 1);
	glutTimerFunc(100, timer, 3);
	glutTimerFunc(0, timer, 4);
	glutTimerFunc(0, timer2, 2);
	glutTimerFunc(0, timer2, 1);
	glutTimerFunc(100, timer2, 3);
	glutTimerFunc(0, timer2, 4);
	glutMainLoop();
	return 0;
}
