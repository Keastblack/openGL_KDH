#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

void MyDisplay(){
	drawHead();
	glFlush();
}
void MyKeyboard(unsigned char KeyPressed,int x,int y){
	switch (KeyPressed){
		case 'q':
			exit(0);
			break;
	}
}

int main(int argc, char** argv){
	glutInit(&argc, argv);
//	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Simple");

	glutKeyboardFunc(MyKeyboard); //키보드 콜백
	glutDisplayFunc(MyDisplay);
	glutMainLoop();

	return 0;
}