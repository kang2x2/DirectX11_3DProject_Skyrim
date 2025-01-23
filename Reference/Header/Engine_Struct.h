#ifndef Engine_Struct_h__
#define Engine_Struct_h__

namespace Engine
{
	typedef struct tagGraphicDesc
	{
		enum WINMODE { WINMODE_FULL, WINMODE_WIN, WINMODE_END };

		HWND		hWnd;
		WINMODE		eWinMode;
		unsigned int	iWinSizeX, iWinSizeY;

	}GRAPHIC_DESC;

	typedef struct tagLightDesc
	{
		enum TYPE { LIGHT_DIRECTIONAL, LIGHT_POINT, LIGHT_END };

		TYPE eLightType = LIGHT_END;

		// 점 광원
		_float4 vLightPos;
		_float  fLightRange;
		// 방향성 광원
		_float4 vLightDir;

		// 공용
		_float4 vDiffuse;
		_float4 vAmbient;
		_float4 vSpecular;

	}LIGHT_DESC;

	typedef struct tagMeshMaterial
	{
		class CTexture* pTextures[AI_TEXTURE_TYPE_MAX];
	}MESH_MATERIAL;

	typedef struct ENGINE_DLL tagVertex_Position_Cell
	{
		XMFLOAT3 vPosition;

		static const unsigned int				iNumElements = 1;
		static const D3D11_INPUT_ELEMENT_DESC	Elements[iNumElements];
	}VTXPOSCELL;

	typedef struct ENGINE_DLL tagVertex_Position_Color
	{
		XMFLOAT3	vPosition;
		XMFLOAT4	vColor;
	}VTXPOSCOL;

	typedef struct ENGINE_DLL tagVertex_Position_Texcoord
	{
		XMFLOAT3		vPosition;
		XMFLOAT2		vTexcoord;

		static const unsigned int iNumElements = 2;
		static const D3D11_INPUT_ELEMENT_DESC Elements[iNumElements];

	}VTXPOSTEX;

	typedef struct ENGINE_DLL tagVertex_Position_Cube_Texcoord
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vTexcoord;

		// Position과 Texcoord 두 가지를 입력하기에 Elements도 2
		static const unsigned int				iNumElements = 2;
		static const D3D11_INPUT_ELEMENT_DESC	Elements[iNumElements];
	}VTXCUBE;

	typedef struct ENGINE_DLL tagVertex_Position_Normal_Texcoord
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexcoord;

		static const unsigned int iNumElements = 3;
		static const D3D11_INPUT_ELEMENT_DESC Elements[iNumElements];

	}VTXNORTEX;

	typedef struct ENGINE_DLL tagVertex_NonAnim_Mesh
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexcoord;
		XMFLOAT3		vTangent;

		static const unsigned int				iNumElements = 4;
		static const D3D11_INPUT_ELEMENT_DESC	Elements[iNumElements];

	}VTX_NONANIMMESH;

	typedef struct ENGINE_DLL tagVertex_Anim_Mesh
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexcoord;
		XMFLOAT3		vTangent;

		// 자신이 상태를 받아야 할 뼈들의 인덱스를 담는 변수.
		// 최대 4개까지 저장 가능하다.
		XMUINT4			vBlendIndices;
		// 뼈의 상태를 몇 퍼센트 가져올 것인지 저장하는 변수.
		XMFLOAT4		vBlendWeights;

		static const unsigned int				iNumElements = 6;
		static const D3D11_INPUT_ELEMENT_DESC	Elements[iNumElements];

	}VTX_ANIMMESH;

	typedef struct ENGINE_DLL tagVertex_Instance
	{
		XMFLOAT4		vRight;
		XMFLOAT4		vUp;
		XMFLOAT4		vLook;
		XMFLOAT4		vTranslation;

		/* 셰이더에 이러한 정점이 잘 들어가냐? 라는 검증을 위해 */
		/* 이 정점 뿐만아니라 그리기용 정점도 같이 받아야되지. */
		/*static const unsigned int				iNumElements = 4;
		static const D3D11_INPUT_ELEMENT_DESC	Elements[iNumElements];*/
	}VTXINSTANCE;

	typedef struct ENGINE_DLL tagVertex_Rect_Instance
	{
		static const unsigned int				iNumElements = 6;
		static const D3D11_INPUT_ELEMENT_DESC	Elements[iNumElements];
	}VTX_RECT_INSTANCE;

	typedef struct ENGINE_DLL tagVertex_Model_Instance
	{
		static const unsigned int				iNumElements = 8;
		static const D3D11_INPUT_ELEMENT_DESC	Elements[iNumElements];
	}VTX_MODEL_INSTANCE;


	typedef struct tagKeyFrame
	{
		float		fTime;
		XMFLOAT3    vScale;
		XMFLOAT4    vRotation;
		XMFLOAT4    vTranslation;

	}KEYFRAME;
}

#endif // Engine_Struct_h__
