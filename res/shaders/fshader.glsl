#version 430

struct Light
{
    vec4 position;
    vec3 color;
};

struct AmbientLight
{
    vec3 color;
};

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

// Position and normal of the fragment
in vec3 vertexPos;    // Untransformed position of the vertex (in object coordinates)
in vec3 fragPos;      // Transformed position of the vertex (in world coordinates)
in vec3 fragNormal;   // Transformed normal of the vertex (in world coordinates)

// Light and material information
uniform Material material;
uniform Light light;
uniform AmbientLight ambientLight;

// Camera information
uniform vec3 cameraPos;

// Information related to the texture
in vec2 texCoord;
uniform bool useTwoPartMapping;
uniform vec3 sphereCenter;
uniform sampler2D modelTexture;

out vec4 FragColor;

float pi = 3.14159265359;

void main()
{
    // Compute the texture color, optionally using two-part texture mapping
    vec2 texCoords = texCoord;
    if (useTwoPartMapping) {
        vec3 sphereToFragment = normalize(vertexPos - sphereCenter);
        float phi = acos(sphereToFragment.y);
        float theta = atan(sphereToFragment.z, sphereToFragment.x);
        float s = (theta / (2.0 * pi) + 0.5);
        float t = phi / pi;
        texCoords = vec2(1 - s, t);
    } else {
        // Flip the y-coordinate to match the texture.
        texCoords = vec2(texCoords.x, 1 - texCoords.y);
    }

    vec4 texColorA = texture(modelTexture, texCoords).rgba;
    vec3 texColor = texColorA.rgb;

    // Compute common vectors
    vec3 normal = normalize(fragNormal);
    vec3 lightDir = normalize(light.position.xyz - fragPos);

    // Compute the ambient term
    vec3 ambient = ambientLight.color * material.ambient;

    // Compute the diffuse term
    float diffuseStrength = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diffuseStrength * material.diffuse * light.color;

    // Compute the specular term
    vec3 specular = vec3(0.0);
    if (diffuseStrength > 0.0) {
        vec3 viewDir = normalize(cameraPos - fragPos);
        vec3 reflectDir = reflect(-lightDir, normal);
        float dt = max(dot(viewDir, reflectDir), 0.0);
        float specularStrength = pow(dt, material.shininess);
        specularStrength = max(min(specularStrength, 1.0), 0.0);
        specular = specularStrength * material.specular * light.color;
    }

    // Compute the final color
    vec3 result = (ambient + diffuse + specular) * texColor;
    FragColor = vec4(result, texColorA.a);
}
