/////////////////////*2010136019 김동현 *//////////////////////////////
///////////컴퓨터 그래픽스 텀프로젝트-벽돌깨기 게임////////////////////

/*****컨트롤 키*****/
/********************
1 : 하트모양 블록
2 : CG모양 블록
3 : 집모양 블록
4 : 병모양 블록

+ : 공의 속도를 0.001만큼 증가
- : 공의 속도를 0.001만큼 감소

Q : 종료
V : 자동승리(시연용)
********************/

/*각 함수별로 static 변수가 선언되어 있다.*/
#include <iostream>
#include <math.h>
#include<windows.h>
#include<MMSystem.h> //음원 재생(sndPlaySound())
#include "ase_mod.h"
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

using namespace std;

#define PI 3.14159265

/*점 구조체*/
typedef struct point{
	GLfloat x;
	GLfloat y;
}point;
/*ball 객체*/
class ball{
	GLfloat speed; //1밀리초당 점의 이동거리 (스칼라 값)
	point position; //ball의 위치
	GLfloat angle;
public:
	ball(){speed=0.004; position.x=0.0; position.y=0.0; angle=40.0;} //초당 0.0~1.0을 움직임 / (-1.0,0.0)에서 시작 / 원래 (0.0, -0.7)
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
/*바(bar) 객체 - 마우스를 사용하여 좌우로 움직인다.*/
class bar{
	point position; //bar의 중점 위치
	GLfloat width; //바의 길이(폭)
	GLfloat speed; //10밀리초당 바의 이동거리 (벡터 값)
public:
	bar(){width=0.4;}
	void set_positon(GLfloat _x,GLfloat _y){position.x=_x; position.y=_y;}
	point get_position(){return position;}
	void set_width(GLfloat w){width=w;}
	GLfloat get_width(){return width;}
	void set_speed(GLfloat s){speed=s;}
	GLfloat get_speed(){return speed;}
};
/*정사각형 방해물(block)*/
class square{
	point position;
	GLfloat width;
	square *prev;
	square *next; //이후 리스트
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

int WinWidth = 800, WinHeight = 1600; //윈도우 크기
GLfloat Cursor = 0; //마우스 커서의 x좌표

ball pb; //playball
bar pbar; //playbar
square *s_head=NULL; //Heart 블록 모델을 그리는데 사용되는 연결리스트의 시작점

bool alive = true; //ball이 화면 아래로 떨어지면 false
bool complete = false; //모든 블록을 없애면 true

Mesh *Bar; //Bar 메시
Mesh *Ball; //Bar 메시
Mesh *FAIL[4]; //FAIL 메시
Mesh *COMPLETE[8]; //COMPLETE 메시

square temp_square;

enum{
	HEART, CG, HOME, BOTTLE, VICTORY, NONE
}block_model=NONE;

void MyTimer(int);
/*
R,G,B와 a로 색조정
*/
void setColor(float r, float g, float b, float a){
	static GLfloat mat_ambient[6]; //물체의 주변광 계수
	static GLfloat mat_diffuse[6]; //물체의 확산광 계수

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
void InitLight(GLfloat x=-0.2, GLfloat y=-0.5, GLfloat z=0.1, GLfloat n=0.5){ //x,y,z,n의 매개변수 전달받음
	// mesh 의 재질 색, 광원 값
	GLfloat mat_specular[ ] = {0.5, 1.0, 0.5, 1.0};
	GLfloat mat_shininess[ ] = {70.0};
	// 조명의 색, 광원 값
	GLfloat light_specular[ ] = {0.8, 0.5, 0.8, 1.0};
	GLfloat light_diffuse[ ] = {0.7, 0.7, 0.7, 1.0};
	GLfloat light_ambient[ ] = {0.4, 0.4, 0.4, 1.0};
	GLfloat light_position[ ] = {x, y, z, n};
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);  // 조명 사용 설정
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE); // 정규화
	/* mesh 의 재질 색, 광원  및  조명의 색, 광원 설정*/
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
}
/*
메시 로딩
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
/*bar를 ASE파일로부터 import해서 그림*/
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
	for(int i=0; i<m->faceNum; i++){ //실수로 bar그림을 반절만 그려서, x축 대칭으로 두 번 그리게 하였다.
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
/*ASE파일로부터 import해서 그림*/
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
/*메뉴 출력*/
void print_menu(){
	system("cls");
	cout<<"<벽돌깨기 게임>\n\n";
	cout<<"마우스로 좌우 조작\n\n1 : 하트모양 블록\n2 : CG모양 블록\n3 : 집모양 블록\n4 : 병모양 블록\n\n+ : 공의 속도 증가\n- : 공의 속도 감소\n\nQ : 종료\n\n";
}
/*θ(세타)값을 radian값으로 바꾼다.*/
GLfloat radian(GLfloat ang){
	return ang*PI/180.0;
}
/*윈도우 벽에 충돌할 때*/
void wall_crash(point *move){
	static GLfloat _angle;

	_angle=pb.get_angle();
	if(move->x>=0.975 || move->x<=-0.975 || move->y>=0.975){ // 윈도우 벽면에 부딛히는 경우
		sndPlaySound(NULL, SND_ASYNC); //이전의 효과음 제거(충돌 방지)
		sndPlaySound(TEXT("효과음\\hit_wall.wav"), SND_ASYNC); //벽면에 튕기는 소리
		
		if(move->x>=0.975 && move->y>=0.975){ //1사분면의 외곽으로 벗어난 경우
			move->x=0.975; move->y=0.975;
			pb.set_angle(_angle+180.0);}
		else if(move->x<=-0.975 && move->y>=0.975){ //4사분면의 외곽으로 벗어난 경우
			move->x=-0.975; move->y=0.975;
			pb.set_angle(_angle+180.0);}
		else if(move->x>=0.975){ //윈도우의 왼쪽면에 부딛힌 경우
			move->x=0.975;
			pb.set_angle(180-_angle);}
		else if(move->x<=-0.975){ //윈도우의 오른쪽면에 부딛힌 경우
			move->x=-0.975;
			pb.set_angle(180-_angle);}
		else if(move->y>=0.975){ //윈도우의 윗면에 부딛힌 경우
			move->y=0.975;
			pb.set_angle(360-_angle);}
	}
	else if(move->y<=-0.975){ //윈도우의 아랫면으로 떨어지는 경우
		sndPlaySound(NULL, SND_ASYNC); //이전의 효과음 제거(충돌 방지)
		sndPlaySound(TEXT("효과음\\fail.wav"), SND_ASYNC); //실패시 소리
		alive=false;
		pb.set_positon(0.5,-0.5);
		pb.set_angle(45.0);
	}
}
/*bar에 충돌할 때 (ball에 닿는 bar의 위치에 따라 볼의 angle 조정)*/
void bar_crash_Position(point move){
	static float quat_width;
	quat_width=pbar.get_width()/4.0;

	if(move.y<=-0.9 && move.x>=Cursor-quat_width*2 && move.x<=Cursor+quat_width*2){
		sndPlaySound(NULL, SND_ASYNC); //이전의 효과음 제거(충돌 방지)
		sndPlaySound(TEXT("효과음\\hit_bar.wav"), SND_ASYNC); //bar에 튕기는 소리
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
/*정사각형 블록에 충돌할 때*/
square *square_crash(square *s, point *move, point past_move, point current_move){
	static GLfloat _angle;
	static GLfloat f_x, f_y, m; //한 타이밍 이전의 ball좌표와 현재 이동할 ball좌표를 이어서 생기는 직선과 어떤 x=a 혹은 y=b와 만나는 점의 지점 / m=기울기
	static GLfloat left, right, bottom, top; //square의 좌, 우, 상, 하
	static square *temp, *temp_next;

	_angle=pb.get_angle();
	m=(current_move.y-past_move.y)/(current_move.x-past_move.x); //한 타이밍 이전의 ball좌표와 현재 이동할 ball좌표를 잇는 선의 기울기

	left=s->get_position().x-s->get_width()/2.0;
	right=s->get_position().x+s->get_width()/2.0;
	bottom=s->get_position().y-s->get_width()/2.0;
	top=s->get_position().y+s->get_width()/2.0;

	if(move->x>left && move->x<right && move->y>bottom && move->y<top){ //ball이 square안에 들어온 경우
		sndPlaySound(NULL, SND_ASYNC); //이전의 효과음 제거(충돌 방지)
		sndPlaySound(TEXT("효과음\\hit_block.wav"), SND_ASYNC); //block에 튕기는 소리
		if(_angle>0 && _angle<90){
			f_x=m*(left-past_move.x)+past_move.y; //f(x)
			f_y=(bottom-past_move.y)/m+past_move.x; //f(y)

			if(f_x>=bottom && f_x<=top){ //squre의 왼쪽 벽면에 맞은 경우
				move->x=left-0.025;
				pb.set_angle(180.0-_angle);}
			else if(f_y>=left && f_y<=right){ //square의 아래 벽면에 맞은 경우
				move->y=bottom-0.025;
				pb.set_angle(360.0-_angle);}}
		else if(_angle>90 && _angle<180){
			f_x=m*(right-past_move.x)+past_move.y; //f(x)
			f_y=(bottom-past_move.y)/m+past_move.x; //f(y)

			if(f_x>=bottom && f_x<=top){ //squre의 오른쪽 벽면에 맞은 경우
				move->x=right+0.025;
				pb.set_angle(180.0-_angle);}
			else if(f_y>=left && f_y<=right){ //square의 아래 벽면에 맞은 경우
				move->y=bottom-0.025;
				pb.set_angle(360.0-_angle);}}
		else if(_angle>180 && _angle<270){
			f_x=m*(right-past_move.x)+past_move.y; //f(x)
			f_y=(top-past_move.y)/m+past_move.x; //f(y)

			if(f_x>=bottom && f_x<=top){ //squre의 오른쪽 벽면에 맞은 경우
				move->x=right+0.025;
				pb.set_angle(180.0-_angle);}
			else if(f_y>=left && f_y<=right){ //square의 위 벽면에 맞은 경우
				move->y=top+0.025;
				pb.set_angle(360.0-_angle);}}
		else if(_angle>270 && _angle<360){
			f_x=m*(left-past_move.x)+past_move.y; //f(x)
			f_y=(top-past_move.y)/m+past_move.x; //f(y)

			if(f_x>=bottom && f_x<=top){ //squre의 왼쪽 벽면에 맞은 경우
				move->x=left-0.025;
				pb.set_angle(180.0-_angle);}
			else if(f_y>left && f_y<right){ //square의 위 벽면에 맞은 경우
				move->y=top+0.025;
				pb.set_angle(360.0-_angle);}}
	
		if(s!=s_head && s->get_next()!=NULL){ //temp가 가장 앞 혹은 가장 뒤가 아닐 때
			temp=s->get_next();
			//삭제하기전 앞 노드와 뒤 노드를 이어줌
			s->get_prev()->set_next(s->get_next());
			s->get_next()->set_prev(s->get_prev());
			delete s;
			return temp;
		}
		else if(s==s_head){
			s_head=s->get_next();
			delete s; //해당 블록 삭제
			return s_head;
		}
		else if(s->get_next()==NULL){ //가장 마지막 노드인 경우
			s->get_prev()->set_next(NULL); //삭제될 노드의 앞 노드의 next부분만 수정하면 됨
			delete s;
			return NULL;
		}
		else{ //예외발생시 종료하도록 하였다.(지금까지 시행 결과 예외는 발생하지 않았다.)
			cout<<"예외 발생!!"<<endl;
			exit(1);
		}
	}
	else //충돌하지 않은 경우
		return s->get_next();
}
/*블록 그리기*/
void draw_square(square *s){
	glBegin(GL_POLYGON);
	glVertex2f(s->get_position().x+s->get_width()/2.0, s->get_position().y-s->get_width()/2.0);
	glVertex2f(s->get_position().x+s->get_width()/2.0, s->get_position().y+s->get_width()/2.0);
	glVertex2f(s->get_position().x-s->get_width()/2.0, s->get_position().y+s->get_width()/2.0);
	glVertex2f(s->get_position().x-s->get_width()/2.0, s->get_position().y-s->get_width()/2.0);
	glEnd();
}
/*맵의 벽돌 생성 - HEART*/
void create_heart(){
	int i; //반복문에 사용될 인자 

	/*맨 처음 블록*/
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
/*맵의 벽돌 생성 - CG*/
void create_cg(){
	int i; //반복문에 사용될 인자 

	/*맨 처음 블록*/
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
/*맵의 벽돌 생성 - HOME*/
void create_home(){
	int i; //반복문에 사용될 인자 

	/*맨 처음 블록*/
	square *temp = new square(0.0, 0.8);
	s_head=temp;
	temp->set_next(NULL);
	/*집의 왼쪽 면*/
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
	/*집의 오른쪽 면*/
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
	/*중간점*/
	for(i=0;i<1;i++){
		square *temp = new square(0.0, -0.4);
		s_head->set_prev(temp);
		temp->set_next(s_head);
		s_head=temp;
	}
}
/*맵의 벽돌 생성 - BOTTLE*/
void create_bottle(){
	int i; //반복문에 사용될 인자 

	/*맨 처음 블록*/
	square *temp = new square(0.0, 0.8);
	s_head=temp;
	temp->set_next(NULL);
	/*병의 왼쪽 면*/
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
	/*병의 오른쪽 면*/
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
	/*중간점*/
	for(i=0;i<1;i++){
		square *temp = new square(0.0, -0.3);
		s_head->set_prev(temp);
		temp->set_next(s_head);
		s_head=temp;
	}
}
/*블록모델 그리기*/
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
/*생성된 벽돌 제거*/
void remove_blocks(){
	square *temp;

	block_model=NONE;
	while(s_head!=NULL){
		temp=s_head->get_next();
		s_head=NULL; //추가해봄
		delete s_head;
		s_head=temp;
	}
}
/*complete 표기*/
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
/*game over 표기*/
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
/*마우스 움직임을 토대로 좌표 생성*/
void MyMouseMove(int X, int Y){
	static GLfloat half_WinWidth=(GLfloat)WinWidth/2.0; //윈도우 너비의 반절
	if(X>=half_WinWidth) //x>=0
		Cursor=((GLfloat)X-half_WinWidth)/half_WinWidth;
	else //X<half_WinWidth / x<0
		Cursor=-(half_WinWidth-(GLfloat)X)/half_WinWidth;
}
/*디스플레이*/
void MyDisplay(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 중복 그려지기 방지

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if(block_model!=NONE && s_head==NULL){ //모든 블록 제거
		complete=true;
		if(alive){ //한번만 실행되도록 함
			sndPlaySound(NULL, SND_ASYNC); //이전의 효과음 제거(충돌 방지)
			sndPlaySound(TEXT("효과음\\complete.wav"), SND_ASYNC); //박수 소리
		}
		alive=false;
		//COMPLETE! 표기
		draw_complete();
	}
	else{ //블록이 남아있음
		if(alive){
			//볼 그리기//
			glPushMatrix();
			glTranslatef(pb.get_position().x, pb.get_position().y, 0.0);
			setColor(0.9, 0.8, 0.8, 1.0);
			draw_mesh(Ball);
			glPopMatrix();
			
			//바 그리기//
			glPushMatrix();
			glTranslatef(Cursor,-0.98,0);
			setColor(0.9, 0.8, 0.8, 1.0);
			draw_bar(Bar);
			glPopMatrix();

			//블록 그리기
			draw_blocks();
		}
		else if(!complete){ //alive==false && complete==false
			//GAME OVER 표기
			draw_fail();
		}
	}
	glFlush();
}
/*키보드*/
void MyKeyboard(unsigned char KeyPressed,int x,int y){
	switch (KeyPressed){
		case 'q': case 'Q':
			exit(0);
			break;
		case 'v': case 'V': //승리 트리거(승리 조건을 만족시켜서 발동시킴)
			remove_blocks();
			block_model=VICTORY;
			break;
		case '1':
			print_menu();
			cout<<"HEART block model을 그립니다.\n\n";
			remove_blocks();
			create_heart();
			block_model=HEART;
			alive=true;
			complete=false;
			break;
		case '2':
			print_menu();
			cout<<"CG block model을 그립니다.\n\n";
			remove_blocks();
			create_cg();
			block_model=CG;
			alive=true;
			complete=false;
			break;
		case '3':
			print_menu();
			cout<<"HOME block model을 그립니다.\n\n";
			remove_blocks();
			create_home();
			block_model=HOME;
			alive=true;
			complete=false;
			break;
		case '4':
			print_menu();
			cout<<"BOTTLE block model을 그립니다.\n\n";
			remove_blocks();
			create_bottle();
			block_model=BOTTLE;
			alive=true;
			complete=false;
			break;
		case '+':
			pb.set_speed(pb.get_speed()+0.001);
			cout<<"\n공속도 0.001만큼 증가\t현재 공속도: "<<pb.get_speed();
			break;
		case '-':
			if(pb.get_speed()>0.001){
				pb.set_speed(pb.get_speed()-0.001);
				cout<<"\n공속도 0.001만큼 감소\t현재 공속도: "<<pb.get_speed();
			}
			else
				cout<<"\n공의 속도가 충분히 느리므로 더이상 줄일 수 없습니다.";
			break;
	}
	glutPostRedisplay();
}
/*타이머 - 공의 움직임과 충돌판정에 관하여 작성*/
void MyTimer(int value){
	static point move= {pb.get_position().x, pb.get_position().y}; //ball의 x,y좌표 움직임
	static point past_move=move; //기존의 move.x, move.y 값
	static point current_move; //변화된 현재의 move.x, move.y 값
	static GLfloat _angle;
	static GLfloat ball_speed;
	static square *node;

	if(alive){ //alive 상태일 때만 작동하도록
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
/*메인*/
int main(int argc, char** argv){
	print_menu();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("2010136019 김동현_벽돌깨기 게임");
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 0.0); 
	InitLight(-0.7,-0.5,0.1,3.5); //음영을 준다.

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);  // 숨겨진 면을 제거, 시점으로부터 mesh 의 각 픽셀의 깊이 값을 계산하여 출력
	glShadeModel(GL_SMOOTH);

	meshLoad();

	glutPassiveMotionFunc(MyMouseMove); //마우스의 움직임에 따라 좌표 생성
	glutKeyboardFunc(MyKeyboard); //키보드 콜백
	glutTimerFunc(1,MyTimer,1); //타이머 함수
	glutDisplayFunc(MyDisplay);
	glutMainLoop();

	delete Ball;
	delete Bar;
	delete[] FAIL;
	delete[] COMPLETE;

	return 0;
}
/*in LAB*/