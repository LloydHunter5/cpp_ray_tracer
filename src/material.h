#ifndef MATERIAL_H
#define MATERIAL_H

#include "rtutils.h"
#include "hittable.h"

struct hit_record;

class material {
    public:
        virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
        ) const = 0;
};


class lambertian : public material {
    public:
        lambertian(const color& a) : albedo(a) {}

        virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
        ) const override {
            auto scatter_direction = rec.normal + vec3::random();

            if(scatter_direction.near_zero()){
                scatter_direction = rec.normal;
            }

            scattered = ray(rec.p, scatter_direction);
            attenuation = albedo;
            return true;
        }

    public:
        color albedo;
};

class metal : public material {
    public:
        metal(const color& a, float fuzz) : albedo(a), fuzz(fuzz) {}

        virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
        ) const override {
            auto reflected_direction = reflect(unit_vector(r_in.direction()),rec.normal);
            scattered = ray(rec.p, reflected_direction + (fuzz * random_in_unit_sphere()));
            attenuation = albedo;
            return dot(scattered.direction(),rec.normal) > 0;
        }

    public:
        color albedo;
        float fuzz;
};

#endif