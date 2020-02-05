#version 410 core

#define GAMMA 2.2

out vec4	FragColor;

in VS_OUT {
	vec3 FragPos;
	vec3 Normal;
} fs_in;

struct	Material {
	vec3		specular;
	float		shininess;
};

struct DirLight {
	vec3		direction;

	vec3		ambient;
	vec3		diffuse;
	vec3		specular;
};

struct PointLight {
    bool		enabled;
    vec3		position;

    float		constant;
    float		linear;
    float		quadratic;

    vec3		ambient;
    vec3		diffuse;
    vec3		specular;
};

uniform vec4		color;
uniform vec3		viewPos;
uniform Material	material;
uniform DirLight	dirLight;
uniform PointLight	pointLight;

vec3 calcDirLight(DirLight light, vec3 norm, vec3 viewDir) {
	vec3	lightDir = normalize(-light.direction);
	// diffuse
	float	diff = max(dot(norm, lightDir), 0.0);
	// specular
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(norm, halfwayDir), 0.0), material.shininess * 4);

	// ambiant, diffuse
	vec3	ambient = light.ambient;
	vec3	diffuse = light.diffuse;

	vec3 tmp = vec3(color);
	ambient *= tmp;
	diffuse *= diff * tmp;

	vec3 specular = light.specular;
	specular *= spec * pow(material.specular, vec3(GAMMA));

	return ambient + diffuse + specular;
}

vec3 calcPointLight(PointLight light, vec3 norm, vec3 fragPos, vec3 viewDir) {
	vec3    lightDir = normalize(light.position - fragPos);
	// diffuse
	float	diff = max(dot(norm, lightDir), 0.0);
	// specular
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(norm, halfwayDir), 0.0), material.shininess * 4);

	// ambiant, diffuse
	vec3	ambient = light.ambient;
	vec3	diffuse = light.diffuse;

	vec3 tmp = vec3(color);
	ambient *= tmp;
	diffuse *= diff * tmp;

	// specular
	vec3 specular = light.specular;
	specular *= spec * pow(material.specular, vec3(GAMMA));

	// attenuation
	float	distance = length(light.position - fragPos);
	float	attenuation = 1.0 / (light.constant + light.linear * distance + \
	light.quadratic * (distance * distance));

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	return (ambient + diffuse + specular);
}

void main() {
	vec3 norm = normalize(fs_in.Normal);
	vec3 viewDir = normalize(viewPos - fs_in.FragPos);

	// Directional lighting
	vec3 res = calcDirLight(dirLight, norm, viewDir);

	// PointLight
	if (pointLight.enabled) {
		res += calcPointLight(pointLight, norm, fs_in.FragPos, viewDir);
	}

	FragColor = vec4(res, 1.0);

	// apply gamma correction
    FragColor.rgb = pow(FragColor.rgb, vec3(1.0 / GAMMA));
}
