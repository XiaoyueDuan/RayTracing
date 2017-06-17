#pragma once
#include <algorithm>
#include <random>

#include "Geometry.h"
#include "Constant.h"
#include "FileLoader.h"
#include "Options.h"
#include "ImageSaver.h"
#include "Light.h"

//#define GI

class Path
{
public:
	// Diffuse
	// N is normal: Y
	// Nt : Z
	// Nb : X
	// Sample(world coordinate)= Sample(local) * Transform matrix
	// Transform matrix:
	// |Nt.x Nt.y Nt.z|
	// |N.x  N.y  N.z |
	// |Nb.x Nb.y Nb.z|
	void createCoordinateSystem(const Vec3f &N, Vec3f &Nt, Vec3f &Nb)
	{
		if (fabs(N.x) > fabs(N.y))
			Nt = Vec3f(N.z, 0, -N.x) / sqrtf(N.x * N.x + N.z * N.z);
		else
			Nt = Vec3f(0, -N.z, N.y) / sqrtf(N.y * N.y + N.z * N.z);
		Nb = N.crossProduct(Nt);
	}

	// r1 and r2 are subject to [0, 1] Uniform distribuation
	Vec3f uniformSampleHemisphere(const float &r1, const float &r2)
	{
		// cos(theta) = u1 = y
		// cos^2(theta) + sin^2(theta) = 1 -> sin(theta) = srtf(1 - cos^2(theta))
		float sinTheta = sqrtf(1 - r1 * r1);
		float phi = 2 * M_PI * r2;
		float x = sinTheta * cosf(phi);
		float z = sinTheta * sinf(phi);
		return Vec3f(x, r1, z);
	}

	// Reflect
	Vec3f reflect(const Vec3f &I, const Vec3f &N)
	{
		return I - 2 * I.dotProduct(N) * N;
	}

	// Refraction
	Vec3f refract(const Vec3f &I, const Vec3f &N, const float &ior)
	{
		float cosi = min(max(float(-1.0), I.dotProduct(N)), float(1.0));
		float etai = 1, etat = ior;
		Vec3f n = N;
		if (cosi < 0) 
			cosi = -cosi;
		else 
		{
			swap(etai, etat); 
			n = -N; 
		}
		float eta = etai / etat;
		float k = 1 - eta * eta * (1 - cosi * cosi);
		return k < 0 ? 0 : eta * I + (eta * cosi - sqrtf(k)) * n;
	}
};

