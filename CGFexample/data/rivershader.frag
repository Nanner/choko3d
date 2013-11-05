uniform sampler2D waterImage;
uniform sampler2D heightImage;
uniform float time;

void main()
{
		vec2 textCoords = gl_TexCoord[0].st;
		textCoords.t += time * 0.00001;
		vec4 color = texture2D(waterImage, textCoords);
        
        gl_FragColor = color;
}