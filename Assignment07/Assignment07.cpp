/*******************************
2010136019 김동현 Assignment07
*******************************/
/*cmd창에 인터페이스를 적용하였습니다.*/
/*
'q' : 프로그램 종료
'w' : Wire 이미지 변환
'e' : Solid 이미지 변환
'1' : ORTHO 시점 모드
'2' : FRUSTUM 시점 모드
'3' : PERSPECTIVE 시점 모드
'a' : RUN 모드
's' : JAP 모드
'd' : ROCKET 모드
'f' : SHOW 모드
'z' : 애니메이션 일시중지/재개
'9' : 카메라 시점 매개변수 변경
'0' : PROJECTION 매개변수 변경
'M' : 메뉴 다시보기
'J', 'K', 'L' : 카메라 시점에서 변경할 매개변수 선택
'U', 'I', 'O', 'P', '[', ']' : PROJECTION에서 변경할 매개변수 선택
'+' : 해당 매개변수 0.1씩 증가
'-' : 해당 매개변수 0.1씩 감소
*/

/*
필수적인 헤더 파일들
*/
#include <iostream>
#include <stdlib.h>
#include <windows.h>
#include <MMSystem.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
using namespace std;

static double time=0;  // Run_time 변수
static double time2=0; // Jap_time 변수
static double time3=0; // Rocket_time 변수
static double time4=0; // ground_time 변수
static double time6=0; // exit_time 변수

GLfloat R_Arm_x=0; // 오른쪽 어깨
GLfloat R_Arm_y=0; // 오른쪽 팔
GLfloat L_Arm_x=0; // 왼쪽 어깨
GLfloat L_Arm_y=0; // 왼쪽 팔
GLfloat R_Leg_x=0; // 오른 허벅지
GLfloat R_Leg_y=0; // 오른 종아리
GLfloat L_Leg_x=0; // 왼 허벅지
GLfloat L_Leg_y=0; // 왼 종아리
GLfloat R=0; // 왼쪽 로켓 펀치 움직임 변수
GLfloat R2=0;// 오른쪽 로켓 펀치 움직임 변수

GLUquadricObj* cyl;// 실린더 객체 선언

int a=0; // x축 기준(값이 1일 때 x축을 기준으로 회전)
int b=0; // y축 기준(값이 1일 때 y축을 기준으로 회전)
int c=0; // z축 기준(값이 1일 때 z축을 기준으로 회전)

static int flag=0; // wireframe 모드와 solid rendering 모드 구분 변수
static int key=1;  // 동작 모드와 색상 변경 변수

GLint WinWidth = 0, WinHeight = 0;	// reshape를 위한 화면의 종횡크기 전역변수
/*다음은 PROJECTION 및 MODELVIEW를 위한 매개변수들이다.*/
GLdouble pleft=-1.0, pright=1.0, bottom=-1.0, top=1.0, glNear=-1.0, glFar=1.0;	//glOrtho와 glFrustum의 공통 매개변수
GLdouble fovy=100.0, aspect = 1.0, gluNear=-1.0, gluFar=1.0;	//gluPerspective의 매개변수
GLdouble eye_x=0.0, eye_y=0.0, eye_z=1.0;	//gluLookAt의 매개변수
/*GLdouble형으로 매개변수 값의 범위를 조정하면 자료형의 특성상 정확한 조정을 하기
어려우므로 int형으로 정확히 측정한다.*/
int range_left=0, range_right=0, range_bottom=0, range_top=0, range_fovy=0, range_aspect=0; 
int range_glNear=0, range_glFar=0, range_gluNear=0, range_gluFar=0; 
int range_x=0, range_y=0, range_z=10;	
enum{
	NONE, PROJECTION, MODELVIEW
}select_mod = NONE;	//선택한 모드의 매개변수 조정, 초기값은 NONE(선택 이전이므로)
enum{
	ORTHO, FRUSTUM, PERSPECTIVE
}mode = ORTHO;	//시점 모드 변수, 초기값은 ORTHO
enum{
	LEFT, RIGHT, BOTTOM, TOP, FOVY, ASPECT, zNear, zFar
}proj_co = LEFT;	//프로젝션 모드에서 조정할 매개변수 지정
enum{
	X, Y, Z
}eye_co = X;	//gluLookAt()에서 eye_x, eye_y, eye_z 중 조정할 매개변수 지정

#define RUN		1					
#define JAP		2							
#define ROCKET	3	
#define SHOW	4
#define FLY		5
#define PAUSE	6	

void redisplay_all();
void key_operation(unsigned char);
/*
메뉴얼 출력 함수
*/
void print_menual(void){
	system("cls");	//cmd 화면 초기화
	cout << "--- Control Key ---\nRun : A\nJap : S\nRocket Punch : D\nSkating Motion : F\nFlying Motion : G\n\nORTHO VIEW : 1\n";
	cout << "FRUSTUM VIEW : 2\nPERSPECTIVE VIEW : 3\n\nWireframe : W\nSolidframe : E\n\nPAUSE/RESUME: Z\nQUIT : Q\n\n";
	cout << "카메라 시점 매개변수 변경 : 9\nPROJECTION 매개변수 변경 : 0\n\n메뉴 다시보기 : M\n" << endl;
	cout<<"매개변수를 조정할 모드를 선택하세요.[9/0]";
}
void glInit(void){ // 지엘 초기화 함수 
	glEnable(GL_DEPTH_TEST); // 깊이 테스팅 사용
	glEnable(GL_NORMALIZE);  // 정규화
	glEnable(GL_SMOOTH);     // 각 정점의 색상을 부드럽게 연결하여 칠해지도록  하므로, 각정점에 적용된 색상이 혼합되어 적용된다. 
	glEnable(GL_LIGHTING);   // 지엘 조명. 빛을 받는 각도에 따라 로봇 색상이 변화

	/* 빛을 받는 각도에 따라 로봇 색깔의 변화를 위한 설정 */
	GLfloat ambientLight[]={0.3f,0.3f,0.3f,1.0f};
	GLfloat diffuseLight[]={0.7f,0.7f,0.7f,1.0f};
	GLfloat specular[]={1.0f,1.0f,1.0f,1.0f};
	GLfloat specref[]={1.0f,1.0f,1.0f,1.0f};
	GLfloat position[]={400.0,300.0,-700.0,1.0};
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glMateriali(GL_FRONT, GL_SHININESS, 128);

	glClearColor(1.0,1.0,1.0,1.0); // 배경 검은색
}

void Draw_Color(int i){ // 색상을 그리는 함수
	if(i==RUN){
		glColor3f(0.0,1.0,0.0);}
	else if(i==JAP){
		glColor3f(1.0,1.0,0.0);}
	else if(i==ROCKET){
		glColor3f(0.0,1.0,1.0);}
	else if(i==SHOW){	
		glColor3f(1.0,0.0,1.0);}
	else if(i==FLY){	
		glColor3f(0.9,0.8,0.6);}
}

