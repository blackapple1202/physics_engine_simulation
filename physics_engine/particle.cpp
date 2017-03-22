#include "stdafx.h"
#include <assert.h>
#include "particle.h"
using namespace cyclone;

//	���� (�� ������Ʈ�� �Լ�)
void Particle::integrate(real duration)
{
	//���Ѵ� ������ ���ڴ� �������� �ʴ´�
	if (inverseMass <= 0.0f)
		return;
	assert(duration > 0.0);

	//��ġ�� ������Ʈ �Ѵ� (s += v * t)
	position.addScaledVector(velocity, duration);

	//	�����κ��� ���ӵ��� ����Ѵ�
	//	( a += f( = ma) * (1 / m) )
	Vector3 resultingAcc = acceleration;
	resultingAcc.addScaledVector(forceAccum, inverseMass);

	//���ӵ��κ��� �ӵ��� ������Ʈ �Ѵ� (v += a * t)
	velocity.addScaledVector(resultingAcc, duration);
	
	// drag�� �����Ѵ� (v = v * damping^t)
	velocity *= real_pow(damping, duration);

	//�շ��� �����, �� �� �����ӿ��� ������ ������Ʈ�� ������ �շ��� 0���� �ʱ�ȭ�Ѵ�. (�շ� = 0)
	clearAccumulator();

}

// ���� ���� �ʱ�ȭ�Ѵ�
void Particle::setMass(const real mass)
{
	assert(mass != 0);
	Particle::inverseMass = ((real)1.0) / mass;
}

// ���� ���� �ҷ��´�
real Particle::getMass() const
{
	if (inverseMass == 0) {
		return REAL_MAX;
	}
	else {
		return ((real)1.0) / inverseMass;
	}
}

//	������ ���� ���� �ʱ�ȭ�Ѵ�
void Particle::setInverseMass(const real inverseMass)
{
	Particle::inverseMass = inverseMass;
}

//	������ ���� ���� �ҷ��´�
real Particle::getInverseMass() const
{
	return inverseMass;
}

//	������ ���Ѵ밡 �ƴ� ��� true ��ȯ
bool Particle::hasFiniteMass() const
{
	return inverseMass >= 0.0f;
}

//	damping ���� �ʱ�ȭ�Ѵ�
void Particle::setDamping(const real damping)
{
	Particle::damping = damping;
}

//	damping ���� �ҷ��´�
real Particle::getDamping() const
{
	return damping;
}

//	��ġ ���� �ʱ�ȭ�Ѵ�
void Particle::setPosition(const Vector3 &position)
{
	Particle::position = position;
}

//	��ġ ���� �ʱ�ȭ�Ѵ�
void Particle::setPosition(const real x, const real y, const real z)
{
	position.x = x;
	position.y = y;
	position.z = z;
}

//	��ġ ���� �����´�
void Particle::getPosition(Vector3 *position) const
{
	*position = Particle::position;
}

//	��ġ ���� �����´�
Vector3 Particle::getPosition() const
{
	return position;
}

//	�ӵ� ���� �ʱ�ȭ�Ѵ�
void Particle::setVelocity(const Vector3 &velocity)
{
	Particle::velocity = velocity;
}

//	�ӵ� ���� �ʱ�ȭ�Ѵ�
void Particle::setVelocity(const real x, const real y, const real z)
{
	velocity.x = x;
	velocity.y = y;
	velocity.z = z;
}

//	�ӵ� ���� �ҷ��´�
void Particle::getVelocity(Vector3 *velocity) const
{
	*velocity = Particle::velocity;
}

//	�ӵ� ���� �ҷ��´�
Vector3 Particle::getVelocity() const
{
	return velocity;
}

//	���ӵ� ���� �ʱ�ȭ�Ѵ�
void Particle::setAcceleration(const Vector3 &acceleration)
{
	Particle::acceleration = acceleration;
}

//	���ӵ� ���� �ʱ�ȭ�Ѵ�
void Particle::setAcceleration(const real x, const real y, const real z)
{
	acceleration.x = x;
	acceleration.y = y;
	acceleration.z = z;
}

//	���ӵ� ���� �ҷ��´�
void Particle::getAcceleration(Vector3 *acceleration) const
{
	*acceleration = Particle::acceleration;
}

//	���ӵ� ���� �ҷ��´�
Vector3 Particle::getAcceleration() const
{
	return acceleration;
}

//	�� �׸��� �����.
void Particle::clearAccumulator()
{
	forceAccum.clear();
}

//	���� force ���͸�ŭ �շ� ��(forceAccum)�� ���Ѵ�
void Particle::addForce(const Vector3 &force)
{
	forceAccum += force;
}
