#pragma once
#ifndef CYCLONE_PFGEN_H
#define CYCLONE_PFGEN_H

#include "core.h"
#include "particle.h"
#include <vector>

namespace cyclone {

	//	하나 또는 그 이상의 입자에 힘을 더해주기 위한 힘 발생기(Force Generator)를 호출한다
	class ParticleForceGenerator
	{
	public:

		/*
		주어진 입자에 적용하는 힘을 계산하고 업데이트 하기 위해, 이 메소드를 재정의 (유형별로 정의)
			힘 발생기 종류는 크게 4가지 유형이 있다.
			1. 일정한 힘 (중력)
			2. 물체의 현재 속성(위치, 속도 등)에 따라 생기는 힘
			3. 사용자 입력에 의한 힘 (외력)
			4. 시간에 따라 변하는 힘 (스프링 등)
		*/
		virtual void updateForce(Particle *particle, real duration) = 0;
	};

	//	중력 힘 발생기
	class ParticleGravity : public ParticleForceGenerator
	{
		// 중력에 의한 가속도 값 저장 (g = 9.8 m/s)
		Vector3 gravity;

	public:

		//	생성자
		ParticleGravity(const Vector3 &gravity);

		//	중력을 입자에 적용
		virtual void updateForce(Particle *particle, real duration);
	};

	// 저항력 힘 발생기 (저항력은 물체의 속도에 따라 작용하는 힘) => ******공기저항*********
	class ParticleDrag : public ParticleForceGenerator
	{
		//	속도 값에 비례하는 k1 계수
		real k1;

		//	속도의 제곱 값에 비례하는 k2 계수
		real k2;

	public:

		//	저항력 생산자
		ParticleDrag(real k1, real k2);

		//	저항을 입자에 적용
		virtual void updateForce(Particle *particle, real duration);
	};


	/*
		스프링 힘을 적용하는 힘 발생기
	*/
	class ParticleSpring : public ParticleForceGenerator
	{
		//	스프링 반대쪽 끝의 입자
		Particle *other;

		//	스프링 탄성계수
		real springConstant;

		//	스프링의 휴지길이
		real restLength;

	public:

		//	생성자
		ParticleSpring(Particle *other,	real springConstant, real restLength);

		//	주어진 입자에 탄성력을 적용한다.
		virtual void updateForce(Particle *particle, real duration);
	};



	/*
		고정된 스프링의 힘을 적용하는 힘의 발생기
	*/
	class ParticleAnchoredSpring : public ParticleForceGenerator
	{
	protected:
		//	스프링 고정단 위치
		Vector3 *anchor;

		//	스프링 상수
		real springConstant;

		//	스프링 휴지 길이
		real restLength;

	public:
		ParticleAnchoredSpring();

		//생성자
		ParticleAnchoredSpring(Vector3 *anchor,	real springConstant,real restLength);

		//스프링 고정단 위치 값을 가져오는 함수
		const Vector3* getAnchor() const { return anchor; }

		//스프링의 속성 설정
		void init(Vector3 *anchor,real springConstant,real restLength);

		//탄성력을 입자에 적용
		virtual void updateForce(Particle *particle, real duration);
	};

	/*
		잡아늘였을 때만 스프링 힘을 적용하는 힘 발생기
	*/
	class ParticleBungee : public ParticleForceGenerator
	{
		//	스프링의 반대편에 끝에 있는 입자
		Particle *other;

		//	스프링 탄성 계수
		real springConstant;

		//	휴지길이
		real restLength;

	public:

		//	생성자
		ParticleBungee(Particle *other,	real springConstant, real restLength);

		//	탄성력을 입자에 적용
		virtual void updateForce(Particle *particle, real duration);
	};


	/*
		한쪽이 고정된 잡아늘였을 때만 스프링 힘을 적용하는 힘 발생기
	*/
	class ParticleAnchoredBungee : public ParticleAnchoredSpring
	{
	public:
		//탄성력을 입자에 적용
		virtual void updateForce(Particle *particle, real duration);
	};

	/*
		뻣뻣한 스프링 힘을 흉내내는 힘 발생기
		한쪽 끝은 고정점에 연결되어 있다
	*/
	class ParticleFakeSpring : public ParticleForceGenerator
	{
		//	스프링의 고정된 끝점 위치
		Vector3 *anchor;

		//	스프링 탄성계수
		real springConstant;

		//	스프링 진동의 댐핑
		real damping;

	public:

		//	생성자
		ParticleFakeSpring(Vector3 *anchor, real springConstant,
			real damping);

		//	탄성력을 입자에 적용
		virtual void updateForce(Particle *particle, real duration);
	};

	

	

	/*
		xz평면에 평행한 수면에 대해 부력을 적용하는
		힘 발생기
	*/
	class ParticleBuoyancy : public ParticleForceGenerator
	{
		//	최대 부력을 발생하기 전 개체의 최대 침수 깊이
		real maxDepth;

		//	물체의 부피
		real volume;

		//	수면이 y = 0 으로부터 이동한 높이
		//	수면은 xz 평면에 평행
		real waterHeight;

		//	액체의 밀도
		//	순수한 물의 밀도는
		//	100 kg/m^-3
		real liquidDensity;

	public:

		//	생성자
		ParticleBuoyancy(real maxDepth, real volume, real waterHeight,real liquidDensity = 1000.0f);

		//	부력을 입자에 적용
		virtual void updateForce(Particle *particle, real duration);
	};

	// 힘 발생기와 적용하는 대상 입자의 모든 목록을 저장한다 (std::vector 사용)
	class ParticleForceRegistry
	{
	protected:

		//	하나의 힘 방생기와 힘을 적용할 입자를 추적
		struct ParticleForceRegistration
		{
			//입자
			Particle *particle;

			//힘 발생기
			ParticleForceGenerator *fg;
		};

		//	registrations라는 목록으로 유지
		typedef std::vector<ParticleForceRegistration> Registry;
		Registry registrations;

	public:
		//	주어진 힘 발생기를 주어진 입자에 등록
		void add(Particle* particle, ParticleForceGenerator *fg);

		/*
			주어진 등록된 쌍을 목록에서 제거한다.
			해당 쌍이 등록되지 않았다면,
			이 메소드는 아무런 영향을 끼치지 않는다.
		*/
		void remove(Particle* particle, ParticleForceGenerator *fg);

		/*
			모든 등록 항목을 제거한다.
			입자나 힘 발생기 자체는 제거하지 않고
			그 연결성만 제거한다
		*/
		void clear();

		/*
			해당하는 입자의 힘을 업데이트하도록 모든 힘 발생기를 call하고 적용한다.
		*/
		void updateForces(real duration);
	};
}

#endif // CYCLONE_PFGEN_H

