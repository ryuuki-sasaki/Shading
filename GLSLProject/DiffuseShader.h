//=====================================
//制作者：佐々木隆貴
//日付：2016/5/6
//=====================================
#ifndef DIFFUSE_SHADER_H
#define DIFFUSE_SHADER_H

#include "Shader.h"

class VBOTeapot;
class GLSLProgram;

class DiffuseShader :
	public Shader
{
private:
	GLSLProgram* prog;
	VBOTeapot *teapot;
	
	mat4 model;
	mat4 view;
	mat4 projection;
	
	int width, height;

public:
	DiffuseShader(){ ; };
	~DiffuseShader();
	void Render();
	void InitShader();
	void Resize(int w, int h);

private:
	void SetMatrices();
	void CompileAndLinkShader();
};

#endif