#version 430
 
//入力変数
layout (location=0) in vec3 vertexPosition;	
layout (location=1) in vec3 vertexNormal;

//出力変数
out vec3 lightIntensity;	

//ユニフォーム変数
uniform vec4 lightPosition;		//視点座標のライト位置
uniform vec3 kd;	//ディフューズ反射光
uniform vec3 ld;	//光源の強度

uniform mat4 modelViewMatrix;
uniform mat3 normalMatrix;
uniform mat4 projectionMatrix;
uniform mat4 MVP;	//投影  * モデルビュー

void main()
{
	//法線と位置を視点座標に変換
	vec3 tnorm = normalize(normalMatrix * vertexNormal);	//法線を視点(カメラ)座標に変換
	vec4 eyeCoords = modelViewMatrix * vec4(vertexPosition, 1.0);	//頂点位置を視点(カメラ)座標に変換
	vec3 s = normalize(vec3(lightPosition - eyeCoords));	//光源に対する正規化方向

	//ディフューズシェーディング方程式で光の強度を求める
	lightIntensity = ld * kd * max(dot(s, tnorm), 0.0);	//正規化されたベクトルの内積をとる(dotで0以下(角度90度を超える)の値を値をとらないようにする)
	//位置をクリップ座標に変換して後に渡す
	gl_Position = MVP * vec4(vertexPosition, 1.0);
}
