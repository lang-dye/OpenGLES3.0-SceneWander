#ifndef CAMERA_H
#define CAMERA_H

class Camera
{
private:
	float pos[3], lookAt[3], up[3];		// lookAt,upÎªÏòÁ¿
public:
	Camera();
	void position(float posX, float posY, float posZ);
	void lookAtDir(float lookAtX, float lookAtY, float lookAtZ);
	void upDir(float upX, float upY, float upZ);
};

#endif

