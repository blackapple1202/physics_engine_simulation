#pragma once

#ifndef CYCLONE_PARTICLE_H
#define CYCLONE_PARTICLE_H

#include "core.h"

namespace cyclone {

	class Particle
	{
	protected :
		//������ ��ġ ���� ��ǥ��
		Vector3 position;

		//������ �ӵ� ���� ��ǥ��
		Vector3 velocity;

		//������ ���ӵ� ���� ��ǥ��
		Vector3 acceleration;

		//��ġ ��������� ���� ������ �������� �ٿ��ִ� ������ �Ѵ�.
		real damping;

		//������ ����
		real inverseMass;

		/*
		��ü�� �ۿ��ϴ� ������ ���� ���� �����ϴ� ����
		(�޶������� ����)
		�� ���� �ܰ迡�� (�� �����Ӹ���) 0���� ������ְ�
		�����Ϳ� �����Ӹ��� �շ��� �����־ ������Ʈ�ϴ� �����̴�.
		*/
		Vector3 forceAccum;

	public :

		//	���� (�� ������Ʈ�� �Լ�)
		void integrate(real duration);

		//	���� ���� �ʱ�ȭ�Ѵ�
		void setMass(const real mass);

		//	���� ���� �ҷ��´�
		real getMass() const;

		//	������ ���� ���� �ʱ�ȭ�Ѵ�
		void setInverseMass(const real inverseMass);

		//	������ ���� ���� �ҷ��´�
		real getInverseMass() const;

		//	������ ���Ѵ밡 �ƴ� ��� true ��ȯ
		bool hasFiniteMass() const;

		//	damping ���� �ʱ�ȭ�Ѵ�
		void setDamping(const real damping);

		//	damping ���� �ҷ��´�
		real getDamping() const;

		//	��ġ ���� �ʱ�ȭ�Ѵ�
		void setPosition(const Vector3 &position);

		//	��ġ ���� �ʱ�ȭ�Ѵ�
		void setPosition(const real x, const real y, const real z);

		//	��ġ ���� �����´�
		void getPosition(Vector3 *position) const;

		//	��ġ ���� �����´�
		Vector3 getPosition() const;

		//	�ӵ� ���� �ʱ�ȭ�Ѵ�
		void setVelocity(const Vector3 &velocity);

		//	�ӵ� ���� �ʱ�ȭ�Ѵ�
		void setVelocity(const real x, const real y, const real z);

		//	�ӵ� ���� �ҷ��´�
		void getVelocity(Vector3 *velocity) const;

		//	�ӵ� ���� �ҷ��´�
		Vector3 getVelocity() const;

		//	���ӵ� ���� �ʱ�ȭ�Ѵ�
		void setAcceleration(const Vector3 &acceleration);

		//	���ӵ� ���� �ʱ�ȭ�Ѵ�
		void setAcceleration(const real x, const real y, const real z);

		//	���ӵ� ���� �ҷ��´�
		void getAcceleration(Vector3 *acceleration) const;

		//	���ӵ� ���� �ҷ��´�
		Vector3 getAcceleration() const;

		//	���� force ���͸�ŭ ���Ѵ�
		void addForce(const Vector3 &force);

		//	�շ��� 0���� �ʱ�ȭ�Ѵ�.
		void clearAccumulator();
	};
}

#endif