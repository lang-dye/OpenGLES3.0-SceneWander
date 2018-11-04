#include <cstring>
#include <cstdlib>

#include "esUtil.h"
#include "LoadFile.h"
#include "Math.h"

extern "C" {int esMain(ESContext *esContext);}

typedef struct {
	GLint u_LightColor;
	GLint u_LightDir;
	GLint u_AmbientColor;
} light_t;

typedef struct {
	GLuint program;
	GLuint vao;
	GLuint vbo[2];

	GLint u_mvpMatrix;

	light_t *light;
	scene_t *scene;

	// shadowmap
	GLuint shadowProgram;
	GLuint shadowFb;
	GLuint shadowTex;
} UserData;

GLfloat tx = -50.0f, ty = 0.0f, tz = 0.0f;
GLfloat sx = 1.0f, sy = 1.0f, sz = 1.0f;
GLfloat angle = 30.0f, x = 0.0f, y = 0.0f, z = 0.0f;
float posX = 0.0f, posY = 0.0f, posZ = 500.0f;
float lookAtX = 0.0f, lookAtY = 0.0f, lookAtZ = 1.0f;
float upX = 0.0f, upY = 1.0f, upZ = 0.0f;

ESMatrix GetMatrix(ESContext *esContext)
{
	UserData *userData = (UserData*)esContext->userData;
	ESMatrix model, view, projection;
	ESMatrix modelview, mvpMatrix;

	float aspect = esContext->width / esContext->height;
	esMatrixLoadIdentity(&projection);
	esPerspective(&projection, 60.0f, aspect, 1.0f, 100.0f);

	esMatrixLoadIdentity(&model);
	esTranslate(&model, tx, -1.0f, 0.0f);
	esRotate(&model, angle, 0.0, 1.0f, 0.0);
	esScale(&model, 0.02f, 0.02f, 0.02f);

	esMatrixLookAt(&view, 0.0f, 0.0f, 50.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);

	esMatrixMultiply(&modelview, &model, &view);
	esMatrixMultiply(&mvpMatrix, &modelview, &projection);

	return mvpMatrix;
}

int Init(ESContext *esContext)
{
	UserData *userData = (UserData*)esContext->userData;

	const char *filename = "C:\\Users\\lang\\Desktop\\text.obj";
	scene_t *scene = LoadObject(filename);
	glGenVertexArrays(1, &userData->vao);
	glBindVertexArray(userData->vao);

	glGenBuffers(2, userData->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, userData->vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, scene->vertexCnt * sizeof(vertex_t), scene->vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, userData->vbo[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, scene->vertexCnt * sizeof(unsigned short), scene->indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)offsetof(vertex_t, p));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)offsetof(vertex_t, n));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)offsetof(vertex_t, t));

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

	GLuint program = esLoadProgram(vs, fs);
	if (!program) {
		fprintf(stderr, "Error loading shader!\n");
		return GL_FALSE;
	}
	light_t *light = (light_t*)malloc(sizeof(light_t));
	light->u_AmbientColor = glGetUniformLocation(program, "_AmbientColor");
	light->u_LightColor = glGetUniformLocation(program, "_LightColor");
	light->u_LightDir = glGetUniformLocation(program, "_LightDir");
	userData->u_mvpMatrix = glGetUniformLocation(program, "u_mvpMatrix");

	userData->scene = scene;
	userData->program = program;
	userData->light = light;

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	return GL_TRUE;
}

void Draw(ESContext *esContext)
{
	UserData *userData = (UserData*)esContext->userData;
	light_t *light = userData->light;
	scene_t *scene = userData->scene;

	glViewport(0, 0, esContext->width, esContext->height);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	static float ambientColor[3] = { 0.4f, 0.4f, 0.4f };
	static float lightColor[3] = { 0.8f, 0.8f, 0.8f };
	static float lightDir[3] = { 1.0f, 2.0f, 0.0f };

	glUseProgram(userData->program);
	glUniform3fv(light->u_AmbientColor, 1, ambientColor);
	glUniform3fv(light->u_LightColor, 1, lightColor);
	glUniform3fv(light->u_LightDir, 1, lightDir);
	glUniformMatrix4fv(userData->u_mvpMatrix, 1, GL_FALSE, &GetMatrix(esContext).m[0][0]);

	glBindVertexArray(userData->vao);
	glDrawElements(GL_TRIANGLES, scene->vertexCnt, GL_UNSIGNED_SHORT, (void*)0);
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
	glDeleteProgram(userData->program);
	glDeleteBuffers(2, userData->vbo);
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