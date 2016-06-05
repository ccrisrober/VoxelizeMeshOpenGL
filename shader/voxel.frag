#version 430
in vec3 outNormal;
in vec3 outPosition;
out vec4 FragColor;

uniform vec3 viewPos;

float constant = 1.0;
float linear = 0.045;
float quadratic = 0.007;

uniform vec3 Kd;
uniform vec3 lightPosition;
vec3 lightColor = vec3(1.0, 0.0, 0.0);

void main() {
	float dist = length(lightPosition - outPosition);
	float attenuation = 1.0f / (constant + linear * dist + quadratic * (dist * dist));

	// Ambient
	vec3 ambient = 0.05 * vec3(0.1, 0.18725, 0.745);

	// Diffuse
	vec3 lightDir = normalize(lightPosition - outPosition);
	vec3 normal = normalize(outNormal);
	float diff = max(dot(vec3(0.396, 0.74151, 0.69102), normal), 0.0);
	vec3 diffuse = diff * vec3(0.396, 0.74151, 0.69102);

	// Specular
	vec3 viewDir = normalize(viewPos - outPosition);
	vec3 reflectDir = reflect(-lightDir, normal);
	vec3 halfwayDir = normalize(lightDir + viewDir);  
	vec3 specular = vec3(0.3) * pow(max(dot(normal, halfwayDir), 0.0), 0.1) * vec3(0.297254, 0.30829, 0.306678);

	// Apply attenuation
	//ambient *= attenuation;
	//diffuse *= attenuation;
	//specular *= attenuation;

	FragColor = vec4(ambient + diffuse + specular, 1.0f);
}