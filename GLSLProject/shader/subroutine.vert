#version 430

//�T�u���[�`���^��`
subroutine vec3 ShadeModelType( vec4 position, vec3 norm );
subroutine uniform ShadeModelType shadeModel;	//�֐��|�C���^�Ƃ��Ďg�p���郆�j�t�H�[���ϐ�
 
//���͕ϐ�
layout (location=0) in vec3 vertexPosition;	
layout (location=1) in vec3 vertexNormal;

//�o�͕ϐ�
out vec3 lightIntensity;

struct lightInfo
{
	vec4 position;	//���_���W�ł̃��C�g�ʒu
	vec3 la;		//�A���r�G���g�@���C�g���x
	vec3 ld;		//�f�B�t���[�Y	���C�g���x
	vec3 ls;		//�X�y�L����	���C�g���x
};
uniform lightInfo light;

struct materialInfo
{
	vec3 ka;		//�A���r�G���g�@���˗�
	vec3 kd;		//�f�B�t���[�Y	���˗�
	vec3 ks;		//�X�y�L����	���˗�
	float shininess;	//�X�y�L�����P���W��
};
uniform materialInfo material;

//���j�t�H�[���ϐ�
uniform mat4 modelViewMatrix;
uniform mat3 normalMatrix;
uniform mat4 projectionMatrix;
uniform mat4 MVP;	//���e  * ���f���r���[

//=====================================
//�@���ƈʒu�����_���W�ɕϊ�
//=====================================
void GetEyeSpace( out vec3 norm, out vec4 position )
{
	norm = normalize(normalMatrix * vertexNormal);	//�@�������_(�J����)���W�ɕϊ�
	position = modelViewMatrix * vec4(vertexPosition, 1.0);	//���_�ʒu�����_(�J����)���W�ɕϊ�
}

//=====================================
//�t�H���V�F�[�f�B���O
//=====================================
subroutine( ShadeModelType )
vec3 PhongModel( vec4 position, vec3 norm )
{
	vec3 s = normalize(vec3(light.position - position));	//�����ɑ΂��鐳�K������
	vec3 v = normalize(-position.xyz);	//���_�ɑ΂������
	vec3 r = reflect(-s, norm);	//��������̓��˃x�N�g���ɑ΂��锽�˃x�N�g��

	vec3 ambient = light.la * material.ka;	//�A���r�G���g�����v�Z

	float sDotN = max(dot(s, norm), 0.0);	//���K�����ꂽ�x�N�g���̓��ς��Ƃ�(dot��0�ȉ�(�p�x90�x�𒴂���)�̒l��l���Ƃ�Ȃ��悤�ɂ���)
	vec3 diffuse = light.ld * material.kd * sDotN;	//�f�B�t���[�Y�����v�Z

	vec3 spec = vec3(0.0);

	//�T�[�t�F�C�X�ɓ��B����������邩
	if(sDotN > 0)
	{
		spec = light.ls * material.ks * pow(max(dot(r, v), 0.0), material.shininess);	//�X�y�L���������v�Z
	}

	//ADS�V�F�[�f�B���O�������Ō��̋��x�����߂�
	return ambient + diffuse + spec;
}

//=====================================
//�f�B�t���[�Y�V�F�[�f�B���O
//=====================================
subroutine( ShadeModelType )
vec3 DiffuseOnlyModel( vec4 position, vec3 norm )
{
	vec3 s = normalize(vec3(light.position - position));	//�����ɑ΂��鐳�K������
	return light.ld * material.kd *  max(dot(s, norm), 0.0);	//�f�B�t���[�Y�����v�Z
}

void main()
{
	vec3 eyeNorm;
	vec4 eyePosition;

	//���_��Ԃ̈ʒu�Ɩ@�����擾
	GetEyeSpace(eyeNorm, eyePosition);

	//���C�e�B���O��������]��
	lightIntensity = shadeModel(eyePosition, eyeNorm);	//shadeModel�ɉ����ăT�u���[�`���֐��̂����ꂩ�Ƀo�C���h

	//�ʒu���N���b�v���W�ɕϊ����Č�ɓn��
	gl_Position = MVP * vec4(vertexPosition, 1.0);
}