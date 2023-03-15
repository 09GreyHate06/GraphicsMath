#include "TestApp.h"

#include <d3dcompiler.h>
#include "Utils/TextReader.h"
#include "Utils/BasicMesh.h"
#include "Math/Operators.h"
#include "Math/Functions.h"

#include "Event/Input.h"

using namespace GM;
using namespace Microsoft::WRL;

using namespace DirectX;



TestApp::TestApp()
{
	m_window = std::make_unique<Window>(WindowDesc(1280, 720, "Test app", "TestApp"));
	m_window->SetEventCallback(GM_BIND_EVENT_FN(OnEvent));

	DXGI_SWAP_CHAIN_DESC scDesc = {};
	scDesc.BufferDesc.Width = 0;
	scDesc.BufferDesc.Height = 0;
	scDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scDesc.BufferDesc.RefreshRate.Numerator = 0;
	scDesc.BufferDesc.RefreshRate.Denominator = 0;
	scDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	scDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scDesc.SampleDesc.Count = 1;
	scDesc.SampleDesc.Quality = 0;
	scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scDesc.BufferCount = 1;
	scDesc.OutputWindow = m_window->GetNativeWindow();
	scDesc.Windowed = TRUE;
	scDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scDesc.Flags = 0;

	UINT swapCreateFlags = 0;
#ifdef GM_DEBUG
	swapCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif // GM_DEBUG

	// create device and front/back buffers, swap chain and rendering context
	HR(D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		swapCreateFlags,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&scDesc,
		&m_swapChain,
		&m_device,
		nullptr,
		&m_context
	));

	{
		ComPtr<ID3D11Texture2D> backbuffer;
		HR(m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &backbuffer));
		D3D11_RENDER_TARGET_VIEW_DESC desc = {};
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipSlice = 0;
		HR(m_device->CreateRenderTargetView(backbuffer.Get(), &desc, &m_rtv));
	}


	{
		D3D11_TEXTURE2D_DESC texDesc = {};
		texDesc.Width = m_window->GetDesc().width;
		texDesc.Height = m_window->GetDesc().height;
		texDesc.MipLevels = 1;
		texDesc.ArraySize = 1;
		texDesc.Format = DXGI_FORMAT_D32_FLOAT;
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		texDesc.CPUAccessFlags = 0;
		texDesc.MiscFlags = 0;

		ComPtr<ID3D11Texture2D> tex;
		HR(m_device->CreateTexture2D(&texDesc, nullptr, &tex));

		D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
		dsvDesc.Format = texDesc.Format;
		dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Texture2D.MipSlice = 0;
		HR(m_device->CreateDepthStencilView(tex.Get(), &dsvDesc, &m_dsv));
	}


	D3D11_VIEWPORT vp = {};
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	vp.Width = (float)m_window->GetDesc().width;
	vp.Height = (float)m_window->GetDesc().height;
	vp.MaxDepth = 1.0f;
	vp.MinDepth = 0.0f;
	m_context->RSSetViewports(1, &vp);

	m_imguiManager = std::make_unique<ImGuiManager>(m_window.get(), m_device.Get(), m_context.Get());
	
	SetRendererStates();
	SetShaders();
	SetBuffers();


	CameraDesc desc = {};
	desc.fov = 45.0f;
	desc.width = (float)m_window->GetDesc().width;
	desc.height = (float)m_window->GetDesc().height;
	desc.nearZ = 0.1f;
	desc.farZ = 1000.0f;
	desc.position = Vector(0.0f, 0.0f, -8.0f, 1.0f);
	desc.rotation = Vector(0.0f, 0.0f, 0.0f, 0.0f);
	m_cameras[0].Set(desc, m_device.Get());
	m_cameras[1].Set(desc, m_device.Get());

	m_fpsCamController.Set(&m_cameras[m_activeCamera], m_window.get(), 0.5f, 0.5f);
}

void TestApp::Run()
{
	while (!m_window->GetState().shouldClose)
	{
		OnUpdate();
		OnRender();
		OnImGuiRender();

		Window::PollEvents();
		HR(m_swapChain->Present(1, 0));
	}
}

