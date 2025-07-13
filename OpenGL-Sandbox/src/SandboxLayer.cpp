#include "SandboxLayer.h"

using namespace GLCore;
using namespace GLCore::Utils;

SandboxLayer::SandboxLayer()
	: m_CameraController(16.0f / 9.0f)
{
}

SandboxLayer::~SandboxLayer()
{
}

void SandboxLayer::OnAttach()
{
	EnableGLDebugging();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//@TODO: Init particle props here
	m_oParticleProps.vColorBegin = { 61 / 255.0f, 158 / 255.0f, 219 / 255.0f, 1.0f };
	m_oParticleProps.vColorEnd = { 197 / 255.0f, 219 / 255.0f, 233 / 255.0f, 1.0f };
	m_oParticleProps.fLifeTime = 10.f;
	m_oParticleProps.fSizeBegin = 0.6f;
	m_oParticleProps.fSizeEnd = 0.01f;
	m_oParticleProps.fGravityScalar = -6.f;
	m_oParticleProps.fDrag = 0.0f;
	m_oParticleProps.fRestituition = 0.1f;

	m_oParticleProps.vInitialPosition = { 0, 0 };
	m_oParticleProps.vInitialVelocity = { 5, 5 };
	m_oParticleProps.vSizeVariation = { 0, 0 };
	m_oParticleProps.vMinVelocityVariation = { 2, 4 };
	m_oParticleProps.vMaxVelocityVariation = { 2, 4 };
	m_oParticleProps.vCollisionNormal = { 0, 0 };
	m_oParticleProps.vSurfacePoint = { 0, 0 };

}

void SandboxLayer::OnDetach()
{
	// Shutdown
}

void SandboxLayer::OnEvent(Event& event)
{
	// Events
	m_CameraController.OnEvent(event);

	if (event.GetEventType() == EventType::WindowResize)
	{
		WindowResizeEvent& e = (WindowResizeEvent&)event;
		glViewport(0, 0, e.GetWidth(), e.GetHeight());
	}
}

void SandboxLayer::OnUpdate(Timestep _fTimeStep)
{
	m_CameraController.OnUpdate(_fTimeStep);

	// Render

	glClearColor(0,0,0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//--- Get emitter position based on camera zoom ---
	float x = 0.f, y = 0.f;
	auto width = GLCore::Application::Get().GetWindow().GetWidth();
	auto height = GLCore::Application::Get().GetWindow().GetHeight();

	auto bounds = m_CameraController.GetBounds();
	x = 0.5 * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
	y = bounds.GetHeight() * 0.5f - 0.5f * bounds.GetHeight();
	//------------------------------------------------------

	// initial pos
	m_oParticleProps.vInitialPosition = { x, y };
	// emit
	m_ParticleSystem.Emit(m_oParticleProps);
	// sumatorial de fuerzas externas
	glm::vec2 vTotalForces = { 0, 0 };
	for (int i = 0; i < m_oParticleProps.vExternalForces.size(); ++i)
	{
		vTotalForces += m_oParticleProps.vExternalForces.at(i);
	}
	// update
	m_ParticleSystem.OnUpdate(_fTimeStep, vTotalForces);
	// render
	m_ParticleSystem.OnRender(m_CameraController.GetCamera());
}

void SandboxLayer::OnImGuiRender()
{
	// @TODO: Full ImGui editor menu
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Birth Color", glm::value_ptr(m_oParticleProps.vColorBegin));
	ImGui::ColorEdit4("Death Color", glm::value_ptr(m_oParticleProps.vColorEnd));

	ImGui::Separator();

	// float		initial size
	ImGui::Text("Size");
	ImGui::InputFloat("Initial Size", &m_oParticleProps.fSizeBegin);
	// float		final size
	ImGui::InputFloat("Final Size", &m_oParticleProps.fSizeEnd);
	// v2float	size variation
	ImGui::InputFloat2("Size Variation", glm::value_ptr(m_oParticleProps.vSizeVariation));
	// float		life time
	ImGui::InputFloat("Life Time", &m_oParticleProps.fLifeTime);
	
	ImGui::Separator();
	
	ImGui::Text("Velocity");
	// v2float	initial velocity
	ImGui::InputFloat2("Initial Velocity", glm::value_ptr(m_oParticleProps.vInitialVelocity));
	// v4float velocity variation
	ImGui::InputFloat4("Velocity Variation", reinterpret_cast<float*>(&m_oParticleProps.vMinVelocityVariation));
	// bool		simulate gravity
	ImGui::Checkbox("Simulate Gravity", &m_oParticleProps.bSimulateGravity);
	// float		gravity scalar
	ImGui::InputFloat("Gravity Scalar", &m_oParticleProps.fGravityScalar);
	// float		drag
	ImGui::InputFloat("Drag", &m_oParticleProps.fDrag);

	ImGui::Separator();

	ImGui::Text("Collision");
	// bool		simulate collision
	ImGui::Checkbox("Simulate Collision", &m_oParticleProps.bSimulateCollision);
	// float		restituition
	ImGui::InputFloat("Restitution", &m_oParticleProps.fRestituition);
	// v2float	collision normal
	float* f2CollisionArray[2] = { &m_oParticleProps.vCollisionNormal.x, &m_oParticleProps.vCollisionNormal.y };
	ImGui::InputFloat2("Collision Normal", *f2CollisionArray);
	// v2float	surface point
	float* f2SurfaceArray[2] = { &m_oParticleProps.vSurfacePoint.x, &m_oParticleProps.vSurfacePoint.y };
	ImGui::InputFloat2("Surface Point", *f2SurfaceArray);

	ImGui::Separator();

	ImGui::Text("External Forces");

	if (ImGui::CollapsingHeader("External Forces"))
	{
		for (size_t i = 0; i < m_oParticleProps.vExternalForces.size(); ++i)
		{
			std::string label = "Force " + std::to_string(i);
			ImGui::InputFloat2(label.c_str(), glm::value_ptr(m_oParticleProps.vExternalForces[i]));
		}

		if (ImGui::Button("Add Force"))
			m_oParticleProps.vExternalForces.emplace_back(0.0f, 0.0f);

		if (!m_oParticleProps.vExternalForces.empty())
		{
			ImGui::SameLine();
			if (ImGui::Button("Remove Last"))
				m_oParticleProps.vExternalForces.pop_back();
		}
	}

	ImGui::End();
}
