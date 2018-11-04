#ifndef LOAD_FILE
#define LOAD_FILE

#include <stdio.h>
#include <stdlib.h>

typedef struct {
	float p[3];
	float n[3];
	float t[2];
} vertex_t;

typedef struct {
	vertex_t       *vertices;      // 顶点的索引
	unsigned short *indices;	   // 面中顶点位置的索引，法线和贴图的索引和位置的索引相同
	unsigned int    vertexCnt;     // 顶点的总数
} scene_t;

//scene_t *LoadObject(char* filename);

scene_t *LoadObject(const char* filename)	// 只能读取三角片面
{
	// 打开文件
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
	scene_t *scene = (scene_t*)malloc(sizeof(scene_t));
	scene->indices = (unsigned short*)malloc(nf * 3 * sizeof(unsigned short));
	scene->vertices = (vertex_t*)malloc(nf * 3 * sizeof(vertex_t));
	scene->vertexCnt = 3 * nf;

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
				float *vp = (scene->vertices + iv)->p;
				int ivp = (int)strtoul(e3 + 1, &e1, 10) - 1;		// e + 1跳过‘\’
				vp[0] = position[ivp * 3];
				vp[1] = position[ivp * 3 + 1];
				vp[2] = position[ivp * 3 + 2];

				float *vt = (scene->vertices + iv)->t;
				int ivt = (int)strtoul(e1 + 1, &e2, 10) - 1;
				vt[0] = texture[ivt * 2];
				vt[1] = texture[ivt * 2 + 1];

				float *vn = (scene->vertices + iv)->n;
				int ivn = (int)strtoul(e2 + 1, &e3, 10) - 1;
				vn[0] = normal[ivn * 3];
				vn[1] = normal[ivn * 3 + 1];
				vn[2] = normal[ivn * 3 + 2];
				*(scene->indices + iv) = (unsigned short)iv;	  // indices的索引
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

	return scene;
}

#endif
