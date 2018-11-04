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
	vertex_t       *vertices;      // ���������
	unsigned short *indices;	   // ���ж���λ�õ����������ߺ���ͼ��������λ�õ�������ͬ
	unsigned int    vertexCnt;     // ���������
} scene_t;

//scene_t *LoadObject(char* filename);

scene_t *LoadObject(const char* filename)	// ֻ�ܶ�ȡ����Ƭ��
{
	// ���ļ�
	FILE* file;
	fopen_s(&file, filename, "rt");
	if (!file) return 0;

	// ��һ�ζ�ȡ��ͳ�ƶ�����������������
	char line[1000];
	unsigned int np = 0, nn = 0, nt = 0, nf = 0;
	while (!feof(file)) {
		fgets(line, 1000, file);	// fgets��ȡһ�У��ԡ�\0��Ϊ����	*��\0��Ϊ�ַ����Ľ�����*
		if (line[0] == '#') continue;
		if (line[0] == 'v') {
			if (line[1] == ' ') { np++; continue; }
			if (line[1] == 'n') { nn++; continue; }
			if (line[1] == 't') { nt++; continue; }
		}
		if (line[0] == 'f' && line[1] == ' ') { nf++; continue; }
	}

	// ����洢�ռ�
	scene_t *scene = (scene_t*)malloc(sizeof(scene_t));
	scene->indices = (unsigned short*)malloc(nf * 3 * sizeof(unsigned short));
	scene->vertices = (vertex_t*)malloc(nf * 3 * sizeof(vertex_t));
	scene->vertexCnt = 3 * nf;

	// �ڶ��ζ�ȡ���洢�����������
	float *position = (float*)malloc(np * 3 * sizeof(float));
	float *normal = (float*)malloc(nn * 3 * sizeof(float));
	float *texture = (float*)malloc(nt * 2 * sizeof(float));

	fseek(file, 0, SEEK_SET);				// ���ļ���ָ���Ƶ���ͷ
	int ip = 0, in = 0, it = 0, iv = 0;
	while (!feof(file)) {
		fgets(line, 1000, file);
		if (line[0] == '#') continue;

		if (line[0] == 'v') {
			// �洢��������
			if (line[1] == ' ') {
				float *p = position + ip;
				char *e1, *e2;
				p[0] = (float)strtod(line + 2, &e1);	// strtod��line+2��ʼ���ַ���ת����double,�������ĵ�һ����������e1���أ�
				p[1] = (float)strtod(e1, &e2);
				p[2] = (float)strtod(e2, NULL);
				ip += 3;
				continue;
			}
			// �洢��������
			if (line[1] == 'n') {
				float *n = normal + in;
				char *e1, *e2;
				n[0] = (float)strtod(line + 2, &e1);
				n[1] = (float)strtod(e1, &e2);
				n[2] = (float)strtod(e2, NULL);
				in += 3;
				continue;
			}
			// �洢��ͼ����
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

			// ������������
			char *e1, *e2, *e3 = line + 1;
			for (int i = 0; i < 3; i++) {
				float *vp = (scene->vertices + iv)->p;
				int ivp = (int)strtoul(e3 + 1, &e1, 10) - 1;		// e + 1������\��
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
				*(scene->indices + iv) = (unsigned short)iv;	  // indices������
				iv++;
			}
			continue;
		}
	}

	// �ͷ��ڴ�
	free(position);
	free(normal);
	free(texture);
	fclose(file);

	return scene;
}

#endif
