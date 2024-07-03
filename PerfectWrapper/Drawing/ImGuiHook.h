#pragma once
#include "..//Global.h"
#include "Drawing.h"
#include "..//MinHook/MinHook.h"
#include "..//ImGui/imgui.h"
#include "..//Indicium/IndiciumCore.h"
#include "..//ImGui/imgui_impl_win32.h"
#include "..//ImGui/imgui_impl_dx9.h"
#include "..//ImGui/imgui_impl_dx10.h"
#include "..//ImGui/imgui_impl_dx11.h"
#include "..//Indicium/IndiciumDirect3D9.h"
#include "..//Indicium/IndiciumDirect3D10.h"
#include "..//Indicium/IndiciumDirect3D11.h"
using namespace Drawing;


typedef LRESULT(WINAPI* t_WindowProc)(
	_In_ HWND hWnd,
	_In_ UINT Msg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
	);

LRESULT WINAPI DetourDefWindowProc(
	_In_ HWND hWnd,
	_In_ UINT Msg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
);

LRESULT WINAPI DetourWindowProc(
	_In_ HWND hWnd,
	_In_ UINT Msg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
);

void HookWindowProc(HWND hWnd);
void RenderScene();

bool ImGui_ImplWin32_UpdateMouseCursor();
IMGUI_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


EVT_INDICIUM_GAME_HOOKED EvtIndiciumGameHooked;
EVT_INDICIUM_GAME_UNHOOKED EvtIndiciumGameUnhooked;

EVT_INDICIUM_D3D9_PRESENT EvtIndiciumD3D9Present;
EVT_INDICIUM_D3D9_RESET EvtIndiciumD3D9PreReset;
EVT_INDICIUM_D3D9_RESET EvtIndiciumD3D9PostReset;
EVT_INDICIUM_D3D9_PRESENT_EX EvtIndiciumD3D9PresentEx;
EVT_INDICIUM_D3D9_RESET_EX EvtIndiciumD3D9PreResetEx;
EVT_INDICIUM_D3D9_RESET_EX EvtIndiciumD3D9PostResetEx;

EVT_INDICIUM_D3D10_PRESENT EvtIndiciumD3D10Present;
EVT_INDICIUM_D3D10_RESIZE_BUFFERS EvtIndiciumD3D10PreResizeBuffers;
EVT_INDICIUM_D3D10_RESIZE_BUFFERS EvtIndiciumD3D10PostResizeBuffers;

EVT_INDICIUM_D3D11_PRE_PRESENT EvtIndiciumD3D11Present;
EVT_INDICIUM_D3D11_PRE_RESIZE_BUFFERS EvtIndiciumD3D11PreResizeBuffers;
EVT_INDICIUM_D3D11_POST_RESIZE_BUFFERS EvtIndiciumD3D11PostResizeBuffers;

/**
 * \fn	TOGGLE_STATE(int key, bool& toggle)
 *
 * \brief	Overly complicated key toggle helper
 *
 * \author	Benjamin "Nefarius" Höglinger-Stelzer
 * \date	7/09/2018
 *
 * \param 		  	key   	The key.
 * \param [in,out]	toggle	The value to toggle.
 */
VOID
FORCEINLINE
TOGGLE_STATE(int key, bool& toggle)
{
	static auto pressedPast = false, pressedNow = false;

	if (GetAsyncKeyState(key) & 0x8000)
	{
		pressedNow = true;
	}
	else
	{
		pressedPast = false;
		pressedNow = false;
	}

	if (!pressedPast && pressedNow)
	{
		toggle = !toggle;

		pressedPast = true;
	}
}

