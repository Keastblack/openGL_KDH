#include <iostream>
#include <stdlib.h>
#include <math.h> //abs()���
#include "ase_����.h" //ASE ���Ͽ��� �̹��� �ε��� ����
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <string.h>
using namespace std;

int ViewX = 0, ViewY = 0; //���콺 �����ӿ� ���� ������ �ٲٱ� ���� ����

float angle = 0.0f; // ī�޶�� �ޱ� 1
float angle2 = 0.0f; // ī�޶�� �ޱ� 2
int tx = 0, ty = 0;

GLfloat mat_diffuse[6]; //?
GLfloat mat_ambient[6]; //?

GLfloat scale=193.0;

Mesh *Head; //�Ӹ�
Mesh *Bar; //��
Mesh *Block; //���

//Mesh *F, *A, *I, *L;
Mesh *FAIL[4];
Mesh *COMPLETE[8];

GLfloat l_position[]={-0.7,-0.5,0.1,3.5};

bool start=true;
void MyTimer(int);
/*
R,G,B�� a�� ������?
*/
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
/*
�޽��� ���� ����
*/
void InitLight(GLfloat x=-0.2, GLfloat y=-0.5, GLfloat z=0.1){
	// mesh �� ���� ��, ���� ��
	GLfloat mat_specular[ ] = {0.5, 1.0, 0.5, 1.0};
	GLfloat mat_shininess[ ] = {70.0};
	// ������ ��, ���� ��
	GLfloat light_specular[ ] = {0.8, 0.5, 0.8, 1.0};
	GLfloat light_diffuse[ ] = {0.7, 0.7, 0.7, 1.0};
	GLfloat light_ambient[ ] = {0.4, 0.4, 0.4, 1.0};
	GLfloat light_position[ ] = {x, y, z, 0.0};
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);  // ���� ��� ����
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE); // ����ȭ
	// mesh �� ���� ��, ����  ��  ������ ��, ���� ����
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
}
void InitLight(GLfloat *params){
	// mesh �� ���� ��, ���� ��
	GLfloat mat_specular[ ] = {0.5, 1.0, 0.5, 1.0};
	GLfloat mat_shininess[ ] = {70.0};
	// ������ ��, ���� ��
	GLfloat light_specular[ ] = {0.8, 0.5, 0.8, 1.0};
	GLfloat light_diffuse[ ] = {0.7, 0.7, 0.7, 1.0};
	GLfloat light_ambient[ ] = {0.4, 0.4, 0.4, 1.0};
//	GLfloat *params; //�Լ��� ���ڰ����� ��ü
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);  // ���� ��� ����
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE); // ����ȭ
	// mesh �� ���� ��, ����  ��  ������ ��, ���� ����
	glLightfv(GL_LIGHT0, GL_POSITION, params);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
}
/*�޽� �ε�*/
void meshLoad(){
	Head = new Mesh();
	Head->readAse("S_Head.ASE");
	Head->setScale(50.0);

	Bar = new Mesh();
	Bar->readAse("Bar.ASE");
	Bar->setScale(140.0);
	
	FAIL[0] = new Mesh();
	FAIL[0]->readAse("F.ASE");
	FAIL[0]->setScale(179.0);

	FAIL[1] = new Mesh();
	FAIL[1]->readAse("A.ASE");
	FAIL[1]->setScale(195.0);

	FAIL[2] = new Mesh();
	FAIL[2]->readAse("I.ASE");
	FAIL[2]->setScale(195.0);

	FAIL[3] = new Mesh();
	FAIL[3]->readAse("L.ASE");
	FAIL[3]->setScale(200.0);

	COMPLETE[0] = new Mesh();
	COMPLETE[0]->readAse("C.ASE");
	COMPLETE[0]->setScale(193.0);

	COMPLETE[1] = new Mesh();
	COMPLETE[1]->readAse("O.ASE");
	COMPLETE[1]->setScale(280.0);

	COMPLETE[2] = new Mesh();
	COMPLETE[2]->readAse("M.ASE");
	COMPLETE[2]->setScale(235.0);

	COMPLETE[3] = new Mesh();
	COMPLETE[3]->readAse("P.ASE");
	COMPLETE[3]->setScale(260.0);

	COMPLETE[4] = new Mesh();
	COMPLETE[4]->readAse("L.ASE");
	COMPLETE[4]->setScale(260.0);

	COMPLETE[5] = new Mesh();
	COMPLETE[5]->readAse("E.ASE");
	COMPLETE[5]->setScale(220.0);

	COMPLETE[6] = new Mesh();
	COMPLETE[6]->readAse("T.ASE");
	COMPLETE[6]->setScale(290.0);

	COMPLETE[7] = new Mesh();
	COMPLETE[7]->readAse("E.ASE");
	COMPLETE[7]->setScale(220.0);
}
/*�޽� �׸���*/
void draw_mesh(Mesh *m){
	static float scale;
	scale = m->getScale();

	glBegin(GL_TRIANGLES);	
	for(int i=0; i<m->faceNum; i++){
		glNormal3f(m->normalList[i].faceNormal[0], m->normalList[i].faceNormal[1], m->normalList[i].faceNormal[2]);

		glVertex3f(m->vertexList[m->faceList[i].vertexIndex[0]].x / scale, 
			m->vertexList[m->faceList[i].vertexIndex[0]].y / scale, 
			m->vertexList[m->faceList[i].vertexIndex[0]].z / scale);	

		glVertex3f(m->vertexList[m->faceList[i].vertexIndex[1]].x / scale, 
			m->vertexList[m->faceList[i].vertexIndex[1]].y / scale, 
			m->vertexList[m->faceList[i].vertexIndex[1]].z / scale);

		glVertex3f(m->vertexList[m->faceList[i].vertexIndex[2]].x / scale, 
			m->vertexList[m->faceList[i].vertexIndex[2]].y / scale, 
			m->vertexList[m->faceList[i].vertexIndex[2]].z / scale);
	}
	glEnd();
}
void draw_mesh2(Mesh *m){
//	static float scale = m->getScale();

	glBegin(GL_TRIANGLES);	
	for(int i=0; i<m->faceNum; i++){
		glNormal3f(m->normalList[i].faceNormal[0], m->normalList[i].faceNormal[1], m->normalList[i].faceNormal[2]);

		glVertex3f(m->vertexList[m->faceList[i].vertexIndex[0]].x / scale, 
			m->vertexList[m->faceList[i].vertexIndex[0]].y / scale, 
			m->vertexList[m->faceList[i].vertexIndex[0]].z / scale);	

		glVertex3f(m->vertexList[m->faceList[i].vertexIndex[1]].x / scale, 
			m->vertexList[m->faceList[i].vertexIndex[1]].y / scale, 
			m->vertexList[m->faceList[i].vertexIndex[1]].z / scale);

		glVertex3f(m->vertexList[m->faceList[i].vertexIndex[2]].x / scale, 
			m->vertexList[m->faceList[i].vertexIndex[2]].y / scale, 
			m->vertexList[m->faceList[i].vertexIndex[2]].z / scale);
	}
	glEnd();
}
/*Ŭ���� ���콺�� �����ӿ� ���� �Լ�(��ü ȸ���� ���� �Լ�)*/
void MyRotate(int x, int y){
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
/*���콺 �����ӿ� ���� �Լ�*/
void MyMouseMove(int botton, int state, GLint X, GLint Y){
    if (botton == GLUT_LEFT_BUTTON && state == GLUT_DOWN){	//��Ŭ����
        ViewX = X;	//���콺�� ��ǥ�� ����Ѵ�.
        ViewY = Y;
		glutPostRedisplay();
    }
}
/*���÷���*/
void MyDisplay(){
	static float time1=0.0, time2=0.5, time3=1.0, time4=1.5;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // �ߺ� �׷����� ����

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glRotatef(angle, 0.0, 1.0, 0.0);
	glRotatef(angle2, 1.0, 0.0, 0.0);

	glPushMatrix();
	setColor(0.8, 0.7, 0.2, 1.0);
	glTranslatef(-0.8,0.05+0.1*sin(time1),0.0);
	glRotatef(25.0*sin(time1),0.8,0.5,1.0);
	time1+=0.0025;
	draw_mesh(COMPLETE[0]);
	glPopMatrix();

	glPushMatrix();
	setColor(0.1, 0.5, 0.6, 1.0);
	glTranslatef(-0.55,0.05+0.1*cos(time2),0.0);
	glRotatef(45.0*sin(time2),0.0,1.0,1.0);
	time2+=0.003;
	draw_mesh(COMPLETE[1]);
	glPopMatrix();

	glPushMatrix();
	setColor(0.1, 0.5, 0.8, 1.0);
	glTranslatef(-0.3+0.07*cos(time3),0.04*cos(time3),0.0);
	glRotatef(45.0*sin(time3),0.0,0.0,1.0);
	time3+=0.006;
	draw_mesh(COMPLETE[2]);
	glPopMatrix();

	glPushMatrix();
	setColor(0.4, 0.7, 0.1, 1.0);
	glTranslatef(-0.1,-0.05,0.0);
	glRotatef(60.0*sin(time4),0.0,1.0,0.5);
	time4+=0.0035;
	draw_mesh(COMPLETE[3]);
	glPopMatrix();

	glPushMatrix();
	setColor(0.8, 0.7, 0.2, 1.0);
	glTranslatef(0.2,0.05+0.1*cos(time1),0.0);
	glRotatef(45.0*sin(time1),1.0,1.0,0.0);
	time1+=0.0025;
	draw_mesh(COMPLETE[4]);
	glPopMatrix();

	glPushMatrix();
	setColor(0.1, 0.5, 0.6, 1.0);
	glTranslatef(0.4,0.05+0.1*sin(time2),0.0);
	glRotatef(45.0*sin(time2),0.0,1.0,1.0);
	time2+=0.003;
	draw_mesh(COMPLETE[5]);
	glPopMatrix();

	glPushMatrix();
	setColor(0.1, 0.5, 0.8, 1.0);
	glTranslatef(0.6+0.02*cos(time3),0.04*cos(time3),0.0);
	glRotatef(45.0*sin(time3),0.0,0.0,1.0);
	time3+=0.00025;
	draw_mesh(COMPLETE[6]);
	glPopMatrix();

	glPushMatrix();
	setColor(0.4, 0.7, 0.1, 1.0);
	glTranslatef(0.8,-0.05,0.0);
	glRotatef(60.0*cos(time4),0.0,1.0,0.5);
	time4+=0.0035;
	draw_mesh(COMPLETE[7]);
	glPopMatrix();

	glFlush();
}
/*Ű����*/
void MyKeyboard(unsigned char KeyPressed,int x,int y){
	static GLfloat *p = l_position;
	
	switch (KeyPressed){
		case 'q':
			exit(0);
			break;
	/*
		case 'x':
			system("cls");
			cout<<"x�� ����\n\n";
			p=l_position;
			break;
		case 'y':
			system("cls");
			cout<<"y�� ����\n\n";
			p=l_position+1;
			break;
		case 'z':
			system("cls");
			cout<<"z�� ����\n\n";
			p=l_position+2;
			break;
		case 'n':
			system("cls");
			cout<<"n�� ����\n\n";
			p=l_position+3;
			break;
		case '+':
			*p+=0.1;
			printf("x : %.1f\ty : %.1f\tz : %.1f\tn : %.1f\n", *l_position, *(l_position+1), *(l_position+2), *(l_position+3));
			break;
		case '-':
			*p-=0.1;
			printf("x : %.1f\ty : %.1f\tz : %.1f\tn : %.1f\n", *l_position, *(l_position+1), *(l_position+2), *(l_position+3));
			break;
	*/
		case '+':
			scale+=1.0;
			cout<<scale<<endl;
			break;
		case '-':
			scale-=1.0;
			cout<<scale<<endl;
			break;
		case 'r':
			start=true;
			//glutPostRedisplay();
			glutTimerFunc(40, MyTimer, 1);
			break;
		case 's':
			start=false;
			break;
	}
	glutPostRedisplay();
}
/*Ÿ�̸�*/
void MyTimer(int value){
	if(start){
		glutPostRedisplay();
		glutTimerFunc(1,MyTimer,1);
	}
}
	
/*����*/
int main(int argc, char** argv){
	glutInit(&argc, argv);
//	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE); //ȭ���� �Ͼ�� ����
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("TEST");
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //�̰� �־�� ��µ�
//	glClearColor(1.0, 1.0, 1.0, 1.0); //ȭ���� �Ͼ��
	glClearColor(0.0, 0.5, 0.8, 1.0);
	InitLight();       // ���� ����

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);  // ������ ���� ����, �������κ��� mesh �� �� �ȼ��� ���� ���� ����Ͽ� ���
	glShadeModel(GL_SMOOTH);
	
	meshLoad();
//	meshInit();

	glutMouseFunc(MyMouseMove);
	glutMotionFunc(MyRotate);
	glutKeyboardFunc(MyKeyboard); //Ű���� �ݹ�
	glutDisplayFunc(MyDisplay);
	glutTimerFunc(1,MyTimer,1);
	glutMainLoop();

	if(Head)
		delete Head;
	if(Bar)
		delete Bar;

	delete[] FAIL;
	delete[] COMPLETE;

	return 0;
}