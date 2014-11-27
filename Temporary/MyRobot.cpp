#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

float	angle=0;
float	dist=0;

void DrawBody() {
    glPushMatrix();
		glScalef( 0.2, 0.4, 0.1 );
		glutWireCube( 1.0 );
    glPopMatrix();
}
void DrawHead() {
    glPushMatrix();
		glTranslatef( 0, 0.4, 0 );
		glTranslatef( dist, 0, 0 );
//		glTranslatef( 0, 0.42 + dist, 0 );
		glutWireSphere( 0.1, 10, 10 );
    glPopMatrix();
}
void DrawLeftArm() {
    glPushMatrix();
		glTranslatef( -0.25 - dist, 0.1, 0 );
//		glTranslatef( -0.25 , 0.1, 0 );

		glPushMatrix();
			glScalef( 1, 2, 1 );
			glutWireCube( 0.1 );
		glPopMatrix();

		glTranslatef( 0, -0.2, 0 );

		glPushMatrix();
			glTranslatef( 0, -dist*0.5, 0 );

			glScalef( 1, 2, 1 );
			glutWireCube( 0.1 );
		glPopMatrix();

    glPopMatrix();
}
void DrawRightArm() {
    glPushMatrix();
		glTranslatef( 0.25 + dist, 0.1, 0 );

		glPushMatrix();
			glScalef( 1, 2, 1 );
			glutWireCube( 0.1 );
		glPopMatrix();

		glTranslatef( 0, -0.2, 0 );

		glPushMatrix();
			glScalef( 1, 2, 1 );
			glTranslatef( 0, -0.05 -dist*0.5, 0 );
			glutWireCube( 0.1 );
		glPopMatrix();

    glPopMatrix();
}

void MyDisplay() {
    glClear(GL_COLOR_BUFFER_BIT);       //�÷����ۿ� �ʱ�ȭ ���� ����

	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

//	glRotatef( 90, 0, 1, 0 );		// ���鵵
//	glRotatef( 90, 1, 0, 0 );		// ��鵵
	glRotatef( 45, 0.5, 1, 0.3 );	// ������ȯ �ڵ�
	glScalef( 1, 1, 2 );			// ��ü ũ�� ��ȯ �ڵ�

//	glTranslatef( 0, 0, dist*3 );		// �ִϸ��̼�

	DrawBody();
	DrawHead();

	DrawLeftArm();
//	DrawRightArm();

	glFlush();
}

void MyKeyboard(unsigned char KeyPressed, int X, int Y) {
    switch (KeyPressed){
    case 'Q':
    case 'q':
    case 27:                //'esc' Ű�� �ƽ�Ű �ڵ� ��
        exit(0); break;

    case 'l':                //'esc' Ű�� �ƽ�Ű �ڵ� ��
		angle += 10.;
		glutPostRedisplay();
		break;
    case 'r':                //'esc' Ű�� �ƽ�Ű �ڵ� ��
		angle += -10.;
		glutPostRedisplay();
		break;
    }
}
#include <math.h>
void MyTimer(int Value) {
	angle = ((int)angle + 10 ) % 360;
    dist = 0.1*sin(3.14159*angle/180.);
    glutPostRedisplay();
    glutTimerFunc(50, MyTimer, 1);
}

int main(int argc, char** argv) {
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("�ǽ� 6��");
    glClearColor(0.3, 0.3, 1.0, 1.0);   //�ʱ�ȭ ���� ���
    glutDisplayFunc(MyDisplay);
    glutKeyboardFunc(MyKeyboard);
    glutTimerFunc(40, MyTimer, 1);
    glutMainLoop();
    return 0;
}
