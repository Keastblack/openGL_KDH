#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>

int WinWidth, WinHeight;

int AnimationMenu = 0;

bool IsTeapot=true;
bool IsCube=false;
bool IsCone=false;
bool IsSphere=false;
bool IsTorus=false;

GLfloat unit_size = 0.0;
GLfloat unit_rotate = 0.0;
GLfloat unit_move = 0.0;

void print_name(){ //이름 출력
	///////////// '동' 부분/////////////
	glBegin(GL_QUADS);

	glVertex3f(-0.8, 0.8, 0.0);
	glVertex3f(-0.8, 0.4, 0.0);
	glVertex3f(-0.7, 0.4, 0.0);
	glVertex3f(-0.7, 0.8, 0.0);

	glVertex3f(-0.7, 0.5, 0.0);
	glVertex3f(-0.2, 0.5, 0.0);
	glVertex3f(-0.2, 0.4, 0.0);
	glVertex3f(-0.7, 0.4, 0.0);

	glVertex3f(-0.7, 0.8, 0.0);
	glVertex3f(-0.2, 0.8, 0.0);
	glVertex3f(-0.2, 0.7, 0.0);
	glVertex3f(-0.7, 0.7, 0.0);

	glVertex3f(-0.55, 0.3, 0.0);
	glVertex3f(-0.45, 0.3, 0.0);
	glVertex3f(-0.45, 0.2, 0.0);
	glVertex3f(-0.55, 0.2, 0.0);

	glVertex3f(-0.9, 0.2, 0.0);
	glVertex3f(-0.1, 0.2, 0.0);
	glVertex3f(-0.1, 0.1, 0.0);
	glVertex3f(-0.9, 0.1, 0.0);

	glVertex3f(-0.6, -0.1, 0.0);
	glVertex3f(-0.4, -0.1, 0.0);
	glVertex3f(-0.4, -0.2, 0.0);
	glVertex3f(-0.6, -0.2, 0.0);

	glVertex3f(-0.8, -0.3, 0.0);
	glVertex3f(-0.7, -0.3, 0.0);
	glVertex3f(-0.7, -0.4, 0.0);
	glVertex3f(-0.8, -0.4, 0.0);

	glVertex3f(-0.3, -0.3, 0.0);
	glVertex3f(-0.2, -0.3, 0.0);
	glVertex3f(-0.2, -0.4, 0.0);
	glVertex3f(-0.3, -0.4, 0.0);

	glVertex3f(-0.6, -0.5, 0.0);
	glVertex3f(-0.4, -0.5, 0.0);
	glVertex3f(-0.4, -0.6, 0.0);
	glVertex3f(-0.6, -0.6, 0.0);

	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(-0.8, -0.2, 0.0);
	glVertex3f(-0.7, -0.1, 0.0);
	glVertex3f(-0.6, -0.1, 0.0);
	glVertex3f(-0.6, -0.2, 0.0);
	glVertex3f(-0.7, -0.3, 0.0);
	glVertex3f(-0.8, -0.3, 0.0);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(-0.4, -0.1, 0.0);
	glVertex3f(-0.4, -0.2, 0.0);
	glVertex3f(-0.3, -0.3, 0.0);
	glVertex3f(-0.2, -0.3, 0.0);
	glVertex3f(-0.2, -0.2, 0.0);
	glVertex3f(-0.3, -0.1, 0.0);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(-0.8, -0.4, 0.0);
	glVertex3f(-0.8, -0.5, 0.0);
	glVertex3f(-0.7, -0.6, 0.0);
	glVertex3f(-0.6, -0.6, 0.0);
	glVertex3f(-0.6, -0.5, 0.0);
	glVertex3f(-0.7, -0.4, 0.0);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(-0.4, -0.5, 0.0);
	glVertex3f(-0.3, -0.4, 0.0);
	glVertex3f(-0.2, -0.4, 0.0);
	glVertex3f(-0.2, -0.5, 0.0);
	glVertex3f(-0.3, -0.6, 0.0);
	glVertex3f(-0.4, -0.6, 0.0);
	glEnd();

	//////////////// '현' 부분////////////////////

	glBegin(GL_QUADS);
	glVertex3f(0.2, 0.9, 0.0);
	glVertex3f(0.5, 0.9, 0.0);
	glVertex3f(0.5, 0.8, 0.0);
	glVertex3f(0.2, 0.8, 0.0);

	glVertex3f(0.1, 0.7, 0.0);
	glVertex3f(0.6, 0.7, 0.0);
	glVertex3f(0.6, 0.6, 0.0);
	glVertex3f(0.1, 0.6, 0.0);

	glVertex3f(0.6, 0.5, 0.0);
	glVertex3f(0.7, 0.5, 0.0);
	glVertex3f(0.7, 0.4, 0.0);
	glVertex3f(0.6, 0.4, 0.0);

	glVertex3f(0.6, 0.3, 0.0);
	glVertex3f(0.7, 0.3, 0.0);
	glVertex3f(0.7, 0.2, 0.0);
	glVertex3f(0.6, 0.2, 0.0);

	glVertex3f(0.7, 0.8, 0.0);
	glVertex3f(0.8, 0.8, 0.0);
	glVertex3f(0.8, -0.1, 0.0);
	glVertex3f(0.7, -0.1, 0.0);

	glVertex3f(0.2, -0.1, 0.0);
	glVertex3f(0.3, -0.1, 0.0);
	glVertex3f(0.3, -0.6, 0.0);
	glVertex3f(0.2, -0.6, 0.0);

	glVertex3f(0.3, -0.5, 0.0);
	glVertex3f(0.8, -0.5, 0.0);
	glVertex3f(0.8, -0.6, 0.0);
	glVertex3f(0.3, -0.6, 0.0);

	glVertex3f(0.2, 0.5, 0.0);
	glVertex3f(0.4, 0.5, 0.0);
	glVertex3f(0.4, 0.4, 0.0);
	glVertex3f(0.2, 0.4, 0.0);

	glVertex3f(0.1, 0.4, 0.0);
	glVertex3f(0.2, 0.4, 0.0);
	glVertex3f(0.2, 0.2, 0.0);
	glVertex3f(0.1, 0.2, 0.0);

	glVertex3f(0.4, 0.4, 0.0);
	glVertex3f(0.5, 0.4, 0.0);
	glVertex3f(0.5, 0.2, 0.0);
	glVertex3f(0.4, 0.2, 0.0);

	glVertex3f(0.2, 0.2, 0.0);
	glVertex3f(0.4, 0.2, 0.0);
	glVertex3f(0.4, 0.1, 0.0);
	glVertex3f(0.2, 0.1, 0.0);

	glEnd();

	glBegin(GL_TRIANGLES);

	glVertex3f(0.1, 0.4, 0.0);
	glVertex3f(0.2, 0.5, 0.0);
	glVertex3f(0.2, 0.4, 0.0);

	glVertex3f(0.1, 0.2, 0.0);
	glVertex3f(0.2, 0.2, 0.0);
	glVertex3f(0.2, 0.1, 0.0);

	glVertex3f(0.4, 0.1, 0.0);
	glVertex3f(0.4, 0.2, 0.0);
	glVertex3f(0.5, 0.2, 0.0);

	glVertex3f(0.4, 0.4, 0.0);
	glVertex3f(0.4, 0.5, 0.0);
	glVertex3f(0.5, 0.4, 0.0);

	glEnd();
}

