//=====================================
//����ҁF���X�ؗ��M
//���t�F2016/5/6
//=====================================
#ifndef PHONG_SHADER_H
#define PHONG_SHADER_H

#include "Shader.h"

class VBOTeapot;
class GLSLProgram;

class PhongShader :
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
	PhongShader() : angle(0.0f){ ; }
	~PhongShader();
	void Render();
	void InitShader();
	void Resize(int w, int h);

private:
	void SetMatrices();
	void CompileAndLinkShader();
};

#endif