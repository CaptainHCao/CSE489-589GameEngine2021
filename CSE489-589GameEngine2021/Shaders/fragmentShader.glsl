// Targeting version 4.5 of GLSL. If the compiler does not support 4.5 it will cause an error.
#version 450 core

in vec3 worldPos;
in vec3 worldNorm;
in vec2 texCoord0;
out vec4 fragmentColor;

const int MaxLights = 16;

// Structure for holding general light properties
struct GeneralLight
{
	vec4 ambientColor;		// ambient color of the light
	vec4 diffuseColor;		// diffuse color of the light
	vec4 specularColor;		// specular color of the light

	// Either the position or direction
	// if w = 0 then the light is directional and direction specified 
	// is the negative of the direction the light is shinning
	// if w = 1 then the light is positional
	vec4 positionOrDirection;

	// Spotlight attributes
	vec3 spotDirection;		// the direction the cone of light is shinning    
	bool isSpot;				// 1 if the light is a spotlight  
	float spotCutoffCos;	// Cosine of the spot cutoff angle
	float spotExponent;		// For gradual falloff near cone edge

	// Attenuation coefficients
	float constant;
	float linear;
	float quadratic;

	bool enabled;			// true if light is "on"

};

layout(shared) uniform LightBlock
{
	GeneralLight lights[MaxLights];
};

layout(shared) uniform worldEyeBlock
{
	vec3 worldEyePosition;
};


struct Material
{
	vec4 ambientMat;
	vec4 diffuseMat;
	vec4 specularMat;
	vec4 emmissiveMat;
	float specularExp;
	int textureMode;
	bool diffuseTextureEnabled;
	bool specularTextureEnabled;
};

layout(shared) uniform MaterialBlock
{
	Material object;
};

//vec3 lightPosition = vec3( -10.f, 10.0f, 10.0f); // Positional light
//
//vec3 l = normalize(lightPosition - worldPos); // Positional light
//vec3 v = normalize( worldEyePosition - worldPos );
//vec3 r = normalize(reflect(-l, worldNorm));
//
//vec4 lightAmbient = vec4(0.2f, 0.2f, 0.2f, 1.0f);
//vec4 lightDif = vec4(1.0f, 1.0f, 1.0f, 1.0f);
//vec4 lightSpec = vec4(1.0f, 1.0f, 1.0f, 1.0f);
//
//vec4 objSpec = vec4(1.0f, 1.0f, 1.0f, 1.0f);

layout(binding = 0) uniform sampler2D diffuseSampler;
layout(binding = 1) uniform sampler2D specularSampler;

//void main()
//{
//	vec4 ambColor;
//	vec4 difColor;
//	vec4 specularColor;
//
//	fragmentColor = object.emmissiveMat;
//
//	if ( object.textureMode != 0) {
//
//		ambColor = texture( diffuseSampler, texCoord0.st );
//		difColor = ambColor;
//	}
//	else {
//		ambColor = object.ambientMat;
//		difColor = object.diffuseMat;
//	}
//
//	if(object.specularTextureEnabled) {
//
//		specularColor = texture( specularSampler, texCoord0.st );
//	}
//	else {
//		specularColor = object.specularMat;
//	}
//
//	fragmentColor += lightAmbient * ambColor;
//	fragmentColor += max(0, dot(l, worldNorm.xyz)) * lightDif * difColor; 
//	fragmentColor += pow(max(0, dot(r, v)), object.specularExp) *  lightSpec * object.specularMat;
//	fragmentColor = clamp(fragmentColor, 0.0f, 1.0f);
//
//}

void main()
{
	vec4 totalColor = object.emmissiveMat;

	vec4 ambientColor = object.ambientMat;

	vec4 diffuseColor = object.diffuseMat;

	vec4 specularColor = object.specularMat;

	if(object.textureMode != 0 && object.diffuseTextureEnabled) {

		diffuseColor = texture( diffuseSampler, texCoord0 );
		ambientColor = diffuseColor;
	 }

	 if(object.textureMode != 0 && object.specularTextureEnabled) {

		specularColor = texture( specularSampler, texCoord0 );
	 }

	 if(object.textureMode != 1) {

		 for(int i = 0; i < MaxLights ; i++) {

			 if(lights[i].enabled == true ) {

				vec3 lightVector;

				float attenuation;

				float spotCosine = 1.0;

				float fallOffFactor = 1.0f;

				if(lights[i].positionOrDirection.w < 0.98f) {
					// Directional
					lightVector = normalize(lights[i].positionOrDirection.xyz);

					attenuation = 1.0f;
				}
				else {
					// Positional
					lightVector = normalize(lights[i].positionOrDirection.xyz - worldPos);

					float distanceToLight = distance(lights[i].positionOrDirection.xyz, worldPos);

					attenuation = 1.0f / (lights[i].constant + lights[i].linear *distanceToLight + lights[i].quadratic * distanceToLight * distanceToLight);


					if (lights[i].isSpot == true) {

						spotCosine = dot(-lightVector, normalize(lights[i].spotDirection));

						fallOffFactor = 1.0f - (1.0f - spotCosine) / (1.0f - lights[i].spotCutoffCos);

					}
				}

				// Is it a spot light and are we in the cone?
				if (lights[i].isSpot == false || (lights[i].isSpot == true && spotCosine >= lights[i].spotCutoffCos)) {

					totalColor += ambientColor * lights[i].ambientColor;

					totalColor += max(dot(worldNorm, lightVector), 0.0f) * diffuseColor * lights[i].diffuseColor;

					vec3 reflection = normalize(reflect(-lightVector, worldNorm));

					vec3 viewVector = normalize(worldEyePosition - worldPos);

					totalColor += attenuation * fallOffFactor * pow(max(dot(reflection, viewVector),0.0f),object.specularExp) * specularColor * lights[i].specularColor;

				}
			}
		}
	}
	else {

		totalColor = diffuseColor;
	}

	fragmentColor = totalColor;

}