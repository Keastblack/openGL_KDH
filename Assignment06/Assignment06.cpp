/*************************************
q: 종료
w: wire-solid 토글링
s: 표면 Flat / Smooth 토글링
a: 로봇 움직임( 사선으로 날아감 )
**************************************/

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

int FlatShaded = 0; int Wireframed = 0;
	//flatshading과 wireframe을 토글링하기 위한 부울 변수

int ViewX = 0, ViewY = 0;
	//마우스 움직임에 따라 시점을 바꾸기 위한 변수

float angle = 0.0f; // 카메라용 앵글 1
float angle2 = 0.0f; // 카메라용 앵글 2

float t_angle=0; // 타이머용 앵글
float dist=0; // 움직임의 정도를 결정짓는 변수

int tx = 0, ty = 0;

bool Movement = false; // 로봇의 움직임을 on/off
float unit_move = 0.0; // 움직임의 정도

void InitLight(){
	GLfloat mat_diffuse[] = {0.5, 1.4, 0.7, 1.0};
	GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat mat_ambient[] = {0.5, 1.4, 0.3, 1.0};
	GLfloat mat_shininess[] = {15.0};
	GLfloat light_specular[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat light_diffuse[] = {0.8, 0.8, 0.8, 1.0};
	GLfloat light_ambient[] = {0.3, 0.3, 0.3, 1.0};
	GLfloat light_position[] = {-3, 6, 3.0, 0.0};
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}

void MyRotate(int x, int y){	//물체 회전에 관한 함수
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

void MyMouseMove(int botton, int state, GLint X, GLint Y){	//마우스 움직임에 관한 함수
    if (botton == GLUT_LEFT_BUTTON && state == GLUT_DOWN){	//버튼을 누른채로 움직이는 경우,
        ViewX = X;	//마우스의 좌표를 기록한다.
        ViewY = Y;
		glutPostRedisplay();
    }
}

void MyKeyboard(unsigned char key, int x, int y){
	switch(key){
	case 'q': case'Q': case '\033':
		exit(0);
		break;
	case 'a':	//로봇 움직이기, 토글링 형식
		if(Movement)
			Movement = false;
		else
			Movement = true;
		break;
	case 's':
		if(FlatShaded){
			FlatShaded = 0;
			glShadeModel(GL_SMOOTH);
		}
		else{
			FlatShaded = 1;
			glShadeModel(GL_FLAT);
		}
		glutPostRedisplay();
		break;
	case 'w':	//WireFramed 토글
		if(Wireframed){
			Wireframed=0;
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		else{
			Wireframed=1;
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		glutPostRedisplay();
		break;
	}
}

void DrawBody() {
    glPushMatrix();
		glScalef( 0.4, 0.4, 0.2 );
		glutSolidCube( 1.0 );
    glPopMatrix();
}
void DrawHead() {
    glPushMatrix();
		glTranslatef( 0, 0.4, 0 );
		glTranslatef( dist, 0, 0 );
//		glTranslatef( 0, 0.42 + dist, 0 );
		glScalef(1,1,0.8);
		glutSolidSphere( 0.1, 10, 10 );
    glPopMatrix();
}
void DrawLeftArm() {
    glPushMatrix();
		glTranslatef( -0.35 - dist, 0.1, 0 );

		glPushMatrix();
			glScalef( 1, 2, 1 );
			glutSolidCube( 0.1 );
		glPopMatrix();

		glTranslatef( 0, -0.2, 0 );

		glPushMatrix();
			glTranslatef( 0, -dist*0.5, 0 );

			glScalef( 1, 2, 1 );
			glutSolidCube( 0.1 );
		glPopMatrix();

    glPopMatrix();
}
void DrawRightArm() {
    glPushMatrix();
		glTranslatef( 0.35 + dist, 0.1, 0 );

		glPushMatrix();
			glScalef( 1, 2, 1 );
			glutSolidCube( 0.1 );
		glPopMatrix();

		glTranslatef( 0, -0.2, 0 );

		glPushMatrix();
			glTranslatef( 0, -dist*0.5, 0 );
			glScalef( 1, 2, 1 );
			glutSolidCube( 0.1 );
		glPopMatrix();

    glPopMatrix();
}
void DrawLeftLeg() {
    glPushMatrix();
		glTranslatef( -0.15 - dist/1.5, -0.35, 0 );
//		glTranslatef( -0.25 , 0.1, 0 );

		glPushMatrix();
			glScalef( 1, 1.8, 1 );
			glutSolidCube( 0.15 );
		glPopMatrix();

		glTranslatef( 0, -0.3, 0 );

		glPushMatrix();
			glTranslatef( 0, -dist*0.5, 0 );

			glScalef( 1, 2, 1 );
			glutSolidCube( 0.15 );
		glPopMatrix();

    glPopMatrix();
}
void DrawRightLeg() {
    glPushMatrix();
		glTranslatef( 0.15 + dist/1.5, -0.35, 0 );

		glPushMatrix();
			glScalef( 1, 1.8, 1 );
			glutSolidCube( 0.15 );
		glPopMatrix();

		glTranslatef( 0, -0.3, 0 );

		glPushMatrix();
			glTranslatef( 0, -dist*0.5, 0 );
			glScalef( 1, 2, 1 );
			glutSolidCube( 0.15 );
		glPopMatrix();

    glPopMatrix();
}
void MyDisplay(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glRotatef(200, 0.0, 1.0, 0.0); // 초기 카메라 앵글
    glRotatef(-30, 1.0, 0.0, 0.0); // 초기 카메라 앵글

	glRotatef(angle, 0.0, 1.0, 0.0);
    glRotatef(angle2, 1.0, 0.0, 0.0);

	glTranslatef(unit_move, unit_move, unit_move); // 로봇의 움직임

	DrawBody();
	DrawHead();

	DrawLeftArm();
	DrawRightArm();
	DrawLeftLeg();
	DrawRightLeg();
	
	gluLookAt(1.0, 1.0, 0.5, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	glFlush();
}

#include <math.h>
void MyTimer(int Value) {
	t_angle = ((int)t_angle + 10 ) % 360;
    dist = 0.1*sin(3.14159*t_angle/180.);

	if(Movement){
		unit_move = 0.1*tan(3.14159*t_angle/180.);
	}
    glutPostRedisplay();
    glutTimerFunc(50, MyTimer, 1);
}

int main(int argc, char** argv) {
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("2010136019_김동현_Assignment05");
    glClearColor(0.3, 0.3, 1.0, 1.0);  
	InitLight();
    glutDisplayFunc(MyDisplay);
    glutKeyboardFunc(MyKeyboard);
	glutMouseFunc(MyMouseMove);
    glutMotionFunc(MyRotate);
    glutTimerFunc(40, MyTimer, 1);
    glutMainLoop();
    return 0;
}