
#include "../rays/shapes.h"
#include "debug.h"
#include <iostream>

namespace PT {
const char *Shape_Type_Names[(int)Shape_Type::count] = {"None", "Sphere"};

BBox Sphere::bbox() const {

    BBox box;
    box.enclose(Vec3(-radius));
    box.enclose(Vec3(radius));
    return box;
}

Trace Sphere::hit(const Ray &ray) const {

    // TODO (PathTracer): Task 2
    // Intersect this ray with a sphere of radius Sphere::radius centered at the origin.

    // If the ray intersects the sphere twice, ret should
    // represent the first intersection, but remember to respect
    // ray.time_bounds! For example, if there are two intersections,
    // but only the _later_ one is within ray.time_bounds, you should
    // return that one!

    // Note: ray.dir is not necessarily a unit vector if this object has a scale transform.
    // If you want it to be so, you should use ray.dir.unit(), and also note that you will
    // have to re-scale ret.time to be the distance along the original non-unit ray.dir.

    // RAY DIRECTION MUST BE UNIT IF YOU USE 
    // THE SIMPLIFIED QUADRATIC FORMULA IN SLIDES (not used here)
    Vec3 d = ray.dir;
    // Vec3 d = ray.dir.unit();
    Vec3 o = ray.point;
    float od = dot(o, d);
    float d2 = d.norm_squared();
    float o2 = o.norm_squared();
    float discriminant = od*od - d2*(o2 - radius*radius); // b^2 - 4ac
    float t = 0.f;
    bool hit = false;

    if (discriminant <= 0) {
    } else {
        float root_discriminant = sqrt(discriminant);
        float t1 = (-od + root_discriminant)/(d2); // later
        float t2 = (-od - root_discriminant)/(d2); // earlier

        if (t2 <= ray.time_bounds.y && t2 >= ray.time_bounds.x) {
            t = t2;
            hit = true;
        } else if (t1 <= ray.time_bounds.y && t1 >= ray.time_bounds.x) {
            t = t1;
            hit = true;
        }
    }
    // t *= ray.dir.norm(); // SCALE BACK TIME IF YOU USE UNIT RAY DIR

    Trace ret;
    ret.hit = hit; // was there an intersection?
    if (!hit) return ret;
    ret.time = t; // at what time did the intersection occur?
    ray.time_bounds.y = t; // update time bounds for efficiency
    ret.position = ray.at(t); // where was the intersection?
    ret.normal = ret.position.unit(); // what was the surface normal at the intersection?
    return ret;
}

} // namespace PT
