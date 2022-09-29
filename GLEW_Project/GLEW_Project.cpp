#pragma comment(lib, "glfw3_mt.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "tokamak_d.lib")

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <tokamak.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb-master/stb_image.h>

using namespace std;
using namespace glm;

#define PI 3.141592653

static int WIDTH = 640;
static int HEIGHT = 480;

double prevTime = 0;
double crntTime = 0;
double timeDiff;
int counter = 0;

string s = "FPS: " + to_string(10);
char* str;

neSimulator* pSimulator = NULL;
neAnimatedBody* pFloor = NULL;

unsigned int texture;

class meCube {
public:
	neRigidBody* pCube = NULL;
	neGeometry* cubeShape;
	float time_create;
	neT3 t;

	meCube() {
		float x = float(rand() % 9 - 4);
		pCube = pSimulator->CreateRigidBody();
		cubeShape = pCube->AddGeometry();
		cubeShape->SetBoxSize(1.f, 1.f, 1.f);
		pCube->UpdateBoundingInfo();
		pCube->SetInertiaTensor(neBoxInertiaTensor(1.f, 1.f, 1.f, 2.f));
		pCube->SetMass(2.f);
		time_create = glfwGetTime();
		neV3 cubePos;
		cubePos.Set(x, 7.f, 0.f);
		pCube->SetPos(cubePos);
	}

	bool RemoveCube(float delta) {
		neT3 p = pFloor->GetTransform();
		if (delta > time_create + 15 || t.pos[1] < p.pos[1] - 0.8) {
			pCube->RemoveGeometry(cubeShape);
			return true;
		}
		return false;
	}

	void DrawCube(float size) {
		glPushMatrix();

		t = pCube->GetTransform();
		float transform[] = {
			t.rot[0][0], t.rot[0][1], t.rot[0][2], 0.f,
			t.rot[1][0], t.rot[1][1], t.rot[1][2], 0.f,
			t.rot[2][0], t.rot[2][1], t.rot[2][2], 0.f,
			t.pos[0], t.pos[1], t.pos[2], 1.f
		};
		glMultMatrixf(&transform[0]);

		glBindTexture(GL_TEXTURE_2D, texture);

		glBegin(GL_QUADS);
		
		glColor3f(1, 1, 1);

		//glColor3f(0.0, 0.5, 0.0);    // Color Green - TOP
		glNormal3f(0, 1, 0);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-size / 2, size / 2, size / 2);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(size / 2, size / 2, size / 2);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(size / 2, size / 2, -size / 2);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-size / 2, size / 2, -size / 2);

		//glCosizeor3f(1, 0.0, 0.0); // Cosizeor RED - Bottom
		glNormal3f(0, -1, 0);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-size / 2, -size / 2, size / 2);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-size / 2, -size / 2, -size / 2);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(size / 2, -size / 2, -size / 2);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(size / 2, -size / 2, size / 2);

		//glCosizeor3f(0.5, 0.5, 0.0); // Cosizeor Yesizesizeow - Back
		glNormal3f(0, 0, -1);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-size / 2, -size / 2, -size / 2);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-size / 2, size / 2, -size / 2);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(size / 2, size / 2, -size / 2);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(size / 2, -size / 2, -size / 2);

		//glCosizeor3f(0.5, 0.0, 0.5); // Cosizeor Viosizeet - Front
		glNormal3f(0, 0, 1);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-size / 2, -size / 2, size / 2);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(size / 2, -size / 2, size / 2);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(size / 2, size / 2, size / 2);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-size / 2, size / 2, size / 2);

		//glCosizeor3f(0.0, 0.0, 0.5); //Cosizeor Bsizeue - RIGHT
		glNormal3f(1, 0, 0);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(size / 2, -size / 2, -size / 2);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(size / 2, size / 2, -size / 2);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(size / 2, size / 2, size / 2);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(size / 2, -size / 2, size / 2);

		//glCosizeor3f(0.5, 0.5, 0.0); //Cosizeor Orange - sizeeft
		glNormal3f(-1, 0, 0);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-size / 2, -size / 2, size / 2);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-size / 2, size / 2, size / 2);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-size / 2, size / 2, -size / 2);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-size / 2, -size / 2, -size / 2);

		glEnd();

		glPopMatrix();
	}
};

vector<meCube>* pMassCube = new vector<meCube>();

void drawFloor(float l) {
	glBindTexture(GL_TEXTURE_2D, texture);

	glBegin(GL_QUADS);
	
	glColor3f(0.8f, 0.8f, 0.8f);

	glNormal3f(0, 1, 0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-l / 2, 0, l / 2);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(l / 2, 0, l / 2);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(l / 2, 0, -l / 2);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-l / 2, 0, -l / 2);

	glEnd();
}

