//=====================================
//制作者：佐々木隆貴
//日付：2016/5/6
//=====================================
//include
#include "PhongShader.h"
#include <iostream>
#include <GL/glew.h>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtx/transform.hpp>
#include "vboteapot.h"
#include "glslprogram.h"

using namespace std;

//link
#pragma comment (lib, "glew32.lib")

PhongShader::~PhongShader()
{
	delete teapot;
	delete prog;
	teapot = 0;
	prog = 0;
}

//=====================================
//描画
//=====================================
void PhongShader::Render()
{
	//ウィンドウを塗りつぶす(カラーバッファ、デプスバッファ指定)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//モデルの姿勢
	model = mat4(1.0f);
	model *= glm::translate(vec3(0.0, -1.0, 0.0));
	model *= glm::rotate(glm::radians(-90.0f), vec3(1.0f, 0.0f, 0.0f));

	SetMatrices();
	teapot->render();
}

//=====================================
//シェーダー初期化
//=====================================
void PhongShader::InitShader()
{
	prog = new GLSLProgram();
	CompileAndLinkShader();

	glEnable(GL_DEPTH_TEST);

	mat4 transform = glm::translate(mat4(1.0f), vec3(0.0f, 1.5f, 0.25f));
	teapot = new VBOTeapot(13, transform);
	
	//ビュー
	view = glm::lookAt(vec3(2.0f, 4.0f, 2.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
	//プロジェクション
	projection = mat4(1.0f);
	//ワールド座標のライト位置
	vec4 worldLight = vec4(5.0f, 5.0f, 2.0f, 1.0f);

	//ユニフォーム変数に値設定
	prog->setUniform("material.kd", 0.9f, 0.5f, 0.3f);	//ディフューズ	反射率
	prog->setUniform("light.ld", 1.0f, 1.0f, 1.0f);		//ディフューズ	ライト強度
	prog->setUniform("light.position", view * worldLight);	//ビューに対するライト位置
	prog->setUniform("material.ka", 0.9f, 0.5f, 0.3f);	//アンビエント　反射率
	prog->setUniform("light.la", 0.4f, 0.4f, 0.4f);		//アンビエント　ライト強度
	prog->setUniform("material.ks", 0.8f, 0.8f, 0.8f);	//スペキュラ	反射率
	prog->setUniform("light.ls", 1.0f, 1.0f, 1.0f);		//スペキュラ	ライト強度
	prog->setUniform("material.shininess", 100.0f);	//スペキュラの輝き係数(値が大きくなるに連れてハイライトは小さくなる)
}

//=====================================
//マテリアル設定
//=====================================
void PhongShader::SetMatrices()
{
	mat4 mv = view * model;
	prog->setUniform("modelViewMatrix", mv);	//モデルビュー行列
	prog->setUniform("normalMatrix", mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));	//モデルビュー行列(vvec3との計算のため、3 * 3)
	prog->setUniform("MVP", projection * mv);	//投影  * モデルビュー行列
}

//=====================================
//ウィンドウの拡大縮小に対する処理
//=====================================
void PhongShader::Resize(int w, int h)
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
void PhongShader::CompileAndLinkShader()
{
	try {
		prog->compileShader("shader/phong.vert");
		prog->compileShader("shader/phong.frag");
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