static t_WindowProc OriginalDefWindowProc = nullptr;
static t_WindowProc OriginalWindowProc = nullptr;
static PINDICIUM_ENGINE engine = nullptr;
//RenderText(ImDrawList* draw_list, float size, ImVec2 pos, ImU32 col, const ImVec4& clip_rect, const char* text_begin, const char* text_end, float wrap_width = 0.0f, bool cpu_fine_clip = false) const;
float RenderText(const std::string& text, const D3DVector& position, float size, const D3DColor& color, bool center, bool outline, float transparency)
{
	auto Drawlist = ImGui::GetBackgroundDrawList();

	std::stringstream stream(text);
	std::string line;

	float y = 0.0f;
	int i = 0;

	ImGuiContext* sexx = ImGui::GetCurrentContext();

	while (std::getline(stream, line))
	{
		ImVec2 textSize = sexx->Font->CalcTextSizeA(size, FLT_MAX, 0.0f, line.c_str());

		if (center)
		{
			if (outline)
			{
				//Drawlist->AddText(0, size, ImVec2(position.x - textSize.x / 2.0f + 1, position.y + textSize.y * i + 1), ImColor(0.0f, 0.0f, 0.0f, transp), line.c_str());
				//Drawlist->AddText(0, size, ImVec2(position.x - textSize.x / 2.0f - 1, position.y + textSize.y * i - 1), ImColor(0.0f, 0.0f, 0.0f, transp), line.c_str());
				//Drawlist->AddText(0, size, ImVec2(position.x - textSize.x / 2.0f + 1, position.y + textSize.y * i - 1), ImColor(0.0f, 0.0f, 0.0f, transp), line.c_str());
				//Drawlist->AddText(0, size, ImVec2(position.x - textSize.x / 2.0f - 1, position.y + textSize.y * i + 1), ImColor(0.0f, 0.0f, 0.0f, transp), line.c_str());
			}
			Drawlist->AddText(0, size, ImVec2(position.x - (textSize.x / 2.0f), position.y + textSize.y * i), ImColor(color.r, color.g, color.b, transparency), line.c_str());
		}
		else
		{
			if (outline)
			{
				//Drawlist->AddText(0, size, ImVec2(position.x + 1, position.y + textSize.y * i + 1), color, line.c_str());
				//Drawlist->AddText(0, size, ImVec2(position.x - 1, position.y + textSize.y * i - 1), color, line.c_str());
				//Drawlist->AddText(0, size, ImVec2(position.x + 1, position.y + textSize.y * i - 1), color, line.c_str());
				//Drawlist->AddText(0, size, ImVec2(position.x - 1, position.y + textSize.y * i + 1), color, line.c_str());
			}
			Drawlist->AddText(0, size, ImVec2(position.x, position.y + textSize.y * i), ImColor(color.r, color.g, color.b, transparency), line.c_str());
		}

		y = position.y + textSize.y * (i + 1);
		i++;
	}

	return y;
}

void EvtIndiciumGameHooked(PINDICIUM_ENGINE EngineHandle, const INDICIUM_D3D_VERSION GameVersion)
{
	MH_Initialize();
#if DebugDrawingLib
	printf("Loading ImGui plugin\n");
#endif
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	INDICIUM_D3D9_EVENT_CALLBACKS d3d9;
	INDICIUM_D3D9_EVENT_CALLBACKS_INIT(&d3d9);
	d3d9.EvtIndiciumD3D9PrePresent = EvtIndiciumD3D9Present;
	d3d9.EvtIndiciumD3D9PreReset = EvtIndiciumD3D9PreReset;
	d3d9.EvtIndiciumD3D9PostReset = EvtIndiciumD3D9PostReset;
	d3d9.EvtIndiciumD3D9PrePresentEx = EvtIndiciumD3D9PresentEx;
	d3d9.EvtIndiciumD3D9PreResetEx = EvtIndiciumD3D9PreResetEx;
	d3d9.EvtIndiciumD3D9PostResetEx = EvtIndiciumD3D9PostResetEx;
	INDICIUM_D3D10_EVENT_CALLBACKS d3d10;
	INDICIUM_D3D10_EVENT_CALLBACKS_INIT(&d3d10);
	d3d10.EvtIndiciumD3D10PrePresent = EvtIndiciumD3D10Present;
	d3d10.EvtIndiciumD3D10PreResizeBuffers = EvtIndiciumD3D10PreResizeBuffers;
	d3d10.EvtIndiciumD3D10PostResizeBuffers = EvtIndiciumD3D10PostResizeBuffers;
	INDICIUM_D3D11_EVENT_CALLBACKS d3d11;
	INDICIUM_D3D11_EVENT_CALLBACKS_INIT(&d3d11);
	d3d11.EvtIndiciumD3D11PrePresent = EvtIndiciumD3D11Present;
	d3d11.EvtIndiciumD3D11PreResizeBuffers = EvtIndiciumD3D11PreResizeBuffers;
	d3d11.EvtIndiciumD3D11PostResizeBuffers = EvtIndiciumD3D11PostResizeBuffers;
	switch (GameVersion)
	{
	case IndiciumDirect3DVersion9:
		IndiciumEngineSetD3D9EventCallbacks(EngineHandle, &d3d9);
		break;
	case IndiciumDirect3DVersion10:
		IndiciumEngineSetD3D10EventCallbacks(EngineHandle, &d3d10);
		break;
	case IndiciumDirect3DVersion11:
		IndiciumEngineSetD3D11EventCallbacks(EngineHandle, &d3d11);
		break;
	}

}

