#version 430
 
 //���͑���
in vec3 frontColor;
in vec3 backColor;

layout (location=0) out vec4 flagColor;	//�o�͕ϐ�

void main()
{
	//�t���O�����g���O�����܂��͌������̃|���S��
	if(gl_FrontFacing)
	{
		flagColor = vec4(frontColor, 1.0);
	} else {
		flagColor = vec4(backColor, 1.0);
	}
		
}													  
