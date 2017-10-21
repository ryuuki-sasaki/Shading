#version 430
 
//入力変数
layout (location=0) in vec3 vertexPosition;	
layout (location=1) in vec3 vertexNormal;

//出力変数
out vec3 frontColor;
out vec3 backColor;

struct lightInfo
{
	vec4 position;	//頂点座標でのライト位置
	vec3 la;		//アンビエント　ライト強度
	vec3 ld;		//ディフューズ	ライト強度
	vec3 ls;		//スペキュラ	ライト強度
};
uniform lightInfo light;

struct materialInfo
{
	vec3 ka;		//アンビエント　反射率
	vec3 kd;		//ディフューズ	反射率
	vec3 ks;		//スペキュラ	反射率
	float shininess;	//スペキュラ輝き係数
};
uniform materialInfo material;

//ユニフォーム変数
uniform mat4 modelViewMatrix;
uniform mat3 normalMatrix;
uniform mat4 projectionMatrix;
uniform mat4 MVP;	//投影  * モデルビュー

//=====================================
//法線と位置を視点座標に変換
//=====================================
void GetEyeSpace( out vec3 norm, out vec4 position )
{
	norm = normalize(normalMatrix * vertexNormal);	//法線を視点(カメラ)座標に変換
	position = modelViewMatrix * vec4(vertexPosition, 1.0);	//頂点位置を視点(カメラ)座標に変換
}

//=====================================
//光の強度を求める
//=====================================
vec3 PhongModel( vec4 position, vec3 norm )
{
	vec3 s = normalize(vec3(light.position - position));	//光源に対する正規化方向
	vec3 v = normalize(-position.xyz);	//視点に対する方向
	vec3 r = reflect(-s, norm);	//光源からの入射ベクトルに対する反射ベクトル

	vec3 ambient = light.la * material.ka;	//アンビエント成分計算

	float sDotN = max(dot(s, norm), 0.0);	//正規化されたベクトルの内積をとる(dotで0以下(角度90度を超える)の値を値をとらないようにする)
	vec3 diffuse = light.ld * material.kd * sDotN;	//ディフューズ成分計算

	vec3 spec = vec3(0.0);

	//サーフェイスに到達する光があるか
	if(sDotN > 0)
	{
		spec = light.ls * material.ks * pow(max(dot(r, v), 0.0), material.shininess);	//スペキュラ成分計算
	}

	//ADSシェーディング方程式で光の強度を求める
	return ambient + diffuse + spec;
}

void main()
{
	vec3 eyeNorm;
	vec4 eyePosition;

	//視点空間の位置と法線を取得
	GetEyeSpace(eyeNorm, eyePosition);

	//ライティング方程式を評価(ポリゴンの両面)
	frontColor = PhongModel(eyePosition, eyeNorm);
	backColor = PhongModel(eyePosition, -eyeNorm);

	//位置をクリップ座標に変換して後に渡す
	gl_Position = MVP * vec4(vertexPosition, 1.0);
}
