/*	A more sophisticated example of modern OpenGL
	Perry Kivolowitz - UW - Madison - CS 559 demo

	In this example program, I will build a solid
	object comprised of two triangle fans. Note 
	these are topologically triangle fans but the
	OpenGL triangle fan functionality is not being
	used. 

	Created:	02/28/13
	Updates:	03/05/13 - continued improvements
				Added solid color shader for drawing normals
				Switched to timer based redisplay.
*/
#include <iostream>
#include <assert.h>
#include <vector>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define _USE_MATH_DEFINES
#include <math.h>

#include "background.h"
#include "top.h"



using namespace std;
using namespace glm;

class Window
{
public:
	Window()
	{
		this->time_last_pause_began = this->total_time_paused = 0;
		this->normals = this->wireframe = this->paused = false;
		this->slices = 16;
		this->interval = 1000 / 120;
		this->window_handle = -1;
		this->horizontal_rotation = 0.0f;
		this->vertical_rotation = 0.0f;
	}

	

	float horizontal_rotation;
	float vertical_rotation;

	float time_last_pause_began;
	float total_time_paused;
	bool paused , wireframe, normals;
	int window_handle;
	int interval;
	int slices;
	ivec2 size;
	float window_aspect;
	vector<string> instructions;
} window;

Background background;
Top top;

void DisplayInstructions()
{
	if (window.window_handle == -1)
		return;

	vector<string> * s = &window.instructions;
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glColor3f(1.0f, 1.0f, 1.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, window.size.x, 0, window.size.y, 1, 10);
	glViewport(0, 0, window.size.x, window.size.y);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslated(10, 15 * s->size(), -5.5);
	glScaled(0.1, 0.1, 1.0);
	for (auto i = s->begin(); i < s->end(); ++i)
	{
		glPushMatrix();
		glutStrokeString(GLUT_STROKE_MONO_ROMAN, (const unsigned char *) (*i).c_str());
		glPopMatrix();
		glTranslated(0, -150, 0);
	}
}

void CloseFunc()
{
	window.window_handle = -1;
	background.TakeDown();
	top.TakeDown();
	_CrtDumpMemoryLeaks();
}

void ReshapeFunc(int w, int h)
{
	// Question for reader: Why is this 'if' statement here?
	if (h > 0)
	{
		window.size = ivec2(w, h);
		window.window_aspect = float(w) / float(h);
	}
}

void KeyboardFunc(unsigned char c, int x, int y)
{
	float current_time = float(glutGet(GLUT_ELAPSED_TIME)) / 1000.0f;

	switch (c)
	{
	case 's':
		top.StepShader();
		break;

	case 'n':
		top.EnableNormals(window.normals = !window.normals);
		break;

	case 'w':
		window.wireframe = !window.wireframe;
		break;

	case 'p':
		if (window.paused == true)
		{
			// Will be leaving paused state
			window.total_time_paused += (current_time - window.time_last_pause_began);
		}
		else
		{
			// Will be entering paused state
			window.time_last_pause_began = current_time;
		}
		window.paused = !window.paused;
		break;
	case 'i':
		if (window.vertical_rotation < 84.0f) {
			window.vertical_rotation += 3.0f;
		}
		break;
	case 'k':
		if (window.vertical_rotation > -84.0f) {
			window.vertical_rotation -= 3.0f;
		}
		break;
	case 'j':
		window.horizontal_rotation -= 3.0f;
		if(window.horizontal_rotation < 0) window.horizontal_rotation += 360.0f;
		break;
	case 'l':
		window.horizontal_rotation += 3.0f;
		if(window.horizontal_rotation > 360) window.horizontal_rotation -= 360.0f;
		break;
	case 'x':
	case 27:
		glutLeaveMainLoop();
		return;
	}
}

void SpecialFunc(int c, int x, int y)
{
	switch (c)
	{
	case GLUT_KEY_UP:
		++window.slices;
		top.TakeDown();
		top.Initialize(window.slices);
		break;

	case GLUT_KEY_DOWN:
		if (window.slices > 4)
		{
			--window.slices;
			top.TakeDown();
			top.Initialize(window.slices);
		}
		break;
	}
}

