#include <GL/glut.h>
#include <math.h>

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

void init(void)
{
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
	glutWireCube(1.0);
	glPopMatrix();

	glTranslatef(1, 0, 0.0);
	glRotatef((GLfloat)elbow2, 0.0, 0.0, 1.0);
	glTranslatef(1.0, 0.0, 0.0);
	glPushMatrix();
	glScalef(2.0, 0.6, 1.0);
	glutWireCube(1.0);
	glPopMatrix();

	//Draw finger flang c
	glPushMatrix();
	glTranslatef(1.0, 0.3, 0.0);
	glRotatef((GLfloat)rfingerBase, 0.0, 0.0, 1.0);
	glTranslatef(0.15, 0.0, 0.0);
	glPushMatrix();
	glScalef(0.3, 0.1, 0.1);
	glutWireCube(1);
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
	glutWireCube(1);
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
	glutWireCube(1);
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
	glutWireCube(1);
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
	glutWireCube(1.0);
	glPopMatrix();

	glTranslatef(1, 0, 0.0);
	glRotatef((GLfloat)elbow, 0.0, 0.0, 1.0);
	glTranslatef(1.0, 0.0, 0.0);
	glPushMatrix();
	glScalef(2.0, 0.6, 1.0);
	glutWireCube(1.0);
	glPopMatrix();

	//Draw finger flang c
	glPushMatrix();
	glTranslatef(1.0, -0.3, 0.0);
	glRotatef((GLfloat)lfingerBase, 0.0, 0.0, 1.0);
	glTranslatef(0.15, 0.0, 0.0);
	glPushMatrix();
	glScalef(0.3, 0.1, 0.1);
	glutWireCube(1);
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
	glutWireCube(1);
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
	glutWireCube(1);
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
	glutWireCube(1);
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
	glutWireCube(1.0);
	glPopMatrix();

	glTranslatef(1.5, 0, 0.0);
	glRotatef((GLfloat)rknee, 0.0, 1.0, 0.0);
	glTranslatef(1.5, 0.0, 0.0);
	glPushMatrix();
	glScalef(3.0, 1., 1.0);
	glutWireCube(1.0);
	glPopMatrix();
	glTranslatef(2, 0.0, 0.0);
	glPushMatrix();
	glScalef(1, 1, 2);
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
	glutWireCube(1.0);
	glPopMatrix();

	glTranslatef(1.5, 0, 0.0);
	glRotatef((GLfloat)lknee, 0.0, 1.0, 0.0);
	glTranslatef(1.5, 0.0, 0.0);
	glPushMatrix();
	glScalef(3.0, 1., 1.0);
	glutWireCube(1.0);
	glPopMatrix();
	glTranslatef(2, 0.0, 0.0);
	glPushMatrix();
	glScalef(1, 1, 2);
	glutSolidCube(1);
	glPopMatrix();

	glPopMatrix();
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]);

	// draw trunck
	glPushMatrix();
	glTranslatef(0, -2.5, 0);
	glRotatef((GLfloat)trunk, 1, 0, 0);
	glTranslatef(0, 2.5, 0);
	glPushMatrix();
	glScalef(5, 5, 2);
	glutWireCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 4, 0);
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

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(1000, 1000);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("body");
	init();
	glutDisplayFunc(display);
	glutSpecialFunc(specialKeys);
	glutKeyboardFunc(keyboard);

	glutMainLoop();
	return 0;
}
