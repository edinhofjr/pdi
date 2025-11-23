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

    std::string data() const;

    Image *translate(int dx, int dy);
    Image* rotate(float); 
    Image* mirror_h();
    Image* mirror_v();

    Image* brightness(int);
    Image* contrast(float);
    Image *threshold(int threshold);
    Image *gray_scale();
    Image* tunning();

    int w;
    int h;
    int channels;
    unsigned char* pixels;
};

#endif // IMAGE_H
