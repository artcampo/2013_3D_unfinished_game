Texture2D textPos;
Texture2D textNor;
Texture2D textTan;
Texture2D textCol;
Texture2D textLig;

Texture1D <float3> textRandDir;

float4x4 View     : VIEW;
float4x4 ViewProj : VIEWPROJECTION;

float   g_Radius;
float   g_TanBias;
float2  g_InvFocal;
float2  g_Focal;
float   g_FarPlane;
float   g_NearPlane;
float2  g_Resolution;
float2  g_InvResolution;


RasterizerState rs_noCull
{
CullMode=None;
};

BlendState NoBlending
{
    BlendEnable[0] = FALSE;
    RenderTargetWriteMask[0] = 0x0F;
};

SamplerState samLinear
{
    Filter = MIN_MAG_MIP_POINT;
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
// Functions
//--------------------------------------------------------------------------------------

//  try to force register usage
static const float PI = 3.14159265f;



float2 rotate_direction( float2 v1, float2 v2 )
{
  return float2( v1.x*v2.y - v1.y*v2.x, v1.y*v2.y + v1.x*v2.x );
}

float computeAo ( float4 pWS, float4 nWS, float dMax, const float2 uv )
{
  float4  sWS   = textPos.Sample( samLinear, uv);  

  float   d     = distance( pWS.xyz, sWS.xyz );
  float   t     = 1 - min(d*d / dMax*dMax , 1);
  float3  psWS  = sWS.xyz - pWS.xyz;
  float   costh = max( 0, dot(nWS.xyz, psWS.xyz ) );

  return  t*costh;
}

float aoSampleLevel( float4 pWS, float4 nWS, float dMax, const float2 uv, const float radius, const int numSamples )
{
  float2 stepSize = float2( g_InvResolution.x, g_InvResolution.y );
  stepSize        = stepSize*radius;

  int2 pos        = uv* g_Resolution;
  pos             = pos + int2(radius, numSamples);
  pos             = int2( int(pos.x & 63), int(pos.y & 63) );
  float3 randDir  = textRandDir.Load( int2(pos.x + 64*pos.y, 0 ) ).xyz;

  float ao = 0;
  for ( int i = 0; i < numSamples ; ++i )
    {
      float angle = 2*PI*float(i) / float(numSamples);
      float2 sUV  = float2( cos(angle), sin(angle ) );
      sUV = rotate_direction ( stepSize, sUV.xy );
      sUV = rotate_direction ( sUV, randDir.xy );
 
      ao   += computeAo( pWS, nWS, dMax, uv + sUV );
    }


  return 1 - ( ao / numSamples*4);
}


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

float rand(float2 co){
    return frac(sin(dot(co.xy ,float2(12.9898,78.233))) * 43758.5453);
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------



float4 PS_FINAL_COMPOSITION( VS_Output Input ) : SV_TARGET
{    

  static const float PI = 3.14159265f;

  //  Init variables
   float4 pWS   = textPos.Sample( samLinear, Input.Tex);  
   float4   nWS   = textNor.Sample( samLinear, Input.Tex);  
   float   dMax  = 3;

  //  Compute

//float aoSampleLevel( float4 pWS, float4 nWS, float dMax, const float2 uv, const float radius, const int numSamples )
  float finalAo = aoSampleLevel(pWS, nWS, 4,  Input.Tex, 4,  32);

  float4 color  = float4( finalAo, finalAo, finalAo, 1 );
    

	return color;
}

//--------------------------------------------------------------------------------------
technique10 SSAO
{

    pass P0
    {
        SetBlendState( NoBlending, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
        SetRasterizerState(rs_noCull);
        SetVertexShader( CompileShader( vs_4_0, VS_FINAL_COMPOSITION() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS_FINAL_COMPOSITION() ) );
    }
}
