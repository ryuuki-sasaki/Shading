//=====================================
//制作者：佐々木隆貴
//日付：2016/5/6
//=====================================
//include
#include "DiffuseShader.h"
#include <iostream>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtx/transform.hpp>
#include "vboteapot.h"
#include "glslprogram.h"

using namespace std;

//link
#pragma comment (lib, "glew32.lib")

DiffuseShader::~DiffuseShader()
{
	delete teapot;
	delete prog;
	teapot = 0;
	prog = 0;
}

//=====================================
//描画
//=====================================
void DiffuseShader::Render()
{
	//ウィンドウを塗りつぶす(カラーバッファ、デプスバッファ指定)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	SetMatrices();
	teapot->render();
}

//=====================================
//シェーダー初期化
//=====================================
void DiffuseShader::InitShader()
{
	prog = new GLSLProgram();
	CompileAndLinkShader();

	glEnable(GL_DEPTH_TEST);

	teapot = new VBOTeapot(13, mat4(1.0f));
	
	//モデルの姿勢
	model = mat4(1.0f);
	model *= glm::translate(vec3(0.0, -1.0, 0.0));
	model *= glm::rotate(glm::radians(-90.0f), vec3(1.0f, 0.0f, 0.0f));
	//ビュー
	view = glm::lookAt(vec3(2.0f, 4.0f, 2.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
	//プロジェクション
	projection = mat4(1.0f);

	prog->setUniform("kd", 0.9f, 0.5f, 0.3f);	//ディフューズ	反射率
	prog->setUniform("ld", 1.0f, 1.0f, 1.0f);	//ディフューズ	ライト強度
	prog->setUniform("lightPosition", view * vec4(5.0f, 5.0f, 2.0f, 1.0f));	//ビューに対するライト位置
}

//=====================================
//マテリアル設定
//=====================================
void DiffuseShader::SetMatrices()
{
	mat4 mv = view * model;
	prog->setUniform("modelViewMatrix", mv);	//モデルビュー行列
	prog->setUniform("normalMatrix", mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));	//モデルビュー行列(vvec3との計算のため、3 * 3)
	prog->setUniform("MVP", projection * mv);	//投影  * モデルビュー行列
}

//=====================================
//ウィンドウの拡大縮小に対する処理
//=====================================
void DiffuseShader::Resize(int w, int h)
{
	glViewport(0, 0, w, h);
	width = w;
	height = h;
	//プロジェクション設定
	projection = glm::perspective(glm::radians(70.0f), (float)w / h, 0.3f, 100.0f);
}

//=====================================
//プログラムオブジェクト作成
//=====================================
void DiffuseShader::CompileAndLinkShader()
{
	try {
		prog->compileShader("shader/diffuse.vert");
		prog->compileShader("shader/diffuse.frag");
		prog->link();
		prog->use();
		prog->printActiveUniforms();
		prog->printActiveUniformBlocks();
		prog->printActiveAttribs();
	}
	catch (GLSLProgramException &e) {
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}
}

