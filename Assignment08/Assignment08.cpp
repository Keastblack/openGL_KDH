/*******************************
2010136019 �赿�� Assignment08
*******************************/
/*

���콺 Ŭ���� �� �� �Ҷ����� �� �� ���� �մ� ������ �׷����ϴ�.
������â�� �ø��� ���׸��Ⱑ ����Ȯ�����Ƿ� �������� ũ�� ������ ���Ѵٸ�
�� �ڵ� ���� WinWidth, WinHeight ���� �����ϴ� ���� �����ϴ�.
*/
/*
���� ��ư : Q
*/

#include <iostream>
#include <stdlib.h>
#include <math.h> //abs()���
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <string.h>
using namespace std;

int WinWidth = 800, WinHeight = 800; //������ ũ��
GLfloat ViewX = 0, ViewY = 0; //���콺�� Ŭ���Ͽ��� ���� x,y��ǥ
bool MouseClick = false; //���콺 Ŭ���� �Ǳ� ������ false
/*
���콺�� Ŭ���Ͽ��� �� ��ǥ�� glutWindow �������� ����Ѵ�.
*/
void MyMouseMove(int botton, int state, GLint X, GLint Y){	//���콺 �����ӿ� ���� �Լ�
    if (botton == GLUT_LEFT_BUTTON && state == GLUT_DOWN){	//���콺 ���� ��ư�� ������ ��
		MouseClick = true; //���콺 Ŭ���� ���۵��� ����.
		GLfloat half_WinWidth=(GLfloat)WinWidth/2.0, half_WinHeight=(GLfloat)WinHeight/2.0; //������ ũ���� ����
		if(X>=half_WinWidth && Y>=half_WinHeight/2){ //(X,Y)�� ��1��и鿡 �ִ� ���
				ViewX=((GLfloat)X-half_WinWidth)/half_WinWidth;
				ViewY=(half_WinHeight-(GLfloat)Y)/half_WinHeight;}
		else if(X>=half_WinWidth && Y<WinHeight/2){ //(X,Y)�� ��2��и鿡 �ִ� ���
				ViewX=((GLfloat)X-half_WinWidth)/half_WinWidth;
				ViewY=-((GLfloat)Y-half_WinHeight)/half_WinHeight;}
		else if(X<half_WinWidth && Y<WinHeight/2){ //(X,Y)�� ��3��и鿡 �ִ� ���
				ViewX=-(half_WinWidth-(GLfloat)X)/half_WinWidth;
				ViewY=-((GLfloat)Y-half_WinHeight)/half_WinHeight;}
		else{ //(X,Y)�� ��4��и鿡 �ִ� ���
				ViewX=-(half_WinWidth-(GLfloat)X)/half_WinWidth;
				ViewY=(half_WinHeight-(GLfloat)Y)/half_WinHeight;}
		cout<<"ViewX : "<<ViewX<<"\tViewY : "<<ViewY<<endl;
		glutPostRedisplay();
    }
}
/*
�־��� ���� �������� ���簢�� ��Ʈ�� ��´�.
*/
void DrawDot(GLfloat x, GLfloat y){
	GLfloat d=0.0125; //���簢�� ��Ʈ�� �� ���� ���� ����
	glBegin(GL_POLYGON);
		glVertex2f(x+d,y+d);
		glVertex2f(x+d,y-d);
		glVertex2f(x-d,y-d);
		glVertex2f(x-d,y+d);
	glEnd();
}
/*
�극��� �˰��� ����
�� �� ������ ������ �׸���.
*/
void bres_line(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2){
	GLfloat dx, dy;
	GLfloat p_value; //������ ��Ʈ ���� �ȼ��� ����
	GLfloat inc_minus; //���Ⱑ ���� �� p_value ����
	GLfloat inc_plus; //���Ⱑ ���� �� p_value ����
	GLfloat inc_value=0.025; //ȭ�Ҵ���(��, �� ���α׷������� ��Ʈ�� �Ÿ��̹Ƿ� ���簢�� ��Ʈ�� ������ �� ũ��� �Ѵ�.
	GLfloat temp; //�ӽ����庯��

	dx = abs(x2-x1); //x�� ��ȭ��
	dy = abs(y2-y1); //y�� ��ȭ��

	if(dx >= dy){ //y���� x�� ��ȭ���� Ŭ ��� ��, ����
		inc_minus=2*dy;
		inc_plus=2*(dy-dx); //���� dx-dy

		if(x2<x1){ //(x1,y1)�� ����, (x2,y2)�� ������ �д�.
			temp = x1;
			x1 = x2;
			x2 = temp;

			temp = y1;
			y1 = y2;
			y2 = temp;}
	
		if(y1 >= y2) //������ �Ʒ��� ��������(����<0) ����
			inc_value *= -1.0;

		//DrawDot(x1,y1); //������ǥ(������ ��ǥ) ���
		p_value = 2*dy-dx; //���� �ȼ��� ����

		for(GLfloat seek_x=x1; seek_x<x2; seek_x+=abs(inc_value)){ //x��ǥ�� x2�� �̸������� ��ǥ�� ��� ��´�.
			if(p_value<0){ //���Ⱑ ���Ƽ� �Ʒ��� �ȼ��� ������
				p_value += inc_minus;}
			//y1���� �״��
			else { //���Ⱑ ���Ƽ� ���� �ȼ��� ������
				p_value += inc_plus; 
				y1 += inc_value;} //y1->y2�������� ������ŭ �̵�
			DrawDot(seek_x,y1);} //�˰��� ���� ��ǥ�� ��´�.
	}
	else{ //x���� y�� ��ȭ���� Ŭ ��� ��, ��
		inc_minus= 2.0*dx;
		inc_plus= 2.0*(dx-dy);

		if(y2 < y1){ //(x1,y1)�� �Ʒ���, (x2,y2)�� ������ �д�.
			temp = y1;
			y1 = y2;
			y2 = temp;

			temp = x1;
			x1 = x2;
			x2 = temp;}

		if(x1>=x2)
			inc_value *= -1.0;

		//DrawDot(x1,y1); //������ǥ(�Ʒ����� ��ǥ) ���
		p_value = 2*dx - dy; //���� �ȼ��� ����

		for(GLfloat seek_y=y1; seek_y < y2; seek_y+=abs(inc_value)){
			if(p_value<0){ //���Ⱑ ���Ƽ� �Ʒ��� �ȼ��� ������
				p_value += inc_minus;}
			//x1���� �״��
			else{
				p_value += inc_plus;
				x1 += inc_value;} //x1->x2�������� ������ŭ �̵�             
			DrawDot(x1,seek_y);} //�˰��� ���� ��ǥ�� ��´�.
	}
}
void MyDisplay(){
	static int i=0;//�迭����, ��ǥ���� �� ���� �޵��� �Ѵ�.
	static GLfloat tempX[2], tempY[2];
	if(MouseClick){ //���콺Ŭ���� Ȱ��ȭ �Ǿ��� ��
		tempX[i]=ViewX; tempY[i]=ViewY; //��ǥ�� �Է¹޴´�. ù ��° ��ǥ�� tempX[0],tempY[0]��, �� ��° ��ǥ�� tempX[1],tempY[1]�� �Է��Ѵ�.
		DrawDot(tempX[i],tempY[i]); //������ǥ�� ����ǥ�� ��´�. ����� ���� �ߺ��ǵ� �������.
		glFlush();
		i++;
		if((tempX[0]==tempX[1]) && (tempY[0]==tempY[1])) //�ߺ��� ��ǥ
			i=1; //�ٽ� �Է¹޴´�.
		if(i==2){ //�� ��ǥ�� �Է¹޾��� ��
			bres_line(tempX[0],tempY[0],tempX[1],tempY[1]);
			i=0; //�ٽ� ù ��° ��ǥ���� �Է¹޴´�.
		}
		MouseClick=false;
	}
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

int main(int argc, char** argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(WinWidth, WinHeight);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("2010136019_Assignment08");
	//glClearColor(0.0, 0.0, 0.0, 0.0); //�� ���� �����Ƿ� �ּ�ó���Ͽ����ϴ�.
	glutMouseFunc(MyMouseMove);
	glutKeyboardFunc(MyKeyboard); //Ű���� �ݹ�
	glutDisplayFunc(MyDisplay);
	glutMainLoop();
	return 0;
}