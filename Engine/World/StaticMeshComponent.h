#pragma once

#include "Component.h"
#include "../Engine/Mesh.h"
#include "../Engine/Shader.h"

class StaticMeshComponent : public Component
{
public:
	StaticMeshComponent(Actor* Parent, bool StartActive, StaticMesh* Mesh, ShaderProgram* Shader);

	StaticMesh* GetMesh() const;
	ShaderProgram* GetShader() const;

	void SetMesh(StaticMesh* Mesh);
	void SetShader(ShaderProgram* Shader);

	virtual void Draw() override;

	void SetR(float val) {R = val;}
	void SetG(float val) {G = val;}
	void SetB(float val) {B = val;}

private:
	StaticMesh* Mesh;
	ShaderProgram* Shader;

	float R = 0;
	float G = 0;
	float B = 0;
};