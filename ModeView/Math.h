#ifndef MATH_H
#define MATH_H

#include <cmath>

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
// ���캯��
	// Ĭ�Ϲ��캯������ִ���κβ���
	Vector3(){}
	// ���ƹ��캯��
	Vector3(const Vector3 &a) : x(a.x), y(a.y), z(a.z) {}
	// �������Ĺ��캯����������ֵ��ɳ�ʼ��
	Vector3(float nx, float ny, float nz) : x(nx), y(ny), z(nz) {}

// ��׼�������
	// ���C����ϰ�ߣ����ظ�������������������ã���ʵ����ֵ
	Vector3 &operator =(const Vector3 &a) {
		x = a.x; y = a.y; z = a.z;
		return *this;
	}
	// ���ء�==��������
	bool operator ==(const Vector3 &a) { return x == a.x && y == a.y && z == a.z; }
	bool operator !=(const Vector3 &a) { return x != a.x && y != a.y && z != a.z; }
// ��������
	// ��Ϊ������
	void zero() { x = y = z = 0.0f; }
	// ����һԪ��-������
	Vector3 operator -() const { return Vector3(-x, -y, -z); }
	// ���ض�Ԫ��+���͡�-�������
	Vector3 operator +(const Vector3 &a) { return Vector3(x + a.x, y + a.y, z + a.z); }
	Vector3 operator -(const Vector3 &a) { return Vector3(x - a.x, y - a.y, z - a.z); }
	// ������ĳ˳���
	Vector3 operator *(float a) const { return Vector3(x * a, y * a, z * a); }
	Vector3 operator /(float a) const { return Vector3(x / a, y / a, z / a); }
	// �����Է�����
	Vector3 &operator +=(const Vector3 &a) {
		x += a.x; y += a.y; z += a.z;
		return *this;
	}
	Vector3 &operator -=(const Vector3 &a) {
		x -= a.x; y -= a.y; z -= a.z;
		return *this;
	}
	Vector3 &operator *=(float a) {
		x *= a; y *= a; z *= a;
		return *this;
	}
	Vector3 &operator /=(float a) {
		x /= a; y /= a; z /= a;
		return *this;
	}
	// ������׼��
	void normalize() {
		float magSq = x * x + y * y + z * z;
		if (magSq > 0) {
			float oneOverMag = 1 / sqrt(magSq);
			x *= oneOverMag;
			y *= oneOverMag;
			z *= oneOverMag;
		}
	}

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
