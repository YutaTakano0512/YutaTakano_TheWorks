
//*****************************************************************************
// 定数バッファ
//*****************************************************************************
cbuffer ConstantBuffer : register( b0 )
{
	//唯一CPU側から書き込める場所
	matrix WorldViewProjection;
	float4 Transition;
	float4 TexScale;
	float4 TexUV;
	float4 TexColor;
	float4 FixedColor;
}


//=============================================================================
// 頂点シェーダ
//=============================================================================
void VS(in  float4 inPosition	: POSITION0,
		in  float4 inDiffuse	: COLOR0,
		out float4 outPosition	: SV_POSITION,
		out float4 outDiffuse	: COLOR,
		in  float2 inTexCoord : TEXCOORD0,
		out float2 outTexCoord : TEXCOORD00)
{
	outPosition = mul( inPosition + Transition, WorldViewProjection );

	outDiffuse = inDiffuse * TexColor;
	outTexCoord = inTexCoord * float2(TexScale.x, TexScale.y);
}


Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);

//=============================================================================
// ピクセルシェーダ
//=============================================================================
void PS( in  float4 inPosition	: POSITION0,
		 in  float4 inDiffuse	: COLOR0,
		 in float2 inTexCoord : TEXCOORD0,
		 out float4 outDiffuse	: SV_Target)
{
	//outDiffuse = g_Texture.Sample(g_SamplerState, inTexCoord + float2(TexUV.x, TexUV.y)) * TexColor;
	float4 px = g_Texture.Sample(g_SamplerState, inTexCoord + float2(TexUV.x, TexUV.y)) * TexColor;
	/*px.x += TexColor.x;
	px.y += TexColor.y;
	px.z += TexColor.z;
	px.w *= TexColor.w;*/
		 
	if (px.w > 0
		&& FixedColor.x >= 0 && FixedColor.y >= 0 && FixedColor.z >= 0 && FixedColor.w >= 0)
	{
		outDiffuse = float4(FixedColor.x, FixedColor.y, FixedColor.z, FixedColor.w);
		//outDiffuse = float4(0.0f, 0.0f, 0.0f, 1.0f);
	}
	else
	{
		outDiffuse = px;
	}

	//outDiffuse = inDiffuse;
	//outDiffuse = g_Texture.Sample(g_SamplerState, inTexCoord * TexScale, TexUVpx);
	//outDiffuse = g_Texture.Sample(g_SamplerState, float4(1.0f, 1.0f, 1.0f, 1.0f), float4(1.0f, 1.0f, 1.0f, 1.0f) * TexScale);
	//outDiffuse = g_Texture.Sample(g_SamplerState, inTexCoord * TexScale, TexUVpx);
	//outDiffuse = float4(1.0f, 1.0f, 1.0f, 1.0f);
}