void Draw1(){ //좌측 viewport 그리기
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity(); //초기화 개념
	glViewport(0, 0, WinWidth/2, WinHeight);
	glColor3f(1.0, 0.5, 0.5);
	print_name();
}


void Draw2(){ //우측 viewport 그리기
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	glViewport(WinWidth/2, 0, WinWidth/2, WinHeight);
	glColor3f(0.5, 1.0, 0.5);
	if (IsTeapot)
		glutWireTeapot(0.7);
	else if (IsCube)
		glutWireCube(2);
	else if (IsCone)
		glutWireCone(1.0, 1.0, 20, 20);
	else if (IsSphere)
		glutWireSphere(1.0, 20, 20);
	else if (IsTorus)
		glutWireTorus(0.2, 0.9, 40, 20);
}
void MyMainMenu(int entryID) {
	switch(entryID){
	case 1:
		IsTeapot = true;               
		IsCube = false;
		IsCone = false;
		IsSphere = false;
		IsTorus = false;
		break;
	case 2:
		IsTeapot = false;               
		IsCube = true;
		IsCone = false;
		IsSphere = false;
		IsTorus = false;
		break;
	case 3:
		IsTeapot = false;               
		IsCube = false;
		IsCone = true;
		IsSphere = false;
		IsTorus = false;
		break;
	case 4:
		IsTeapot = false;               
		IsCube = false;
		IsCone = false;
		IsSphere = true;
		IsTorus = false;
		break;
	case 5:
		IsTeapot = false;               
		IsCube = false;
		IsCone = false;
		IsSphere = false;
		IsTorus = true;
		break;
	case 6:
		exit(0); 
		break;
	}
	glutPostRedisplay();
}
void MySubAnimationMenu(int entryID) {
    AnimationMenu = entryID-1;
}
void MyDisplay() {
	glClear(GL_COLOR_BUFFER_BIT);
	Draw1();
	Draw2();
	glEnd();
	glFlush();
}
void MyReshape(int NewWidth, int NewHeight) {
	WinWidth = NewWidth, WinHeight = NewHeight;

	glViewport(0, 0, NewWidth, NewHeight);
	GLfloat WidthFactor = (GLfloat) NewWidth / (GLfloat) 300;
	GLfloat HeightFactor = (GLfloat) NewHeight / (GLfloat) 300;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0 * WidthFactor, 1.0 * WidthFactor,
		-1.0 * HeightFactor, 1.0 * HeightFactor, -1.0, 1.0);
}
void MyKeyboard(unsigned char KeyPressed, int X, int Y) {
	switch (KeyPressed){
	case 'Q': case 'q': case 27: //'esc' 키의 아스키 코드 값
		exit(0); break;
	case 'a':
		glTranslatef(-0.05, 0.0, 0.0); break;
	case 'd':
		glTranslatef(0.05, 0.0, 0.0); break;
	case 'w':
		glTranslatef(0.0, 0.05, 0.0); break;
	case 's':
		glTranslatef(0.0, -0.05, 0.0); break;
	case '+':
		glScalef(1.05, 1.05, 1.05); break;
	case '-':
		glScalef(0.95, 0.95, 0.95); break;
	}
	glutPostRedisplay();
}
void MySpecialInput(int key, int X, int Y){
	switch(key){
		case GLUT_KEY_UP:
			glRotatef(10.0, -0.5, 0.0, 0.0);
			break;
		case GLUT_KEY_DOWN:
			glRotatef(10.0, 0.5, 0.0, 0.0);
			break;
		case GLUT_KEY_LEFT:
			glRotatef(10.0, 0.0, -0.5, 0.0);
			break;
		case GLUT_KEY_RIGHT:
			glRotatef(10.0, 0.0, 0.5, 0.0);
			break;
	}
	glutPostRedisplay();
}
void MyTimer(int Value) {
	switch(AnimationMenu) {	//애니메이션 스위치
	case 0:	// off
		break;
	case 1:	// 이동
		if(unit_move < 0.05) unit_move += 0.005;
		else unit_move = -0.05;
		glTranslatef(unit_move, 0, 0);
		break;
	case 2:	// 회전
		unit_rotate += 0.3;
		glRotatef(2.0, unit_rotate, unit_rotate, unit_rotate);
		break;
	case 3:	// 크기 변화
		if(unit_size < 0.05) unit_size += 0.005;
		else unit_size = -0.05;
		glScalef(1 + unit_size, 1 + unit_size, 1 + unit_size);
		break;
	}
	glutPostRedisplay();
	glutTimerFunc(40, MyTimer, 1);
}
int main(int argc, char** argv) {
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGB);
	glutInitWindowSize(700, 700);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("OpenGL Drawing Example");

	GLint MySubAnimationMenuID = glutCreateMenu(MySubAnimationMenu);
	glutAddMenuEntry("off", 1);
	glutAddMenuEntry("이동", 2);
	glutAddMenuEntry("회전", 3);
	glutAddMenuEntry("크기 변화", 4);

	GLint MyMainMenuID = glutCreateMenu(MyMainMenu);
	glutAddMenuEntry("Draw Teapot", 1);
	glutAddMenuEntry("Draw Cube", 2);
	glutAddMenuEntry("Draw Cone", 3);
	glutAddMenuEntry("Draw Sphere", 4);
	glutAddMenuEntry("Draw Torus", 5);
	glutAddSubMenu("Animation", MySubAnimationMenuID);
	glutAddMenuEntry("Exit", 6);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutDisplayFunc(MyDisplay);
	glutReshapeFunc(MyReshape);
	glutKeyboardFunc(MyKeyboard);
	glutSpecialFunc(MySpecialInput);
	glutTimerFunc(40, MyTimer, 1);
	glutMainLoop();
	return 0;
}

