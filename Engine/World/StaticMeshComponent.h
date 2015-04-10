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

private:
	StaticMesh* Mesh;
	ShaderProgram* Shader;
};