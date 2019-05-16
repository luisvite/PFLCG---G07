//Semestre 2019 - 2
//************************************************************//
//************************************************************//
//************** Alumno (s): *********************************//
//*************		Luis Enrique Vite Aquino			******//
//*************											******//
//************************************************************//
//************************************************************//

#include "texture.h"
#include "figuras.h"
#include "Camera.h"

CCamera objCamera;  //Creramos un objeto de tipo camara
GLfloat g_lookupdown = 0.0f; 
int font = (int)GLUT_BITMAP_HELVETICA_18;

// Variables usadas para calcular frames por segundo: (Windows)
DWORD dwFrames = 0;
DWORD dwCurrentTime = 0;
DWORD dwLastUpdateTime = 0;
DWORD dwLastUpdateTime2 = 0;
DWORD dwLastUpdateTime3 = 0;
DWORD dwElapsedTime = 0;
DWORD dwElapsedTime2 = 0;
DWORD dwElapsedTime3 = 0;

//Variables usadas para crear el movimiento
int tor = 0, tor2=2, tor3=0;
int anim = 0;
int t = 0;
GLfloat ejex = 0, ejey = 5.5, ejez=0;

GLfloat Diffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };				// Diffuse Light Values
GLfloat Specular[] = { 1.0, 1.0, 1.0, 1.0 };				// Specular Light Values
GLfloat Position[] = { 0.0f, 7.0f, -5.0f, 0.0f };			// Light Position
GLfloat Position2[] = { 0.0f, 0.0f, -5.0f, 1.0f };			// Light Position

CTexture text1;
CTexture text2;
CTexture text3;

CFiguras fig1;
CFiguras tornado;

void InitGL(GLvoid) {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);				// Negro de fondo	
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);

	glLightfv(GL_LIGHT1, GL_POSITION, Position);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, Diffuse);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glEnable(GL_COLOR_MATERIAL);

	glClearDepth(1.0f);									// Configuramos Depth Buffer
	glEnable(GL_DEPTH_TEST);							// Habilitamos Depth Testing
	glDepthFunc(GL_LEQUAL);								// Tipo de Depth Testing a realizar
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);

	text1.LoadBMP("cielo.bmp");
	text1.BuildGLTexture();
	text1.ReleaseImage();
	
	text2.LoadTGA("textura/grass.tga");
	text2.BuildGLTexture();
	text2.ReleaseImage();

	text3.LoadTGA("textura/tornado.tga");
	text3.BuildGLTexture();
	text3.ReleaseImage();

	objCamera.Position_Camera(0, 5.5f, 50, 0, 5.5f, 0, 0, 1, 0);
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glPushMatrix();
		glRotatef(g_lookupdown, 1.0f, 0, 0);

		gluLookAt(objCamera.mPos.x, objCamera.mPos.y, objCamera.mPos.z,
			objCamera.mView.x, objCamera.mView.y, objCamera.mView.z,
			objCamera.mUp.x, objCamera.mUp.y, objCamera.mUp.z);

		glPushMatrix(); //Creamos cielo y suelo
			glDisable(GL_LIGHTING);
			glTranslatef(0, 65, 0);
			fig1.skybox(500.0, 130.0, 500.0, text1.GLindex, text2.GLindex);
			glEnable(GL_LIGHTING);
		glPopMatrix();

		glPushMatrix();
			glRotatef(tor, 0.0, 1.0, 0.0);
			glTranslatef(50, 0, 0);
			glRotatef(tor3, 0.0, 1.0, 0.0);
			glDisable(GL_LIGHTING);
			tornado.tornado(text3.GLindex,tor2);
			glEnable(GL_LIGHTING);
		glPopMatrix();
	glPopMatrix();

	glutSwapBuffers();
}

