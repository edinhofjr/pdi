#include "image.h"
#include <iostream>
#include <vector>
#include <algorithm>

struct point {
    int x;
    int y;
};

struct object {
    struct point start;
    int size;
};

int dist2(const object& o) {
    return o.start.x*o.start.x + o.start.y*o.start.y;
}

bool cmp_object(const object& a, const object& b) {
    return dist2(a) < dist2(b);
}


void print_pixel(Image &img, int x, int y) {
    std::cout << "Pixel at (" << x << ", " << y << "): ";
    int index = (y * img.w + x) * img.channels;
    
    for (int c = 0; c < img.channels; c++) {
        std::cout << static_cast<int>(img.pixels[index + c]);
        if (c < img.channels - 1)
            std::cout << ", ";
    }
    std::cout << std::endl;
}

bool is_white_u8(unsigned char v) {
    return v == 255;
}

bool is_black_u8(unsigned char v) {
    return v == 0;
}

unsigned char* Image::get_pixel(int x, int y) {
    int index = (y * w + x) * channels;
    return &pixels[index];
}

int flood_paint(Image* img, int x, int y) {
    // Fora dos limites
    if (x < 0 || x >= img->w || y < 0 || y >= img->h)
        return 0;

    unsigned char* pixel = img->get_pixel(x, y);

    // Ignorar se não for preto
    if (*pixel != 0)
        return 0;

    // Pintar este pixel
    *pixel = 127;

    int count = 1; // conta este pixel

    // Quatro direções (4-conectividade)
    count += flood_paint(img, x + 1, y);
    count += flood_paint(img, x - 1, y);
    count += flood_paint(img, x, y + 1);
    count += flood_paint(img, x, y - 1);

    return count;
}

void challenge() {
    Image img("/home/edinho/Documents/PDI/images/moedas.png");
    img = *img.gray_scale()->threshold(254);
    
    std::vector<object> obj1;

    for (int i = 0; i < img.w; i++) {
        for (int j = 0; j < img.h / 3; j++) {
            auto pixel = img.get_pixel(i, j);
            if (is_white_u8(*pixel)) continue;
            
            int painted_pixels =  flood_paint(&img, i, j);
            if (painted_pixels > 100) obj1.push_back({{i, j}, painted_pixels});
        }
    }

    std::sort(obj1.begin(), obj1.end(), cmp_object);

    std::vector<object> obj2;
    for (int i = 0; i < img.w; i++) {
        for (int j = img.h / 3; j < img.h; j++) {
            auto pixel = img.get_pixel(i, j);
            if (is_white_u8(*pixel)) continue;
            
            int painted_pixels =  flood_paint(&img, i, j);
            if (painted_pixels > 100) obj2.push_back({{i, j}, painted_pixels});
        }
    }

    std::vector<object> objects;
    objects.insert(objects.end(), obj1.begin(), obj1.end());
    objects.insert(objects.end(), obj2.begin(), obj2.end());

    for (size_t i = 0; i < objects.size(); i++) {
        std::cout << "Objeto " << i + 1 << ": Início em (" 
                  << objects[i].start.x << ", " << objects[i].start.y 
                  << "), Tamanho: " << objects[i].size << " pixels\n";
    }

    img.save("/home/edinho/Documents/PDI/images/moedas_out.png");

}