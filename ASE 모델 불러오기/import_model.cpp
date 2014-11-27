//업데이트 확인용
#include <iostream>
#include <stdlib.h>
#include <math.h> //abs()사용
#include "ase.h" //ASE 파일에서 이미지 로딩을 위함
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <string.h>
using namespace std;

int ViewX = 0, ViewY = 0; //마우스 움직임에 따라 시점을 바꾸기 위한 변수

float angle = 0.0f; // 카메라용 앵글 1
float angle2 = 0.0f; // 카메라용 앵글 2
int tx = 0, ty = 0;

GLfloat mat_diffuse[6]; //?
GLfloat mat_ambient[6]; //?

float scale; //메시 크기를 조절하는 단위
Mesh *Head; //머리
/*
R,G,B와 a로 색조정?
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
메시의 조명 설정
*/
void InitLight(){
	// mesh 의 재질 색, 광원 값
	GLfloat mat_specular[ ] = {0.5, 1.0, 0.5, 1.0};
	GLfloat mat_shininess[ ] = {70.0};
	// 조명의 색, 광원 값
	GLfloat light_specular[ ] = {0.8, 0.5, 0.8, 1.0};
	GLfloat light_diffuse[ ] = {0.7, 0.7, 0.7, 1.0};
	GLfloat light_ambient[ ] = {0.4, 0.4, 0.4, 1.0};
//	GLfloat light_position[ ] = {-1.5 / scaleRate, 0.5 / scaleRate, 0.5, 0.0};
	GLfloat light_position[ ] = {-0.2, -0.5, 0.1, 0.0}; //수정함
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
/*
mesh 의 회전방향, 이동 위치 좌표를 초기화
*/
void meshInit() {
	/*
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
	*/
	scale = (float)floor((double)(Head->max+0.5)) * 2;
	cout<<"scale : "<<scale<<endl; //scale값 알아보기 
}
/*
메시 로딩
*/
void meshLoad(){
	Head = new Mesh();
	Head->readAse("S_Head.ASE");
//	Head->readAse("mask.ASE");
/*
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
*/
}
/*
각 파트를 그리는 함수
*/
void draw(Mesh *m){
	glBegin(GL_TRIANGLES);	
	for(int i=0; i<m->faceNum; i++){
		glNormal3f(m->normalList[i].faceNormal[0], m->normalList[i].faceNormal[1], m->normalList[i].faceNormal[2]);

		glVertex3f(m->vertexList[Head->faceList[i].vertexIndex[0]].x / scale, 
			m->vertexList[Head->faceList[i].vertexIndex[0]].y / scale, 
			m->vertexList[Head->faceList[i].vertexIndex[0]].z / scale);	

		glVertex3f(m->vertexList[Head->faceList[i].vertexIndex[1]].x / scale, 
			m->vertexList[Head->faceList[i].vertexIndex[1]].y / scale, 
			m->vertexList[Head->faceList[i].vertexIndex[1]].z / scale);

		glVertex3f(m->vertexList[Head->faceList[i].vertexIndex[2]].x / scale, 
			m->vertexList[Head->faceList[i].vertexIndex[2]].y / scale, 
			m->vertexList[Head->faceList[i].vertexIndex[2]].z / scale);
	}
	glEnd( );
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
/*
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
*/
/*
각 파트를 조립하여 하나의 로봇을 그림
*/
/*
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
*/
/*
클릭한 마우스의 움직임에 관한 함수(물체 회전에 관한 함수)
*/
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
/*
 마우스 움직임에 관한 함수
 */
void MyMouseMove(int botton, int state, GLint X, GLint Y){
    if (botton == GLUT_LEFT_BUTTON && state == GLUT_DOWN){	//좌클릭시
        ViewX = X;	//마우스의 좌표를 기록한다.
        ViewY = Y;
		glutPostRedisplay();
    }
}
void MyDisplay(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 중복 그려지기 방지

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glRotatef(angle, 0.0, 1.0, 0.0);
	glRotatef(angle2, 1.0, 0.0, 0.0);
	
	setColor(0.8, 0.8, 0.8, 1.0);
	draw(Head);
	glFlush();
}
void MyKeyboard(unsigned char KeyPressed,int x,int y){
	switch (KeyPressed){
		case 'q':
			exit(0);
			break;
		case '+':
			scale+=0.1;
			break;
		case '-':
			scale-=0.1;
			break;
	}
	cout<<"scale : "<<scale<<endl;
	glutPostRedisplay();
}

int main(int argc, char** argv){
	glutInit(&argc, argv);
//	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE); //화면을 하얗게 만듬
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("TEST");
//	glClearColor(1.0, 1.0, 1.0, 1.0); //화면을 하얗게
	glClearColor(0.0, 0.5, 0.8, 1.0);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //이게 있어야 출력됨
	InitLight();       // 조명값 설정
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);  // 숨겨진 면을 제거, 시점으로부터 mesh 의 각 픽셀의 깊이 값을 계산하여 출력
	glShadeModel(GL_SMOOTH);	 
	
	meshLoad();
	meshInit();

	glutMouseFunc(MyMouseMove);
	glutMotionFunc(MyRotate);
	glutKeyboardFunc(MyKeyboard); //키보드 콜백
	glutDisplayFunc(MyDisplay);
	glutMainLoop();

	delete Head;
	return 0;
}