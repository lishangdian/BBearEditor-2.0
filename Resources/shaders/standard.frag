varying vec4 V_world_pos;
varying vec4 V_Color;
varying vec4 V_Normal;
varying vec4 V_world_pos_light_space;

uniform vec4 BBLightSettings0;
uniform vec4 BBLightSettings1;
uniform vec4 BBLightSettings2;
uniform vec4 BBLightPosition;
uniform vec4 BBLightColor;
uniform vec4 BBCameraPosition;
uniform sampler2D BBShadowMap;

float getLambertPointLightIntensity(vec3 normal, float radius, float distance, float attenuation, vec3 L)
{
    float delta = radius - distance;
    float intensity = 0.0;
    if (delta >= 0)
    {
        intensity = max(0.0, dot(L, normal) * attenuation * delta / radius);
    }
    return intensity;
}

float getLambertSpotLightIntensity(vec3 normal, float radius, float distance, float attenuation, vec3 L)
{
    float intensity = max(0.0, dot(normal, L));
    if (intensity > 0.0)
    {
        vec3 spot_direction = normalize(BBLightSettings2.xyz);
        // Cosine of the angle between the current point and the spotlight direction
        float current_cos = max(0.0, dot(spot_direction, -L));
        // cutoff cosine
        float cutoff_radian = BBLightSettings0.z / 2 * 3.14 / 180.0;
        float cutoff_cos = cos(cutoff_radian);
        if (current_cos > cutoff_cos)
        {
            // Within the cutoff range
            float delta = radius - distance;
            intensity = pow(current_cos, BBLightSettings0.w) * 2.0 * attenuation * delta / radius;
        }
        else
        {
            intensity = 0.0;
        }
    }
    return intensity;
}

// shadow
float calculateShadow()
{
    vec3 pos = V_world_pos_light_space.xyz / V_world_pos_light_space.w;
    // -1~1 -> 0~1
    pos = pos * 0.5 + vec3(0.5);
    // For the current fragment, get the corresponding depth from the depth map in the light viewport
    // float depth_shadow_map = texture2D(BBShadowMap, uv).r;
    // actual depth value of current fragment
    float current_depth = pos.z;
    current_depth = clamp(current_depth, 0.0, 1.0);
    // PCF
    vec2 texel_size = 1.0 / textureSize(BBShadowMap, 0);
    float shadow = 0.0;
    for (int y = -1; y <= 1; y++)
    {
        for (int x = -1; x <= 1; x++)
        {
            float depth_shadow_map = texture2D(BBShadowMap, pos.xy + texel_size * vec2(x, y)).r;
            shadow += (current_depth - 0.004) > depth_shadow_map ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;
    shadow = (current_depth - 0.004) > texture2D(BBShadowMap, pos.xy).r ? 1.0 : 0.0;
    return shadow;
}

void main(void)
{
    vec4 final_color = V_Color;
    vec3 normal = normalize(V_Normal.xyz);
    if (BBLightPosition.w == 0.0)
    {
        // there is a directional light
        // Lambert
        vec3 L = normalize(BBLightPosition.xyz);
        float intensity = max(0.0, dot(normal, L));
        final_color = intensity * BBLightColor;
        // // phong
        // if (intensity > 0.0)
        // {
        //     vec3 view_dir = normalize(cameraPosition.xyz - V_world_pos.xyz);
        //     vec3 reflect_dir = normalize(reflect(-L, normal));
        //     float phong_intensity = pow(max(0.0, dot(reflect_dir, view_dir)), 4.0f);
        //     final_color += phong_intensity * lightColor;
        // }
        // blinn-phong
        // if (intensity > 0.0)
        // {
        //     vec3 view_dir = normalize(BBCameraPosition.xyz - V_world_pos.xyz);
        //     vec3 half_vector = normalize(L + view_dir);
        //     float phong_intensity = pow(max(0.0, dot(normal, half_vector)), 4.0f);
        //     final_color += phong_intensity * BBLightColor;
        // }
    }
    else if (BBLightPosition.w == 1.0)
    {
        float radius = BBLightSettings1.x;
        float constant_factor = BBLightSettings1.y;
        float linear_factor = BBLightSettings1.z;
        float quadric_factor = BBLightSettings1.w;
        vec3 L = BBLightPosition.xyz - V_world_pos.xyz;
        float distance = length(L);
        float attenuation = 1.0 / (constant_factor + linear_factor * distance + quadric_factor * quadric_factor * distance);
        L = normalize(L);

        float intensity;
        if (BBLightSettings0.x == 2.0)
        {
            // there is a spot light
            intensity = getLambertSpotLightIntensity(normal, radius, distance, attenuation, L);
        }
        else
        {
            // there is a point light
            intensity = getLambertPointLightIntensity(normal, radius, distance, attenuation, L);
        }
        final_color = BBLightColor * intensity * BBLightSettings0.y;
    }
    
    // shadow
    final_color = final_color * vec4(vec3(1.0 - calculateShadow()), 1.0);
    gl_FragColor = final_color * V_Color;
}
