uniform sampler2D grassTex;
uniform sampler2D waterTex;
uniform sampler2D snowTex;
varying float height;

void main()
{
	vec4 color;

	if(height < 0.05f)
	{
		//water
		color = texture2D(waterTex,gl_TexCoord[0].st);
	}

	else if(height < 0.1f)
	{
		//water, grass
		float localheight = height - 0.05f;
		float percent = localheight / 0.05f;
		vec4 c1 = texture2D(waterTex,gl_TexCoord[0].st);
		vec4 c2 = texture2D(grassTex,gl_TexCoord[0].st);
		
		color = mix(c1, c2, percent);
	}

	else if(height < 0.6f)
	{
		//grass
		color = texture2D(grassTex,gl_TexCoord[0].st);
	}

	else if(height < 0.75f){
		//grass, rock
		float localheight = height - 0.6f;
		float percent = localheight / 0.15f;
		vec4 c1 = texture2D(grassTex,gl_TexCoord[0].st);
		vec4 c2 = texture2D(snowTex,gl_TexCoord[0].st);
		
		color = mix(c1, c2, percent);
	}

	else
	{
		//rock
		color = texture2D(snowTex,gl_TexCoord[0].st);
	}
		 
	//vec4 color = texture2D(waterTex,gl_TexCoord[0].st);
	//vec4 color = texture2D(snowTex,gl_TexCoord[0].st);
    gl_FragColor = gl_Color * color;
}