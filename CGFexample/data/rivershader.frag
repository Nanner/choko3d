uniform sampler2D waterImage;
uniform sampler2D heightImage;
uniform float time;

void main()
{
		vec2 textCoords = gl_TexCoord[0].st;
        
        gl_FragColor = texture2D(waterImage, textCoords);
}