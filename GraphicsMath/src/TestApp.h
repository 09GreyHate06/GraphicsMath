#pragma once

#include "Core/Window.h"
#include "Event/Event.h"
#include "Event/ApplicationEvent.h"
#include "ImGui/ImGuiManager.h"
#include "Math/Types.h"
#include "Rendering/Camera.h"
#include "Utils/FPSCamController.h"

#include <array>
#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl.h>

class TestApp
{
public:
	TestApp();
	~TestApp() = default;

	void Run();

private:
	void OnEvent(GM::Event& event);
	void OnUpdate();
	void OnRender();
	void OnImGuiRender();

	bool OnWindowResizedEvent(GM::WindowResizeEvent& e);


	void SetRendererStates();
	void SetShaders();
	void SetBuffers();

	void DrawWorldGrid();
	void DrawCameraFrustum();

	std::unique_ptr<GM::Window> m_window;
	std::unique_ptr<GM::ImGuiManager> m_imguiManager;


	Microsoft::WRL::ComPtr<ID3D11Device> m_device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_context;
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;


	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_rtv;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_dsv;

	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_rsState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_dsState;
	Microsoft::WRL::ComPtr<ID3D11BlendState> m_blendState;

	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vs;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_ps;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_wgVS;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_colorVS;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_colorPS;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_wgInputLayout;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_colorInputLayout;

	Microsoft::WRL::ComPtr<ID3D11Buffer> m_vb;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_ib;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_wgVB;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_wgIB;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_basicVSSysCBuf;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_basicVSEntCBuf;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_wgVSSysCBuf;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_colorVSSysCBuf;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_colorVSEntCBuf;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_colorPSEntCBuf;

	std::array<GM::Camera, 2> m_cameras;
	GM::Utils::FPSCamController m_fpsCamController;

	GM::Vector m_cubePos;
	GM::Vector m_cubeRot;
	GM::Vector m_cubeSca = GM::Vector(1.0, 1.0f, 1.0f, 0.0f);

	int m_activeCamera = 0;
	bool m_showGrid = false;

};