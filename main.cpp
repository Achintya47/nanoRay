#include <iostream>
#include "util/color.h"
#include "util/vec3.h"
#include "util/ray.h"

#include <iostream>

// hit_sphere using 
double hit_sphere(const point3& center, double radius, const ray& r) {

    vec3 oc = center - r.origin();
    double a = r.direction().length_squared();
    double h = dot(r.direction(), oc);
    double c = oc.length_squared() - radius * radius;
    double discriminant = h * h - a * c;

    if (discriminant < 0) 
        return -1.0;
    else
        return (h - std::sqrt(discriminant)) / a;
}

color ray_color(const ray& r) {

    double t = hit_sphere(point3(0, 0, -1), 0.5, r);
    if (t > 0.0){
        vec3 N = unit_vector(r.at(t) - vec3(0, 0, -1));
        return 0.5 * color(N.x() + 1, N.y() + 1, N.z() + 1);
    }

    vec3 unit_direction = unit_vector(r.direction());
    double a = 0.5*(unit_direction.y() + 1.0);
    // A lerp bend of White and Blue-
    return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
}


int main() {

    // IMAGE

    double aspect_ratio = 16.0 / 9.0;
    int image_width = 400;
    
    // calculate image height and width and make sure that it's at least 1
    int image_height = int(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    // CAMERA

    double focal_length = 1.0;
    // Viewport widths less than one are ok since they are real valued
    double viewport_height = 2.0;
    // Why not use aspect ratio here? it's because value set to aspect_ratio is the ideal ratio,
    // it may not be the actual ratio between image_width and image_height. What?
    // Meaning, we have two addition constraints, first is that the height is rounded to the nearest integer
    // and second is that height is not allowed to be less than 1.
    double viewport_width = viewport_height * (double(image_width) / image_height);
    point3 camera_center = point3(0, 0, 0);

    // Calculate the vectors across horizontal and down the vertical viewport edges
    // V_u -> (thus V_v points down)
    //  -----------
    // |           |  
    // |           |
    // |           |
    // |           |
    //  -----------

    // Calculation error, viewport_height -> viewport_width
    vec3 viewport_u = vec3(viewport_width, 0, 0);
    vec3 viewport_v = vec3(0, -viewport_height, 0);

    // Horizontal and vertical delta vectors from pixel to pixel
    vec3 pixel_delta_u = viewport_u / image_width;
    vec3 pixel_delta_v = viewport_v / image_height;

    // Calculate location of upper left pixel
    point3 viewport_upper_left = camera_center - vec3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
    point3 pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    // RENDER
       
    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    for(int j = 0; j < image_height; j++){
        std::clog << "\rScanLines remaining: " << (image_height - j) << " " << std::flush;
        for(int i = 0; i < image_width; i++){
            point3 pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
            point3 ray_direction = pixel_center - camera_center;
            ray r(camera_center, ray_direction);

            color pixel_color = ray_color(r);
            write_color(std::cout, pixel_color);
        }
    }
    std::clog << "\rDone.\n";
}