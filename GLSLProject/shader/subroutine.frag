#version 430
 
 //���͑���
in vec3 lightIntensity;

layout (location=0) out vec4 flagColor;	//�o�͕ϐ�

void main()
{
	flagColor = vec4(lightIntensity, 1.0);
}													  
