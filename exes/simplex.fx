Texture2D<uint>   PermTexture;   // permutation texture

// Find which simplex we are in by magnitude sorting

void Simplex3D( const in float3 P, out float3 simplex[4] )
{
    float3 T = P.xzy >= P.yxz;
    simplex[0] = 0;
    simplex[1] = T.xzy > T.yxz;
    simplex[2] = T.yxz <= T.xzy;
    simplex[3] = 1;
}


// Hashing functions

int Hash( const in float3 P )
{
    //P /= 256;   // normalize texture coordinate
    //return PermTexture.SampleLevel( SamplerRepeat, P.xy, 0 ).r ^ PermTexture.SampleLevel( SamplerRepeat, float2( P.z, 0 ), 0 ).r;
	return PermTexture.Load( int3(P.xy, 0) ).r ^ PermTexture.Load( int3( P.z, 0, 0 ) ).r;
}

int Hash( const in float4 P )
{
    //P /= 256;   // normalize texture coordinate
    //return PermTexture.SampleLevel( SamplerRepeat, P.xy, 0 ).r ^ PermTexture.SampleLevel( SamplerRepeat, P.zw, 0 ).r;
	return PermTexture.Load( int3(P.xy, 0) ).r ^ PermTexture.Load( int3(P.zw, 0) ).r;

    //return PermTexture.SampleLevel( SamplerRepeat,
    //    float2(PermTexture.SampleLevel( SamplerRepeat, P.xy, 0 ).r, P.z) + P.w, 0 ).r;
}

// gradient tables

cbuffer Gradients
{
    // 3D gradients
    
    const float3 Grad3[] = {
        float3( -1, -1, 0 ), // 0
        float3( -1, +1, 0 ), // 1
        float3( +1, -1, 0 ), // 2
        float3( +1, +1, 0 ), // 3

        float3( -1, 0, -1 ), // 4
        float3( -1, 0, +1 ), // 5
        float3( +1, 0, -1 ), // 6
        float3( +1, 0, +1 ), // 7

        float3( 0, -1, -1 ), // 8
        float3( 0, -1, +1 ), // 9
        float3( 0, +1, -1 ), // 10
        float3( 0, +1, +1 ), // 11

        // padding   
        float3( +1, +1, 0 ), // 12
        float3( -1, -1, 0 ), // 13
        float3( 0, -1, +1 ), // 14
        float3( 0, -1, -1 )  // 15
    };

    // 4D case is more regular

    const float4 Grad4[] = {
        // x, y, z
        float4( -1, -1, -1, 0 ), // 0
        float4( -1, -1, +1, 0 ), // 1
        float4( -1, +1, -1, 0 ), // 2
        float4( -1, +1, +1, 0 ), // 3
        float4( +1, -1, -1, 0 ), // 4
        float4( +1, -1, +1, 0 ), // 5
        float4( +1, +1, -1, 0 ), // 6
        float4( +1, +1, +1, 0 ), // 7
        // w, x, y
        float4( -1, -1, 0, -1 ), // 8
        float4( -1, +1, 0, -1 ), // 9
        float4( +1, -1, 0, -1 ), // 10
        float4( +1, +1, 0, -1 ), // 11
        float4( -1, -1, 0, +1 ), // 12
        float4( -1, +1, 0, +1 ), // 13
        float4( +1, -1, 0, +1 ), // 14
        float4( +1, +1, 0, +1 ), // 15
        // z, w, x
        float4( -1, 0, -1, -1 ), // 16
        float4( +1, 0, -1, -1 ), // 17
        float4( -1, 0, -1, +1 ), // 18
        float4( +1, 0, -1, +1 ), // 19
        float4( -1, 0, +1, -1 ), // 20
        float4( +1, 0, +1, -1 ), // 21
        float4( -1, 0, +1, +1 ), // 22
        float4( +1, 0, +1, +1 ), // 23
        // y, z, w
        float4( 0, -1, -1, -1 ), // 24
        float4( 0, -1, -1, +1 ), // 25
        float4( 0, -1, +1, -1 ), // 26
        float4( 0, -1, +1, +1 ), // 27
        float4( 0, +1, -1, -1 ), // 28
        float4( 0, +1, -1, +1 ), // 29
        float4( 0, +1, +1, -1 ), // 30
        float4( 0, +1, +1, +1 ) // 31
    };
};

// Basic 3D simplex noise

#define F3 0.333333333333
#define G3 0.166666666667

float Snoise3D( const in float3 P )
{

    // Skew the (x,y,z) space to determine which cell of 6 simplices we're in

    float s = dot( P, F3 );
    float3 Pi = floor( P + s );
    float t = dot( Pi, G3 );

    float3 P0 = Pi - t;               // Unskew the cell origin back to (x,y,z) space
    float3 Pf0 = P - P0;            // The x,y,z distances from the cell origin

    // For the 3D case, the simplex shape is a slightly irregular tetrahedron.
    // To find out which of the six possible tetrahedra we're in, we need to
    // determine the magnitude ordering of x, y and z components of Pf0.

    float3 simplex[4];
    Simplex3D( Pf0, simplex );

    float n = 0;

    [unroll]
    for (int i = 0; i<4; i++)
    {
        float3 Pf = Pf0 - simplex[i] + G3 * i;
        int h = Hash( Pi + simplex[i] );
        float d = saturate( 0.6f - dot( Pf, Pf ) );
        d *= d; 
        n += d * d * dot(Grad3[ h & 31 ], Pf);
        //n += d * d * grad( Pf, h );
    }

    return 32.0f * n;
}

float noise3D( const in float3 P )
{
  return  Snoise3D(P)*0.5 + 0.5;
}