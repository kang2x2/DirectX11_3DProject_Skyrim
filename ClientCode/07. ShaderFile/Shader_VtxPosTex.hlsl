
#include "Engine_Shader_Defines.hpp"

matrix			g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture2D		g_Texture;
texture2D		g_Textures[2];

float4	g_UIColor;
float   g_fAlpha;

struct VS_IN
{
	float3		vPosition : POSITION;
	float2		vTexcoord : TEXCOORD0;
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float2		vTexcoord : TEXCOORD0;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);

	Out.vTexcoord = In.vTexcoord;

	return Out;
}

struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float2		vTexcoord : TEXCOORD0;
};

struct PS_OUT
{
	float4	vColor : SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	vector		vSourColor = g_Textures[0].Sample(LinearSampler, In.vTexcoord);
	vector		vDestColor = g_Textures[1].Sample(LinearSampler, In.vTexcoord);

	Out.vColor = vSourColor + vDestColor;

	return Out;
}

PS_OUT PS_NONALPHA(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	vector		vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);

	if (vColor.a < 0.3)
		discard;

	Out.vColor = vColor;

	return Out;
}

PS_OUT PS_ALPHA(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	vector		vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);

	if (vColor.a <= 0.1f)
		discard;

	vColor.a = g_fAlpha;

	Out.vColor = vColor;

	return Out;
}

PS_OUT PS_CURSOR(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	vector		vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);
	
	if (vColor.a < 0.3)
		discard;

	Out.vColor = vColor;

	return Out;
}

PS_OUT PS_COLORUI(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	vector		vSourColor = g_Texture.Sample(LinearSampler, In.vTexcoord);
	vector		vDestColor = g_UIColor;

	Out.vColor = vSourColor + vDestColor;

	return Out;
}


technique11 DefaultTechnique
{
	pass UI
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_NONALPHA();
	}

	pass Alpha
	{
		SetRasterizerState(RS_Sky);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_ALPHA();
	}

	pass Cursor
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_CURSOR();
	}

	pass ColorUI
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_COLORUI();
	}
}




