#include "scene.h"
#include "esUtil.h"
#include <cstdio>

static GLuint LoadShader(GLenum type, const char *shaderSrc)
{
	GLuint shader;
	GLint compiled;

	// Create the shader object
	shader = glCreateShader(type);
	if (shader == 0) return 0;

	// Load the shader source
	glShaderSource(shader, 1, &shaderSrc, NULL);
	// compile the shader
	glCompileShader(shader);
	// check the compile status
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		GLint infoLen = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
		if (!(infoLen > 1))
		{
			char *infoLog = (char*)malloc(sizeof(char) * infoLen);
			glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
			esLogMessage("Error compiling shader:\n%s\n", infoLog);
			free(infoLog);
		}
		glDeleteShader(shader);
		return 0;
	}
	return shader;
}

Scene::Scene(){}

int Scene::CreateProgram(const char *vStr, const char *fStr)
{
	program = glCreateProgram();
	if (!program) {
		fprintf(stderr, "Error create program!\n");
		return FALSE;
	}
	vs = LoadShader(GL_VERTEX_SHADER, vStr);
	fs = LoadShader(GL_FRAGMENT_SHADER, fStr);
	glAttachShader(program, vs);
	glAttachShader(program, fs);

	GLint linked;
	glLinkProgram(program);								// Link the program
	glGetProgramiv(program, GL_LINK_STATUS, &linked);	// Check the link status
	if (!linked)
	{
		GLint infoLen = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLen);
		if (infoLen > 1)
		{
			char *infoLog = (char*)malloc(sizeof(char) * infoLen);
			glGetProgramInfoLog(program, infoLen, NULL, infoLog);
			esLogMessage("Error linking program:\n%s\n", infoLog);
			free(infoLog);
		}
		glDeleteProgram(program);
		return FALSE;
	}

	// 生成顶点数组
	glGenVertexArrays(1, &vao);
	return TRUE;
}

void Scene::SceneBegin() {
	glBindVertexArray(vao);
}

void Scene::SceneEnd() {
	glBindVertexArray(0);
}

void Scene::SceneDestroy() {
	glDeleteProgram(program);
	glDeleteVertexArrays(1, &vao);
	glDeleteShader(vs);
	glDeleteShader(fs);
}


