#include "image.h"
#include "stb_image.h"
#include "stb_image_write.h"
#include <algorithm> // std::fill
#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <cstdint>

Image::Image(const char* filename) {
    pixels = stbi_load(filename, &w, &h, &channels, 0);
}

Image::Image() : w(0), h(0), channels(0), pixels(nullptr) {}

Image::~Image() {
    if (pixels) stbi_image_free(pixels);
}

void Image::save(const char *filename) const {
    stbi_write_jpg(filename, w, h, channels, pixels, 100);
}

void Image::save_png(const char *filename) const {
    stbi_write_png(filename, w, h, channels, pixels, w * channels);
}

std::string Image::data() const {
    return "Width: " + std::to_string(w) +
           ", Height: " + std::to_string(h) +
           ", Channels: " + std::to_string(channels);
}

//translatar
Image* Image::translate(int dx, int dy) {
    Image* output = new Image();
    output->w = w;
    output->h = h;
    output->channels = channels;

    int totalSize = w * h * channels;
    output->pixels = new unsigned char[totalSize];
    std::fill(output->pixels, output->pixels + totalSize, 0);

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            int newX = x + dx;
            int newY = y + dy;

            if (newX >= 0 && newX < w && newY >= 0 && newY < h) {
                int oldIndex = (y * w + x) * channels;
                int newIndex = (newY * w + newX) * channels;

                for (int c = 0; c < channels; c++)
                    output->pixels[newIndex + c] = pixels[oldIndex + c];
            }
        }
    }

    return output;
}

//rotacionar
Image* Image::rotate(float angle_deg) {
    if (!pixels || w <= 0 || h <= 0)
        return new Image();

    float angle = angle_deg * M_PI / 180.0f;
    float cosA = std::cos(angle);
    float sinA = std::sin(angle);

    float cx = w / 2.0f;
    float cy = h / 2.0f;

    int newW = std::abs(w * cosA) + std::abs(h * sinA);
    int newH = std::abs(w * sinA) + std::abs(h * cosA);

    Image* output = new Image();
    output->w = newW;
    output->h = newH;
    output->channels = channels;

    int totalSize = newW * newH * channels;
    output->pixels = new unsigned char[totalSize];
    std::fill(output->pixels, output->pixels + totalSize, 0);

    float ncx = newW / 2.0f;
    float ncy = newH / 2.0f;

    for (int y = 0; y < newH; y++) {
        for (int x = 0; x < newW; x++) {
            float oldX = (x - ncx) * cosA + (y - ncy) * sinA + cx;
            float oldY = -(x - ncx) * sinA + (y - ncy) * cosA + cy;

            int ox = (int)std::round(oldX);
            int oy = (int)std::round(oldY);

            if (ox >= 0 && ox < w && oy >= 0 && oy < h) {
                int oldIndex = (oy * w + ox) * channels;
                int newIndex = (y * newW + x) * channels;
                for (int c = 0; c < channels; c++)
                    output->pixels[newIndex + c] = pixels[oldIndex + c];
            }
        }
    }

    return output;
}

//espelhar horizontalmente
Image* Image::mirror_h() {
    Image* output = new Image();
    output->w = w;
    output->h = h;
    output->channels = channels;

    int totalSize = w * h * channels;
    output->pixels = new unsigned char[totalSize];

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            int oldIndex = (y * w + x) * channels;
            int newIndex = (y * w + (w - 1 - x)) * channels;

            for (int c = 0; c < channels; c++)
                output->pixels[newIndex + c] = pixels[oldIndex + c];
        }
    }

    return output;
}
//espelhar verticalmente
Image* Image::mirror_v() {
    Image* output = new Image();
    output->w = w;
    output->h = h;
    output->channels = channels;

    int totalSize = w * h * channels;
    output->pixels = new unsigned char[totalSize];

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            int oldIndex = (y * w + x) * channels;
            int newIndex = ((h - 1 - y) * w + x) * channels;

            for (int c = 0; c < channels; c++)
                output->pixels[newIndex + c] = pixels[oldIndex + c];
        }
    }

    return output;
}

//aumentar e diminuir
Image* Image::resize(float scale)
{
    if (scale <= 0) return nullptr;

    int newW = (int)(w * scale);
    int newH = (int)(h * scale);

    Image* out = new Image();
    out->w = newW;
    out->h = newH;
    out->channels = this->channels;
    out->pixels = new unsigned char[newW * newH * channels];

    for (int y = 0; y < newH; y++) {
        float srcY = y / scale;

        int y0 = (int)srcY;
        int y1 = std::min(y0 + 1, h - 1);

        float fy = srcY - y0;

        for (int x = 0; x < newW; x++) {
            float srcX = x / scale;

            int x0 = (int)srcX;
            int x1 = std::min(x0 + 1, w - 1);

            float fx = srcX - x0;

            int idxOut = (y * newW + x) * channels;

            for (int c = 0; c < channels; c++) {
                float p00 = pixels[(y0 * w + x0) * channels + c];
                float p10 = pixels[(y0 * w + x1) * channels + c];
                float p01 = pixels[(y1 * w + x0) * channels + c];
                float p11 = pixels[(y1 * w + x1) * channels + c];

                float p0 = p00 * (1 - fx) + p10 * fx;
                float p1 = p01 * (1 - fx) + p11 * fx;
                float p = p0 * (1 - fy) + p1 * fy;

                out->pixels[idxOut + c] = (unsigned char)p;
            }
        }
    }

    return out;
}

