//=====================================
//����ҁF���X�ؗ��M
//���t�F2016/5/7
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
//�`��
//=====================================
void SubRoutine::Render()
{
	//�E�B���h�E��h��Ԃ�(�J���[�o�b�t�@�A�f�v�X�o�b�t�@�w��)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//�����̈ʒu
	vec4 lightPos = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	prog->setUniform("light.position", lightPos);

	GLuint programHandle = prog->getHandle();

	//�e�T�u���[�`���֐��̃C���f�b�N�X��₢���킹
	GLuint adsIndex = glGetSubroutineIndex(programHandle,
		GL_VERTEX_SHADER,
		"PhongModel");
	GLuint diffuseIndex = glGetSubroutineIndex(programHandle,
		GL_VERTEX_SHADER,
		"DiffuseOnlyModel");

	//�K�؂ȃT�u���[�`���֐��̑I�������邽�߂ɃT�u���[�`�����j�t�H�[���ϐ��ɒl�ݒ�
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
//�V�F�[�_�[������
//=====================================
void SubRoutine::InitShader()
{
	prog = new GLSLProgram();
	CompileAndLinkShader();

	glEnable(GL_DEPTH_TEST);

	teapot = new VBOTeapot(13, mat4(1.0f));

	//�r���[
	view = glm::lookAt(vec3(0.0f, 0.0f, 10.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
	//�v���W�F�N�V����
	projection = mat4(1.0f);

	//���j�t�H�[���ϐ��ɒl�ݒ�
	prog->setUniform("material.kd", 0.9f, 0.5f, 0.3f);	//�f�B�t���[�Y	���˗�
	prog->setUniform("light.ld", 1.0f, 1.0f, 1.0f);		//�f�B�t���[�Y	���C�g���x
	prog->setUniform("material.ka", 0.9f, 0.5f, 0.3f);	//�A���r�G���g�@���˗�
	prog->setUniform("light.la", 0.4f, 0.4f, 0.4f);		//�A���r�G���g�@���C�g���x
	prog->setUniform("material.ks", 0.8f, 0.8f, 0.8f);	//�X�y�L����	���˗�
	prog->setUniform("light.ls", 1.0f, 1.0f, 1.0f);		//�X�y�L����	���C�g���x
	prog->setUniform("material.shininess", 100.0f);	//�X�y�L�����̋P���W��(�l���傫���Ȃ�ɘA��ăn�C���C�g�͏������Ȃ�)
}

//=====================================
//�}�e���A���ݒ�
//=====================================
void SubRoutine::SetMatrices()
{
	mat4 mv = view * model;
	prog->setUniform("modelViewMatrix", mv);	//���f���r���[�s��
	prog->setUniform("normalMatrix", mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));	//���f���r���[�s��(vvec3�Ƃ̌v�Z�̂��߁A3 * 3)
	prog->setUniform("MVP", projection * mv);	//���e  * ���f���r���[�s��
}

//=====================================
//�E�B���h�E�̊g��k���ɑ΂��鏈��
//=====================================
void SubRoutine::Resize(int w, int h)
{
	glViewport(0, 0, w, h);
	width = w;
	height = h;
	//�v���W�F�N�V�����ݒ�
	projection = glm::perspective(glm::radians(70.0f), (float)w / h, 0.3f, 100.0f);
}

//=====================================
//�v���O�����I�u�W�F�N�g�쐬
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

