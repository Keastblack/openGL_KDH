/*******************************
2010136019 �赿�� Assignment07
*******************************/
/*cmdâ�� �������̽��� �����Ͽ����ϴ�.*/
/*
'q' : ���α׷� ����
'w' : Wire �̹��� ��ȯ
'e' : Solid �̹��� ��ȯ
'1' : ORTHO ���� ���
'2' : FRUSTUM ���� ���
'3' : PERSPECTIVE ���� ���
'a' : RUN ���
's' : JAP ���
'd' : ROCKET ���
'f' : SHOW ���
'z' : �ִϸ��̼� �Ͻ�����/�簳
'9' : ī�޶� ���� �Ű����� ����
'0' : PROJECTION �Ű����� ����
'M' : �޴� �ٽú���
'J', 'K', 'L' : ī�޶� �������� ������ �Ű����� ����
'U', 'I', 'O', 'P', '[', ']' : PROJECTION���� ������ �Ű����� ����
'+' : �ش� �Ű����� 0.1�� ����
'-' : �ش� �Ű����� 0.1�� ����
*/

/*
�ʼ����� ��� ���ϵ�
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

static double time=0;  // Run_time ����
static double time2=0; // Jap_time ����
static double time3=0; // Rocket_time ����
static double time4=0; // ground_time ����
static double time6=0; // exit_time ����

GLfloat R_Arm_x=0; // ������ ���
GLfloat R_Arm_y=0; // ������ ��
GLfloat L_Arm_x=0; // ���� ���
GLfloat L_Arm_y=0; // ���� ��
GLfloat R_Leg_x=0; // ���� �����
GLfloat R_Leg_y=0; // ���� ���Ƹ�
GLfloat L_Leg_x=0; // �� �����
GLfloat L_Leg_y=0; // �� ���Ƹ�
GLfloat R=0; // ���� ���� ��ġ ������ ����
GLfloat R2=0;// ������ ���� ��ġ ������ ����

GLUquadricObj* cyl;// �Ǹ��� ��ü ����

int a=0; // x�� ����(���� 1�� �� x���� �������� ȸ��)
int b=0; // y�� ����(���� 1�� �� y���� �������� ȸ��)
int c=0; // z�� ����(���� 1�� �� z���� �������� ȸ��)

static int flag=0; // wireframe ���� solid rendering ��� ���� ����
static int key=1;  // ���� ���� ���� ���� ����

GLint WinWidth = 0, WinHeight = 0;	// reshape�� ���� ȭ���� ��Ⱦũ�� ��������
/*������ PROJECTION �� MODELVIEW�� ���� �Ű��������̴�.*/
GLdouble pleft=-1.0, pright=1.0, bottom=-1.0, top=1.0, glNear=-1.0, glFar=1.0;	//glOrtho�� glFrustum�� ���� �Ű�����
GLdouble fovy=100.0, aspect = 1.0, gluNear=-1.0, gluFar=1.0;	//gluPerspective�� �Ű�����
GLdouble eye_x=0.0, eye_y=0.0, eye_z=1.0;	//gluLookAt�� �Ű�����
/*GLdouble������ �Ű����� ���� ������ �����ϸ� �ڷ����� Ư���� ��Ȯ�� ������ �ϱ�
�����Ƿ� int������ ��Ȯ�� �����Ѵ�.*/
int range_left=0, range_right=0, range_bottom=0, range_top=0, range_fovy=0, range_aspect=0; 
int range_glNear=0, range_glFar=0, range_gluNear=0, range_gluFar=0; 
int range_x=0, range_y=0, range_z=10;	
enum{
	NONE, PROJECTION, MODELVIEW
}select_mod = NONE;	//������ ����� �Ű����� ����, �ʱⰪ�� NONE(���� �����̹Ƿ�)
enum{
	ORTHO, FRUSTUM, PERSPECTIVE
}mode = ORTHO;	//���� ��� ����, �ʱⰪ�� ORTHO
enum{
	LEFT, RIGHT, BOTTOM, TOP, FOVY, ASPECT, zNear, zFar
}proj_co = LEFT;	//�������� ��忡�� ������ �Ű����� ����
enum{
	X, Y, Z
}eye_co = X;	//gluLookAt()���� eye_x, eye_y, eye_z �� ������ �Ű����� ����

#define RUN		1					
#define JAP		2							
#define ROCKET	3	
#define SHOW	4
#define FLY		5
#define PAUSE	6	

void redisplay_all();
void key_operation(unsigned char);
/*
�޴��� ��� �Լ�
*/
void print_menual(void){
	system("cls");	//cmd ȭ�� �ʱ�ȭ
	cout << "--- Control Key ---\nRun : A\nJap : S\nRocket Punch : D\nSkating Motion : F\nFlying Motion : G\n\nORTHO VIEW : 1\n";
	cout << "FRUSTUM VIEW : 2\nPERSPECTIVE VIEW : 3\n\nWireframe : W\nSolidframe : E\n\nPAUSE/RESUME: Z\nQUIT : Q\n\n";
	cout << "ī�޶� ���� �Ű����� ���� : 9\nPROJECTION �Ű����� ���� : 0\n\n�޴� �ٽú��� : M\n" << endl;
	cout<<"�Ű������� ������ ��带 �����ϼ���.[9/0]";
}
void glInit(void){ // ���� �ʱ�ȭ �Լ� 
	glEnable(GL_DEPTH_TEST); // ���� �׽��� ���
	glEnable(GL_NORMALIZE);  // ����ȭ
	glEnable(GL_SMOOTH);     // �� ������ ������ �ε巴�� �����Ͽ� ĥ��������  �ϹǷ�, �������� ����� ������ ȥ�յǾ� ����ȴ�. 
	glEnable(GL_LIGHTING);   // ���� ����. ���� �޴� ������ ���� �κ� ������ ��ȭ

	/* ���� �޴� ������ ���� �κ� ������ ��ȭ�� ���� ���� */
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

	glClearColor(1.0,1.0,1.0,1.0); // ��� ������
}

