#ifndef IMAGE_H
#define IMAGE_H

#include <string>
class Image
{
public:
    Image(const char* filename);
    Image();
    ~Image();

    void save(const char* filename) const;

    void save_png(const char *filename) const;

    std::string data() const;

    Image *translate(int dx, int dy);
    Image* rotate(float); 
    Image* mirror_h();
    Image* mirror_v();

    Image *resize(float scale);

    Image *brightness(int);
    Image* contrast(float);
    Image *dilate();
    Image *erosion();
    Image *opening();
    Image *closing();
    Image *threshold(int threshold);
    Image* gray_scale();

    Image* high_pass();
    Image* low_pass();

    
    int w;
    int h;
    int channels;
    unsigned char* pixels;
};

#endif // IMAGE_H
