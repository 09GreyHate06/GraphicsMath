#pragma once
#include "Core/Window.h"
#include <imgui.h>
#include <d3d11.h>

namespace GM
{
	class ImGuiManager
	{
	public:
		ImGuiManager(GM::Window* window, ID3D11Device* device, ID3D11DeviceContext* context);

		void Begin();
		void End();

	private:
		GM::Window* m_window;
	};
}