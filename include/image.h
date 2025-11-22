#ifndef IMAGE_H
#define IMAGE_H

class Image
{
public:
    Image(const char* filename);
    Image();
    ~Image();
    Image* translate(int dx, int dy);
    Image* rotate(float); 
    Image* mirror_h();
    Image* mirror_v();


    int w;
    int h;
    int channels;
    unsigned char* pixels;
};

#endif // IMAGE_H