void Change_Wire_Or_Solid(int i){          // 솔리드, 와이어 선택
	if(flag==1){
		gluQuadricDrawStyle(cyl,GLU_LINE); // 스타일을 와이어로 변경
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);	// glut로 그린 도형에도 적용
	}
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);	//스타일을 솔리드로 변경
}
/*
로봇 왼팔을 그리는 함수
*/
void DrawL_Arm(int x,int a,int b,int c){
	glPushMatrix();
	Draw_Color(key);       // 색 설정
	cyl=gluNewQuadric();   // 실린더 객체 생성
	glRotatef(x,a, b, c);  // 파라미터 값만큼 회전
	glRotatef(90.0,1.0, 0.0, 0.0); // 실린더를 x축 기준으로 90도 회전(실린더 생성 시 각도 변경을 위해)
	glTranslatef(0.25,0.0,0.0);    // 왼쪽 어깨 시작점으로 이동
	glRotatef(15.0,0.0, 1.0, 0.0); // 왼쪽 어깨를 y축을 기준으로 15도 회전
	Change_Wire_Or_Solid(flag);
	gluCylinder(cyl,0.05,0.05,0.2,50,1); // 실린더를 그림
}

/*
로봇 왼쪽 손을 그리는 함수
*/
void DrawL_Hand(int y,int a,int b, int c){
	glPushMatrix();
	Draw_Color(key);       // 색 설정
	cyl=gluNewQuadric();   // 실린더 객체 생성
	glTranslatef(0.0,0.0,0.22); // 왼손 시작점
	glRotatef(y,a,b, c);        // 파라미터 값만큼 회전
	Change_Wire_Or_Solid(flag);
	if(key == SHOW) gluCylinder(cyl,0.05,0.02,0.2,15,1); // 깔때기 모양 실린더를 그림
	else gluCylinder(cyl,0.05,0.05,0.2,15,1);           // 실린더를 그림
	//glPopMatrix();
}
/*
로봇 왼쪽 주먹
*/
void DrawL_HandRocket(){
	glPushMatrix();
	glColor3f(0, 0.5, 1.0);
	glTranslatef(0,0,R); // 왼쪽 주먹 위치를 파라미터만큼 이동
	Change_Wire_Or_Solid(flag);
	glutSolidCone(0.05, 0.5, 20, 20);
	glPopMatrix();
}
/*
로봇 오른팔을 그리는 함수
*/
void DrawR_Arm(int x, GLfloat a, GLfloat b,GLfloat c){
	glPushMatrix();
	Draw_Color(key);     // 색 설정
	cyl=gluNewQuadric(); // 실린더 객체 생성
	glRotatef(x,a,b,c);  // 파라미터 값만큼 회전
	glRotatef(90.0,1.0, 0.0, 0.0); // 실린더를 x축을 기준으로 90도 회전(실린더 생성 시 각도 변경을 위해)
	glTranslatef(-0.25,0.0,0.0); //우측 어깨 시작점으로 이동
	glRotatef(-15.0,0.0, 1.0, 0.0); //우측 어깨 -15도 y축기준 회전
	Change_Wire_Or_Solid(flag);
	gluCylinder(cyl,0.05,0.05,0.2,50,1);// 실린더 그림

}
/*
로봇 오른쪽 손을 그리는 함수
*/
void DrawR_Hand(int y,int a,int b, int c){
	glPushMatrix();
	Draw_Color(key); // 색 설정
	cyl=gluNewQuadric();//실린더 객체 생성
	glTranslatef(0.0,0.0,0.22);//오른쪽 손 시작점
	glRotatef(y, a , b, c);// 파라미터값 만큼 회전
	Change_Wire_Or_Solid(flag);
	if(key == SHOW) gluCylinder(cyl,0.05,0.02,0.2,50,1);// 깔때기 모양 실린더 그림
	else gluCylinder(cyl,0.05,0.05,0.2,50,1);// 실린더 그림
}
/*
로봇 주먹을 그리는 함수
*/
void DrawFist(){
	glTranslatef(0, 0, 0.25);
	glColor3f(0.6, 0.8, 0.2);
	glutSolidCube(0.1);
	Draw_Color(key);
	glPopMatrix();
}
/*
로봇 글러브를 그리는 함수
*/
void DrawGlove(){
	glTranslatef(0, 0, 0.25);
	glRotatef(45, 0, 0, 1.0);
	glColor3f(0.9, 0.2, 0.2);
	glutSolidCube(0.25);
	Draw_Color(key);
	glPopMatrix();
}
/*
로봇 오른쪽 로켓트 함수
*/
void DrawR_HandRocket(){
	glPushMatrix();
	glColor3f(0, 0.5, 1.0);
	glTranslatef(0,0,R2);//오른주먹 위치 파라미터만큼 이동
	Change_Wire_Or_Solid(flag);
	glutSolidCone(0.05, 0.5, 20, 20);
	glPopMatrix();
}
/*
로봇 몸체를 그리는 함수
*/
void DrawBody(int x,int a,int b,int c){
	glPushMatrix();
	Draw_Color(key);
	cyl=gluNewQuadric();//실린더 객체 생성
	glRotatef(90.0, 1.0, 0.0, 0.0);//실린더 90도 x축기준 회전 (실린더 생성시 각도 변경을 위하여)
	glRotatef(x, a , b, c);//파라미터값 만큼 회전
	Change_Wire_Or_Solid(flag);
	gluCylinder(cyl,0.2,0.2,0.45,50,1);// 실린더 그림
	glPopMatrix();
}
/*
로봇 왼쪽 허벅지를 그리는 함수
*/
void DrawL_Legs(int x, int a,int b,int c){
	glPushMatrix();
	Draw_Color(key);
	cyl=gluNewQuadric();//실린더 객체 생성
	glRotatef(90.0, 1.0, 0.0, 0.0);//실린더 90도 x축기준 회전 (실린더 생성시 각도 변경을 위하여)
	glTranslatef(0.1,0.0,0.42);//왼쪽 허벅지 시작점
	glRotatef(x,a, b, c);//파라미터만큼 회전
	Change_Wire_Or_Solid(flag);
	gluCylinder(cyl,0.05,0.05,0.15,50,1);// 실린더 그림

}
/*
로봇 왼쪽 종아리를 그리는 함수
*/
void DrawL_foot(int y,int a,int b,int c){
	glPushMatrix();
	Draw_Color(key);
	cyl=gluNewQuadric();//실린더 객체 생성
	glTranslatef(0.0,0.0,0.18);//왼쪽 종아리 시작점
	glRotatef(y,a, b, c);//파라미터만큼 회전
	Change_Wire_Or_Solid(flag);
	if(key == SHOW) gluCylinder(cyl,0.05,0.03,0.2,50,1);// 깔때기 모양 실린더 그림
	else gluCylinder(cyl,0.05,0.05,0.2,50,1);// 실린더 그림
}
/*
로봇 신발을 그리는 함수
*/
void DrawShoe(){
	glTranslatef(0, 0, 0.25);
	glColor3f(0.6, 0.0, 0.0);
	glTranslatef(0, 0.048, 0); 
	glScalef(1.0, 1.7, 0.7);
	glutSolidCube(0.1);
	Draw_Color(key);
	glPopMatrix();
}
/*
로봇 스케이트슈즈를 그리는 함수
*/
void DrawSkateShoe(){
	glTranslatef(0, 0, 0.25);
	glColor3f(0.9, 0.9, 0.9);
	glTranslatef(0, 0.048, 0); 
	glScalef(0.5, 1.7, 0.7);
	glutSolidCube(0.1);
	Draw_Color(key);
}
/*
로봇 오른쪽 허벅지를 그리는 함수
*/
void DrawR_Legs(int x,int a,int b,int c){
	glPushMatrix();
	Draw_Color(key);
	cyl=gluNewQuadric();//실린더 객체 생성
	glRotatef(90.0, 1.0, 0.0, 0.0);//실린더 90도 x축기준 회전 (실린더 생성시 각도 변경을 위하여)
	glTranslatef(-0.1,0.0,0.42);//오른쪽 허벅지 시작점
	glRotatef(x,a,b,c);//파라미터만큼 회전
	Change_Wire_Or_Solid(flag);
	gluCylinder(cyl,0.05,0.05,0.15,50,1);// 실린더 그림
}
/*
로봇 오른쪽 종아리를 그리는 함수
*/
void DrawR_foot(int y,int a,int b,int c){
	glPushMatrix();
	Draw_Color(key);
	cyl=gluNewQuadric();//실린더 객체 생성

	glTranslatef(0.0,0.0,0.18);//오른쪽 종아리 시작점
	glRotatef(y,a, b, c);//파라미터만큼 회전

	Change_Wire_Or_Solid(flag);
	if(key == SHOW) gluCylinder(cyl,0.05,0.03,0.2,15,1);// 깔때기 모양 실린더 그림
	else gluCylinder(cyl,0.05,0.05,0.2,15,1);// 실린더 그림
}
/*
로봇 목을 그리는 함수
*/
void Drawneck(){
	glPushMatrix();
	glColor3f(1.0, 0.0,0.3);//색 지정
	cyl=gluNewQuadric();//실린더 객체 생성
	glRotatef(90.0, 1.0, 0.0, 0.0);//실린더 90도 x축기준 회전 (실린더 생성시 각도 변경을 위하여)
	glTranslatef(0.0,0.0,-0.045); // 목 시작점
	Change_Wire_Or_Solid(flag);
	gluCylinder(cyl,0.2,0.2,0.025,100,1);// 실린더 그림
	glPopMatrix();
}
/*
땅을 그리는 함수
땅은 커다란 구로 설정하였다.
땅은 시간에 따라 회전하여서 마치 로봇이 
앞으로 뛰어가는 느낌이 들도록 설계하였다.
*/
void DrawGround(){
	Draw_Color(flag);//색깔 설정
	glTranslatef(0.0,-2.73,0.0);//땅 시작점
	glRotatef(45.0,0,1,0);//45도만큼 y축 방향으로 회전
	glRotatef(0.0+time4-15,0.0, 0.0, 1);// z축 기준으로 time4만큼 땅이 회전 
	Change_Wire_Or_Solid(flag);
	gluSphere(cyl, 2.5, 30, 90);	
}
/*
로봇 머리를 그리는 함수
네모난 머리에 파란색 고글을 썼다.
*/
void DrawHead(){
	glTranslatef(0.0,0.02,0.0);//머리 시작점
	glPushMatrix();// 처음 저장 좌표 다시 저장

	glTranslatef(0, 0.18, 0);
	Draw_Color(key);//색깔 설정
	glutSolidCube(0.3);//머리그리기

	/*파란색 고글*/
	glBegin(GL_POLYGON);
	glColor3f(0, 0, 1);
	glVertex3f(-0.1, 0.1, 0.16);
	glVertex3f(0.1, 0.1, 0.16);
	glVertex3f(0.1, 0.01, 0.16);
	glVertex3f(-0.1, 0.01, 0.16);
	glEnd();		

	glPopMatrix();// 처음 저장 좌표 다시 저장
}
/*
안드로이드 전체 모습을 그리는 함수
*/
void DrawAndroid(){
	DrawBody(0,0,0,0); // 몸통 함수 호출
	Drawneck(); // 목함수 호출
	DrawHead(); // 머리 호출
	DrawR_Arm(R_Arm_x,1,0,0);//우측팔뚝
	if(key==ROCKET) DrawR_HandRocket(); // 로켓모드일때 우측로켓호출
	else{
		DrawR_Hand(R_Arm_y, 1, 0, 0);  //우측팔호출
		if(key==JAP)
			DrawGlove();
		else 
			DrawFist();	//우측주먹
	}
	DrawL_Arm(L_Arm_x,1,0,0);   //좌측팔뚝
	if(key==ROCKET) DrawL_HandRocket(); // 로켓모드일때 우측로켓호출
	else{
		DrawL_Hand(L_Arm_y,1,0,0);//좌측손호출
		if(key==JAP)
			DrawGlove();
		else
			DrawFist();	//좌측주먹
	}
	DrawL_Legs(L_Leg_x,1,0,0);  //좌측다리
	DrawL_foot(L_Leg_y,1,0,0);  //좌측발
	DrawShoe();	//좌측신발
	DrawR_Legs(R_Leg_x,1,0,0);  //우측다리
	DrawR_foot(R_Leg_y,1,0,0);  //우측발	
	DrawShoe();	//우측신발
}
/*
로봇이 달리는 것을 표현한 함수.
로봇의 관절이 움직이는 것을 표현했고,
로봇이 달리면서 상,하,좌,우로 움직이는 모습을 표현했다.
*/
void Run(){
	glLoadIdentity();//CTM 초기화
	/*
	기본적인 로봇의 움직임의 가속도는 sin() 함수를 통해 표현하였다
	또한 관절의 제한범위를 생각하여 abs() 함수를 통해 관절의 움직임을 제한하였다.
	*/
	L_Arm_x=sin(time)*80;           //왼팔은 80도 각도까지 움직이되 sin()함수를 사용하여 주기적인 움직임 설정
	R_Arm_x=-L_Arm_x;               //오른팔은 왼팔반대로 80도각도까지 움직이되 sin()함수를 사용하여 주기적인 움직임 설정.
	R_Arm_y=-abs(sin(time)*60+50);  //우측팔뚝 각도조절(abs절대값을 줌으로써 팔뚝이 뒤로꺾이지 않는 한계점을 설정) 
	L_Arm_y=-abs(-sin(time)*60+50); //좌측팔뚝 각도조절(abs절대값을 줌으로써 팔뚝이 뒤로꺾이지 않는 한계점을 설정) 

	R_Leg_y=abs(-sin(time)*30-30);  //우측종아리 각도조절(abs절대값을 줌으로써 종아리가 앞으로 꺾이지 않는 한계점을 설정)
	L_Leg_y=abs(sin(time)*30-30);   //좌측종아리 각도조절(abs절대값을 줌으로써 종아리가 앞으로 꺾이지 않는 한계점을 설정)
	R_Leg_x=sin(time)*60;          //우측다리는 60도 각도까지 움직이되 sin()함수를 사용하여 주기적인 움직임 설정
	L_Leg_x=-R_Leg_x;              //좌측다리는 우측다리반대로 60도 각도까지 움직이되 sin()함수를 사용하여 주기적인 움직임 설정

	////////////////display////////////////

	cyl=gluNewQuadric(); //실린더 객체 생성
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT); //초기화
	glMatrixMode(GL_MODELVIEW); //모드 설정

	DrawGround();    //지면 호출
	glLoadIdentity();//CTM 초기화
	gluLookAt(eye_x, eye_y, eye_z, 0.0, 0.0, 0.0, 0.0, 1.0, 0);	//카메라 시점

	/*
	로봇이 달리면서 앞,뒤로 뒤뚱거리고 몸이 틀어지는 것을 표현 
	*/
	glRotatef(-230.0, 0, 1, 0);//y축으로 기울임 
	glRotatef(abs(sin(time)*16),1,0,0);//x축으로 16도 까지 각도틀어짐(abs절대값을 줌으로써 몸체가 뒤로 꺾이지 않는 한계점을 설정)
	glRotatef(sin(time)*16,0,1,0); //y축으로 16도 까지 각도틀어짐, sin()함수를 사용하여 주기적인 움직임 설정

	/*
	로봇이 달리면서 상,하로 움직이는 것을 표현
	*/
	float i=0;
	i=abs(sin(time)*0.08); //i변수값 설정
	glPushMatrix();// 처음 저장 좌표 다시 저장
	glTranslatef(0.0,i,0); //변수 i만큼 로봇의 몸체가 Y축기준으로 움직임.

	glTranslatef(0.0,0.5,0.0);//최초 로봇의 위치 
	DrawAndroid();
	glutSwapBuffers();
}
/*
로봇이 잽을 날리는 것을 표현한 함수
*/
void Jap(){
	glLoadIdentity();//CTM 초기화
	/*
	로봇의 기본적인 관절의 움직임 범위를 제한하는 곳
	*/
	L_Arm_x=(-40)+sin(time2)*60;//왼쪽 어깨의 각도시작은 -40상태에서 sin()함수를 사용하여 주기적인 움직임 설정
	R_Arm_x=(-80)-L_Arm_x;      //우측 어깨의 각도시작은 -80상태에서 왼쪽어깨 움직임의 반대로 설정
	R_Arm_y=-abs(cos(time2)*80);  //우측팔뚝 각도조절(팔을 뻗는 움직임표현을위하여 어깨의 sin()함수와 반대인 cos()함수 사용)
	L_Arm_y=-abs(-cos(time2)*80); //좌측팔뚝 각도조절(팔을 뻗는 움직임표현을위하여 어깨의 sin()함수와 반대인 cos()함수 사용)

	R_Leg_y=abs(-sin(time)*30-20);  //우측종아리 각도조절(abs절대값을 줌으로써 종아리가 앞으로 꺾이지 않는 한계점을 설정)
	L_Leg_y=abs(sin(time)*30-20);   //좌측종아리 각도조절(abs절대값을 줌으로써 종아리가 앞으로 꺾이지 않는 한계점을 설정)
	R_Leg_x=sin(time)*30;          //우측다리는 30도 각도까지 움직이되 sin()함수를 사용하여 주기적인 움직임 설정
	L_Leg_x=-R_Leg_x;              //좌측다리는 우측다리반대로 30도 각도까지 움직이되 sin()함수를 사용하여 주기적인 움직임 설정

	////////////////display////////////////

	cyl=gluNewQuadric(); //실린더 객체 생성
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT); //초기화
	glMatrixMode(GL_MODELVIEW); //모드 설정

	DrawGround(); //지면 호출
	glLoadIdentity(); //CTM 초기화
	gluLookAt(eye_x, eye_y, eye_z, 0.0, 0.0, 0.0, 0.0, 1.0, 0);	//카메라 시점

	glRotatef(-230.0, 0, 1, 0);//y축으로 기울임 

	glRotatef(sin(time)*10,0,0,1);//로봇의 좌,우 반동 표현(좌우로 10도 만큼 주기적인 움직임 설정)

	/*
	로봇이 잽을 날리면서 상,하로 움직이는 것을 표현
	*/
	float j=0;    
	j=abs(sin(time2)*0.085);// j값 설정
	glPushMatrix();// 처음 저장 좌표 다시 저장
	glTranslatef(0.0,j,0); //변수 j만큼 로봇의 몸체가 Y축기준으로 움직임.
	glTranslatef(0.0,0.5,0.0);//최초 위치 
	DrawAndroid();	
	glutSwapBuffers();
}
/*
피겨스케이팅 동작을 취하는 함수
*/
void Show()
{
	glLoadIdentity(); //CTM 초기화

	/*
	로봇의 기본적인 관절의 움직임 범위를 제한하는 곳
	*/
	L_Arm_x=(-40)+sin(time2)*60;//왼쪽 어깨의 각도시작은 -40상태에서 sin()함수를 사용하여 주기적인 움직임 설정
	R_Arm_x=(-80)-L_Arm_x;       //우측 어깨의 각도시작은 -80상태에서 왼쪽어깨 움직임의 반대로 설정
	R_Arm_y=-abs(cos(time2)*10);  //우측팔뚝 각도조절(팔을 뻗는 움직임표현을위하여 어깨의 sin()함수와 반대인 cos()함수 사용)
	L_Arm_y=-abs(-cos(time2)*10); //좌측팔뚝 각도조절(팔을 뻗는 움직임표현을위하여 어깨의 sin()함수와 반대인 cos()함수 사용)

	R_Leg_y=abs(-sin(time)*30-30); //우측종아리 각도조절(abs절대값을 줌으로써 종아리가 앞으로 꺾이지 않는 한계점을 설정)
	L_Leg_y=abs(sin(time)*30-30);  //좌측종아리 각도조절(abs절대값을 줌으로써 종아리가 앞으로 꺾이지 않는 한계점을 설정)
	R_Leg_x=sin(time)*60;          //우측다리는 60도 각도까지 움직이되 sin()함수를 사용하여 주기적인 움직임 설정
	L_Leg_x=-R_Leg_x;             //좌측다리는 우측다리반대로 60도 각도까지 움직이되 sin()함수를 사용하여 주기적인 움직임 설정

	////////////////display////////////////

	cyl=gluNewQuadric(); //실린더 객체 생성
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT); //초기화
	glMatrixMode(GL_MODELVIEW); //모드 설정

	DrawGround(); //지면 호출
	glLoadIdentity(); //CTM 초기화
	gluLookAt(eye_x, eye_y, eye_z, 0.0, 0.0, 0.0, 0.0, 1.0, 0);	//카메라 시점

	glRotatef(-230.0, 0, 1, 0);//y축으로 기울임
	/*
	로봇이 피겨동작시 몸이 틀어지는 것을 표현 
	*/
	glRotatef(sin(time)*7,0,0,1); //z축기준으로 7도 까지 각도틀어짐 (sin()함수를 사용하여 주기적인 움직임 설정)
	glRotatef(sin(time)*7,0,1,0); //y축으로 7도 까지 각도틀어짐 (sin()함수를 사용하여 주기적인 움직임 설정)

	//로봇 몸체 각도 조절
	glTranslatef(0.0,0.18,0.0); //y축으로 이동
	glRotatef(80,1,0,0);        //x축 기준으로 회전

	glTranslatef(0.0,0.5,0.0);//최초 위치 

	glPushMatrix( ); // 처음 저장 위치

	DrawBody(0,0,0,0); // 몸통 함수 호출

	glPopMatrix( ); // 처음 저장 좌표로 돌아 간다.
	glPushMatrix( );// 처음 저장 좌표 다시 저장

	Drawneck(); // 목함수 호출

	glPopMatrix( ); // 처음 저장 좌표로 돌아 간다.
	glPushMatrix( );// 처음 저장 좌표 다시 저장

	//머리 위치 설정
	glRotatef(-75,1,0,0); //x축기준으로 회전(머리를 위쪽으로 돌리기)
	glTranslatef(0.0,-0.02,0.0); //y축으로 이동 (머리 시작점)

	DrawHead(); // 머리 호출

	glPopMatrix( ); // 처음 저장 좌표로 돌아 간다.
	glPushMatrix( );// 처음 저장 좌표 다시 저장

	//우측전체팔 위치 설정


	DrawR_Arm((R_Arm_y +30),1,0,0);  //우측팔호출
	DrawR_Hand(-(R_Arm_y  - 15),1,0,0); //우측팔뚝

	glPopMatrix( ); // 처음 저장 좌표로 돌아 간다.
	glPushMatrix( );// 처음 저장 좌표 다시 저장

	//좌측전체팔 위치 설정
	glTranslatef(0.0,-0.16,-0.04);//y축,z축으로 이동(좌측팔 시작점)
	glRotatef(40,0,0,1);     //z축 기준으로 회전

	DrawL_Arm((L_Arm_y +30),1,0,0);   //좌측팔호출	
	DrawL_Hand( -(L_Arm_y  - 15),1,0,0);  //좌측팔뚝

	glPopMatrix( ); // 처음 저장 좌표로 돌아 간다.
	glPushMatrix( );// 처음 저장 좌표 다시 저장

	//좌측전체 다리 위치 설정
	glTranslatef(0.0,-0.45,-0.25);//y축,z축으로 이동(좌측다리 시작점)
	glRotatef(-90,1,0,0);  //x축 기준으로 회전

	DrawL_Legs(-30,1,0,0);  //좌측다리
	DrawL_foot(10,1,0,0);  //좌측종아리	
	DrawSkateShoe();	//스케이트슈즈

	glPopMatrix( ); // 처음 저장 좌표로 돌아 간다.
	glPushMatrix( );// 처음 저장 좌표 다시 저장

	//우측전체 다리 위치 설정
	glTranslatef(0.0,-0.5,-0.5);//y축,z축으로 이동(우측다리 시작점)
	glRotatef(-90,1,0,0); //x축 기준으로 회전

	DrawR_Legs(160,1,0,0);  //우측다리
	DrawR_foot(R_Leg_y,1,0,0);  //우측종아리		
	DrawSkateShoe();	//스케이트슈즈

	glPopMatrix( ); // 처음 저장 좌표로 돌아 간다.

	glutSwapBuffers( );
}
/*
하늘을 나는 모션을 취하는 함수
*/
void Fly()
{
	glLoadIdentity(); //CTM 초기화

	/*
	로봇의 기본적인 관절의 움직임 범위를 제한하는 곳
	*/
	L_Arm_x=(-40)+sin(time2)*60;//왼쪽 어깨의 각도시작은 -40상태에서 sin()함수를 사용하여 주기적인 움직임 설정
	R_Arm_x=-5+sin(time2)*6;	//우측 어깨의 각도시작은 -5상태에서 sin()함수를 사용하여 주기적인 움직임 설정
	R_Arm_y=130+sin(time2)*6;	//우측팔뚝 각도조절(sin()함수사용)
	L_Arm_y=20+sin(time2)*6;	//좌측팔뚝 각도조절(sin()함수사용)

	R_Leg_y=5+abs(sin(time)*20);//우측다리는 5도 각도까지 움직이되 sin()함수를 사용하여 주기적인 움직임 설정
	L_Leg_y=5+abs(sin(time)*10);//좌측종아리는 5도 각도까지 움직이되 sin()함수를 사용하여 주기적인 움직임 설정
	R_Leg_x=90+(sin(time)*10);	//우측다리는 90도 각도까지 움직이되 sin()함수를 사용하여 주기적인 움직임 설정
	L_Leg_x=90+(sin(time)*10);	//좌측다리는 90도 각도까지 움직이되 sin()함수를 사용하여 주기적인 움직임 설정

	////////////////display////////////////

	cyl=gluNewQuadric(); //실린더 객체 생성
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT); //초기화
	glMatrixMode(GL_MODELVIEW); //모드 설정

	DrawGround(); //지면 호출
	glLoadIdentity(); //CTM 초기화
	gluLookAt(eye_x, eye_y, eye_z, 0.0, 0.0, 0.0, 0.0, 1.0, 0);	//카메라 시점

	glRotatef(-230.0, 0, 1, 0);//y축으로 기울임
	/*
	로봇이 피겨동작시 몸이 틀어지는 것을 표현 
	*/
	glRotatef(sin(time)*15,0,0,1); //z축기준으로 15도 까지 각도틀어짐 (sin()함수를 사용하여 주기적인 움직임 설정)
	glRotatef(sin(time)*10,0,1,0); //y축으로 10도 까지 각도틀어짐 (sin()함수를 사용하여 주기적인 움직임 설정)

	//로봇 몸체 각도 조절
	glTranslatef(0.0,0.18,0.0); //y축으로 이동
	glRotatef(80,1,0,0);        //x축 기준으로 회전

	glTranslatef(0.0,0.5,0.0);//최초 위치 

	glPushMatrix( ); // 처음 저장 위치

	DrawBody(0,0,0,0); // 몸통 함수 호출

	glPopMatrix( ); // 처음 저장 좌표로 돌아 간다.
	glPushMatrix( );// 처음 저장 좌표 다시 저장

	Drawneck(); // 목함수 호출

	glPopMatrix( ); // 처음 저장 좌표로 돌아 간다.
	glPushMatrix( );// 처음 저장 좌표 다시 저장

	//머리 위치 설정
	glRotatef(-75,1,0,0); //x축기준으로 회전(머리를 위쪽으로 돌리기)
	glTranslatef(0.0,-0.02,0.0); //y축으로 이동 (머리 시작점)

	DrawHead(); // 머리 호출

	glPopMatrix( ); // 처음 저장 좌표로 돌아 간다.
	glPushMatrix( );// 처음 저장 좌표 다시 저장

	//우측전체팔 위치 설정


	DrawR_Arm((R_Arm_y +30),1,0,0);  //우측팔호출
	DrawR_Hand(R_Arm_x,1,0,0); //우측팔뚝
	glRotatef(30, 0, 0, 1.0);
	DrawFist();	//우측 주먹

	glPushMatrix( );// 처음 저장 좌표 다시 저장

	//좌측전체팔 위치 설정
	glTranslatef(0.0,-0.16,-0.04);//y축,z축으로 이동(좌측팔 시작점)
	glRotatef(20,0,0,1);     //z축 기준으로 회전

	DrawL_Arm((L_Arm_y - 30),1,0,0);   //좌측팔호출	
	DrawL_Hand( -(L_Arm_y  - 15),1,0,0);  //좌측팔뚝
	glRotatef(30, 0, 0, 1.0);
	DrawFist();	//좌측 주먹

	glPushMatrix( );// 처음 저장 좌표 다시 저장

	//좌측전체 다리 위치 설정
	glTranslatef(0.1,-0.45,-0.35);//y축,z축으로 이동(좌측다리 시작점)
	glRotatef(-90,1,0,0);  //x축 기준으로 회전

	DrawL_Legs(L_Leg_x,1,0,0);  //좌측다리
	DrawL_foot(L_Leg_y,1,0,0);  //좌측종아리	
	DrawSkateShoe();	//스케이트슈즈(이게 어울려서 붙임)

	glPopMatrix( ); // 처음 저장 좌표로 돌아 간다.
	glPushMatrix( );// 처음 저장 좌표 다시 저장

	//우측전체 다리 위치 설정
	glTranslatef(-0.1,-0.5,-0.35);//y축,z축으로 이동(우측다리 시작점)
	glRotatef(-90,1,0,0); //x축 기준으로 회전

	DrawR_Legs(R_Leg_x,1,0,0);  //우측다리
	DrawR_foot(R_Leg_y,1,0,0);  //우측종아리	
	DrawSkateShoe();	//스케이트슈즈(이게 어울려서 붙임)

	glPopMatrix( ); // 처음 저장 좌표로 돌아 간다.

	glutSwapBuffers( );
}
/*
로봇이 로켓을 날리는 모습을 표현한 함수
*/
void Rocket(){
	glLoadIdentity(); //CTM 초기화

	/*90도 각도로 팔을 앞으로 내민다*/
	L_Arm_x=-90; 
	R_Arm_x=-90;

	R=2*abs(-sin(time2)*0.2-0.2)+0.2;//우측 로켓 움직임 설정
	//abs절대값으로 로켓이 앞쪽으로 나가게 설정.
	//+0.2를 통해 로켓의 처음위치 조절. 2*를 통해 로켓이 나가는 거리조절.
	//sin()함수를 통해 주기적인 움직임 설정.

	R2=2*abs(sin(time2)*0.2-0.2)+0.2;//좌측 로켓 움직임 설정 
	//abs절대값으로 로켓이 앞쪽으로 나가게 설정.
	//+0.2를 통해 로켓의 처음위치 조절. 2*를 통해 로켓이 나가는 거리조절.
	//sin()함수를 통해 주기적인 움직임 설정.

	R_Leg_y=abs(-sin(time)*30-30);  //우측종아리 각도조절(abs절대값을 줌으로써 종아리가 앞으로 꺾이지 않는 한계점을 설정)
	L_Leg_y=abs(sin(time)*30-30);   //좌측종아리 각도조절(abs절대값을 줌으로써 종아리가 앞으로 꺾이지 않는 한계점을 설정)
	R_Leg_x=sin(time)*60;          //우측다리는 60도 각도까지 움직이되 sin()함수를 사용하여 주기적인 움직임 설정
	L_Leg_x=-R_Leg_x;              //좌측다리는 우측다리반대로 60도 각도까지 움직이되 sin()함수를 사용하여 주기적인 움직임 설정

	////////////////display////////////////

	cyl=gluNewQuadric(); //실린더 객체 생성
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT); //초기화
	glMatrixMode(GL_MODELVIEW); //모드 설정

	DrawGround(); //지면 호출

	glLoadIdentity(); //CTM 초기화
	gluLookAt(eye_x, eye_y, eye_z, 0.0, 0.0, 0.0, 0.0, 1.0, 0);	//카메라 시점

	glPushMatrix();// 처음 저장 좌표 다시 저장
	glRotatef(-230.0, 0, 1, 0);//y축으로 기울임

	/*
	로봇이 로켓을 쏠 때 상,하,좌,우로 몸을 트는 모습을 설정
	*/
	glRotatef(-abs(sin(time)*8),1,0,0); //x축으로 8도 까지 각도틀어짐(abs절대값을 줌으로써 몸체가 뒤로 꺾이지 않는 한계점을 설정)
	glRotatef(sin(time)*7,0,0,1);       //z축으로 7도 까지 각도틀어짐, sin()함수를 사용하여 주기적인 움직임 설정

	/*
	로봇이 로켓을 날리며 몸을 튕기는 모습을 표현
	*/
	float i=0;    
	i=abs(sin(time)*0.08); //i값 설정 
	glTranslatef(0.0,i,0); //변수 i만큼 이동
	glTranslatef(0.0,0.5,0.0);//최초 위치 
	DrawAndroid();		
	glutSwapBuffers();
}

