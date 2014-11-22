sampler2D Background : register(s0);

float dx;
float dy;

texture Height;
sampler HeightSampler = sampler_state
{
	Texture = (Height);
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};

float4 PS(float2 uv : TEXCOORD0) : COLOR0
{
	float xoff = tex2D(HeightSampler, float2(saturate(uv.x + dx), uv.y)).x - tex2D(HeightSampler, float2(saturate(uv.x - dx), uv.y)).x;
	float yoff = tex2D(HeightSampler, float2(uv.x, saturate(uv.y + dy))).x - tex2D(HeightSampler, float2(uv.x, saturate(uv.y - dy))).x;
	uv.x = saturate(uv.x + xoff / 20);
	uv.y = saturate(uv.y + yoff / 20);
	float4 color = tex2D(Background, uv);
	color.rgb += (xoff + yoff) / 2;
	return color;
}

technique simple
{
    pass Water
    {
        PixelShader = compile ps_2_0 PS();
    }
}
