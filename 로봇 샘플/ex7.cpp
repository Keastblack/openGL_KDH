#include <iostream>
#include "ase.h"
#include <gl/glut.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <string.h>
#include <math.h>   
using namespace std;
#define PI   (3.14159265358979323846)

int windowX;
int windowY;
GLfloat WidthFactor;
GLfloat HeightFactor;

float scale;         // ase 파일을 불러올 때 그 mesh 객체의 크기를 뷰포트 내에 알맞게 조정하기 위한 변수
float scaleRate;     // 프로그램에서 z, x 키를 통해 줌인/줌아웃을 하기 위한 변수

static float sx, sy;
static float tx = 0.0f;
static float ty = 0.0f;

float rotateX1;   // 1번 뷰포트의 Y축 기준 회전 각도 변수
float rotateY1;   // 1번 뷰포트의 X축 기준 회전 각도 변수
float rotateX2;
float rotateY2;
float rotateX3;
float rotateY3;
float rotateX4;
float rotateY4;
int viewSelected;   // 마우스로 뷰포트 선택하기 위한 변수ㅁ
int viewClick;

float p_near;   // 원근투상의 near 값
float p_scale;  // 원근투상의 확대, 축소를 위한 변수

float camEyeX;  // 카메라 설정 값 변수
float camEyeY;
float camEyeZ;
float camAtX;
float camAtY;
float camAtZ;
float camUpX;
float camUpY;
float camUpZ;

int systemInt;  // 시스템 값 출력을 위한 변수

Mesh *Head;        // 로봇의 몸 부위 객체
Mesh *Body;
Mesh *RightArm;
Mesh *RightHand;
Mesh *LeftArm;
Mesh *LeftHand;
Mesh *RightLeg;
Mesh *RightFoot;
Mesh *LeftLeg;
Mesh *LeftFoot;

bool robotRun;            // 로봇의 달리는 애니메이션 유무
float robotWeight;        // 로봇의 무게
char robotWeightStr[3];   // 로봇의 무게를 출력하기 위한 문자열 
double robotSpeed;        // 로봇의 달리는 속도
char robotSpeedStr[3];    // 로봇의 속도를 출력하기 위한 문자열 
double robotRange;        // 로봇이 달리는 정도를 구하기 위한 각도
double dRadian;           // 위 각도의 변수로 sin, cos 값을 구하기 위한 라디안 변수
float sinR;               // sin, cos 값으로 로봇의 팔,다리, 몸, 머리의 각 회전 각도를 조절
float sinR2;
float cosR;
float d_cosR;
float d_sinR;

GLfloat mat_diffuse[6];
GLfloat mat_ambient[6];

void meshLoad();         // 로봇 ASE 파일을 불러옴
void meshInit();         // 로봇의 동작을 초기화
void InitLight();
void MyDisplay(void);
void MyReshape(int, int);
void MyKeyboard(unsigned char, int, int);
void MySubMenu(int);   // 줌인,줌아웃, 달리기,정지 모션
void MySubMenu2(int);  // 4번째 뷰포트의 카메라 설정
void MySubMenu3(int);  // Backface culling on, off
void MySubMenu4(int);  // 앞면, 뒷면 출력 방식 (Fill, Line, Point 설정)

void MyMainMenu(int);
void MyTimer(int);
void setCamera(float, float, float, float, float, float, float, float, float); // 카메라 위치 설정

void setColor(float, float, float, float);    // 각 객체의 재질 색 설정
void drawRobot();        // 로봇 전체 출력
void drawHead();         // 로봇 머리 출력
void drawBody();         // 로봇 몸통 출력
void drawRightArm();
void drawRightHand();
void drawLeftArm();
void drawLeftHand();
void drawRightLeg();
void drawRightFoot();
void drawLeftLeg();
void drawLeftFoot();
void drawGridPlane();   // 바닥출력
void drawString (const char *);  // 문자열 출력
void drawCoord(int);    // x,y,z 축 출력

void mouse(int button, int state, int x, int y);
void motion(int x, int y);

