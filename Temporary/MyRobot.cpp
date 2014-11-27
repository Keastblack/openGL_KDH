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
    glClear(GL_COLOR_BUFFER_BIT);       //컬러버퍼에 초기화 색을 가함

	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

//	glRotatef( 90, 0, 1, 0 );		// 측면도
//	glRotatef( 90, 1, 0, 0 );		// 평면도
	glRotatef( 45, 0.5, 1, 0.3 );	// 시점변환 코드
	glScalef( 1, 1, 2 );			// 전체 크기 변환 코드

//	glTranslatef( 0, 0, dist*3 );		// 애니메이션

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
    case 27:                //'esc' 키의 아스키 코드 값
        exit(0); break;

    case 'l':                //'esc' 키의 아스키 코드 값
		angle += 10.;
		glutPostRedisplay();
		break;
    case 'r':                //'esc' 키의 아스키 코드 값
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
    glutCreateWindow("실습 6주");
    glClearColor(0.3, 0.3, 1.0, 1.0);   //초기화 색은 백색
    glutDisplayFunc(MyDisplay);
    glutKeyboardFunc(MyKeyboard);
    glutTimerFunc(40, MyTimer, 1);
    glutMainLoop();
    return 0;
}