Vec3f castRay(
	const Vec3f &orig, const Vec3f &dir,
	Scene &scene,
	vector<Light *> &lights,
	const Options &options,
	const int depth,
	const int N = 1)
{
	if (depth > options.depth)
		return options.backgroundColor;

	Vec3f hitColor = options.backgroundColor;
	float tnear = kInfinity;
	Vec2f uv;
	int index = 0;
	Object *hitObject = nullptr;
	if (scene.intersect(orig, dir, tnear, index, uv, hitObject))
	{
		// get the surface properties, including:
		//		uv Coordinates
		//		Hit Normal
		//		Texture Coordinates
		//		Color
		//		Material
		Vec3f hitPoint = orig + dir * tnear;
		Vec3f hitNormal;
		Vec2f hitTexCoordinates;
		Vec3f Color;
		Material *m = nullptr;
		hitObject->getSurfaceProperties(hitPoint, dir, index, uv, hitNormal, hitTexCoordinates, Color, m);

		// If the hit point is light source
		if (m->self_luminous)
			return m->ka*Color;

		Path path;
		// If it's diffuse
		if (m->diffuse)
		{
			random_device rd;  //Will be used to obtain a seed for the random number engine
			mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()

			//default_random_engine generator;
			uniform_real_distribution<float> distribution(0.0, 1.0);

			Vec3f directLighting = 0;
			for (int i = 0; i < lights.size(); ++i)
			{
				Vec3f lightDir, lightIntensity;
				float tNear;
				lights[i]->illuminate(hitPoint, lightDir, lightIntensity, tNear);

				int index=0;
				bool vis = !scene.intersect(hitPoint, -lightDir, tNear, index, uv, hitObject);
				directLighting+=vis * lightIntensity * max(0.f, hitNormal.dotProduct(-lightDir));
			}

			Vec3f indirectLigthing = 0;
			Vec3f Nt, Nb;
			path.createCoordinateSystem(hitNormal, Nt, Nb);
			float pdf = 1 / (2 * M_PI);
			for (int n = 0; n < N; ++n) {
				float r1 = distribution(gen);
				float r2 = distribution(gen);
				Vec3f sample = path.uniformSampleHemisphere(r1, r2);
				Vec3f sampleWorld(
					sample.x * Nb.x + sample.y * hitNormal.x + sample.z * Nt.x,
					sample.x * Nb.y + sample.y * hitNormal.y + sample.z * Nt.y,
					sample.x * Nb.z + sample.y * hitNormal.z + sample.z * Nt.z);
				// don't forget to divide by PDF and multiply by cos(theta)
				indirectLigthing += r1 * castRay(hitPoint, sampleWorld, scene, lights, options, depth + 1) ;
			}
			// divide by N
			indirectLigthing /= (float)N;
			hitColor = (directLighting + 2 * indirectLigthing)*m->kd;
		}

		// If it's specular
		if (m->specular)
		{
			Vec3f Reflect = path.reflect(dir, hitNormal).normalize();
			Vec3f lightIntensity = castRay(hitPoint, Reflect, scene, lights, options, depth + 1);
#ifdef GI
			hitColor += lightIntensity * m->ks;
#else	
			Vec3f specular = 0;
			for (int i = 0; i < lights.size(); ++i)
			{
				Vec3f lightDir, lightIntensity;
				float tNear;
				lights[i]->illuminate(hitPoint, lightDir, lightIntensity, tNear);

				float cosineAlpha = Reflect.dotProduct(lightDir);
				// directLighting += vis * lightIntensity * abs(hitNormal.dotProduct(-lightDir));
				specular += lightIntensity * max(0.f, pow(cosineAlpha, m->ns.exponent));
			}
			specular = specular * m->ks;
			hitColor += specular;
			/*hitColor += lightIntensity * m->ks;*/
#endif		
		}

		// If it's transparent
		if (m->transparent)
		{
			Vec3f refractionColor = 0, reflectionColor = 0;
			// compute fresnel
			float kr = m->tr.ratio;
			bool outside = dir.dotProduct(hitNormal) < 0;
			// compute refraction 	
			Vec3f refractionDirection = path.refract(dir, hitNormal, m->ni.optical_density).normalize();
			refractionColor = castRay(hitPoint, refractionDirection, scene, lights, options, depth + 1);
			// compute reflect 	
			Vec3f reflectionDirection = path.reflect(dir, hitNormal).normalize();
			reflectionColor = castRay(hitPoint, reflectionDirection, scene, lights, options, depth + 1);

			// mix the two
			hitColor = reflectionColor * (1 - kr) + refractionColor * kr;
		}
	}
	return hitColor;
}

Vec3f trim(Vec3f &v)
{
	Vec3f result;
	result.x = max(0.0f, min(v.x, 1.0f));
	result.y = max(0.0f, min(v.y, 1.0f));
	result.z = max(0.0f, min(v.z, 1.0f));

	return result;
}

void render(
	const Options &options,
	Scene &scene,
	Vec3f *pixels,
	vector<Light *> &lights,
	int N = 1)
{
	float scale = tan(options.fov * 0.5*M_PI / 180);
	float imageAspectRatio = options.width / (float)options.height;
	Vec3f orig;
	options.cameraToWorld.multVecMatrix(Vec3f(0), orig);
	Vec3f *start = pixels;

	for (int j = options.height-1; j >=0 ; --j) {
		cout << j << endl;
		for (int i = 0; i < options.width; ++i) {
			Vec3f color;
			int nValid = 0;
			for (int w = 0; w < N; ++w)
			{
				random_device rd;  //Will be used to obtain a seed for the random number engine
				mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()								   
				uniform_real_distribution<float> distribution(0.0, 1.0);//default_random_engine generator;

				// generate primary ray direction
				float r1 = distribution(gen);
				float r2 = distribution(gen);
				float x = (2 * (i + r1) / (float)options.width - 1) * imageAspectRatio * scale;
				float y = (1 - 2 * (j + r2) / (float)options.height) * scale;
				Vec3f dir;
				options.cameraToWorld.multDirMatrix(Vec3f(x, y, 1), dir);
				dir.normalize();

				Vec3f newColor = castRay(orig, dir, scene, lights, options, 0);
				color += newColor;
			}
			
			(*pixels) = trim(color / float(N));
			pixels++;
		}

		if (j %20==0)
		{
			int dpi = 72;
			string saveFileName = "result/" + to_string(j) + ".bmp";
			savebmp(saveFileName, options.width, options.height, dpi, start);
		}
	}
}