#version 430
 
//���͕ϐ�
layout (location=0) in vec3 vertexPosition;	
layout (location=1) in vec3 vertexNormal;

//�o�͕ϐ�
out vec3 lightIntensity;	

//���j�t�H�[���ϐ�
uniform vec4 lightPosition;		//���_���W�̃��C�g�ʒu
uniform vec3 kd;	//�f�B�t���[�Y���ˌ�
uniform vec3 ld;	//�����̋��x

uniform mat4 modelViewMatrix;
uniform mat3 normalMatrix;
uniform mat4 projectionMatrix;
uniform mat4 MVP;	//���e  * ���f���r���[

void main()
{
	//�@���ƈʒu�����_���W�ɕϊ�
	vec3 tnorm = normalize(normalMatrix * vertexNormal);	//�@�������_(�J����)���W�ɕϊ�
	vec4 eyeCoords = modelViewMatrix * vec4(vertexPosition, 1.0);	//���_�ʒu�����_(�J����)���W�ɕϊ�
	vec3 s = normalize(vec3(lightPosition - eyeCoords));	//�����ɑ΂��鐳�K������

	//�f�B�t���[�Y�V�F�[�f�B���O�������Ō��̋��x�����߂�
	lightIntensity = ld * kd * max(dot(s, tnorm), 0.0);	//���K�����ꂽ�x�N�g���̓��ς��Ƃ�(dot��0�ȉ�(�p�x90�x�𒴂���)�̒l��l���Ƃ�Ȃ��悤�ɂ���)
	//�ʒu���N���b�v���W�ɕϊ����Č�ɓn��
	gl_Position = MVP * vec4(vertexPosition, 1.0);
}