int main(int argc, char **argv)
{
	windowX = 800;
	windowY = 800;
	WidthFactor = 1;
	HeightFactor = 1;
	camEyeX = 2.0;
	camEyeY = 1.0;
	camEyeZ = 2.0;
	camAtX = 0.5;
	camAtY = 0.5;
	camAtZ = 0.5;
	camUpX = 0;
	camUpY = 1;
	camUpZ = 0;
	viewSelected = 4;
	viewClick = 0;

	meshLoad();
	meshInit();

	cout << "--- Control Key ---\nRun / Stop : E \nRun Faster : W\nRun Slower : S\nIncrease Weight : A\n";
	cout << "Loose Weight : D\nZoom in : Z\nZoom out : X\nReload : R\nQuit : Q\n" ; 

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(windowX, windowY);
	glutInitWindowPosition(300, 100); 
	glutCreateWindow("2004135058 Opengl #6");
	glClearColor(1.0, 1.0, 1.0, 1.0);
	InitLight();       // 조명값 설정
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);  // 숨겨진 면을 제거, 시점으로부터 mesh 의 각 픽셀의 깊이 값을 계산하여 출력
	glShadeModel(GL_SMOOTH); 

	GLint MySubMenuID = glutCreateMenu(MySubMenu);

	glutAddMenuEntry("Zoom in", 1);
	glutAddMenuEntry("Zoom out", 2);
	glutAddMenuEntry("Run", 3);
	glutAddMenuEntry("Stop", 4);

	GLint MySubMenuID2 = glutCreateMenu(MySubMenu2);

	glutAddMenuEntry("Camera 1", 1);
	glutAddMenuEntry("Camera 2", 2);
	glutAddMenuEntry("Camera 3", 3);
	glutAddMenuEntry("Camera 4", 4);
	glutAddMenuEntry("default Camera", 5);

	GLint MySubMenuID3 = glutCreateMenu(MySubMenu3);

	glutAddMenuEntry("Backface Culling On", 1);
	glutAddMenuEntry("Backface Culling Off", 2);

	GLint MySubMenuID4 = glutCreateMenu(MySubMenu4);

	glutAddMenuEntry("Frontface Fill", 1);
	glutAddMenuEntry("Frontface Line", 2);
	glutAddMenuEntry("Frontface Point", 3);
	glutAddMenuEntry("Backface Fill", 4);
	glutAddMenuEntry("Backface Line", 5);
	glutAddMenuEntry("Backface Point", 6);

	GLint MyMainMenuID = glutCreateMenu(MyMainMenu);
	glutAddSubMenu("Motion", MySubMenuID);
	glutAddSubMenu("Camera", MySubMenuID2);
	glutAddSubMenu("BackfaceCulling", MySubMenuID3);
	glutAddSubMenu("PolygonMode", MySubMenuID4);
	glutAddMenuEntry("Reset", 1);
	glutAddMenuEntry("System Info", 2);
	glutAddMenuEntry("Exit", 0);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutKeyboardFunc(MyKeyboard);
	glutDisplayFunc(MyDisplay);
	glutReshapeFunc(MyReshape);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);

	glOrtho (-1, 1, -1, 1, -4, 4);
	glutMainLoop();

	delete Head;
	delete Body;
	delete RightArm;
	delete RightHand;
	delete LeftArm;
	delete LeftHand;
	delete RightLeg;
	delete RightFoot;
	delete LeftLeg;
	delete LeftFoot;

	return 0;
}

void meshLoad(){      // mesh 의 회전방향, 이동 위치 좌표를 초기화

	Head = new Mesh();
	Head->readAse("S_Head.ASE");

	Body = new Mesh();
	Body->readAse("S_Body.ASE");

	RightArm = new Mesh();
	RightArm->readAse("S_RightArm.ASE");

	RightHand = new Mesh();
	RightHand->readAse("S_RightHand.ASE");

	LeftArm = new Mesh();
	LeftArm->readAse("S_LeftArm.ASE");

	LeftHand = new Mesh();
	LeftHand->readAse("S_LeftHand.ASE");

	RightLeg = new Mesh();
	RightLeg->readAse("S_RightLeg.ASE");

	RightFoot = new Mesh();
	RightFoot->readAse("S_RightFoot.ASE");

	LeftLeg = new Mesh();
	LeftLeg->readAse("S_LeftLeg.ASE");

	LeftFoot = new Mesh();
	LeftFoot->readAse("S_LeftFoot.ASE");

}

void meshInit() {      // mesh 의 회전방향, 이동 위치 좌표를 초기화
	robotWeight = 10;
	robotSpeed = 20;
	robotRun = false;
	robotRange = 0;
	dRadian = 0;
	sinR = 0;
	cosR = 0;
	sinR2 = 0;
	d_cosR = 0;
	d_sinR = 0;

	sprintf(robotWeightStr, "%.f", robotWeight - 5);
	sprintf(robotWeightStr, "%.f", (float)robotSpeed - 15);

	scaleRate = 1;
	rotateX1 = 0.0f;
	rotateY1 = 0.0f;
	rotateX2 = 0.0f;
	rotateY2 = 0.0f;
	rotateX3 = 0.0f;
	rotateY3 = 0.0f;
	rotateX4 = 0.0f;
	rotateY4 = 0.0f;
	p_near = 0.2;
	p_scale = 1;

	if( Body->max > 1 )
		scale = (float)floor((double)(Body->max+0.5)) * 2;
	else
		scale = 1;
}

void InitLight(){
	// mesh 의 재질 색, 광원 값
	GLfloat mat_specular[ ] = {0.5, 1.0, 0.5, 1.0};
	GLfloat mat_shininess[ ] = {70.0};
	// 조명의 색, 광원 값
	GLfloat light_specular[ ] = {0.8, 0.5, 0.8, 1.0};
	GLfloat light_diffuse[ ] = {0.7, 0.7, 0.7, 1.0};
	GLfloat light_ambient[ ] = {0.4, 0.4, 0.4, 1.0};
	GLfloat light_position[ ] = {-1.5 / scaleRate, 0.5 / scaleRate, 0.5, 0.0};
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);  // 조명 사용 설정
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE); // 정규화
	// mesh 의 재질 색, 광원  및  조명의 색, 광원 설정
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);

	setColor(0.5, 0.5, 0.5, 1.0);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
}

