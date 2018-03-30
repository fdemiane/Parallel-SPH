Texture2D AlbedoMap : register(t0);
Texture2D NormalMap : register(t1);
Texture2D DepthMap : register(t2);

SamplerState LinearSampler : register(s0);
SamplerState PointSampler : register(s1);
SamplerState AnisotropySampler : register(s2);


cbuffer ConstantBuffer : register(b0)
{
	matrix Projection;
	matrix View;
	matrix Model;
}


struct VS_OUTPUT
{

	
	float2 UV : TEXCOORD0;

};

VS_OUTPUT VS(float4 iPos:POSITION, out float4 Pos : SV_POSITION)
{
	VS_OUTPUT output;
	Pos = float4(iPos.x, iPos.y, 0, 1);
	float2 ex_UV = float2(Pos.x + 1, -Pos.y + 1) *0.5;
	output.UV = ex_UV;
	return output;
}

float4 PS(VS_OUTPUT input ) : SV_TARGET
{

	// Position reconstruction from depth
	float z = DepthMap.Sample(LinearSampler , input.UV).r;
	z = 2 * z - 1;
	float x = input.UV.x * 2 - 1;
	float y = (1-input.UV.y) * 2 - 1;
	float4 CameraSpacePos = float4(x, y, z, 1);
	CameraSpacePos = mul(CameraSpacePos, Projection);
	CameraSpacePos.xyz /= CameraSpacePos.w;
	float4 CameraSpaceNormals =float4( NormalMap.Sample(LinearSampler, input.UV).rgb , 1);
	CameraSpaceNormals = mul(float4(CameraSpaceNormals.xyz , 0), View);
	CameraSpaceNormals = normalize(float4(CameraSpaceNormals.xyz , 1));

	float3 LightDir = float3(0, 1, 0);
	LightDir = mul(LightDir, View);
	LightDir = -CameraSpacePos.xyz + LightDir;
	LightDir = normalize(LightDir);

	float ndotL = dot(normalize(CameraSpaceNormals.xyz), LightDir);

	//return CameraSpaceNormals;
	return float4(AlbedoMap.Sample(LinearSampler , input.UV).xyz*ndotL+0.2f , 1);
}