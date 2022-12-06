#include "Graphics.h"

Graphics* Graphics::instance = nullptr;
Graphics::Graphics(HWND hwnd)
{
	// Set instance
	instance = this;

	// Get screen size;
	RECT rc;
	GetClientRect(hwnd, &rc);
	UINT screenWidth = rc.right - rc.left;
	UINT screenHeight = rc.bottom - rc.top;
	this->screenWidth = static_cast<float>(screenWidth);
	this->screenHeight = static_cast<float>(screenHeight);


	HRESULT hr{ S_OK };
	// Create swapchain and device
	{
		UINT create_device_flags{ 0 };
		D3D_FEATURE_LEVEL feature_levels{ D3D_FEATURE_LEVEL_11_0 };
		DXGI_SWAP_CHAIN_DESC swap_chain_desc{};
		swap_chain_desc.BufferCount = 1;
		swap_chain_desc.BufferDesc.Width = screenWidth;
		swap_chain_desc.BufferDesc.Height = screenHeight;
		swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swap_chain_desc.BufferDesc.RefreshRate.Numerator = 60;
		swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;
		swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swap_chain_desc.OutputWindow = hwnd;
		swap_chain_desc.SampleDesc.Count = 1;
		swap_chain_desc.SampleDesc.Quality = 0;
		swap_chain_desc.Windowed = TRUE;
		hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, create_device_flags, &feature_levels, 1, D3D11_SDK_VERSION, &swap_chain_desc, swap_chain.GetAddressOf(), device.GetAddressOf(), NULL, device_context.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), L"CREATE SWAPCHAIN FAILED");
	}
	//Create renderTargetView
	{
		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_back_buffer;
		hr = swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(m_back_buffer.GetAddressOf()));
		_ASSERT_EXPR(SUCCEEDED(hr), L"GET BACK BUFFER FAILED");
		hr = device->CreateRenderTargetView(m_back_buffer.Get(), NULL, render_target_view.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), L"CREATE RENDERTARGETVIEW FAILED");
	}
	//Create depthStecilView
	{
		D3D11_TEXTURE2D_DESC m_depth_stencil_buffer_desc{};
		m_depth_stencil_buffer_desc.Width = screenWidth;
		m_depth_stencil_buffer_desc.Height = screenHeight;
		m_depth_stencil_buffer_desc.MipLevels = 1;
		m_depth_stencil_buffer_desc.ArraySize = 1;
		m_depth_stencil_buffer_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		m_depth_stencil_buffer_desc.SampleDesc.Count = 1;
		m_depth_stencil_buffer_desc.SampleDesc.Quality = 0;
		m_depth_stencil_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
		m_depth_stencil_buffer_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		m_depth_stencil_buffer_desc.CPUAccessFlags = 0;
		m_depth_stencil_buffer_desc.MiscFlags = 0;
		hr = device->CreateTexture2D(&m_depth_stencil_buffer_desc, NULL, depth_stencil_buffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), L"CREATE DEPTHSTECILBUFFER FAILED");
		hr = device->CreateDepthStencilView(depth_stencil_buffer.Get(), nullptr, depth_stencil_view.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), L"CREATE DEPTHSTECILVIEW FAILED");
		
	}
	//Create view port
	{
		D3D11_VIEWPORT viewport;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = static_cast<float>(screenWidth);
		viewport.Height = static_cast<float>(screenHeight);
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		device_context->RSSetViewports(1, &viewport);
	}

	{
		imguiClass = std::make_unique<ImGuiClass>(hwnd, device.Get(), device_context.Get());
		debugRenderer = std::make_unique<DebugRenderer>(device.Get());
		geometricPrimitive = std::make_unique<GeometricPrimitive>(device.Get());
	}
}