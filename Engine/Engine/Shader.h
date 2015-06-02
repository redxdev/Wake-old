#pragma once

#include <gl/glew.h>
#include <glm/fwd.hpp>

#include "../Logging/LogMacros.h"

/**
 * Utility to encapsulate a OpenGL uniform.
 */
class Uniform
{
public:
	Uniform(GLuint Program, GLint Location);

	Uniform(const Uniform& Other);

	GLuint GetProgram();
	GLuint GetLocation();

	void Set1f(float x);
	void Set2f(float x, float y);
	void Set3f(float x, float y, float z);
	void Set4f(float x, float y, float z, float w);

	void Set1i(int x);
	void Set2i(int x, int y);
	void Set3i(int x, int y, int z);
	void Set4i(int x, int y, int z, int w);

	void Set1ui(unsigned x);
	void Set2ui(unsigned x, unsigned y);
	void Set3ui(unsigned x, unsigned y, unsigned z);
	void Set4ui(unsigned x, unsigned y, unsigned z, unsigned int w);

	void SetVec2(const glm::vec2& xy);
	void SetVec3(const glm::vec3& xyz);
	void SetVec4(const glm::vec4& xyzw);

	void SetMatrix2(const glm::mat2x2& m22);
	void SetMatrix2x3(const glm::mat2x3& m23);
	void SetMatrix2x4(const glm::mat2x4& m24);

	void SetMatrix3x2(const glm::mat3x2& m32);
	void SetMatrix3(const glm::mat3x3& m33);
	void SetMatrix3x4(const glm::mat3x4& m34);

	void SetMatrix4x2(const glm::mat4x2& m42);
	void SetMatrix4x3(const glm::mat4x3& m43);
	void SetMatrix4(const glm::mat4x4& m44);

private:
	GLuint Program;
	GLint Location;
};

/**
 * Utility to encapsulate an OpenGL shader program.
 */
class ShaderProgram
{
	CLOG_LOGGER_DECL;
public:
	static GLuint LoadFile(const char* Path, GLenum ShaderType);

	static GLuint LoadString(const char* Str, GLenum ShaderType);

	static GLuint LoadProgram(GLuint VertexShader, GLuint FragmentShader);

public:
	ShaderProgram(GLuint Program);
	~ShaderProgram();

	GLuint GetProgram();

	Uniform GetUniform(const char* Name);

	void Use();

private:
	GLuint Program;
};