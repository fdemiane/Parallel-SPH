Texture2D inputTexture : register(t0);

SamplerState LinearSampler : register(s0);


struct VS_OUTPUT
{
	float2 Tex : TEXCOORD0;
};

VS_OUTPUT VS(float4 iPos:POSITION, out float4 Pos : SV_POSITION)
{
	VS_OUTPUT output;
	Pos = float4(iPos.x, iPos.y, 0, 1);
	float2 ex_UV = float2(Pos.x + 1, -Pos.y + 1) *0.5;
	output.Tex = ex_UV;
	return output;
}


float4 PS(VS_OUTPUT input) : SV_Target
{
	return inputTexture.Sample(LinearSampler, input.Tex);
}