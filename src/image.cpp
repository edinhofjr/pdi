#include "image.h"
#include "stb_image.h"
#include "stb_image_write.h"
#include <algorithm> // std::fill
#include <cmath>
#include <iostream>
#include <string>

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

std::string Image::data() const {
    return "Width: " + std::to_string(w) +
           ", Height: " + std::to_string(h) +
           ", Channels: " + std::to_string(channels);
}


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

