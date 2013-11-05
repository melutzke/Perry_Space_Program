/*	A more sophisticated example of modern OpenGL
	Perry Kivolowitz - UW - Madison - CS 559 demo

	In this example program, I will build a solid
	object comprised of two triangle fans. Note 
	these are Marsologically triangle fans but the
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
#include <fstream>
#include <iostream>

#include "ilcontainer.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include "background.h"
#include "Mars.h"
#include "Ship.h"



using namespace std;
using namespace glm;

class Window
{
public:
	Window()
	{
		this->time_last_pause_began = this->total_time_paused = 0;
		this->normals = this->wireframe = this->paused = this->tie_attack = false;
		this->slices = 24;
		this->interval = 1000 / 120;
		this->window_handle = -1;
		this->horizontal_rotation = 0.0f;
		this->vertical_rotation = 0.0f;
		this->pan_angle = 0.0f;
		this->CameraMode = 0;
	}

	

	float horizontal_rotation;
	float vertical_rotation;
	float pan_angle;

	static const int NUM_SATELLITES = 50;

	float time_last_pause_began;
	float total_time_paused;
	bool paused , wireframe, normals, tie_attack;
	int window_handle;
	int interval;
	int slices;
	int CameraMode;
	ivec2 size;
	float window_aspect;
	vector<string> instructions;
	vector<float> satellite_x_rotations;
	vector<float> satellite_z_rotations;
	vector<float> satellite_altitudes;
	vector<float> satellite_speeds;
} window;

Background background;
Mars Mars;
Ship ship;
Ship satellite;

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

vec3 CameraPosition(float radius, float vertical_rotation, float horizontal_rotation){
	float camX1 = radius * sinf((horizontal_rotation)*(float(M_PI)/180.0f)) 
							* cosf((vertical_rotation) * (float(M_PI)/180.0f));

	float camY1 = radius * sinf((vertical_rotation) * (float(M_PI)/180.0f));

	float camZ1 = -radius * cosf((horizontal_rotation)*(float(M_PI)/180.0f)) 
							* cosf((vertical_rotation) * (float(M_PI)/180.0f));

	return vec3(camX1, camY1, camZ1);
}

void CloseFunc()
{
	window.window_handle = -1;
	background.TakeDown();
	Mars.TakeDown();
	ship.TakeDown();
	satellite.TakeDown();
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
		ship.StepShader();
		satellite.StepShader();
		break;

	case 'm':
		Mars.StepShader();
		break;

	case '+':
		window.CameraMode++;
		if(window.CameraMode > 5) window.CameraMode = 0;
		break;

	case '-':
		window.CameraMode--;
		if(window.CameraMode < 0) window.CameraMode = 5;
		break;

	case 'n':
		Mars.EnableNormals(window.normals = !window.normals);
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
	case 'e':
		window.tie_attack = !window.tie_attack;
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
	case GLUT_KEY_LEFT:
		window.pan_angle-=3.0f;
		if (window.pan_angle < 0.0f)
			window.pan_angle += 360.0f;
		break;

	case GLUT_KEY_RIGHT:
		window.pan_angle+=3.0f;
		if (window.pan_angle > 360.0f)
			window.pan_angle -= 360.0f;
		break;
	}
}

void DisplayFunc()
{
	float current_time = float(glutGet(GLUT_ELAPSED_TIME)) / 1000.0f;

	if(!window.paused)
		window.horizontal_rotation += 0.1f;

	glEnable( GL_CULL_FACE );
    glEnable( GL_BLEND );

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, window.size.x, window.size.y);
	
	vec3 y_axis(0.0f, 1.0f, 0.0f);

	// cheap way to stop clicking the window on Perry's code from crashing the program
	if(!window.window_aspect) window.window_aspect = 0.66f;

	mat4 projection = perspective(50.0f, window.window_aspect, 0.1f, 1000.0f);


	// camX, camY, camZ solved with a little help from stack overflow
	// stackoverflow.com/questions/287655/opengl-rotating-a-camera-around-a-point */
	// solving for the x y and z positions prevents us from having to make a series of awkward euler rotations.
	//		this also prevents us from having to worry about things such as gimbal lock

	mat4 modelview;

	glPolygonMode(GL_FRONT_AND_BACK, window.wireframe ? GL_LINE : GL_FILL);

	if (window.CameraMode == 0) {
		// just your slowly turning satellite
		// in view: satellite, STARS
		// what moves: SATELLITE ROTATES

		

		

		modelview = translate(modelview, vec3(0.0f, 0.0f, -5.0f));
		
		background.Draw(projection, modelview, window.size, (window.paused ? window.time_last_pause_began : current_time) - window.total_time_paused, window.CameraMode);
		
		modelview = translate(modelview, vec3(0.0f, 0.0f, -5.0f));
		modelview = rotate(modelview, window.horizontal_rotation, y_axis);

		satellite.Draw(projection, modelview, window.size, (window.paused ? window.time_last_pause_began : current_time) - window.total_time_paused, window.CameraMode);

	} else if (window.CameraMode == 1){
		// just your slowly turning ship
		// in view: SHIP, STARS
		// what moves: SHIP ROTATES

		modelview = translate(modelview, vec3(0.0f, 0.0f, -5.0f));
		
		background.Draw(projection, modelview, window.size, (window.paused ? window.time_last_pause_began : current_time) - window.total_time_paused, window.CameraMode);
		
		modelview = translate(modelview, vec3(0.0f, 0.0f, -5.0f));
		modelview = rotate(modelview, window.horizontal_rotation, y_axis);

		ship.Draw(projection, modelview, window.size, (window.paused ? window.time_last_pause_began : current_time) - window.total_time_paused, window.CameraMode);

	} else if(window.CameraMode == 2){
		// just Mars slowly spinning
		// in view: MARS, STARS
		// what moves: MARS ROTATES
		
		modelview = lookAt(vec3(0.0f, 0.0f, -14.0f), vec3(0.0f), y_axis);

		if(window.tie_attack)
			projection = perspective(75.0f, window.window_aspect, 0.1f, 1000.0f);
		else
			projection = perspective(50.0f, window.window_aspect, 0.1f, 1000.0f);
		
		background.Draw(projection, modelview, window.size, (window.paused ? window.time_last_pause_began : current_time) - window.total_time_paused, window.CameraMode);
		modelview = rotate(modelview, window.horizontal_rotation, y_axis);
		Mars.Draw(projection, modelview, window.size, (window.paused ? window.time_last_pause_began : current_time) - window.total_time_paused, window.CameraMode);
		
		if (window.tie_attack) {
			mat4 temp = modelview;

			for (int i = 0; i < window.NUM_SATELLITES; i++) {
				modelview = temp;
			
				modelview = rotate(modelview, window.satellite_x_rotations[i], vec3(1.0f, 0.0f, 0.0f));
				modelview = rotate(modelview, window.satellite_z_rotations[i], vec3(0.0f, 0.0f, 1.0f));
				modelview = rotate(modelview, window.satellite_speeds[i]*window.horizontal_rotation, y_axis);

				modelview = translate(modelview, vec3(0.0f, 0.0f, 6.0f + window.satellite_altitudes[i]));
				modelview = rotate(modelview, 90.0f, y_axis);
				modelview = scale(modelview, vec3(0.05f));
				satellite.Draw(projection, modelview, window.size, (window.paused ? window.time_last_pause_began : current_time) - window.total_time_paused, window.CameraMode);
			}
		}
	} else if(window.CameraMode == 3){
		// first person view over mars (NO SHIP)
		// in view: MARS, STARS
		// what moves: CAMERA CHANGES POSITION

		float distance1 = 6.0f;
		float distance2 = 5.0f;

		vec3 eye =    CameraPosition(distance1, window.vertical_rotation, window.horizontal_rotation);
		vec3 target = CameraPosition(distance2, window.vertical_rotation, window.horizontal_rotation+45.0f);

		modelview = lookAt(eye, target, target);
		modelview = rotate(modelview, window.pan_angle, eye);

		background.Draw(projection, modelview, window.size, (window.paused ? window.time_last_pause_began : current_time) - window.total_time_paused, window.CameraMode);
		Mars.Draw(projection, modelview, window.size, (window.paused ? window.time_last_pause_began : current_time) - window.total_time_paused, window.CameraMode);

		if (window.tie_attack) {
			mat4 temp = modelview;

			for (int i = 0; i < window.NUM_SATELLITES; i++) {
				modelview = temp;
			
				modelview = rotate(modelview, window.satellite_x_rotations[i], vec3(1.0f, 0.0f, 0.0f));
				modelview = rotate(modelview, window.satellite_z_rotations[i], vec3(0.0f, 0.0f, 1.0f));
				modelview = rotate(modelview, window.satellite_speeds[i]*window.horizontal_rotation, y_axis);

				modelview = translate(modelview, vec3(0.0f, 0.0f, 6.0f + window.satellite_altitudes[i]));
				modelview = rotate(modelview, 90.0f, y_axis);
				modelview = scale(modelview, vec3(0.1));
				satellite.Draw(projection, modelview, window.size, (window.paused ? window.time_last_pause_began : current_time) - window.total_time_paused, window.CameraMode);
			}
		}
	} else if(window.CameraMode == 4) {
		// first person view over mars w/ SHIP
		// in view: MARS, SHIP, STARS
		// what moves: CAMERA CHANGES POSITION, SHIP CHANGES POSITION

		float distance1 = 6.0f;
		float distance2 = 5.0f;

		vec3 eye =    CameraPosition(distance1, window.vertical_rotation, window.horizontal_rotation);
		vec3 target = CameraPosition(distance2, window.vertical_rotation, window.horizontal_rotation+45.0f);

		modelview = lookAt(eye, target, target);
		
		background.Draw(projection, modelview, window.size, (window.paused ? window.time_last_pause_began : current_time) - window.total_time_paused, window.CameraMode);
		Mars.Draw(projection, modelview, window.size, (window.paused ? window.time_last_pause_began : current_time) - window.total_time_paused, window.CameraMode);
		
		// This un-does the rotation of Mars, resulting in the ship being drawn in a static
		// position while the planet spins
		modelview = rotate(modelview, window.horizontal_rotation, vec3(0.0f, -1.0f, 0.0f));

		modelview = rotate(modelview, 160.0f, y_axis);				// This is done to get the ship to appear 20 
																			// degrees 'ahead' of the camera

		modelview = translate(modelview, vec3(0.0f, 0.0f, 5.5f));			// Moves the ship away from the center of Mars
		modelview = rotate(modelview, 90.0f, vec3(0.0f, 0.0f, 1.0f));		// Tilts the ship down (and away from camera)
		modelview = scale(modelview, vec3(0.10f));							// Makes ship appropriately smaller than Mars
		glDisable( GL_CULL_FACE );
		ship.Draw(projection, modelview, window.size, (window.paused ? window.time_last_pause_began : current_time) - window.total_time_paused, window.CameraMode);
		glEnable( GL_CULL_FACE );

	} else if(window.CameraMode == 5) {
		// Nice view of just STAR FIELD
		// in view: STARS
		// what moves: STARS ROTATE

		float distance = 150.0f;

		float camX = distance * sinf((window.horizontal_rotation)*(float(M_PI)/180.0f)) * cosf((window.vertical_rotation)*(float(M_PI)/180.0f));
		float camY = distance * sinf((window.vertical_rotation)*(float(M_PI)/180.0f));
		float camZ = -distance * cosf((window.horizontal_rotation)*(float(M_PI)/180.0f)) * cosf((window.vertical_rotation)*(float(M_PI)/180.0f));

		vec3 eye =    CameraPosition(distance, window.vertical_rotation, window.horizontal_rotation);
		vec3 target = vec3(0.0f);

		modelview = lookAt(eye, target, y_axis);

		background.Draw(projection, modelview, window.size, (window.paused ? window.time_last_pause_began : current_time) - window.total_time_paused, window.CameraMode);
	
	}

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
	string the_file;
	if ( argc != 2 ){ // argc should be 2 for correct execution
		cout << "usage: "<< argv[0] <<" <filename>\n";
		return -1;
	} else {
		the_file = argv[1];
	}


	for (int i = 0; i < window.NUM_SATELLITES; i++) {
		// Generates random changes in altitude from 0 to 5
		window.satellite_altitudes.push_back(float((rand() % 1000 + 1) / 200));
		
		// Generates random changes in direction between 0 and 359
		window.satellite_x_rotations.push_back(float(rand() % 360));
		window.satellite_z_rotations.push_back(float(rand() % 360));

		// Generates random speed factor from 2.75 to 5.75
		window.satellite_speeds.push_back(float((rand() % 30)/10 + 2.75f));
	}

	glutInit(&argc, argv);
	glutInitWindowSize(1024, 512);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);

	window.window_handle = glutCreateWindow("Perry Space Program: Mitchell Lutzke - Steve Krejci");
	glutReshapeFunc(ReshapeFunc);
	glutCloseFunc(CloseFunc);
	glutDisplayFunc(DisplayFunc);
	glutKeyboardFunc(KeyboardFunc);
	glutSpecialFunc(SpecialFunc);
	glutTimerFunc(window.interval, TimerFunc, 0);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);

	window.instructions.push_back("Mitchell Lutzke and Steve Krejci - CS559 - UW-Madison");
	window.instructions.push_back("");
	window.instructions.push_back("[On Mars overview] e - enable TIE fighter invasion");
	window.instructions.push_back("[On Mars flyover] Left arrow / Right arrow - pan camera");
	window.instructions.push_back("p - toggles pause");
	window.instructions.push_back("w - toggles wireframe");
	window.instructions.push_back("x - exits");

	if (glewInit() != GLEW_OK)
	{
		cerr << "GLEW failed to initialize." << endl;
		return 0;
	}

	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);

	if (!Mars.Initialize(the_file))
		return 0;

	if (!background.Initialize())
		return 0;

	if (!ship.Initialize(window.slices, true))
		return 0;

	if (!satellite.Initialize(window.slices, false))
		return 0;

	glutMainLoop();
}
