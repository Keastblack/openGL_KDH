/*******************************
2010136019 김동현 Assignment08
*******************************/
/*

마우스 클릭을 두 번 할때마다 그 두 점을 잇는 선분이 그려집니다.
윈도우창을 늘리면 선그리기가 부정확해지므로 윈도우의 크기 변경을 원한다면
본 코드 내의 WinWidth, WinHeight 값을 수정하는 것이 좋습니다.
*/
/*
종료 버튼 : Q
*/

#include <iostream>
#include <stdlib.h>
#include <math.h> //abs()사용
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <string.h>
using namespace std;

int WinWidth = 800, WinHeight = 800; //윈도우 크기
GLfloat ViewX = 0, ViewY = 0; //마우스를 클릭하였을 때의 x,y좌표
bool MouseClick = false; //마우스 클릭이 되기 전에는 false
/*
마우스를 클릭하였을 때 좌표를 glutWindow 기준으로 계산한다.
*/
void MyMouseMove(int botton, int state, GLint X, GLint Y){	//마우스 움직임에 관한 함수
    if (botton == GLUT_LEFT_BUTTON && state == GLUT_DOWN){	//마우스 왼쪽 버튼을 눌렀을 때
		MouseClick = true; //마우스 클릭이 시작됨을 뜻함.
		GLfloat half_WinWidth=(GLfloat)WinWidth/2.0, half_WinHeight=(GLfloat)WinHeight/2.0; //윈도우 크기의 반절
		if(X>=half_WinWidth && Y>=half_WinHeight/2){ //(X,Y)가 제1사분면에 있는 경우
				ViewX=((GLfloat)X-half_WinWidth)/half_WinWidth;
				ViewY=(half_WinHeight-(GLfloat)Y)/half_WinHeight;}
		else if(X>=half_WinWidth && Y<WinHeight/2){ //(X,Y)가 제2사분면에 있는 경우
				ViewX=((GLfloat)X-half_WinWidth)/half_WinWidth;
				ViewY=-((GLfloat)Y-half_WinHeight)/half_WinHeight;}
		else if(X<half_WinWidth && Y<WinHeight/2){ //(X,Y)가 제3사분면에 있는 경우
				ViewX=-(half_WinWidth-(GLfloat)X)/half_WinWidth;
				ViewY=-((GLfloat)Y-half_WinHeight)/half_WinHeight;}
		else{ //(X,Y)가 제4사분면에 있는 경우
				ViewX=-(half_WinWidth-(GLfloat)X)/half_WinWidth;
				ViewY=(half_WinHeight-(GLfloat)Y)/half_WinHeight;}
		cout<<"ViewX : "<<ViewX<<"\tViewY : "<<ViewY<<endl;
		glutPostRedisplay();
    }
}
/*
주어진 점을 기준으로 정사각형 도트를 찍는다.
*/
void DrawDot(GLfloat x, GLfloat y){
	GLfloat d=0.0125; //정사각형 도트의 한 변의 반절 길이
	glBegin(GL_POLYGON);
		glVertex2f(x+d,y+d);
		glVertex2f(x+d,y-d);
		glVertex2f(x-d,y-d);
		glVertex2f(x-d,y+d);
	glEnd();
}
/*
브레즈넘 알고리즘에 의한
두 점 사이의 선분을 그린다.
*/
void bres_line(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2){
	GLfloat dx, dy;
	GLfloat p_value; //다음에 도트 찍을 픽셀의 기준
	GLfloat inc_minus; //기울기가 낮을 때 p_value 조정
	GLfloat inc_plus; //기울기가 높을 때 p_value 조정
	GLfloat inc_value=0.025; //화소단위(즉, 이 프로그램에서는 도트간 거리이므로 정사각형 도트의 지름을 그 크기로 한다.
	GLfloat temp; //임시저장변수

	dx = abs(x2-x1); //x의 변화폭
	dy = abs(y2-y1); //y의 변화폭

	if(dx >= dy){ //y보다 x의 변화폭이 클 경우 즉, 저각
		inc_minus=2*dy;
		inc_plus=2*(dy-dx); //원래 dx-dy

		if(x2<x1){ //(x1,y1)을 좌측, (x2,y2)를 우측에 둔다.
			temp = x1;
			x1 = x2;
			x2 = temp;

			temp = y1;
			y1 = y2;
			y2 = temp;}
	
		if(y1 >= y2) //위에서 아래로 내려가는(기울기<0) 직선
			inc_value *= -1.0;

		//DrawDot(x1,y1); //시작좌표(좌측의 좌표) 찍기
		p_value = 2*dy-dx; //다음 픽셀의 기준

		for(GLfloat seek_x=x1; seek_x<x2; seek_x+=abs(inc_value)){ //x좌표가 x2에 이를때까지 좌표를 계속 찍는다.
			if(p_value<0){ //기울기가 낮아서 아랫쪽 픽셀을 지정함
				p_value += inc_minus;}
			//y1값은 그대로
			else { //기울기가 높아서 위쪽 픽셀을 지정함
				p_value += inc_plus; 
				y1 += inc_value;} //y1->y2방향으로 단위만큼 이동
			DrawDot(seek_x,y1);} //알고리즘에 따라 좌표를 찍는다.
	}
	else{ //x보다 y의 변화폭이 클 경우 즉, 고각
		inc_minus= 2.0*dx;
		inc_plus= 2.0*(dx-dy);

		if(y2 < y1){ //(x1,y1)을 아래측, (x2,y2)를 위측에 둔다.
			temp = y1;
			y1 = y2;
			y2 = temp;

			temp = x1;
			x1 = x2;
			x2 = temp;}

		if(x1>=x2)
			inc_value *= -1.0;

		//DrawDot(x1,y1); //시작좌표(아래측의 좌표) 찍기
		p_value = 2*dx - dy; //다음 픽셀의 기준

		for(GLfloat seek_y=y1; seek_y < y2; seek_y+=abs(inc_value)){
			if(p_value<0){ //기울기가 낮아서 아랫쪽 픽셀을 지정함
				p_value += inc_minus;}
			//x1값은 그대로
			else{
				p_value += inc_plus;
				x1 += inc_value;} //x1->x2방향으로 단위만큼 이동             
			DrawDot(x1,seek_y);} //알고리즘에 따라 좌표를 찍는다.
	}
}
void MyDisplay(){
	static int i=0;//배열인자, 좌표값을 두 개씩 받도록 한다.
	static GLfloat tempX[2], tempY[2];
	if(MouseClick){ //마우스클릭이 활성화 되었을 때
		tempX[i]=ViewX; tempY[i]=ViewY; //좌표를 입력받는다. 첫 번째 좌표는 tempX[0],tempY[0]에, 두 번째 좌표는 tempX[1],tempY[1]에 입력한다.
		DrawDot(tempX[i],tempY[i]); //시작좌표와 끝좌표를 찍는다. 점찍는 것은 중복되도 상관없다.
		glFlush();
		i++;
		if((tempX[0]==tempX[1]) && (tempY[0]==tempY[1])) //중복된 좌표
			i=1; //다시 입력받는다.
		if(i==2){ //두 좌표를 입력받았을 때
			bres_line(tempX[0],tempY[0],tempX[1],tempY[1]);
			i=0; //다시 첫 번째 좌표부터 입력받는다.
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
	//glClearColor(0.0, 0.0, 0.0, 0.0); //쓸 일이 없으므로 주석처리하였습니다.
	glutMouseFunc(MyMouseMove);
	glutKeyboardFunc(MyKeyboard); //키보드 콜백
	glutDisplayFunc(MyDisplay);
	glutMainLoop();
	return 0;
}