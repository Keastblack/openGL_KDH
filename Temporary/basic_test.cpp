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

bool clock=true; //angle, position 등을 변경할 때 timer를 잠깐 끄기 위해서 생성된 변수

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
	GLfloat speed; //10밀리초당 점의 이동거리 (스칼라 값)
	point position; //ball의 위치
	GLfloat angle;
public:
	ball(){speed=0.04; position.x=-0.5; position.y=0.0; angle=310.0;} //초당 0.0~1.0을 움직임 / (-1.0,0.0)에서 시작 / 원래 (0.0, -0.7)
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
	point position; //bar의 중점 위치
	GLfloat width; //바의 길이(폭)
	GLfloat speed; //10밀리초당 바의 이동거리 (벡터 값)
public:
	bar(){width=0.4;}
	void set_positon(GLfloat _x,GLfloat _y){
		clock=false;
		position.x=_x; position.y=_y;
		clock=true;}
	point get_position(){return position;}
	void set_width(GLfloat w){
		clock=false;
		width=w;
		clock=true;}
	GLfloat get_width(){return width;}
	void set_speed(GLfloat s){
		clock=false;
		speed=s;
		clock=true;}
	GLfloat get_speed(){return speed;}
};
/*
정사각형 방해물
*/
class square{
	point position;
	GLfloat width;
public:
	square(GLfloat _x, GLfloat _y, GLfloat _width=0.1){
		position.x=_x; position.y=_y; width=_width;}
	void set_positon(GLfloat _x,GLfloat _y){position.x=_x; position.y=_y;}		
	point get_position(){return position;}
};

int WinWidth = 800, WinHeight = 800; //윈도우 크기
GLfloat Cursor = 0; //마우스 커서의 x좌표
//int sen=3; //sensitivity(민감도)
GLfloat sen=0.010;
bool float_check = false; //ball이 벽 안에서 움직이는지 확인

ball pb; //playball
bar pbar; //playbar
bool limit_x=false; //좌우 벽면에 부딛힌 경우
bool limit_y=false; //상하 벽면에 부딛힌 경우

