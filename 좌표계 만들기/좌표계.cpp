#include <iostream>
#include <stdlib.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <string.h>
using namespace std;

int ViewX = 0, ViewY = 0;
	//���콺 �����ӿ� ���� ������ �ٲٱ� ���� ����

float angle = 0.0f; // ī�޶�� �ޱ� 1
float angle2 = 0.0f; // ī�޶�� �ޱ� 2
int tx = 0, ty = 0;

enum{
	ORTHO, FRUSTUM
}mode=ORTHO;

void MyRotate(int x, int y){	//��ü ȸ���� ���� �Լ�
    tx = x - ViewX;
    ty = y - ViewY;

    if (tx > 0) angle += 2.0f;
    else if (tx < 0) angle -= 2.0f;
    else angle;

    if (ty > 0) angle2 += 2.0f;
    else if (ty < 0) angle2 -= 2.0f;
    else angle2;

    if (angle >= 360.0f)
        angle = angle - 360.0f;

    ViewX = x;
    ViewY = y;

    glutPostRedisplay();
}

void MyMouseMove(int botton, int state, GLint X, GLint Y){	//���콺 �����ӿ� ���� �Լ�
    if (botton == GLUT_LEFT_BUTTON && state == GLUT_DOWN){	//��ư�� ����ä�� �����̴� ���,
        ViewX = X;	//���콺�� ��ǥ�� ����Ѵ�.
        ViewY = Y;
		glutPostRedisplay();
    }
}
void drawSphere(){
	glPushMatrix();
		glScalef(0.5, 0.5, 0.5);
		glutWireSphere(0.3, 20, 20);
	glPopMatrix();
}

void drawGridPlane()
{
	int		i;
	double	x, z;

	glLineWidth(1);

    glPushMatrix();
		glTranslatef(0, -0.5, 0);
		glRotatef(30.0f, 1.0, 0.0, 0.0);
		glRotatef(30.0f, 0.0, 1.0, 0.0);
		glScalef(0.7, 0.7, 0.7);
		glBegin (GL_LINES);
			x = -1.0;
			z = -1.0;
			for( i=0 ; i<=10 ; i++ ) {
				glVertex3f (x, 0, z);
				glVertex3f (x, 0, z+2);
				glVertex3f (z, 0, x);
				glVertex3f (z+2, 0, x);
				x += 0.2;
			}
		glEnd();
    glPopMatrix();
}
void MyReshape2(int width, int height){
	glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, 1.0, 1.0);
//	glFrustum(-1.0, 1.0, -1.0, 1.0, 0.1, 1.0);
//	gluPerspective(130.0, (GLfloat)width/height, 0.0, 10.0);
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
void drawTeapot(){
	glTranslatef(0.2, 0.5, 0.0);
	glRotatef(45, 0, 1.0, 0);
	glutWireTeapot(0.1);
}
void drawFunc(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glColor3f(0.0,1.0,0.0);

	//glRotatef(angle, 0.0, 1.0, 0.0);
	//glRotatef(angle2, 1.0, 0.0, 0.0);
	glRotatef(-30.0, 1.0, 0, 0);//x������ �����
	glRotatef(-230.0, 0, 1.0, 0);//y������ ����� ���� -230
	glutWireTeapot(0.3);
	glLoadIdentity(); //�����ִ� ����

	//drawGridPlane();
	glRotatef(90.0, 0, 0, 1.0);
	glPushMatrix();
	drawTeapot();
	glutSwapBuffers();
}
void MyDisplay(){
	drawFunc();
	glPopMatrix();
	glFlush();
}
void MyKeyboard(unsigned char KeyPressed,int x,int y){
	switch (KeyPressed){
		case 'q': case 'Q':
			exit(0);
			break;
		case '1':
			cout << "mode = ORTHO" << endl;
			mode=ORTHO;
			break;
		case '2':
			cout << "mode = FRUSTUM" << endl;
			mode=FRUSTUM;
			break;
	}
}

int main(int argc, char** argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("��ǥ�� �����");
	glClearColor(0.3, 0.3, 0.3, 1.0);
	//glutMouseFunc(MyMouseMove);
	//glutMotionFunc(MyRotate);
	glutKeyboardFunc(MyKeyboard); //Ű���� �ݹ�

	glutReshapeFunc(MyReshape2);
	glutDisplayFunc(MyDisplay);

	//glutReshapeFunc(MyReshape);
	glutMainLoop();
	return 0;
}
