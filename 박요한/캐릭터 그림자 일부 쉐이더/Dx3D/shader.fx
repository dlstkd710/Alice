float4x4 matWorld : World;
float4x4 matView : View;
float4x4 matProjection : Projection;
float4	 color;
	
struct VS_INPUT 
{
   float4 Position : POSITION0;   
};

struct VS_OUTPUT 
{
   float4 Position : POSITION0;
};

VS_OUTPUT Default_DirectX_Effect_Pass_0_Vertex_Shader_vs_main( VS_INPUT Input )
{
   VS_OUTPUT Output;

   Output.Position = mul( Input.Position, matWorld );
   Output.Position = mul( Output.Position, matView );
   Output.Position = mul( Output.Position, matProjection );
   
   return( Output );
}

float4 Default_DirectX_Effect_Pass_0_Pixel_Shader_ps_main() : COLOR0
{   
   return( color );
   
}




//--------------------------------------------------------------//
// Technique Section for Default_DirectX_Effect
//--------------------------------------------------------------//
technique Default_DirectX_Effect
{
   pass Pass_0
   {
      VertexShader = compile vs_2_0 Default_DirectX_Effect_Pass_0_Vertex_Shader_vs_main();
      PixelShader = compile ps_2_0 Default_DirectX_Effect_Pass_0_Pixel_Shader_ps_main();
   }

}