/*
θ(세타)값을 radian값으로 바꾼다.
*/
GLfloat radian(GLfloat ang){
	return ang*PI/180.0;
}
/*
윈도우 벽에 충돌할 때
*/
void wall_crash(GLfloat move_x, GLfloat move_y){
	static GLfloat _angle;

	_angle=pb.get_angle();

	if(move_x>=0.975){
		move_x=0.975;
		pb.set_angle(180-_angle);}
	else if(move_x<=-0.975){
		move_x=-0.975;
		pb.set_angle(180-_angle);}

	if(move_y>=0.975){
		move_y=0.975;
		pb.set_angle(360-_angle);}
	else if(move_y<=-0.975){
		move_y=-0.975;
		pb.set_angle(360-_angle);}

	/*이동하는 점의 탈선을 막기 위함*/
	if(move_x>=1.0){
		cout<<"탈선함!"<<endl;
		move_x=2.0-move_x;
		pb.set_angle(180-_angle);}
	else if(move_x<=-1.0){
		cout<<"탈선함!"<<endl;
		move_x=-2.0-move_x;
		pb.set_angle(180-_angle);}
	if(move_y>=1.0){
		cout<<"탈선함!"<<endl;
		move_y=2.0-move_y;
		pb.set_angle(360-_angle);}
	else if(move_y<=-1.0){
		cout<<"탈선함!"<<endl;
		move_y=-2.0-move_y;
		pb.set_angle(360-_angle);}
}
/*
bar에 충돌할 때 (bar의 움직이는 속도에 비례하여 볼의 angle 조정)
*/
void bar_crash_Speed(){
	static float _x,_y, _angle, half_width, bar_speed;
	half_width=(int)(pbar.get_width()/0.05)/2;
	_x=pb.get_position().x;
	_y=pb.get_position().y;
	_angle=pb.get_angle();
	bar_speed=pbar.get_speed();
/*
	if((_x>=Cursor-half_width*0.05 && _x<=Cursor+half_width*0.05) && _y<=-0.9){
		cout<<"민감도 : "<<sen<<endl;
		if(_speed==0){ //bar가 가만히 있는 경우
			pb.set_angle(360-_angle);}
		else{ //bar가 움직이는 경우
			if(pb.get_angle()>180 && pb.get_angle()<=270){ //ball이 오른쪽에서 왼쪽으로 내려오는 경우
				if(_speed<0) //bar가 오른쪽에서 왼쪽으로 움직이는 경우
					pb.set_angle(360-_angle-(_angle-180.0)*abs(_speed)); //속도가 빨라질수록 반사각이 작아짐
				else //bar가 왼쪽에서 오른쪽으로 움직이는 경우
					pb.set_angle(_angle-180-(_angle-180.0)*abs(_speed));} //속도가 빨라질수록 반사각이 작아짐
			else if(pb.get_angle()>270 && pb.get_angle()<360){ //ball이 왼쪽에서 오른쪽으로 내려오는 경우
				if(_speed<0) //bar가 오른쪽에서 왼쪽으로 움직이는 경우
					pb.set_angle(_angle-180+(360-_angle)*abs(_speed)); //속도가 빨라질수록 반사각이 작아짐
				else //bar가 왼쪽에서 오른쪽으로 움직이는 경우
					pb.set_angle(360-_angle-(360-_angle)*abs(_speed));}} //속도가 빨라질수록 반사각이 작아짐
	}
*/
	if((_x>=Cursor-half_width*0.05 && _x<=Cursor+half_width*0.05) && _y<=-0.9){
		cout<<"bar speed : "<<bar_speed<<endl;
		if(bar_speed==0){ //bar가 가만히 있는 경우
			pb.set_speed(pb.get_speed()-0.005); //부딛힐 때마다 속도 감소
			pb.set_angle(360-_angle);}
		else{ //bar가 움직이는 경우
			if(pb.get_angle()>180 && pb.get_angle()<=270){ //ball이 오른쪽에서 왼쪽으로 내려오는 경우
				if(bar_speed<0) //bar가 오른쪽에서 왼쪽으로 움직이는 경우
					pb.set_angle(360-_angle-2.5*(_angle-180.0)*abs(bar_speed)); //속도가 빨라질수록 반사각이 작아짐
				else //bar가 왼쪽에서 오른쪽으로 움직이는 경우
					pb.set_angle(_angle-180-2.5*(_angle-180.0)*abs(bar_speed));} //속도가 빨라질수록 반사각이 작아짐
			else if(pb.get_angle()>270 && pb.get_angle()<360){ //ball이 왼쪽에서 오른쪽으로 내려오는 경우
				if(bar_speed<0) //bar가 오른쪽에서 왼쪽으로 움직이는 경우
					pb.set_angle(_angle-180+2.5*(360-_angle)*abs(bar_speed)); //속도가 빨라질수록 반사각이 작아짐
				else //bar가 왼쪽에서 오른쪽으로 움직이는 경우
					pb.set_angle(360-_angle-2.5*(360-_angle)*abs(bar_speed));}
		pb.set_speed(pb.get_speed()+sen*abs(bar_speed));} //속도가 빨라질수록 반사각이 작아짐
	}
	//대부분 속도는 0.01~0.09 사이
}
/*
bar에 충돌할 때 (ball에 닿는 bar의 위치에 따라 볼의 angle 조정)
*/
void bar_crash_Position(GLfloat move_x, GLfloat move_y){
	static float quat_width;
	quat_width=pbar.get_width()/4.0;

	if(move_y<=-0.9){
		if(move_x>=Cursor-quat_width*2 && move_x<Cursor-quat_width)
			pb.set_angle(135.0);
		else if(move_x>=Cursor-quat_width && move_x<Cursor)
			pb.set_angle(100.0);
		else if(move_x>=Cursor && move_x<Cursor+quat_width)
			pb.set_angle(80.0);
		else if(move_x>=Cursor+quat_width && move_x<=Cursor+quat_width*2)
			pb.set_angle(45.0);}

	//대부분 속도는 0.01~0.09 사이
}
/*
정사각형 블록에 충돌할 때
*/
void square_crash(square s, GLfloat move_x, GLfloat move_y){
	static GLfloat temp, _angle;

	_angle=pb.get_angle();

	//블록의 중점과 move_x 사이의 거리로 계산하자
	if(move_x>=0.975){
	//	temp=0.975;
		move_x=0.975;
		pb.set_angle(180-_angle);}
	else if(temp<=-0.975){
	//	temp=-0.975;
		move_x=-0.975;
		pb.set_angle(180-_angle);}

	temp = move_y;
	if(temp>=0.975){
	//	temp=0.975;
		move_y=0.975;
		pb.set_angle(360-_angle);}
	else if(temp<=-0.975){
	//	temp=-0.975;
		move_y=-0.975;
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
void draw_dot(GLfloat _x,GLfloat _y){
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
	draw_dot(pb.get_position());

	//탈선과 set타이밍은 관계가 없다.
}
/*
bar 그리기
*/
/*
void draw_bar(){
	static GLfloat i=0; //bar를 그리는데 사용되는 인수
	i=Cursor-pbar.get_width()/2.0;
	while(i<Cursor+pbar.get_width()/2.0){
		draw_dot(i,-0.95);
		i+=0.05;} //Dot 변의 길이
}
*/
void draw_bar(){
	static int i; //bar를 그리는데 사용되는 인수
	static int j;
	j=(int)(pbar.get_width()/0.05);

	for(i=0; i<=j; i++)
		draw_dot(Cursor+(-j/2+i)*0.05,-0.95);
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
	draw_bar();
	
	glFlush();
}
void MyKeyboard(unsigned char KeyPressed,int x,int y){
	switch (KeyPressed){
		case 'q': case 'Q':
			exit(0);
			break;
		case 'r': //ball 초기화
			pb.set_angle(80.0);
		//	pb.set_speed(0.06);
			break;
		case '+':
			sen+=0.001;
			cout<<"민감도 : "<<sen<<endl;
			break;
		case '-':
			sen-=0.001;
			cout<<"민감도 : "<<sen<<endl;
			break;
	}
	glutPostRedisplay();
}
void MyTimer(int value){
	static GLfloat move_x=pb.get_position().x, move_y=pb.get_position().y; //ball의 x,y좌표 움직임
	static GLfloat past_cursor=0; //기존의 커서 값
	static GLfloat current_cursor; //변화된 현재의 커서 값
	static GLfloat temp, _angle;
	static GLfloat ball_speed;

	_angle=pb.get_angle();
	ball_speed=pb.get_speed();

	current_cursor=Cursor;
	pbar.set_speed(current_cursor-past_cursor);
	past_cursor=current_cursor;
//	cout<<"speed : "<<pbar.get_speed()<<endl;

	move_x += pb.get_speed()*cos(radian(pb.get_angle()));
	move_y += pb.get_speed()*sin(radian(pb.get_angle()));

	wall_crash(move_x, move_y);
	bar_crash_Position(move_x, move_y);


//	move_x += sen*cos(radian(pb.get_angle()));
//	move_y += sen*sin(radian(pb.get_angle()));
	pb.set_positon(move_x, move_y); //이부분을 어디로 옮기지? -> 가장 마지막줄로

	glutPostRedisplay();
	glutTimerFunc(10,MyTimer,1);
}
int main(int argc, char** argv){
	cout<<"민감도 : "<<sen<<endl;

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

/*속도 감쇄 관련///////
if(ball_speed-sen>0){
				pb.set_speed(ball_speed-sen);
				cout<<pb.get_speed()<<endl;}//부딛힐 때마다 속도 감소
			else
				pb.set_speed(0.0);}
*///////////////////////