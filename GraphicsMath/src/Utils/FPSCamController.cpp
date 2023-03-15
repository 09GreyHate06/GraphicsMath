#include "FPSCamController.h"

#include "Event/Input.h"
#include "Event/KeyEvent.h"

namespace GM
{
	Utils::FPSCamController::FPSCamController(Camera* camera, const Window* window, float moveSpeed, float rotateSpeed)
		: m_camera(camera), m_window(window), m_moveSpeed(moveSpeed), m_rotateSpeed(rotateSpeed)
	{
	}

	Utils::FPSCamController::FPSCamController()
		: FPSCamController(nullptr, nullptr, 0.0f, 0.0f)
	{
	}

	void Utils::FPSCamController::Set(Camera* camera, const Window* window, float moveSpeed, float rotateSpeed)
	{
		m_camera = camera;
		m_window = window;
		m_moveSpeed = moveSpeed;
		m_rotateSpeed = rotateSpeed;
	}

	void Utils::FPSCamController::SetCamera(Camera* camera)
	{
		m_camera = camera;
	}

	void Utils::FPSCamController::SetWindow(const Window* m_window)
	{
		m_window = m_window;
	}

	void Utils::FPSCamController::SetMoveSpeed(float moveSpeed)
	{
		m_moveSpeed = moveSpeed;
	}

	void Utils::FPSCamController::SetRotateSpeed(float rotateSpeed)
	{
		m_rotateSpeed = rotateSpeed;
	}

	void Utils::FPSCamController::OnEvent(Event& event)
	{
		if (event.GetEventType() == EventType::KeyPressed)
		{
			KeyPressedEvent& e = static_cast<KeyPressedEvent&>(event);
			
			if (e.GetKeyCode() == Key::SpaceBar)
			{
				m_active = !m_active;
				if (m_active)
				{
					Window::HideCursor();
					
					m_initialPos = Input::GetMousePos(m_window) - m_offset;
				}
				else
				{
					Window::ShowCursor();
				}
			}
		}
	}

	void Utils::FPSCamController::OnUpdate()
	{
		if (!m_active || !m_camera || !m_window) return;

		if (Input::GetKey(m_window, Key::W))
		{
			Vector pos = m_camera->GetDesc().position;
			pos += m_camera->GetForwardDirection() * m_moveSpeed;
			m_camera->SetPosition(pos);
		}

		if (Input::GetKey(m_window, Key::S))
		{
			Vector pos = m_camera->GetDesc().position;
			pos -= m_camera->GetForwardDirection() * m_moveSpeed;
			m_camera->SetPosition(pos);
		}

		if (Input::GetKey(m_window, Key::D))
		{
			Vector pos = m_camera->GetDesc().position;
			pos += m_camera->GetRightDirection() * m_moveSpeed;
			m_camera->SetPosition(pos);
		}

		if (Input::GetKey(m_window, Key::A))
		{
			Vector pos = m_camera->GetDesc().position;
			pos -= m_camera->GetRightDirection() * m_moveSpeed;
			m_camera->SetPosition(pos);
		}


		m_offset = Input::GetMousePos(m_window) - m_initialPos;
		Vector rot = m_camera->GetDesc().rotation;

		rot.x = m_offset.y * m_rotateSpeed;
		rot.y = m_offset.x * m_rotateSpeed;

		m_camera->SetRotation(rot);
	}
}

