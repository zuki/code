// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Renderer.h"
#include "Texture.h"
#include "Mesh.h"
#include <algorithm>
#include <sstream>
#include "Shader.h"
#include "VertexArray.h"
#include "SpriteComponent.h"
#include "MeshComponent.h"
#include <GL/glew.h>

Renderer::Renderer(Game* game)
	:mGame(game)
	,mSpriteShader(nullptr)
	,mPtsLightCount(0)
{
}

Renderer::~Renderer()
{
}

bool Renderer::Initialize(float screenWidth, float screenHeight)
{
	mScreenWidth = screenWidth;
	mScreenHeight = screenHeight;

	// Set OpenGL attributes
	// Use the core OpenGL profile
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	// Specify version 3.3
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	// Request a color buffer with 8-bits per RGBA channel
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	// Enable double buffering
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	// Force OpenGL to use hardware acceleration
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	mWindow = SDL_CreateWindow("Game Programming in C++ (Chapter 6)", 100, 100,
		static_cast<int>(mScreenWidth), static_cast<int>(mScreenHeight), SDL_WINDOW_OPENGL);
	if (!mWindow)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	// Create an OpenGL context
	mContext = SDL_GL_CreateContext(mWindow);

	// Initialize GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		SDL_Log("Failed to initialize GLEW.");
		return false;
	}

	// On some platforms, GLEW will emit a benign error code,
	// so clear it
	glGetError();

	// Make sure we can create/compile shaders
	if (!LoadShaders())
	{
		SDL_Log("Failed to load shaders.");
		return false;
	}

	// Create quad for drawing sprites
	CreateSpriteVerts();

	return true;
}

void Renderer::Shutdown()
{
	delete mSpriteVerts;
	mSpriteShader->Unload();
	delete mSpriteShader;
	for (auto shader : mMeshShaders)
	{
		shader.second->Unload();
		delete shader.second;
	}
	SDL_GL_DeleteContext(mContext);
	SDL_DestroyWindow(mWindow);
}

void Renderer::UnloadData()
{
	// Destroy textures
	for (auto i : mTextures)
	{
		i.second->Unload();
		delete i.second;
	}
	mTextures.clear();

	// Destroy meshes
	for (auto i : mMeshes)
	{
		i.second->Unload();
		delete i.second;
	}
	mMeshes.clear();
}

void Renderer::Draw()
{
	// Set the clear color to mPtsLights[0] grey
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	// Clear the color buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Draw mesh components
	// Enable depth buffering/disable alpha blend
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	for (auto pair : mMeshShaders)
	{
		auto meshShader = pair.second;
		// Set the mesh shader active
		meshShader->SetActive();
		// Update view-projection matrix
		meshShader->SetMatrixUniform("uViewProj", mView * mProjection);
		// Update mPtsLights uniforms
		SetLightUniforms(meshShader);
		for (auto mc : mMeshComps)
		{
			auto mesh = mc->GetMesh();
			if (mesh->GetShaderName() == pair.first)
			{
				mc->Draw(meshShader);
			}
		}
	}

	// Draw all sprite components
	// Disable depth buffering
	glDisable(GL_DEPTH_TEST);
	// Enable alpha blending on the color buffer
	glEnable(GL_BLEND);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

	// Set shader/vao as active
	mSpriteShader->SetActive();
	mSpriteVerts->SetActive();
	for (auto sprite : mSprites)
	{
		sprite->Draw(mSpriteShader);
	}

	// Swap the buffers
	SDL_GL_SwapWindow(mWindow);
}

void Renderer::AddSprite(SpriteComponent* sprite)
{
	// Find the insertion point in the sorted vector
	// (The first element with a higher draw order than me)
	int myDrawOrder = sprite->GetDrawOrder();
	auto iter = mSprites.begin();
	for (;
		iter != mSprites.end();
		++iter)
	{
		if (myDrawOrder < (*iter)->GetDrawOrder())
		{
			break;
		}
	}

	// Inserts element before position of iterator
	mSprites.insert(iter, sprite);
}

void Renderer::RemoveSprite(SpriteComponent* sprite)
{
	auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
	mSprites.erase(iter);
}

void Renderer::AddMeshComp(MeshComponent* mesh)
{
	mMeshComps.emplace_back(mesh);
}

void Renderer::RemoveMeshComp(MeshComponent* mesh)
{
	auto iter = std::find(mMeshComps.begin(), mMeshComps.end(), mesh);
	mMeshComps.erase(iter);
}

Texture* Renderer::GetTexture(const std::string& fileName)
{
	Texture* tex = nullptr;
	auto iter = mTextures.find(fileName);
	if (iter != mTextures.end())
	{
		tex = iter->second;
	}
	else
	{
		tex = new Texture();
		if (tex->Load(fileName))
		{
			mTextures.emplace(fileName, tex);
		}
		else
		{
			delete tex;
			tex = nullptr;
		}
	}
	return tex;
}

Mesh* Renderer::GetMesh(const std::string & fileName)
{
	Mesh* m = nullptr;
	auto iter = mMeshes.find(fileName);
	if (iter != mMeshes.end())
	{
		m = iter->second;
	}
	else
	{
		m = new Mesh();
		if (m->Load(fileName, this))
		{
			mMeshes.emplace(fileName, m);
		}
		else
		{
			delete m;
			m = nullptr;
		}
	}
	return m;
}

