//=====================================
//制作者：佐々木隆貴
//日付：2016/5/6
//=====================================
#ifndef FLAT_SHADER_H
#define FLAT_SHADER_H

#include "Shader.h"

class VBOTeapot;
class GLSLProgram;

class FlatShader :
	public Shader
{
private:
	GLSLProgram* prog;
	VBOTeapot *teapot;
	float angle;

	mat4 model;
	mat4 view;
	mat4 projection;

	int width, height;

public:
	FlatShader() : angle(0.0f){ ; }
	~FlatShader();
	void Render();
	void InitShader();
	void Resize(int w, int h);

private:
	void SetMatrices();
	void CompileAndLinkShader();
};

#endif