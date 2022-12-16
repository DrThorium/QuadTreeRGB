#include "QuadTree.h"
#include "lodepng.cpp"
#include <vector>

void saveImage(const std::vector<std::vector<std::vector<int>>> &image, const std::string& filename) {
    // Determine the width and height of the image
    int width = static_cast<int>(image[0].size());
    int height = static_cast<int>(image.size());

    // Allocate memory for the image data
    std::vector<unsigned char> data(width * height * 4);

    // Copy the image data into the data vector
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            data[y * width * 4 + x * 4 + 0] = image[y][x][0];
            data[y * width * 4 + x * 4 + 1] = image[y][x][1];
            data[y * width * 4 + x * 4 + 2] = image[y][x][2];
            data[y * width * 4 + x * 4 + 3] = image[y][x][3];
        }
    }

    // Encode the image data and save it to a file
    unsigned error = lodepng::encode(filename, data, width, height);

    // Check for encoding errors
    if (error) {
        throw std::runtime_error("Error saving PNG image: " + std::to_string(error));
    }
    else{
        std::cout << "Success! ... saved png image at: ./" << filename << std::endl;
    }
}

std::vector<std::vector<std::vector<int>>> readPNGImage(const std::string& filename)
{
    std::vector<unsigned char> image;
    unsigned width, height;
    unsigned error = lodepng::decode(image, width, height, filename);

    if (error)
    {
        throw std::runtime_error("Error loading PNG image: " + std::to_string(error));
    }

    std::vector<std::vector<std::vector<int>>> pixels;
    pixels.resize(height);
    for (unsigned i = 0; i < height; ++i)
    {
        pixels[i].resize(width);
        for (unsigned j = 0; j < width; ++j)
        {
            pixels[i][j].resize(4); // 4 ints per pixel (RGBA)
        }
    }

    for (unsigned y = 0; y < height; ++y)
    {
        for (unsigned x = 0; x < width; ++x)
        {
            const unsigned char* pixel = &image[(y * width + x) * 4];
            pixels[y][x][0] = static_cast<int>(pixel[0]);
            pixels[y][x][1] = static_cast<int>(pixel[1]);
            pixels[y][x][2] = static_cast<int>(pixel[2]);
            pixels[y][x][3] = static_cast<int>(pixel[3]);
        }
    }

    return pixels;
}

int main() {
    std::string strg_;
    int tol;
    std::cout << "Filename ? ... " << std::endl;
    std::cin >> strg_;
    std::cout << std::endl;
    std::cout << "Tolerance?" << std::endl;
    std::cin >> tol;
    std::vector<std::vector<std::vector<int>>> input_grid_ = readPNGImage(strg_+".png");
    QuadTree qt_ = QuadTree(input_grid_, tol);
    std::cout << std::endl;
    saveImage(qt_.getMainMatrix(), strg_+"EXIT.png");
    return 0;
}
