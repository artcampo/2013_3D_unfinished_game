
cbuffer cbChangesEveryResize
  {
  matrix    mP;
  matrix    mV;
  float3    camPos;
  };

cbuffer cbChangesForEveryLight
  {
  matrix    mW;
  float3    lPos;
  float4    lCol;
  float     lRad;
  };

Texture2D textPos2;
Texture2D textNor2;
Texture2D textTan2;
Texture2D textCol2;
Texture2D textLightP;

SamplerState samLinear
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Clamp;
    AddressV = Clamp;
};

DepthStencilState EnableDepthLess
{
    DepthEnable = TRUE;
    DepthWriteMask = ALL;
	  DepthFunc = LESS;
};

DepthStencilState StencilState_NoDepth
{
    DepthEnable = false;
    DepthWriteMask = ZERO;
    DepthFunc = Less;
};

BlendState NoBlending
{
    BlendEnable[0] = FALSE;
    RenderTargetWriteMask[0] = 0x0F;
};

BlendState SrcAlphaBlendingAdd
{
    BlendEnable[0] = TRUE;
    SrcBlend = SRC_ALPHA;
    DestBlend = INV_SRC_ALPHA;
    BlendOp = ADD;
    SrcBlendAlpha = ONE;
    DestBlendAlpha = ONE;
    BlendOpAlpha = ADD;
    RenderTargetWriteMask[0] = 0x0F;
};

BlendState BlendingDef
{
    AlphaToCoverageEnable = false;
    BlendEnable[0] = TRUE;
    SrcBlend = ONE;
    DestBlend = ONE;
    BlendOp = ADD;
    SrcBlendAlpha = SRC_ALPHA;
    DestBlendAlpha = DEST_ALPHA;
    BlendOpAlpha = ADD;
    RenderTargetWriteMask[0] = 0x0F;
};

struct VS_INPUT
{
    float3 Pos : POSITION;
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;   
    float4 PosWC : POSITION;   
    float2 Tex : TEXCOORD0;
};



//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS( VS_INPUT input )
{

    VS_OUTPUT output;

	  float4x4 gWVP;
	  gWVP = mul(mW,mV);
	  gWVP = mul(gWVP,mP);
    output.Pos 	  = mul ( float4(input.Pos.xyz,1.0f) , gWVP);
    output.PosWC  = mul ( float4(input.Pos.xyz,1.0f) , mW);

    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------

float3 lighting(float3 pos, float3 base, float3 normal ){

	// Lighting vectors
	float3 lVec = (lPos - pos);
	float3 lightVec = normalize(lVec);
	float3 viewVec = normalize(pos - camPos);

	// Attenution that falls off to zero at light radius
	const float a = 0.15;
  float distance = length(lVec);
	//float atten = 2 * saturate((1.0 + a) / (1.0 + (1.0 / a) * dot(lVec, lVec)) - a);
//float atten = 1.0 / (distance * distance);
//float atten = 1.0 / (1.0f + 2.0f *distance + 2.0f * distance * distance);

  float invRadius = 1.0f / lRad ;
  float atten = clamp(1.0 - invRadius * distance, 0.0, 1.0);
//atten = 1.0f;

	// Lighting
	float diffuse = saturate(dot(lightVec, normal));
	float specular = 0.2 * pow(saturate(dot(reflect(-viewVec, normal), lightVec)), 10.0);


  return ((atten * (diffuse + specular))*base);
//  return atten * base;
}

float3 lightingSpecParams(float3 pos, float3 base, float3 normal, float specIntensity, float3 specLight ){

	float3 lVec = (lPos - pos);
	float3 lightVec = normalize(lVec);
	float3 viewVec = normalize(pos - camPos);

	const float a = 0.15;
  float distance = length(lVec);

  float invRadius = 1.0f / lRad ;
  float atten = clamp(1.0 - invRadius * distance, 0.0, 1.0);
  atten *= specIntensity;


	// Lighting
	float diffuse = saturate(dot(lightVec, normal));
	float specular = 0.2 * pow(saturate(dot(reflect(-viewVec, normal), lightVec)), 10.0);


  return ((atten * (diffuse*base + specular*specLight)));
}

float4 PS( VS_OUTPUT input ) : SV_Target
{ 
  float2 coordTex = float2( input.Pos.x/1024, input.Pos.y/768);
  
  float4 vertexPos = textPos2.Sample( samLinear, coordTex );    
  float4 vertexNor = textNor2.Sample( samLinear, coordTex );   


  float4  pixelLightPropierties = textLightP.Sample( samLinear, coordTex );
  float   pixelSpecIntensity    = pixelLightPropierties.a;
  float3  pixelSpecLight        = pixelLightPropierties.xyz;
  
  float4  color = float4 (
                  lightingSpecParams( vertexPos.xyz , 
                                      lCol.xyz, 
                                      vertexNor.xyz, 
                                      pixelSpecIntensity, 
                                      pixelSpecLight  ) 
                  , 1);


  return color;
}

//--------------------------------------------------------------------------------------
technique10 SSDO_LIGHTING_PASS
{

    pass P0
    {
        SetBlendState( BlendingDef, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
        SetDepthStencilState( StencilState_NoDepth, 0 );

        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
    }
}
