#version 430
 
in vec3 lightIntensity;	//入力属性

layout (location=0) out vec4 flagColor;	//出力変数

void main()
{
	flagColor = vec4(lightIntensity, 1.0);	
}													  