void Draw_Color(int i){ // ������ �׸��� �Լ�
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

void Change_Wire_Or_Solid(int i){          // �ָ���, ���̾� ����
	if(flag==1){
		gluQuadricDrawStyle(cyl,GLU_LINE); // ��Ÿ���� ���̾�� ����
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);	// glut�� �׸� �������� ����
	}
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);	//��Ÿ���� �ָ���� ����
}
/*
�κ� ������ �׸��� �Լ�
*/
void DrawL_Arm(int x,int a,int b,int c){
	glPushMatrix();
	Draw_Color(key);       // �� ����
	cyl=gluNewQuadric();   // �Ǹ��� ��ü ����
	glRotatef(x,a, b, c);  // �Ķ���� ����ŭ ȸ��
	glRotatef(90.0,1.0, 0.0, 0.0); // �Ǹ����� x�� �������� 90�� ȸ��(�Ǹ��� ���� �� ���� ������ ����)
	glTranslatef(0.25,0.0,0.0);    // ���� ��� ���������� �̵�
	glRotatef(15.0,0.0, 1.0, 0.0); // ���� ����� y���� �������� 15�� ȸ��
	Change_Wire_Or_Solid(flag);
	gluCylinder(cyl,0.05,0.05,0.2,50,1); // �Ǹ����� �׸�
}

