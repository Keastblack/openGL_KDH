#include <GL/glut.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <math.h>
int FlatShaded = 0; int Wireframed = 0;
	//flatshading�� wireframe�� ��۸��ϱ� ���� �ο� ����

int ViewX = 0, ViewY = 0;
	//���콺 �����ӿ� ���� ������ �ٲٱ� ���� ����

float angle = 0.0f;
float angle2 = 0.0f;

int tx = 0, ty = 0;

//������ ���� ����
bool IsTeapot=true;
bool IsCube=false;
bool IsSphere=false;
bool IsTorus=false;


//Ź���� �����迭; Ź�ڴٸ��� �����Ϸ��� �ڵ尡 �ʹ� ������Ƿ� Ź�ڴٸ��� �����Ͽ���.
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

void MyRotate(int x, int y){	//��ü ȸ���� ���� �Լ�
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

void MyMouseMove(int botton, int state, GLint X, GLint Y){	//���콺 �����ӿ� ���� �Լ�
    if (botton == GLUT_LEFT_BUTTON && state == GLUT_DOWN){	//��ư�� ����ä�� �����̴� ���,
        ViewX = X;	//���콺�� ��ǥ�� ����Ѵ�.
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
	case 'w':	//WireFramed ���
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

	for(GLint i=0; i<6; i++)	//Ź�� ������ �����迭�� ���
		glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_BYTE, &MyVertexList[4*i]);

	//�� ���� ���� �� �ϳ��� ���
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

void MyMainMenu(int entryID) {	// �޴��� �����Ͽ� �� ���� ���� �� �ϳ��� ����
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
	glutCreateWindow("2010136019_�赿��_Assignment04");

	//�޴� ����
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