void EvtIndiciumGameUnhooked()
{
#ifdef WNDPROC_HOOK
	auto& logger = Logger::get(__func__);

	if (MH_DisableHook(MH_ALL_HOOKS) != MH_OK)
	{
#if DebugDrawingLib
		printf("Couldn't disable hooks, host process might crash\n");
#endif
		return;
	}

	printf("Hooks disabled\n");

	if (MH_Uninitialize() != MH_OK)
	{
#if DebugDrawingLib
		logger.fatal("Couldn't shut down hook engine, host process might crash");
#endif
		return;
	}
#endif
}

void RenderScene()
{
	auto Drawlist = ImGui::GetBackgroundDrawList();//Redo this
	size_t listsize = DrawingsVector.size();
	if (listsize > 0)
	{
		for (auto sex : DrawingsVector)
		{
			if (sex->Visible)
			{
				switch (sex->type)
				{
				case DrawingEnum::Text:
				{
					const auto text = (D3DText*)sex->Object;
					RenderText(text->Text->c_str(), text->Position, text->Size, text->Color, text->Center, text->Outline, text->Transparency);
					//Drawlist->AddText(0, text->Size, ImVec2(text->Position.x, text->Position.y), ImColor(text->Color.r, text->Color.g, text->Color.b), text->Text->c_str());
					break;
				}
				case DrawingEnum::Circle:
				{
					const auto Circle = reinterpret_cast<D3DCircle*>(sex->Object);
					if (Circle->Filled)
					{
						Drawlist->AddCircleFilled(ImVec2(Circle->Position.x, Circle->Position.y), Circle->Radius, ImColor(Circle->Color.r, Circle->Color.g, Circle->Color.b, Circle->Transparency), Circle->NumSides);
					}
					else
					{
						Drawlist->AddCircle(ImVec2(Circle->Position.x, Circle->Position.y), Circle->Radius, ImColor(Circle->Color.r, Circle->Color.g, Circle->Color.b, Circle->Transparency), Circle->NumSides, Circle->Thickness);
					}
					break;
				}
				case DrawingEnum::Line:
				{
					const auto Line = reinterpret_cast<D3DLine*>(sex->Object);
					Drawlist->AddLine(ImVec2(Line->From.x, Line->From.y), ImVec2(Line->To.x, Line->To.y), ImColor(Line->Color.r, Line->Color.g, Line->Color.b, Line->Transparency), Line->Thickness);
					break;
				}
				case DrawingEnum::Square:
				{
					const auto Square = reinterpret_cast<D3DSquare*>(sex->Object);
					if (Square->Filled)
					{
						Drawlist->AddRectFilled(ImVec2(Square->Position.x, Square->Position.y), ImVec2(Square->Size.x + Square->Position.x, Square->Size.y + Square->Position.y), ImColor(Square->Color.r, Square->Color.g, Square->Color.b, Square->Transparency), 0, 0);
					}
					else
					{
						Drawlist->AddRect(ImVec2(Square->Position.x, Square->Position.y), ImVec2(Square->Size.x + Square->Position.x, Square->Size.y + Square->Position.y), ImColor(Square->Color.r, Square->Color.g, Square->Color.b, Square->Transparency), 0, 0, Square->Thickness);
					}
					break;
				}
				}
			}
		}
	}
}

#pragma region D3D9(Ex)

