
Texture2D Diffuse : register(t0);

SamplerState LinearSampler : register(s0);
SamplerState PointSampler : register(s1);
SamplerState AnisotropySampler : register(s2);

cbuffer ConstantBuffer : register(b0)
{
	matrix Projection;
	matrix View;
	matrix Model;
}


struct VS_INPUT
{
	float4 Pos : POSITION;
	float2 UV : TEXCOORD0;
	float3 Normal : NORMAL;
	float3 Tangent : TANGENT;
	float3 Bitangent : BITANGENT;
};

struct VS_OUTPUT
{

	float4 PosW : POSITION;
	float3 NormalW : NORMAL;
	float2 UV : TEXCOORD0;
	
};

VS_OUTPUT VS(VS_INPUT input, out float4 Pos : SV_POSITION)
{
	VS_OUTPUT output;

	Pos = input.Pos;
	Pos = mul(Pos, Model);
	Pos = mul(Pos, View);
	Pos = mul(Pos, Projection);

	output.PosW = input.Pos;
	output.NormalW = input.Normal;
	output.UV = input.UV;

	return output;
}

float4 PS(VS_OUTPUT input) : SV_TARGET
{
	float nDotL = dot(input.NormalW , float3(1,0,0));
	nDotL = 1;
	float3 color = Diffuse.Sample(LinearSampler ,input.UV );
	return float4(color , 1);
}




