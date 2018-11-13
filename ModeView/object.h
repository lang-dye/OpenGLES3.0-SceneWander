#ifndef OBJECT_H
#define OBJECT_H

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

class Object
{
private:
	// Program
	GLuint vbo, ibo;
	GLuint vs, fs;
	object_t *obj;

	// Model matrix
	float t[3];
	float s[3];
	float r[3];
public:
	Object();

	// Matrix
	void Translate(float x, float y, float z);
	void Scale(float x, float y, float z);
	void Rotate(float angleX, float angleY, float angleZ);

	// Create
	int CreateObject(const char* filename);

	// Draw
	void DrawObject();

	// Destroy
	void DestroyObject();

};

#endif