void EvtIndiciumD3D9Present(
	LPDIRECT3DDEVICE9   pDevice,
	const RECT* pSourceRect,
	const RECT* pDestRect,
	HWND                hDestWindowOverride,
	const RGNDATA* pDirtyRegion
)
{
	static auto initialized = false;
	static bool show_overlay = true;
	static std::once_flag init;

	//
	// This section is only called once to initialize ImGui
	// 
	std::call_once(init, [&](LPDIRECT3DDEVICE9 pd3dDevice)
		{
			D3DDEVICE_CREATION_PARAMETERS params;

			const auto hr = pd3dDevice->GetCreationParameters(&params);
			if (FAILED(hr))
			{
#if DebugDrawingLib
				printf("Couldn't get creation parameters from device\n");
#endif
				return;
			}

			ImGui_ImplWin32_Init(params.hFocusWindow);
			ImGui_ImplDX9_Init(pd3dDevice);
#if DebugDrawingLib
			printf("ImGui (DX9) initialized\n");
#endif

			HookWindowProc(params.hFocusWindow);

			initialized = true;

		}, pDevice);

	if (!initialized)
		return;

	TOGGLE_STATE(VK_F12, show_overlay);
	if (!show_overlay)
		return;

	// Start the Dear ImGui frame
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	RenderScene();

	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

void EvtIndiciumD3D9PreReset(
	LPDIRECT3DDEVICE9       pDevice,
	D3DPRESENT_PARAMETERS* pPresentationParameters
)
{
	ImGui_ImplDX9_InvalidateDeviceObjects();
}

void EvtIndiciumD3D9PostReset(
	LPDIRECT3DDEVICE9       pDevice,
	D3DPRESENT_PARAMETERS* pPresentationParameters
)
{
	ImGui_ImplDX9_CreateDeviceObjects();
}

void EvtIndiciumD3D9PresentEx(
	LPDIRECT3DDEVICE9EX     pDevice,
	const RECT* pSourceRect,
	const RECT* pDestRect,
	HWND                    hDestWindowOverride,
	const RGNDATA* pDirtyRegion,
	DWORD                   dwFlags
)
{
	static auto initialized = false;
	static bool show_overlay = true;
	static std::once_flag init;

	//
	// This section is only called once to initialize ImGui
	// 
	std::call_once(init, [&](LPDIRECT3DDEVICE9EX pd3dDevice)
		{
			D3DDEVICE_CREATION_PARAMETERS params;

			const auto hr = pd3dDevice->GetCreationParameters(&params);
			if (FAILED(hr))
			{
#if DebugDrawingLib
				printf("Couldn't get creation parameters from device\n");
#endif
				return;
			}

			ImGui_ImplWin32_Init(params.hFocusWindow);
			ImGui_ImplDX9_Init(pd3dDevice);
#if DebugDrawingLib
			printf("ImGui (DX9Ex) initialized\n");
#endif

			HookWindowProc(params.hFocusWindow);

			initialized = true;

		}, pDevice);

	if (!initialized)
		return;

	TOGGLE_STATE(VK_F12, show_overlay);
	if (!show_overlay)
		return;

	// Start the Dear ImGui frame
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	RenderScene();

	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

void EvtIndiciumD3D9PreResetEx(
	LPDIRECT3DDEVICE9EX     pDevice,
	D3DPRESENT_PARAMETERS* pPresentationParameters,
	D3DDISPLAYMODEEX* pFullscreenDisplayMode
)
{
	ImGui_ImplDX9_InvalidateDeviceObjects();
}

void EvtIndiciumD3D9PostResetEx(
	LPDIRECT3DDEVICE9EX     pDevice,
	D3DPRESENT_PARAMETERS* pPresentationParameters,
	D3DDISPLAYMODEEX* pFullscreenDisplayMode
)
{
	ImGui_ImplDX9_CreateDeviceObjects();
}
#pragma endregion

#pragma region D3D10

void EvtIndiciumD3D10Present(
	IDXGISwapChain* pSwapChain,
	UINT            SyncInterval,
	UINT            Flags
)
{
	static auto initialized = false;
	static bool show_overlay = true;
	static std::once_flag init;

	//
	// This section is only called once to initialize ImGui
	// 
	std::call_once(init, [&](IDXGISwapChain* pChain)
		{
#if DebugDrawingLib
			printf("Grabbing device and context pointers\n");
#endif

			ID3D10Device* pDevice;
			if (FAILED(D3D10_DEVICE_FROM_SWAPCHAIN(pChain, &pDevice)))
			{
#if DebugDrawingLib
				printf("Couldn't get device from swapchain\n");
#endif
				return;
			}

			DXGI_SWAP_CHAIN_DESC sd;
			pChain->GetDesc(&sd);
#if DebugDrawingLib
			printf("Initializing ImGui\n");
#endif

			ImGui_ImplWin32_Init(sd.OutputWindow);
			ImGui_ImplDX10_Init(pDevice);
#if DebugDrawingLib
			printf("ImGui (DX10) initialized\n");
#endif

			HookWindowProc(sd.OutputWindow);

			initialized = true;

		}, pSwapChain);

	if (!initialized)
		return;

	TOGGLE_STATE(VK_F12, show_overlay);
	if (!show_overlay)
		return;


	// Start the Dear ImGui frame
	ImGui_ImplDX10_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	RenderScene();

	ImGui_ImplDX10_RenderDrawData(ImGui::GetDrawData());
}

void EvtIndiciumD3D10PreResizeBuffers(
	IDXGISwapChain* pSwapChain,
	UINT            BufferCount,
	UINT            Width,
	UINT            Height,
	DXGI_FORMAT     NewFormat,
	UINT            SwapChainFlags
)
{
	ImGui_ImplDX10_InvalidateDeviceObjects();
}

void EvtIndiciumD3D10PostResizeBuffers(
	IDXGISwapChain* pSwapChain,
	UINT            BufferCount,
	UINT            Width,
	UINT            Height,
	DXGI_FORMAT     NewFormat,
	UINT            SwapChainFlags
)
{
	ImGui_ImplDX10_CreateDeviceObjects();
}

#pragma endregion

#pragma region D3D11

// TODO: lazy global, improve
static ID3D11RenderTargetView* g_d3d11_mainRenderTargetView = nullptr;

void EvtIndiciumD3D11Present(
	IDXGISwapChain* pSwapChain,
	UINT						SyncInterval,
	UINT						Flags,
	PINDICIUM_EVT_PRE_EXTENSION Extension
)
{
	static auto initialized = false;
	static bool show_overlay = true;
	static std::once_flag init;

	static ID3D11DeviceContext* pContext;

	//
	// This section is only called once to initialize ImGui
	// 
	std::call_once(init, [&](IDXGISwapChain* pChain)
		{
#if DebugDrawingLib
			printf("Grabbing device and context pointers\n");
#endif

			ID3D11Device* pDevice;
			if (FAILED(D3D11_DEVICE_IMMEDIATE_CONTEXT_FROM_SWAPCHAIN(pChain, &pDevice, &pContext)))
			{
#if DebugDrawingLib
				printf("Couldn't get device and context from swapchain\n");
#endif
				return;
			}

			ID3D11Texture2D* pBackBuffer;
			pChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
			pDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_d3d11_mainRenderTargetView);
			pBackBuffer->Release();

			DXGI_SWAP_CHAIN_DESC sd;
			pChain->GetDesc(&sd);
#if DebugDrawingLib
			printf("Initializing ImGui\n");
#endif

			ImGui_ImplWin32_Init(sd.OutputWindow);
			ImGui_ImplDX11_Init(pDevice, pContext);
#if DebugDrawingLib
			printf("ImGui (DX11) initialized\n");
#endif

			HookWindowProc(sd.OutputWindow);

			initialized = true;

		}, pSwapChain);

	if (!initialized)
		return;

	TOGGLE_STATE(VK_F12, show_overlay);
	if (!show_overlay)
		return;
	 //Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	pContext->OMSetRenderTargets(1, &g_d3d11_mainRenderTargetView, NULL);
	RenderScene();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

//
// Called prior to the original invocation of ResizeBuffers.
// 
void EvtIndiciumD3D11PreResizeBuffers(
	IDXGISwapChain* pSwapChain,
	UINT						BufferCount,
	UINT						Width,
	UINT						Height,
	DXGI_FORMAT					NewFormat,
	UINT						SwapChainFlags,
	PINDICIUM_EVT_PRE_EXTENSION Extension
)
{
	if (g_d3d11_mainRenderTargetView)
	{
		g_d3d11_mainRenderTargetView->Release();
		g_d3d11_mainRenderTargetView = nullptr;
	}
}

//
// Called after the original invocation of ResizeBuffers.
// 
void EvtIndiciumD3D11PostResizeBuffers(
	IDXGISwapChain* pSwapChain,
	UINT							BufferCount,
	UINT							Width,
	UINT							Height,
	DXGI_FORMAT						NewFormat,
	UINT							SwapChainFlags,
	PINDICIUM_EVT_POST_EXTENSION	Extension
)
{
	ID3D11Texture2D* pBackBuffer;
	ID3D11DeviceContext* pContext;
	ID3D11Device* pDevice;
	D3D11_DEVICE_IMMEDIATE_CONTEXT_FROM_SWAPCHAIN(pSwapChain, &pDevice, &pContext);

	pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	pDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_d3d11_mainRenderTargetView);
	pBackBuffer->Release();
}

#pragma endregion

#pragma region WNDPROC Hooking

void HookWindowProc(HWND hWnd)
{
#ifdef WNDPROC_HOOK

	auto& logger = Logger::get(__func__);

	MH_STATUS ret;

	if ((ret = MH_CreateHook(
		&DefWindowProcW,
		&DetourDefWindowProc,
		reinterpret_cast<LPVOID*>(&OriginalDefWindowProc))
		) != MH_OK)
	{
#if DebugDrawingLib
		printf("Couldn't create hook for DefWindowProcW: %lu\n", static_cast<ULONG>(ret));
#endif
		return;
	}

	if (ret == MH_OK && MH_EnableHook(&DefWindowProcW) != MH_OK)
	{
#if DebugDrawingLib
		printf("Couldn't enable DefWindowProcW hook\n");
#endif
	}

	if ((ret = MH_CreateHook(
		&DefWindowProcA,
		&DetourDefWindowProc,
		reinterpret_cast<LPVOID*>(&OriginalDefWindowProc))
		) != MH_OK)
	{
#if DebugDrawingLib
		printf("Couldn't create hook for DefWindowProcA: %lu\n", static_cast<ULONG>(ret));
#endif
		return;
	}

	if (ret == MH_OK && MH_EnableHook(&DefWindowProcA) != MH_OK)
	{
#if DebugDrawingLib
		printf("Couldn't enable DefWindowProcW hook\n");
#endif
	}

	auto lptrWndProc = reinterpret_cast<t_WindowProc>(GetWindowLongPtr(hWnd, GWLP_WNDPROC));

	if (MH_CreateHook(lptrWndProc, &DetourWindowProc, reinterpret_cast<LPVOID*>(&OriginalWindowProc)) != MH_OK)
	{
#if DebugDrawingLib
		printf("Couldn't create hook for GWLP_WNDPROC\n");
#endif
		return;
	}

	if (MH_EnableHook(lptrWndProc) != MH_OK)
	{
#if DebugDrawingLib
		printf("Couldn't enable GWLP_WNDPROC hook\n");
#endif
	}
#endif
}

LRESULT WINAPI DetourDefWindowProc(
	_In_ HWND hWnd,
	_In_ UINT Msg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
)
{
	static std::once_flag flag;
	std::call_once(flag, []() { 
#if DebugDrawingLib
		printf("++ DetourDefWindowProc called\n");
#endif
		});

	ImGui_ImplWin32_WndProcHandler(hWnd, Msg, wParam, lParam);

	return OriginalDefWindowProc(hWnd, Msg, wParam, lParam);
}

LRESULT WINAPI DetourWindowProc(
	_In_ HWND hWnd,
	_In_ UINT Msg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
)
{
	static std::once_flag flag;
	std::call_once(flag, []() { 
#if DebugDrawingLib
		printf("++ DetourWindowProc called\n");
#endif
		});

	ImGui_ImplWin32_WndProcHandler(hWnd, Msg, wParam, lParam);

	return OriginalWindowProc(hWnd, Msg, wParam, lParam);
}

#pragma endregion