/*
키보드 콜백 설정 함수
w를 누르면 wire 모드로, s를 누르면 solid 모드로, q를 누르면 종료를 표현
*/
void MyKeyboard(unsigned char KeyPressed,int x,int y){
	static int toggle = 0;	//'z'키를 눌렀을 때 일시중지/재개를 구별하는 변수
	static int temp = 0;	//'z'키를 눌러서 일시중지를 시킬 때 그 전의 키값 저장
	switch (KeyPressed){
	case 'w':
		flag=1; 
		break;
	case 'e':
		flag=0;	
		break;
	case 'z':
		if(toggle){
			key=temp;
			toggle=0;}
		else{
			temp=key;
			key=6;
			toggle=1;}
		break;
	case 'q':
		exit(0);
		break;
		/*프로젝션 설정*/
	case '1':
		if(select_mod==PROJECTION){
			system("cls");
			cout<<"glOrtho()의 매개변수를 조정합니다.\nU : left\tI : right\tO : bottom\tP : top\t[ : zNear\t] : zFar\n"<<endl;}
		mode=ORTHO;
		break;
	case '2':
		if(select_mod==PROJECTION){
			system("cls");
			cout<<"glFrustum()의 매개변수를 조정합니다.\nU : left\tI : right\tO : bottom\tP : top\t[ : zNear\t] : zFar\n"<<endl;}
		mode=FRUSTUM;
		break;
	case '3':
		if(select_mod==PROJECTION){
			system("cls");
			cout<<"gluPerspective()의 매개변수를 조정합니다.\nO : fovy\tP : aspect\t[ : zNear\t] : zFar\n"<<endl;}
		mode=PERSPECTIVE;
		break;
		/*로봇 동작 설정*/
	case 'a':
		key=1;
		break;
	case 's':
		key=2;
		break;
	case 'd':
		key=3;
		break;
	case 'f':
		key=4;
		break;
	case 'g':
		key=5;
		break;
		/*MODELVIEW와 PROJECTION 중 매개변수를 조정할 모드 설정*/
	case '9':
		system("cls");
		cout<<"gluLookAt()의 매개변수를 조정합니다.\nJ : eye_x\tK : eye_y\tL : eye_z\n"<<endl;
		select_mod=MODELVIEW;
		break;
	case '0':
		system("cls");
		if(mode==ORTHO)
			cout<<"glOrtho()의 매개변수를 조정합니다.\nU : left\tI : right\tO : bottom\tP : top\t[ : zNear\t] : zFar\n"<<endl;
		else if(mode==FRUSTUM)
			cout<<"glFrustum()의 매개변수를 조정합니다.\nU : left\tI : right\tO : bottom\tP : top\t[ : zNear\t] : zFar\n"<<endl;
		else	//PERSPECTIVE
			cout<<"gluPerspective()의 매개변수를 조정합니다.\nO : fovy\tP : aspect\t[ : zNear\t] : zFar\n"<<endl;
		select_mod=PROJECTION;
		break;
		/*MODELVIEW에서 조정할 매개변수 선택*/
	case 'j':
		cout<<"eye_x 조정"<<endl;
		eye_co=X;
		break;
	case 'k':
		cout<<"eye_y 조정"<<endl;
		eye_co=Y;
		break;
	case 'l':
		cout<<"eye_z 조정"<<endl;
		eye_co=Z;
		break;
		/*PROJECTION에서 조정할 매개변수 선택*/
	case 'u':
		if(mode!=PERSPECTIVE){
			cout<<"left 조정"<<endl;
			proj_co=LEFT;}
		break;
	case 'i':
		if(mode!=PERSPECTIVE){
			cout<<"right 조정"<<endl;
			proj_co=RIGHT;}
		break;
	case 'o':
		if(mode==PERSPECTIVE){
			cout<<"fovy 조정"<<endl;
			proj_co=FOVY;}
		else{//glOrtho, glFrustum
			cout<<"bottom 조정"<<endl;
			proj_co=BOTTOM;}
		break;
	case 'p':
		if(mode==PERSPECTIVE){
			cout<<"aspect 조정"<<endl;
			proj_co=ASPECT;}
		else{//glOrtho, glFrustum
			cout<<"top 조정"<<endl;
			proj_co=TOP;}
		break;
	case '[':
		cout<<"zNear 조정"<<endl;
		proj_co=zNear;
		break;
	case ']':
		cout<<"zFar 조정"<<endl;
		proj_co=zFar;
		break;
		/*매개변수 조정, (+)는 0.1씩 가산, (-)는 0.1씩 감산*/
	case '+': case '-':
		key_operation(KeyPressed);
		break;
		/*메뉴 출력*/
	case 'm':
		select_mod = NONE;	//초기화
		print_menual();
		break;
	}
	redisplay_all();
}
/*
선택된 설정값에 따라서 매개변수를 가산 및 감산한다.
*/
void key_operation(unsigned char key){
	switch(key){
	case '+':
		if(select_mod==MODELVIEW){
			if(eye_co==X){
				if(range_x < 50){
					eye_x += 0.1;
					range_x += 1;}
			}
			else if(eye_co==Y){
				if(range_y < 50){
					eye_y += 0.1;
					range_y += 1;}
			}
			else{	//eye_co==Z
				if(range_z < 50){
					eye_z += 0.1;
					range_z += 1;}
			}
		}
		else{	//PROJECTION
			if(proj_co==LEFT){
				if(range_left < 100){
					pleft += 0.1;
					range_left += 1;}
			}
			else if(proj_co==RIGHT){
				if(range_right < 100){
					pright += 0.1;
					range_right += 1;}
			}
			else if(proj_co==BOTTOM){
				if(range_bottom < 100){
					bottom += 0.1;
					range_bottom += 1;}
			}
			else if(proj_co==TOP){
				if(range_top < 100){
					top += 0.1;
					range_top += 1;}
			}
			else if(proj_co==FOVY){
				if(range_fovy < 1790){
					fovy += 0.1;
					range_fovy += 1;}
			}
			else if(proj_co==ASPECT){
				if(range_aspect < 30){
					aspect += 0.1;
					range_aspect += 1;}
			}
			else if(proj_co==zNear){
				if(mode!=PERSPECTIVE){//glOrtho, glFrustum
					if(range_glNear < 50){
						glNear += 0.1;
						range_glNear += 1;}
				}
				else{//gluPerspective
					if(range_gluNear < 100){
					gluNear += 0.1;
					range_gluNear += 1;}
				}
			}
			else if(proj_co==zFar){
				if(mode!=PERSPECTIVE){//glOrtho, glFrustum
					if(range_glFar < 50){
						glFar += 0.1;
						range_glFar += 1;}
				}
				else{//gluPerspective
					if(range_gluFar < 100){
					gluFar += 0.1;
					range_gluFar += 1;}
				}
			}
		}
		break;
	case '-':
		if(select_mod==MODELVIEW){
			if(eye_co==X){
				if(range_x > -50){
					eye_x -= 0.1;
					range_x -= 1;}
			}
			else if(eye_co==Y){
				if(range_y > -50){
					eye_y -= 0.1;
					range_y -= 1;}
			}
			else{	//eye_co==Z
				if(range_z > -50){
					eye_z -= 0.1;
					range_z -= 1;}
			}
		}
		else{	//PROJECTION
			if(proj_co==LEFT){
				if(range_left > -100){
					pleft -= 0.1;
					range_left -= 1;}
			}
			else if(proj_co==RIGHT){
				if(range_right > -100){
					pright -= 0.1;
					range_right -= 1;}
			}
			else if(proj_co==BOTTOM){
				if(range_bottom > -100){
					bottom -= 0.1;
					range_bottom -= 1;}
			}
			else if(proj_co==TOP){
				if(range_top > -100){
					top -= 0.1;
					range_top -= 1;}
			}
			else if(proj_co==FOVY){
				if(range_fovy > -1790){
					fovy -= 0.1;
					range_fovy -= 1;}
			}
			else if(proj_co==ASPECT){
				if(range_aspect > -30){
					aspect -= 0.1;
					range_aspect -= 1;}
			}
			else if(proj_co==zNear){
				if(mode!=PERSPECTIVE){//glOrtho, glFrustum
					if(range_glNear > -50){
						glNear -= 0.1;
						range_glNear -= 1;}
				}
				else{//gluPerspective
					if(range_gluNear > -100){
					gluNear -= 0.1;
					range_gluNear -= 1;}
				}
			}
			else if(proj_co==zFar){
				if(mode!=PERSPECTIVE){//glOrtho, glFrustum
					if(range_glFar > -50){
						glFar -= 0.1;
						range_glFar -= 1;}
				}
				else{//gluPerspective
					if(range_gluFar > -100){
					gluFar -= 0.1;
					range_gluFar -= 1;}
				}
			}
		}
		break;
	}
}
/*
타이머 함수
각 장면마다 상태 표현을 다르게 두기 위해 대양한 시간 변수들이 쓰였다.
*/
void MyTimer(int Value){
	time=time+0.1;//Run()에 쓰인 타이머 변수
	time2=time2+0.5;//Jap()에 쓰인 타이머 변수
	time3=time3+0.01;//Rocket()에 쓰인 타이머 변수
	time4=time4+1.0;//Show()와 Fly()에 쓰인 타이머 변수
	glutPostRedisplay();
	glutTimerFunc(40,MyTimer,1);
}
/*
마우스 오른쪽 클릭을 해서 메뉴를 선택하면 실행되는 함수.
기본적으로 장면을 전환할땐 마우스 콜백을 이용하여 변환하도록 설정하였다.
*/
void MyMainMenu(int entryID){ //메뉴
	if(entryID != 7)
		key=entryID;
	else{	//7번 즉, 메뉴 보기를 클릭하였을 때
		select_mod = NONE;	//초기화
		print_menual();
	}
}
/*
시점변환 Reshape
*/
void screen_reshape(int width, int height)
{
	WinWidth = width; WinHeight = height;
	glViewport(0, 0, width, height);

	if(select_mod == MODELVIEW){
		printf("eye_x : %.1lf eye_y : %.1lf eye_z : %.1lf \n\n", eye_x, eye_y, eye_z);}
	else if(select_mod == PROJECTION){
		if(mode==ORTHO || mode==FRUSTUM)
			printf("left : %.1lf right : %.1lf bottom : %.1lf top : %.1lf zNear : %.1lf zFar : %.1lf\n\n", 
			pleft, pright, bottom, top, glNear, glFar);
		else	//PERSPECTIVE
			printf("fovy : %.1lf aspect : %.1lf zNear : %.1lf zFar : %.1lf\n\n", 
			fovy, aspect, gluNear, gluFar);
	}
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	/*
	각 모드에 맞추어 PROJECTION 한다.
	*/
	switch(mode){
	case ORTHO:
		glOrtho(pleft, pright, bottom, top, glNear, glFar);
		break;
	case FRUSTUM:
		glFrustum(pleft, pright, bottom, top, glNear, glFar);
		break;
	case PERSPECTIVE:
		gluPerspective(fovy, aspect, gluNear, gluFar);
		break;
	}
	glMatrixMode(GL_MODELVIEW);	
	glLoadIdentity();
}
/*
screen reshape 동작을 
화면에 바로 출력함
*/
void redisplay_all(void)
{
	screen_reshape(WinWidth, WinHeight);
	glutPostRedisplay();
}
/*
서브메뉴 선언
*/
void menu_proj(int none){}	//중간단계의 메뉴이므로 내용은 없다.
void menu_mod(int none){}	//중간단계의 메뉴이므로 내용은 없다.
void menu_select_proj(int entryID){//시점변환을 위한 서브메뉴
	switch(entryID){
	case 1:
		mode = ORTHO;
		break;
	case 2:
		mode = FRUSTUM;
		break;
	case 3:
		mode = PERSPECTIVE;
		break;
	}
	redisplay_all();
}
void menu_coor(int entryID){//gluLookAt()의 변경할 매개변수 선택
	system("cls");
	cout<<"gluLookAt()의 매개변수를 조정합니다.\nJ : eye_x\tK : eye_y\tL : eye_z\n"<<endl;
	select_mod=MODELVIEW;
	switch(entryID){
	case 1:
		eye_co=X;
		break;
	case 2:
		eye_co=Y;
		break;
	case 3:
		eye_co=Z;
		break;
	}
	redisplay_all();
}
void menu_glPM(int entryID){//glOrtho() 혹은 glFrustum()의 변경할 매개변수 선택
	system("cls");	//cmd 화면 초기화
	cout<<"glOrtho() 혹은 glFrustum()의 매개변수를 조정합니다.\nU : left\tI : right\tO : bottom\tP : top\t[ : zNear\t] : zFar\n"<<endl;
	select_mod=PROJECTION;
	if(mode==PERSPECTIVE)	
		mode=ORTHO;	//이 함수의 취지에 맞게 설정
	switch(entryID){
	case 1:
		proj_co=LEFT;
		break;
	case 2:
		proj_co=RIGHT;
		break;
	case 3:
		proj_co=BOTTOM;
		break;
	case 4:
		proj_co=TOP;
		break;
	case 5:
		proj_co=zNear;
		break;
	case 6:
		proj_co=zFar;
		break;
	}
	redisplay_all();
}
void menu_gluPM(int entryID){//gluPerspective()의 변경할 매개변수 선택
	system("cls");	//cmd 화면 초기화
	cout<<"gluPerspective()의 매개변수를 조정합니다.\nO : fovy\tP : aspect\t[ : zNear\t] : zFar\n"<<endl;
	select_mod=PROJECTION;
	mode=PERSPECTIVE;	//이 함수의 취지에 맞게 설정
	switch(entryID){
	case 1:
		proj_co=FOVY;
		break;
	case 2:
		proj_co=ASPECT;
		break;
	case 3:
		proj_co=zNear;
		break;
	case 4:
		proj_co=zFar;
		break;
	}
	redisplay_all();
}
/*
각 key 값의 뜻은 각 장면을 뜻한다.
*/
void MyDisplay(){
	if(key==RUN){//로봇이 달릴때		
		Run();
		glPopMatrix( );
	}
	else if(key==JAP){//로봇이 잽을 날릴 때
		Jap();
		glPopMatrix( );		
	}
	else if(key==ROCKET){//로봇이 로켓을 발사할때
		Rocket();
		glPopMatrix( );
	}
	else if(key==SHOW){//로봇이 피겨스케이팅 모션을 취할 때
		Show();
		glPopMatrix( );
	}
	else if(key==FLY){//로봇이 비행 모션을 취할 때
		Fly();
		glPopMatrix();
	}
	else if(key==PAUSE){//중지가 선택됐을 때
		sndPlaySound(NULL,SND_ASYNC);
	}
}

