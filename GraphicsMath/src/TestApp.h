#pragma once

#include "Core/Window.h"
#include "Event/Event.h"
#include "Event/ApplicationEvent.h"
#include "ImGui/ImGuiManager.h"
#include "Math/Types.h"

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
	void UpdateViewProjection();

	void DrawWorldGrid();

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
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_colorPS;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_wgInputLayout;

	Microsoft::WRL::ComPtr<ID3D11Buffer> m_vb;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_ib;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_wgVB;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_wgIB;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_basicVSSysCBuf;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_basicVSEntCBuf;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_wgVSSysCBuf;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_colorPSEntCBuf;

	DirectX::XMFLOAT4X4 m_viewProjection;

	GM::Vector m_camPos = GM::Vector(0.0f, 0.0f, -8.0f, 0.0f);
	GM::Vector m_camRot;

	GM::Vector m_cubePos;
	GM::Vector m_cubeRot;
	GM::Vector m_cubeSca = GM::Vector(1.0, 1.0f, 1.0f, 0.0f);

	bool m_useXM = false;
	bool m_showGrid = false;


	// temps
	GM::Vector t_maxPos = GM::Vector(3.0f, 5.0f, 0.0f, 0.0f);
	GM::Vector t_minPos = GM::Vector(3.0f, -5.0f, 0.0f, 0.0f);
	float t_t = 0.0f;

};