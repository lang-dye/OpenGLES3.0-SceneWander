#include <cstring>
#include <cstdlib>
#include <iostream>

#include "esUtil.h"
#include "scene.h"


extern "C" {int esMain(ESContext *esContext);}

typedef struct {
	GLint u_LightColor;
	GLint u_LightDir;
	GLint u_AmbientColor;
} light_t;

typedef struct {
	Scene *scene;
	Object *teapot;
	light_t *light;
} UserData;

int Init(ESContext *esContext)
{
	UserData *userData = (UserData*)esContext->userData;
	userData->scene = (Scene*)malloc(sizeof(Scene));
	userData->teapot = (Object*)malloc(sizeof(Object));
	userData->light = (light_t*)malloc(sizeof(light_t));
	const char *filename = "C:\\Users\\lang\\Desktop\\text.obj";
	float aspect = esContext->width / esContext->height;

	const char *vs =
		"#version 300 es\n"
		"layout(location = 0) in vec3 a_position;\n"
		"layout(location = 1) in vec3 a_normal;\n"
		"layout(location = 2) in vec2 a_texcoord;\n"

		"uniform vec3 _LightDir;\n"
		"uniform mat4 u_mvpMatrix;\n"

		"out float ndotl;\n"

		"void main()\n"
		"{\n"
		"    vec3 normal = normalize(a_normal);\n"
		"    vec3 lightDir = normalize(_LightDir);\n"

		"    ndotl = dot(normal, lightDir);\n"
		"    gl_Position = u_mvpMatrix * vec4(a_position, 1.0f);\n"
		"}\n";
	const char *fs =
		"#version 300 es\n"
		"precision mediump float;\n"
		"in float ndotl;\n"

		"uniform vec3 _AmbientColor;\n"
		"uniform vec3 _LightColor;\n"

		"out vec4 o_color;\n"

		"void main()\n"
		"{\n"
		"    vec3 albedo = ndotl * _LightColor;\n"
		"    o_color = vec4(_AmbientColor + albedo, 1.0f);\n"
		"}\n";

	Scene scene = *userData->scene;
	Object teapot = *userData->teapot;
	scene.CreateProgram(vs, fs, "u_mvpMatrix");

	scene.SceneCreate(60.0f, aspect, 0.0f, 0.0f, 10.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	teapot.CreateObject(filename, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	scene.SceneEnd();

	light_t light = *userData->light;
	light.u_AmbientColor = glGetUniformLocation(scene.program, "_AmbientColor");
	light.u_LightColor = glGetUniformLocation(scene.program, "_LightColor");
	light.u_LightDir = glGetUniformLocation(scene.program, "_LightDir");

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	return GL_TRUE;
}

void Draw(ESContext *esContext)
{
	UserData *userData = (UserData*)esContext->userData;
	light_t *light = userData->light;
	Object teapot = *userData->teapot;

	glViewport(0, 0, esContext->width, esContext->height);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	static float ambientColor[3] = { 0.4f, 0.4f, 0.4f };
	static float lightColor[3] = { 0.8f, 0.8f, 0.8f };
	static float lightDir[3] = { 1.0f, 2.0f, 0.0f };

	glUseProgram(teapot.program);
	glUniform3fv(light->u_AmbientColor, 1, ambientColor);
	glUniform3fv(light->u_LightColor, 1, lightColor);
	glUniform3fv(light->u_LightDir, 1, lightDir);
	glUniformMatrix4fv(userData->u_mvpMatrix, 1, GL_FALSE, &GetMatrix(esContext).m[0][0]);

	teapot.DrawObject();
}

void KeyInput(ESContext *esContext, unsigned char key, int x, int y)
{
	if (key == 'a') angle -= 5.0f;
	if (key == 'd') angle += 5.0f;
	if (key == 'z') tx += 10.0f;
	if (key == 'x') tx -= 10.0f;
}

void Update(ESContext *esContext, float a)
{
	//angle = angle + 100.0f*a;
}

void Shutdown(ESContext *esContext)
{
	UserData *userData = (UserData*)esContext->userData;
	Object teapot = *userData->teapot;
	teapot.Destroy();
}

int esMain(ESContext *esContext)
{
	esContext->userData = malloc(sizeof(UserData));
	esCreateWindow(esContext, "ModelView", 1280, 720, ES_WINDOW_RGB);
	if (!Init(esContext)) return GL_FALSE;

	esRegisterShutdownFunc(esContext, Shutdown);
	esRegisterKeyFunc(esContext, KeyInput);
	esRegisterUpdateFunc(esContext, Update);
	esRegisterDrawFunc(esContext, Draw);

	return GL_TRUE;
}