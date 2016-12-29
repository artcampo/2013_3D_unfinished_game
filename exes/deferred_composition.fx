#include "shader_defines.fx"
#include "simplex.fx"

Texture2D textCol;
Texture2D textNor;
Texture2D textLig;
Texture2D textPos;
Texture2D textSSDO;

float     fogYPlaneTop;
float     fogYPlaneBottom;
float     fogCircRadius;
float3    fogCircCenter;

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



float4 PS_FINAL_COMPOSITION( VS_Output Input ) : SV_TARGET
{    
  float2 vSamplePosition = Input.Tex;

  float4 ssdoTerm  = textSSDO.Sample( samLinear, vSamplePosition);    
  float4 vertexCol = textCol.Sample ( samLinear, vSamplePosition);      
  float4 vertexLig = textLig.Sample ( samLinear, vSamplePosition);    
  float4 vertexNor = textNor.Sample ( samLinear, vSamplePosition);    
  float4 vertexPos = textPos.Sample ( samLinear, vSamplePosition);    

  float4 pixelColor;


  static const float PI = 3.14159265f;

  ///////////////////////////////////////////////////////////////////////////
  //  Ambiental Light
  ///////////////////////////////////////////////////////////////////////////
  float   elevationAngle = acos ( vertexNor.y );
  float   orientationAngle = atan2 ( vertexNor.x, vertexNor.z) + PI;

//  if ( abs(vertexNor.y - 1 ) < 0.1 )
  if ( isnan( elevationAngle ) )
  {
    elevationAngle    = PI;
    orientationAngle  = 0;
  }

  float   rangeOfColors     = PI*0.5*1.9;
  float   rangeOfColorsInv  = 1/rangeOfColors;

  float   angleMod1   = min( distance( orientationAngle, PI/2), distance( orientationAngle-2*PI, PI/2) );
  float   intensity1  = min( rangeOfColors, angleMod1 );
  intensity1          *= rangeOfColorsInv;
  intensity1          = 1 - intensity1;

  float   angleMod2   = min( distance( orientationAngle, (PI*3/2)), distance( orientationAngle+2*PI, (PI*3/2)) );
  float   intensity2  = min ( rangeOfColors, angleMod2 );
  intensity2          *= rangeOfColorsInv;
  intensity2          = 1 - intensity2;
  float3  ambientColour1 = float3(0.878, 0.858,0.757);
  float3  ambientColour2 = float3(0.541,0.373,0.286);
  float3  ambientColour = ambientColour1*intensity1 + ambientColour2*intensity2;
  float   lightAmbientIntensity = elevationAngle / PI;

  lightAmbientIntensity   = 1- lightAmbientIntensity;

  ///////////////////////////////////////////////////////////////////////////
  //  Pixel Color
  ///////////////////////////////////////////////////////////////////////////

  pixelColor.xyz = (lightAmbientIntensity*ambientColour + vertexLig.xyz ) *vertexCol.xyz;
//  pixelColor.xyz = vertexCol.xyz;

  ///////////////////////////////////////////////////////////////////////////
  //  Fog
  ///////////////////////////////////////////////////////////////////////////

  float3  fogColor            = float3(1,1,1);
  float   fogPlaneIntensity   = (-1)*(vertexPos.y - fogYPlaneTop);
  float   fogPlaneHeight      = fogYPlaneTop - fogYPlaneBottom;
  fogPlaneIntensity           = fogPlaneIntensity / fogPlaneHeight;
  fogPlaneIntensity           = min(1, fogPlaneIntensity );

  float   fogCircIntensity    = length( vertexPos - fogCircCenter );
  fogCircIntensity            /= fogCircRadius;
  fogCircIntensity            = min(1, fogCircIntensity );

  float   fogIntensity        = min( fogPlaneIntensity, fogCircIntensity );
  float   fogPositionIntensity  = noise3D(vertexPos.xyz*0.5) * noise3D(vertexPos.xyz*0.2 + float3(10,10,10));
  fogIntensity  = fogIntensity*fogPositionIntensity;

  ///////////////////////////////////////////////////////////////////////////
  //  Final pixel
  ///////////////////////////////////////////////////////////////////////////


  fogIntensity = clamp(fogIntensity,0,1);
  pixelColor.xyz = lerp( pixelColor.xyz, fogColor, fogIntensity);
//  pixelColor.xyz = float3(1,1,1)*fogIntensity;

////////////////////////////////////////  DEBUG
/*
  //  Debug that intensity is in range
  float3 debugCol = float3(1,1,1);
  if ( fogIntensity > 1 )
    debugCol = float3(1,0,0);
  if (  fogIntensity < 0)
    debugCol = float3(0,1,0);
  pixelColor.xyz = debugCol;
*/
////////////////////////////////////////  /DEBUG


	return pixelColor;
}

//--------------------------------------------------------------------------------------
technique10 Comp
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
