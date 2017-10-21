//=====================================
//制作者：佐々木隆貴
//日付：2016/5/7
//=====================================
#ifndef SHADER_H
#define SHADER_H

#include <glm/glm/glm.hpp>
using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;
using glm::mat3;

class Shader
{
public:
	virtual void Render() = 0;
	virtual void InitShader() = 0;
	virtual void Resize(int w, int h) = 0;
	virtual void SetMatrices() = 0;
	virtual void CompileAndLinkShader() = 0;
};

#endif