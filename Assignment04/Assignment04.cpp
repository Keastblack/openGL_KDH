#include <GL/glut.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <math.h>
int FlatShaded = 0; int Wireframed = 0;
	//flatshading과 wireframe을 토글링하기 위한 부울 변수

int ViewX = 0, ViewY = 0;
	//마우스 움직임에 따라 시점을 바꾸기 위한 변수

float angle = 0.0f;
float angle2 = 0.0f;

int tx = 0, ty = 0;

//도형에 관한 변수
bool IsTeapot=true;
bool IsCube=false;
bool IsSphere=false;
bool IsTorus=false;


//탁자의 정점배열; 탁자다리를 구현하려면 코드가 너무 길어지므로 탁자다리는 제외하였다.
GLfloat MyVertices[8][3] = {{-0.4, -0.4, 0.4}, {-0.4, -0.25, 0.4}, {0.4, -0.25, 0.4}, {0.4, -0.4, 0.4}, {-0.4, -0.4, -0.4}, {-0.4, -0.25, -0.4},
	{0.4, -0.25, -0.4}, {0.4, -0.4, -0.4}};
GLfloat MyColors[8][3] = {{0.2, 0.2, 0.2}, {1.0, 0.0, 0.0}, {1.0, 1.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}, {1.0, 0.0, 1.0}, 
	{1.0, 1.0, 1.0}, {0.0, 1.0, 1.0}};
GLubyte MyVertexList[24] = {0,3,2,1, 2,3,7,6, 0,4,7,3, 1,2,6,5, 4,5,6,7, 0,1,5,4};


void InitLight(){
	GLfloat mat_diffuse[] = {0.5, 0.4, 0.1, 1.0};
	GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat mat_ambient[] = {0.5, 0.4, 0.3, 1.0};
	GLfloat mat_shininess[] = {15.0};
	GLfloat light_specular[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat light_diffuse[] = {0.8, 0.8, 0.8, 1.0};
	GLfloat light_ambient[] = {0.3, 0.3, 0.3, 1.0};
	GLfloat light_position[] = {-3, 6, 3.0, 0.0};
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}

void MyRotate(int x, int y){	//물체 회전에 관한 함수
    tx = x - ViewX;
    ty = y - ViewY;

    if (tx > 0) angle += 2.0f;
    else if (tx < 0) angle -= 2.0f;
    else angle;

    if (ty > 0) angle2 += 2.0f;
    else if (ty < 0) angle2 -= 2.0f;
    else angle2;

    if (angle >= 360.0f)
        angle = angle - 360.0f;

    ViewX = x;
    ViewY = y;

    glutPostRedisplay();
}

void MyMouseMove(int botton, int state, GLint X, GLint Y){	//마우스 움직임에 관한 함수
    if (botton == GLUT_LEFT_BUTTON && state == GLUT_DOWN){	//버튼을 누른채로 움직이는 경우,
        ViewX = X;	//마우스의 좌표를 기록한다.
        ViewY = Y;
		glutPostRedisplay();
    }
}

void MyKeyboard(unsigned char key, int x, int y){
	switch(key){
	case 'q': case'Q': case '\033':
		exit(0);
		break;
	case 's':
		if(FlatShaded){
			FlatShaded = 0;
			glShadeModel(GL_SMOOTH);
		}
		else{
			FlatShaded = 1;
			glShadeModel(GL_FLAT);
		}
		glutPostRedisplay();
		break;
	case 'w':	//WireFramed 토글
		if(Wireframed){
			Wireframed=0;
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		else{
			Wireframed=1;
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		glutPostRedisplay();
		break;
	}
}

void MyDisplay(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glFrontFace(GL_CCW);
//	glEnable(GL_CULL_FACE);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glColorPointer(3, GL_FLOAT, 0, MyColors);
	glVertexPointer(3, GL_FLOAT, 0, MyVertices);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glRotatef(angle, 0.0, 1.0, 0.0);
    glRotatef(angle2, 1.0, 0.0, 0.0);

	for(GLint i=0; i<6; i++)	//탁자 모형을 정점배열로 출력
		glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_BYTE, &MyVertexList[4*i]);

	//네 가지 도형 중 하나를 출력
	if (IsTeapot)
		glutSolidTeapot(0.2);
	else if (IsCube)
		glutSolidCube(0.3);
	else if (IsSphere)
		glutSolidSphere(0.1, 20, 20);
	else if (IsTorus)
		glutSolidTorus(0.1, 0.13, 20, 20);
	
	gluLookAt(1.0, 1.0, 0.5, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	glFlush();
}

void MyMainMenu(int entryID) {	// 메뉴를 설정하여 네 가지 도형 중 하나를 선택
	switch(entryID){
	case 1:
		IsTeapot = true;               
		IsCube = false;
		IsSphere = false;
		IsTorus = false;
		break;
	case 2:
		IsTeapot = false;               
		IsCube = true;
		IsSphere = false;
		IsTorus = false;
		break;
	case 3:
		IsTeapot = false;               
		IsCube = false;
		IsSphere = true;
		IsTorus = false;
		break;
	case 4:
		IsTeapot = false;               
		IsCube = false;
		IsSphere = false;
		IsTorus = true;
		break;
	case 5:
		exit(0); 
		break;
	}
	glutPostRedisplay();
}

void MyReshape(int w, int h){
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	GLfloat WidthFactor = (GLfloat) w / (GLfloat) 800;
	GLfloat HeightFactor = (GLfloat) h / (GLfloat) 800;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0 * WidthFactor, 1.0 * WidthFactor,
		-1.0 * HeightFactor, 1.0 * HeightFactor, -1.0, 1.0);
}

int main(int argc, char** argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("2010136019_김동현_Assignment04");

	//메뉴 설정
	GLint MyMainMenuID = glutCreateMenu(MyMainMenu);
	glutAddMenuEntry("Draw Teapot", 1);
	glutAddMenuEntry("Draw Cube", 2);
	glutAddMenuEntry("Draw Sphere", 3);
	glutAddMenuEntry("Draw Torus", 4);
	glutAddMenuEntry("Exit", 5);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glClearColor(0.4, 0.4, 0.4, 0.0);
	InitLight();
	glutDisplayFunc(MyDisplay);
	glutKeyboardFunc(MyKeyboard);
	glutMouseFunc(MyMouseMove);
    glutMotionFunc(MyRotate);
	glutReshapeFunc(MyReshape);
	glutMainLoop();
}


