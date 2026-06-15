
//*****************************************************************************
// 定数バッファ
//*****************************************************************************
cbuffer ConstantBuffer : register( b0 )
{
	//唯一CPU側から書き込める場所
    matrix WorldViewProjection;
    matrix World;
    matrix View;
    matrix Projection;
    
    float4 Transition;
    float4 TexScale;
    float4 TexUV;
    float4 TexColor;
    float4 FixedColor;
    //int4 EnableShade;
    int ShaderType;
    //int4 NoAlpha;
    int NoAlpha;
    
    //float4 ScreenSize;
    float2 ScreenSize;
    float4 CameraPos;
}


//=============================================================================
// 頂点シェーダ
//=============================================================================
void VS(in  float4 inPosition	: POSITION0,
		in  float4 inDiffuse	: COLOR0,
		out float4 outPosition	: SV_POSITION,
		out float4 outDiffuse	: COLOR,
		in float2 inTexCoord : TEXCOORD0,
        in float4 inNormal : NORMAL0,
		out float2 outTexCoord : TEXCOORD00,
		out float4 outNormal : NORMAL0)
{
	outPosition = mul( inPosition + Transition, WorldViewProjection );
	
	outDiffuse = inDiffuse * TexColor;
	//outDiffuse = inDiffuse;
    outTexCoord = inTexCoord * float2(TexScale.x, TexScale.y);
	
    outNormal = inNormal;
}


Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);

#define COLORBORDERMULMOD 0.2
#define COLORBORDERADDMOD 0.1
#define COLORBORDER 0.5
//=============================================================================
// ピクセルシェーダ
//=============================================================================
void PS( in  float4 inPosition	: POSITION0,
		 in  float4 inDiffuse	: COLOR0,
		 in float2 inTexCoord : TEXCOORD0,
         in float4 inNormal : NORMAL0,
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
	
    //if (EnableShade.x == 1)
    {
		 // ランバート拡散証明モデル
        inNormal.xyz = normalize(inNormal.xyz); // 正規化(長さ1にする)
		
		{
            //float3 lightDirection = normalize(float3(0.5, -1.0, 0.25));
			
            //float3 lightDirection = normalize(float3(-0.5, 0.3, -0.6));
            //float3 lightDirection = normalize(float3(-0.5, 0.8, -0.8));
            //float3 lightDirection = normalize(float3(-0.1, 0.3, -1.0));
            float3 lightDirection = normalize(float3(0.0, 0.0, -1.0));
		
            float3 lightColor = float3(-0.5, -0.5, -0.4); // ライトの色設定
            //float3 ambientColor = float3(0.0, 0.01, 0.1); // 環境光の色設定
            float3 ambientColor = float3(1.0, 1.0, 1.0); // 環境光の色設定
            //float3 ambientColor = float3(0.11, 0.15, 0.3); // 環境光の色設定
            //float3 ambientColor = float3(0.3, 0.3, 0.7); // 環境光の色設定
            //float3 ambientColor = float3(0.5, 0.5, 0.8); // 環境光の色設定
            //float3 ambientColor = float3(0.4, 0.4, 0.7); // 環境光の色設定
    
            float3 light = -dot(inNormal.xyz, lightDirection);
            light = saturate(light) * lightColor + ambientColor; // ライトの色処理

    
            outDiffuse.rgb *= light;
        }
		//{
  //          float3 lightDirection = normalize(float3(0.0, 0.2, -1.0));
		
  //          float3 lightColor = float3(1.0, 1.0, 0.7); // ライトの色設定
  //          float3 ambientColor = float3(0.8, 0.7, 0.8); // 環境光の色設定
    
  //          float3 light = -dot(inNormal.xyz, lightDirection);
  //          light = saturate(light) * lightColor + ambientColor; // ライトの色処理
            
  //          outDiffuse.rgb *= light;
  //      }
    }
	{
        outDiffuse.x += (outDiffuse.x - COLORBORDER) * COLORBORDERMULMOD + COLORBORDERADDMOD;
        outDiffuse.y += (outDiffuse.y - COLORBORDER) * COLORBORDERMULMOD + COLORBORDERADDMOD;
        outDiffuse.z += (outDiffuse.z - COLORBORDER) * COLORBORDERMULMOD + COLORBORDERADDMOD;
    }
	
	//アルファテスト
    //if (outDiffuse.w <= 0)
    //{
    //    clip(outDiffuse.w - 0.1);
    //}
    clip(outDiffuse.w - 0.01);
	
    if (NoAlpha.x == 1)
    {
        outDiffuse.w = 1.0f;
    }

	//outDiffuse = inDiffuse;
	//outDiffuse = g_Texture.Sample(g_SamplerState, inTexCoord * TexScale, TexUVpx);
	//outDiffuse = g_Texture.Sample(g_SamplerState, float4(1.0f, 1.0f, 1.0f, 1.0f), float4(1.0f, 1.0f, 1.0f, 1.0f) * TexScale);
	//outDiffuse = g_Texture.Sample(g_SamplerState, inTexCoord * TexScale, TexUVpx);
	//outDiffuse = float4(1.0f, 1.0f, 1.0f, 1.0f);
 }
