///////////*��ī���̵�*///////////////
/*////////////�ǰ�///////////////////////////////////////////////////
glutTimerFunc()�� �и������� ������ ���ϼ��� �������� ��������.
�׷��� �и��������� ������ �� �ڸ����� �ϸ� ���콺 �����ӿ� ��������
������ �޴´�.(���콺�� �����ϼ��� ������ ���)
�ٵ� �� �ڸ������ʹ� �׷��� �ʴ�. ���� �� �ڸ����� �ּ��� 10����
�Ͽ���.
///////////////////////////////////////////////////////////////////*/

#include <iostream>
#include <math.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
using namespace std;

#define PI 3.14159265

bool clock=true; //angle, position ���� ������ �� timer�� ��� ���� ���ؼ� ������ ����

/*
�� ����ü
*/
struct point{
	GLfloat x;
	GLfloat y;
};
/*
ball ��ü
*/
class ball{
	GLfloat speed; //10�и��ʴ� ���� �̵��Ÿ� (��Į�� ��)
	point position; //ball�� ��ġ
	GLfloat angle;
public:
	ball(){speed=0.04; position.x=-0.5; position.y=0.0; angle=310.0;} //�ʴ� 0.0~1.0�� ������ / (-1.0,0.0)���� ���� / ���� (0.0, -0.7)
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
���簢�� ���ع�
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

int WinWidth = 800, WinHeight = 800; //������ ũ��
GLfloat Cursor = 0; //���콺 Ŀ���� x��ǥ
//int sen=3; //sensitivity(�ΰ���)
GLfloat sen=0.010;
bool float_check = false; //ball�� �� �ȿ��� �����̴��� Ȯ��

ball pb; //playball
bar pbar; //playbar
bool limit_x=false; //�¿� ���鿡 �ε��� ���
bool limit_y=false; //���� ���鿡 �ε��� ���

/*
��(��Ÿ)���� radian������ �ٲ۴�.
*/
GLfloat radian(GLfloat ang){
	return ang*PI/180.0;
}
/*
������ ���� �浹�� ��
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

	/*�̵��ϴ� ���� Ż���� ���� ����*/
	if(move_x>=1.0){
		cout<<"Ż����!"<<endl;
		move_x=2.0-move_x;
		pb.set_angle(180-_angle);}
	else if(move_x<=-1.0){
		cout<<"Ż����!"<<endl;
		move_x=-2.0-move_x;
		pb.set_angle(180-_angle);}
	if(move_y>=1.0){
		cout<<"Ż����!"<<endl;
		move_y=2.0-move_y;
		pb.set_angle(360-_angle);}
	else if(move_y<=-1.0){
		cout<<"Ż����!"<<endl;
		move_y=-2.0-move_y;
		pb.set_angle(360-_angle);}
}
/*
bar�� �浹�� �� (bar�� �����̴� �ӵ��� ����Ͽ� ���� angle ����)
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
		cout<<"�ΰ��� : "<<sen<<endl;
		if(_speed==0){ //bar�� ������ �ִ� ���
			pb.set_angle(360-_angle);}
		else{ //bar�� �����̴� ���
			if(pb.get_angle()>180 && pb.get_angle()<=270){ //ball�� �����ʿ��� �������� �������� ���
				if(_speed<0) //bar�� �����ʿ��� �������� �����̴� ���
					pb.set_angle(360-_angle-(_angle-180.0)*abs(_speed)); //�ӵ��� ���������� �ݻ簢�� �۾���
				else //bar�� ���ʿ��� ���������� �����̴� ���
					pb.set_angle(_angle-180-(_angle-180.0)*abs(_speed));} //�ӵ��� ���������� �ݻ簢�� �۾���
			else if(pb.get_angle()>270 && pb.get_angle()<360){ //ball�� ���ʿ��� ���������� �������� ���
				if(_speed<0) //bar�� �����ʿ��� �������� �����̴� ���
					pb.set_angle(_angle-180+(360-_angle)*abs(_speed)); //�ӵ��� ���������� �ݻ簢�� �۾���
				else //bar�� ���ʿ��� ���������� �����̴� ���
					pb.set_angle(360-_angle-(360-_angle)*abs(_speed));}} //�ӵ��� ���������� �ݻ簢�� �۾���
	}
*/
	if((_x>=Cursor-half_width*0.05 && _x<=Cursor+half_width*0.05) && _y<=-0.9){
		cout<<"bar speed : "<<bar_speed<<endl;
		if(bar_speed==0){ //bar�� ������ �ִ� ���
			pb.set_speed(pb.get_speed()-0.005); //�ε��� ������ �ӵ� ����
			pb.set_angle(360-_angle);}
		else{ //bar�� �����̴� ���
			if(pb.get_angle()>180 && pb.get_angle()<=270){ //ball�� �����ʿ��� �������� �������� ���
				if(bar_speed<0) //bar�� �����ʿ��� �������� �����̴� ���
					pb.set_angle(360-_angle-2.5*(_angle-180.0)*abs(bar_speed)); //�ӵ��� ���������� �ݻ簢�� �۾���
				else //bar�� ���ʿ��� ���������� �����̴� ���
					pb.set_angle(_angle-180-2.5*(_angle-180.0)*abs(bar_speed));} //�ӵ��� ���������� �ݻ簢�� �۾���
			else if(pb.get_angle()>270 && pb.get_angle()<360){ //ball�� ���ʿ��� ���������� �������� ���
				if(bar_speed<0) //bar�� �����ʿ��� �������� �����̴� ���
					pb.set_angle(_angle-180+2.5*(360-_angle)*abs(bar_speed)); //�ӵ��� ���������� �ݻ簢�� �۾���
				else //bar�� ���ʿ��� ���������� �����̴� ���
					pb.set_angle(360-_angle-2.5*(360-_angle)*abs(bar_speed));}
		pb.set_speed(pb.get_speed()+sen*abs(bar_speed));} //�ӵ��� ���������� �ݻ簢�� �۾���
	}
	//��κ� �ӵ��� 0.01~0.09 ����
}
/*
bar�� �浹�� �� (ball�� ��� bar�� ��ġ�� ���� ���� angle ����)
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

	//��κ� �ӵ��� 0.01~0.09 ����
}
/*
���簢�� ��Ͽ� �浹�� ��
*/
void square_crash(square s, GLfloat move_x, GLfloat move_y){
	static GLfloat temp, _angle;

	_angle=pb.get_angle();

	//����� ������ move_x ������ �Ÿ��� �������
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

	//Ż���� setŸ�̹��� ���谡 ����.
}
/*
bar �׸���
*/
/*
void draw_bar(){
	static GLfloat i=0; //bar�� �׸��µ� ���Ǵ� �μ�
	i=Cursor-pbar.get_width()/2.0;
	while(i<Cursor+pbar.get_width()/2.0){
		draw_dot(i,-0.95);
		i+=0.05;} //Dot ���� ����
}
*/
void draw_bar(){
	static int i; //bar�� �׸��µ� ���Ǵ� �μ�
	static int j;
	j=(int)(pbar.get_width()/0.05);

	for(i=0; i<=j; i++)
		draw_dot(Cursor+(-j/2+i)*0.05,-0.95);
}
/*
���콺 �������� ���� ��ǥ ����
*/
void MyMouseMove(int X, int Y){
	GLfloat half_WinWidth=(GLfloat)WinWidth/2.0; //������ �ʺ��� ����
	if(X>=half_WinWidth) //x>=0
		Cursor=((GLfloat)X-half_WinWidth)/half_WinWidth;
	else //X<half_WinWidth / x<0
		Cursor=-(half_WinWidth-(GLfloat)X)/half_WinWidth;
}
void MyDisplay(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // �ߺ� �׷����� ����

	draw_ball();
	draw_bar();
	
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
		case '+':
			sen+=0.001;
			cout<<"�ΰ��� : "<<sen<<endl;
			break;
		case '-':
			sen-=0.001;
			cout<<"�ΰ��� : "<<sen<<endl;
			break;
	}
	glutPostRedisplay();
}
void MyTimer(int value){
	static GLfloat move_x=pb.get_position().x, move_y=pb.get_position().y; //ball�� x,y��ǥ ������
	static GLfloat past_cursor=0; //������ Ŀ�� ��
	static GLfloat current_cursor; //��ȭ�� ������ Ŀ�� ��
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
	pb.set_positon(move_x, move_y); //�̺κ��� ���� �ű���? -> ���� �������ٷ�

	glutPostRedisplay();
	glutTimerFunc(10,MyTimer,1);
}
int main(int argc, char** argv){
	cout<<"�ΰ��� : "<<sen<<endl;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("��ī���̵�");
	glClearColor(0.0, 0.0, 0.0, 0.0); 

	glutPassiveMotionFunc(MyMouseMove); //���콺�� �����ӿ� ���� ��ǥ ����
	glutKeyboardFunc(MyKeyboard); //Ű���� �ݹ�
	glutTimerFunc(10,MyTimer,1); //Ÿ�̸� �Լ�
	glutDisplayFunc(MyDisplay);
	glutMainLoop();
	return 0;
}

/*�ݻ簢 ����*/
/******************
if(angle==(0 or 90 or 180 or 270 or 360) then next_angle=angle+180
	if(next_angle>=360) then next_angle-=360
elif(0<angle<180) then next_angle=180-angle
elif(180<angle<360) then next_angle=540-angle
******************/

/*�ӵ� ���� ����///////
if(ball_speed-sen>0){
				pb.set_speed(ball_speed-sen);
				cout<<pb.get_speed()<<endl;}//�ε��� ������ �ӵ� ����
			else
				pb.set_speed(0.0);}
*///////////////////////