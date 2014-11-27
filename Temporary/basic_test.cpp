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
	GLfloat speed; //10�и��ʴ� ���� �̵��Ÿ�
	point position;
	GLfloat angle;
public:
	ball(){speed=0.12; position.x=0.0; position.y=-0.7; angle=45.0;} //�ʴ� 0.0~1.0�� ������ / (-1.0,0.0)���� ����
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
	GLfloat width; //���� ����(��)
public:
	bar(){width=0.6;}
	void set_width(GLfloat w){width=w;}
	GLfloat get_width(){return width;}
};
int WinWidth = 800, WinHeight = 800; //������ ũ��
//int WinWidth = 1300, WinHeight = 1300; //������ ũ��
GLfloat Cursor = 0; //���콺 Ŀ���� x��ǥ
ball pb; //playball
GLfloat move_x = pb.get_position().x; //pb�� x��ǥ ������
GLfloat move_y = pb.get_position().y; //pb�� x��ǥ ������
/*
��(��Ÿ)���� radian������ �ٲ۴�.
*/
GLfloat radian(GLfloat ang){
	return ang*PI/180.0;
}
/*
������ ���� �浹�� ��
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
void draw_dot(GLfloat _x, GLfloat _y){
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
//	pb.set_positon(0.0, move);
	pb.set_positon(move_x, move_y);
	draw_dot(pb.get_position());
}
/*
bar �׸���
*/
void draw_bar(){
	draw_dot(Cursor,-0.95);
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