/*
�κ� ���� ���� �׸��� �Լ�
*/
void DrawL_Hand(int y,int a,int b, int c){
	glPushMatrix();
	Draw_Color(key);       // �� ����
	cyl=gluNewQuadric();   // �Ǹ��� ��ü ����
	glTranslatef(0.0,0.0,0.22); // �޼� ������
	glRotatef(y,a,b, c);        // �Ķ���� ����ŭ ȸ��
	Change_Wire_Or_Solid(flag);
	if(key == SHOW) gluCylinder(cyl,0.05,0.02,0.2,15,1); // �򶧱� ��� �Ǹ����� �׸�
	else gluCylinder(cyl,0.05,0.05,0.2,15,1);           // �Ǹ����� �׸�
	//glPopMatrix();
}
/*
�κ� ���� �ָ�
*/
void DrawL_HandRocket(){
	glPushMatrix();
	glColor3f(0, 0.5, 1.0);
	glTranslatef(0,0,R); // ���� �ָ� ��ġ�� �Ķ���͸�ŭ �̵�
	Change_Wire_Or_Solid(flag);
	glutSolidCone(0.05, 0.5, 20, 20);
	glPopMatrix();
}
/*
�κ� �������� �׸��� �Լ�
*/
void DrawR_Arm(int x, GLfloat a, GLfloat b,GLfloat c){
	glPushMatrix();
	Draw_Color(key);     // �� ����
	cyl=gluNewQuadric(); // �Ǹ��� ��ü ����
	glRotatef(x,a,b,c);  // �Ķ���� ����ŭ ȸ��
	glRotatef(90.0,1.0, 0.0, 0.0); // �Ǹ����� x���� �������� 90�� ȸ��(�Ǹ��� ���� �� ���� ������ ����)
	glTranslatef(-0.25,0.0,0.0); //���� ��� ���������� �̵�
	glRotatef(-15.0,0.0, 1.0, 0.0); //���� ��� -15�� y����� ȸ��
	Change_Wire_Or_Solid(flag);
	gluCylinder(cyl,0.05,0.05,0.2,50,1);// �Ǹ��� �׸�

}
/*
�κ� ������ ���� �׸��� �Լ�
*/
void DrawR_Hand(int y,int a,int b, int c){
	glPushMatrix();
	Draw_Color(key); // �� ����
	cyl=gluNewQuadric();//�Ǹ��� ��ü ����
	glTranslatef(0.0,0.0,0.22);//������ �� ������
	glRotatef(y, a , b, c);// �Ķ���Ͱ� ��ŭ ȸ��
	Change_Wire_Or_Solid(flag);
	if(key == SHOW) gluCylinder(cyl,0.05,0.02,0.2,50,1);// �򶧱� ��� �Ǹ��� �׸�
	else gluCylinder(cyl,0.05,0.05,0.2,50,1);// �Ǹ��� �׸�
}
/*
�κ� �ָ��� �׸��� �Լ�
*/
void DrawFist(){
	glTranslatef(0, 0, 0.25);
	glColor3f(0.6, 0.8, 0.2);
	glutSolidCube(0.1);
	Draw_Color(key);
	glPopMatrix();
}
/*
�κ� �۷��긦 �׸��� �Լ�
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
�κ� ������ ����Ʈ �Լ�
*/
void DrawR_HandRocket(){
	glPushMatrix();
	glColor3f(0, 0.5, 1.0);
	glTranslatef(0,0,R2);//�����ָ� ��ġ �Ķ���͸�ŭ �̵�
	Change_Wire_Or_Solid(flag);
	glutSolidCone(0.05, 0.5, 20, 20);
	glPopMatrix();
}
/*
�κ� ��ü�� �׸��� �Լ�
*/
void DrawBody(int x,int a,int b,int c){
	glPushMatrix();
	Draw_Color(key);
	cyl=gluNewQuadric();//�Ǹ��� ��ü ����
	glRotatef(90.0, 1.0, 0.0, 0.0);//�Ǹ��� 90�� x����� ȸ�� (�Ǹ��� ������ ���� ������ ���Ͽ�)
	glRotatef(x, a , b, c);//�Ķ���Ͱ� ��ŭ ȸ��
	Change_Wire_Or_Solid(flag);
	gluCylinder(cyl,0.2,0.2,0.45,50,1);// �Ǹ��� �׸�
	glPopMatrix();
}
/*
�κ� ���� ������� �׸��� �Լ�
*/
void DrawL_Legs(int x, int a,int b,int c){
	glPushMatrix();
	Draw_Color(key);
	cyl=gluNewQuadric();//�Ǹ��� ��ü ����
	glRotatef(90.0, 1.0, 0.0, 0.0);//�Ǹ��� 90�� x����� ȸ�� (�Ǹ��� ������ ���� ������ ���Ͽ�)
	glTranslatef(0.1,0.0,0.42);//���� ����� ������
	glRotatef(x,a, b, c);//�Ķ���͸�ŭ ȸ��
	Change_Wire_Or_Solid(flag);
	gluCylinder(cyl,0.05,0.05,0.15,50,1);// �Ǹ��� �׸�

}
/*
�κ� ���� ���Ƹ��� �׸��� �Լ�
*/
void DrawL_foot(int y,int a,int b,int c){
	glPushMatrix();
	Draw_Color(key);
	cyl=gluNewQuadric();//�Ǹ��� ��ü ����
	glTranslatef(0.0,0.0,0.18);//���� ���Ƹ� ������
	glRotatef(y,a, b, c);//�Ķ���͸�ŭ ȸ��
	Change_Wire_Or_Solid(flag);
	if(key == SHOW) gluCylinder(cyl,0.05,0.03,0.2,50,1);// �򶧱� ��� �Ǹ��� �׸�
	else gluCylinder(cyl,0.05,0.05,0.2,50,1);// �Ǹ��� �׸�
}
/*
�κ� �Ź��� �׸��� �Լ�
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
�κ� ������Ʈ��� �׸��� �Լ�
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
�κ� ������ ������� �׸��� �Լ�
*/
void DrawR_Legs(int x,int a,int b,int c){
	glPushMatrix();
	Draw_Color(key);
	cyl=gluNewQuadric();//�Ǹ��� ��ü ����
	glRotatef(90.0, 1.0, 0.0, 0.0);//�Ǹ��� 90�� x����� ȸ�� (�Ǹ��� ������ ���� ������ ���Ͽ�)
	glTranslatef(-0.1,0.0,0.42);//������ ����� ������
	glRotatef(x,a,b,c);//�Ķ���͸�ŭ ȸ��
	Change_Wire_Or_Solid(flag);
	gluCylinder(cyl,0.05,0.05,0.15,50,1);// �Ǹ��� �׸�
}
/*
�κ� ������ ���Ƹ��� �׸��� �Լ�
*/
void DrawR_foot(int y,int a,int b,int c){
	glPushMatrix();
	Draw_Color(key);
	cyl=gluNewQuadric();//�Ǹ��� ��ü ����

	glTranslatef(0.0,0.0,0.18);//������ ���Ƹ� ������
	glRotatef(y,a, b, c);//�Ķ���͸�ŭ ȸ��

	Change_Wire_Or_Solid(flag);
	if(key == SHOW) gluCylinder(cyl,0.05,0.03,0.2,15,1);// �򶧱� ��� �Ǹ��� �׸�
	else gluCylinder(cyl,0.05,0.05,0.2,15,1);// �Ǹ��� �׸�
}
/*
�κ� ���� �׸��� �Լ�
*/
void Drawneck(){
	glPushMatrix();
	glColor3f(1.0, 0.0,0.3);//�� ����
	cyl=gluNewQuadric();//�Ǹ��� ��ü ����
	glRotatef(90.0, 1.0, 0.0, 0.0);//�Ǹ��� 90�� x����� ȸ�� (�Ǹ��� ������ ���� ������ ���Ͽ�)
	glTranslatef(0.0,0.0,-0.045); // �� ������
	Change_Wire_Or_Solid(flag);
	gluCylinder(cyl,0.2,0.2,0.025,100,1);// �Ǹ��� �׸�
	glPopMatrix();
}
/*
���� �׸��� �Լ�
���� Ŀ�ٶ� ���� �����Ͽ���.
���� �ð��� ���� ȸ���Ͽ��� ��ġ �κ��� 
������ �پ�� ������ �鵵�� �����Ͽ���.
*/
void DrawGround(){
	Draw_Color(flag);//���� ����
	glTranslatef(0.0,-2.73,0.0);//�� ������
	glRotatef(45.0,0,1,0);//45����ŭ y�� �������� ȸ��
	glRotatef(0.0+time4-15,0.0, 0.0, 1);// z�� �������� time4��ŭ ���� ȸ�� 
	Change_Wire_Or_Solid(flag);
	gluSphere(cyl, 2.5, 30, 90);	
}
/*
�κ� �Ӹ��� �׸��� �Լ�
�׸� �Ӹ��� �Ķ��� ����� ���.
*/
void DrawHead(){
	glTranslatef(0.0,0.02,0.0);//�Ӹ� ������
	glPushMatrix();// ó�� ���� ��ǥ �ٽ� ����

	glTranslatef(0, 0.18, 0);
	Draw_Color(key);//���� ����
	glutSolidCube(0.3);//�Ӹ��׸���

	/*�Ķ��� ���*/
	glBegin(GL_POLYGON);
	glColor3f(0, 0, 1);
	glVertex3f(-0.1, 0.1, 0.16);
	glVertex3f(0.1, 0.1, 0.16);
	glVertex3f(0.1, 0.01, 0.16);
	glVertex3f(-0.1, 0.01, 0.16);
	glEnd();		

	glPopMatrix();// ó�� ���� ��ǥ �ٽ� ����
}
/*
�ȵ���̵� ��ü ����� �׸��� �Լ�
*/
void DrawAndroid(){
	DrawBody(0,0,0,0); // ���� �Լ� ȣ��
	Drawneck(); // ���Լ� ȣ��
	DrawHead(); // �Ӹ� ȣ��
	DrawR_Arm(R_Arm_x,1,0,0);//�����ȶ�
	if(key==ROCKET) DrawR_HandRocket(); // ���ϸ���϶� ��������ȣ��
	else{
		DrawR_Hand(R_Arm_y, 1, 0, 0);  //������ȣ��
		if(key==JAP)
			DrawGlove();
		else 
			DrawFist();	//�����ָ�
	}
	DrawL_Arm(L_Arm_x,1,0,0);   //�����ȶ�
	if(key==ROCKET) DrawL_HandRocket(); // ���ϸ���϶� ��������ȣ��
	else{
		DrawL_Hand(L_Arm_y,1,0,0);//������ȣ��
		if(key==JAP)
			DrawGlove();
		else
			DrawFist();	//�����ָ�
	}
	DrawL_Legs(L_Leg_x,1,0,0);  //�����ٸ�
	DrawL_foot(L_Leg_y,1,0,0);  //������
	DrawShoe();	//�����Ź�
	DrawR_Legs(R_Leg_x,1,0,0);  //�����ٸ�
	DrawR_foot(R_Leg_y,1,0,0);  //������	
	DrawShoe();	//�����Ź�
}
/*
�κ��� �޸��� ���� ǥ���� �Լ�.
�κ��� ������ �����̴� ���� ǥ���߰�,
�κ��� �޸��鼭 ��,��,��,��� �����̴� ����� ǥ���ߴ�.
*/
void Run(){
	glLoadIdentity();//CTM �ʱ�ȭ
	/*
	�⺻���� �κ��� �������� ���ӵ��� sin() �Լ��� ���� ǥ���Ͽ���
	���� ������ ���ѹ����� �����Ͽ� abs() �Լ��� ���� ������ �������� �����Ͽ���.
	*/
	L_Arm_x=sin(time)*80;           //������ 80�� �������� �����̵� sin()�Լ��� ����Ͽ� �ֱ����� ������ ����
	R_Arm_x=-L_Arm_x;               //�������� ���ȹݴ�� 80���������� �����̵� sin()�Լ��� ����Ͽ� �ֱ����� ������ ����.
	R_Arm_y=-abs(sin(time)*60+50);  //�����ȶ� ��������(abs���밪�� �����ν� �ȶ��� �ڷβ����� �ʴ� �Ѱ����� ����) 
	L_Arm_y=-abs(-sin(time)*60+50); //�����ȶ� ��������(abs���밪�� �����ν� �ȶ��� �ڷβ����� �ʴ� �Ѱ����� ����) 

	R_Leg_y=abs(-sin(time)*30-30);  //�������Ƹ� ��������(abs���밪�� �����ν� ���Ƹ��� ������ ������ �ʴ� �Ѱ����� ����)
	L_Leg_y=abs(sin(time)*30-30);   //�������Ƹ� ��������(abs���밪�� �����ν� ���Ƹ��� ������ ������ �ʴ� �Ѱ����� ����)
	R_Leg_x=sin(time)*60;          //�����ٸ��� 60�� �������� �����̵� sin()�Լ��� ����Ͽ� �ֱ����� ������ ����
	L_Leg_x=-R_Leg_x;              //�����ٸ��� �����ٸ��ݴ�� 60�� �������� �����̵� sin()�Լ��� ����Ͽ� �ֱ����� ������ ����

	////////////////display////////////////

	cyl=gluNewQuadric(); //�Ǹ��� ��ü ����
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT); //�ʱ�ȭ
	glMatrixMode(GL_MODELVIEW); //��� ����

	DrawGround();    //���� ȣ��
	glLoadIdentity();//CTM �ʱ�ȭ
	gluLookAt(eye_x, eye_y, eye_z, 0.0, 0.0, 0.0, 0.0, 1.0, 0);	//ī�޶� ����

	/*
	�κ��� �޸��鼭 ��,�ڷ� �ڶװŸ��� ���� Ʋ������ ���� ǥ�� 
	*/
	glRotatef(-230.0, 0, 1, 0);//y������ ����� 
	glRotatef(abs(sin(time)*16),1,0,0);//x������ 16�� ���� ����Ʋ����(abs���밪�� �����ν� ��ü�� �ڷ� ������ �ʴ� �Ѱ����� ����)
	glRotatef(sin(time)*16,0,1,0); //y������ 16�� ���� ����Ʋ����, sin()�Լ��� ����Ͽ� �ֱ����� ������ ����

	/*
	�κ��� �޸��鼭 ��,�Ϸ� �����̴� ���� ǥ��
	*/
	float i=0;
	i=abs(sin(time)*0.08); //i������ ����
	glPushMatrix();// ó�� ���� ��ǥ �ٽ� ����
	glTranslatef(0.0,i,0); //���� i��ŭ �κ��� ��ü�� Y��������� ������.

	glTranslatef(0.0,0.5,0.0);//���� �κ��� ��ġ 
	DrawAndroid();
	glutSwapBuffers();
}
/*
�κ��� ���� ������ ���� ǥ���� �Լ�
*/
void Jap(){
	glLoadIdentity();//CTM �ʱ�ȭ
	/*
	�κ��� �⺻���� ������ ������ ������ �����ϴ� ��
	*/
	L_Arm_x=(-40)+sin(time2)*60;//���� ����� ���������� -40���¿��� sin()�Լ��� ����Ͽ� �ֱ����� ������ ����
	R_Arm_x=(-80)-L_Arm_x;      //���� ����� ���������� -80���¿��� ���ʾ�� �������� �ݴ�� ����
	R_Arm_y=-abs(cos(time2)*80);  //�����ȶ� ��������(���� ���� ������ǥ�������Ͽ� ����� sin()�Լ��� �ݴ��� cos()�Լ� ���)
	L_Arm_y=-abs(-cos(time2)*80); //�����ȶ� ��������(���� ���� ������ǥ�������Ͽ� ����� sin()�Լ��� �ݴ��� cos()�Լ� ���)

	R_Leg_y=abs(-sin(time)*30-20);  //�������Ƹ� ��������(abs���밪�� �����ν� ���Ƹ��� ������ ������ �ʴ� �Ѱ����� ����)
	L_Leg_y=abs(sin(time)*30-20);   //�������Ƹ� ��������(abs���밪�� �����ν� ���Ƹ��� ������ ������ �ʴ� �Ѱ����� ����)
	R_Leg_x=sin(time)*30;          //�����ٸ��� 30�� �������� �����̵� sin()�Լ��� ����Ͽ� �ֱ����� ������ ����
	L_Leg_x=-R_Leg_x;              //�����ٸ��� �����ٸ��ݴ�� 30�� �������� �����̵� sin()�Լ��� ����Ͽ� �ֱ����� ������ ����

	////////////////display////////////////

	cyl=gluNewQuadric(); //�Ǹ��� ��ü ����
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT); //�ʱ�ȭ
	glMatrixMode(GL_MODELVIEW); //��� ����

	DrawGround(); //���� ȣ��
	glLoadIdentity(); //CTM �ʱ�ȭ
	gluLookAt(eye_x, eye_y, eye_z, 0.0, 0.0, 0.0, 0.0, 1.0, 0);	//ī�޶� ����

	glRotatef(-230.0, 0, 1, 0);//y������ ����� 

	glRotatef(sin(time)*10,0,0,1);//�κ��� ��,�� �ݵ� ǥ��(�¿�� 10�� ��ŭ �ֱ����� ������ ����)

	/*
	�κ��� ���� �����鼭 ��,�Ϸ� �����̴� ���� ǥ��
	*/
	float j=0;    
	j=abs(sin(time2)*0.085);// j�� ����
	glPushMatrix();// ó�� ���� ��ǥ �ٽ� ����
	glTranslatef(0.0,j,0); //���� j��ŭ �κ��� ��ü�� Y��������� ������.
	glTranslatef(0.0,0.5,0.0);//���� ��ġ 
	DrawAndroid();	
	glutSwapBuffers();
}
/*
�ǰܽ������� ������ ���ϴ� �Լ�
*/
void Show()
{
	glLoadIdentity(); //CTM �ʱ�ȭ

	/*
	�κ��� �⺻���� ������ ������ ������ �����ϴ� ��
	*/
	L_Arm_x=(-40)+sin(time2)*60;//���� ����� ���������� -40���¿��� sin()�Լ��� ����Ͽ� �ֱ����� ������ ����
	R_Arm_x=(-80)-L_Arm_x;       //���� ����� ���������� -80���¿��� ���ʾ�� �������� �ݴ�� ����
	R_Arm_y=-abs(cos(time2)*10);  //�����ȶ� ��������(���� ���� ������ǥ�������Ͽ� ����� sin()�Լ��� �ݴ��� cos()�Լ� ���)
	L_Arm_y=-abs(-cos(time2)*10); //�����ȶ� ��������(���� ���� ������ǥ�������Ͽ� ����� sin()�Լ��� �ݴ��� cos()�Լ� ���)

	R_Leg_y=abs(-sin(time)*30-30); //�������Ƹ� ��������(abs���밪�� �����ν� ���Ƹ��� ������ ������ �ʴ� �Ѱ����� ����)
	L_Leg_y=abs(sin(time)*30-30);  //�������Ƹ� ��������(abs���밪�� �����ν� ���Ƹ��� ������ ������ �ʴ� �Ѱ����� ����)
	R_Leg_x=sin(time)*60;          //�����ٸ��� 60�� �������� �����̵� sin()�Լ��� ����Ͽ� �ֱ����� ������ ����
	L_Leg_x=-R_Leg_x;             //�����ٸ��� �����ٸ��ݴ�� 60�� �������� �����̵� sin()�Լ��� ����Ͽ� �ֱ����� ������ ����

	////////////////display////////////////

	cyl=gluNewQuadric(); //�Ǹ��� ��ü ����
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT); //�ʱ�ȭ
	glMatrixMode(GL_MODELVIEW); //��� ����

	DrawGround(); //���� ȣ��
	glLoadIdentity(); //CTM �ʱ�ȭ
	gluLookAt(eye_x, eye_y, eye_z, 0.0, 0.0, 0.0, 0.0, 1.0, 0);	//ī�޶� ����

	glRotatef(-230.0, 0, 1, 0);//y������ �����
	/*
	�κ��� �ǰܵ��۽� ���� Ʋ������ ���� ǥ�� 
	*/
	glRotatef(sin(time)*7,0,0,1); //z��������� 7�� ���� ����Ʋ���� (sin()�Լ��� ����Ͽ� �ֱ����� ������ ����)
	glRotatef(sin(time)*7,0,1,0); //y������ 7�� ���� ����Ʋ���� (sin()�Լ��� ����Ͽ� �ֱ����� ������ ����)

	//�κ� ��ü ���� ����
	glTranslatef(0.0,0.18,0.0); //y������ �̵�
	glRotatef(80,1,0,0);        //x�� �������� ȸ��

	glTranslatef(0.0,0.5,0.0);//���� ��ġ 

	glPushMatrix( ); // ó�� ���� ��ġ

	DrawBody(0,0,0,0); // ���� �Լ� ȣ��

	glPopMatrix( ); // ó�� ���� ��ǥ�� ���� ����.
	glPushMatrix( );// ó�� ���� ��ǥ �ٽ� ����

	Drawneck(); // ���Լ� ȣ��

	glPopMatrix( ); // ó�� ���� ��ǥ�� ���� ����.
	glPushMatrix( );// ó�� ���� ��ǥ �ٽ� ����

	//�Ӹ� ��ġ ����
	glRotatef(-75,1,0,0); //x��������� ȸ��(�Ӹ��� �������� ������)
	glTranslatef(0.0,-0.02,0.0); //y������ �̵� (�Ӹ� ������)

	DrawHead(); // �Ӹ� ȣ��

	glPopMatrix( ); // ó�� ���� ��ǥ�� ���� ����.
	glPushMatrix( );// ó�� ���� ��ǥ �ٽ� ����

	//������ü�� ��ġ ����


	DrawR_Arm((R_Arm_y +30),1,0,0);  //������ȣ��
	DrawR_Hand(-(R_Arm_y  - 15),1,0,0); //�����ȶ�

	glPopMatrix( ); // ó�� ���� ��ǥ�� ���� ����.
	glPushMatrix( );// ó�� ���� ��ǥ �ٽ� ����

	//������ü�� ��ġ ����
	glTranslatef(0.0,-0.16,-0.04);//y��,z������ �̵�(������ ������)
	glRotatef(40,0,0,1);     //z�� �������� ȸ��

	DrawL_Arm((L_Arm_y +30),1,0,0);   //������ȣ��	
	DrawL_Hand( -(L_Arm_y  - 15),1,0,0);  //�����ȶ�

	glPopMatrix( ); // ó�� ���� ��ǥ�� ���� ����.
	glPushMatrix( );// ó�� ���� ��ǥ �ٽ� ����

	//������ü �ٸ� ��ġ ����
	glTranslatef(0.0,-0.45,-0.25);//y��,z������ �̵�(�����ٸ� ������)
	glRotatef(-90,1,0,0);  //x�� �������� ȸ��

	DrawL_Legs(-30,1,0,0);  //�����ٸ�
	DrawL_foot(10,1,0,0);  //�������Ƹ�	
	DrawSkateShoe();	//������Ʈ����

	glPopMatrix( ); // ó�� ���� ��ǥ�� ���� ����.
	glPushMatrix( );// ó�� ���� ��ǥ �ٽ� ����

	//������ü �ٸ� ��ġ ����
	glTranslatef(0.0,-0.5,-0.5);//y��,z������ �̵�(�����ٸ� ������)
	glRotatef(-90,1,0,0); //x�� �������� ȸ��

	DrawR_Legs(160,1,0,0);  //�����ٸ�
	DrawR_foot(R_Leg_y,1,0,0);  //�������Ƹ�		
	DrawSkateShoe();	//������Ʈ����

	glPopMatrix( ); // ó�� ���� ��ǥ�� ���� ����.

	glutSwapBuffers( );
}
/*
�ϴ��� ���� ����� ���ϴ� �Լ�
*/
void Fly()
{
	glLoadIdentity(); //CTM �ʱ�ȭ

	/*
	�κ��� �⺻���� ������ ������ ������ �����ϴ� ��
	*/
	L_Arm_x=(-40)+sin(time2)*60;//���� ����� ���������� -40���¿��� sin()�Լ��� ����Ͽ� �ֱ����� ������ ����
	R_Arm_x=-5+sin(time2)*6;	//���� ����� ���������� -5���¿��� sin()�Լ��� ����Ͽ� �ֱ����� ������ ����
	R_Arm_y=130+sin(time2)*6;	//�����ȶ� ��������(sin()�Լ����)
	L_Arm_y=20+sin(time2)*6;	//�����ȶ� ��������(sin()�Լ����)

	R_Leg_y=5+abs(sin(time)*20);//�����ٸ��� 5�� �������� �����̵� sin()�Լ��� ����Ͽ� �ֱ����� ������ ����
	L_Leg_y=5+abs(sin(time)*10);//�������Ƹ��� 5�� �������� �����̵� sin()�Լ��� ����Ͽ� �ֱ����� ������ ����
	R_Leg_x=90+(sin(time)*10);	//�����ٸ��� 90�� �������� �����̵� sin()�Լ��� ����Ͽ� �ֱ����� ������ ����
	L_Leg_x=90+(sin(time)*10);	//�����ٸ��� 90�� �������� �����̵� sin()�Լ��� ����Ͽ� �ֱ����� ������ ����

	////////////////display////////////////

	cyl=gluNewQuadric(); //�Ǹ��� ��ü ����
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT); //�ʱ�ȭ
	glMatrixMode(GL_MODELVIEW); //��� ����

	DrawGround(); //���� ȣ��
	glLoadIdentity(); //CTM �ʱ�ȭ
	gluLookAt(eye_x, eye_y, eye_z, 0.0, 0.0, 0.0, 0.0, 1.0, 0);	//ī�޶� ����

	glRotatef(-230.0, 0, 1, 0);//y������ �����
	/*
	�κ��� �ǰܵ��۽� ���� Ʋ������ ���� ǥ�� 
	*/
	glRotatef(sin(time)*15,0,0,1); //z��������� 15�� ���� ����Ʋ���� (sin()�Լ��� ����Ͽ� �ֱ����� ������ ����)
	glRotatef(sin(time)*10,0,1,0); //y������ 10�� ���� ����Ʋ���� (sin()�Լ��� ����Ͽ� �ֱ����� ������ ����)

	//�κ� ��ü ���� ����
	glTranslatef(0.0,0.18,0.0); //y������ �̵�
	glRotatef(80,1,0,0);        //x�� �������� ȸ��

	glTranslatef(0.0,0.5,0.0);//���� ��ġ 

	glPushMatrix( ); // ó�� ���� ��ġ

	DrawBody(0,0,0,0); // ���� �Լ� ȣ��

	glPopMatrix( ); // ó�� ���� ��ǥ�� ���� ����.
	glPushMatrix( );// ó�� ���� ��ǥ �ٽ� ����

	Drawneck(); // ���Լ� ȣ��

	glPopMatrix( ); // ó�� ���� ��ǥ�� ���� ����.
	glPushMatrix( );// ó�� ���� ��ǥ �ٽ� ����

	//�Ӹ� ��ġ ����
	glRotatef(-75,1,0,0); //x��������� ȸ��(�Ӹ��� �������� ������)
	glTranslatef(0.0,-0.02,0.0); //y������ �̵� (�Ӹ� ������)

	DrawHead(); // �Ӹ� ȣ��

	glPopMatrix( ); // ó�� ���� ��ǥ�� ���� ����.
	glPushMatrix( );// ó�� ���� ��ǥ �ٽ� ����

	//������ü�� ��ġ ����


	DrawR_Arm((R_Arm_y +30),1,0,0);  //������ȣ��
	DrawR_Hand(R_Arm_x,1,0,0); //�����ȶ�
	glRotatef(30, 0, 0, 1.0);
	DrawFist();	//���� �ָ�

	glPushMatrix( );// ó�� ���� ��ǥ �ٽ� ����

	//������ü�� ��ġ ����
	glTranslatef(0.0,-0.16,-0.04);//y��,z������ �̵�(������ ������)
	glRotatef(20,0,0,1);     //z�� �������� ȸ��

	DrawL_Arm((L_Arm_y - 30),1,0,0);   //������ȣ��	
	DrawL_Hand( -(L_Arm_y  - 15),1,0,0);  //�����ȶ�
	glRotatef(30, 0, 0, 1.0);
	DrawFist();	//���� �ָ�

	glPushMatrix( );// ó�� ���� ��ǥ �ٽ� ����

	//������ü �ٸ� ��ġ ����
	glTranslatef(0.1,-0.45,-0.35);//y��,z������ �̵�(�����ٸ� ������)
	glRotatef(-90,1,0,0);  //x�� �������� ȸ��

	DrawL_Legs(L_Leg_x,1,0,0);  //�����ٸ�
	DrawL_foot(L_Leg_y,1,0,0);  //�������Ƹ�	
	DrawSkateShoe();	//������Ʈ����(�̰� ������ ����)

	glPopMatrix( ); // ó�� ���� ��ǥ�� ���� ����.
	glPushMatrix( );// ó�� ���� ��ǥ �ٽ� ����

	//������ü �ٸ� ��ġ ����
	glTranslatef(-0.1,-0.5,-0.35);//y��,z������ �̵�(�����ٸ� ������)
	glRotatef(-90,1,0,0); //x�� �������� ȸ��

	DrawR_Legs(R_Leg_x,1,0,0);  //�����ٸ�
	DrawR_foot(R_Leg_y,1,0,0);  //�������Ƹ�	
	DrawSkateShoe();	//������Ʈ����(�̰� ������ ����)

	glPopMatrix( ); // ó�� ���� ��ǥ�� ���� ����.

	glutSwapBuffers( );
}
/*
�κ��� ������ ������ ����� ǥ���� �Լ�
*/
void Rocket(){
	glLoadIdentity(); //CTM �ʱ�ȭ

	/*90�� ������ ���� ������ ���δ�*/
	L_Arm_x=-90; 
	R_Arm_x=-90;

	R=2*abs(-sin(time2)*0.2-0.2)+0.2;//���� ���� ������ ����
	//abs���밪���� ������ �������� ������ ����.
	//+0.2�� ���� ������ ó����ġ ����. 2*�� ���� ������ ������ �Ÿ�����.
	//sin()�Լ��� ���� �ֱ����� ������ ����.

	R2=2*abs(sin(time2)*0.2-0.2)+0.2;//���� ���� ������ ���� 
	//abs���밪���� ������ �������� ������ ����.
	//+0.2�� ���� ������ ó����ġ ����. 2*�� ���� ������ ������ �Ÿ�����.
	//sin()�Լ��� ���� �ֱ����� ������ ����.

	R_Leg_y=abs(-sin(time)*30-30);  //�������Ƹ� ��������(abs���밪�� �����ν� ���Ƹ��� ������ ������ �ʴ� �Ѱ����� ����)
	L_Leg_y=abs(sin(time)*30-30);   //�������Ƹ� ��������(abs���밪�� �����ν� ���Ƹ��� ������ ������ �ʴ� �Ѱ����� ����)
	R_Leg_x=sin(time)*60;          //�����ٸ��� 60�� �������� �����̵� sin()�Լ��� ����Ͽ� �ֱ����� ������ ����
	L_Leg_x=-R_Leg_x;              //�����ٸ��� �����ٸ��ݴ�� 60�� �������� �����̵� sin()�Լ��� ����Ͽ� �ֱ����� ������ ����

	////////////////display////////////////

	cyl=gluNewQuadric(); //�Ǹ��� ��ü ����
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT); //�ʱ�ȭ
	glMatrixMode(GL_MODELVIEW); //��� ����

	DrawGround(); //���� ȣ��

	glLoadIdentity(); //CTM �ʱ�ȭ
	gluLookAt(eye_x, eye_y, eye_z, 0.0, 0.0, 0.0, 0.0, 1.0, 0);	//ī�޶� ����

	glPushMatrix();// ó�� ���� ��ǥ �ٽ� ����
	glRotatef(-230.0, 0, 1, 0);//y������ �����

	/*
	�κ��� ������ �� �� ��,��,��,��� ���� Ʈ�� ����� ����
	*/
	glRotatef(-abs(sin(time)*8),1,0,0); //x������ 8�� ���� ����Ʋ����(abs���밪�� �����ν� ��ü�� �ڷ� ������ �ʴ� �Ѱ����� ����)
	glRotatef(sin(time)*7,0,0,1);       //z������ 7�� ���� ����Ʋ����, sin()�Լ��� ����Ͽ� �ֱ����� ������ ����

	/*
	�κ��� ������ ������ ���� ƨ��� ����� ǥ��
	*/
	float i=0;    
	i=abs(sin(time)*0.08); //i�� ���� 
	glTranslatef(0.0,i,0); //���� i��ŭ �̵�
	glTranslatef(0.0,0.5,0.0);//���� ��ġ 
	DrawAndroid();		
	glutSwapBuffers();
}

