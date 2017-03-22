#pragma once
#include <math.h>
#ifndef CYCLONE_CORE_H
#define CYCLONE_CORE_H
#include "precision.h"

namespace cyclone {

	
	extern real sleepEpsilon;

	void setSleepEpsilon(real value);

	
	real getSleepEpsilon();



	//������ ��ǥ�� xyz
	class Vector3 {
	public :
		//���� x ��ǥ��
		real x;
		//���� y ��ǥ��
		real y;
		//���� z ��ǥ��
		real z;

	private :
		//���� ����� ���� 4��° ��
		real pad;

	public :
		//	���� ������
		Vector3() : x(0), y(0), z(0) {}
		Vector3(const real x, const real y, const real z) : x(x), y(y), z(z) {}
		
		const static Vector3 GRAVITY;
		const static Vector3 HIGH_GRAVITY;
		const static Vector3 UP;
		const static Vector3 RIGHT;
		const static Vector3 OUT_OF_SCREEN;
		const static Vector3 X;
		const static Vector3 Y;
		const static Vector3 Z;

		real operator[](unsigned i) const
		{
			if (i == 0) return x;
			if (i == 1) return y;
			return z;
		}

		real& operator[](unsigned i)
		{
			if (i == 0) return x;
			if (i == 1) return y;
			return z;
		}

		//��ǥ�� ������ �Լ� (x = -x)
		void invert() {
			x = -x;
			y = -y;
			z = -z;
		}

		//������ ũ�� ���ϴ� �Լ�
		real magnitude() const {
			return real_sqrt(x*x + y*y + z*z);
		}

		//������ ũ�� ���� ���ϴ� �Լ�
		real squareMagnitude() const {
			return x*x + y*y + z*z;
		}

		void operator+=(const Vector3 &v)
		{
			x += v.x;
			y += v.y;
			z += v.z;
		}

		//������ ��
		Vector3 operator+(const Vector3 &v) const
		{
			return Vector3(x + v.x, y + v.y, z + v.z);
		}

		void operator-=(const Vector3 &v)
		{
			x -= v.x;
			y -= v.y;
			z -= v.z;
		}

		//������ ����
		Vector3 operator-(const Vector3 &v) const
		{
			return Vector3(x - v.x, y - v.y, z - v.z);
		}

		void operator*=(const real value)
		{
			x *= value;
			y *= value;
			z *= value;
		}

		//��Į�� ��
		Vector3 operator*(const real value)
		{
			return Vector3(x*value, y*value, z*value);
		}

		//��Į�� ������ ���͸� ���ϴ� �Լ� ( x += (x' * scale))
		void addScaledVector(const Vector3& vector, real scale)
		{
			x += vector.x * scale;
			y += vector.y * scale;
			z += vector.z * scale;
		}

		//������ ���� ����
		Vector3 componentProduct(const Vector3 &vector) const {
			return Vector3(x * vector.x, y * vector.y, z * vector.z);
		}

		void componentProductUpdate(const Vector3 &vector) {
			x *= vector.x;
			y *= vector.y;
			z *= vector.z;
		}

		//������ ����
		real scalarProduct(const Vector3 &vector) const {
			return x * vector.x + y * vector.y + z * vector.z;
		}

		real operator *(const Vector3 &vector) {
			return x * vector.x + y * vector.y + z * vector.z;
		}

		//������ ����
		Vector3 vectorProduct(const Vector3 &vector) const {
			return Vector3(y * vector.z - z* vector.y, z*vector.x - x*vector.z, x*vector.y - y* vector.x);
		}

		void operator %=(const Vector3 &vector) {
			*this = vectorProduct(vector);
		}

		Vector3 operator%(const Vector3 &vector) const {
			return Vector3(y * vector.z - z* vector.y, z*vector.x - x*vector.z, x*vector.y - y* vector.x);
		}

		//�������� ���� (xyz ũ�Ⱑ 1�� ��ǥ���� ����� �Լ�)
		void makeOrthonormalBasis(Vector3 *a, Vector3 *b, Vector3 *c) {
			a->normalize();
			(*c) = (*a) % (*b);
			if (c->squareMagnitude() == 0.0f)
				return;
			c->normalize();
			(*b) = (*c) % (*a);
		}

		//����ȭ
		void normalize() {
			real d = magnitude();
			if (d > 0) {
				(*this) *= ((real) 1) / d;
			}
		}


		/** Limits the size of the vector to the given maximum. */
		void trim(real size)
		{
			if (squareMagnitude() > size*size)
			{
				normalize();
				x *= size;
				y *= size;
				z *= size;
			}
		}

		/** Returns the normalised version of a vector. */
		Vector3 unit() const
		{
			Vector3 result = *this;
			result.normalize();
			return result;
		}

		/** Checks if the two vectors have identical components. */
		bool operator==(const Vector3& other) const
		{
			return x == other.x &&
				y == other.y &&
				z == other.z;
		}

		/** Checks if the two vectors have non-identical components. */
		bool operator!=(const Vector3& other) const
		{
			return !(*this == other);
		}

		/**
		* Checks if this vector is component-by-component less than
		* the other.
		*
		* @note This does not behave like a single-value comparison:
		* !(a < b) does not imply (b >= a).
		*/
		bool operator<(const Vector3& other) const
		{
			return x < other.x && y < other.y && z < other.z;
		}

		/**
		* Checks if this vector is component-by-component less than
		* the other.
		*
		* @note This does not behave like a single-value comparison:
		* !(a < b) does not imply (b >= a).
		*/
		bool operator>(const Vector3& other) const
		{
			return x > other.x && y > other.y && z > other.z;
		}

		/**
		* Checks if this vector is component-by-component less than
		* the other.
		*
		* @note This does not behave like a single-value comparison:
		* !(a <= b) does not imply (b > a).
		*/
		bool operator<=(const Vector3& other) const
		{
			return x <= other.x && y <= other.y && z <= other.z;
		}

		/**
		* Checks if this vector is component-by-component less than
		* the other.
		*
		* @note This does not behave like a single-value comparison:
		* !(a <= b) does not imply (b > a).
		*/
		bool operator>=(const Vector3& other) const
		{
			return x >= other.x && y >= other.y && z >= other.z;
		}

		//x = y = z = 0
		void clear()
		{
			x = y = z = 0;
		}

	};
}
#endif