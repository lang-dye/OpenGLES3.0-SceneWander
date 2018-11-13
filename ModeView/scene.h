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

	// ˵�� ����program����shader,��ȡ�任��֤��location
	// ���� ������ɫ�����ַ���
	// ���� Ƭ����ɫ�����ַ���
	int CreateProgram(const char *vStr, const char *fStr);

	// ˵�� �󶨶���������󣬱����SceneEnd����ʹ�ã����ڴ�������
	void SceneBegin();
	// ˵�� ��󶥵��������
	void SceneEnd();

	void SceneDestroy();
};

#endif
