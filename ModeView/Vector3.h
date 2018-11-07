// copy  ��3D��ѧ������ͼ������Ϸ������

#ifndef VECTOR3_H
#define VECTOR3_H

#include <cmath>

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

// ������ģ
inline float vectorMag(const Vector3 &a) {
	return sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
}
// �����������ĵ��
inline float dot(const Vector3 &a, const Vector3 &b) {
	return a.x*b.x + a.y*b.y + a.z*b.z;
}
// �����������Ĳ��
inline Vector3 cross(const Vector3 &a, const Vector3 &b) {
	return Vector3(
		a.y*b.z - a.z*b.y,
		a.z*b.x - a.x*b.z,
		a.x*b.y - a.y*b.x
	);
}
// ʵ�ֱ������
inline Vector3 operator *(float k, const Vector3 &a) {
	return Vector3(k*a.x, k*a.y, k*a.z);
}
// ���������ľ���
inline float distance(const Vector3 &a, const Vector3 &b) {
	float dx = a.x - b.x;
	float dy = a.y - b.y;
	float dz = a.z - b.z;
	return sqrt(dx*dx + dy * dy + dz * dz);
}

// �ṩһ��ȫ��������
extern const Vector3 ZeroVector3;
#endif
