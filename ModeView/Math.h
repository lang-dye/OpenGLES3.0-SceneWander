#ifndef MATH_H
#define MATH_H

class Vector2 
{
public:
	float x, y;
public:
	Vector2()                   { x = 0.0; y = 0.0; }
	Vector2(float nx)           { x = nx;  y = nx; }
	Vector2(float nx, float ny) { x = nx;  y = ny; }

	Vector2 operator+(const Vector2& add) const { return Vector2(this->x + add.x, this->y + add.y); }
	Vector2 operator-(const Vector2& sub) const { return Vector2(this->x - sub.x, this->y - sub.y); }
	Vector2 operator*(const Vector2& mul) const { return Vector2(this->x * mul.x, this->y * mul.y); }

	float dot(Vector2 a, Vector2 b){ return (a.x*b.x + a.y*b.y); }
};

class Vector3
{
public:
	float x, y, z;
public:
	Vector3()                             { x = 0.0; y = 0.0; z = 0.0; }
	Vector3(float nx)                     { x = nx;  y = nx;  z = nx; }
	Vector3(float nx, float ny, float nz) { x = nx;  y = ny;  z = nz; }

	Vector3 operator+(const Vector3& add) const { return Vector3(this->x + add.x, this->y + add.y, this->z + add.z); }
	Vector3 operator-(const Vector3& sub) const { return Vector3(this->x - sub.x, this->y - sub.y, this->z - sub.z); }
	Vector3 operator*(const Vector3& mul) const { return Vector3(this->x * mul.x, this->y * mul.y, this->z * mul.z); }

	float dot(Vector3 a, Vector3 b) { return (a.x*b.x + a.y*b.y + a.z*b.z); }
	Vector3 cross(Vector3 a, Vector3 b)
	{
		float nx = a.y*b.z - a.z*b.y;
		float ny = a.z*b.x - a.x*b.z;
		float nz = a.x*b.y - a.y*b.x;
		return Vector3(nx, ny, nz);
	}
};

class Vector4
{
public:
	float x, y, z, w;
	Vector3 xyz;
public:
	Vector4(float nx, float ny, float nz, float nw)
	{
		x = nx; y = ny; z = nz; w = nw;
		xyz = Vector3(nx, ny, nz);
	}
	Vector4(Vector3 a, float nw)
	{
		x = a.x; y = a.y; z = a.z;
		xyz = a;
	}
};

class Matrix
{
public:
	float m[4][4];
public:
	Vector4 operator*(const Vector4& vec4)
	{

	}
};

#endif
