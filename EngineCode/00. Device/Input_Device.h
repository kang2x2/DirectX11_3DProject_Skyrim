#ifndef InputDev_h__
#define InputDev_h__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CInput_Device : public CBase
{
	DECLARE_SINGLETON(CInput_Device)

public:
	enum MOUSEKEYSTATE { MKS_LBUTTON, MKS_RBUTTON, MKS_WHEELBUTTON, MKS_XBUTTON, MKS_END };
	enum MOUSEMOVESTATE { MMS_X, MMS_Y, MMS_WHEEL, MMS_END };

private:
	CInput_Device(void);

	virtual ~CInput_Device(void) = default;

public:
	_bool   Get_DIKeyDown(_ubyte byKeyID);
	_bool   Get_DIKeyUp(_ubyte byKeyID);
	_bool   Get_DIKeyPress(_ubyte byKeyID);
	_byte	Get_DIKeyState(_ubyte byKeyID) { return m_byKeyState[byKeyID]; }
	
	_byte	Get_DIMouseState(MOUSEKEYSTATE eMouse) { return m_tMouseState.rgbButtons[eMouse]; }
	_bool   Get_DIMouseDown(MOUSEKEYSTATE eMouse);
	_long	Get_DIMouseMove(MOUSEMOVESTATE eMouseState)
	{
		return *(((_long*)&m_tMouseState) + eMouseState);
	}

public:
	HRESULT Ready_Input_Device(HINSTANCE hInst, HWND hWnd);
	void	Update_InputDev(void);

private:
	LPDIRECTINPUT8			m_pInputSDK = nullptr;

private:
	LPDIRECTINPUTDEVICE8	m_pKeyBoard = nullptr;
	LPDIRECTINPUTDEVICE8	m_pMouse = nullptr;

private:
	_byte					m_curKey = 0;
	_bool					m_byKeyState[256];		// 키보드에 있는 모든 키값을 저장하기 위한 변수
	
	DIMOUSESTATE			m_tMouseState;
	_byte					m_curMouseKey = MKS_END; // 가장 마지막에 눌린 마우스 버튼.

public:
	virtual void	Free(void);

};
END
#endif // InputDev_h__