void TestApp::OnEvent(GM::Event& event)
{
	EventDispatcher e(event);
	e.Dispatch<WindowResizeEvent>(GM_BIND_EVENT_FN(OnWindowResizedEvent));

	m_fpsCamController.OnEvent(event);
}

void TestApp::OnUpdate()
{
	m_fpsCamController.OnUpdate();
}

void TestApp::OnRender()
{
	float clearColor[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	m_context->ClearRenderTargetView(m_rtv.Get(), clearColor);
	m_context->ClearDepthStencilView(m_dsv.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0xff);

	m_context->RSSetState(m_rsState.Get());
	m_context->OMSetBlendState(m_blendState.Get(), nullptr, 0xffffffff);
	m_context->OMSetDepthStencilState(m_dsState.Get(), 0xff);


	m_context->OMSetRenderTargets(1, m_rtv.GetAddressOf(), m_dsv.Get());

	m_context->VSSetShader(m_vs.Get(), nullptr, 0);
	m_context->PSSetShader(m_ps.Get(), nullptr, 0);
	m_context->IASetInputLayout(m_inputLayout.Get());

	m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	
	// Set basic.vs System CBuf
	{
		m_context->VSSetConstantBuffers(0, 1, m_basicVSSysCBuf.GetAddressOf());
		D3D11_MAPPED_SUBRESOURCE msr = {};
		HR(m_context->Map(m_basicVSSysCBuf.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr));
		memcpy(msr.pData, &MatTranspose(m_cameras[m_activeCamera].GetViewProjection()), sizeof(Matrix));
		m_context->Unmap(m_basicVSSysCBuf.Get(), 0);
	}

	// Set basic.vs Entity CBuf
	{
		m_context->VSSetConstantBuffers(1, 1, m_basicVSEntCBuf.GetAddressOf());
		Quaternion rotQuat = QuatRotationRollPitchYaw(ToRadians(m_cubeRot.x), ToRadians(m_cubeRot.y), ToRadians(m_cubeRot.z));

		Matrix transform =
			MatScale(m_cubeSca.x, m_cubeSca.y, m_cubeSca.z) *
			MatRotationQuaternion(rotQuat) *
			MatTranslate(m_cubePos.x, m_cubePos.y, m_cubePos.z);

		D3D11_MAPPED_SUBRESOURCE msr = {};
		HR(m_context->Map(m_basicVSEntCBuf.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr));
		memcpy(msr.pData, &MatTranspose(transform), sizeof(Matrix));
		m_context->Unmap(m_basicVSEntCBuf.Get(), 0);
	}

	// Set Vertex and Index Buffer
	{
		const uint32_t offset = 0;
		const uint32_t stride = 7 * sizeof(float);
		m_context->IASetVertexBuffers(0, 1, m_vb.GetAddressOf(), &stride, &offset);

		m_context->IASetIndexBuffer(m_ib.Get(), DXGI_FORMAT_R32_UINT, 0);
	}

	m_context->DrawIndexed(36, 0, 0);
	DrawCameraFrustum();

	if (m_showGrid)
	{
		DrawWorldGrid();
	}
}

void TestApp::OnImGuiRender()
{
	m_imguiManager->Begin();

	ImGui::Begin("Cube Transform");
	ImGui::DragFloat3("Position", &m_cubePos[0], 0.1f);
	ImGui::DragFloat3("Rotation", &m_cubeRot[0], 0.1f);
	ImGui::DragFloat3("Scale", &m_cubeSca[0], 0.1f);
	ImGui::End();

	ImGui::Begin("Config");
	ImGui::Checkbox("Show grid", &m_showGrid);
	ImGui::End();

	ImGui::Begin("Camera");
	const char* items = " 1\0 2";
	if (ImGui::Combo("Active", &m_activeCamera, items))
	{
		m_fpsCamController.SetCamera(&m_cameras[m_activeCamera]);
	}

	CameraDesc camDesc = m_cameras[m_activeCamera].GetDesc();
	if (ImGui::DragFloat3("Position", &camDesc.position[0], 0.1f) ||
		ImGui::DragFloat3("Rotation", &camDesc.rotation[0], 0.1f))
	{
		m_cameras[m_activeCamera].SetPosition(camDesc.position);
		m_cameras[m_activeCamera].SetRotation(camDesc.rotation);
	}

	if (ImGui::DragFloat("Fov", &camDesc.fov, 0.1f))
	{
		m_cameras[m_activeCamera].SetFov(camDesc.fov);
	}

	if (ImGui::DragFloat2("Size", &camDesc.width, 0.1f))
	{
		m_cameras[m_activeCamera].SetSize(camDesc.width, camDesc.height);
	}

	if (ImGui::DragFloat2("Near, Far", &camDesc.nearZ, 0.1f))
	{
		m_cameras[m_activeCamera].SetNearFarZ(camDesc.nearZ, camDesc.farZ);
	}

	ImGui::End();

	m_imguiManager->End();
}

bool TestApp::OnWindowResizedEvent(GM::WindowResizeEvent& e)
{
	if (e.GetWidth() == 0 || e.GetHeight() == 0) return false;

	D3D11_VIEWPORT vp = {};
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	vp.Width = (float)e.GetWidth();
	vp.Height = (float)e.GetHeight();
	vp.MaxDepth = 1.0f;
	vp.MinDepth = 0.0f;
	m_context->RSSetViewports(1, &vp);

	m_rtv.Reset();
	m_dsv.Reset();

	HR(m_swapChain->ResizeBuffers(1, e.GetWidth(), e.GetHeight(), DXGI_FORMAT_R8G8B8A8_UNORM, 0));
	ComPtr<ID3D11Texture2D> backBuffer;
	HR(m_swapChain->GetBuffer(0, __uuidof(ID3D11Resource), &backBuffer));

	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice = 0;
	HR(m_device->CreateRenderTargetView(backBuffer.Get(), &rtvDesc, &m_rtv));


	D3D11_TEXTURE2D_DESC texDesc = {};
	texDesc.Width = e.GetWidth();
	texDesc.Height = e.GetHeight();
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_D32_FLOAT;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	ComPtr<ID3D11Texture2D> tex;
	HR(m_device->CreateTexture2D(&texDesc, nullptr, &tex));

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = texDesc.Format;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;
	HR(m_device->CreateDepthStencilView(tex.Get(), &dsvDesc, &m_dsv));

	m_cameras[0].SetSize((float)e.GetWidth(), (float)e.GetHeight());
	m_cameras[1].SetSize((float)e.GetWidth(), (float)e.GetHeight());
	
	return false;
}

void TestApp::SetRendererStates()
{
	{
		D3D11_RASTERIZER_DESC desc = CD3D11_RASTERIZER_DESC(CD3D11_DEFAULT());
		HR(m_device->CreateRasterizerState(&desc, &m_rsState));
	}

	{
		D3D11_DEPTH_STENCIL_DESC desc = CD3D11_DEPTH_STENCIL_DESC(CD3D11_DEFAULT());
		HR(m_device->CreateDepthStencilState(&desc, &m_dsState));
	}

	{
		D3D11_BLEND_DESC desc = CD3D11_BLEND_DESC(CD3D11_DEFAULT());
		HR(m_device->CreateBlendState(&desc, &m_blendState));
	}
}

void TestApp::SetShaders()
{
	{
		ComPtr<ID3DBlob> byteCode;
		ComPtr<ID3DBlob> errorBlob;
		std::string src = Utils::TextReader::Read("res/shaders/basic.vs.hlsl");
		HR_MSG(D3DCompile(src.data(), src.size(), nullptr, nullptr, nullptr, "main", "vs_4_0",
			0, 0, &byteCode, &errorBlob), static_cast<const char*>(errorBlob->GetBufferPointer()));

		HR(m_device->CreateVertexShader(byteCode->GetBufferPointer(), byteCode->GetBufferSize(), nullptr, &m_vs));

		D3D11_INPUT_ELEMENT_DESC elementDescs[2] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		HR(m_device->CreateInputLayout(elementDescs, 2, byteCode->GetBufferPointer(),
			byteCode->GetBufferSize(), &m_inputLayout));
	}

	
	{
		ComPtr<ID3DBlob> byteCode;
		ComPtr<ID3DBlob> errorBlob;
		std::string src = Utils::TextReader::Read("res/shaders/basic.ps.hlsl");
		HR_MSG(D3DCompile(src.data(), src.size(), nullptr, nullptr, nullptr, "main", "ps_4_0",
			0, 0, &byteCode, &errorBlob), static_cast<const char*>(errorBlob->GetBufferPointer()));


		HR(m_device->CreatePixelShader(byteCode->GetBufferPointer(), byteCode->GetBufferSize(), nullptr, &m_ps));
	}


	{
		ComPtr<ID3DBlob> byteCode;
		ComPtr<ID3DBlob> errorBlob;
		std::string src = Utils::TextReader::Read("res/shaders/world_grid.vs.hlsl");
		HR_MSG(D3DCompile(src.data(), src.size(), nullptr, nullptr, nullptr, "main", "vs_4_0",
			0, 0, &byteCode, &errorBlob), static_cast<const char*>(errorBlob->GetBufferPointer()));

		HR(m_device->CreateVertexShader(byteCode->GetBufferPointer(), byteCode->GetBufferSize(), nullptr, &m_wgVS));

		D3D11_INPUT_ELEMENT_DESC elementDescs[2] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		HR(m_device->CreateInputLayout(elementDescs, 1, byteCode->GetBufferPointer(),
			byteCode->GetBufferSize(), &m_wgInputLayout));
	}

	{
		ComPtr<ID3DBlob> byteCode;
		ComPtr<ID3DBlob> errorBlob;
		std::string src = Utils::TextReader::Read("res/shaders/color.vs.hlsl");
		HR_MSG(D3DCompile(src.data(), src.size(), nullptr, nullptr, nullptr, "main", "vs_4_0",
			0, 0, &byteCode, &errorBlob), static_cast<const char*>(errorBlob->GetBufferPointer()));

		HR(m_device->CreateVertexShader(byteCode->GetBufferPointer(), byteCode->GetBufferSize(), nullptr, &m_colorVS));

		D3D11_INPUT_ELEMENT_DESC elementDescs[2] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		HR(m_device->CreateInputLayout(elementDescs, 1, byteCode->GetBufferPointer(),
			byteCode->GetBufferSize(), &m_colorInputLayout));
	}

	{
		ComPtr<ID3DBlob> byteCode;
		ComPtr<ID3DBlob> errorBlob;
		std::string src = Utils::TextReader::Read("res/shaders/color.ps.hlsl");
		HR_MSG(D3DCompile(src.data(), src.size(), nullptr, nullptr, nullptr, "main", "ps_4_0",
			0, 0, &byteCode, &errorBlob), static_cast<const char*>(errorBlob->GetBufferPointer()));


		HR(m_device->CreatePixelShader(byteCode->GetBufferPointer(), byteCode->GetBufferSize(), nullptr, &m_colorPS));
	}
}

void TestApp::SetBuffers()
{
	// cube vertices
	{
		std::array<float, 168> cubeVert =
		{
			//  back		             						  
			 0.5f,  0.5f, 0.5f,  1.0f, 0.0f, 0.0f, 1.0f,
			-0.5f,  0.5f, 0.5f,	 1.0f, 0.0f, 0.0f, 1.0f,
			-0.5f, -0.5f, 0.5f,	 1.0f, 0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, 0.5f,	 1.0f, 0.0f, 0.0f, 1.0f,

			// front
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f,

			// bottom			     	      
			 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f, 1.0f,

			// top		      	     					  
			-0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f, 1.0f,

			// left				     	     
			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f, 1.0f,

			// right			             
			 0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f, 1.0f,
		};

		D3D11_BUFFER_DESC desc = {};
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.ByteWidth = (uint32_t)cubeVert.size() * sizeof(float);
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 7 * sizeof(float);
		desc.Usage = D3D11_USAGE_DEFAULT;

		D3D11_SUBRESOURCE_DATA srd = {};
		srd.pSysMem = cubeVert.data();
		HR(m_device->CreateBuffer(&desc, &srd, &m_vb));
	}

	// cube indices
	{
		auto cubeInd = Utils::BasicMesh::CreateCubeIndices();

		D3D11_BUFFER_DESC desc = {};
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		desc.ByteWidth = (uint32_t)cubeInd.size() * sizeof(uint32_t);
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		desc.StructureByteStride = sizeof(uint32_t);
		desc.Usage = D3D11_USAGE_DEFAULT;

		D3D11_SUBRESOURCE_DATA srd = {};
		srd.pSysMem = cubeInd.data();
		HR(m_device->CreateBuffer(&desc, &srd, &m_ib));
	}

	// wolrd grid
	{
		const float max = 18.0f;
		const float widthPerGrid = 1.0f;
		const float depthPerGrid = 1.0f;

		std::vector<float> vert;
		std::vector<uint32_t> ind;
		for (int i = 0; i <= max; i++)
		{
			vert.emplace_back(-(max / 2) + widthPerGrid * i);
			vert.emplace_back(0.0f);
			vert.emplace_back(-(max / 2));

			vert.emplace_back(-(max / 2) + widthPerGrid * i);
			vert.emplace_back(0.0f);
			vert.emplace_back(max / 2);

			vert.emplace_back(-(max / 2.0f));
			vert.emplace_back(0.0f);
			vert.emplace_back(-(max / 2) + depthPerGrid * i);

			vert.emplace_back(max / 2.0f);
			vert.emplace_back(0.0f);
			vert.emplace_back(-(max / 2) + depthPerGrid * i);
		}


		for (int i = 0; i < vert.size(); i++)
		{
			ind.emplace_back(i);
		}
		
		D3D11_BUFFER_DESC desc = {};
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.ByteWidth = (uint32_t)vert.size() * sizeof(float);
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 3 * sizeof(float);
		desc.Usage = D3D11_USAGE_DEFAULT;

		D3D11_SUBRESOURCE_DATA srd = {};
		srd.pSysMem = vert.data();
		HR(m_device->CreateBuffer(&desc, &srd, &m_wgVB));


		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		desc.ByteWidth = (uint32_t)ind.size() * sizeof(uint32_t);
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		desc.StructureByteStride = sizeof(uint32_t);
		desc.Usage = D3D11_USAGE_DEFAULT;

		srd.pSysMem = ind.data();
		HR(m_device->CreateBuffer(&desc, &srd, &m_wgIB));
	}



	{
		D3D11_BUFFER_DESC desc = {};
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.ByteWidth = sizeof(Matrix);
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		HR(m_device->CreateBuffer(&desc, nullptr, &m_basicVSSysCBuf));
	}

	{
		D3D11_BUFFER_DESC desc = {};
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.ByteWidth = sizeof(Matrix);
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		HR(m_device->CreateBuffer(&desc, nullptr, &m_basicVSEntCBuf));
	}

	{
		D3D11_BUFFER_DESC desc = {};
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.ByteWidth = sizeof(Matrix);
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		HR(m_device->CreateBuffer(&desc, nullptr, &m_wgVSSysCBuf));
	}

	{
		D3D11_BUFFER_DESC desc = {};
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.ByteWidth = sizeof(Matrix);
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		HR(m_device->CreateBuffer(&desc, nullptr, &m_colorVSSysCBuf));
	}

	{
		D3D11_BUFFER_DESC desc = {};
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.ByteWidth = sizeof(Matrix);
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		HR(m_device->CreateBuffer(&desc, nullptr, &m_colorVSEntCBuf));
	}

	{
		D3D11_BUFFER_DESC desc = {};
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.ByteWidth = sizeof(Vector);
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		HR(m_device->CreateBuffer(&desc, nullptr, &m_colorPSEntCBuf));
	}
}

void TestApp::DrawWorldGrid()
{
	m_context->VSSetShader(m_wgVS.Get(), nullptr, 0);
	m_context->PSSetShader(m_colorPS.Get(), nullptr, 0);
	m_context->IASetInputLayout(m_wgInputLayout.Get());

	m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	// Set world_grid.vs System CBuf
	{
		m_context->VSSetConstantBuffers(0, 1, m_wgVSSysCBuf.GetAddressOf());
		D3D11_MAPPED_SUBRESOURCE msr = {};
		HR(m_context->Map(m_wgVSSysCBuf.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr));
		memcpy(msr.pData, &MatTranspose(m_cameras[m_activeCamera].GetViewProjection()), sizeof(Matrix));
		m_context->Unmap(m_wgVSSysCBuf.Get(), 0);
	}

	// Set color.ps ent cbuf
	{
		Vector color(0.5f, 0.5f, 0.5f, 1.0f);
		m_context->PSSetConstantBuffers(0, 1, m_colorPSEntCBuf.GetAddressOf());
		D3D11_MAPPED_SUBRESOURCE msr = {};
		HR(m_context->Map(m_colorPSEntCBuf.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr));
		memcpy(msr.pData, &color, sizeof(Vector));
		m_context->Unmap(m_colorPSEntCBuf.Get(), 0);
	}

	// Set Vertex and Index Buffer
	{
		const uint32_t offset = 0;
		const uint32_t stride = 3 * sizeof(float);
		m_context->IASetVertexBuffers(0, 1, m_wgVB.GetAddressOf(), &stride, &offset);

		m_context->IASetIndexBuffer(m_wgIB.Get(), DXGI_FORMAT_R32_UINT, 0);
	}

	m_context->DrawIndexed(80, 0, 0);
}

void TestApp::DrawCameraFrustum()
{
	m_context->VSSetShader(m_colorVS.Get(), nullptr, 0);
	m_context->PSSetShader(m_colorPS.Get(), nullptr, 0);
	m_context->IASetInputLayout(m_colorInputLayout.Get());

	m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	// Set world_grid.vs System CBuf
	{
		m_context->VSSetConstantBuffers(0, 1, m_colorVSSysCBuf.GetAddressOf());
		D3D11_MAPPED_SUBRESOURCE msr = {};
		HR(m_context->Map(m_colorVSSysCBuf.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr));
		memcpy(msr.pData, &MatTranspose(m_cameras[m_activeCamera].GetViewProjection()), sizeof(Matrix));
		m_context->Unmap(m_colorVSSysCBuf.Get(), 0);
	}

	// Set world_grid.vs System CBuf
	{
		CameraDesc cam = m_cameras[!m_activeCamera].GetDesc();
		Quaternion rotQuat = QuatRotationRollPitchYaw(ToRadians(cam.rotation.x), ToRadians(cam.rotation.y), ToRadians(cam.rotation.z));
		Matrix transform = MatRotationQuaternion(rotQuat) * MatTranslate(cam.position.x, cam.position.y, cam.position.z);

		m_context->VSSetConstantBuffers(1, 1, m_colorVSEntCBuf.GetAddressOf());
		D3D11_MAPPED_SUBRESOURCE msr = {};
		HR(m_context->Map(m_colorVSEntCBuf.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr));
		memcpy(msr.pData, &MatTranspose(transform), sizeof(Matrix));
		m_context->Unmap(m_colorVSEntCBuf.Get(), 0);
	}

	// Set color.ps ent cbuf
	{
		Vector color(0.5f, 0.0f, 0.0f, 1.0f);
		m_context->PSSetConstantBuffers(0, 1, m_colorPSEntCBuf.GetAddressOf());
		D3D11_MAPPED_SUBRESOURCE msr = {};
		HR(m_context->Map(m_colorPSEntCBuf.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr));
		memcpy(msr.pData, &color, sizeof(Vector));
		m_context->Unmap(m_colorPSEntCBuf.Get(), 0);
	}

	// Set Vertex and Index Buffer
	{
		const uint32_t offset = 0;
		const uint32_t stride = 3 * sizeof(float);
		auto vb = m_cameras[!m_activeCamera].GetFrustumVB();
		m_context->IASetVertexBuffers(0, 1, &vb, &stride, &offset);

		m_context->IASetIndexBuffer(m_cameras[!m_activeCamera].GetFrustumIB(), DXGI_FORMAT_R32_UINT, 0);
	}

	m_context->DrawIndexed(75, 0, 0);
}
