#ifndef SCENE_H
#define SCENE_H

ESMatrix sceneMatrix;	// 投影举证 x lookat矩阵
GLint u_mvpMatrix;		// mvpMatrix的location

#include "object.h"

class Scene
{
private:
	GLuint program, vao;
	GLuint vs, fs;
public:
	Scene(){}

	// 说明 创建program，绑定shader,获取变换举证的location
	// 参数 顶点着色器的字符串
	// 参数 片段着色器的字符串
	// 参数 mvpMatrix的字符串
	int CreateProgram(const char *vStr, const char *fStr, const char *mvpMatrix)
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
		// 获得变换矩阵的全局location，给DrawObject使用
		u_mvpMatrix = glGetUniformLocation(program, mvpMatrix);
		return TRUE;
	}

	// 说明 绑定顶点数组对象，必须和SceneEnd配置使用，用于创建物体
	void SceneCreate() {
		glBindVertexArray(vao);
	}
	// 说明 绑定顶点数组对象，设置sceneMatrix,必须和SceneEnd()配合使用，用于绘画物体
	void SceneDraw() {
		glBindVertexArray(vao);
	}
	// 说明 解绑顶点数组对象
	void SceneEnd() {
		glBindVertexArray(0);
	}

	void LookAt(); 

private:
	// Shader
	GLuint LoadShader(GLenum type, const char *shaderSrc)
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
};

#endif
