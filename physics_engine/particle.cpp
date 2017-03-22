#include "stdafx.h"
#include <assert.h>
#include "particle.h"
using namespace cyclone;

//	적분 (값 업데이트용 함수)
void Particle::integrate(real duration)
{
	//무한대 질량인 입자는 적분하지 않는다
	if (inverseMass <= 0.0f)
		return;
	assert(duration > 0.0);

	//위치를 업데이트 한다 (s += v * t)
	position.addScaledVector(velocity, duration);

	//	힘으로부터 가속도를 계산한다
	//	( a += f( = ma) * (1 / m) )
	Vector3 resultingAcc = acceleration;
	resultingAcc.addScaledVector(forceAccum, inverseMass);

	//가속도로부터 속도를 업데이트 한다 (v += a * t)
	velocity.addScaledVector(resultingAcc, duration);
	
	// drag를 적용한다 (v = v * damping^t)
	velocity *= real_pow(damping, duration);

	//합력을 지운다, 즉 이 프레임에서 값들의 업데이트가 끝나면 합력은 0으로 초기화한다. (합력 = 0)
	clearAccumulator();

}

// 질량 값을 초기화한다
void Particle::setMass(const real mass)
{
	assert(mass != 0);
	Particle::inverseMass = ((real)1.0) / mass;
}

// 질량 값을 불러온다
real Particle::getMass() const
{
	if (inverseMass == 0) {
		return REAL_MAX;
	}
	else {
		return ((real)1.0) / inverseMass;
	}
}

//	질량의 역수 값을 초기화한다
void Particle::setInverseMass(const real inverseMass)
{
	Particle::inverseMass = inverseMass;
}

//	질량의 역수 값을 불러온다
real Particle::getInverseMass() const
{
	return inverseMass;
}

//	질량이 무한대가 아닐 경우 true 반환
bool Particle::hasFiniteMass() const
{
	return inverseMass >= 0.0f;
}

//	damping 값을 초기화한다
void Particle::setDamping(const real damping)
{
	Particle::damping = damping;
}

//	damping 값을 불러온다
real Particle::getDamping() const
{
	return damping;
}

//	위치 값을 초기화한다
void Particle::setPosition(const Vector3 &position)
{
	Particle::position = position;
}

//	위치 값을 초기화한다
void Particle::setPosition(const real x, const real y, const real z)
{
	position.x = x;
	position.y = y;
	position.z = z;
}

//	위치 값을 가져온다
void Particle::getPosition(Vector3 *position) const
{
	*position = Particle::position;
}

//	위치 값을 가져온다
Vector3 Particle::getPosition() const
{
	return position;
}

//	속도 값을 초기화한다
void Particle::setVelocity(const Vector3 &velocity)
{
	Particle::velocity = velocity;
}

//	속도 값을 초기화한다
void Particle::setVelocity(const real x, const real y, const real z)
{
	velocity.x = x;
	velocity.y = y;
	velocity.z = z;
}

//	속도 값을 불러온다
void Particle::getVelocity(Vector3 *velocity) const
{
	*velocity = Particle::velocity;
}

//	속도 값을 불러온다
Vector3 Particle::getVelocity() const
{
	return velocity;
}

//	가속도 값을 초기화한다
void Particle::setAcceleration(const Vector3 &acceleration)
{
	Particle::acceleration = acceleration;
}

//	가속도 값을 초기화한다
void Particle::setAcceleration(const real x, const real y, const real z)
{
	acceleration.x = x;
	acceleration.y = y;
	acceleration.z = z;
}

//	가속도 값을 불러온다
void Particle::getAcceleration(Vector3 *acceleration) const
{
	*acceleration = Particle::acceleration;
}

//	가속도 값을 불러온다
Vector3 Particle::getAcceleration() const
{
	return acceleration;
}

//	힘 항목을 지운다.
void Particle::clearAccumulator()
{
	forceAccum.clear();
}

//	힘을 force 벡터만큼 합력 값(forceAccum)에 더한다
void Particle::addForce(const Vector3 &force)
{
	forceAccum += force;
}
