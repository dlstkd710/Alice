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
// Textured
//--------------------------------------------------------------//
//--------------------------------------------------------------//
// Pass 0
//--------------------------------------------------------------//
string Textured_Pass_0_Model : ModelData = "..\\..\\..\\Program Files (x86)\\AMD\\RenderMonkey 1.82\\Examples\\Media\\Models\\Sphere.3ds";

float4x4 matViewProjection : ViewProjection;
float    fTime : Time0_X;

struct VS_INPUT 
{
   float4 Position : POSITION0;
   float2 Texcoord : TEXCOORD0;
   
};

struct VS_OUTPUT 
{
   float4 Position : POSITION0;
   float2 Texcoord : TEXCOORD0;
   
};

VS_OUTPUT Textured_Pass_0_Vertex_Shader_vs_main( VS_INPUT Input )
{
   VS_OUTPUT Output;

   Output.Position = mul( Input.Position, matViewProjection );
   Output.Texcoord = float2(Input.Texcoord.x + fTime, Input.Texcoord.y);

   return( Output );
   
}




texture base_Tex
<
   string ResourceName = "..\\..\\..\\Program Files (x86)\\AMD\\RenderMonkey 1.82\\Examples\\Media\\Textures\\Earth.jpg";
>;
sampler2D baseMap = sampler_state
{
   Texture = (base_Tex);
   ADDRESSU = WRAP;
   ADDRESSV = WRAP;
   MINFILTER = LINEAR;
   MAGFILTER = LINEAR;
};
texture Coin_Tex
<
   string ResourceName = "..\\..\\..\\Program Files (x86)\\AMD\\RenderMonkey 1.82\\Examples\\Media\\Textures\\Coin.dds";
>;
sampler2D Texture1 = sampler_state
{
   Texture = (Coin_Tex);
};

struct PS_INPUT 
{
   float2 Texcoord : TEXCOORD0;
   
};

float4 Textured_Pass_0_Pixel_Shader_ps_main( PS_INPUT Input ) : COLOR0
{   
   float4 albedo1 = tex2D(baseMap ,Input.Texcoord);
   float4 albedo2 = tex2D(Texture1 ,Input.Texcoord);
   return( (albedo1 + albedo2) / 2 ) ;
   
}




//--------------------------------------------------------------//
// Technique Section for Textured
//--------------------------------------------------------------//
technique Textured
{
   pass Pass_0
   {
      VertexShader = compile vs_2_0 Textured_Pass_0_Vertex_Shader_vs_main();
      PixelShader = compile ps_2_0 Textured_Pass_0_Pixel_Shader_ps_main();
   }

}

