uniform float normScale;
uniform sampler2D heightImage;
uniform sampler2D waterImage;
uniform float time;

void main() 
{
        vec4 distortion = gl_Vertex;
        
		vec4 textCoords = gl_MultiTexCoord0;
		textCoords.t += mod(time, 10000.0) / 10000.0;
        vec4 heightMap = texture2D(heightImage, textCoords.st);

		// change vertex distortion in relation to height map information
		// The "whiter" the current texel is, the higher and more dislocated our wave needs to be
        distortion.y += float(heightMap.r + heightMap.g + heightMap.b);
		distortion.z += float(heightMap.r + heightMap.g + heightMap.b) * 0.05;
         
        // Set the position of the current vertex
        gl_Position = gl_ModelViewProjectionMatrix * distortion;

        // pass texture coordinates from VS to FS.
        // "gl_MultiTexCoord0" has the texture coordinates assigned to this vertex in the first set of coordinates.
        // This index has to do with the set of texture COORDINATES, it is NOT RELATED to the texture UNIT.
        // "gl_TexCoord[0]" is a built-in varying that will be interpolated in the FS.
        gl_TexCoord[0] = textCoords;
}