bool Renderer::LoadShaders()
{
	// スプライトシェーダの作成
	mSpriteShader = new Shader();
	if (!mSpriteShader->Load("Shaders/Sprite.vert", "Shaders/Sprite.frag"))
	{
		return false;
	}

	mSpriteShader->SetActive();
	// Set the view-projection matrix
	Matrix4 viewProj = Matrix4::CreateSimpleViewProj(mScreenWidth, mScreenHeight);
	mSpriteShader->SetMatrixUniform("uViewProj", viewProj);

	// 基本メッシュシェーダの作成
	class Shader* phongShader = new Shader();
	if (!phongShader->Load("Shaders/Phong.vert", "Shaders/Phong.frag"))
	{
		return false;
	}
	phongShader->SetActive();
	mMeshShaders.emplace("Phong", phongShader);

	class Shader* bmShader = new Shader();
	if (!bmShader->Load("Shaders/BasicMesh.vert", "Shaders/BasicMesh.frag"))
	{
		return false;
	}
	bmShader->SetActive();
	mMeshShaders.emplace("BasicMesh", bmShader);

	// Set the view-projection matrix
	mView = Matrix4::CreateLookAt(Vector3::Zero, Vector3::UnitX, Vector3::UnitZ);
	mProjection = Matrix4::CreatePerspectiveFOV(Math::ToRadians(70.0f),
		mScreenWidth, mScreenHeight, 25.0f, 10000.0f);

	return true;
}

void Renderer::CreateSpriteVerts()
{
	float vertices[] = {
		-0.5f, 0.5f, 0.f, 0.f, 0.f, 0.0f, 0.f, 0.f, // top left
		0.5f, 0.5f, 0.f, 0.f, 0.f, 0.0f, 1.f, 0.f, // top right
		0.5f,-0.5f, 0.f, 0.f, 0.f, 0.0f, 1.f, 1.f, // bottom right
		-0.5f,-0.5f, 0.f, 0.f, 0.f, 0.0f, 0.f, 1.f  // bottom left
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	mSpriteVerts = new VertexArray(vertices, 4, indices, 6);
}

void Renderer::SetLightUniforms(Shader* shader)
{
	// Camera position is from inverted view
	Matrix4 invView = mView;
	invView.Invert();
	shader->SetVectorUniform("uCameraPos", invView.GetTranslation());
	// Ambient mPtsLights[0]
	shader->SetVectorUniform("uAmbientLight", mAmbientLight);
	// Directional mPtsLights[0]
	shader->SetVectorUniform("uDirLight.mDirection",
		mDirLight.mDirection);
	shader->SetVectorUniform("uDirLight.mDiffuseColor",
		mDirLight.mDiffuseColor);
	shader->SetVectorUniform("uDirLight.mSpecColor",
		mDirLight.mSpecColor);

	shader->SetVectorUniform("uPtsLights[0].mPosition", mPtsLights[0].mPosition);
	shader->SetVectorUniform("uPtsLights[0].mDiffuseColor", mPtsLights[0].mDiffuseColor);
	shader->SetVectorUniform("uPtsLights[0].mSpecColor", mPtsLights[0].mSpecColor);
	shader->SetFloatUniform("uPtsLights[0].mSpecPower", mPtsLights[0].mSpecPower);
	shader->SetFloatUniform("uPtsLights[0].mInfluRadius", mPtsLights[0].mInfluRadius);

	shader->SetVectorUniform("uPtsLights[1].mPosition", mPtsLights[1].mPosition);
	shader->SetVectorUniform("uPtsLights[1].mDiffuseColor", mPtsLights[1].mDiffuseColor);
	shader->SetVectorUniform("uPtsLights[1].mSpecColor", mPtsLights[1].mSpecColor);
	shader->SetFloatUniform("uPtsLights[1].mSpecPower", mPtsLights[1].mSpecPower);
	shader->SetFloatUniform("uPtsLights[1].mInfluRadius", mPtsLights[1].mInfluRadius);

	shader->SetVectorUniform("uPtsLights[2].mPosition", mPtsLights[2].mPosition);
	shader->SetVectorUniform("uPtsLights[2].mDiffuseColor", mPtsLights[2].mDiffuseColor);
	shader->SetVectorUniform("uPtsLights[2].mSpecColor", mPtsLights[2].mSpecColor);
	shader->SetFloatUniform("uPtsLights[2].mSpecPower", mPtsLights[2].mSpecPower);
	shader->SetFloatUniform("uPtsLights[2].mInfluRadius", mPtsLights[2].mInfluRadius);

	shader->SetVectorUniform("uPtsLights[3].mPosition", mPtsLights[3].mPosition);
	shader->SetVectorUniform("uPtsLights[3].mDiffuseColor", mPtsLights[3].mDiffuseColor);
	shader->SetVectorUniform("uPtsLights[3].mSpecColor", mPtsLights[3].mSpecColor);
	shader->SetFloatUniform("uPtsLights[3].mSpecPower", mPtsLights[3].mSpecPower);
	shader->SetFloatUniform("uPtsLights[3].mInfluRadius", mPtsLights[3].mInfluRadius);
}

void Renderer::SetPointLight(PointLight light)
{
	if (mPtsLightCount < 4)
	{
		mPtsLights[mPtsLightCount].mPosition = light.mPosition;
		mPtsLights[mPtsLightCount].mDiffuseColor = light.mDiffuseColor;
		mPtsLights[mPtsLightCount].mSpecColor = light.mSpecColor;
		mPtsLights[mPtsLightCount].mSpecPower = light.mSpecPower;
		mPtsLights[mPtsLightCount].mInfluRadius = light.mInfluRadius;
		mPtsLightCount++;
	}
}
