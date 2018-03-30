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

struct PS_OUT
{
	float4 albedo : SV_TARGET0;
	float4 normal : SV_TARGET1;
	float4 parameters : SV_TARGET2;
	float4 param2 : SV_TARGET3;
};

VS_OUTPUT VS(VS_INPUT input, out float4 Pos : SV_POSITION)
{
	VS_OUTPUT output;

	Pos = input.Pos;
	//Pos.z = - Pos.z;
	Pos = mul(Pos, Model);
	Pos = mul(Pos, View);
	Pos = mul(Pos, Projection);
	//Pos.z = 1 - Pos.z;

	output.PosW = input.Pos;
	output.NormalW = input.Normal;
	output.UV = input.UV;

	return output;
}

PS_OUT PS(VS_OUTPUT input) 
{
	PS_OUT output;
	output.albedo = float4(Diffuse.Sample(LinearSampler, input.UV).rgb,1);
	output.albedo = float4(0, 0.8f, 1, 1);
	output.normal = float4(input.NormalW*0.5 + 0.5, 1);
	output.parameters = float4(1, 1, 1, 1);
	output.param2 = float4(1, 1, 1, 1);
	return output;
	
}