/*
Ű���� �ݹ� ���� �Լ�
w�� ������ wire ����, s�� ������ solid ����, q�� ������ ���Ḧ ǥ��
*/
void MyKeyboard(unsigned char KeyPressed,int x,int y){
	static int toggle = 0;	//'z'Ű�� ������ �� �Ͻ�����/�簳�� �����ϴ� ����
	static int temp = 0;	//'z'Ű�� ������ �Ͻ������� ��ų �� �� ���� Ű�� ����
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
		/*�������� ����*/
	case '1':
		if(select_mod==PROJECTION){
			system("cls");
			cout<<"glOrtho()�� �Ű������� �����մϴ�.\nU : left\tI : right\tO : bottom\tP : top\t[ : zNear\t] : zFar\n"<<endl;}
		mode=ORTHO;
		break;
	case '2':
		if(select_mod==PROJECTION){
			system("cls");
			cout<<"glFrustum()�� �Ű������� �����մϴ�.\nU : left\tI : right\tO : bottom\tP : top\t[ : zNear\t] : zFar\n"<<endl;}
		mode=FRUSTUM;
		break;
	case '3':
		if(select_mod==PROJECTION){
			system("cls");
			cout<<"gluPerspective()�� �Ű������� �����մϴ�.\nO : fovy\tP : aspect\t[ : zNear\t] : zFar\n"<<endl;}
		mode=PERSPECTIVE;
		break;
		/*�κ� ���� ����*/
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
		/*MODELVIEW�� PROJECTION �� �Ű������� ������ ��� ����*/
	case '9':
		system("cls");
		cout<<"gluLookAt()�� �Ű������� �����մϴ�.\nJ : eye_x\tK : eye_y\tL : eye_z\n"<<endl;
		select_mod=MODELVIEW;
		break;
	case '0':
		system("cls");
		if(mode==ORTHO)
			cout<<"glOrtho()�� �Ű������� �����մϴ�.\nU : left\tI : right\tO : bottom\tP : top\t[ : zNear\t] : zFar\n"<<endl;
		else if(mode==FRUSTUM)
			cout<<"glFrustum()�� �Ű������� �����մϴ�.\nU : left\tI : right\tO : bottom\tP : top\t[ : zNear\t] : zFar\n"<<endl;
		else	//PERSPECTIVE
			cout<<"gluPerspective()�� �Ű������� �����մϴ�.\nO : fovy\tP : aspect\t[ : zNear\t] : zFar\n"<<endl;
		select_mod=PROJECTION;
		break;
		/*MODELVIEW���� ������ �Ű����� ����*/
	case 'j':
		cout<<"eye_x ����"<<endl;
		eye_co=X;
		break;
	case 'k':
		cout<<"eye_y ����"<<endl;
		eye_co=Y;
		break;
	case 'l':
		cout<<"eye_z ����"<<endl;
		eye_co=Z;
		break;
		/*PROJECTION���� ������ �Ű����� ����*/
	case 'u':
		if(mode!=PERSPECTIVE){
			cout<<"left ����"<<endl;
			proj_co=LEFT;}
		break;
	case 'i':
		if(mode!=PERSPECTIVE){
			cout<<"right ����"<<endl;
			proj_co=RIGHT;}
		break;
	case 'o':
		if(mode==PERSPECTIVE){
			cout<<"fovy ����"<<endl;
			proj_co=FOVY;}
		else{//glOrtho, glFrustum
			cout<<"bottom ����"<<endl;
			proj_co=BOTTOM;}
		break;
	case 'p':
		if(mode==PERSPECTIVE){
			cout<<"aspect ����"<<endl;
			proj_co=ASPECT;}
		else{//glOrtho, glFrustum
			cout<<"top ����"<<endl;
			proj_co=TOP;}
		break;
	case '[':
		cout<<"zNear ����"<<endl;
		proj_co=zNear;
		break;
	case ']':
		cout<<"zFar ����"<<endl;
		proj_co=zFar;
		break;
		/*�Ű����� ����, (+)�� 0.1�� ����, (-)�� 0.1�� ����*/
	case '+': case '-':
		key_operation(KeyPressed);
		break;
		/*�޴� ���*/
	case 'm':
		select_mod = NONE;	//�ʱ�ȭ
		print_menual();
		break;
	}
	redisplay_all();
}
/*
���õ� �������� ���� �Ű������� ���� �� �����Ѵ�.
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
Ÿ�̸� �Լ�
�� ��鸶�� ���� ǥ���� �ٸ��� �α� ���� ����� �ð� �������� ������.
*/
void MyTimer(int Value){
	time=time+0.1;//Run()�� ���� Ÿ�̸� ����
	time2=time2+0.5;//Jap()�� ���� Ÿ�̸� ����
	time3=time3+0.01;//Rocket()�� ���� Ÿ�̸� ����
	time4=time4+1.0;//Show()�� Fly()�� ���� Ÿ�̸� ����
	glutPostRedisplay();
	glutTimerFunc(40,MyTimer,1);
}
/*
���콺 ������ Ŭ���� �ؼ� �޴��� �����ϸ� ����Ǵ� �Լ�.
�⺻������ ����� ��ȯ�Ҷ� ���콺 �ݹ��� �̿��Ͽ� ��ȯ�ϵ��� �����Ͽ���.
*/
void MyMainMenu(int entryID){ //�޴�
	if(entryID != 7)
		key=entryID;
	else{	//7�� ��, �޴� ���⸦ Ŭ���Ͽ��� ��
		select_mod = NONE;	//�ʱ�ȭ
		print_menual();
	}
}
/*
������ȯ Reshape
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
	�� ��忡 ���߾� PROJECTION �Ѵ�.
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
screen reshape ������ 
ȭ�鿡 �ٷ� �����
*/
void redisplay_all(void)
{
	screen_reshape(WinWidth, WinHeight);
	glutPostRedisplay();
}
/*
����޴� ����
*/
void menu_proj(int none){}	//�߰��ܰ��� �޴��̹Ƿ� ������ ����.
void menu_mod(int none){}	//�߰��ܰ��� �޴��̹Ƿ� ������ ����.
void menu_select_proj(int entryID){//������ȯ�� ���� ����޴�
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
void menu_coor(int entryID){//gluLookAt()�� ������ �Ű����� ����
	system("cls");
	cout<<"gluLookAt()�� �Ű������� �����մϴ�.\nJ : eye_x\tK : eye_y\tL : eye_z\n"<<endl;
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
void menu_glPM(int entryID){//glOrtho() Ȥ�� glFrustum()�� ������ �Ű����� ����
	system("cls");	//cmd ȭ�� �ʱ�ȭ
	cout<<"glOrtho() Ȥ�� glFrustum()�� �Ű������� �����մϴ�.\nU : left\tI : right\tO : bottom\tP : top\t[ : zNear\t] : zFar\n"<<endl;
	select_mod=PROJECTION;
	if(mode==PERSPECTIVE)	
		mode=ORTHO;	//�� �Լ��� ������ �°� ����
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
void menu_gluPM(int entryID){//gluPerspective()�� ������ �Ű����� ����
	system("cls");	//cmd ȭ�� �ʱ�ȭ
	cout<<"gluPerspective()�� �Ű������� �����մϴ�.\nO : fovy\tP : aspect\t[ : zNear\t] : zFar\n"<<endl;
	select_mod=PROJECTION;
	mode=PERSPECTIVE;	//�� �Լ��� ������ �°� ����
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
�� key ���� ���� �� ����� ���Ѵ�.
*/
void MyDisplay(){
	if(key==RUN){//�κ��� �޸���		
		Run();
		glPopMatrix( );
	}
	else if(key==JAP){//�κ��� ���� ���� ��
		Jap();
		glPopMatrix( );		
	}
	else if(key==ROCKET){//�κ��� ������ �߻��Ҷ�
		Rocket();
		glPopMatrix( );
	}
	else if(key==SHOW){//�κ��� �ǰܽ������� ����� ���� ��
		Show();
		glPopMatrix( );
	}
	else if(key==FLY){//�κ��� ���� ����� ���� ��
		Fly();
		glPopMatrix();
	}
	else if(key==PAUSE){//������ ���õ��� ��
		sndPlaySound(NULL,SND_ASYNC);
	}
}

int main(int argc, char** argv){
	glutInit(&argc, argv);//�ʱ�ȭ
	glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE | GLUT_DEPTH);//���÷��� ��� ����
	glutInitWindowSize(800,800);//������ ũ�� ����
	glutInitWindowPosition(0,0);//������ �ʱ� ��ġ ����
	glutCreateWindow("2010136019_�赿��_Assignment07");//������ ����
	glInit(); //���� �ʱ�ȭ
	print_menual();	//�޴��� ���

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

	GLint MyMainMenuID = glutCreateMenu(MyMainMenu);//�޴��߰�
	glutAddSubMenu("select Projection", MySubMenuID1);//������ȯ �޴� �߰�
	glutAddMenuEntry("RUN",1);//�κ��� �޸��� ��� �޴� �߰�
	glutAddMenuEntry("JAP",2);//�κ��� �쳯���� ��� �޴� �߰�
	glutAddMenuEntry("ROCKET",3);//�κ��� ������ġ�� ������ ��� �޴� �߰�
	glutAddMenuEntry("SHOW",4);//�κ��� �ǰܽ��������ϴ� ��� �޴� �߰�
	glutAddMenuEntry("FLY",5);//�κ��� �ϴ��� ���� ��� �޴� �߰�
	glutAddMenuEntry("����/�簳",6);//�κ��� �ϴ� ���� ���߰� �ϰų� �簳�ϴ� �޴� �߰�
	glutAddSubMenu("�Ű����� ��ȯ", MySubMenuID2);
	glutAddMenuEntry("�޴� ����", 7);//cmdâ�� �ʱ�ȭ�ϰ� �޴��� ����մϴ�.
	glutAttachMenu(GLUT_RIGHT_BUTTON);//�����ʸ��콺Ŭ���ø޴��˾�

	glutKeyboardFunc(MyKeyboard); //Ű���� �ݹ�
	glutTimerFunc(40,MyTimer,1); //Ÿ�̸� �Լ�
	glutReshapeFunc(screen_reshape); //Reshape
	glutDisplayFunc(MyDisplay); //���÷���

	glutMainLoop( );
	return 0;
}