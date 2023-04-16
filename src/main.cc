#include "rtutils.h"
#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"

#include <iostream>
#include <chrono>
#include <thread>
#include <execution>




void update_progress_bar(int lines, int linesPrinted){
    float progress = float(linesPrinted) / lines;
    int barWidth = 70;

    if(lines == linesPrinted){
        // Clears the current progress bar
        for(int i = 0; i < barWidth + 7; i++){
            std::cerr << ' ';
        }
        std::cerr << '\r';
        //Prints a done message in place
        std::cerr << "[ DONE! ] 100 %\n";
        std::cerr.flush();
        return;
    }

    std::cerr << "[";
    int pos = barWidth * progress;
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) std::cerr << "=";
        else if (i == pos) std::cerr << ">";
        else std::cerr << " ";
    }
    std::cerr << "] " << int(progress * 100.0) << " %\r";
    std::cerr.flush();
}



color ray_color(const ray& r, const hittable& world, int depth){
    hit_record rec;
    if(depth <= 0){
        return color(0,0,0);
    }
    if(world.hit(r,0.001,infinity,rec)){
        ray scattered;
        color attenuation;
        if(rec.mat_ptr->scatter(r, rec, attenuation, scattered)){
            return attenuation * ray_color(scattered,world,depth-1);
        }
        return color(0,0,0);
    }
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5*(unit_direction.y() + 1.0);
    return (1.0-t)*color(1,1,1) + t * color(0.5,0.7,1.0);
}


int main(){

    // Image Parameters
    const double aspect_ratio = 16.0 / 9.0;
    const int image_width = 1920;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const bool antialiasing = true;
    const int samples_per_pixel = antialiasing ? 32 : 1;
    const int max_depth = 20;
    

    // World Setup
    hittable_list world;
    auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = make_shared<metal>(color(0.7, 0.3, 0.3),0.0);
    auto material_left   = make_shared<metal>(color(0.8, 0.8, 0.8), 0.05);
    auto material_right  = make_shared<metal>(color(0.8, 0.6, 0.2), 0.1);

    world.add(make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<sphere>(point3( 0.0,    0.0, -1.0),   0.5, material_center));
    world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.25, material_left));
    world.add(make_shared<sphere>(point3( 1.0,    0.0, -1.0),   0.5, material_right));

    // Camera Setup
    camera cam;

    // Render

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height-1; j >= 0; --j) {
        // Writing pixels to screen
        for (int i = 0; i < image_width; ++i) {
            color pixel_color(0,0,0);
            for(int s = 0; s < samples_per_pixel; s++){
                auto u = (i + random_double()) / (image_width - 1);
                auto v = (j + random_double()) / (image_height - 1);
                ray r = cam.get_ray(u,v);
                pixel_color += ray_color(r,world,max_depth);

            }
            write_color(std::cout,pixel_color, samples_per_pixel);

        }

        // Draws a progress bar based on the number of lines left to print
        update_progress_bar(image_height, image_height - j);
    }


}