void MyDisplay() {

	if(robotRun == true){
		robotRange += robotSpeed;
		if(robotRange > 360)
			robotRange -= 360;
		dRadian = (PI/180.0) * robotRange;
		sinR = (float)sin(dRadian);
		sinR2 = (float)sin(2*dRadian);
		cosR =  (float)cos(dRadian);

		dRadian = (PI/180.0) * (robotRange - 180);
		d_cosR = (float)cos(dRadian);
		d_sinR = (float)sin(dRadian);
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho (-1 * WidthFactor, 1 * WidthFactor, -1 * HeightFactor, 1 * HeightFactor, -4, 4);

	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity();


	glViewport(0, windowY/2, windowX/2, windowY/2);        // 1 뷰포트
	if(viewSelected == 1){
		setColor(1.0, 0.0, 0.0, 1.0);
		glBegin(GL_LINES);
		glVertex3f(-0.99, -0.99, 0.0);  // 해당 뷰포트 마우스로 클릭되었음을 나타내는 사각형
		glVertex3f(0.99, -0.99, 0.0);  
		glVertex3f(0.99, -0.99, 0.0);  
		glVertex3f(0.99, 0.99, 0.0);  
		glVertex3f(0.99, 0.99, 0.0);  
		glVertex3f(-0.99, 0.99, 0.0); 
		glVertex3f(-0.99, 0.99, 0.0);  
		glVertex3f(-0.99, -0.99, 0.0);  
		glEnd();
	}
	glPushMatrix();
	gluLookAt(0.0, 0.5, 1.0,  0.0, 0.5, 0.0,  0.0, 1.0, 0.0 );

	glPushMatrix();

	glTranslated (0, 0.8, 0);
	glScalef (0.8f, 0.8f, 0.8f);
	glRotatef (rotateY1, 1, 0, 0);
	glRotatef (rotateX1, 0, 1, 0); 

	drawRobot();                  // 로봇 출력
	glPopMatrix();


	drawCoord (1);
	drawGridPlane();

	glPopMatrix();


	glViewport(windowX/2, windowY/2, windowX/2, windowY/2);        // 2 뷰포트
	if(viewSelected == 2){
		setColor(1.0, 0.0, 0.0, 1.0);
		glBegin(GL_LINES);
		glVertex3f(-0.99, -0.99, 0.0);  // 해당 뷰포트 마우스로 클릭되었음을 나타내는 사각형
		glVertex3f(0.99, -0.99, 0.0);  
		glVertex3f(0.99, -0.99, 0.0);  
		glVertex3f(0.99, 0.99, 0.0);  
		glVertex3f(0.99, 0.99, 0.0);  
		glVertex3f(-0.99, 0.99, 0.0); 
		glVertex3f(-0.99, 0.99, 0.0);  
		glVertex3f(-0.99, -0.99, 0.0);  
		glEnd();
	}
	glPushMatrix();
	gluLookAt(1.0, 0.5, 0.0,  0.0, 0.5, 0.0,  0.0, 1.0, 0.0 );

	glPushMatrix();

	glTranslated (0, 0.8, 0);
	glScalef (0.8f, 0.8f, 0.8f);
	glRotatef (rotateY2, 1, 0, 0);
	glRotatef (rotateX2, 0, 1, 0); 

	drawRobot();                  // 로봇 출력
	glPopMatrix();


	drawCoord (1);
	drawGridPlane();

	glPopMatrix();


	glViewport(0, 0, windowX/2, windowY/2);        // 3 뷰포트
	if(viewSelected == 3){
		setColor(1.0, 0.0, 0.0, 1.0);
		glBegin(GL_LINES);
		glVertex3f(-0.99, -0.99, 0.0);  // 해당 뷰포트 마우스로 클릭되었음을 나타내는 사각형
		glVertex3f(0.99, -0.99, 0.0);  
		glVertex3f(0.99, -0.99, 0.0);  
		glVertex3f(0.99, 0.99, 0.0);  
		glVertex3f(0.99, 0.99, 0.0);  
		glVertex3f(-0.99, 0.99, 0.0); 
		glVertex3f(-0.99, 0.99, 0.0);  
		glVertex3f(-0.99, -0.99, 0.0);  
		glEnd();
	}
	glPushMatrix();
	gluLookAt(0.0, 1.0, 0.0,  0.0, 0.0, 0.0,  0.0, 0.0, -1.0 );

	glPushMatrix();

	glTranslated (0, 0.8, 0);
	glScalef (0.8f, 0.8f, 0.8f);
	glRotatef (rotateY3, 1, 0, 0);
	glRotatef (rotateX3, 0, 1, 0); 

	drawRobot();                  // 로봇 출력
	glPopMatrix();


	drawCoord (1);
	drawGridPlane();

	glPopMatrix();		




	glViewport(windowX/2, 0, windowX/2, windowY/2);        // 4 뷰포트
	if(viewSelected == 4){
		setColor(1.0, 0.0, 0.0, 1.0);
		glBegin(GL_LINES);
		glVertex3f(-0.99, -0.99, 0.0);  // 해당 뷰포트 마우스로 클릭되었음을 나타내는 사각형
		glVertex3f(0.99, -0.99, 0.0);  
		glVertex3f(0.99, -0.99, 0.0);  
		glVertex3f(0.99, 0.99, 0.0);  
		glVertex3f(0.99, 0.99, 0.0);  
		glVertex3f(-0.99, 0.99, 0.0); 
		glVertex3f(-0.99, 0.99, 0.0);  
		glVertex3f(-0.99, -0.99, 0.0);  
		glEnd();
	}

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glFrustum(-0.1 * WidthFactor, 0.1 * WidthFactor, -0.1 * HeightFactor, 0.1 * HeightFactor, p_near, 20);
	//	gluPerspective(30, 2.0, 0.2, 20.0);

	glMatrixMode (GL_MODELVIEW);

	glPushMatrix();

	gluLookAt(camEyeX, camEyeY, camEyeZ, camAtX, camAtY, camAtZ, camUpX, camUpY, camUpZ);

	glScalef (1.0f / scaleRate, 1.0f / scaleRate, 1.0f / scaleRate);
	glPushMatrix();

	glTranslated (0, 0.8, 0);
	glScalef (0.8f / p_scale, 0.8f / p_scale, 0.8f / p_scale);
	//	glScalef (1.0f / p_scale, 1.0f / p_scale, 1.0f / p_scale);
	//	glRotatef (rotateY4, 1, 0, 0);
	//	glRotatef (rotateX4, 0, 1, 0); 

	drawRobot();                  // 로봇 출력
	glPopMatrix();

	drawCoord (1);
	drawGridPlane();

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glutSwapBuffers();
	glFlush();
}

void MyReshape(int NewWidth, int NewHeight) {
	WidthFactor = (GLfloat)NewWidth / (GLfloat)800;
	HeightFactor = (GLfloat)NewHeight / (GLfloat)800;

	windowX = NewWidth;
	windowY = NewHeight;
}

void MyKeyboard(unsigned char KeyPressed, int X, int Y){
	switch(KeyPressed){
	case 'Q':
		exit(0);
		break;
	case 'q':
		exit(0);
		break;
	case 'W':
		if(robotSpeed < 35)
			robotSpeed++;
		break;
	case 'w':
		if(robotSpeed < 35)
			robotSpeed++;
		break;
	case 'S':
		if(robotSpeed > 16)
			robotSpeed--;
		break;
	case 's':
		if(robotSpeed > 16)
			robotSpeed--;
		break;
	case 'A':
		if(robotWeight < 20)
			robotWeight++;
		break;
	case 'a':
		if(robotWeight < 20)
			robotWeight++;
		break;
	case 'D':
		if(robotWeight > 6)
			robotWeight--;
		break;
	case 'd':
		if(robotWeight > 6)
			robotWeight--;
		break;
	case 'E':
		if(robotRun == true){
			robotRun = false;
		}
		else {
			robotRun = true;
			glutPostRedisplay();
			glutTimerFunc(40, MyTimer, 1);  // 타이머 시작
		} 
		break;
	case 'e':
		if(robotRun == true){
			robotRun = false;
		}
		else {
			robotRun = true;
			glutPostRedisplay();
			glutTimerFunc(40, MyTimer, 1);  // 타이머 시작
		}
		break;
	case 'R':
		meshInit();      // 기본 위치로 
		glutPostRedisplay();
		break;
	case 'r':
		meshInit();      // 기본 위치로 
		glutPostRedisplay();
		break;
	case 'Z':
		if(scaleRate > 0.5)
			scaleRate -= 0.25;
		glutPostRedisplay();
		break;
	case 'z':
		if(scaleRate > 0.5)
			scaleRate -= 0.25;
		glutPostRedisplay();
		break;
	case 'X':
		if(scaleRate < 3)
			scaleRate += 0.25;
		glutPostRedisplay();
		break;
	case 'x':
		if(scaleRate < 3)
			scaleRate += 0.25;
		glutPostRedisplay();
		break;
	}
}


void MyTimer(int Value){
	if(robotRun){                       // 달리는 모션이면 타이머 함수 적용
		glutPostRedisplay();
		glutTimerFunc(40, MyTimer, 1);
	}
}

void MySubMenu(int entryID){
	switch(entryID){
	case 1:                               // 줌인
		if(scaleRate > 0.5)
			scaleRate -= 0.25;
		glutPostRedisplay();
		break;
	case 2:                               // 줌아웃
		if(scaleRate < 2)
			scaleRate += 0.25;
		glutPostRedisplay();
		break;
	case 3:
		if(robotRun == false){
			robotRun = true;
			glutPostRedisplay();
			glutTimerFunc(40, MyTimer, 1);  // 타이머 시작
		}
		break;
	case 4:
		if(robotRun == true)
			robotRun = false;
		break;
	}
}
void MySubMenu2(int entryID){              // 카메라 선택
	switch(entryID){
	case 1:
		setCamera(0.0, 1.5, 2.0,  0.0, 0.0, -0.5, 0.0, 1.0, 0.0);
		glutPostRedisplay();
		break;
	case 2:
		setCamera(2.0, -0.5, 2.0,  0.5, 0.2, 0.5, 0.0, 1.0, 0.0);
		glutPostRedisplay();
		break;
	case 3:
		setCamera(-2.0, 0.5, -2.0,  -0.5, 0.2, -0.5, 0.0, 1.0, 0.0);
		glutPostRedisplay();
		break;
	case 4:
		setCamera(2.0, 2.0, 2.0,  0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
		glutPostRedisplay();
		break;
	case 5:
		setCamera(2.0, 1.0, 2.0,  0.5, 0.5, 0.5, 0.0, 1.0, 0.0);
		glutPostRedisplay();
		break;
	}
}
void MySubMenu3(int entryID){              // Backface culling 선택
	switch(entryID){
	case 1:
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glutPostRedisplay();
		break;
	case 2:
		glDisable(GL_CULL_FACE);
		glutPostRedisplay();
		break;
	}
}
void MySubMenu4(int entryID){              // 폴리곤 모드
	switch(entryID){
	case 1:
		glPolygonMode(GL_FRONT, GL_FILL);
		glutPostRedisplay();
		break;
	case 2:
		glPolygonMode(GL_FRONT, GL_LINE);
		glutPostRedisplay();
		break;
	case 3:
		glPolygonMode(GL_FRONT, GL_POINT);
		glutPostRedisplay();
		break;
	case 4:
		glPolygonMode(GL_BACK, GL_FILL);
		glutPostRedisplay();
		break;
	case 5:
		glPolygonMode(GL_BACK, GL_LINE);
		glutPostRedisplay();
		break;
	case 6:
		glPolygonMode(GL_BACK, GL_POINT);
		glutPostRedisplay();
		break;
	}
}
void MyMainMenu(int entryID){
	switch(entryID){
	case 0:
		exit(0);
		break;
	case 1:
		meshInit();      // 기본 위치로 
		glutPostRedisplay();
		break;
	case 2:
		glGetIntegerv(GL_MAX_CLIP_PLANES, &systemInt);
		cout << "--- System Info ---\nGL_MAX_CLIP_PLANES : " << systemInt << endl;
		glGetIntegerv(GL_DEPTH_BITS, &systemInt);
		cout << "GL_DEPTH_BITS : " << systemInt << endl;
		glGetIntegerv(GL_RED_BITS, &systemInt);
		cout << "GL_RED_BITS : " << systemInt << endl;
		glGetIntegerv(GL_GREEN_BITS, &systemInt);
		cout << "GL_GREEN_BITS : " << systemInt << endl;
		glGetIntegerv(GL_BLUE_BITS, &systemInt);
		cout << "GL_BLUE_BITS : " << systemInt << endl;
	}
}

void setColor(float r, float g, float b, float a){

	mat_diffuse[0] = r;
	mat_diffuse[1] = g;
	mat_diffuse[2] = b;
	mat_diffuse[3] = a;

	mat_ambient[0] = r;
	mat_ambient[1] = g;
	mat_ambient[2] = b;
	mat_ambient[3] = a;

	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
}

void setCamera(float EyeX, float EyeY, float EyeZ, float AtX, float AtY, float AtZ, float UpX, float UpY, float UpZ){
	camEyeX = EyeX;
	camEyeY = EyeY;
	camEyeZ = EyeZ;
	camAtX = AtX;
	camAtY = AtY;
	camAtZ = AtZ;
	camUpX = UpX;
	camUpY = UpY;
	camUpZ = UpZ;
}

void drawRobot(){

	glPushMatrix();
	setColor(0.5, 0.8, 0.8, 1.0);
	glTranslated(0.0, (sinR2 * 0.08 + 0.08), 0);      // 몸통의 위치 설정
	//		glScalef (1.0f, 1.0f, 1.0f);
	glRotatef (sinR2*(20-robotWeight) + 10, 1, 0, 0);       // 몸통의 상하회전 설정
	glRotatef (sinR*(18-robotWeight), 0, 1, 0);             // 몸통의 좌우회전 설정

	drawBody();                            // 몸통 출력

	glPushMatrix();
	setColor(0.8, 0.8, 0.8, 1.0);
	glTranslated(0.0, 0.45, 0.05);
	glScalef (1.0f, 1.0f, 1.0f);
	//	glRotatef (rotateY, 1, 0, 0);
	glRotatef (cosR*20, 0, 1, 0);

	drawHead();                       // 머리 출력

	glPopMatrix();

	glPushMatrix();
	setColor(0.4, 0.4, 0.8, 1.0);
	glTranslated(-0.25, 0.32, 0.0);
	glScalef (1.0f, 1.0f, 1.0f);
	glRotatef (cosR*(70-robotWeight), 1, 0, 0);
	glRotatef (sinR*5 + 5, 0, 1, 0);

	drawRightArm();                                   // 오른팔 상단 출력

	glPushMatrix();
	setColor(0.8, 0.8, 0.8, 1.0);
	glTranslated(-0.05, -0.3, 0.0);
	glScalef (1.0f, 1.0f, 1.0f);
	glRotatef (cosR*(50-robotWeight) - 70, 1, 0, 0);
	glRotatef (sinR*20 + (20-robotWeight), 0, 1, 0);

	drawRightHand();                               // 오른팔 하단 출력

	glPopMatrix();

	glPopMatrix();

	glPushMatrix();
	setColor(0.4, 0.4, 0.8, 1.0);
	glTranslated(0.25, 0.32, 0.0);
	glScalef (1.0f, 1.0f, 1.0f);
	glRotatef (d_cosR*(70-robotWeight), 1, 0, 0);
	glRotatef (d_sinR*5 - 5, 0, 1, 0);

	drawLeftArm();                                // 왼팔 상단 출력

	glPushMatrix();
	setColor(0.8, 0.8, 0.8, 1.0);
	glTranslated(0.05, -0.3, 0.0);
	glScalef (1.0f, 1.0f, 1.0f);
	glRotatef (d_cosR*(50-robotWeight) - 70, 1, 0, 0);
	glRotatef (d_sinR*20 -(20-robotWeight), 0, 1, 0);

	drawLeftHand();                           // 왼팔 하단 출력

	glPopMatrix();

	glPopMatrix();

	glPushMatrix();
	setColor(0.8, 0.8, 0.8, 1.0);
	glTranslated(-0.12, -0.1, 0.0);
	glScalef (1.0f, 1.0f, 1.0f);
	glRotatef (d_cosR*(50-robotWeight) - 35, 1, 0, 0);      
	//glRotatef (0, 0, 1, 0);

	drawRightLeg();                            // 오른다리 허벅지 출력

	glPushMatrix();
	setColor(0.5, 0.5, 0.6, 1.0);
	glTranslated(0.0, -0.40, 0.0);
	glScalef (1.0f, 1.0f, 1.0f);
	glRotatef (d_sinR * 50 + (60 - robotWeight), 1, 0, 0);   
	//	glRotatef (robotRange/20, 0, 1, 0);

	drawRightFoot();                    // 오른다리 다리,발 출력

	glPopMatrix();

	glPopMatrix();

	glPushMatrix();
	setColor(0.8, 0.8, 0.8, 1.0);
	glTranslated(0.12, -0.1, 0.0);
	glScalef (1.0f, 1.0f, 1.0f);
	glRotatef (cosR*(50-robotWeight) - 35, 1, 0, 0);    
	//glRotatef (robotRange/20, 0, 1, 0);

	drawLeftLeg();                        // 왼다리 허벅지 출력

	glPushMatrix();
	setColor(0.5, 0.5, 0.6, 1.0);
	glTranslated(0.0, -0.40, 0.0);
	glScalef (1.0f, 1.0f, 1.0f);
	glRotatef (sinR * 50 + (60 - robotWeight), 1, 0, 0);
	//	glRotatef (-1 * (robotRange/20), 0, 1, 0);

	drawLeftFoot();                  // 왼다리 다리,발 출력

	glPopMatrix();

	glPopMatrix();

	glPopMatrix();
}

void drawHead(){
	glBegin(GL_TRIANGLES);	
	for(int i=0; i<Head->faceNum; i++){
		glNormal3f(Head->normalList[i].faceNormal[0], Head->normalList[i].faceNormal[1], Head->normalList[i].faceNormal[2]);

		glVertex3f(Head->vertexList[Head->faceList[i].vertexIndex[0]].x / scale, 
			Head->vertexList[Head->faceList[i].vertexIndex[0]].y / scale, 
			Head->vertexList[Head->faceList[i].vertexIndex[0]].z / scale);	

		glVertex3f(Head->vertexList[Head->faceList[i].vertexIndex[1]].x / scale, 
			Head->vertexList[Head->faceList[i].vertexIndex[1]].y / scale, 
			Head->vertexList[Head->faceList[i].vertexIndex[1]].z / scale);

		glVertex3f(Head->vertexList[Head->faceList[i].vertexIndex[2]].x / scale, 
			Head->vertexList[Head->faceList[i].vertexIndex[2]].y / scale, 
			Head->vertexList[Head->faceList[i].vertexIndex[2]].z / scale);
	}
	glEnd( );
}
void drawBody(){
	glBegin(GL_TRIANGLES);	
	for(int i=0; i<Body->faceNum; i++){
		glNormal3f(Body->normalList[i].faceNormal[0], Body->normalList[i].faceNormal[1], Body->normalList[i].faceNormal[2]);

		glVertex3f(Body->vertexList[Body->faceList[i].vertexIndex[0]].x / scale, 
			Body->vertexList[Body->faceList[i].vertexIndex[0]].y / scale, 
			Body->vertexList[Body->faceList[i].vertexIndex[0]].z / scale);	

		glVertex3f(Body->vertexList[Body->faceList[i].vertexIndex[1]].x / scale, 
			Body->vertexList[Body->faceList[i].vertexIndex[1]].y / scale, 
			Body->vertexList[Body->faceList[i].vertexIndex[1]].z / scale);

		glVertex3f(Body->vertexList[Body->faceList[i].vertexIndex[2]].x / scale, 
			Body->vertexList[Body->faceList[i].vertexIndex[2]].y / scale, 
			Body->vertexList[Body->faceList[i].vertexIndex[2]].z / scale);
	}
	glEnd( );
}
void drawRightArm(){
	glBegin(GL_TRIANGLES);	
	for(int i=0; i<RightArm->faceNum; i++){
		glColor3f(0.8, 0.8, 0.8);
		glNormal3f(RightArm->normalList[i].faceNormal[0], RightArm->normalList[i].faceNormal[1], RightArm->normalList[i].faceNormal[2]);

		glVertex3f(RightArm->vertexList[RightArm->faceList[i].vertexIndex[0]].x / scale, 
			RightArm->vertexList[RightArm->faceList[i].vertexIndex[0]].y / scale, 
			RightArm->vertexList[RightArm->faceList[i].vertexIndex[0]].z / scale);	

		glVertex3f(RightArm->vertexList[RightArm->faceList[i].vertexIndex[1]].x / scale, 
			RightArm->vertexList[RightArm->faceList[i].vertexIndex[1]].y / scale, 
			RightArm->vertexList[RightArm->faceList[i].vertexIndex[1]].z / scale);

		glVertex3f(RightArm->vertexList[RightArm->faceList[i].vertexIndex[2]].x / scale, 
			RightArm->vertexList[RightArm->faceList[i].vertexIndex[2]].y / scale, 
			RightArm->vertexList[RightArm->faceList[i].vertexIndex[2]].z / scale);
	}
	glEnd( );
}
void drawRightHand(){
	glBegin(GL_TRIANGLES);	
	for(int i=0; i<RightHand->faceNum; i++){
		glNormal3f(RightHand->normalList[i].faceNormal[0], RightHand->normalList[i].faceNormal[1], RightHand->normalList[i].faceNormal[2]);

		glVertex3f(RightHand->vertexList[RightHand->faceList[i].vertexIndex[0]].x / scale, 
			RightHand->vertexList[RightHand->faceList[i].vertexIndex[0]].y / scale, 
			RightHand->vertexList[RightHand->faceList[i].vertexIndex[0]].z / scale);	

		glVertex3f(RightHand->vertexList[RightHand->faceList[i].vertexIndex[1]].x / scale, 
			RightHand->vertexList[RightHand->faceList[i].vertexIndex[1]].y / scale, 
			RightHand->vertexList[RightHand->faceList[i].vertexIndex[1]].z / scale);

		glVertex3f(RightHand->vertexList[RightHand->faceList[i].vertexIndex[2]].x / scale, 
			RightHand->vertexList[RightHand->faceList[i].vertexIndex[2]].y / scale, 
			RightHand->vertexList[RightHand->faceList[i].vertexIndex[2]].z / scale);
	}
	glEnd( );
}
void drawLeftArm(){
	glBegin(GL_TRIANGLES);	
	for(int i=0; i<LeftArm->faceNum; i++){
		glNormal3f(LeftArm->normalList[i].faceNormal[0], LeftArm->normalList[i].faceNormal[1], LeftArm->normalList[i].faceNormal[2]);

		glVertex3f(LeftArm->vertexList[LeftArm->faceList[i].vertexIndex[0]].x / scale, 
			LeftArm->vertexList[LeftArm->faceList[i].vertexIndex[0]].y / scale, 
			LeftArm->vertexList[LeftArm->faceList[i].vertexIndex[0]].z / scale);	

		glVertex3f(LeftArm->vertexList[LeftArm->faceList[i].vertexIndex[1]].x / scale, 
			LeftArm->vertexList[LeftArm->faceList[i].vertexIndex[1]].y / scale, 
			LeftArm->vertexList[LeftArm->faceList[i].vertexIndex[1]].z / scale);

		glVertex3f(LeftArm->vertexList[LeftArm->faceList[i].vertexIndex[2]].x / scale, 
			LeftArm->vertexList[LeftArm->faceList[i].vertexIndex[2]].y / scale, 
			LeftArm->vertexList[LeftArm->faceList[i].vertexIndex[2]].z / scale);
	}
	glEnd( );
}
void drawLeftHand(){
	glBegin(GL_TRIANGLES);	
	for(int i=0; i<LeftHand->faceNum; i++){
		glNormal3f(LeftHand->normalList[i].faceNormal[0], LeftHand->normalList[i].faceNormal[1], LeftHand->normalList[i].faceNormal[2]);

		glVertex3f(LeftHand->vertexList[LeftHand->faceList[i].vertexIndex[0]].x / scale, 
			LeftHand->vertexList[LeftHand->faceList[i].vertexIndex[0]].y / scale, 
			LeftHand->vertexList[LeftHand->faceList[i].vertexIndex[0]].z / scale);	

		glVertex3f(LeftHand->vertexList[LeftHand->faceList[i].vertexIndex[1]].x / scale, 
			LeftHand->vertexList[LeftHand->faceList[i].vertexIndex[1]].y / scale, 
			LeftHand->vertexList[LeftHand->faceList[i].vertexIndex[1]].z / scale);

		glVertex3f(LeftHand->vertexList[LeftHand->faceList[i].vertexIndex[2]].x / scale, 
			LeftHand->vertexList[LeftHand->faceList[i].vertexIndex[2]].y / scale, 
			LeftHand->vertexList[LeftHand->faceList[i].vertexIndex[2]].z / scale);
	}
	glEnd( );
}
void drawRightLeg(){
	glBegin(GL_TRIANGLES);	
	for(int i=0; i<RightLeg->faceNum; i++){
		glNormal3f(RightLeg->normalList[i].faceNormal[0], RightLeg->normalList[i].faceNormal[1], RightLeg->normalList[i].faceNormal[2]);

		glVertex3f(RightLeg->vertexList[RightLeg->faceList[i].vertexIndex[0]].x / scale, 
			RightLeg->vertexList[RightLeg->faceList[i].vertexIndex[0]].y / scale, 
			RightLeg->vertexList[RightLeg->faceList[i].vertexIndex[0]].z / scale);	

		glVertex3f(RightLeg->vertexList[RightLeg->faceList[i].vertexIndex[1]].x / scale, 
			RightLeg->vertexList[RightLeg->faceList[i].vertexIndex[1]].y / scale, 
			RightLeg->vertexList[RightLeg->faceList[i].vertexIndex[1]].z / scale);

		glVertex3f(RightLeg->vertexList[RightLeg->faceList[i].vertexIndex[2]].x / scale, 
			RightLeg->vertexList[RightLeg->faceList[i].vertexIndex[2]].y / scale, 
			RightLeg->vertexList[RightLeg->faceList[i].vertexIndex[2]].z / scale);
	}
	glEnd( );
}
void drawRightFoot(){
	glBegin(GL_TRIANGLES);	
	for(int i=0; i<RightFoot->faceNum; i++){
		glNormal3f(RightFoot->normalList[i].faceNormal[0], RightFoot->normalList[i].faceNormal[1], RightFoot->normalList[i].faceNormal[2]);

		glVertex3f(RightFoot->vertexList[RightFoot->faceList[i].vertexIndex[0]].x / scale, 
			RightFoot->vertexList[RightFoot->faceList[i].vertexIndex[0]].y / scale, 
			RightFoot->vertexList[RightFoot->faceList[i].vertexIndex[0]].z / scale);	

		glVertex3f(RightFoot->vertexList[RightFoot->faceList[i].vertexIndex[1]].x / scale, 
			RightFoot->vertexList[RightFoot->faceList[i].vertexIndex[1]].y / scale, 
			RightFoot->vertexList[RightFoot->faceList[i].vertexIndex[1]].z / scale);

		glVertex3f(RightFoot->vertexList[RightFoot->faceList[i].vertexIndex[2]].x / scale, 
			RightFoot->vertexList[RightFoot->faceList[i].vertexIndex[2]].y / scale, 
			RightFoot->vertexList[RightFoot->faceList[i].vertexIndex[2]].z / scale);
	}
	glEnd( );
}
void drawLeftLeg(){
	glBegin(GL_TRIANGLES);	
	for(int i=0; i<LeftLeg->faceNum; i++){
		glNormal3f(LeftLeg->normalList[i].faceNormal[0], LeftLeg->normalList[i].faceNormal[1], LeftLeg->normalList[i].faceNormal[2]);

		glVertex3f(LeftLeg->vertexList[LeftLeg->faceList[i].vertexIndex[0]].x / scale, 
			LeftLeg->vertexList[LeftLeg->faceList[i].vertexIndex[0]].y / scale, 
			LeftLeg->vertexList[LeftLeg->faceList[i].vertexIndex[0]].z / scale);	

		glVertex3f(LeftLeg->vertexList[LeftLeg->faceList[i].vertexIndex[1]].x / scale, 
			LeftLeg->vertexList[LeftLeg->faceList[i].vertexIndex[1]].y / scale, 
			LeftLeg->vertexList[LeftLeg->faceList[i].vertexIndex[1]].z / scale);

		glVertex3f(LeftLeg->vertexList[LeftLeg->faceList[i].vertexIndex[2]].x / scale, 
			LeftLeg->vertexList[LeftLeg->faceList[i].vertexIndex[2]].y / scale, 
			LeftLeg->vertexList[LeftLeg->faceList[i].vertexIndex[2]].z / scale);
	}
	glEnd( );
}
void drawLeftFoot(){
	glBegin(GL_TRIANGLES);	
	for(int i=0; i<LeftFoot->faceNum; i++){
		glNormal3f(LeftFoot->normalList[i].faceNormal[0], LeftFoot->normalList[i].faceNormal[1], LeftFoot->normalList[i].faceNormal[2]);
		glVertex3f(LeftFoot->vertexList[LeftFoot->faceList[i].vertexIndex[0]].x / scale, 
			LeftFoot->vertexList[LeftFoot->faceList[i].vertexIndex[0]].y / scale, 
			LeftFoot->vertexList[LeftFoot->faceList[i].vertexIndex[0]].z / scale);	
		glVertex3f(LeftFoot->vertexList[LeftFoot->faceList[i].vertexIndex[1]].x / scale, 
			LeftFoot->vertexList[LeftFoot->faceList[i].vertexIndex[1]].y / scale, 
			LeftFoot->vertexList[LeftFoot->faceList[i].vertexIndex[1]].z / scale);
		glVertex3f(LeftFoot->vertexList[LeftFoot->faceList[i].vertexIndex[2]].x / scale, 
			LeftFoot->vertexList[LeftFoot->faceList[i].vertexIndex[2]].y / scale, 
			LeftFoot->vertexList[LeftFoot->faceList[i].vertexIndex[2]].z / scale);
	}
	glEnd( );
}
void drawGridPlane()
{
	int		i;
	double	x, z;

	glLineWidth(1);

	glPushMatrix();
	glBegin (GL_LINES);
	x = -1.0;
	z = -1.0;
	for( i=0 ; i<=10 ; i++ ) {
		setColor(i/20.f, 0, 0, 0.2);
		glVertex3f (x, 0, z);
		glVertex3f (x, 0, z+2);

		setColor(0, 0, i/20.f, 0.2);
		glVertex3f (z, 0, x);
		glVertex3f (z+2, 0, x);
		x += 0.2;
	}
	glEnd();
	glPopMatrix();
}
void drawString (const char *s)
{
	unsigned int i;

	for (i= 0; i < strlen (s); i++)
		glutBitmapCharacter (GLUT_BITMAP_HELVETICA_12, s[i]);
}

void drawCoord(int lineWidth)
{
	glLineWidth(lineWidth);

	glPushMatrix();
	glScaled (0.9, 0.9, 0.9);
	glBegin (GL_LINES);
	setColor(1.0f, 0, 0, 0);
	//	glColor3f  (1.0f, 0, 0);
	glVertex3f (0.0f, 0, 0);
	glVertex3f (1.0f, 0, 0);	// RED   ==> +x axis

	setColor(0, 1.0f, 0, 0);
	//	glColor3f  (0, 1.0f, 0);
	glVertex3f (0, 0.0f, 0);
	glVertex3f (0, 1.0f, 0);	// GREEN ==> +y axis

	setColor(0, 0, 1.0f, 0);
	//	glColor3f  (0, 0, 1.0f);
	glVertex3f (0, 0, 0.0f);
	glVertex3f (0, 0, 1.0f);	// BLUE  ==> +z axis
	glEnd();

	glLineWidth(1);

	setColor(1.0f, 0, 0, 0);
	glRasterPos3f (1.0f, 0, 0);
	drawString ("x");

	setColor(0, 1.0f, 0, 0);
	glRasterPos3f (0, 1.0f, 0);
	drawString ("y");

	setColor(0, 1.0f, 0, 0);
	glRasterPos3f (0, 0, 1.0f);
	drawString ("z");

	glPopMatrix();
}


void mouse(int button, int state, int x, int y)
{
	sx = x;
	sy = y;
	if(state == GLUT_DOWN){                 // 뷰포트를 클릭 시
		if( x < windowX/2 ){
			if( y > (windowY/2))
				viewSelected = 3;
			else
				viewSelected = 1;
		}
		else {
			if( y > (windowY/2))
				viewSelected = 4;
			else
				viewSelected = 2;
		}
	}
	glutPostRedisplay();

}

void motion(int x, int y)
{
	switch(viewSelected){                 // 선택된 뷰포트
	case 1:
		rotateX1 += (x-sx)/2;
		rotateY1 += (y-sy)/2;
		break;
	case 2:
		rotateX2 += (x-sx)/2;
		rotateY2 += (y-sy)/2;
		break;
	case 3:
		rotateX3 += (x-sx)/2;
		rotateY3 += (y-sy)/2;
		break;
	case 4:
		p_scale += (x-sx)/200;      // 마우스 좌우이동 시 로봇의 사이즈 조정
		p_near += (y-sy)/200;       // 마우스 상하이동 시 원근투상의 near 값 조정
		//rotateX4 += (x-sx)/2;
		//rotateY4 += (y-sy)/2;
		break;
	}
	sx = x;
	sy = y;
	if(robotRun == false)
		glutPostRedisplay(); // display();
}
