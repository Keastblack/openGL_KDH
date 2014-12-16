/////////////////////*2010136019 �赿�� *//////////////////////////////
///////////��ǻ�� �׷��Ƚ� ��������Ʈ-�������� ����////////////////////

/*****��Ʈ�� Ű*****/
/********************
1 : ��Ʈ��� ���
2 : CG��� ���
3 : ����� ���
4 : ����� ���

+ : ���� �ӵ��� 0.001��ŭ ����
- : ���� �ӵ��� 0.001��ŭ ����

Q : ����
V : �ڵ��¸�(�ÿ���)
********************/

/*�� �Լ����� static ������ ����Ǿ� �ִ�.*/
#include <iostream>
#include <math.h>
#include<windows.h>
#include<MMSystem.h> //���� ���(sndPlaySound())
#include "ase_mod.h"
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

using namespace std;

#define PI 3.14159265

/*�� ����ü*/
typedef struct point{
	GLfloat x;
	GLfloat y;
}point;
/*ball ��ü*/
class ball{
	GLfloat speed; //1�и��ʴ� ���� �̵��Ÿ� (��Į�� ��)
	point position; //ball�� ��ġ
	GLfloat angle;
public:
	ball(){speed=0.004; position.x=0.0; position.y=0.0; angle=40.0;} //�ʴ� 0.0~1.0�� ������ / (-1.0,0.0)���� ���� / ���� (0.0, -0.7)
	void set_speed(GLfloat s){speed=s;}
	GLfloat get_speed(){return speed;}
	void set_positon(GLfloat _x,GLfloat _y){position.x=_x; position.y=_y;}		
	point get_position(){return position;}
	void set_angle(GLfloat ang){
		if(ang>=360)
			angle=ang-360;
		else if(ang<=0)
			angle=ang+360;
		else //0<ang<360
			angle=ang;}		
	GLfloat get_angle(){return angle;}
};
/*��(bar) ��ü - ���콺�� ����Ͽ� �¿�� �����δ�.*/
class bar{
	point position; //bar�� ���� ��ġ
	GLfloat width; //���� ����(��)
	GLfloat speed; //10�и��ʴ� ���� �̵��Ÿ� (���� ��)
public:
	bar(){width=0.4;}
	void set_positon(GLfloat _x,GLfloat _y){position.x=_x; position.y=_y;}
	point get_position(){return position;}
	void set_width(GLfloat w){width=w;}
	GLfloat get_width(){return width;}
	void set_speed(GLfloat s){speed=s;}
	GLfloat get_speed(){return speed;}
};
/*���簢�� ���ع�(block)*/
class square{
	point position;
	GLfloat width;
	square *prev;
	square *next; //���� ����Ʈ
public:
	square(GLfloat _x=0.0, GLfloat _y=0.0, GLfloat _width=0.1){
		position.x=_x; position.y=_y; width=_width;}
	void set_positon(point p){position.x=p.x; position.y=p.y;}		
	void set_positon(GLfloat _x,GLfloat _y){position.x=_x; position.y=_y;}		
	point get_position(){return position;}
	void set_width(GLfloat w){width=w;}
	GLfloat get_width(){return width;}
	void set_prev(square *p){prev=p;}
	square* get_prev(){return prev;}
	void set_next(square *n){next=n;}
	square* get_next(){return next;}
};

int WinWidth = 800, WinHeight = 1600; //������ ũ��
GLfloat Cursor = 0; //���콺 Ŀ���� x��ǥ

ball pb; //playball
bar pbar; //playbar
square *s_head=NULL; //Heart ��� ���� �׸��µ� ���Ǵ� ���Ḯ��Ʈ�� ������

bool alive = true; //ball�� ȭ�� �Ʒ��� �������� false
bool complete = false; //��� ����� ���ָ� true

Mesh *Bar; //Bar �޽�
Mesh *Ball; //Bar �޽�
Mesh *FAIL[4]; //FAIL �޽�
Mesh *COMPLETE[8]; //COMPLETE �޽�

square temp_square;

enum{
	HEART, CG, HOME, BOTTLE, VICTORY, NONE
}block_model=NONE;