void animacion()
{
	// Calculate the number of frames per one second:
	//dwFrames++;
	dwCurrentTime = GetTickCount(); // Even better to use timeGetTime()
	dwElapsedTime = dwCurrentTime - dwLastUpdateTime;
	dwElapsedTime2 = dwCurrentTime - dwLastUpdateTime2;
	dwElapsedTime3 = dwCurrentTime - dwLastUpdateTime3;

	if (anim == 1)
	{
		if (dwElapsedTime >= 80)
		{
			tor = (tor - 3) % 360;
			dwLastUpdateTime = dwCurrentTime;
		}
		if (dwElapsedTime2 >= 20)
		{
			tor3 = (tor3 - 1) % 360;
			dwLastUpdateTime2 = dwCurrentTime;
		}
	}
	glutPostRedisplay();
}

void reshape(int width, int height)   // Creamos funcion Reshape
{
	if (height == 0){ //Prevenimos division por cero
		height = 1;
	}

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);						// Seleccionamos Projection Matrix
	glLoadIdentity();

	// Tipo de Vista
	glFrustum(-0.1, 0.1, -0.1, 0.1, 0.1, 1000.0);
	glMatrixMode(GL_MODELVIEW);							// Seleccionamos Modelview Matrix
	glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y)  // Create Keyboard Function
{
	switch (key) {
		case 'q':
			if (anim == 0)
			{
				anim = 1;
			}
			else {
				anim = 0;
			}
			break;
		case 'w':   //Movimientos de camara
		case 'W':
			objCamera.Move_Camera(CAMERASPEED + 0.2);
			break;
		case 's':
		case 'S':
			objCamera.Move_Camera(-(CAMERASPEED + 0.2));
			break;
		case 'a':
		case 'A':
			objCamera.Strafe_Camera(-(CAMERASPEED + 0.4));
			break;
		case 'd':
		case 'D':
			objCamera.Strafe_Camera(CAMERASPEED + 0.4);
			break;
		case 27:        // Cuando Esc es presionado...
			exit(0);   // Salimos del programa
			break;
		default:        // Cualquier otra
			break;
	}
	glutPostRedisplay();
}

void arrow_keys(int a_keys, int x, int y)  // Funcion para manejo de teclas especiales (arrow keys)
{
	switch (a_keys) {
		case GLUT_KEY_PAGE_UP:
			objCamera.UpDown_Camera(CAMERASPEED+ 0.3);
			break;
		case GLUT_KEY_PAGE_DOWN:
			objCamera.UpDown_Camera(-(CAMERASPEED + 0.3));
			break;
		case GLUT_KEY_UP:     // Presionamos tecla ARRIBA...
			g_lookupdown -= 1.0f;
			break;
		case GLUT_KEY_DOWN:               // Presionamos tecla ABAJO...
			g_lookupdown += 1.0f;
			break;
		case GLUT_KEY_LEFT:
			objCamera.Rotate_View(-CAMERASPEED);
			break;
		case GLUT_KEY_RIGHT:
			objCamera.Rotate_View(CAMERASPEED);
			break;
		default:
			break;
	}
	glutPostRedisplay();
}

int main(int argc, char** argv)   // Main Function
{

	glutInit(&argc, argv); // Inicializamos OpenGL
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH); // Display Mode (Clores RGB y alpha | Buffer Doble )
	glutInitWindowSize(500, 500);	// Tama�o de la Ventana
	glutInitWindowPosition(0, 0);	//Posicion de la Ventana
	glutCreateWindow("PFCG"); // Nombre de la Ventana
	//glutFullScreen     ( );         // Full Screen
	InitGL();						// Parametros iniciales de la aplicacion
	glutDisplayFunc(display);  //Indicamos a Glut funci�n de dibujo
	glutReshapeFunc(reshape);	//Indicamos a Glut funci�n en caso de cambio de tamano
	glutKeyboardFunc(keyboard);	//Indicamos a Glut funci�n de manejo de teclado
	glutSpecialFunc(arrow_keys);	//Otras
	glutIdleFunc(animacion);

	glutMainLoop();          // 

	return 0;
}