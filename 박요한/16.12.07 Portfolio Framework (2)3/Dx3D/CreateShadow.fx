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
// CreateShadow
//--------------------------------------------------------------//
string ShadowMapping_CreateShadow_Torus : ModelData = "C:\\Program Files (x86)\\AMD\\RenderMonkey 1.82\\Examples\\Media\\Models\\Torus.3ds";

texture ShadowMap_Tex : RenderColorTarget
<
   float2 RenderTargetDimensions = {2048,2048};
   string Format="D3DFMT_R32F";
   float  ClearDepth=1.000000;
   int    ClearColor=-1;
>;
struct VS_INPUT
{
   float4 pos : POSITION;   
};

struct VS_OUTPUT
{
   float4 pos : POSITION;
   float4 clipPos : TEXCOORD1;
};


float4x4 gWorldMat : World;
float4x4 gLightViewMat
<
   string UIName = "gLightViewMat";
   string UIWidget = "Numeric";
   bool UIVisible =  false;
> = float4x4( 1.00, 0.00, 0.00, 0.00, 0.00, 1.00, 0.00, 0.00, 0.00, 0.00, 1.00, 0.00, 0.00, 0.00, 0.00, 1.00 );
float4x4 gLightProjMat : Projection;

VS_OUTPUT ShadowMapping_CreateShadow_Vertex_Shader_vs_main( VS_INPUT input)
{
   VS_OUTPUT output;
   
   float4x4 lightViewMat = gLightViewMat;  
      
   output.pos = mul( input.pos, gWorldMat);
   output.pos = mul( output.pos, lightViewMat);
   output.pos = mul( output.pos, gLightProjMat); 
   
   output.clipPos = output.pos;
      
   return output;
}
struct PS_INPUT
{
   float4 clipPos :TEXCOORD1;
};

float4 ShadowMapping_CreateShadow_Pixel_Shader_ps_main( PS_INPUT input) : COLOR
{   
   float depth = input.clipPos.z / input.clipPos.w;
   return float4( depth.xxx, 1);
}




//--------------------------------------------------------------//
// Technique Section for ShadowMapping
//--------------------------------------------------------------//
technique ShadowMapping
{
   pass CreateShadow
   <
      string Script = "RenderColorTarget0 = ShadowMap_Tex;"
                      "ClearColor = (255, 255, 255, 255);"
                      "ClearDepth = 1.000000;";
   >
   {
      VertexShader = compile vs_2_0 ShadowMapping_CreateShadow_Vertex_Shader_vs_main();
      PixelShader = compile ps_2_0 ShadowMapping_CreateShadow_Pixel_Shader_ps_main();
   }

}

