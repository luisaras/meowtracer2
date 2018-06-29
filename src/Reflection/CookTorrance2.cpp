inline float saturate(float x) {
	return clamp(x, -1, 1);
}

// http://www.codinglabs.net/article_physically_based_rendering_cook_torrance.aspx

float chiGGX(float v) {
    return v > 0 ? 1 : 0;
}

float GGX_PartialGeometryTerm(const Vec3 &v, const Vec3 &n, const Vec3 &h, float alpha) {
    float VoH2 = saturate(Vec3::dot(v, h));
    float chi = chiGGX(VoH2 / saturate(Vec3::dot(v, n)));
    VoH2 = VoH2 * VoH2;
    float tan2 = (1 - VoH2) / VoH2;
    return (chi * 2) / (sqrt(alpha * alpha * tan2 + 1) + 1);
}

Vec3 Fresnel_Schlick(float cosT, const Vec3 &F0) {
  return F0 + (1 - F0) * pow(1 - cosT, 5);
}

 Vec3 GGX_Specular(const Color& specular, const Vec3& normal, const Vec3& viewVector, 
 		float roughness, const Vec3& F0, Vec3& kS) {

    Vec3 radiance(0, 0, 0);
    float NoV = saturate(Vec3::dot(normal, viewVector));

    for(int i = 0; i < SamplesCount; i++) {
        // Generate a sample vector in some local space
        Vec3 sampleVector = GGX(roughness);

        // Convert the vector in world space
        sampleVector = Vec3::normalize(sampleVector);

        // Calculate the half vector
        Vec3 halfVector = Vec3::normalize(sampleVector + viewVector);
        float cosT = saturate(Vec3::dot(sampleVector, normal));
        float sinT = sqrt(1 - cosT * cosT);

        // Calculate fresnel
        Vec3 fresnel = Fresnel_Schlick(saturate(Vec3::dot(halfVector, viewVector)), F0);
        // Geometry term
        float geometry = GGX_PartialGeometryTerm(viewVector, normal, halfVector, roughness) 
        	* GGX_PartialGeometryTerm(sampleVector, normal, halfVector, roughness);
        // Calculate the Cook-Torrance denominator
        float denominator = saturate(4 * (NoV * saturate(Vec3::dot(halfVector, normal)) + 0.05));
        kS += fresnel;
        
        // Accumulate the radiance
        radiance += specular * geometry * fresnel * sinT / denominator;
    }

    // Scale back for the samples count
    kS = (kS / SamplesCount).clamp(-1, 1);
    return radiance / SamplesCount;        
}

Color CookTorrance::lightColor(Light* light, LightHit& lh, Color& texture) {
    Material* mat = lh.rayHit.hitable->material;

    // Material properties
    float ior = mat->refraction;
    float roughness = mat->roughness;
    float metallic = mat->reflectivity;

    // Calculate color at normal incidence
    float f = std::abs((1.0 - ior) / (1.0 + ior));
    Vec3 F0(f, f, f);
    F0 = F0 * F0;
    F0 = Vec3::lerp(F0, texture, 1 - metallic);
        
    // Calculate the specular contribution
    Vec3 ks(0, 0, 0);
    Vec3 specular = GGX_Specular(mat->ks, 
    	lh.rayHit.normal, lh.ray.direction, 
    	roughness, F0, ks);
    Vec3 kd = (1 - ks) * (1 - metallic);
    // Calculate the diffuse contribution
    Vec3 irradiance = lh.color;
    Vec3 diffuse = texture * irradiance;

    return kd * diffuse + specular; 

}