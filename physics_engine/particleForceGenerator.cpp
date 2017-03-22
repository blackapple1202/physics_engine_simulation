#include "particleForceGenerator.h"
using namespace cyclone;

/*
	�ش��ϴ� ������ ���� ������Ʈ�ϵ��� ��� �� �߻��⸦ call�ϰ� �����Ѵ�
*/
void ParticleForceRegistry::updateForces(real duration)
{
	Registry::iterator i = registrations.begin();
	for (; i != registrations.end(); i++)
	{
		i->fg->updateForce(i->particle, duration);
	}
}

//	�־��� �� �߻��⸦ �־��� ���ڿ� ���
void ParticleForceRegistry::add(Particle* particle, ParticleForceGenerator *fg)
{
	ParticleForceRegistry::ParticleForceRegistration registration;
	registration.particle = particle;
	registration.fg = fg;
	registrations.push_back(registration);
}

// �߷� ������
ParticleGravity::ParticleGravity(const Vector3& gravity)
	: gravity(gravity)
{
}

//	�߷��� ���ڿ� ����
void ParticleGravity::updateForce(Particle* particle, real duration)
{
	// ������ ������ ���Ѵ��̸� ���� ����
	if (!particle->hasFiniteMass()) return;

	// ���ڿ� ���� �����Ѵ�. (f = m * a = m * g)
	particle->addForce(gravity * particle->getMass());
}

// ���׷� ������
ParticleDrag::ParticleDrag(real k1, real k2)
	: k1(k1), k2(k2)
{
}

//	������ ���ڿ� ����
void ParticleDrag::updateForce(Particle* particle, real duration)
{
	Vector3 force;
	particle->getVelocity(&force);

	// �� ���׷� ����� ����Ѵ�.
	real dragCoeff = force.magnitude();
	dragCoeff = k1 * dragCoeff + k2 * dragCoeff * dragCoeff;

	// �������� ���� ���׷¿� �����ϰ� ���ڿ� �����Ų��
	force.normalize();
	force *= -dragCoeff;
	particle->addForce(force);
}

ParticleSpring::ParticleSpring(Particle *other, real sc, real rl)
	: other(other), springConstant(sc), restLength(rl)
{
}

//ź������ ���ڿ� ����
void ParticleSpring::updateForce(Particle* particle, real duration)
{
	// ������ ���� ��� (d ���� ���)
	Vector3 force;
	particle->getPosition(&force);
	force -= other->getPosition();

	// ���� ũ�� ���
	real magnitude = force.magnitude();
	magnitude = real_abs(magnitude - restLength);
	magnitude *= springConstant;

	// �������� ����Ͽ� ���ڿ� ����
	force.normalize();
	force *= -magnitude;
	particle->addForce(force);
}

void ParticleAnchoredSpring::updateForce(Particle* particle, real duration)
{
	// ������ ������ �������� ���� ���� ���
	Vector3 force;
	particle->getPosition(&force);
	force -= *anchor;

	// ���� ũ�� ���
	real magnitude = force.magnitude();
	magnitude = (restLength - magnitude) * springConstant;

	// �������� ����Ͽ� ���ڿ� ����
	force.normalize();
	force *= magnitude;
	particle->addForce(force);
}


ParticleBuoyancy::ParticleBuoyancy(real maxDepth,
	real volume,
	real waterHeight,
	real liquidDensity)
	:
	maxDepth(maxDepth), volume(volume),
	waterHeight(waterHeight), liquidDensity(liquidDensity)
{
}

void ParticleBuoyancy::updateForce(Particle* particle, real duration)
{
	// ��ü�� ��ġ�ϴ� ���� y��ǥ�� �����´�
	real depth = particle->getPosition().y;

	//	�� �ۿ� �ִ��� Ȯ��, �ִٸ� return
	if (depth >= waterHeight + maxDepth) return;
	Vector3 force(0, 0, 0);

	// ���� �ִ� ���̿� �ִ��� Ȯ��, ��ü�� �ִ� ���̸� �Ѵ´ٸ� return
	if (depth <= waterHeight - maxDepth)
	{
		force.y = liquidDensity * volume;
		particle->addForce(force);
		return;
	}

	// �ƴϸ� �κ������� ����ִ� �� (���)
	force.y = liquidDensity * volume *
		(depth - maxDepth - waterHeight) / 2 * maxDepth;
	particle->addForce(force);
}

ParticleBungee::ParticleBungee(Particle *other, real sc, real rl)
	: other(other), springConstant(sc), restLength(rl)
{
}

void ParticleBungee::updateForce(Particle* particle, real duration)
{
	// ��ƴÿ��� ���� �������� ���͸� ����Ѵ�.
	Vector3 force;
	particle->getPosition(&force);
	force -= other->getPosition();

	// ��ƴÿ��� ���� �������� �����ִ� �� Ȯ��, �����ִٸ� return
	real magnitude = force.magnitude();
	if (magnitude <= restLength) return;

	// ���� ũ�⸦ ���
	magnitude = springConstant * (restLength - magnitude);

	// �������� ����Ͽ� ���ڿ� ����
	force.normalize();
	force *= -magnitude;
	particle->addForce(force);
}

ParticleFakeSpring::ParticleFakeSpring(Vector3 *anchor, real sc, real d)
	: anchor(anchor), springConstant(sc), damping(d)
{
}

void ParticleFakeSpring::updateForce(Particle* particle, real duration)
{
	// ������ ���Ѵ����� Ȯ��, ���Ѵ��� return
	if (!particle->hasFiniteMass()) return;

	// �������� �������� ��ü�� ��� ��ǥ ���
	Vector3 position;
	particle->getPosition(&position);
	position -= *anchor;

	// ��� ���� ����ϰ� �������� �ִ��� �˻�
	real gamma = 0.5f * real_sqrt(4 * springConstant - damping*damping);
	if (gamma == 0.0f) return;
	Vector3 c = position * (damping / (2.0f * gamma)) +
		particle->getVelocity() * (1.0f / gamma);

	// �Űܰ��� ���ϴ� ������ ��ǥ���
	Vector3 target = position * real_cos(gamma * duration) +
		c * real_sin(gamma * duration);
	target *= real_exp(-0.5f * duration * damping);

	// �ʿ��� ���ӵ��� ���� ���
	Vector3 accel = (target - position) * ((real)1.0 / (duration*duration)) -
		particle->getVelocity() * ((real)1.0 / duration);
	particle->addForce(accel * particle->getMass());
}

ParticleAnchoredSpring::ParticleAnchoredSpring()
{
}

ParticleAnchoredSpring::ParticleAnchoredSpring(Vector3 *anchor,
	real sc, real rl)
	: anchor(anchor), springConstant(sc), restLength(rl)
{
}

void ParticleAnchoredSpring::init(Vector3 *anchor, real springConstant,
	real restLength)
{
	ParticleAnchoredSpring::anchor = anchor;
	ParticleAnchoredSpring::springConstant = springConstant;
	ParticleAnchoredSpring::restLength = restLength;
}

void ParticleAnchoredBungee::updateForce(Particle* particle, real duration)
{
	// �������� ���� ���� ���
	Vector3 force;
	particle->getPosition(&force);
	force -= *anchor;

	// ���� ũ�⸦ ���
	real magnitude = force.magnitude();
	if (magnitude < restLength) return;

	magnitude = magnitude - restLength;
	magnitude *= springConstant;

	// �������� ����ϰ� ���ڿ� ����
	force.normalize();
	force *= -magnitude;
	particle->addForce(force);
}

