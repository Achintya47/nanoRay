#include <iostream>
#include "util/color.h"
#include "util/vec3.h"
#include "util/ray.h"

#include <iostream>

color ray_color(const ray& r) {
    return color(0, 0, 0);
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
    vec3 viewport_u = vec3(viewport_height, 0, 0);
    vec3 viewport_v = vec3(0, -viewport_height, 0);

    // Horizontal and vertical delta vectors from pixel to pixel
    vec3 pixel_delta_u = viewport_u / image_width;
    vec3 pixel_delta_v = viewport_v / image_height;

    // Calculate location of upper left pixel
    point3 viewport_upper_left = camera_center - vec3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
    point3 pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    // RENDER
       
    std::cout << "P3\n" << image_height << " " << image_height << "\n255\n";

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