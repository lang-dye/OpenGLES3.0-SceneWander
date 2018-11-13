#include "camera.h"

Camera::Camera(){}

void Camera::position(float posX, float posY, float posZ)
{
	pos[0] = posX; pos[1] = posY; pos[2] = posZ;
}

void Camera::lookAtDir(float lookAtX, float lookAtY, float lookAtZ)
{
	lookAt[0] = lookAtX; lookAt[1] = lookAtY; lookAt[2] = lookAtZ;
}

void Camera::upDir(float upX, float upY, float upZ)
{
	up[0] = upX; up[1] = upY; up[2] = upZ;
}