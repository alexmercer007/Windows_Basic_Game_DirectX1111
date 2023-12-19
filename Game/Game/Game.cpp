// Game.cpp : Define el punto de entrada de la aplicación.
//

#include "framework.h"
#include "Game.h"
#include "windows.h"
#include "d3d11.h"
#include "DXGI.h"


#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "DXGI.lib")

LRESULT CALLBACK WinPro( HWND hwnd, UINT uMsg,  WPARAM wParam, LPARAM lParam ) {

	switch (uMsg) {

	case WM_CLOSE:

		DestroyWindow(hwnd);

		break;

	case WM_DESTROY:

		PostQuitMessage(0);

		break;

	default:

		return DefWindowProc(hwnd, uMsg, wParam, lParam);

	}

	return 0;

}

int APIENTRY wWinMain(
_In_ HINSTANCE hInstance, 
 _In_opt_ HINSTANCE hPrevInstance,
 _In_ PWSTR lpCmdLine,
 _In_ int nCmdShow) {

	const wchar_t windows [ ] = L"WinGame";
	 
	WNDCLASSEX wndclass = {};  

	wndclass.hInstance = hInstance;
	wndclass.lpfnWndProc = WinPro;
	wndclass.lpszClassName = windows;
	wndclass.style = CS_HREDRAW | CS_VREDRAW ;
	wndclass.cbSize = sizeof(WNDCLASSEX);

	 
	RegisterClassEx(&wndclass); 

	HWND hwnd = CreateWindowExW(

		0,
		windows,
		L"Game",
	    WS_OVERLAPPEDWINDOW,
		0,
		0,
		1200,
		720,
		nullptr,
		nullptr,
		hInstance,
		nullptr
	);

	ShowWindow(hwnd, nCmdShow);

	// Pointers

	IDXGIAdapter* adapter = nullptr;
	IDXGISwapChain* SwapChain = nullptr;
	
	ID3D11Device* device = nullptr;
	ID3D11DeviceContext* DeviceContext = nullptr;
	
	D3D_FEATURE_LEVEL levels[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_11_1 };
	D3D_FEATURE_LEVEL selectedFeatureLevel = D3D_FEATURE_LEVEL_11_0;

	ID3D11Texture2D* pTexture2d = nullptr;
	ID3D11RenderTargetView* pRenderTargetView = nullptr; 


	//SwapChainDesc

	DXGI_SWAP_CHAIN_DESC SwapChainDesc = {};

	ZeroMemory(&SwapChainDesc, sizeof(SwapChainDesc));

	SwapChainDesc.BufferDesc.Width = 1200;
	SwapChainDesc.BufferDesc.Height = 720;
	SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // Formato común para texturas RGBA de 8 bits por canal.
	SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	SwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	SwapChainDesc.BufferCount = 1; 
	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; 
	SwapChainDesc.SampleDesc.Count = 1; 
	SwapChainDesc.SampleDesc.Quality = 0; 
	SwapChainDesc.OutputWindow = hwnd;
	SwapChainDesc.Windowed = true;


	//CreateDeviceAndSwapChain
	
	HRESULT result = D3D11CreateDeviceAndSwapChain(

		adapter, 
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		levels,
		1,
		D3D11_SDK_VERSION,
		&SwapChainDesc,
		&SwapChain,
		&device,
		&selectedFeatureLevel,
		&DeviceContext);

	if (FAILED(result)) {

		MessageBox(

			hwnd,
			L"Error creating the device and swap chain. Please configure the code line correctly.",
			L"Error Critical",
			MB_OK | MB_ICONERROR);
	};

	// Retrieve the back buffer from the swap chain. This operation obtains a reference
    // to the 2D texture representing the back buffer, allowing for further operations 
    // or rendering to be performed on it. 

	result = SwapChain->GetBuffer (0, __uuidof(ID3D11Texture2D), (LPVOID*)&pTexture2d);
	
	if (FAILED(result)) {

		MessageBox(

			hwnd,
			L"Error GetBuffer.",
			L"Error Critical",
			MB_OK | MB_ICONERROR);
     };

    // Initialize Texture2D desc

	D3D11_TEXTURE2D_DESC Texture2dDes = {};

	Texture2dDes.Width = 1200;  // Ancho de la textura 
	Texture2dDes.Height = 720;  // Altura de la textura 
	Texture2dDes.MipLevels = 1; // Niveles de mip (puedes ajustar según tus necesidades) 
	Texture2dDes.ArraySize = 1; // Tamaño del array (puedes ajustar según tus necesidades) 
	Texture2dDes.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // Formato de la textura (puedes ajustar según tus necesidades)
	Texture2dDes.SampleDesc.Count = 1;  // Cantidad de muestras por píxel (puedes ajustar según tus necesidades) 
	Texture2dDes.SampleDesc.Quality = 0; // Calidad de las muestras (puedes ajustar según tus necesidades) 
	Texture2dDes.Usage = D3D11_USAGE_DEFAULT; // Uso predeterminado  
	Texture2dDes.BindFlags = D3D11_BIND_RENDER_TARGET; // Puedes ajustar según tus necesidades 
	Texture2dDes.CPUAccessFlags = 0; // Sin acceso desde la CPU  
	Texture2dDes.MiscFlags = 0; // Sin flags misceláneos 

	// Create Texture2D

	result = device->CreateTexture2D(
		
		&Texture2dDes, 
		nullptr, 
		&pTexture2d);

	if (FAILED(result)) {

		MessageBox(

			hwnd,
			L"Error CreateTexture2D. Please configure the code line correctly ",
			L"Error Critical",
			MB_OK | MB_ICONERROR);
	};

    // Initialize RenderTargetViewDesc

	D3D11_RENDER_TARGET_VIEW_DESC RenderTargetViewDesc = {};
	
	RenderTargetViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    RenderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D; // Puedes ajustar según tus necesidades

	// Creating CreateRenderTargetView

	result = device->CreateRenderTargetView(
		
		pTexture2d, 
		&RenderTargetViewDesc, 
		&pRenderTargetView);

	if (FAILED(result)) {

		MessageBox(

			hwnd,
			L"Error CreateTexture2D. Please configure the code line correctly ",
			L"Error Critical",
			MB_OK | MB_ICONERROR);
	};
	
	if (pTexture2d) pTexture2d->Release(); 

	// Set the render targets in the output-merger stage. This function binds the specified render target views 
    // to the output-merger stage of the graphics pipeline, allowing rendering to the associated textures.

	DeviceContext->OMSetRenderTargets( 

		1,
		&pRenderTargetView, 
		nullptr); 

	if (pRenderTargetView) pRenderTargetView->Release(); 

	// Define the viewport parameters. This structure specifies the transformation 
    // from normalized device coordinates to screen coordinates, affecting how the
    // rendered scene is mapped to the output window.
	
	D3D11_VIEWPORT vp;

	vp.MinDepth = 0.0f;
	vp.MaxDepth = 0.1f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Width = 1200;
	vp.Height = 720;

	MSG msg = {};

	while ( msg.message != WM_QUIT ) { 

		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			
			// Main loop: handle messages and render the frame.

			DeviceContext->ClearState();

		
			
			DeviceContext->Draw(0, 0);
			SwapChain->Present(1, 0);

		}
	}

	// Clear Memory

	if (adapter) adapter->Release(); 
	if (SwapChain) SwapChain->Release(); 
	if (device) device->Release(); 
	if (DeviceContext) DeviceContext->Release(); 

	return static_cast<int>(msg.wParam);

}