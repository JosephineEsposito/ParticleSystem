#pragma once

#include <GLCore.h>
#include <GLCoreUtils.h>

#include "ParticleSystem.h"

class SandboxLayer : public GLCore::Layer
{
public:
	SandboxLayer();
	virtual ~SandboxLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnEvent(GLCore::Event& event) override;

	/**
	 * @brief out Tick method
	 * @param ts 
	 */
	virtual void OnUpdate(GLCore::Timestep ts) override;

	/**
	 * @brief Handles the menu with hot reaload
	 */
	virtual void OnImGuiRender() override;
private:
	GLCore::Utils::OrthographicCameraController m_CameraController;
	ParticleProperties m_oParticleProps;
	ParticleSystem m_ParticleSystem;
};