void MyTimer(int);
/*
R,G,B�� a�� ������
*/
void setColor(float r, float g, float b, float a){
	static GLfloat mat_ambient[6]; //��ü�� �ֺ��� ���
	static GLfloat mat_diffuse[6]; //��ü�� Ȯ�걤 ���

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
void InitLight(GLfloat x=-0.2, GLfloat y=-0.5, GLfloat z=0.1, GLfloat n=0.5){ //x,y,z,n�� �Ű����� ���޹���
	// mesh �� ���� ��, ���� ��
	GLfloat mat_specular[ ] = {0.5, 1.0, 0.5, 1.0};
	GLfloat mat_shininess[ ] = {70.0};
	// ������ ��, ���� ��
	GLfloat light_specular[ ] = {0.8, 0.5, 0.8, 1.0};
	GLfloat light_diffuse[ ] = {0.7, 0.7, 0.7, 1.0};
	GLfloat light_ambient[ ] = {0.4, 0.4, 0.4, 1.0};
	GLfloat light_position[ ] = {x, y, z, n};
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);  // ���� ��� ����
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE); // ����ȭ
	/* mesh �� ���� ��, ����  ��  ������ ��, ���� ����*/
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
}
/*
�޽� �ε�
*/
void meshLoad(){
	Bar = new Mesh();
	Bar->readAse("ASE\\Bar.ASE");
	Bar->setScale(140.0);

	Ball = new Mesh();
	Ball->readAse("ASE\\Ball.ASE");
	Ball->setScale(701.0);

	FAIL[0] = new Mesh();
	FAIL[0]->readAse("ASE\\F.ASE");
	FAIL[0]->setScale(179.0);

	FAIL[1] = new Mesh();
	FAIL[1]->readAse("ASE\\A.ASE");
	FAIL[1]->setScale(195.0);

	FAIL[2] = new Mesh();
	FAIL[2]->readAse("ASE\\I.ASE");
	FAIL[2]->setScale(195.0);

	FAIL[3] = new Mesh();
	FAIL[3]->readAse("ASE\\L.ASE");
	FAIL[3]->setScale(200.0);

	COMPLETE[0] = new Mesh();
	COMPLETE[0]->readAse("ASE\\C.ASE");
	COMPLETE[0]->setScale(193.0);

	COMPLETE[1] = new Mesh();
	COMPLETE[1]->readAse("ASE\\O.ASE");
	COMPLETE[1]->setScale(280.0);

	COMPLETE[2] = new Mesh();
	COMPLETE[2]->readAse("ASE\\M.ASE");
	COMPLETE[2]->setScale(235.0);

	COMPLETE[3] = new Mesh();
	COMPLETE[3]->readAse("ASE\\P.ASE");
	COMPLETE[3]->setScale(260.0);

	COMPLETE[4] = new Mesh();
	COMPLETE[4]->readAse("ASE\\L.ASE");
	COMPLETE[4]->setScale(260.0);

	COMPLETE[5] = new Mesh();
	COMPLETE[5]->readAse("ASE\\E.ASE");
	COMPLETE[5]->setScale(220.0);

	COMPLETE[6] = new Mesh();
	COMPLETE[6]->readAse("ASE\\T.ASE");
	COMPLETE[6]->setScale(290.0);

	COMPLETE[7] = new Mesh();
	COMPLETE[7]->readAse("ASE\\E.ASE");
	COMPLETE[7]->setScale(220.0);
}
/*bar�� ASE���Ϸκ��� import�ؼ� �׸�*/
void draw_bar(Mesh *m){
	static float scale = m->getScale();

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
	for(int i=0; i<m->faceNum; i++){ //�Ǽ��� bar�׸��� ������ �׷���, x�� ��Ī���� �� �� �׸��� �Ͽ���.
		glVertex3f(-m->vertexList[m->faceList[i].vertexIndex[0]].x / scale, 
			m->vertexList[m->faceList[i].vertexIndex[0]].y / scale, 
			m->vertexList[m->faceList[i].vertexIndex[0]].z / scale);	

		glVertex3f(-m->vertexList[m->faceList[i].vertexIndex[1]].x / scale, 
			m->vertexList[m->faceList[i].vertexIndex[1]].y / scale, 
			m->vertexList[m->faceList[i].vertexIndex[1]].z / scale);

		glVertex3f(-m->vertexList[m->faceList[i].vertexIndex[2]].x / scale, 
			m->vertexList[m->faceList[i].vertexIndex[2]].y / scale, 
			m->vertexList[m->faceList[i].vertexIndex[2]].z / scale);
	}
	glEnd();
}
/*ASE���Ϸκ��� import�ؼ� �׸�*/
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
/*�޴� ���*/
void print_menu(){
	system("cls");
	cout<<"<�������� ����>\n\n";
	cout<<"���콺�� �¿� ����\n\n1 : ��Ʈ��� ���\n2 : CG��� ���\n3 : ����� ���\n4 : ����� ���\n\n+ : ���� �ӵ� ����\n- : ���� �ӵ� ����\n\nQ : ����\n\n";
}
/*��(��Ÿ)���� radian������ �ٲ۴�.*/
GLfloat radian(GLfloat ang){
	return ang*PI/180.0;
}
/*������ ���� �浹�� ��*/
void wall_crash(point *move){
	static GLfloat _angle;

	_angle=pb.get_angle();
	if(move->x>=0.975 || move->x<=-0.975 || move->y>=0.975){ // ������ ���鿡 �ε����� ���
		sndPlaySound(NULL, SND_ASYNC); //������ ȿ���� ����(�浹 ����)
		sndPlaySound(TEXT("ȿ����\\hit_wall.wav"), SND_ASYNC); //���鿡 ƨ��� �Ҹ�
		
		if(move->x>=0.975 && move->y>=0.975){ //1��и��� �ܰ����� ��� ���
			move->x=0.975; move->y=0.975;
			pb.set_angle(_angle+180.0);}
		else if(move->x<=-0.975 && move->y>=0.975){ //4��и��� �ܰ����� ��� ���
			move->x=-0.975; move->y=0.975;
			pb.set_angle(_angle+180.0);}
		else if(move->x>=0.975){ //�������� ���ʸ鿡 �ε��� ���
			move->x=0.975;
			pb.set_angle(180-_angle);}
		else if(move->x<=-0.975){ //�������� �����ʸ鿡 �ε��� ���
			move->x=-0.975;
			pb.set_angle(180-_angle);}
		else if(move->y>=0.975){ //�������� ���鿡 �ε��� ���
			move->y=0.975;
			pb.set_angle(360-_angle);}
	}
	else if(move->y<=-0.975){ //�������� �Ʒ������� �������� ���
		sndPlaySound(NULL, SND_ASYNC); //������ ȿ���� ����(�浹 ����)
		sndPlaySound(TEXT("ȿ����\\fail.wav"), SND_ASYNC); //���н� �Ҹ�
		alive=false;
		pb.set_positon(0.5,-0.5);
		pb.set_angle(45.0);
	}
}
/*bar�� �浹�� �� (ball�� ��� bar�� ��ġ�� ���� ���� angle ����)*/
void bar_crash_Position(point move){
	static float quat_width;
	quat_width=pbar.get_width()/4.0;

	if(move.y<=-0.9 && move.x>=Cursor-quat_width*2 && move.x<=Cursor+quat_width*2){
		sndPlaySound(NULL, SND_ASYNC); //������ ȿ���� ����(�浹 ����)
		sndPlaySound(TEXT("ȿ����\\hit_bar.wav"), SND_ASYNC); //bar�� ƨ��� �Ҹ�
		if(move.x>=Cursor-quat_width*2 && move.x<Cursor-quat_width)
			pb.set_angle(135.0);
		else if(move.x>=Cursor-quat_width && move.x<Cursor)
			pb.set_angle(100.0);
		else if(move.x>=Cursor && move.x<Cursor+quat_width)
			pb.set_angle(80.0);
		else if(move.x>=Cursor+quat_width && move.x<=Cursor+quat_width*2)
			pb.set_angle(45.0);
	}
}
/*���簢�� ��Ͽ� �浹�� ��*/
square *square_crash(square *s, point *move, point past_move, point current_move){
	static GLfloat _angle;
	static GLfloat f_x, f_y, m; //�� Ÿ�̹� ������ ball��ǥ�� ���� �̵��� ball��ǥ�� �̾ ����� ������ � x=a Ȥ�� y=b�� ������ ���� ���� / m=����
	static GLfloat left, right, bottom, top; //square�� ��, ��, ��, ��
	static square *temp, *temp_next;

	_angle=pb.get_angle();
	m=(current_move.y-past_move.y)/(current_move.x-past_move.x); //�� Ÿ�̹� ������ ball��ǥ�� ���� �̵��� ball��ǥ�� �մ� ���� ����

	left=s->get_position().x-s->get_width()/2.0;
	right=s->get_position().x+s->get_width()/2.0;
	bottom=s->get_position().y-s->get_width()/2.0;
	top=s->get_position().y+s->get_width()/2.0;

	if(move->x>left && move->x<right && move->y>bottom && move->y<top){ //ball�� square�ȿ� ���� ���
		sndPlaySound(NULL, SND_ASYNC); //������ ȿ���� ����(�浹 ����)
		sndPlaySound(TEXT("ȿ����\\hit_block.wav"), SND_ASYNC); //block�� ƨ��� �Ҹ�
		if(_angle>0 && _angle<90){
			f_x=m*(left-past_move.x)+past_move.y; //f(x)
			f_y=(bottom-past_move.y)/m+past_move.x; //f(y)

			if(f_x>=bottom && f_x<=top){ //squre�� ���� ���鿡 ���� ���
				move->x=left-0.025;
				pb.set_angle(180.0-_angle);}
			else if(f_y>=left && f_y<=right){ //square�� �Ʒ� ���鿡 ���� ���
				move->y=bottom-0.025;
				pb.set_angle(360.0-_angle);}}
		else if(_angle>90 && _angle<180){
			f_x=m*(right-past_move.x)+past_move.y; //f(x)
			f_y=(bottom-past_move.y)/m+past_move.x; //f(y)

			if(f_x>=bottom && f_x<=top){ //squre�� ������ ���鿡 ���� ���
				move->x=right+0.025;
				pb.set_angle(180.0-_angle);}
			else if(f_y>=left && f_y<=right){ //square�� �Ʒ� ���鿡 ���� ���
				move->y=bottom-0.025;
				pb.set_angle(360.0-_angle);}}
		else if(_angle>180 && _angle<270){
			f_x=m*(right-past_move.x)+past_move.y; //f(x)
			f_y=(top-past_move.y)/m+past_move.x; //f(y)

			if(f_x>=bottom && f_x<=top){ //squre�� ������ ���鿡 ���� ���
				move->x=right+0.025;
				pb.set_angle(180.0-_angle);}
			else if(f_y>=left && f_y<=right){ //square�� �� ���鿡 ���� ���
				move->y=top+0.025;
				pb.set_angle(360.0-_angle);}}
		else if(_angle>270 && _angle<360){
			f_x=m*(left-past_move.x)+past_move.y; //f(x)
			f_y=(top-past_move.y)/m+past_move.x; //f(y)

			if(f_x>=bottom && f_x<=top){ //squre�� ���� ���鿡 ���� ���
				move->x=left-0.025;
				pb.set_angle(180.0-_angle);}
			else if(f_y>left && f_y<right){ //square�� �� ���鿡 ���� ���
				move->y=top+0.025;
				pb.set_angle(360.0-_angle);}}
	
		if(s!=s_head && s->get_next()!=NULL){ //temp�� ���� �� Ȥ�� ���� �ڰ� �ƴ� ��
			temp=s->get_next();
			//�����ϱ��� �� ���� �� ��带 �̾���
			s->get_prev()->set_next(s->get_next());
			s->get_next()->set_prev(s->get_prev());
			delete s;
			return temp;
		}
		else if(s==s_head){
			s_head=s->get_next();
			delete s; //�ش� ��� ����
			return s_head;
		}
		else if(s->get_next()==NULL){ //���� ������ ����� ���
			s->get_prev()->set_next(NULL); //������ ����� �� ����� next�κи� �����ϸ� ��
			delete s;
			return NULL;
		}
		else{ //���ܹ߻��� �����ϵ��� �Ͽ���.(���ݱ��� ���� ��� ���ܴ� �߻����� �ʾҴ�.)
			cout<<"���� �߻�!!"<<endl;
			exit(1);
		}
	}
	else //�浹���� ���� ���
		return s->get_next();
}
/*��� �׸���*/
void draw_square(square *s){
	glBegin(GL_POLYGON);
	glVertex2f(s->get_position().x+s->get_width()/2.0, s->get_position().y-s->get_width()/2.0);
	glVertex2f(s->get_position().x+s->get_width()/2.0, s->get_position().y+s->get_width()/2.0);
	glVertex2f(s->get_position().x-s->get_width()/2.0, s->get_position().y+s->get_width()/2.0);
	glVertex2f(s->get_position().x-s->get_width()/2.0, s->get_position().y-s->get_width()/2.0);
	glEnd();
}
/*���� ���� ���� - HEART*/
void create_heart(){
	int i; //�ݺ����� ���� ���� 

	/*�� ó�� ���*/
	square *temp = new square(0.0, -0.2);
	s_head=temp;
	temp->set_next(NULL);

	for(i=0;i<5;i++){
		square *temp = new square(0.1+0.1*i, -0.1+0.1*i);
		s_head->set_prev(temp);
		temp->set_next(s_head);
		s_head=temp;
	}
	for(i=0;i<4;i++){
		square *temp = new square(0.5, 0.4+0.1*i);
		s_head->set_prev(temp);
		temp->set_next(s_head);
		s_head=temp;
	}
	for(i=0;i<3;i++){
		square *temp = new square(0.4-0.1*i, 0.8);
		s_head->set_prev(temp);
		temp->set_next(s_head);
		s_head=temp;
	}
	for(i=0;i<2;i++){
		square *temp = new square(0.1-0.1*i, 0.7-0.1*i);
		s_head->set_prev(temp);
		temp->set_next(s_head);
		s_head=temp;
	}
	for(i=0;i<2;i++){
		square *temp = new square(-0.1-0.1*i, 0.7+0.1*i);
		s_head->set_prev(temp);
		temp->set_next(s_head);
		s_head=temp;
	}
	for(i=0;i<2;i++){
		square *temp = new square(-0.3-0.1*i, 0.8);
		s_head->set_prev(temp);
		temp->set_next(s_head);
		s_head=temp;
	}
	for(i=0;i<5;i++){
		square *temp = new square(-0.5, 0.7-0.1*i);
		s_head->set_prev(temp);
		temp->set_next(s_head);
		s_head=temp;
	}
	for(i=0;i<4;i++){
		square *temp = new square(-0.4+0.1*i, 0.2-0.1*i);
		s_head->set_prev(temp);
		temp->set_next(s_head);
		s_head=temp;
	}
}
/*���� ���� ���� - CG*/
void create_cg(){
	int i; //�ݺ����� ���� ���� 

	/*�� ó�� ���*/
	square *temp = new square(-0.1, 0.0);
	s_head=temp;
	temp->set_next(NULL);
	/*C*/
	for(i=0;i<4;i++){
		square *temp = new square(-0.2-0.1*i, -0.1);
		s_head->set_prev(temp);
		temp->set_next(s_head);
		s_head=temp;
	}
	for(i=0;i<2;i++){
		square *temp = new square(-0.6-0.1*i, 0.1*i);
		s_head->set_prev(temp);
		temp->set_next(s_head);
		s_head=temp;
	}
	for(i=0;i<4;i++){
		square *temp = new square(-0.7, 0.2+0.1*i);
		s_head->set_prev(temp);
		temp->set_next(s_head);
		s_head=temp;
	}
	for(i=0;i<2;i++){
		square *temp = new square(-0.6+0.1*i, 0.6+0.1*i);
		s_head->set_prev(temp);
		temp->set_next(s_head);
		s_head=temp;
	}
	for(i=0;i<3;i++){
		square *temp = new square(-0.4+0.1*i, 0.7);
		s_head->set_prev(temp);
		temp->set_next(s_head);
		s_head=temp;
	}
	for(i=0;i<1;i++){
		square *temp = new square(-0.1, 0.6);
		s_head->set_prev(temp);
		temp->set_next(s_head);
		s_head=temp;
	}
	/*G*/
	for(i=0;i<3;i++){
		square *temp = new square(0.7+0.1*i, 0.2);
		s_head->set_prev(temp);
		temp->set_next(s_head);
		s_head=temp;
	}
	for(i=0;i<3;i++){
		square *temp = new square(0.8-0.1*i, 0.1-0.1*i);
		s_head->set_prev(temp);
		temp->set_next(s_head);
		s_head=temp;
	}
	for(i=0;i<2;i++){
		square *temp = new square(0.5-0.1*i, -0.1);
		s_head->set_prev(temp);
		temp->set_next(s_head);
		s_head=temp;
	}
	for(i=0;i<2;i++){
		square *temp = new square(0.3-0.1*i, 0.1*i);
		s_head->set_prev(temp);
		temp->set_next(s_head);
		s_head=temp;
	}
	for(i=0;i<4;i++){
		square *temp = new square(0.2, 0.2+0.1*i);
		s_head->set_prev(temp);
		temp->set_next(s_head);
		s_head=temp;
	}
	for(i=0;i<2;i++){
		square *temp = new square(0.3+0.1*i, 0.6+0.1*i);
		s_head->set_prev(temp);
		temp->set_next(s_head);
		s_head=temp;
	}
	for(i=0;i<3;i++){
		square *temp = new square(0.5+0.1*i, 0.7);
		s_head->set_prev(temp);
		temp->set_next(s_head);
		s_head=temp;
	}
	for(i=0;i<1;i++){
		square *temp = new square(0.8, 0.6);
		s_head->set_prev(temp);
		temp->set_next(s_head);
		s_head=temp;
	}
}
/*���� ���� ���� - HOME*/
void create_home(){
	int i; //�ݺ����� ���� ���� 

	/*�� ó�� ���*/
	square *temp = new square(0.0, 0.8);
	s_head=temp;
	temp->set_next(NULL);
	/*���� ���� ��*/
	for(i=0;i<7;i++){
		square *temp = new square(-0.1-0.1*i, 0.8-0.1*i);
		s_head->set_prev(temp);
		temp->set_next(s_head);
		s_head=temp;
	}
	for(i=0;i<4;i++){
		square *temp = new square(-0.7+0.1*i, 0.1);
		s_head->set_prev(temp);
		temp->set_next(s_head);
		s_head=temp;
	}
	for(i=0;i<4;i++){
		square *temp = new square(-0.4, -0.1*i);
		s_head->set_prev(temp);
		temp->set_next(s_head);
		s_head=temp;
	}
	for(i=0;i<4;i++){
		square *temp = new square(-0.4+0.1*i, -0.4);
		s_head->set_prev(temp);
		temp->set_next(s_head);
		s_head=temp;
	}
	/*���� ������ ��*/
	for(i=0;i<7;i++){
		square *temp = new square(0.1+0.1*i, 0.8-0.1*i);
		s_head->set_prev(temp);
		temp->set_next(s_head);
		s_head=temp;
	}
	for(i=0;i<4;i++){
		square *temp = new square(0.7-0.1*i, 0.1);
		s_head->set_prev(temp);
		temp->set_next(s_head);
		s_head=temp;
	}
	for(i=0;i<4;i++){
		square *temp = new square(0.4, -0.1*i);
		s_head->set_prev(temp);
		temp->set_next(s_head);
		s_head=temp;
	}
	for(i=0;i<4;i++){
		square *temp = new square(0.4-0.1*i, -0.4);
		s_head->set_prev(temp);
		temp->set_next(s_head);
		s_head=temp;
	}
	/*�߰���*/
	for(i=0;i<1;i++){
		square *temp = new square(0.0, -0.4);
		s_head->set_prev(temp);
		temp->set_next(s_head);
		s_head=temp;
	}
}
/*���� ���� ���� - BOTTLE*/
void create_bottle(){
	int i; //�ݺ����� ���� ���� 

	/*�� ó�� ���*/
	square *temp = new square(0.0, 0.8);
	s_head=temp;
	temp->set_next(NULL);
	/*���� ���� ��*/
	for(i=0;i<3;i++){
		square *temp = new square(-0.1, 0.8-0.1*i);
		s_head->set_prev(temp);
		temp->set_next(s_head);
		s_head=temp;
	}
	for(i=0;i<3;i++){
		square *temp = new square(-0.2-0.1*i, 0.5-0.1*i);
		s_head->set_prev(temp);
		temp->set_next(s_head);
		s_head=temp;
	}
	for(i=0;i<6;i++){
		square *temp = new square(-0.4, 0.2-0.1*i);
		s_head->set_prev(temp);
		temp->set_next(s_head);
		s_head=temp;
	}
	for(i=0;i<3;i++){
		square *temp = new square(-0.3+0.1*i, -0.3);
		s_head->set_prev(temp);
		temp->set_next(s_head);
		s_head=temp;
	}
	/*���� ������ ��*/
	for(i=0;i<3;i++){
		square *temp = new square(0.1, 0.8-0.1*i);
		s_head->set_prev(temp);
		temp->set_next(s_head);
		s_head=temp;
	}
	for(i=0;i<3;i++){
		square *temp = new square(0.2+0.1*i, 0.5-0.1*i);
		s_head->set_prev(temp);
		temp->set_next(s_head);
		s_head=temp;
	}
	for(i=0;i<6;i++){
		square *temp = new square(0.4, 0.2-0.1*i);
		s_head->set_prev(temp);
		temp->set_next(s_head);
		s_head=temp;
	}
	for(i=0;i<3;i++){
		square *temp = new square(0.3-0.1*i, -0.3);
		s_head->set_prev(temp);
		temp->set_next(s_head);
		s_head=temp;
	}
	/*�߰���*/
	for(i=0;i<1;i++){
		square *temp = new square(0.0, -0.3);
		s_head->set_prev(temp);
		temp->set_next(s_head);
		s_head=temp;
	}
}
/*��ϸ� �׸���*/
void draw_blocks(){
	static square *temp;

	if(block_model==HEART)
		setColor(0.9, 0.2, 0.2, 1.0);
	else if(block_model==CG)
		setColor(0.9, 0.9, 0.9, 1.0);
	else if(block_model==HOME)
		setColor(1.0, 1.0, 0.0, 1.0);
	else if(block_model==BOTTLE)
		setColor(0.2, 0.9, 0.1, 1.0);

	temp=s_head;
	while(temp!=NULL){
		draw_square(temp);
		temp=temp->get_next();}
}
/*������ ���� ����*/
void remove_blocks(){
	square *temp;

	block_model=NONE;
	while(s_head!=NULL){
		temp=s_head->get_next();
		s_head=NULL; //�߰��غ�
		delete s_head;
		s_head=temp;
	}
}
/*complete ǥ��*/
void draw_complete(){
	static float time1=0.0, time2=0.5, time3=1.0, time4=1.5;

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
}
/*game over ǥ��*/
void draw_fail(){
	static float time1=0.0, time2=0.5, time3=1.0, time4=1.5;

	glPushMatrix();
	setColor(0.8, 0.7, 0.2, 1.0);
	glTranslatef(-0.5,0.05+0.1*sin(time1),0.0);
	glRotatef(45.0*sin(time1),1.0,1.0,0.0);
	time1+=0.005;
	draw_mesh(FAIL[0]);
	glPopMatrix();

	glPushMatrix();
	setColor(0.1, 0.5, 0.6, 1.0);
	glTranslatef(-0.1,0.05+0.1*sin(time2),0.0);
	glRotatef(45.0*sin(time2),0.0,1.0,1.0);
	time2+=0.006;
	draw_mesh(FAIL[1]);
	glPopMatrix();

	glPushMatrix();
	setColor(0.1, 0.5, 0.8, 1.0);
	glTranslatef(0.3+0.02*cos(time3),0.04*cos(time3),0.0);
	glRotatef(45.0*sin(time3),0.0,0.0,1.0);
	time3+=0.005;
	draw_mesh(FAIL[2]);
	glPopMatrix();

	glPushMatrix();
	setColor(0.4, 0.7, 0.1, 1.0);
	glTranslatef(0.65,-0.05,0.0);
	glRotatef(60.0*sin(time4),0.0,1.0,0.5);
	time4+=0.007;
	draw_mesh(FAIL[3]);
	glPopMatrix();
}
/*���콺 �������� ���� ��ǥ ����*/
void MyMouseMove(int X, int Y){
	static GLfloat half_WinWidth=(GLfloat)WinWidth/2.0; //������ �ʺ��� ����
	if(X>=half_WinWidth) //x>=0
		Cursor=((GLfloat)X-half_WinWidth)/half_WinWidth;
	else //X<half_WinWidth / x<0
		Cursor=-(half_WinWidth-(GLfloat)X)/half_WinWidth;
}
/*���÷���*/
void MyDisplay(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // �ߺ� �׷����� ����

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if(block_model!=NONE && s_head==NULL){ //��� ��� ����
		complete=true;
		if(alive){ //�ѹ��� ����ǵ��� ��
			sndPlaySound(NULL, SND_ASYNC); //������ ȿ���� ����(�浹 ����)
			sndPlaySound(TEXT("ȿ����\\complete.wav"), SND_ASYNC); //�ڼ� �Ҹ�
		}
		alive=false;
		//COMPLETE! ǥ��
		draw_complete();
	}
	else{ //����� ��������
		if(alive){
			//�� �׸���//
			glPushMatrix();
			glTranslatef(pb.get_position().x, pb.get_position().y, 0.0);
			setColor(0.9, 0.8, 0.8, 1.0);
			draw_mesh(Ball);
			glPopMatrix();
			
			//�� �׸���//
			glPushMatrix();
			glTranslatef(Cursor,-0.98,0);
			setColor(0.9, 0.8, 0.8, 1.0);
			draw_bar(Bar);
			glPopMatrix();

			//��� �׸���
			draw_blocks();
		}
		else if(!complete){ //alive==false && complete==false
			//GAME OVER ǥ��
			draw_fail();
		}
	}
	glFlush();
}
/*Ű����*/
void MyKeyboard(unsigned char KeyPressed,int x,int y){
	switch (KeyPressed){
		case 'q': case 'Q':
			exit(0);
			break;
		case 'v': case 'V': //�¸� Ʈ����(�¸� ������ �������Ѽ� �ߵ���Ŵ)
			remove_blocks();
			block_model=VICTORY;
			break;
		case '1':
			print_menu();
			cout<<"HEART block model�� �׸��ϴ�.\n\n";
			remove_blocks();
			create_heart();
			block_model=HEART;
			alive=true;
			complete=false;
			break;
		case '2':
			print_menu();
			cout<<"CG block model�� �׸��ϴ�.\n\n";
			remove_blocks();
			create_cg();
			block_model=CG;
			alive=true;
			complete=false;
			break;
		case '3':
			print_menu();
			cout<<"HOME block model�� �׸��ϴ�.\n\n";
			remove_blocks();
			create_home();
			block_model=HOME;
			alive=true;
			complete=false;
			break;
		case '4':
			print_menu();
			cout<<"BOTTLE block model�� �׸��ϴ�.\n\n";
			remove_blocks();
			create_bottle();
			block_model=BOTTLE;
			alive=true;
			complete=false;
			break;
		case '+':
			pb.set_speed(pb.get_speed()+0.001);
			cout<<"\n���ӵ� 0.001��ŭ ����\t���� ���ӵ�: "<<pb.get_speed();
			break;
		case '-':
			if(pb.get_speed()>0.001){
				pb.set_speed(pb.get_speed()-0.001);
				cout<<"\n���ӵ� 0.001��ŭ ����\t���� ���ӵ�: "<<pb.get_speed();
			}
			else
				cout<<"\n���� �ӵ��� ����� �����Ƿ� ���̻� ���� �� �����ϴ�.";
			break;
	}
	glutPostRedisplay();
}
/*Ÿ�̸� - ���� �����Ӱ� �浹������ ���Ͽ� �ۼ�*/
void MyTimer(int value){
	static point move= {pb.get_position().x, pb.get_position().y}; //ball�� x,y��ǥ ������
	static point past_move=move; //������ move.x, move.y ��
	static point current_move; //��ȭ�� ������ move.x, move.y ��
	static GLfloat _angle;
	static GLfloat ball_speed;
	static square *node;

	if(alive){ //alive ������ ���� �۵��ϵ���
		_angle=pb.get_angle();
		ball_speed=pb.get_speed();

		move.x += pb.get_speed()*cos(radian(pb.get_angle()));
		move.y += pb.get_speed()*sin(radian(pb.get_angle()));
		current_move.x=move.x;
		current_move.y=move.y;

		wall_crash(&move);
		bar_crash_Position(move);

		node=s_head;
		while(node)
			node=square_crash(node, &move, past_move, current_move);

		pb.set_positon(move.x, move.y);

		past_move=current_move;
	}
	glutPostRedisplay();
	glutTimerFunc(1,MyTimer,1);
}
/*����*/
int main(int argc, char** argv){
	print_menu();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("2010136019 �赿��_�������� ����");
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 0.0); 
	InitLight(-0.7,-0.5,0.1,3.5); //������ �ش�.

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);  // ������ ���� ����, �������κ��� mesh �� �� �ȼ��� ���� ���� ����Ͽ� ���
	glShadeModel(GL_SMOOTH);

	meshLoad();

	glutPassiveMotionFunc(MyMouseMove); //���콺�� �����ӿ� ���� ��ǥ ����
	glutKeyboardFunc(MyKeyboard); //Ű���� �ݹ�
	glutTimerFunc(1,MyTimer,1); //Ÿ�̸� �Լ�
	glutDisplayFunc(MyDisplay);
	glutMainLoop();

	delete Ball;
	delete Bar;
	delete[] FAIL;
	delete[] COMPLETE;

	return 0;
}
/*in LAB*/