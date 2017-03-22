#pragma once

#ifndef CYCLONE_PARTICLE_H
#define CYCLONE_PARTICLE_H

#include "core.h"

namespace cyclone {

	class Particle
	{
	protected :
		//입자의 위치 벡터 좌표값
		Vector3 position;

		//입자의 속도 벡터 좌표값
		Vector3 velocity;

		//입자의 가속도 벡터 좌표값
		Vector3 acceleration;

		//수치 연산오차로 인해 증가한 에너지를 줄여주는 역할을 한다.
		real damping;

		//질량의 역수
		real inverseMass;

		/*
		물체에 작용하는 힘들을 전부 더해 저장하는 벡터
		(달랑베르의 원리)
		각 적분 단계에서 (매 프레임마다) 0으로 만들어주고
		영벡터에 프레임마다 합력을 더해주어서 업데이트하는 원리이다.
		*/
		Vector3 forceAccum;

	public :

		//	적분 (값 업데이트용 함수)
		void integrate(real duration);

		//	질량 값을 초기화한다
		void setMass(const real mass);

		//	질량 값을 불러온다
		real getMass() const;

		//	질량의 역수 값을 초기화한다
		void setInverseMass(const real inverseMass);

		//	질량의 역수 값을 불러온다
		real getInverseMass() const;

		//	질량이 무한대가 아닐 경우 true 반환
		bool hasFiniteMass() const;

		//	damping 값을 초기화한다
		void setDamping(const real damping);

		//	damping 값을 불러온다
		real getDamping() const;

		//	위치 값을 초기화한다
		void setPosition(const Vector3 &position);

		//	위치 값을 초기화한다
		void setPosition(const real x, const real y, const real z);

		//	위치 값을 가져온다
		void getPosition(Vector3 *position) const;

		//	위치 값을 가져온다
		Vector3 getPosition() const;

		//	속도 값을 초기화한다
		void setVelocity(const Vector3 &velocity);

		//	속도 값을 초기화한다
		void setVelocity(const real x, const real y, const real z);

		//	속도 값을 불러온다
		void getVelocity(Vector3 *velocity) const;

		//	속도 값을 불러온다
		Vector3 getVelocity() const;

		//	가속도 값을 초기화한다
		void setAcceleration(const Vector3 &acceleration);

		//	가속도 값을 초기화한다
		void setAcceleration(const real x, const real y, const real z);

		//	가속도 값을 불러온다
		void getAcceleration(Vector3 *acceleration) const;

		//	가속도 값을 불러온다
		Vector3 getAcceleration() const;

		//	힘을 force 벡터만큼 가한다
		void addForce(const Vector3 &force);

		//	합력을 0으로 초기화한다.
		void clearAccumulator();
	};
}

#endif