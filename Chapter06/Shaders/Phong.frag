// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

// Request GLSL 3.3
#version 330

// Inputs from vertex shader
// Tex coord
in vec2 fragTexCoord;
// Normal (in world space)
in vec3 fragNormal;
// Position (in world space)
in vec3 fragWorldPos;

// This corresponds to the output color to the color buffer
out vec4 outColor;

// This is used for the texture sampling
uniform sampler2D uTexture;

// Create a struct for directional light
struct DirectionalLight
{
	// Direction of light
	vec3 mDirection;
	// Diffuse color
	vec3 mDiffuseColor;
	// Specular color
	vec3 mSpecColor;
};

// 課題6.2 点光源
struct PointLight
{
	// 光源の位置
	vec3 mPosition;
	// 拡散反射色
	vec3 mDiffuseColor;
	// 鏡面反射色
	vec3 mSpecColor;
	// 鏡面反射指数
	float mSpecPower;
	// 影響半径
	float mInfluRadius;
};

// 円周率
const float PI = 3.14159;

// Uniforms for lighting
// Camera position (in world space)
uniform vec3 uCameraPos;
// Specular power for this surface
uniform float uSpecPower;
// Ambient light level
uniform vec3 uAmbientLight;

// Directional Light
uniform DirectionalLight uDirLight;

// 課題6.2 点光源（4つ）
uniform PointLight uPtsLights[4];

void main()
{
	// Surface normal
	vec3 N = normalize(fragNormal);
	// Vector from surface to light
	vec3 L = normalize(-uDirLight.mDirection);
	// Vector from surface to camera
	vec3 V = normalize(uCameraPos - fragWorldPos);
	// Reflection of -L about N
	vec3 R = normalize(reflect(-L, N));

	// Compute phong reflection
	vec3 Phong = uAmbientLight;
	float NdotL = dot(N, L);
	if (NdotL > 0)
	{
		vec3 Diffuse = uDirLight.mDiffuseColor * NdotL;
		vec3 Specular = uDirLight.mSpecColor * pow(max(0.0, dot(R, V)), uSpecPower);
		Phong += Diffuse + Specular;
	}

	for (int i=0; i<4; i++)
	{
		// 点光源と表面の距離
		float dist = distance(uPtsLights[i].mPosition, fragWorldPos);
		// 減衰率（1/4πr^2とした）
		float attenuation;
		// 距離が有効範囲より小さい場合のみ点光源は有効
		if (dist < uPtsLights[i].mInfluRadius && uPtsLights[i].mInfluRadius > 0.0)
		{
			attenuation = normalize(1.0 /(4.0 * PI * dist * dist));
		}
		else
		{
			attenuation = 0.0;
		}
		L = normalize(-uPtsLights[i].mPosition);
		R = normalize(reflect(-L, N));
		NdotL = dot(N, L);
		if (NdotL > 0)
		{
			vec3 Diffuse = uPtsLights[i].mDiffuseColor * NdotL;
			vec3 Specular = uPtsLights[i].mSpecColor * pow(max(0.0, dot(R, V)), uPtsLights[i].mSpecPower);
			Phong += (Diffuse + Specular) * attenuation;
		}
	}

	// Final color is texture color times phong light (alpha = 1)
    outColor = texture(uTexture, fragTexCoord) * vec4(Phong, 1.0);
}
