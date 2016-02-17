varying float height;
uniform float tallestPoint;
varying vec4 texCoordV;
vec3 normal, lightDir, viewVector, halfVector;
vec4 diffuse, ambient, globalAmbient, specular = vec4(0.0);
float NdotL,NdotHV;

void main()
{
	height = gl_Vertex.y / tallestPoint;

	gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
	texCoordV = gl_TexCoord[0];
   // gl_Position =  gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;

	/* first transform the normal into eye space and normalize the result */
	normal = normalize(gl_NormalMatrix * gl_Normal);
	
	lightDir = normalize(vec3(gl_LightSource[0].position));
	
	
	NdotL = max(dot(normal, lightDir), 0.0);
	
	diffuse = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;
	ambient = gl_FrontMaterial.ambient * gl_LightSource[0].ambient;
	globalAmbient = gl_LightModel.ambient * gl_FrontMaterial.ambient;
	
	/* compute the specular term if NdotL is  larger than zero */
	if (NdotL > 0.0) {

		NdotHV = max(dot(normal, normalize(gl_LightSource[0].halfVector.xyz)),0.0);
		specular = gl_FrontMaterial.specular * gl_LightSource[0].specular * pow(NdotHV,gl_FrontMaterial.shininess);
	}
	
	gl_FrontColor = globalAmbient + NdotL * diffuse + ambient + specular;
	
	gl_Position = ftransform();
}