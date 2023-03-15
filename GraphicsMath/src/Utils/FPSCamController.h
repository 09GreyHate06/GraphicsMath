#pragma once
#include "Rendering/Camera.h"
#include "Core/Window.h"
#include "Event/Event.h"

namespace GM::Utils
{
	class FPSCamController
	{
	public:
		FPSCamController(Camera* camera, const Window* window, float moveSpeed, float rotateSpeed);
		FPSCamController();

		void Set(Camera* camera, const Window* window, float moveSpeed, float rotateSpeed);
		void SetCamera(Camera* camera);
		void SetWindow(const Window* window);
		void SetMoveSpeed(float moveSpeed);
		void SetRotateSpeed(float rotateSpeed);

		void OnEvent(Event& event);
		void OnUpdate();

	private:
		Camera* m_camera;
		const Window* m_window;
		float m_moveSpeed;
		float m_rotateSpeed;

		bool m_active = false;
		Vector m_initialPos;
		Vector m_offset;
	};
}