#version 430
 
 //入力属性
in vec3 lightIntensity;

layout (location=0) out vec4 flagColor;	//出力変数

void main()
{
	flagColor = vec4(lightIntensity, 1.0);
}													  