//brilho
Image* Image::brightness(int factor) {
    Image* output = new Image();
    output->w = w;
    output->h = h;
    output->channels = channels;

    int totalSize = w * h * channels;
    output->pixels = new unsigned char[totalSize];

    for (int i = 0; i < totalSize; i++) {
        int value = static_cast<int>(pixels[i] * factor);
        output->pixels[i] = static_cast<unsigned char>(std::clamp(value, 0, 255));
    }

    return output;
}
//contraste
Image* Image::contrast(float factor) {
    Image* output = new Image();
    output->w = w;
    output->h = h;
    output->channels = channels;

    int totalSize = w * h * channels;
    output->pixels = new unsigned char[totalSize];

    for (int i = 0; i < totalSize; i++) {
        int value = static_cast<int>(((pixels[i] - 128) * factor) + 128);
        output->pixels[i] = static_cast<unsigned char>(std::clamp(value, 0, 255));
    }

    return output;
}
//grayscale
Image* Image::gray_scale() {
    Image* output = new Image();
    output->w = w;
    output->h = h;
    output->channels = 1;

    int totalSize = w * h;
    output->pixels = new unsigned char[totalSize];

    for (int i = 0; i < w * h; i++) {
        int r = pixels[i * channels];
        int g = pixels[i * channels + 1];
        int b = pixels[i * channels + 2];
        unsigned char gray = static_cast<unsigned char>(0.299 * r + 0.587 * g + 0.114 * b);
        output->pixels[i] = gray;
    }

    return output;
}
//passa alta
Image* Image::high_pass() {
    Image* out = new Image();
    out->w = w;
    out->h = h;
    out->channels = channels;
    out->pixels = new unsigned char[w * h * channels];

    int kernel[3][3] = {
        { 0, -1,  0},
        {-1,  4, -1},
        { 0, -1,  0}
    };

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            for (int c = 0; c < channels; c++) {
                int sum = 0;

                for (int ky = -1; ky <= 1; ky++) {
                    for (int kx = -1; kx <= 1; kx++) {
                        int px = std::min(std::max(x + kx, 0), w - 1);
                        int py = std::min(std::max(y + ky, 0), h - 1);

                        sum += pixels[(py * w + px) * channels + c] *
                               kernel[ky + 1][kx + 1];
                    }
                }

                // normaliza para 0–255
                int value = sum + 128;      // evita ficar negativo
                value = std::max(0, std::min(255, value));

                out->pixels[(y * w + x) * channels + c] = value;
            }
        }
    }

    return out;
}
//passa baixa
Image* Image::low_pass() {
    Image* out = new Image();
    out->w = w;
    out->h = h;
    out->channels = channels;
    out->pixels = new unsigned char[w * h * channels];

    int kernel[3][3] = {
        {1, 1, 1},
        {1, 1, 1},
        {1, 1, 1}
    };
    const int factor = 9;

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            for (int c = 0; c < channels; c++) {
                int sum = 0;

                for (int ky = -1; ky <= 1; ky++) {
                    for (int kx = -1; kx <= 1; kx++) {
                        int px = std::min(std::max(x + kx, 0), w - 1);
                        int py = std::min(std::max(y + ky, 0), h - 1);

                        sum += pixels[(py * w + px) * channels + c] *
                               kernel[ky + 1][kx + 1];
                    }
                }

                out->pixels[(y * w + x) * channels + c] = sum / factor;
            }
        }
    }

    return out;
}

Image* Image::threshold(int threshold) {
    Image* output = new Image();
    output->w = w;
    output->h = h;
    output->channels = channels;

    int totalSize = w * h * channels;
    output->pixels = new unsigned char[totalSize];

    for (int i = 0; i < totalSize; i++) {
        output->pixels[i] = (pixels[i] >= threshold) ? 255 : 0;
    }

    return output;
}

inline bool is_white_u8(uint8_t p) { return p > 127; }
inline bool is_black_u8(uint8_t p) { return p <= 127; }
//dilatação
Image* Image::dilate()
{
    if (channels != 1) {
        std::cerr << "dilate() requer imagem binária 1 canal.\n";
        return this;
    }

    Image* out = new Image();
    out->w = w;
    out->h = h;
    out->channels = 1;
    out->pixels = new unsigned char[w * h];

    auto get = [&](int x, int y) {
        return pixels[y * w + x];
    };

    for (int y = 1; y < h - 1; y++) {
        for (int x = 1; x < w - 1; x++) {

            bool dilatePixel =
                is_black_u8(get(x, y)) ||
                is_black_u8(get(x + 1, y)) ||
                is_black_u8(get(x - 1, y)) ||
                is_black_u8(get(x, y + 1)) ||
                is_black_u8(get(x, y - 1));

            out->pixels[y * w + x] = dilatePixel ? 0 : 255;
        }
    }

    return out;
}


Image* Image::erosion() {
    Image* out = new Image();
    out->w = w;
    out->h = h;
    out->channels = channels;
    out->pixels = new unsigned char[w * h * channels];

    for (int i = 0; i < w * h * channels; i++)
        out->pixels[i] = pixels[i];

    auto get = [&](int x, int y) {
        return pixels[(y * w + x) * channels];
    };

    auto set = [&](Image* img, int x, int y, unsigned char v) {
        img->pixels[(y * w + x) * channels] = v;
    };

    for (int y = 1; y < h - 1; y++) {
        for (int x = 1; x < w - 1; x++) {
            bool erode = false;

            for (int j = -1; j <= 1 && !erode; j++) {
                for (int i = -1; i <= 1 && !erode; i++) {
                    if (get(x + i, y + j) == 255)
                        erode = true;
                }
            }

            if (erode)
                set(out, x, y, 255);
            else
                set(out, x, y, 0);
        }
    }

    return out;
}
//abertura
Image* Image::opening()
{
    Image* e = this->erosion();
    Image* d = e->dilate();
    delete e;
    return d;
}
//fechamento
Image* Image::closing()
{
    Image* d = this->dilate();
    Image* e = d->erosion();
    delete d;
    return e;
}