#pragma once

#include <filesystem>
#include <set>
#include <fstream>
#include <commdlg.h>

#include "Client_Defines.h"
#include "ImGui_Base.h"
#include "imgui.h"

BEGIN(Engine)

class CNavigation;

END

BEGIN(Client)

class CImGui_Tool final : public CImGui_Base
{
public:
	typedef struct tagBeforeFolderDesc
	{
		wstring strFolderName;
		wstring strType;
	}TOOL_BEFORE_FOLDERDESC;

	typedef struct tagAfterFolderDesc
	{
		const char* strFolderName;
		wstring strType;
	}TOOL_AFTER_FOLDERDESC;

	typedef struct tagAfterFileDesc
	{
		const char* m_strFileName;
		wstring m_strObjTag = TEXT("");;
		wstring m_strComTag = TEXT("");
		_bool	m_bCheck = false;
	}TOOL_AFTER_FILEDESC;

	typedef struct tagProtoObjDesc 
	{
		string  m_strFileName;
		wstring m_strObjTag = TEXT("");;
		wstring m_strComTag = TEXT("");
		_bool	m_bCheck = false;
	}TOOL_PROTO_OBJDESC;

	typedef struct tagLayOutDesc
	{
		const char*  m_strName;
		ImVec2		 m_vStartLayOutPos;
		ImVec2		 m_vEndLayOutPos;
	}TOOL_LAYOUTDESC;

	typedef struct tagToolCellDesc
	{
		_float3 fCellPos[3];
	}TOOL_CELLDESC;

public:
	enum SAVE_OBJTYPE { SAVETYPE_STATIC, SAVETYPE_DYNAMIC, SAVETYPE_END };
	//enum TOOL_LIGHT_TYPE { TLIGHT_TOWN, TLIGHT_FIRE, TLIGHT_END };
private:
	CImGui_Tool();
	virtual ~CImGui_Tool() = default;

public:
	virtual HRESULT Initialize(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual void	Frame();

private:
	// Mouse
	HRESULT	LayOut_Mouse();
	// main
	void	LayOut_Main();
	// Save / Load
	void	LayOut_ObjSaveLoad();
	void	LayOut_CellSaveLoad();
	// Object Create / Delete
	void	LayOut_Object_PickMode();
	// Navigation
	void	LayOut_Navigation();

	// Object Content
	void    LayOut_FBX_List();
	void    LayOut_Obj_List();
	void    LayOut_Object_Transform();

	// Light Content
	void	LayOut_Light();

	// FindFile
	void	FindFolder();
	void	Ready_FindFBX(TOOL_AFTER_FOLDERDESC _tFileDesc);
	HRESULT Find_FileFBX(const wstring& _wStrPath);

	// Empty Folder & File
	void	Empty_Content();

	// Change Type
	void    ChangeType_Folder();
	void    ChangeType_File();

	// Custom Object
	HRESULT	Create_Object();
	HRESULT	Delete_Object();
	HRESULT	Select_Object();

	// Custom Light
	HRESULT Create_Light();
	void	Select_Light();

	void	Key_Input(class CTransform* _pTransform);

	// Save / Load
	void    ObjFile_Save(SAVE_OBJTYPE _eType);
	void    ObjFile_Load();
	void	CellFile_Save();
	void	CellFile_Load();
	void	LightFile_Save();
	void	LightFile_Load();

	// RangeCheck(레이아웃 범위 밖에서만 피킹 가능하도록)
	void    Add_LayOut_Array(const char* _strName, ImVec2 _LayOutPos, ImVec2 _LayOutSize);
	_bool   Check_LayOut_Range(POINT _MousePos);

public:
	void    Set_ImGuiActive(_bool _isActive) { m_bActive = _isActive; }
	_bool	Get_ImGuiActive() { return m_bActive; }

private:
	// 장치
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pContext = nullptr;

	// mouse
	_float3 ResultPickPos = _float3();

	_bool m_bActive = true;

	// 파일 검색 불리언
	_bool m_bFindFolder = true;
	_bool m_bFindFBX = true;

	_bool m_bFindObj = true;

	// 파일 확장자 불리언
	_bool showFBXListShow = true;
	_bool showOBJListShow = true;

	// 파일 배열
	set<filesystem::path> visitedDirectories;

	vector<TOOL_BEFORE_FOLDERDESC> m_vecBeforeFileDesc; // 검색된 폴더 이름 저장.
	vector<TOOL_AFTER_FOLDERDESC> m_vecAfterFolderDesc; // 검색된 폴더 이름 const char* 형변환.

	vector<wstring> m_vecBeforeFileList; // 변환 전 파일 이름 리스트
	vector<TOOL_AFTER_FILEDESC> m_vecAfterFileDesc; // 변환 후 파일 정보 리스트

	// OBJ
	vector<TOOL_PROTO_OBJDESC> m_vecProtoObjDesc; // ProtoObj 저장 벡터

	// 현재 list에서 선택된 파일 변수에 저장.
	TOOL_AFTER_FILEDESC  m_pCurFile;
	wstring				 m_strCurLayerTag = TEXT(""); // 현재 선택된 폴더명을 레이어 태그로.
	// 선택된 오브젝트
	class CGameObject*   m_pSelectObject = nullptr;
	class CGameObject*   m_pSelectLight = nullptr;

	// Light 생성, 삭제 선택 모드
	_bool				 m_bLightCreateMode = false;
	_bool				 m_bLightDeleteMode = false;
	_bool				 m_bLightSelectMode = false;

	// Object 생성, 삭제, 선택 모드
	_bool				 m_bObjCreateMode = false;
	_bool				 m_bObjDeleteMode = false;
	_bool				 m_bObjSelectMode = false;	
	_bool				 m_bDead = false; // gui의 모든 작업이 끝나고 객체를 삭제하기 위함.
	_bool				 m_bLightDead = false; // gui의 모든 작업이 끝나고 객체를 삭제하기 위함.

	_float			     m_fRotValue = 0.f; // 오브젝트 회전을 위한 변수.

	// Navigation Cell 
	_bool				 m_bCellCreateMode = false;

	_uint					 m_iCellClickIdx = 0;
	TOOL_CELLDESC			 m_CellPoint;
	CNavigation*			 m_pNavigationCom = nullptr;


	// Light
	//string m_lightNameAry[TLIGHT_END] = { "Town", "Fire" };

	// 레이아웃 범위 밖에서만 클라이언트 작업을 수행하기 위함.
	vector<TOOL_LAYOUTDESC> m_vecLayOut;

public:
	static CImGui_Tool* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual void Free() override;

};

END