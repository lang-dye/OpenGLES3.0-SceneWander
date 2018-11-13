#ifndef SCENE_H
#define SCENE_H

class Scene
{
public:
	GLuint program;
private:
	GLuint vao;
	GLuint vs, fs;
public:
	Scene();

	// 说明 创建program，绑定shader,获取变换举证的location
	// 参数 顶点着色器的字符串
	// 参数 片段着色器的字符串
	int CreateProgram(const char *vStr, const char *fStr);

	// 说明 绑定顶点数组对象，必须和SceneEnd配置使用，用于创建物体
	void SceneBegin();
	// 说明 解绑顶点数组对象
	void SceneEnd();

	void SceneDestroy();
};

#endif
