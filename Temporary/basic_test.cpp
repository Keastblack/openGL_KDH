///////////*��ī���̵�*///////////////
/*////////////�ǰ�///////////////////////////////////////////////////
(glutTimerFunc()�� �и������� ������ ���ϼ��� �������� ��������.
�׷��� �и��������� ������ �� �ڸ����� �ϸ� ���콺 �����ӿ� ��������
������ �޴´�.(���콺�� �����ϼ��� ������ ���)
�ٵ� �� �ڸ������ʹ� �׷��� �ʴ�. ���� �� �ڸ����� �ּ��� 10����
�Ͽ���.)
�ٵ� �̰� ��ǻ�͸��� �ٸ� �� ����. ���� ��ǻ�ͷδ� 1�и������� ������
�ص� ���� ���� ���ۿ��� ��Ÿ���� �ʴ´�.
///////////////////////////////////////////////////////////////////*/

/*�� �Լ����� static ������ ����Ǿ� �ִ�.*/
#include <iostream>
#include <math.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
using namespace std;

#define PI 3.14159265

/*
�� ����ü
*/
typedef struct point{
	GLfloat x;
	GLfloat y;
}point;
/*
ball ��ü
*/
class ball{
	GLfloat speed; //1�и��ʴ� ���� �̵��Ÿ� (��Į�� ��)
	point position; //ball�� ��ġ
	GLfloat angle;
public:
//	ball(){speed=0.04; position.x=0.0; position.y=0.0; angle=40.0;} //�ʴ� 0.0~1.0�� ������ / (-1.0,0.0)���� ���� / ���� (0.0, -0.7)
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
/*
��(bar) ��ü - ���콺�� ����Ͽ� �¿�� �����δ�.
*/
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
/*
���簢�� ���ع�
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

int WinWidth = 800, WinHeight = 1600; //������ ũ��
GLfloat Cursor = 0; //���콺 Ŀ���� x��ǥ

ball pb; //playball
bar pbar; //playbar
square *s_head=NULL; //Heart ��� ���� �׸��µ� ���Ǵ� ���Ḯ��Ʈ�� ������
int block_num=0;
int block_remain_num=0;

square heart_blocks[29]; //heart ���s

bool alive = true; //ball�� ȭ�� �Ʒ��� �������� false
bool complete = false; //��� ����� ���ָ� true

enum{
	HEART, CG, HOME, BOTTLE, NONE
}block_model=NONE;

void remove_blocks();
/*
�޴� ���
*/
void print_menu(){
	system("cls");
	cout<<"<�������� ����>\n\n";
	cout<<"���콺�� �¿� ����\n\n1 : ��Ʈ��� ���\n2 : CG��� ���\n3 : ����� ���\n4 : ����� ���\n\nQ : ����\n\n";
}
/*
��(��Ÿ)���� radian������ �ٲ۴�.
*/
GLfloat radian(GLfloat ang){
	return ang*PI/180.0;
}
/*
������ ���� �浹�� ��
*/
void wall_crash(point *move){
	static GLfloat _angle;

	_angle=pb.get_angle();
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
	else if(move->y<=-0.975){ //�������� �Ʒ��鿡 �ε��� ���
		if(complete==true){ //complete ���¶�� �׳� ƨ���(�Ⱥ��̴���)
			move->y=-0.975;
			pb.set_angle(360-_angle);}
		else{ //complete�� �ƴ϶�� ���� ����
			remove_blocks();
			block_model=NONE;
			alive=false;}}
}
/*
bar�� �浹�� �� (ball�� ��� bar�� ��ġ�� ���� ���� angle ����)
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

	//��κ� �ӵ��� 0.01~0.09 ����
}
/*
���簢�� ��Ͽ� �浹�� ��
*/
void block_crash(square *node, point *move, point past_move, point current_move){
	static GLfloat _angle;
	static GLfloat f_x, f_y, m; //�� Ÿ�̹� ������ ball��ǥ�� ���� �̵��� ball��ǥ�� �̾ ����� ������ � x=a Ȥ�� y=b�� ������ ���� ���� / m=����
	static GLfloat left, right, bottom, top; //square�� ��, ��, ��, ��
	static square *temp;

	if(node->get_exist()==false)
		return;
	else{
		_angle=pb.get_angle();
		m=(current_move.y-past_move.y)/(current_move.x-past_move.x); //�� Ÿ�̹� ������ ball��ǥ�� ���� �̵��� ball��ǥ�� �մ� ���� ����

		left=node->get_position().x-node->get_width()/2.0;
		right=node->get_position().x+node->get_width()/2.0;
		bottom=node->get_position().y-node->get_width()/2.0;
		top=node->get_position().y+node->get_width()/2.0;

		if(move->x>left && move->x<right && move->y>bottom && move->y<top){ //ball�� block�� �浹�� ���
			if(_angle>0 && _angle<90){
				f_x=m*(left-past_move.x)+past_move.y; //f(x)
				f_y=(bottom-past_move.y)/m+past_move.x; //f(y)

				if(f_x>=bottom && f_x<=top){ //squre�� ���� ���鿡 ���� ���
					move->x=left-0.025;
					pb.set_angle(180.0-_angle);}
				else if(f_y>=left && f_y<=right){ //square�� �Ʒ� ���鿡 ���� ���
					move->y=bottom-0.025;
					pb.set_angle(360.0-_angle);}
			}
			else if(_angle>90 && _angle<180){
				f_x=m*(right-past_move.x)+past_move.y; //f(x)
				f_y=(bottom-past_move.y)/m+past_move.x; //f(y)

				if(f_x>=bottom && f_x<=top){ //squre�� ������ ���鿡 ���� ���
					move->x=right+0.025;
					pb.set_angle(180.0-_angle);}
				else if(f_y>=left && f_y<=right){ //square�� �Ʒ� ���鿡 ���� ���
					move->y=bottom-0.025;
					pb.set_angle(360.0-_angle);}
			}
			else if(_angle>180 && _angle<270){
				f_x=m*(right-past_move.x)+past_move.y; //f(x)
				f_y=(top-past_move.y)/m+past_move.x; //f(y)

				if(f_x>=bottom && f_x<=top){ //squre�� ������ ���鿡 ���� ���
					move->x=right+0.025;
					pb.set_angle(180.0-_angle);}
				else if(f_y>=left && f_y<=right){ //square�� �� ���鿡 ���� ���
					move->y=top+0.025;
					pb.set_angle(360.0-_angle);}
			}
			else if(_angle>270 && _angle<360){
				f_x=m*(left-past_move.x)+past_move.y; //f(x)
				f_y=(top-past_move.y)/m+past_move.x; //f(y)

				if(f_x>=bottom && f_x<=top){ //squre�� ���� ���鿡 ���� ���
					move->x=left-0.025;
					pb.set_angle(180.0-_angle);
				}
				else if(f_y>left && f_y<right){ //square�� �� ���鿡 ���� ���
					move->y=top+0.025;
					pb.set_angle(360.0-_angle);}
			}
			node->set_exist(false);
			return;
		}
		else //�浹�� �Ͼ�� ���� ���
			return;
	}
}
/*
Dot �׸���
*/
void draw_dot(point p){
	static GLfloat d=0.025; //��Ʈ ���簢���� ���� ���� ����
	glBegin(GL_POLYGON);
	glVertex2f(p.x+d,p.y+d);
	glVertex2f(p.x+d,p.y-d);
	glVertex2f(p.x-d,p.y-d);
	glVertex2f(p.x-d,p.y+d);
	glEnd();
}
void draw_dot(GLfloat _x,GLfloat _y){
	static GLfloat d=0.025; //��Ʈ ���簢���� ���� ���� ����
	glBegin(GL_POLYGON);
	glVertex2f(_x+d,_y+d);
	glVertex2f(_x+d,_y-d);
	glVertex2f(_x-d,_y-d);
	glVertex2f(_x-d,_y+d);
	glEnd();
}
/*
ball �׸���
*/
void draw_ball(){
	draw_dot(pb.get_position());
}
void draw_bar(){
	static int i; //bar�� �׸��µ� ���Ǵ� �μ�
	static int j;
	j=(int)(pbar.get_width()/0.05);

	for(i=0; i<j; i++)
		draw_dot(Cursor+(-j/2+i)*0.05,-0.95);
}
/*
���� �׸���
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
���� ���� ���� - HEART
*/
void create_heart(){ //29�� ���
	int i, j=0; //�ݺ����� ���� ���� 

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
��ϸ� �׸���
*/
void draw_blocks(){
	static int i;

	for(i=0;i<block_num;i++){
		if((s_head+i)->get_exist()) //exist==true�̸� �׸���
			draw_square(s_head+i);
	}
}
/*
������ ���� ����
*/
void remove_blocks(){
	block_model=NONE;
	for(int i=0;i<block_num;i++)
		(s_head+i)->set_exist(false);
	block_remain_num=0;
}
/*
complete ǥ��
*/
void draw_complete(){
	glutWireTeapot(0.3);
}
/*
game over ǥ��
*/
void draw_fail(){
	glutWireSphere(0.3,30,30);
}
/*
���콺 �������� ���� ��ǥ ����
*/
void MyMouseMove(int X, int Y){
	static GLfloat half_WinWidth=(GLfloat)WinWidth/2.0; //������ �ʺ��� ����
	if(X>=half_WinWidth) //x>=0
		Cursor=((GLfloat)X-half_WinWidth)/half_WinWidth;
	else //X<half_WinWidth / x<0
		Cursor=-(half_WinWidth-(GLfloat)X)/half_WinWidth;
}
void MyDisplay(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // �ߺ� �׷����� ����

	if(block_model!=NONE && block_remain_num==0){ //��� ��� ����
		complete=true;
		//COMPLETE! ǥ��
		draw_complete();
	}
	else{ //����� ��������
		if(alive){
			draw_ball();
			draw_bar();
			draw_blocks();
		}
		else{ //alive==false
			//GAME OVER ǥ��
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
		case 'r': //ball �ʱ�ȭ
			pb.set_angle(80.0);
		//	pb.set_speed(0.06);
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
/*
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
*/
	}
}
void MyTimer(int value){
	static point move= {pb.get_position().x, pb.get_position().y}; //ball�� x,y��ǥ ������
	static point past_move=move; //������ move.x, move.y ��
	static point current_move; //��ȭ�� ������ move.x, move.y ��
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

	for(i=0;i<block_num;i++) //�� Timer �ʸ��� ball�� �̵���ǥ�� block�� ��ġ�� ���ؼ� �浹�� �˻� / �� Timer�ʸ��� ��� ����� �˻�
		block_crash(s_head+i, &move, past_move, current_move);
	
	pb.set_positon(move.x, move.y); //�̺κ��� ���� �ű���? -> ���� �������ٷ�

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
	glutCreateWindow("�������� ����");
	glClearColor(0.0, 0.0, 0.0, 0.0); 

	glutPassiveMotionFunc(MyMouseMove); //���콺�� �����ӿ� ���� ��ǥ ����
	glutKeyboardFunc(MyKeyboard); //Ű���� �ݹ�
	glutTimerFunc(1,MyTimer,1); //Ÿ�̸� �Լ�
//	glutTimerFunc(10,MyTimer,1); //Ÿ�̸� �Լ�
	glutDisplayFunc(MyDisplay);
	glutMainLoop();
	return 0;
}