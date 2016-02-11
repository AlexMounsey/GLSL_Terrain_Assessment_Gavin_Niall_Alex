varying float height;
uniform float tallestPoint;
varying vec4 texCoordV;

void main()
{
	
	height = gl_Vertex.y / tallestPoint;
	gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
	texCoordV = gl_TexCoord[0];
    gl_Position =  gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
}