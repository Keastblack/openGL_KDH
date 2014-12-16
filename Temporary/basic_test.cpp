///////////*알카노이드*///////////////
/*////////////의견///////////////////////////////////////////////////
(glutTimerFunc()의 밀리세컨드 단위를 줄일수록 프레임이 높아진다.
그런데 밀리세컨드의 단위를 한 자릿수로 하면 마우스 움직임에 프레임이
영향을 받는다.(마우스를 움직일수록 프레임 상승)
근데 두 자릿수부터는 그렇지 않다. 따라서 두 자릿수중 최소인 10으로
하였다.)
근데 이게 컴퓨터마다 다른 것 같다. 랩실 컴퓨터로는 1밀리세컨드 단위로
해도 위에 적은 부작용이 나타나지 않는다.
///////////////////////////////////////////////////////////////////*/

/*각 함수별로 static 변수가 선언되어 있다.*/
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
typedef struct point{
	GLfloat x;
	GLfloat y;
}point;
/*
ball 객체
*/
class ball{
	GLfloat speed; //1밀리초당 점의 이동거리 (스칼라 값)
	point position; //ball의 위치
	GLfloat angle;
public:
//	ball(){speed=0.04; position.x=0.0; position.y=0.0; angle=40.0;} //초당 0.0~1.0을 움직임 / (-1.0,0.0)에서 시작 / 원래 (0.0, -0.7)
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
/*
바(bar) 객체 - 마우스를 사용하여 좌우로 움직인다.
*/
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
/*
정사각형 방해물
*/
class square{
	point position;
	GLfloat width;
	bool exist;
public:
//	square();
	square(GLfloat _x=0.0, GLfloat _y=0.0, GLfloat _width=0.1, bool _exist=false){
		position.x=_x; position.y=_y; width=_width; exist=_exist;}
	void set_positon(point p){position.x=p.x; position.y=p.y;}		
	void set_positon(GLfloat _x,GLfloat _y){position.x=_x; position.y=_y;}		
	point get_position(){return position;}
	void set_width(GLfloat w){width=w;}
	GLfloat get_width(){return width;}
	void set_exist(bool logic){exist=logic;}
	bool get_exist(){return exist;}
};

int WinWidth = 800, WinHeight = 1600; //윈도우 크기
GLfloat Cursor = 0; //마우스 커서의 x좌표

ball pb; //playball
bar pbar; //playbar
square *s_head=NULL; //Heart 블록 모델을 그리는데 사용되는 연결리스트의 시작점
int block_num=0;
int block_remain_num=0;

square heart_blocks[29]; //heart 블록s

bool alive = true; //ball이 화면 아래로 떨어지면 false
bool complete = false; //모든 블록을 없애면 true

enum{
	HEART, CG, HOME, BOTTLE, NONE
}block_model=NONE;

void remove_blocks();
/*
메뉴 출력
*/
void print_menu(){
	system("cls");
	cout<<"<벽돌깨기 게임>\n\n";
	cout<<"마우스로 좌우 조작\n\n1 : 하트모양 블록\n2 : CG모양 블록\n3 : 집모양 블록\n4 : 병모양 블록\n\nQ : 종료\n\n";
}
/*
θ(세타)값을 radian값으로 바꾼다.
*/
GLfloat radian(GLfloat ang){
	return ang*PI/180.0;
}
/*
윈도우 벽에 충돌할 때
*/
void wall_crash(point *move){
	static GLfloat _angle;

	_angle=pb.get_angle();
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
	else if(move->y<=-0.975){ //윈도우의 아랫면에 부딛힌 경우
		if(complete==true){ //complete 상태라면 그냥 튕기게(안보이더라도)
			move->y=-0.975;
			pb.set_angle(360-_angle);}
		else{ //complete가 아니라면 게임 오버
			remove_blocks();
			block_model=NONE;
			alive=false;}}
}
/*
bar에 충돌할 때 (ball에 닿는 bar의 위치에 따라 볼의 angle 조정)
*/
void bar_crash_Position(point move){
	static float quat_width;
	quat_width=pbar.get_width()/4.0;

	if(move.y<=-0.9){
		if(move.x>=Cursor-quat_width*2 && move.x<Cursor-quat_width)
			pb.set_angle(135.0);
		else if(move.x>=Cursor-quat_width && move.x<Cursor)
			pb.set_angle(100.0);
		else if(move.x>=Cursor && move.x<Cursor+quat_width)
			pb.set_angle(80.0);
		else if(move.x>=Cursor+quat_width && move.x<=Cursor+quat_width*2)
			pb.set_angle(45.0);}

	//대부분 속도는 0.01~0.09 사이
}
/*
정사각형 블록에 충돌할 때
*/
void block_crash(square *node, point *move, point past_move, point current_move){
	static GLfloat _angle;
	static GLfloat f_x, f_y, m; //한 타이밍 이전의 ball좌표와 현재 이동할 ball좌표를 이어서 생기는 직선과 어떤 x=a 혹은 y=b와 만나는 점의 지점 / m=기울기
	static GLfloat left, right, bottom, top; //square의 좌, 우, 상, 하
	static square *temp;

	if(node->get_exist()==false)
		return;
	else{
		_angle=pb.get_angle();
		m=(current_move.y-past_move.y)/(current_move.x-past_move.x); //한 타이밍 이전의 ball좌표와 현재 이동할 ball좌표를 잇는 선의 기울기

		left=node->get_position().x-node->get_width()/2.0;
		right=node->get_position().x+node->get_width()/2.0;
		bottom=node->get_position().y-node->get_width()/2.0;
		top=node->get_position().y+node->get_width()/2.0;

		if(move->x>left && move->x<right && move->y>bottom && move->y<top){ //ball과 block이 충돌한 경우
			if(_angle>0 && _angle<90){
				f_x=m*(left-past_move.x)+past_move.y; //f(x)
				f_y=(bottom-past_move.y)/m+past_move.x; //f(y)

				if(f_x>=bottom && f_x<=top){ //squre의 왼쪽 벽면에 맞은 경우
					move->x=left-0.025;
					pb.set_angle(180.0-_angle);}
				else if(f_y>=left && f_y<=right){ //square의 아래 벽면에 맞은 경우
					move->y=bottom-0.025;
					pb.set_angle(360.0-_angle);}
			}
			else if(_angle>90 && _angle<180){
				f_x=m*(right-past_move.x)+past_move.y; //f(x)
				f_y=(bottom-past_move.y)/m+past_move.x; //f(y)

				if(f_x>=bottom && f_x<=top){ //squre의 오른쪽 벽면에 맞은 경우
					move->x=right+0.025;
					pb.set_angle(180.0-_angle);}
				else if(f_y>=left && f_y<=right){ //square의 아래 벽면에 맞은 경우
					move->y=bottom-0.025;
					pb.set_angle(360.0-_angle);}
			}
			else if(_angle>180 && _angle<270){
				f_x=m*(right-past_move.x)+past_move.y; //f(x)
				f_y=(top-past_move.y)/m+past_move.x; //f(y)

				if(f_x>=bottom && f_x<=top){ //squre의 오른쪽 벽면에 맞은 경우
					move->x=right+0.025;
					pb.set_angle(180.0-_angle);}
				else if(f_y>=left && f_y<=right){ //square의 위 벽면에 맞은 경우
					move->y=top+0.025;
					pb.set_angle(360.0-_angle);}
			}
			else if(_angle>270 && _angle<360){
				f_x=m*(left-past_move.x)+past_move.y; //f(x)
				f_y=(top-past_move.y)/m+past_move.x; //f(y)

				if(f_x>=bottom && f_x<=top){ //squre의 왼쪽 벽면에 맞은 경우
					move->x=left-0.025;
					pb.set_angle(180.0-_angle);
				}
				else if(f_y>left && f_y<right){ //square의 위 벽면에 맞은 경우
					move->y=top+0.025;
					pb.set_angle(360.0-_angle);}
			}
			node->set_exist(false);
			return;
		}
		else //충돌이 일어나지 않은 경우
			return;
	}
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
}
void draw_bar(){
	static int i; //bar를 그리는데 사용되는 인수
	static int j;
	j=(int)(pbar.get_width()/0.05);

	for(i=0; i<j; i++)
		draw_dot(Cursor+(-j/2+i)*0.05,-0.95);
}
/*
벽돌 그리기
*/
void draw_square(square *s){
	glBegin(GL_POLYGON);
	glVertex2f(s->get_position().x+s->get_width()/2.0, s->get_position().y+s->get_width()/2.0);
	glVertex2f(s->get_position().x+s->get_width()/2.0, s->get_position().y-s->get_width()/2.0);
	glVertex2f(s->get_position().x-s->get_width()/2.0, s->get_position().y-s->get_width()/2.0);
	glVertex2f(s->get_position().x-s->get_width()/2.0, s->get_position().y+s->get_width()/2.0);
	glEnd();
}
/*
맵의 벽돌 생성 - HEART
*/
void create_heart(){ //29개 블록
	int i, j=0; //반복문에 사용될 인자 

	for(i=0;i<6;i++,j++){
		heart_blocks[j].set_positon(0.1*i, -0.2+0.1*i);
		heart_blocks[j].set_exist(true);
	}
	for(i=0;i<5;i++,j++){
		heart_blocks[j].set_positon(0.5, 0.3+0.1*i);
		heart_blocks[j].set_exist(true);
	}
	for(i=0;i<3;i++,j++){
		heart_blocks[j].set_positon(0.4-0.1*i, 0.8);
		heart_blocks[j].set_exist(true);
	}
	for(i=0;i<2;i++,j++){
		heart_blocks[j].set_positon(0.1-0.1*i, 0.7-0.1*i);
		heart_blocks[j].set_exist(true);
	}
	for(i=0;i<2;i++,j++){
		heart_blocks[j].set_positon(-0.1-0.1*i, 0.7+0.1*i);
		heart_blocks[j].set_exist(true);
	}
	for(i=0;i<2;i++,j++){
		heart_blocks[j].set_positon(-0.3-0.1*i, 0.8);
		heart_blocks[j].set_exist(true);
	}
	for(i=0;i<5;i++,j++){
		heart_blocks[j].set_positon(-0.5, 0.7-0.1*i);
		heart_blocks[j].set_exist(true);
	}
	for(i=0;i<4;i++,j++){
		heart_blocks[j].set_positon(-0.4+0.1*i, 0.2-0.1*i);
		heart_blocks[j].set_exist(true);
	}
	s_head=heart_blocks;
	block_num=29;
	block_remain_num=29;
}
/*
블록모델 그리기
*/
void draw_blocks(){
	static int i;

	for(i=0;i<block_num;i++){
		if((s_head+i)->get_exist()) //exist==true이면 그리기
			draw_square(s_head+i);
	}
}
/*
생성된 벽돌 제거
*/
void remove_blocks(){
	block_model=NONE;
	for(int i=0;i<block_num;i++)
		(s_head+i)->set_exist(false);
	block_remain_num=0;
}
/*
complete 표기
*/
void draw_complete(){
	glutWireTeapot(0.3);
}
/*
game over 표기
*/
void draw_fail(){
	glutWireSphere(0.3,30,30);
}
/*
마우스 움직임을 토대로 좌표 생성
*/
void MyMouseMove(int X, int Y){
	static GLfloat half_WinWidth=(GLfloat)WinWidth/2.0; //윈도우 너비의 반절
	if(X>=half_WinWidth) //x>=0
		Cursor=((GLfloat)X-half_WinWidth)/half_WinWidth;
	else //X<half_WinWidth / x<0
		Cursor=-(half_WinWidth-(GLfloat)X)/half_WinWidth;
}
void MyDisplay(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 중복 그려지기 방지

	if(block_model!=NONE && block_remain_num==0){ //모든 블록 제거
		complete=true;
		//COMPLETE! 표기
		draw_complete();
	}
	else{ //블록이 남아있음
		if(alive){
			draw_ball();
			draw_bar();
			draw_blocks();
		}
		else{ //alive==false
			//GAME OVER 표기
			draw_fail();
		}
	}
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
		case '1':
			print_menu();
			cout<<"HEART block model을 그립니다.\n\n";
			remove_blocks();
			create_heart();
			block_model=HEART;
			alive=true;
			complete=false;
			break;
/*
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
*/
	}
}
void MyTimer(int value){
	static point move= {pb.get_position().x, pb.get_position().y}; //ball의 x,y좌표 움직임
	static point past_move=move; //기존의 move.x, move.y 값
	static point current_move; //변화된 현재의 move.x, move.y 값
	static GLfloat _angle;
	static GLfloat ball_speed;
	static int i;
//	static square *node;

	_angle=pb.get_angle();
	ball_speed=pb.get_speed();

	move.x += pb.get_speed()*cos(radian(pb.get_angle()));
	move.y += pb.get_speed()*sin(radian(pb.get_angle()));
	current_move.x=move.x;
	current_move.y=move.y;

	wall_crash(&move);
	bar_crash_Position(move);

	for(i=0;i<block_num;i++) //매 Timer 초마다 ball의 이동좌표와 block의 위치를 비교해서 충돌을 검사 / 매 Timer초마다 모든 블록을 검사
		block_crash(s_head+i, &move, past_move, current_move);
	
	pb.set_positon(move.x, move.y); //이부분을 어디로 옮기지? -> 가장 마지막줄로

	past_move=current_move;

	glutPostRedisplay();

	glutTimerFunc(1,MyTimer,1);
//	glutTimerFunc(10,MyTimer,1);
}
int main(int argc, char** argv){
	print_menu();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("벽돌깨기 게임");
	glClearColor(0.0, 0.0, 0.0, 0.0); 

	glutPassiveMotionFunc(MyMouseMove); //마우스의 움직임에 따라 좌표 생성
	glutKeyboardFunc(MyKeyboard); //키보드 콜백
	glutTimerFunc(1,MyTimer,1); //타이머 함수
//	glutTimerFunc(10,MyTimer,1); //타이머 함수
	glutDisplayFunc(MyDisplay);
	glutMainLoop();
	return 0;
}