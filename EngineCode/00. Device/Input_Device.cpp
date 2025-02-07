#include "Input_Device.h"

IMPLEMENT_SINGLETON(CInput_Device)

Engine::CInput_Device::CInput_Device(void)
{

}

HRESULT Engine::CInput_Device::Ready_Input_Device(HINSTANCE hInst, HWND hWnd)
{

	// DInput 컴객체를 생성하는 함수
	if (FAILED(DirectInput8Create(hInst,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&m_pInputSDK,
		nullptr)))
		return E_FAIL;

	// 키보드 객체 생성
	if (FAILED(m_pInputSDK->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, nullptr)))
		return E_FAIL;

	// 생성된 키보드 객체의 대한 정보를 컴 객체에게 전달하는 함수
	m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard);

	// 장치에 대한 독점권을 설정해주는 함수, (클라이언트가 떠있는 상태에서 키 입력을 받을지 말지를 결정하는 함수)
	m_pKeyBoard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	// 장치에 대한 access 버전을 받아오는 함수
	m_pKeyBoard->Acquire();


	// 마우스 객체 생성
	if (FAILED(m_pInputSDK->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr)))
		return E_FAIL;

	// 생성된 마우스 객체의 대한 정보를 컴 객체에게 전달하는 함수
	m_pMouse->SetDataFormat(&c_dfDIMouse);

	// 장치에 대한 독점권을 설정해주는 함수, 클라이언트가 떠있는 상태에서 키 입력을 받을지 말지를 결정하는 함수
	m_pMouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	// 장치에 대한 access 버전을 받아오는 함수
	m_pMouse->Acquire();

	ZeroMemory(m_byKeyState, sizeof(_byte) * 256);

	return S_OK;
}

_bool CInput_Device::Get_DIKeyDown(_ubyte byKeyID)
{
	if (!m_byKeyState[byKeyID] && (GetAsyncKeyState(byKeyID) & 0x8000))
	{
		m_byKeyState[byKeyID] = !m_byKeyState[byKeyID];
		return true;
	}

	for (int i = 0; i < 256; ++i)
	{
		if (m_byKeyState[i] && !(GetAsyncKeyState(i) & 0x8000))
			m_byKeyState[i] = !m_byKeyState[i];
	}

	return false;
}

_bool CInput_Device::Get_DIKeyUp(_ubyte byKeyID)
{
	// 이전에 눌린 적이 있고, 현재는 눌리지 않은 경우
	if (m_byKeyState[byKeyID] && !(GetAsyncKeyState(byKeyID) & 0x8000))
	{
		m_byKeyState[byKeyID] = !m_byKeyState[byKeyID];
		return true;
	}

	for (int i = 0; i < 256; ++i)
	{
		if (!m_byKeyState[byKeyID] && (GetAsyncKeyState(byKeyID) & 0x8000))
			m_byKeyState[i] = !m_byKeyState[i];
	}


	return false;
}

_bool CInput_Device::Get_DIKeyPress(_ubyte byKeyID)
{
	if (GetAsyncKeyState(byKeyID) & 0x8000)
		return true;

	return false;
}

_bool CInput_Device::Get_DIMouseDown(MOUSEKEYSTATE eMouse)
{
	// 눌린 적 없고 지금 눌렀으면
	if (m_curMouseKey != eMouse && Get_DIMouseState(eMouse) & 0x80)
	{
		m_curMouseKey = eMouse;
		return true;
	}

	if(m_curMouseKey == eMouse && !(Get_DIMouseState(eMouse) & 0x80))
		m_curMouseKey = MKS_END;

	return false;
}

void Engine::CInput_Device::Update_InputDev(void)
{
	/* 매 프레임마다 키보드와 마우스의 상태를 저장해준다. */
	//m_pKeyBoard->GetDeviceState(256, m_byKeyState);
	m_pMouse->GetDeviceState(sizeof(m_tMouseState), &m_tMouseState);
}

void Engine::CInput_Device::Free(void)
{
	Safe_Release(m_pKeyBoard);
	Safe_Release(m_pMouse);
	Safe_Release(m_pInputSDK);
}

