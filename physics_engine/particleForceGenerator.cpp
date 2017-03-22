#include "particleForceGenerator.h"
using namespace cyclone;

/*
	해당하는 입자의 힘을 업데이트하도록 모든 힘 발생기를 call하고 적용한다
*/
void ParticleForceRegistry::updateForces(real duration)
{
	Registry::iterator i = registrations.begin();
	for (; i != registrations.end(); i++)
	{
		i->fg->updateForce(i->particle, duration);
	}
}

//	주어진 힘 발생기를 주어진 입자에 등록
void ParticleForceRegistry::add(Particle* particle, ParticleForceGenerator *fg)
{
	ParticleForceRegistry::ParticleForceRegistration registration;
	registration.particle = particle;
	registration.fg = fg;
	registrations.push_back(registration);
}

// 중력 생성자
ParticleGravity::ParticleGravity(const Vector3& gravity)
	: gravity(gravity)
{
}

//	중력을 입자에 적용
void ParticleGravity::updateForce(Particle* particle, real duration)
{
	// 입자의 질량이 무한대이면 함주 종료
	if (!particle->hasFiniteMass()) return;

	// 입자에 힘을 적용한다. (f = m * a = m * g)
	particle->addForce(gravity * particle->getMass());
}

// 저항력 생성자
ParticleDrag::ParticleDrag(real k1, real k2)
	: k1(k1), k2(k2)
{
}

//	저항을 입자에 적용
void ParticleDrag::updateForce(Particle* particle, real duration)
{
	Vector3 force;
	particle->getVelocity(&force);

	// 총 저항력 계수를 계산한다.
	real dragCoeff = force.magnitude();
	dragCoeff = k1 * dragCoeff + k2 * dragCoeff * dragCoeff;

	// 최종적인 힘을 저항력에 적용하고 입자에 적용시킨다
	force.normalize();
	force *= -dragCoeff;
	particle->addForce(force);
}

ParticleSpring::ParticleSpring(Particle *other, real sc, real rl)
	: other(other), springConstant(sc), restLength(rl)
{
}

//탄성력을 입자에 적용
void ParticleSpring::updateForce(Particle* particle, real duration)
{
	// 스프링 벡터 계산 (d 벡터 계산)
	Vector3 force;
	particle->getPosition(&force);
	force -= other->getPosition();

	// 힘의 크기 계산
	real magnitude = force.magnitude();
	magnitude = real_abs(magnitude - restLength);
	magnitude *= springConstant;

	// 최종힘을 계산하여 입자에 적용
	force.normalize();
	force *= -magnitude;
	particle->addForce(force);
}

void ParticleAnchoredSpring::updateForce(Particle* particle, real duration)
{
	// 한쪽이 고정된 스프링의 벡터 값을 계산
	Vector3 force;
	particle->getPosition(&force);
	force -= *anchor;

	// 힘의 크기 계산
	real magnitude = force.magnitude();
	magnitude = (restLength - magnitude) * springConstant;

	// 최종힘을 계산하여 입자에 적용
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
	// 물체가 위치하는 깊이 y좌표를 가져온다
	real depth = particle->getPosition().y;

	//	물 밖에 있는지 확인, 있다면 return
	if (depth >= waterHeight + maxDepth) return;
	Vector3 force(0, 0, 0);

	// 물의 최대 깊이에 있는지 확인, 물체가 최대 깊이를 넘는다면 return
	if (depth <= waterHeight - maxDepth)
	{
		force.y = liquidDensity * volume;
		particle->addForce(force);
		return;
	}

	// 아니면 부분적으로 잠겨있는 지 (계산)
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
	// 잡아늘였을 때만 스프링의 벡터를 계산한다.
	Vector3 force;
	particle->getPosition(&force);
	force -= other->getPosition();

	// 잡아늘였을 때만 스프링이 눌려있는 지 확인, 눌려있다면 return
	real magnitude = force.magnitude();
	if (magnitude <= restLength) return;

	// 힘의 크기를 계산
	magnitude = springConstant * (restLength - magnitude);

	// 최종힘을 계산하여 입자에 적용
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
	// 질량이 무한대인지 확인, 무한대라면 return
	if (!particle->hasFiniteMass()) return;

	// 고정점을 기준으로 물체의 상대 좌표 계산
	Vector3 position;
	particle->getPosition(&position);
	position -= *anchor;

	// 상수 값을 계산하고 범위내에 있는지 검사
	real gamma = 0.5f * real_sqrt(4 * springConstant - damping*damping);
	if (gamma == 0.0f) return;
	Vector3 c = position * (damping / (2.0f * gamma)) +
		particle->getVelocity() * (1.0f / gamma);

	// 옮겨가기 원하는 지점의 좌표계산
	Vector3 target = position * real_cos(gamma * duration) +
		c * real_sin(gamma * duration);
	target *= real_exp(-0.5f * duration * damping);

	// 필요한 가속도와 힘을 계산
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
	// 스프링의 벡터 값을 계산
	Vector3 force;
	particle->getPosition(&force);
	force -= *anchor;

	// 힘의 크기를 계산
	real magnitude = force.magnitude();
	if (magnitude < restLength) return;

	magnitude = magnitude - restLength;
	magnitude *= springConstant;

	// 최종힘을 계산하고 입자에 적용
	force.normalize();
	force *= -magnitude;
	particle->addForce(force);
}

