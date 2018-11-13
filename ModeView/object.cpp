#include "object.h"
#include "esUtil.h"
#include <cstdio>
#include <cstdlib>

static int LoadObject(const char* filename, object_t **obj)	// 只能读取三角片面
{
	FILE* file;
	fopen_s(&file, filename, "rt");
	if (!file) return 0;

	// 第一次读取，统计顶点的总数、面的总数
	char line[1000];
	unsigned int np = 0, nn = 0, nt = 0, nf = 0;
	while (!feof(file)) {
		fgets(line, 1000, file);	// fgets读取一行，以‘\0’为结束	*‘\0’为字符串的结束符*
		if (line[0] == '#') continue;
		if (line[0] == 'v') {
			if (line[1] == ' ') { np++; continue; }
			if (line[1] == 'n') { nn++; continue; }
			if (line[1] == 't') { nt++; continue; }
		}
		if (line[0] == 'f' && line[1] == ' ') { nf++; continue; }
	}

	// 分配存储空间
	*obj = (object_t*)malloc(sizeof(object_t));
	object_t *object = *obj;
	object->indices = (unsigned short*)malloc(nf * 3 * sizeof(unsigned short));
	object->vertices = (vertex_t*)malloc(nf * 3 * sizeof(vertex_t));
	object->vertexCnt = 3 * nf;

	// 第二次读取，存储顶点和面数据
	float *position = (float*)malloc(np * 3 * sizeof(float));
	float *normal = (float*)malloc(nn * 3 * sizeof(float));
	float *texture = (float*)malloc(nt * 2 * sizeof(float));

	fseek(file, 0, SEEK_SET);				// 将文件的指针移到开头
	int ip = 0, in = 0, it = 0, iv = 0;
	while (!feof(file)) {
		fgets(line, 1000, file);
		if (line[0] == '#') continue;

		if (line[0] == 'v') {
			// 存储顶点数据
			if (line[1] == ' ') {
				float *p = position + ip;
				char *e1, *e2;
				p[0] = (float)strtod(line + 2, &e1);	// strtod从line+2开始将字符串转换成double,将碰到的第一个非数字由e1传回；
				p[1] = (float)strtod(e1, &e2);
				p[2] = (float)strtod(e2, NULL);
				ip += 3;
				continue;
			}
			// 存储法线数据
			if (line[1] == 'n') {
				float *n = normal + in;
				char *e1, *e2;
				n[0] = (float)strtod(line + 2, &e1);
				n[1] = (float)strtod(e1, &e2);
				n[2] = (float)strtod(e2, NULL);
				in += 3;
				continue;
			}
			// 存储贴图坐标
			if (line[1] == 't') {
				float *t = texture + it;
				char *e;
				t[0] = (float)strtod(line + 2, &e);
				t[1] = (float)strtod(e, NULL);
				it += 2;
				continue;
			}
		}

		if (line[0] == 'f' && line[1] == ' ') {
			// 重新排序索引
			char *e1, *e2, *e3 = line + 1;
			for (int i = 0; i < 3; i++) {
				float *vp = (object->vertices + iv)->p;
				int ivp = (int)strtoul(e3 + 1, &e1, 10) - 1;		// e + 1跳过‘\’
				vp[0] = position[ivp * 3];
				vp[1] = position[ivp * 3 + 1];
				vp[2] = position[ivp * 3 + 2];

				float *vt = (object->vertices + iv)->t;
				int ivt = (int)strtoul(e1 + 1, &e2, 10) - 1;
				vt[0] = texture[ivt * 2];
				vt[1] = texture[ivt * 2 + 1];

				float *vn = (object->vertices + iv)->n;
				int ivn = (int)strtoul(e2 + 1, &e3, 10) - 1;
				vn[0] = normal[ivn * 3];
				vn[1] = normal[ivn * 3 + 1];
				vn[2] = normal[ivn * 3 + 2];

				// indices的索引
				*(object->indices + iv) = (unsigned short)iv;
				iv++;
			}
			continue;
		}
	}

	// 释放内存
	free(position);
	free(normal);
	free(texture);
	fclose(file);

	return 1;
}

Object::Object() {}
// Matrix
void Object::Translate(float x, float y, float z) {
	t[0] += x; t[1] += y; t[2] += z;
}
void Object::Scale(float x, float y, float z) {
	s[0] = x; s[1] = y; s[2] = z;
}
void Object::Rotate(float angleX, float angleY, float angleZ) {
	r[0] += angleX; r[1] += angleY; r[2] += angleZ;
}
// Object
int Object::CreateObject(const char* filename)
{
	if (!LoadObject(filename, &obj)) {
		fprintf(stderr, "Error loading file!\n");
		return 0;
	}

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, obj->vertexCnt * sizeof(vertex_t), obj->vertices, GL_STATIC_DRAW);
	free(obj->vertices);

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, obj->vertexCnt * sizeof(unsigned short), obj->indices, GL_STATIC_DRAW);
	free(obj->indices);
	free(obj);

	t[0] = 0.0f;   t[1] = 0.0f;   t[2] = 0.0f;
	s[0] = 1.0f;   s[1] = 1.0f;   s[2] = 1.0f;
	r[0] = 0.0f;   r[1] = 0.0f;   r[2] = 0.0f;
	return 1;
}
// Draw
void Object::DrawObject()
{
	// 绑定顶点数据
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)offsetof(vertex_t, p));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)offsetof(vertex_t, n));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)offsetof(vertex_t, t));

	glDrawElements(GL_TRIANGLES, obj->vertexCnt, GL_UNSIGNED_SHORT, (void*)0);
}
// Destroy
void Object::DestroyObject()
{
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
}