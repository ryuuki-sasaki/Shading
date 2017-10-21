//=====================================
//制作者：佐々木隆貴
//日付：2016/5/7
//=====================================
//include
#include "SubRoutine.h"
#include <iostream>
#include <GL/glew.h>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtx/transform.hpp>
#include "vboteapot.h"
#include "glslprogram.h"

using namespace std;

//link
#pragma comment (lib, "glew32.lib")

SubRoutine::~SubRoutine()
{
	delete teapot;
	delete prog;
	teapot = 0;
	prog = 0;
}

//=====================================
//描画
//=====================================
void SubRoutine::Render()
{
	//ウィンドウを塗りつぶす(カラーバッファ、デプスバッファ指定)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//光源の位置
	vec4 lightPos = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	prog->setUniform("light.position", lightPos);

	GLuint programHandle = prog->getHandle();

	//各サブルーチン関数のインデックスを問い合わせ
	GLuint adsIndex = glGetSubroutineIndex(programHandle,
		GL_VERTEX_SHADER,
		"PhongModel");
	GLuint diffuseIndex = glGetSubroutineIndex(programHandle,
		GL_VERTEX_SHADER,
		"DiffuseOnlyModel");

	//適切なサブルーチン関数の選択をするためにサブルーチンユニフォーム変数に値設定
	glUniformSubroutinesuiv(GL_VERTEX_SHADER, 1, &adsIndex);
	model = mat4(1.0f);
	model *= glm::translate(vec3(-3.0, -1.5, 0.0));
	model *= glm::rotate(glm::radians(-90.0f), vec3(1.0f, 0.0f, 0.0f));
	SetMatrices();
	teapot->render();

	glUniformSubroutinesuiv(GL_VERTEX_SHADER, 1, &diffuseIndex);
	model = mat4(1.0f);
	model *= glm::translate(vec3(3.0f, -1.5f, 0.0f));
	model *= glm::rotate(glm::radians(-90.0f), vec3(1.0f, 0.0f, 0.0f));
	SetMatrices();
	teapot->render();
}

//=====================================
//シェーダー初期化
//=====================================
void SubRoutine::InitShader()
{
	prog = new GLSLProgram();
	CompileAndLinkShader();

	glEnable(GL_DEPTH_TEST);

	teapot = new VBOTeapot(13, mat4(1.0f));

	//ビュー
	view = glm::lookAt(vec3(0.0f, 0.0f, 10.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
	//プロジェクション
	projection = mat4(1.0f);

	//ユニフォーム変数に値設定
	prog->setUniform("material.kd", 0.9f, 0.5f, 0.3f);	//ディフューズ	反射率
	prog->setUniform("light.ld", 1.0f, 1.0f, 1.0f);		//ディフューズ	ライト強度
	prog->setUniform("material.ka", 0.9f, 0.5f, 0.3f);	//アンビエント　反射率
	prog->setUniform("light.la", 0.4f, 0.4f, 0.4f);		//アンビエント　ライト強度
	prog->setUniform("material.ks", 0.8f, 0.8f, 0.8f);	//スペキュラ	反射率
	prog->setUniform("light.ls", 1.0f, 1.0f, 1.0f);		//スペキュラ	ライト強度
	prog->setUniform("material.shininess", 100.0f);	//スペキュラの輝き係数(値が大きくなるに連れてハイライトは小さくなる)
}

//=====================================
//マテリアル設定
//=====================================
void SubRoutine::SetMatrices()
{
	mat4 mv = view * model;
	prog->setUniform("modelViewMatrix", mv);	//モデルビュー行列
	prog->setUniform("normalMatrix", mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));	//モデルビュー行列(vvec3との計算のため、3 * 3)
	prog->setUniform("MVP", projection * mv);	//投影  * モデルビュー行列
}

//=====================================
//ウィンドウの拡大縮小に対する処理
//=====================================
void SubRoutine::Resize(int w, int h)
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
void SubRoutine::CompileAndLinkShader()
{
	try {
		prog->compileShader("shader/subroutine.vert");
		prog->compileShader("shader/subroutine.frag");
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

