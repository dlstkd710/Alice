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
// Textured Phong
//--------------------------------------------------------------//
//--------------------------------------------------------------//
// Pass 0
//--------------------------------------------------------------//
string Textured_Phong_Pass_0_Model : ModelData = ".\\16.12.18 Portfolio Framework\\Dx3D\\Object\\th\\th.X";

float4x4 matWorld : World;
float4x4 matViewProjection : ViewProjection;

struct VS_INPUT 
{
   float4 Position : POSITION0;
   float3 Normal   : NORMAL0;
};

struct VS_OUTPUT 
{
   float4 Position : POSITION0;
};

VS_OUTPUT Textured_Phong_Pass_0_Vertex_Shader_vs_main( VS_INPUT Input )
{
   VS_OUTPUT Output;

   Output.Position = mul( Input.Position, matWorld );
   Output.Position = mul( Input.Position, matViewProjection );
   
   float3 normal = mul( Input.Normal, (float3x3)matWorld );
   normal = mul( normal, (float3x3)matViewProjection );
   normal = normalize(normal);
   Output.Position.xy += normal.xy * 1.5f;
      
   return( Output );
   
}




float4 Textured_Phong_Pass_0_Pixel_Shader_ps_main() : COLOR0
{   
   return( float4( 1.0f, 0.0f, 0.0f, 1.0f ) );
   
}


//--------------------------------------------------------------//
// Technique Section for Textured Phong
//--------------------------------------------------------------//
technique Textured_Phong
{
   pass Pass_0
   {
      CULLMODE = CW;
      ZWRITEENABLE = FALSE;

      VertexShader = compile vs_2_0 Textured_Phong_Pass_0_Vertex_Shader_vs_main();
      PixelShader = compile ps_2_0 Textured_Phong_Pass_0_Pixel_Shader_ps_main();
   }
}

