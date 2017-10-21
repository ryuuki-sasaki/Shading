#version 430
 
in vec3 lightIntensity;	//“ü—Í‘®«

layout (location=0) out vec4 flagColor;	//o—Í•Ï”

void main()
{
	flagColor = vec4(lightIntensity, 1.0);	
}													  
