# version 450

in vec3 interpolatedNormal;
varying vec3 vertPos;
varying vec3 f_color;

const vec3 lightPos = vec3(1.0f, 1.0f, 1.0f);

void main(void) {
	float shine = 50;
	float dLight;

	vec3 lightDir = normalize(lightPos - vertPos);
	vec3 viewDir = normalize(-vertPos);

	dLight = dot(normalize(lightDir), normalize(interpolatedNormal));

	dLight = max(dLight, 0.0);
	dLight = min(dLight, 1.0);

	vec3 halfAngle = normalize(lightDir + viewDir);
	float blinn = dot(normalize(interpolatedNormal), halfAngle);

	blinn = max(blinn, 0);

	blinn = dLight != 0.0 ? blinn : 0.0;
	blinn = pow(blinn, shine);

	vec3 color = 0.5f*f_color*dLight + f_color*blinn;
	gl_FragColor = vec4(color, 1.0f);
}