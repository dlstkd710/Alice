//**************************************************************//
//  Effect File exported by RenderMonkey 1.6
//
//  - Although many improvements were made to RenderMonkey FX  
//    file export, there are still situations that may cause   
//    compilation problems once the file is exported, such as  
//    occasional naming conflicts for methods, since FX format 
//    does not support any notions of name spaces. You need to 
//    try to create workspaces in such a way as to minimize    
//    potential naming conflicts on export.                    
//    
//  - Note that to minimize resulting name collisions in the FX 
//    file, RenderMonkey will mangle names for passes, shaders  
//    and function names as necessary to reduce name conflicts. 
//**************************************************************//

//--------------------------------------------------------------//
// ShadowMapping
//--------------------------------------------------------------//
//--------------------------------------------------------------//
// ApplyShadowTorus
//--------------------------------------------------------------//
string ShadowMapping_ApplyShadowTorus_Torus : ModelData = "C:\\Program Files (x86)\\AMD\\RenderMonkey 1.82\\Examples\\Media\\Models\\Torus.3ds";

struct VS_INPUT 
{
   float4 pos : POSITION;
   float3 normal : NORMAL;   
};

struct VS_OUTPUT 
{
   float4 pos : POSITION0;
   float4 clipPos : TEXCOORD1;
   float diffuse : TEXCOORD2;
};

float4x4 gWorldMat : World;
float4x4 gLightViewMat;
float4x4 gLightProjMat : Projection;

float4 gWorldLightPos
<
   string UIName = "gWorldLightPos";
   string UIWidget = "Direction";
   bool UIVisible =  false;
   float4 UIMin = float4( -10.00, -10.00, -10.00, -10.00 );
   float4 UIMax = float4( 10.00, 10.00, 10.00, 10.00 );
   bool Normalize =  false;
> = float4( 500.00, 500.00, -500.00, 1.00 );

float4x4 gViewProjMat : ViewProjection;

VS_OUTPUT ShadowMapping_ApplyShadowTorus_Vertex_Shader_vs_main( VS_INPUT input)
{
   VS_OUTPUT output;
   
   float4x4 lightViewMat = gLightViewMat;  
   
   output.pos = mul( input.pos, gWorldMat);
   output.pos = mul( output.pos, lightViewMat);
   output.pos = mul( output.pos, gLightProjMat);
   
   //render world-view-projection position
   float4 worldPos = mul( input.pos, gWorldMat);
   output.pos = mul( worldPos, gViewProjMat);
   
   //light world-view-projection position
   output.clipPos = mul( worldPos, lightViewMat);
   output.clipPos = mul( output.clipPos, gLightProjMat);
   
   float3 lightDir = normalize( worldPos.xyz - gWorldLightPos.xyz);
   float3 worldNormal = normalize( mul( input.normal, (float3x3)gWorldMat));
   
   output.diffuse = dot(-lightDir, worldNormal);
   
   return output;
   
}




texture ShadowMap_Tex
<
   string ResourceName = ".\\";
>;
sampler2D ShadowSampler = sampler_state
{
   Texture = (ShadowMap_Tex);
};

struct PS_INPUT
{
   float4 clipPos: TEXCOORD1;
   float diffuse: TEXCOORD2;
};

float4 gObjectColor
<
   string UIName = "gObjectColor";
   string UIWidget = "Color";
   bool UIVisible =  true;
> = float4( 0.31, 0.54, 1.00, 1.00 );

float4 ShadowMapping_ApplyShadowTorus_Pixel_Shader_ps_main( PS_INPUT input) : COLOR
{   
   float3 rgb = saturate( input.diffuse) * gObjectColor;
   
   //current pixel depth
   float currentDepth = input.clipPos.z / input.clipPos.w;
   
   //shadowmap pixel depth
   float2 uv = input.clipPos.xy / input.clipPos.w;
   uv.y = -uv.y;
   uv = uv * 0.5 + 0.5;
   
   float shadowDepth = tex2D( ShadowSampler, uv).r;
   
   if( currentDepth > shadowDepth + 0.0000125f)
   {
      rgb *= 0.5f;
   }
   
   return float4( rgb, 1.0f);
}




//--------------------------------------------------------------//
// Technique Section for ShadowMapping
//--------------------------------------------------------------//
technique ShadowMapping
{
   pass ApplyShadowTorus
   {
      VertexShader = compile vs_2_0 ShadowMapping_ApplyShadowTorus_Vertex_Shader_vs_main();
      PixelShader = compile ps_2_0 ShadowMapping_ApplyShadowTorus_Pixel_Shader_ps_main();
   }

}