void DisplayFunc()
{
	float current_time = float(glutGet(GLUT_ELAPSED_TIME)) / 1000.0f;

	glEnable( GL_CULL_FACE );
	glEnable( GL_POINT_SMOOTH );
    glEnable( GL_BLEND );

	glClearColor(0.005f, 0.005f, 0.005f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, window.size.x, window.size.y);
	

	mat4 projection = perspective(50.0f, window.window_aspect, 0.1f, 100.0f);


	//mat4 modelview = lookAt(vec3(0.0f, 0.0f, 10.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
		// camX, camY, camZ solved with a little help from stack overflow
	// stackoverflow.com/questions/287655/opengl-rotating-a-camera-around-a-point */
	// solving for the x y and z positions prevents us from having to make a series of awkward euler rotations.
	//		this also prevents us from having to worry about things such as gimbal lock

	mat4 modelview;

	if(true){
		float distance = 15.0f;

		float camX = distance * sinf((window.horizontal_rotation)*(float(M_PI)/180.0f)) * cosf((window.vertical_rotation)*(float(M_PI)/180.0f));
		float camY = distance * sinf((window.vertical_rotation)*(float(M_PI)/180.0f));
		float camZ = -distance * cosf((window.horizontal_rotation)*(float(M_PI)/180.0f)) * cosf((window.vertical_rotation)*(float(M_PI)/180.0f));

		// for flying perspective, calculate another set of points, at a lower distance

		modelview = lookAt(vec3(camX, camY, camZ), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
	} else {
		float distance1 = 6.0f;
		float distance2 = 5.0f;

		float camX1 = distance1 * sinf((window.horizontal_rotation)*(float(M_PI)/180.0f)) * cosf((window.vertical_rotation)*(float(M_PI)/180.0f));
		float camY1 = distance1 * sinf((window.vertical_rotation)*(float(M_PI)/180.0f));
		float camZ1 = -distance1 * cosf((window.horizontal_rotation)*(float(M_PI)/180.0f)) * cosf((window.vertical_rotation)*(float(M_PI)/180.0f));

		float camX2 = distance2 * sinf((window.horizontal_rotation+45)*(float(M_PI)/180.0f)) * cosf((window.vertical_rotation)*(float(M_PI)/180.0f));
		float camY2 = distance2 * sinf((window.vertical_rotation)*(float(M_PI)/180.0f));
		float camZ2 = -distance2 * cosf((window.horizontal_rotation+45)*(float(M_PI)/180.0f)) * cosf((window.vertical_rotation)*(float(M_PI)/180.0f));
		// for flying perspective, calculate another set of points, at a lower distance

		vec3 eye = vec3(camX1, camY1, camZ1);
		vec3 target = vec3(camX2, camY2, camZ2);

		modelview = lookAt(eye, target, target);
	}


	// EARTH SKIMMING MODE
	//modelview = lookAt(vec3(0.0f, 0.0f, 5.3f), vec3(-8.0f, 0.0f, 0.0f), vec3(-1.0f, 0.0f, 0.0f));
	//modelview = rotate(modelview, window.horizontal_rotation, vec3(0.0f, 1.0f, 0.0f));
	//modelview = rotate(modelview, window.vertical_rotation, vec3(1.0f, 0.0f, 0.0f));
	
	// glPolygonMode is NOT modern OpenGL but will be allowed in Projects 2 and 3
	glPolygonMode(GL_FRONT_AND_BACK, window.wireframe ? GL_LINE : GL_FILL);
	
	if(false){
		background.Draw(window.size);
	} else {
		background.Draw(projection, modelview, window.size, (window.paused ? window.time_last_pause_began : current_time) - window.total_time_paused);
	}
	top.Draw(projection, modelview, window.size, (window.paused ? window.time_last_pause_began : current_time) - window.total_time_paused);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	DisplayInstructions();
	glFlush();
}

void TimerFunc(int value)
{
	// Question for reader: Why is this 'if' statement here?
	if (window.window_handle != -1)
	{
		glutTimerFunc(window.interval, TimerFunc, value);
		glutPostRedisplay();
	}
}

int main(int argc, char * argv[])
{
	glutInit(&argc, argv);
	glutInitWindowSize(1024, 512);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);

	window.window_handle = glutCreateWindow("A More Sophisticated Modern Hello World");
	glutReshapeFunc(ReshapeFunc);
	glutCloseFunc(CloseFunc);
	glutDisplayFunc(DisplayFunc);
	glutKeyboardFunc(KeyboardFunc);
	glutSpecialFunc(SpecialFunc);
	glutTimerFunc(window.interval, TimerFunc, 0);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);

	window.instructions.push_back("This program is an expanded  'Hello World'");
	window.instructions.push_back("using modern OpenGL.");
	window.instructions.push_back("");
	window.instructions.push_back("Perry Kivolowitz - For UW-Madison - CS 559");
	window.instructions.push_back("");
	window.instructions.push_back("UP / DN - changes slice count");
	window.instructions.push_back("n - toggles normals");
	window.instructions.push_back("p - toggles pause");
	window.instructions.push_back("s - cycles shaders");
	window.instructions.push_back("w - toggles wireframe");
	window.instructions.push_back("x - exits");

	if (glewInit() != GLEW_OK)
	{
		cerr << "GLEW failed to initialize." << endl;
		return 0;
	}

	if (!background.Initialize())
		return 0;

	if (!top.Initialize(window.slices))
		return 0;

	glutMainLoop();
}