int main(int argc, char** argv){
	glutInit(&argc, argv);//초기화
	glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE | GLUT_DEPTH);//디스플레이 모드 설정
	glutInitWindowSize(800,800);//윈도우 크기 설정
	glutInitWindowPosition(0,0);//윈도우 초기 위치 설정
	glutCreateWindow("2010136019_김동현_Assignment07");//윈도우 생성
	glInit(); //조명 초기화
	print_menual();	//메뉴얼 출력

	GLint MySubMenuID1 = glutCreateMenu(menu_select_proj);
	glutAddMenuEntry("ORTHO", 1);
	glutAddMenuEntry("FRUSTUM", 2);
	glutAddMenuEntry("PERSPECTIVE", 3);
	
	GLint MySubMenuID3 = glutCreateMenu(menu_coor);
	glutAddMenuEntry("x", 1);
	glutAddMenuEntry("y", 2);
	glutAddMenuEntry("z", 3);

	GLint MySubMenuID5 = glutCreateMenu(menu_glPM);	//PM: ParaMeter
	glutAddMenuEntry("left", 1);
	glutAddMenuEntry("right", 2);
	glutAddMenuEntry("bottom", 3);
	glutAddMenuEntry("top", 4);
	glutAddMenuEntry("zNear", 5);
	glutAddMenuEntry("zFar", 6);

	GLint MySubMenuID6 = glutCreateMenu(menu_gluPM);
	glutAddMenuEntry("fovy", 1);
	glutAddMenuEntry("aspect", 2);
	glutAddMenuEntry("zNear", 3);
	glutAddMenuEntry("zFar", 4);

	GLint MySubMenuID4 = glutCreateMenu(menu_proj);
	glutAddSubMenu("glOrtho", MySubMenuID5);
	glutAddSubMenu("glFrustum", MySubMenuID5);
	glutAddSubMenu("gluPerspective", MySubMenuID6);

	GLint MySubMenuID2 = glutCreateMenu(menu_mod);
	glutAddSubMenu("MODELVIEW", MySubMenuID3);
	glutAddSubMenu("PROJECTION", MySubMenuID4);

	GLint MyMainMenuID = glutCreateMenu(MyMainMenu);//메뉴추가
	glutAddSubMenu("select Projection", MySubMenuID1);//시점변환 메뉴 추가
	glutAddMenuEntry("RUN",1);//로봇이 달리는 모션 메뉴 추가
	glutAddMenuEntry("JAP",2);//로봇이 잽날리는 모션 메뉴 추가
	glutAddMenuEntry("ROCKET",3);//로봇이 로켓펀치를 날리는 모션 메뉴 추가
	glutAddMenuEntry("SHOW",4);//로봇이 피겨스케이팅하는 모션 메뉴 추가
	glutAddMenuEntry("FLY",5);//로봇이 하늘을 나는 모션 메뉴 추가
	glutAddMenuEntry("중지/재개",6);//로봇이 하는 일을 멈추게 하거나 재개하는 메뉴 추가
	glutAddSubMenu("매개변수 변환", MySubMenuID2);
	glutAddMenuEntry("메뉴 보기", 7);//cmd창을 초기화하고 메뉴를 출력합니다.
	glutAttachMenu(GLUT_RIGHT_BUTTON);//오른쪽마우스클릭시메뉴팝업

	glutKeyboardFunc(MyKeyboard); //키보드 콜백
	glutTimerFunc(40,MyTimer,1); //타이머 함수
	glutReshapeFunc(screen_reshape); //Reshape
	glutDisplayFunc(MyDisplay); //디스플레이

	glutMainLoop( );
	return 0;
}