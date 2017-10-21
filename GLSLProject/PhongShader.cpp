//=====================================
//����ҁF���X�ؗ��M
//���t�F2016/5/6
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
//�`��
//=====================================
void PhongShader::Render()
{
	//�E�B���h�E��h��Ԃ�(�J���[�o�b�t�@�A�f�v�X�o�b�t�@�w��)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//���f���̎p��
	model = mat4(1.0f);
	model *= glm::translate(vec3(0.0, -1.0, 0.0));
	model *= glm::rotate(glm::radians(-90.0f), vec3(1.0f, 0.0f, 0.0f));

	SetMatrices();
	teapot->render();
}

//=====================================
//�V�F�[�_�[������
//=====================================
void PhongShader::InitShader()
{
	prog = new GLSLProgram();
	CompileAndLinkShader();

	glEnable(GL_DEPTH_TEST);

	mat4 transform = glm::translate(mat4(1.0f), vec3(0.0f, 1.5f, 0.25f));
	teapot = new VBOTeapot(13, transform);
	
	//�r���[
	view = glm::lookAt(vec3(2.0f, 4.0f, 2.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
	//�v���W�F�N�V����
	projection = mat4(1.0f);
	//���[���h���W�̃��C�g�ʒu
	vec4 worldLight = vec4(5.0f, 5.0f, 2.0f, 1.0f);

	//���j�t�H�[���ϐ��ɒl�ݒ�
	prog->setUniform("material.kd", 0.9f, 0.5f, 0.3f);	//�f�B�t���[�Y	���˗�
	prog->setUniform("light.ld", 1.0f, 1.0f, 1.0f);		//�f�B�t���[�Y	���C�g���x
	prog->setUniform("light.position", view * worldLight);	//�r���[�ɑ΂��郉�C�g�ʒu
	prog->setUniform("material.ka", 0.9f, 0.5f, 0.3f);	//�A���r�G���g�@���˗�
	prog->setUniform("light.la", 0.4f, 0.4f, 0.4f);		//�A���r�G���g�@���C�g���x
	prog->setUniform("material.ks", 0.8f, 0.8f, 0.8f);	//�X�y�L����	���˗�
	prog->setUniform("light.ls", 1.0f, 1.0f, 1.0f);		//�X�y�L����	���C�g���x
	prog->setUniform("material.shininess", 100.0f);	//�X�y�L�����̋P���W��(�l���傫���Ȃ�ɘA��ăn�C���C�g�͏������Ȃ�)
}

//=====================================
//�}�e���A���ݒ�
//=====================================
void PhongShader::SetMatrices()
{
	mat4 mv = view * model;
	prog->setUniform("modelViewMatrix", mv);	//���f���r���[�s��
	prog->setUniform("normalMatrix", mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));	//���f���r���[�s��(vvec3�Ƃ̌v�Z�̂��߁A3 * 3)
	prog->setUniform("MVP", projection * mv);	//���e  * ���f���r���[�s��
}

//=====================================
//�E�B���h�E�̊g��k���ɑ΂��鏈��
//=====================================
void PhongShader::Resize(int w, int h)
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
