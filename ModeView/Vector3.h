// copy  《3D数学基础：图形与游戏开发》

#ifndef VECTOR3_H
#define VECTOR3_H

#include <cmath>

class Vector3
{
public:
	float x, y, z;
// 构造函数
	// 默认构造函数，不执行任何操作
	Vector3(){}
	// 复制构造函数
	Vector3(const Vector3 &a) : x(a.x), y(a.y), z(a.z) {}
	// 带参数的构造函数，用三个值完成初始化
	Vector3(float nx, float ny, float nz) : x(nx), y(ny), z(nz) {}

// 标准对象操作
	// 坚持C语言习惯，重载复制运算符，并返回引用，以实现左值
	Vector3 &operator =(const Vector3 &a) {
		x = a.x; y = a.y; z = a.z;
		return *this;
	}
	// 重载“==”操作符
	bool operator ==(const Vector3 &a) { return x == a.x && y == a.y && z == a.z; }
	bool operator !=(const Vector3 &a) { return x != a.x && y != a.y && z != a.z; }
// 向量运算
	// 置为零向量
	void zero() { x = y = z = 0.0f; }
	// 重载一元“-”运算
	Vector3 operator -() const { return Vector3(-x, -y, -z); }
	// 重载二元“+”和“-”运算符
	Vector3 operator +(const Vector3 &a) { return Vector3(x + a.x, y + a.y, z + a.z); }
	Vector3 operator -(const Vector3 &a) { return Vector3(x - a.x, y - a.y, z - a.z); }
	// 与标量的乘除法
	Vector3 operator *(float a) const { return Vector3(x * a, y * a, z * a); }
	Vector3 operator /(float a) const { return Vector3(x / a, y / a, z / a); }
	// 重载自反运算
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
	// 向量标准化
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

// 向量求模
inline float vectorMag(const Vector3 &a) {
	return sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
}
// 计算两向量的点乘
inline float dot(const Vector3 &a, const Vector3 &b) {
	return a.x*b.x + a.y*b.y + a.z*b.z;
}
// 计算两向量的叉乘
inline Vector3 cross(const Vector3 &a, const Vector3 &b) {
	return Vector3(
		a.y*b.z - a.z*b.y,
		a.z*b.x - a.x*b.z,
		a.x*b.y - a.y*b.x
	);
}
// 实现标量左乘
inline Vector3 operator *(float k, const Vector3 &a) {
	return Vector3(k*a.x, k*a.y, k*a.z);
}
// 计算两点间的距离
inline float distance(const Vector3 &a, const Vector3 &b) {
	float dx = a.x - b.x;
	float dy = a.y - b.y;
	float dz = a.z - b.z;
	return sqrt(dx*dx + dy * dy + dz * dz);
}

// 提供一个全局零向量
extern const Vector3 ZeroVector3;
#endif