static void Initialize(void) {
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (float)640 / (float)480, 0.1f, 1000.0f);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	int width, height, cnt;
	unsigned char *data = stbi_load("wall.png", &width, &height, &cnt, 0);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height,
			0,cnt == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
	stbi_image_free(data);

	GLfloat black[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat yellow[] = { 1.0, 1.0, 0.0, 1.0 };
	GLfloat cyan[] = { 0.0, 1.0, 1.0, 1.0 };
	GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat direction[] = { 1.0, 1.0, 1.0, 0.0 };

	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, cyan);
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMaterialf(GL_FRONT, GL_SHININESS, 30);

	glLightfv(GL_LIGHT0, GL_AMBIENT, black);
	//glLightfv(GL_LIGHT0, GL_DIFFUSE, yellow);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white);
	glLightfv(GL_LIGHT0, GL_POSITION, direction);

	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_TEXTURE_2D);
	
	glEnable(GL_NORMALIZE);

	glEnable(GL_LIGHTING);                
	glEnable(GL_LIGHT0);                
	glEnable(GL_DEPTH_TEST);  
	
	neSimulatorSizeInfo sizeInfo;
	neV3 gravity;

	sizeInfo.rigidBodiesCount = 1000;
	sizeInfo.animatedBodiesCount = 1;
	s32 totalBody = sizeInfo.rigidBodiesCount + sizeInfo.animatedBodiesCount;
	sizeInfo.geometriesCount = totalBody;

	sizeInfo.overlappedPairsCount = totalBody * (totalBody - 1) / 2;
	sizeInfo.rigidParticleCount = 0;
	sizeInfo.constraintsCount = 0;
	sizeInfo.terrainNodesStartCount = 0;

	gravity.Set(0.f, -9.8f, 0.f);
	pSimulator = neSimulator::CreateSimulator(sizeInfo, NULL, &gravity);

	pMassCube->push_back(meCube());

	pFloor = pSimulator->CreateAnimatedBody();
	neGeometry* floorShape = pFloor->AddGeometry();
	floorShape->SetBoxSize(10.f, 0.1f, 10.f);
	pFloor->UpdateBoundingInfo();
	neV3 floorPos;
	floorPos.Set(0.f, 0.f, 0.f);
	pFloor->SetPos(floorPos);

	gluLookAt(0.0f, 10.0f, 15.0f, 0.0f, 3.0f, 0.0f, 0.0f, 1.0f, 0.0f);
}

float GetElapsedTime()
{
	static int oldTime = GetTickCount();
	int newTime = GetTickCount();
	float result = (newTime - oldTime) / 1000.0f;
	oldTime = newTime;
	return result;
}

static void Update(GLFWwindow* window, float delta) {
	static float oldElapsed;
	float newElapsed = GetElapsedTime();
	if (oldElapsed != 0.0f)
	{
		if (newElapsed < 0.8f * oldElapsed)
			newElapsed = 0.8f * oldElapsed;
		if (newElapsed > 1.2f * oldElapsed)
			newElapsed = 1.2f * oldElapsed;
		if (newElapsed > 1.0f / 45.0f)
			newElapsed = 1.0f / 45.0f;
	}
	oldElapsed = newElapsed;
	pSimulator->Advance(newElapsed);
}

static void RenderScene(GLFWwindow* window, float delta) {
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(0.0f, 10.0f, 15.0f, 0.0f, 3.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	glPushMatrix();
	{
		glTranslatef(0.0, 8.0, 0.0);

		GLfloat material_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_diffuse);

		GLfloat light3_diffuse[] = { 0.4, 0.7, 0.2 };
		GLfloat light3_position[] = { 0.0, 8.0, 0.0, 1.0 };
		GLfloat light3_spot_direction[] = { 0.0, 0.0, 0.0 };

		glEnable(GL_LIGHT3);
		glLightfv(GL_LIGHT3, GL_DIFFUSE, light3_diffuse);
		glLightfv(GL_LIGHT3, GL_POSITION, light3_position);
		glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 45);
		glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, light3_spot_direction);

	}
	glPopMatrix();

	for (int i = 0; i < pMassCube->size(); i++) {
		if (!pMassCube[0][i].RemoveCube(glfwGetTime())) {
			pMassCube[0][i].DrawCube(1.f);
		}
		else {
			auto iter = pMassCube[0].begin();
			pMassCube[0].erase(iter + i);
		}
	}

	glPushMatrix();
	{
		neT3 t = pFloor->GetTransform();
		float transform[] = {
			t.rot[0][0], t.rot[0][1], t.rot[0][2], 0.f,
			t.rot[1][0], t.rot[1][1], t.rot[1][2], 0.f,
			t.rot[2][0], t.rot[2][1], t.rot[2][2], 0.f,
			t.pos[0], t.pos[1], t.pos[2], 1.f
		};
		glMultMatrixf(&transform[0]);
		drawFloor(10.f);
	}
	glPopMatrix();

	glDisable(GL_LIGHT3);

	glFlush();
}

static void Resize(GLFWwindow* window, int w, int h) {
	if (h < 1)
		h = 1;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (float)w / (float)h, 0.1f, 1000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

static void MouseClickCallback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS){
		pMassCube->push_back(meCube());
	}
}

void ShowFPS(GLFWwindow* window) {
	crntTime = glfwGetTime();
	timeDiff = crntTime - prevTime;
	counter++;

	if (timeDiff >= 1.0 / 60.0) {
		s = "FPS: " + to_string((1.0 / timeDiff) * counter);
		str = new char[s.length() + 1];
		strcpy(str, s.c_str());
		glfwSetWindowTitle(window, str);
		prevTime = crntTime;
		counter = 0;
	}
}

int main(int argc, char** argv) {
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(640, 480, "title", NULL, NULL);
	if (window != NULL) {
		glfwMakeContextCurrent(window);
		Initialize();
		glfwSetWindowSizeCallback(window, Resize);
		glfwSetMouseButtonCallback(window, MouseClickCallback);
		while (!glfwWindowShouldClose(window)) {
			float delta = glfwGetTime();
			Update(window, delta);
			RenderScene(window, delta);
			ShowFPS(window);
			glfwSwapInterval(1);
			glfwSwapBuffers(window);
			glfwPollEvents();
		}
		neSimulator::DestroySimulator(pSimulator);
		pSimulator = 0;
		glfwDestroyWindow(window);
	}
	else {
		cout << "ERROR window = NULL" << endl;
		system("pause");
		glfwTerminate();
	}
	return 0;
}