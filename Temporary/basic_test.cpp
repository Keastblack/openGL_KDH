///////////*알카노이드*///////////////
/*////////////의견///////////////////////////////////////////////////
glutTimerFunc()의 밀리세컨드 단위를 줄일수록 프레임이 높아진다.
그런데 밀리세컨드의 단위를 한 자릿수로 하면 마우스 움직임에 프레임이
영향을 받는다.(마우스를 움직일수록 프레임 상승)
근데 두 자릿수부터는 그렇지 않다. 따라서 두 자릿수중 최소인 10으로
하였다.
///////////////////////////////////////////////////////////////////*/

#include <iostream>
#include <math.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
using namespace std;

#define PI 3.14159265

/*
점 구조체
*/
struct point{
	GLfloat x;
	GLfloat y;
};
/*
ball 객체
*/
class ball{
	GLfloat speed; //10밀리초당 점의 이동거리
	point position;
	GLfloat angle;
public:
	ball(){speed=0.12; position.x=0.0; position.y=-0.7; angle=45.0;} //초당 0.0~1.0을 움직임 / (-1.0,0.0)에서 시작
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
/*
바(bar) 객체 - 마우스를 사용하여 좌우로 움직인다.
*/
class bar{
	GLfloat width; //바의 길이(폭)
public:
	bar(){width=0.6;}
	void set_width(GLfloat w){width=w;}
	GLfloat get_width(){return width;}
};
int WinWidth = 800, WinHeight = 800; //윈도우 크기
//int WinWidth = 1300, WinHeight = 1300; //윈도우 크기
GLfloat Cursor = 0; //마우스 커서의 x좌표
ball pb; //playball
GLfloat move_x = pb.get_position().x; //pb의 x좌표 움직임
GLfloat move_y = pb.get_position().y; //pb의 x좌표 움직임
/*
θ(세타)값을 radian값으로 바꾼다.
*/
GLfloat radian(GLfloat ang){
	return ang*PI/180.0;
}
/*
윈도우 벽에 충돌할 때
*/
void wall_crash(){
	static float _x,_y, _angle;
	_x=pb.get_position().x;
	_y=pb.get_position().y;
	_angle=pb.get_angle();

	if(_x>=0.975 || _x<=-0.975){
		if(_angle==90 || _angle==270)
			pb.set_angle(_angle+180);
		else
			pb.set_angle(180-_angle);}
	else if(_y>=0.975 || _y<=-0.975){
		if(_angle==0 || _angle==180)
			pb.set_angle(_angle+180);
		else // 180<_angle<360
			pb.set_angle(360-_angle);}
}
/*
Dot 그리기
*/
void draw_dot(point p){
	static GLfloat d=0.025; //도트 정사각형의 변의 반절 길이
	glBegin(GL_POLYGON);
	glVertex2f(p.x+d,p.y+d);
	glVertex2f(p.x+d,p.y-d);
	glVertex2f(p.x-d,p.y-d);
	glVertex2f(p.x-d,p.y+d);
	glEnd();
}
void draw_dot(GLfloat _x, GLfloat _y){
	static GLfloat d=0.025; //도트 정사각형의 변의 반절 길이
	glBegin(GL_POLYGON);
	glVertex2f(_x+d,_y+d);
	glVertex2f(_x+d,_y-d);
	glVertex2f(_x-d,_y-d);
	glVertex2f(_x-d,_y+d);
	glEnd();
}
/*
ball 그리기
*/
void draw_ball(){
//	pb.set_positon(0.0, move);
	pb.set_positon(move_x, move_y);
	draw_dot(pb.get_position());
}
/*
bar 그리기
*/
void draw_bar(){
	draw_dot(Cursor,-0.95);
}
/*
마우스 움직임을 토대로 좌표 생성
*/
void MyMouseMove(int X, int Y){
	GLfloat half_WinWidth=(GLfloat)WinWidth/2.0; //윈도우 너비의 반절
	if(X>=half_WinWidth) //x>=0
		Cursor=((GLfloat)X-half_WinWidth)/half_WinWidth;
	else //X<half_WinWidth / x<0
		Cursor=-(half_WinWidth-(GLfloat)X)/half_WinWidth;
}
void MyDisplay(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 중복 그려지기 방지

	draw_ball();
	wall_crash();
	draw_bar();
	
	glFlush();
}
void MyKeyboard(unsigned char KeyPressed,int x,int y){
	switch (KeyPressed){
		case 'q': case 'Q':
			exit(0);
			break;
	}
	glutPostRedisplay();
}
void MyTimer(int value){
	move_x += pb.get_speed()*cos(radian(pb.get_angle()));
	move_y += pb.get_speed()*sin(radian(pb.get_angle()));

	glutPostRedisplay();
	glutTimerFunc(10,MyTimer,1);
}
int main(int argc, char** argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("알카노이드");
	glClearColor(0.0, 0.0, 0.0, 0.0); 

	glutPassiveMotionFunc(MyMouseMove); //마우스의 움직임에 따라 좌표 생성
	glutKeyboardFunc(MyKeyboard); //키보드 콜백
	glutTimerFunc(10,MyTimer,1); //타이머 함수
	glutDisplayFunc(MyDisplay);
	glutMainLoop();
	return 0;
}

/*반사각 원리*/
/******************
if(angle==(0 or 90 or 180 or 270 or 360) then next_angle=angle+180
	if(next_angle>=360) then next_angle-=360
elif(0<angle<180) then next_angle=180-angle
elif(180<angle<360) then next_angle=540-angle
******************/