#version 430
 
 //入力属性
in vec3 frontColor;
in vec3 backColor;

layout (location=0) out vec4 flagColor;	//出力変数

void main()
{
	//フラグメントが前向きまたは後ろ向きのポリゴン
	if(gl_FrontFacing)
	{
		flagColor = vec4(frontColor, 1.0);
	} else {
		flagColor = vec4(backColor, 1.0);
	}
		
}													  
