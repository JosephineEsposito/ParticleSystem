#pragma once

#include <GLCore.h>
#include <GLCoreUtils.h>

/**
 * @brief Generic properties for all praticles
 */
struct ParticleProperties
{
	bool bSimulateCollision = true;
	bool bSimulateGravity = true;

	float fLifeTime = 1.0f;
	float fSizeBegin = 0.5f;
	float fSizeEnd = 10.0f;;
	float fGravityScalar = -9.f;
	float fDrag = 0.25f;
	float fRestituition = 1.0f;

	glm::vec2 vInitialPosition = glm::vec2(0, 0);
	glm::vec2 vInitialVelocity = glm::vec2(0, 0);
	glm::vec2 vSizeVariation = glm::vec2(0, 1);
	glm::vec2 vMinVelocityVariation = glm::vec2(0, 0);
	glm::vec2 vMaxVelocityVariation = glm::vec2(1, 1);
	glm::vec2 vCollisionNormal = glm::vec2(0, 0);
	glm::vec2 vSurfacePoint = glm::vec2(0, 0);

	glm::vec4 vColorBegin = glm::vec4(0, 0, 0, 0);
	glm::vec4 vColorEnd = glm::vec4(0, 0, 0, 0);

	std::vector<glm::vec2> vExternalForces;
};

class ParticleSystem
{
public:
	ParticleSystem();

	/**
	 * @brief The tick method
	 * @param _fTimeStep 
	 */
	void OnUpdate(GLCore::Timestep _fTimeStep, glm::vec2 _vTotalForces);

	/**
	 * @brief Renders the particles on screen
	 * @param _oCamera 
	 */
	void OnRender(GLCore::Utils::OrthographicCamera& _oCamera);

	/**
	 * @brief Handles the initialization of the particles
	 * @param _oParticleProps 
	 */
	void Emit(const ParticleProperties& _oParticleProps);
private:

	/**
	 * @brief Private for all the systems
	 */
	struct Particle
	{
		glm::vec2 vPosition;
		glm::vec2 vVelocity;
		float fSpin = 0.0f;

		float fLifeRemaining = 0.0f;
		ParticleProperties oParticleProps;

		bool bActive = false;
	};
	std::vector<Particle> m_tParticlePool;
	uint32_t m_uPoolIndex = 999;

	GLuint m_iQuadVA = 0;
	std::unique_ptr<GLCore::Utils::Shader> m_pParticleShader;
	GLint m_ParticleShaderViewProj, m_ParticleShaderTransform, m_ParticleShaderColor;
};