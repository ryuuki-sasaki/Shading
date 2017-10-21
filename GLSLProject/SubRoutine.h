//=====================================
//制作者：佐々木隆貴
//日付：2016/5/7
//=====================================
#ifndef SUB_ROUTINE_H
#define SUB_ROUTINE_H

#include "Shader.h"

class VBOTeapot;
class GLSLProgram;

class SubRoutine :
	public Shader
{
public:
	GLSLProgram* prog;
	VBOTeapot *teapot;
	float angle;

	mat4 model;
	mat4 view;
	mat4 projection;

	int width, height;

public:
	SubRoutine() : angle(0.0f){ ; }
	~SubRoutine();
	void Render();
	void InitShader();
	void Resize(int w, int h);

private:
	void SetMatrices();
	void CompileAndLinkShader();
};

#endif

