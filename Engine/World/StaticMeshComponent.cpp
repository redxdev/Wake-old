#include "StaticMeshComponent.h"

#include "Actor.h"

StaticMeshComponent::StaticMeshComponent(Actor* Parent, bool StartActive, StaticMesh* Mesh, ShaderProgram* Shader)
	: Component(Parent, StartActive)
{
	this->Mesh = Mesh;
	this->Shader = Shader;
}

StaticMesh* StaticMeshComponent::GetMesh() const
{
	return Mesh;
}

ShaderProgram* StaticMeshComponent::GetShader() const
{
	return Shader;
}

void StaticMeshComponent::SetMesh(StaticMesh* Mesh)
{
	this->Mesh = Mesh;
}

void StaticMeshComponent::SetShader(ShaderProgram* Shader)
{
	this->Shader = Shader;
}

void StaticMeshComponent::Draw()
{
	if (Shader == nullptr || Mesh == nullptr)
		return;

	Shader->Use();
	Shader->GetUniform("modelMatrix").Set(GetParent()->CreateMatrix());
	Shader->GetUniform("viewMatrix").Set(glm::mat4());
	Shader->GetUniform("projectionMatrix").Set(glm::mat4());

	Mesh->Draw();
}
