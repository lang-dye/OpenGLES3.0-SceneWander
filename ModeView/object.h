#ifndef OBJECT_H
#define OBJECT_H

#include <cstdio>
#include <cstdlib>

typedef struct {
	float p[3];
	float n[3];
	float t[2];
} vertex_t;

typedef struct {
	vertex_t       *vertices;    
	unsigned short *indices;	  
	unsigned int    vertexCnt;     
} object_t;

static int LoadObject(const char* filename, object_t **obj)	// 只能读取三角片面
{
	FILE* file;
	fopen_s(&file, filename, "rt");
	if (!file) return FALSE;

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

	return TRUE;
}

class Object
{
private:
	// Program
	GLuint vbo, ibo;
	GLuint vs, fs;
	object_t *obj;

	// Matrix
	ESMatrix model;
	ESMatrix mvpMatrix;
	float tx, ty, tz;
	float sx, sy, sz;
	float rx, ry, rz;
public:
	Object() {}

	// Matrix
	void Translate(float x, float y, float z) {
		tx = x; ty = y; tz = z;
	}
	void Scale(float x, float y, float z) {
		sx = x; sy = y; sz = z;
	}
	void Rotate(float angleX, float angleY, float angleZ) {
		rx = angleX; ry = angleY; rz = angleZ;
	}

	// Object
	int CreateObject(const char* filename)
	{
		if (!LoadObject(filename, &obj)) {
			fprintf(stderr, "Error loading file!\n");
			return FALSE;
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

		tx = 0.0f; ty = 0.0f; tz = 0.0f;
		sx = 1.0f; sy = 1.0f; sz = 1.0f;
		rx = 0.0f; ry = 0.0f; rz = 0.0f;
		return TRUE;
	}

	// Draw
	void DrawObject()
	{
		esMatrixLoadIdentity(&model);
		esTranslate(&model, tx, ty, tz);
		esScale(&model, sx, sy, sz);
		esRotate(&model, rx, 1.0f, 0.0f, 0.0f);
		esRotate(&model, ry, 0.0f, 1.0f, 0.0f);
		esRotate(&model, rz, 0.0f, 0.0f, 1.0f);

		esMatrixMultiply(&mvpMatrix, &sceneMatrix, &model);
		glUniformMatrix4fv(u_mvpMatrix, 1, false, &mvpMatrix.m[0][0]);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)offsetof(vertex_t, p));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)offsetof(vertex_t, n));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)offsetof(vertex_t, t));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

		glDrawElements(GL_TRIANGLES, obj->vertexCnt, GL_UNSIGNED_SHORT, (void*)0);
	}

	// Destroy
	void DestroyObject()
	{
		glDeleteBuffers(1, &vbo);
		glDeleteBuffers(1, &ibo);
	}

};

#endif
