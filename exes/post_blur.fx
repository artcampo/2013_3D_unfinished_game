Texture2D textCol;

float   Width;
float   Height;

SamplerState samLinear
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Clamp;
    AddressV = Clamp;
};

struct VS_Input
{
    float4 Pos : POSITION;
    float2 Tex : TEXCOORD0;
};

struct VS_Output
{
    float4 Pos : SV_POSITION;              
    float2 Tex : TEXCOORD0;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_Output VS_FINAL_COMPOSITION( VS_Input Input )
{
  VS_Output Output;
  Output.Pos = Input.Pos;
  Output.Tex = Input.Tex;

  return Output;
}



//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------

float4 PS_BLURH( VS_Output Input ) : SV_TARGET
{    
  float off[3] = {0,1,2};
  float kernel[3] = { 0.2270270270, 0.3162162162, 0.0702702703 };
  float2 vSamplePosition  = Input.Tex;

  float   offset = 1.0f / Width;

  float4 vColor = textCol.Sample( samLinear, vSamplePosition) * kernel[0];    
  for ( int i = 1; i < 3; ++i)
  {
    vColor.xyz += textCol.Sample( samLinear, vSamplePosition + float2( offset*off[i], 0 ))*kernel[i];    
    vColor.xyz += textCol.Sample( samLinear, vSamplePosition - float2( offset*off[i], 0 ))*kernel[i];    
  }

  vColor.a = 1.0f;



  return vColor;

}

float4 PS_BLURV( VS_Output Input ) : SV_TARGET
{    
  float off[3] = {0,1,2};
  float kernel[3] = { 0.2270270270, 0.3162162162, 0.0702702703 };
  float2 vSamplePosition  = Input.Tex;

  float   offset = 1.0f / Height;

  float4 vColor = textCol.Sample( samLinear, vSamplePosition) * kernel[0];    
  for ( int i = 1; i < 3; ++i)
  {
    vColor.xyz += textCol.Sample( samLinear, vSamplePosition + float2( 0,offset*off[i] ))*kernel[i];    
    vColor.xyz += textCol.Sample( samLinear, vSamplePosition - float2( 0,offset*off[i] ))*kernel[i];    
  }
  vColor.a = 1.0f;

  return vColor;

}

//--------------------------------------------------------------------------------------
technique10 BLUR
{

    pass H
    {
        SetVertexShader( CompileShader( vs_4_0, VS_FINAL_COMPOSITION() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS_BLURH() ) );
    }

    pass V
    {
        SetVertexShader( CompileShader( vs_4_0, VS_FINAL_COMPOSITION() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS_BLURV() ) );
    }
}
