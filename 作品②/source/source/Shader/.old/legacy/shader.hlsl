
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
    outTexCoord = inTexCoord * float2(TexScale.x, TexScale.y) + float2(TexUV.x, TexUV.y);
    
    //float2 texc = inTexCoord;
    //texc.x *= TexScale.x;
    //texc.y *= TexScale.y;
    
    //outTexCoord = texc + float2(TexUV.x, TexUV.y);
    //float2 texc = inTexCoord;
    //float texScaX = 1.0f / TexScale.x;
    //float texScaY = 1.0f / TexScale.y;
    
    //outTexCoord = inTexCoord + float2(TexUV.x / TexScale.x, TexUV.y / TexScale.y);
	
    outNormal = inNormal;
}


Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);

//=============================================================================
// ピクセルシェーダ
//=============================================================================
void PS(in float4 inPosition : POSITION0,
		 in  float4 inDiffuse	: COLOR0,
		 in float2 inTexCoord : TEXCOORD0,
         in float4 inNormal : NORMAL0,
		 out float4 outDiffuse	: SV_Target)
{
	//outDiffuse = g_Texture.Sample(g_SamplerState, inTexCoord + float2(TexUV.x, TexUV.y)) * TexColor;
    float4 px = g_Texture.Sample(g_SamplerState, inTexCoord) * TexColor;
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
	
     //METAL_ENEMY
    if (ShaderType == 2 || ShaderType == 3 || ShaderType == 4)
    {
        if (
            (ShaderType == 2 && px.x < 0.6 && px.y >= 0.9 && px.z < 0.6)
            ||
            (ShaderType == 3 && px.x >= 0.8 && px.y < 0.6 && px.z < 0.75)
            //|| (px.x >= 0.9 && px.y < 0.5 && px.z >= 0.9)
        )
        {
            outDiffuse.xyz = px;
        }
        else
        {
		 // Phong鏡面反射モデル
        
            inNormal.xyz = normalize(inNormal.xyz); // 正規化(長さ1にする)
		
            //float3 ambientColor = float3(0.2, 0.2, 0.6);
            float3 ambientColor = float3(0.35, 0.35, 0.65);
        
            float3 lig;
            float3 lig2;
        {
                float3 lightDirection = normalize(float3(0.0, 0.0, 0.3));
                //float3 lightDirection = normalize(float3(0.3, 0.9, 0.3));
                //float3 lightDirection = normalize(float3(0.3, 0.5, 0.0));
            //float3 lightDirection = normalize(float3(0.0, 1.0, 0.7));
            //float3 lightDirection = normalize(float3(-0.0, -1.0, -1.0));
        
                float3 lightColor = float3(1.0, 1.0, 0.8); // ライトの色設定
        //float3 lightColor = float3(0.5, 0.5, 1.0); // ライトの色設定
        
        // 1. ライトの方向と面の法線とで内積を計算する
                float t = dot(inNormal.xyz, lightDirection);
        // 1.で求めた結果に-1を乗算してライトの強さを求める
                t *= -1.0f;

        // ライトの強さにマイナスの値は必要ないため
                if (t < 0.0f)
                    t = 0.0f;

        
        // 拡散反射光を求める
                float3 diffuseLig = lightColor * t;

        // 1. ライトが面に入射して、反射したベクトルを求める
                float3 refVec = reflect(lightDirection, inNormal.xyz);

        //2. 光が入射した面から視点に向かって伸びるベクトルを求める
                float3 toEye = CameraPos.xyz - inPosition.xyz;
                toEye = normalize(toEye);

        // 1と2で求めたベクトルの内積を使って、鏡面反射の強さを求める
                t = dot(refVec, toEye);
        // 内積の結果が0以下なら0にする
                if (t < 0.0f)
                {
                    t = 0.0f;
                }

        // step-7 鏡面反射の強さを絞る
                t = pow(t, 0.4f);

        // 4. 3で求めた鏡面反射の強さを絞って、最終的な反射の強さを求める
                float3 specularLig = lightColor * t;
    

        // 拡散反射光と鏡面反射光を足し算して、最終的な光を求める
                lig2 = diffuseLig + specularLig;

            }
        //{
        //    float3 lightDirection = normalize(float3(0.0, 0.3, -1.0));
        ////float3 lightDirection = normalize(float3(0.0, 1.0, 0.7));
        ////float3 lightDirection = normalize(float3(-0.0, -1.0, -1.0));
        
        //    float3 lightColor = float3(0.8, 0.7, 0.9); // ライトの色設定
        ////float3 lightColor = float3(0.5, 0.5, 1.0); // ライトの色設定
        
        //// 1. ライトの方向と面の法線とで内積を計算する
        //    float t = dot(inNormal.xyz, lightDirection);
        //// 1.で求めた結果に-1を乗算してライトの強さを求める
        //    t *= -1.0f;

        //// ライトの強さにマイナスの値は必要ないため
        //    if (t < 0.0f)
        //        t = 0.0f;

        
        //// 拡散反射光を求める
        //    float3 diffuseLig = lightColor * t;

        //// 1. ライトが面に入射して、反射したベクトルを求める
        //    float3 refVec = reflect(lightDirection, inNormal.xyz);

        ////2. 光が入射した面から視点に向かって伸びるベクトルを求める
        //    float3 toEye = CameraPos.xyz - inPosition.xyz;
        //    toEye = normalize(toEye);

        //// 1と2で求めたベクトルの内積を使って、鏡面反射の強さを求める
        //    t = dot(refVec, toEye);
        //// 内積の結果が0以下なら0にする
        //    if (t < 0.0f)
        //    {
        //        t = 0.0f;
        //    }

        //// step-7 鏡面反射の強さを絞る
        //    t = pow(t, 0.5f);

        //// 4. 3で求めた鏡面反射の強さを絞って、最終的な反射の強さを求める
        //    float3 specularLig = lightColor * t;
    

        //// 拡散反射光と鏡面反射光を足し算して、最終的な光を求める
        //    lig = diffuseLig + specularLig;
        //    lig *= 0.25f;
        //}
        
        
        // テクスチャカラーに求めた光を乗算して最終出力カラーを求める
            outDiffuse.xyz = px;
            outDiffuse.xyz *= ambientColor + lig2;
        //outDiffuse.xyz += ambientColor;
        
        
       //  //アンビエント
       // float3 ambientColor = float3(0.4, 0.4, 0.5);
        
       // float3 lightDirection = normalize(float3(-0.8, -1.0, -1.0));
       // float3 lightColor = float3(1.0, 1.0, 0.6); // ライトの色設定
       // float3 lightVec = -dot(inNormal.xyz, lightDirection);
       // float3 light = saturate(lightVec) * lightColor + ambientColor; // ライトの色処理

       // //ディフューズ
       // float3 L = normalize(inPosition.xyz);
       // float3 N = normalize(inNormal.xyz);
       // float NL = dot(N, L);
       //// float3 diffuse = float3(1.0f, 1.0f, 1.0f) * saturate(NL) * 0.3f;
       // float3 diffuse = light * saturate(NL) * 0.8f;

       // //スペキュラ
       // float3 R = 2.0f * N * NL - L;
       // float3 V = normalize(CameraPos.xyz - inNormal.xyz);
       // float RV = saturate(dot(R, V));
       // //float3 specular = float3(1.0f, 1.0f, 1.0f) * pow(RV, 10.0f) * 0.15f;
       // float3 specular = saturate(lightVec) * float3(1.0f, 1.0f, 1.0f) * pow(RV, 10.0f) * 1.8f;

       // //return float4(ambient + diffuse + specular, 1.0f);
       // outDiffuse.rgb *= diffuse + specular;
        
        
        
        //float3 ligDirection = normalize(float3(0.4, 0.8, -0.7));
        //float3 ligColor = float3(1.0, 1.0, 0.7); // ライトの色設定
        //float3 ambientColor = float3(0.2, 0.2, 0.4); // 環境光の色設定
        //float light = -dot(inNormal.xyz, ligDirection);
        
        //{
        //// 拡散反射光を求める
        //    float3 diffuseLig = ligDirection * light;

        //    // 1. ライトが面に入射して、反射したベクトルを求める
        //    float3 refVec = reflect(ligDirection, inNormal.xyz);

        //    //2. 光が入射した面から視点に向かって伸びるベクトルを求める
        //    //float3 toEye = eyePos - inPosition.xyz;
        //    //toEye = normalize(toEye);
            
        //    float3 toEye = -inPosition.xyz;
        //    toEye = normalize(toEye);
            

        //    // 1と2で求めたベクトルの内積を使って、鏡面反射の強さを求める
        //    light = dot(refVec, toEye);
        //    // 内積の結果が0以下なら0にする
        //    if (light < 0.0f)
        //    {
        //        light = 0.0f;
        //    }

        //    // step-7 鏡面反射の強さを絞る
        //    light = pow(light, 5.0f);

        //    // 4. 3で求めた鏡面反射の強さを絞って、最終的な反射の強さを求める
        //    float3 specularLig = ligColor * light;
    

        //    // 拡散反射光と鏡面反射光を足し算して、最終的な光を求める
        //    float3 lig = diffuseLig + specularLig;

        //    // テクスチャからカラーをフェッチする
        //    float4 finalColor = px;

        //    // テクスチャカラーに求めた光を乗算して最終出力カラーを求める
        //    finalColor.xyz *= lig;
            
        //    outDiffuse.rgb = finalColor;
        //}
        
		//{
  //          //float3 lightDirection = normalize(float3(0.5, -1.0, 0.25));
			
  //          //float3 lightDirection = normalize(float3(-0.5, 0.3, -0.6));
  //          //float3 lightDirection = normalize(float3(-0.5, 0.8, -0.8));
  //          float3 lightDirection = normalize(float3(-0.4, 0.4, -1.0));
		
  //          float3 lightColor = float3(1.0, 1.0, 0.7); // ライトの色設定
  //          float3 ambientColor = float3(0.2, 0.2, 0.4); // 環境光の色設定
  //          //float3 ambientColor = float3(0.3, 0.3, 0.7); // 環境光の色設定
  //          //float3 ambientColor = float3(0.5, 0.5, 0.8); // 環境光の色設定
  //          //float3 ambientColor = float3(0.4, 0.4, 0.7); // 環境光の色設定
    
  //          float3 light = -dot(inNormal.xyz, lightDirection);
  //          light = saturate(light) * lightColor + ambientColor; // ライトの色処理

    
  //          outDiffuse.rgb *= light;
  //      }
		//{
  //          float3 lightDirection = normalize(float3(0.0, 0.2, -1.0));
		
  //          float3 lightColor = float3(1.0, 1.0, 0.7); // ライトの色設定
  //          float3 ambientColor = float3(0.8, 0.7, 0.8); // 環境光の色設定
    
  //          float3 light = -dot(inNormal.xyz, lightDirection);
  //          light = saturate(light) * lightColor + ambientColor; // ライトの色処理
            
  //          outDiffuse.rgb *= light;
  //      }
        }
    }
    
     //CUTIN
    if (ShaderType == 4)
    {
        {
		 //// Phong鏡面反射モデル
        
   //         inNormal.xyz = normalize(inNormal.xyz); // 正規化(長さ1にする)
		
   //         //float3 ambientColor = float3(0.2, 0.2, 0.6);
   //         float3 ambientColor = float3(0.35, 0.35, 0.65);
        
   //         float3 lig;
   //         float3 lig2;
   //     {
   //             float3 lightDirection = normalize(float3(0.0, 0.0, 0.3));
   //             //float3 lightDirection = normalize(float3(0.3, 0.9, 0.3));
   //             //float3 lightDirection = normalize(float3(0.3, 0.5, 0.0));
   //         //float3 lightDirection = normalize(float3(0.0, 1.0, 0.7));
   //         //float3 lightDirection = normalize(float3(-0.0, -1.0, -1.0));
        
   //             float3 lightColor = float3(1.0, 1.0, 0.8); // ライトの色設定
   //     //float3 lightColor = float3(0.5, 0.5, 1.0); // ライトの色設定
        
   //     // 1. ライトの方向と面の法線とで内積を計算する
   //             float t = dot(inNormal.xyz, lightDirection);
   //     // 1.で求めた結果に-1を乗算してライトの強さを求める
   //             t *= -1.0f;

   //     // ライトの強さにマイナスの値は必要ないため
   //             if (t < 0.0f)
   //                 t = 0.0f;

        
   //     // 拡散反射光を求める
   //             float3 diffuseLig = lightColor * t;

   //     // 1. ライトが面に入射して、反射したベクトルを求める
   //             float3 refVec = reflect(lightDirection, inNormal.xyz);

   //     //2. 光が入射した面から視点に向かって伸びるベクトルを求める
   //             float3 toEye = CameraPos.xyz - inPosition.xyz;
   //             toEye = normalize(toEye);

   //     // 1と2で求めたベクトルの内積を使って、鏡面反射の強さを求める
   //             t = dot(refVec, toEye);
   //     // 内積の結果が0以下なら0にする
   //             if (t < 0.0f)
   //             {
   //                 t = 0.0f;
   //             }

   //     // step-7 鏡面反射の強さを絞る
   //             t = pow(t, 0.001f);

   //     // 4. 3で求めた鏡面反射の強さを絞って、最終的な反射の強さを求める
   //             float3 specularLig = lightColor * t;
    

   //     // 拡散反射光と鏡面反射光を足し算して、最終的な光を求める
   //             lig2 = diffuseLig + specularLig;

   //         }
        
        
   //     // テクスチャカラーに求めた光を乗算して最終出力カラーを求める
   //         outDiffuse.xyz = px;
   //         outDiffuse.xyz *= ambientColor + lig2;
            
            //outDiffuse.xyz *= 2.0f;
            
            
            
            //if (px.x >= 0.7f && px.y <= 0.6f && px.z <= 0.6f)
            //{
            //    outDiffuse.x = 1.0f;
            //    outDiffuse.y = 0.0f;
            //    outDiffuse.z = 0.0f;
            //}
            //else if (px.x <= 0.6f && px.y >= 0.7f && px.z <= 0.6f)
            //{
            //    outDiffuse.x = 0.0f;
            //    outDiffuse.y = 1.0f;
            //    outDiffuse.z = 0.0f;
            //}
            //else if (px.x <= 0.6f && px.y <= 0.6f && px.z >= 0.7f)
            //{
            //    outDiffuse.x = 0.0f;
            //    outDiffuse.y = 0.0f;
            //    outDiffuse.z = 1.0f;
            //}
            //else if (outDiffuse.x >= 0.8f && outDiffuse.y >= 0.8f && outDiffuse.z >= 0.8f)
            //{
            //    outDiffuse.x = 1.0f;
            //    outDiffuse.y = 1.0f;
            //    outDiffuse.z = 1.0f;
            //}
            //else if (outDiffuse.x >= 0.6f && outDiffuse.y >= 0.6f && outDiffuse.z >= 0.6f)
            //{
            //    outDiffuse.x = 0.0f;
            //    outDiffuse.y = 1.0f;
            //    outDiffuse.z = 1.0f;
            //}
            //else if (outDiffuse.x >= 0.3f && outDiffuse.y >= 0.3f && outDiffuse.z >= 0.3f)
            //{
            //    outDiffuse.x = 1.0f;
            //    outDiffuse.y = 0.2f;
            //    outDiffuse.z = 1.0f;
            //}
            //else
            //{
            //    outDiffuse.x = 0.0f;
            //    outDiffuse.y = 0.0f;
            //    outDiffuse.z = 0.75f;
            //}
            
            //if (
            //   (outDiffuse.x <= 0.3f && outDiffuse.y <= 0.3f && outDiffuse.z <= 0.3f)
            //    && (
            //        (((int) inPosition.x) % 2 == 0 && ((int) inPosition.y) % 2 != 0)
            //        || (((int) inPosition.x) % 2 != 0 && ((int) inPosition.y) % 2 == 0)
            //        //|| ((int) inPosition.z) % 2 == 0
            //    )
            //)
            //{
            //    outDiffuse.x *= 0.75f;
            //    outDiffuse.y *= 0.75f;
            //    outDiffuse.z *= 0.75f;
            //}
            
            //if (px.x < px.y && px.x < px.x)
            //{
            //    outDiffuse.x = 0.0f;
            //    outDiffuse.y = 0.5f;
            //    outDiffuse.z = 0.5f;
            //}
            //else if (px.y < px.x && px.y < px.z)
            //{
            //    outDiffuse.x = 0.5f;
            //    outDiffuse.y = 0.0f;
            //    outDiffuse.z = 0.5f;
            //}
            //else if (px.z < px.x && px.z < px.y)
            //{
            //    outDiffuse.x = 0.5f;
            //    outDiffuse.y = 0.5f;
            //    outDiffuse.z = 0.0f;
            //}
            
            //else if (px.x > px.y && px.x > px.x)
            //{
            //    outDiffuse.x = 0.5f;
            //    outDiffuse.y = 0.0f;
            //    outDiffuse.z = 0.0f;
            //}
            //else if (px.y > px.x && px.y > px.z)
            //{
            //    outDiffuse.x = 0.0f;
            //    outDiffuse.y = 0.5f;
            //    outDiffuse.z = 0.0f;
            //}
            //else if (px.z > px.x && px.z > px.y)
            //{
            //    outDiffuse.x = 0.0f;
            //    outDiffuse.y = 0.0f;
            //    outDiffuse.z = 0.5f;
            //}
            //else
            //{
            //    outDiffuse.x = 0.5f;
            //    outDiffuse.y = 0.5f;
            //    outDiffuse.z = 0.5f;
            //}
            
            //{
            //    float gs = (px.x + px.y + px.z) / 3;
            //    outDiffuse.x += gs;
            //    outDiffuse.y += gs;
            //    outDiffuse.z += gs;
            //    //outDiffuse.x += (outDiffuse.x - 0.5f) * 3.0f + 0.5f;
            //    //outDiffuse.y += (outDiffuse.y - 0.5f) * 3.0f + 0.5f;
            //    //outDiffuse.z += (outDiffuse.z - 0.5f) * 3.0f + 0.5f;
            //}
            
            //{
            //    int bun = 256;
            //    outDiffuse.x = (float) ((int) (outDiffuse.x * bun)) / bun;
            //    outDiffuse.y = (float) ((int) (outDiffuse.y * bun)) / bun;
            //    outDiffuse.z = (float) ((int) (outDiffuse.z * bun)) / bun;
            //}
            
            {
                //outDiffuse.x += (outDiffuse.x - 0.5f) * 1.08f + 0.4f;
                //outDiffuse.y += (outDiffuse.y - 0.5f) * 1.08f + 0.4f;
                //outDiffuse.z += (outDiffuse.z - 0.5f) * 1.08f + 0.4f;
                outDiffuse.x += (outDiffuse.x - 0.5f) * 0.2f + 0.2f;
                outDiffuse.y += (outDiffuse.y - 0.5f) * 0.2f + 0.2f;
                outDiffuse.z += (outDiffuse.z - 0.5f) * 0.2f + 0.2f;
                //if (px.x > px.y && px.x > px.z)
                //{
                //    outDiffuse.x *= 1.5f;
                //}
                //if (px.y > px.x && px.y > px.z)
                //{
                //    outDiffuse.y *= 1.5f;
                //}
                //if (px.z > px.x && px.z > px.y)
                //{
                //    outDiffuse.z *= 1.5f;
                //}

            }
            
            int pX = (int) (inPosition.x * 2);
            int pY = (int) (inPosition.y * 2);
            if (
                (
                    (pX % 2 == 0 && pY % 2 != 0)
                    || (pX % 2 != 0 && pY % 2 == 0)
                    //|| ((int) inPosition.z) % 2 == 0
                )
            )
            {
                //outDiffuse.x += (min(outDiffuse.x - 0.5f, 0.5f) - 0.5f) * 0.35f;
                //outDiffuse.y += (min(outDiffuse.y - 0.5f, 0.5f) - 0.5f) * 0.35f;
                //outDiffuse.z += (min(outDiffuse.z - 0.5f, 0.5f) - 0.5f) * 0.35f;
                //outDiffuse.x += (1.0f - outDiffuse.x) * -0.12f;
                //outDiffuse.y += (1.0f - outDiffuse.y) * -0.12f;
                //outDiffuse.z += (1.0f - outDiffuse.z) * -0.12f;
                outDiffuse.x += max(min(outDiffuse.x, 1.0f), 0.0f) * 0.24f * (outDiffuse.x < 0.5f ? -1: 1);
                outDiffuse.y += max(min(outDiffuse.y, 1.0f), 0.0f) * 0.24f * (outDiffuse.y < 0.5f ? -1: 1);
                outDiffuse.z += max(min(outDiffuse.z, 1.0f), 0.0f) * 0.24f * (outDiffuse.z < 0.5f ? -1: 1);
            }

        }
    }
	
    //EFFECT_DIS
    if (ShaderType == 5)
    {
        //px.x += 0.1f;
        //px.y += 0.1f;
        //px.z += 0.1f;
        //if (px.x >= 0.95f && px.y >= 0.95f && px.z >= 0.95f)
        //{
        //    outDiffuse.w = 0;
        //}
        
	    {
            px.x += (px.x - 0.5f) * 2.5f;
            px.y += (px.y - 0.5f) * 2.5f;
            px.z += (px.z - 0.5f) * 2.5f;
        }
        
        if (px.x <= 0.1f && px.y <= 0.1f && px.z <= 0.1f)
        {
            outDiffuse.w = 0;
        }
        else if (px.x <= 0.3f && px.y <= 0.3f && px.z <= 0.3f)
        {
            outDiffuse.x = 0.0f * 0.6;
            outDiffuse.y = 0.4f * 0.6;
            outDiffuse.z = 0.6f * 0.6;
        }
        else if (px.x <= 0.7f && px.y <= 0.7f && px.z <= 0.7f)
        {
            outDiffuse.x = 0.1f * 0.6;
            outDiffuse.y = 0.6f * 0.6;
            outDiffuse.z = 0.4f * 0.6;
        }
        
        //outDiffuse = px;

    }
    //{
    //    //px.x += 0.1f;
    //    //px.y += 0.1f;
    //    //px.z += 0.1f;
    //    //if (px.x >= 0.95f && px.y >= 0.95f && px.z >= 0.95f)
    //    //{
    //    //    outDiffuse.w = 0;
    //    //}
        
	   // {
    //        px.x += (px.x - 0.5f) * 3.0f;
    //        px.y += (px.y - 0.5f) * 3.0f;
    //        px.z += (px.z - 0.5f) * 3.0f;
    //    }
        
    //    if (px.x <= 0.1f && px.y <= 0.1f && px.z <= 0.1f)
    //    {
    //        outDiffuse.w = 0;
    //    }
    //    else if (px.x <= 0.3f && px.y <= 0.3f && px.z <= 0.3f)
    //    {
    //        outDiffuse.x = 0.4f;
    //        outDiffuse.y = 0.9f;
    //        outDiffuse.z = 0.9f;
    //    }
        
    //    //outDiffuse = px;

    //}
    
     //EFFECT_WARP
    if (ShaderType == 6)
    {
        //px.x += 0.1f;
        //px.y += 0.1f;
        //px.z += 0.1f;
        //if (px.x >= 0.95f && px.y >= 0.95f && px.z >= 0.95f)
        //{
        //    outDiffuse.w = 0;
        //}
        
	    {
            px.x += (px.x - 0.5f) * 2.5f;
            px.y += (px.y - 0.5f) * 2.5f;
            px.z += (px.z - 0.5f) * 2.5f;
        }
        
        if (px.x <= 0.1f && px.y <= 0.1f && px.z <= 0.1f)
        {
            outDiffuse.w = 0;
        }
        else if (px.x <= 0.3f && px.y <= 0.3f && px.z <= 0.3f)
        {
            outDiffuse.x = 0.4f * 0.6;
            outDiffuse.y = 0.0f * 0.6;
            outDiffuse.z = 0.6f * 0.6;
        }
        else if (px.x <= 0.7f && px.y <= 0.7f && px.z <= 0.7f)
        {
            outDiffuse.x = 0.6f * 0.6;
            outDiffuse.y = 0.1f * 0.6;
            outDiffuse.z = 0.4f * 0.6;
        }
        
        //outDiffuse = px;

    }
    
     //DX_SHADER_TYPE_EFFECT_FRESNEL
    if (ShaderType == 7)
    {
        //v2f o;
        //o.vertex = UnityObjectToClipPos(v.vertex);
        //half3 viewDir = normalize(ObjSpaceViewDir(v.vertex));
        //o.vdotn = dot(viewDir, v.normal.xyz);
        //o.reflDir = mul(unity_ObjectToWorld, reflect(-viewDir, v.normal.xyz));
        
        half _F0 = 0.12f;
        //half3 viewDir = normalize(half3(
        //    (inPosition.x - CameraPos.x) * -1,
        //    (inPosition.z - CameraPos.z) * 1,
        //    (inPosition.y - CameraPos.y) * 1
        //));
        half3 viewDir = normalize(half3(
            (inPosition.x - CameraPos.x) * 1,
            (inPosition.y - CameraPos.y) * 1,
            (inPosition.z - CameraPos.z) * 1
        ));

        //half3 viewDir = normalize(half3(
        //    (inPosition.x - 0.0f),
        //    (inPosition.y - 0.0f),
        //    (inPosition.z - 0.0f)
        //));
        
        //half fresnel = _F0 + (1.0h - _F0) * pow(1.0h - dot(viewDir, inNormal.xyz), 10);
        half fresnel = _F0 + (1.0h - _F0) * pow(1.0h - dot(viewDir, inNormal.xyz), 2);
        
        //float4 colo = px * fresnel;
        float4 colo = fresnel;
        colo.x = 1.0f - colo.x;
        colo.y = 1.0f - colo.y;
        colo.z = 1.0f - colo.z;
        if ((colo.x < 0.2 && colo.y < 0.2 && colo.z < 0.2))
        {
            outDiffuse.w = 0;
        }
        else
        {
            //outDiffuse = px * (1.0f / colo);
            //outDiffuse.w = (1.0f / colo);
            outDiffuse.w = 1.0f - (colo.x / 0.8f);
            outDiffuse.w *= pow((1.0f - outDiffuse.w) + 1.0f, 3);
        }
        
        //outDiffuse = px * fresnel;
        
        //inNormal.xyz = normalize(inNormal.xyz); // 正規化(長さ1にする)
		
        //half3 viewDir = normalize(half3(
        //    (inPosition.x - CameraPos.x) * -1,
        //    (inPosition.y - CameraPos.y),
        //    (inPosition.z - CameraPos.z)
        //));
        
        //if (abs(inNormal.x - 0) < 0.1
        //    && abs(inNormal.y - 0) < 0.1
        //    && abs(inNormal.z - -1) < 0.1
        //)
        //{
        //    outDiffuse.w = 0;
        //}
        //else
        //{
        //    outDiffuse = px;
        //}
    }
    
    //DX_SHADER_TYPE_ALPHA_DISSOLVE
    if (ShaderType == 8)
    {
        if (outDiffuse.w > 0.01)
        {
            int pX = (int) (inPosition.x * 2);
            int pY = (int) (inPosition.y * 2);
            if (
                (
                    (pX % 2 == 0 && pY % 2 != 0)
                    || (pX % 2 != 0 && pY % 2 == 0)
                    //|| ((int) inPosition.z) % 2 == 0
                )
            )
            {
                outDiffuse.w *= outDiffuse.w;
            }
        }
    }
    
    
     //METAL_ENEMY
    if (ShaderType == 9)
    {
        if (
            (px.x < 0.6 && px.y >= 0.9 && px.z < 0.6)
            ||
            (px.x >= 0.8 && px.y < 0.6 && px.z < 0.75)
        )
        {
            outDiffuse.xyz = px;
        }
        else
        {
		 // Phong鏡面反射モデル
        
            inNormal.xyz = normalize(inNormal.xyz); // 正規化(長さ1にする)
		
            //float3 ambientColor = float3(0.2, 0.2, 0.6);
            //float3 ambientColor = float3(0.35, 0.35, 0.65);
            float3 ambientColor = float3(0.24, 0.24, 0.25);
        
            float3 lig;
            float3 lig2;
        {
                float3 lightDirection = normalize(float3(0.0, 0.0, 0.3));
                //float3 lightDirection = normalize(float3(0.3, 0.9, 0.3));
                //float3 lightDirection = normalize(float3(0.3, 0.5, 0.0));
            //float3 lightDirection = normalize(float3(0.0, 1.0, 0.7));
            //float3 lightDirection = normalize(float3(-0.0, -1.0, -1.0));
        
                float3 lightColor = float3(1.0, 1.0, 0.95); // ライトの色設定
        //float3 lightColor = float3(0.5, 0.5, 1.0); // ライトの色設定
        
        // 1. ライトの方向と面の法線とで内積を計算する
                float t = dot(inNormal.xyz, lightDirection);
        // 1.で求めた結果に-1を乗算してライトの強さを求める
                t *= -1.0f;

        // ライトの強さにマイナスの値は必要ないため
                if (t < 0.0f)
                    t = 0.0f;

        
        // 拡散反射光を求める
                float3 diffuseLig = lightColor * t;

        // 1. ライトが面に入射して、反射したベクトルを求める
                float3 refVec = reflect(lightDirection, inNormal.xyz);

        //2. 光が入射した面から視点に向かって伸びるベクトルを求める
                float3 toEye = CameraPos.xyz - inPosition.xyz;
                toEye = normalize(toEye);

        // 1と2で求めたベクトルの内積を使って、鏡面反射の強さを求める
                t = dot(refVec, toEye);
        // 内積の結果が0以下なら0にする
                if (t < 0.0f)
                {
                    t = 0.0f;
                }

        // step-7 鏡面反射の強さを絞る
                t = pow(t, 0.4f);

        // 4. 3で求めた鏡面反射の強さを絞って、最終的な反射の強さを求める
                float3 specularLig = lightColor * t;
    

        // 拡散反射光と鏡面反射光を足し算して、最終的な光を求める
                lig2 = diffuseLig + specularLig;

            }
        
        // テクスチャカラーに求めた光を乗算して最終出力カラーを求める
            outDiffuse.xyz = px;
            outDiffuse.xyz *= ambientColor + lig2;
            
            outDiffuse.x += (outDiffuse.x - 0.5f) * 0.16f + 0.05f;
            outDiffuse.y += (outDiffuse.y - 0.5f) * 0.16f + 0.05f;
            outDiffuse.z += (outDiffuse.z - 0.5f) * 0.16f + 0.05f;
        }
    }
    
    //DX_SHADER_TYPE_GAUSSIANBLUR
    if (ShaderType == 10)
    {
        float4 color = px;
        float alpha = color.a; // アルファ値を取得

        // 境界部分をぼかすための補間
        float blurFactor = smoothstep(0.0, 1.0, alpha); // アルファ値に基づく補間
        color.rgb *= blurFactor; // 色をアルファ値に応じて調整

        
        
        float luminance = dot(color.rgb, float3(0.1, 0.1, 0.1)); // 輝度を計算
        // にじみの強度を計算
        float smudgeFactor = saturate((luminance - 0.5) * 2.0); // 閾値を調整

        // 周囲のピクセルをサンプリングして混ぜる
        float2 texOffset = float2(1.0 / ScreenSize.x, 1.0 / ScreenSize.y);
        float4 blurredColor = float4(0, 0, 0, 0);
        blurredColor += g_Texture.Sample(g_SamplerState, inTexCoord + texOffset * float2(1, 0));
        blurredColor += g_Texture.Sample(g_SamplerState, inTexCoord - texOffset * float2(1, 0));
        blurredColor += g_Texture.Sample(g_SamplerState, inTexCoord + texOffset * float2(0, 1));
        blurredColor += g_Texture.Sample(g_SamplerState, inTexCoord - texOffset * float2(0, 1));

        blurredColor /= 4.0; // 平均化

        // 元の色とにじみ色を補間
        color.rgb = lerp(color.rgb, blurredColor.rgb, smudgeFactor);



        outDiffuse.rgb = color.rgb * 1.5;
    }
    
	//アルファテスト
    //if (outDiffuse.w <= 0)
    //{
    //    clip(outDiffuse.w - 0.1);
    //}
        clip(outDiffuse.w - 0.01);
	
    if (NoAlpha == 1)
    {
        outDiffuse.w = 1.0f;
    }

	//outDiffuse = inDiffuse;
	//outDiffuse = g_Texture.Sample(g_SamplerState, inTexCoord * TexScale, TexUVpx);
	//outDiffuse = g_Texture.Sample(g_SamplerState, float4(1.0f, 1.0f, 1.0f, 1.0f), float4(1.0f, 1.0f, 1.0f, 1.0f) * TexScale);
	//outDiffuse = g_Texture.Sample(g_SamplerState, inTexCoord * TexScale, TexUVpx);
	//outDiffuse = float4(1.0f, 1.0f, 1.0f, 1.0f);
 }
