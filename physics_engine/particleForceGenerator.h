#pragma once
#ifndef CYCLONE_PFGEN_H
#define CYCLONE_PFGEN_H

#include "core.h"
#include "particle.h"
#include <vector>

namespace cyclone {

	//	�ϳ� �Ǵ� �� �̻��� ���ڿ� ���� �����ֱ� ���� �� �߻���(Force Generator)�� ȣ���Ѵ�
	class ParticleForceGenerator
	{
	public:

		/*
		�־��� ���ڿ� �����ϴ� ���� ����ϰ� ������Ʈ �ϱ� ����, �� �޼ҵ带 ������ (�������� ����)
			�� �߻��� ������ ũ�� 4���� ������ �ִ�.
			1. ������ �� (�߷�)
			2. ��ü�� ���� �Ӽ�(��ġ, �ӵ� ��)�� ���� ����� ��
			3. ����� �Է¿� ���� �� (�ܷ�)
			4. �ð��� ���� ���ϴ� �� (������ ��)
		*/
		virtual void updateForce(Particle *particle, real duration) = 0;
	};

	//	�߷� �� �߻���
	class ParticleGravity : public ParticleForceGenerator
	{
		// �߷¿� ���� ���ӵ� �� ���� (g = 9.8 m/s)
		Vector3 gravity;

	public:

		//	������
		ParticleGravity(const Vector3 &gravity);

		//	�߷��� ���ڿ� ����
		virtual void updateForce(Particle *particle, real duration);
	};

	// ���׷� �� �߻��� (���׷��� ��ü�� �ӵ��� ���� �ۿ��ϴ� ��) => ******��������*********
	class ParticleDrag : public ParticleForceGenerator
	{
		//	�ӵ� ���� ����ϴ� k1 ���
		real k1;

		//	�ӵ��� ���� ���� ����ϴ� k2 ���
		real k2;

	public:

		//	���׷� ������
		ParticleDrag(real k1, real k2);

		//	������ ���ڿ� ����
		virtual void updateForce(Particle *particle, real duration);
	};


	/*
		������ ���� �����ϴ� �� �߻���
	*/
	class ParticleSpring : public ParticleForceGenerator
	{
		//	������ �ݴ��� ���� ����
		Particle *other;

		//	������ ź�����
		real springConstant;

		//	�������� ��������
		real restLength;

	public:

		//	������
		ParticleSpring(Particle *other,	real springConstant, real restLength);

		//	�־��� ���ڿ� ź������ �����Ѵ�.
		virtual void updateForce(Particle *particle, real duration);
	};



	/*
		������ �������� ���� �����ϴ� ���� �߻���
	*/
	class ParticleAnchoredSpring : public ParticleForceGenerator
	{
	protected:
		//	������ ������ ��ġ
		Vector3 *anchor;

		//	������ ���
		real springConstant;

		//	������ ���� ����
		real restLength;

	public:
		ParticleAnchoredSpring();

		//������
		ParticleAnchoredSpring(Vector3 *anchor,	real springConstant,real restLength);

		//������ ������ ��ġ ���� �������� �Լ�
		const Vector3* getAnchor() const { return anchor; }

		//�������� �Ӽ� ����
		void init(Vector3 *anchor,real springConstant,real restLength);

		//ź������ ���ڿ� ����
		virtual void updateForce(Particle *particle, real duration);
	};

	/*
		��ƴÿ��� ���� ������ ���� �����ϴ� �� �߻���
	*/
	class ParticleBungee : public ParticleForceGenerator
	{
		//	�������� �ݴ��� ���� �ִ� ����
		Particle *other;

		//	������ ź�� ���
		real springConstant;

		//	��������
		real restLength;

	public:

		//	������
		ParticleBungee(Particle *other,	real springConstant, real restLength);

		//	ź������ ���ڿ� ����
		virtual void updateForce(Particle *particle, real duration);
	};


	/*
		������ ������ ��ƴÿ��� ���� ������ ���� �����ϴ� �� �߻���
	*/
	class ParticleAnchoredBungee : public ParticleAnchoredSpring
	{
	public:
		//ź������ ���ڿ� ����
		virtual void updateForce(Particle *particle, real duration);
	};

	/*
		������ ������ ���� �䳻���� �� �߻���
		���� ���� �������� ����Ǿ� �ִ�
	*/
	class ParticleFakeSpring : public ParticleForceGenerator
	{
		//	�������� ������ ���� ��ġ
		Vector3 *anchor;

		//	������ ź�����
		real springConstant;

		//	������ ������ ����
		real damping;

	public:

		//	������
		ParticleFakeSpring(Vector3 *anchor, real springConstant,
			real damping);

		//	ź������ ���ڿ� ����
		virtual void updateForce(Particle *particle, real duration);
	};

	

	

	/*
		xz��鿡 ������ ���鿡 ���� �η��� �����ϴ�
		�� �߻���
	*/
	class ParticleBuoyancy : public ParticleForceGenerator
	{
		//	�ִ� �η��� �߻��ϱ� �� ��ü�� �ִ� ħ�� ����
		real maxDepth;

		//	��ü�� ����
		real volume;

		//	������ y = 0 ���κ��� �̵��� ����
		//	������ xz ��鿡 ����
		real waterHeight;

		//	��ü�� �е�
		//	������ ���� �е���
		//	100 kg/m^-3
		real liquidDensity;

	public:

		//	������
		ParticleBuoyancy(real maxDepth, real volume, real waterHeight,real liquidDensity = 1000.0f);

		//	�η��� ���ڿ� ����
		virtual void updateForce(Particle *particle, real duration);
	};

	// �� �߻���� �����ϴ� ��� ������ ��� ����� �����Ѵ� (std::vector ���)
	class ParticleForceRegistry
	{
	protected:

		//	�ϳ��� �� ������ ���� ������ ���ڸ� ����
		struct ParticleForceRegistration
		{
			//����
			Particle *particle;

			//�� �߻���
			ParticleForceGenerator *fg;
		};

		//	registrations��� ������� ����
		typedef std::vector<ParticleForceRegistration> Registry;
		Registry registrations;

	public:
		//	�־��� �� �߻��⸦ �־��� ���ڿ� ���
		void add(Particle* particle, ParticleForceGenerator *fg);

		/*
			�־��� ��ϵ� ���� ��Ͽ��� �����Ѵ�.
			�ش� ���� ��ϵ��� �ʾҴٸ�,
			�� �޼ҵ�� �ƹ��� ������ ��ġ�� �ʴ´�.
		*/
		void remove(Particle* particle, ParticleForceGenerator *fg);

		/*
			��� ��� �׸��� �����Ѵ�.
			���ڳ� �� �߻��� ��ü�� �������� �ʰ�
			�� ���Ἲ�� �����Ѵ�
		*/
		void clear();

		/*
			�ش��ϴ� ������ ���� ������Ʈ�ϵ��� ��� �� �߻��⸦ call�ϰ� �����Ѵ�.
		*/
		void updateForces(real duration);
	};
}

#endif // CYCLONE_PFGEN_H

