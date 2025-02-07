
#include "Engine_Shader_Defines.hpp"

matrix			g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture2D		g_Texture;
texture2D		g_Texture2;
texture2D		g_Textures[2];

float4	g_UIColor;
float g_fAlpha;

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

PS_OUT PS_BLOODFLARE(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;
	vector			vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);
	vector			vColor2 = g_Texture2.Sample(LinearSampler, In.vTexcoord);
	
	float fCenterRadius = 1.f / 1.f;
	float2 vCenter = float2(0.5f, 0.5f);

	float fDistance = distance(In.vTexcoord, vCenter);

	if (fDistance < fCenterRadius)
	{
		vColor.r = 1.0f;

		vColor.a = g_fAlpha * saturate(fDistance / fCenterRadius);
	}

	Out.vColor = vColor;

	return Out;
}

PS_OUT PS_BLOODSPOT(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;
	vector		vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);
	vector		vColor2 = g_Texture2.Sample(LinearSampler, In.vTexcoord);

	vector OutColor = lerp(vColor, vColor2, vColor2.a);

	if (OutColor.r <= 0.1f || OutColor.a <= 0.f)
		discard;

	OutColor.a = g_fAlpha * OutColor.a;
	Out.vColor = OutColor;

	return Out;
}

PS_OUT PS_FADE(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;
	vector			vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);

	vColor.a = g_fAlpha;

	Out.vColor = vColor;

	return Out;
}

PS_OUT PS_BLUR(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	float2 uv = In.vPosition.xy / float2(1280, 720); // 화면 좌표를 [0,1] 범위로 정규화

	float4 blurColor = 0;
	float blurAmount = 5.0; // blur 정도 조절

	for (int i = -5; i <= 5; i++)
	{
		blurColor += g_Texture.Sample(LinearSampler, uv + float2(i, 0) / 1280) / 14.0;
	}

	if (blurColor.r <= 0.01f)
		discard;

	Out.vColor = blurColor;

	return Out;
}


PS_OUT PS_COMBATSPARK(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;
	vector			vColor = g_Texture.Sample(PointSampler, In.vTexcoord);

	if (vColor.a <= 0.1f)
		discard;

	vColor.a = g_fAlpha;
	Out.vColor = vColor;

	return Out;
}



technique11 DefaultTechnique
{
	pass BloodFlare
	{
		SetRasterizerState(RS_Sky);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_BLOODFLARE();
	}

	pass BloodSpot
	{
		SetRasterizerState(RS_Sky);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_BLOODSPOT();
	}

	pass Fade
	{
		SetRasterizerState(RS_Sky);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_FADE();
	}

	pass Blur
	{
		SetRasterizerState(RS_Sky);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_BLUR();
	}

	pass CombatSpark
	{
		SetRasterizerState(RS_Sky);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_COMBATSPARK();
	}
}




