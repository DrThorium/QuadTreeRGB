//
// Created by Juan Salazar on 15/12/22.
//

#ifndef QUADTREERGB_QUADTREE_H
#define QUADTREERGB_QUADTREE_H

#include <iostream>
#include <vector>
#include <list>
#include "lodepng.cpp"

int tolerance_;

class QuadTree {
private:

    void setTolerance(int tol_){
        tolerance_ = tol_;
    }

    struct Pixel {

        int R_{}, G_{}, B_{}, A_{};

        Pixel &operator=(const Pixel &cref_) = default;

        bool operator!=(const Pixel &cref_) const {
            int total_difference = 0;
            total_difference += std::abs((R_ - cref_.R_));
            total_difference += std::abs((G_ - cref_.G_));
            total_difference += std::abs((B_ - cref_.B_));
            total_difference += std::abs((A_ - cref_.A_));
            return total_difference > tolerance_;
        }
    };

    struct QuadTreeNode {
        bool isLeaf_;
        Pixel pixel_information_{};
        QuadTreeNode *children_[4]{};

        QuadTreeNode() {
            for (auto &i: children_) {
                i = nullptr;
            }
            isLeaf_ = false;
            pixel_information_ = {0, 0, 0, 0};
        }

        QuadTreeNode(bool isLeaf, Pixel pixel) : isLeaf_(isLeaf), pixel_information_(pixel) {
            for (auto &i: children_) {
                i = nullptr;
            }
        }
    };

    QuadTreeNode root_;
    int size_{};
    std::vector<std::vector<std::vector<int>>> main_matrix_;

    static std::vector<std::vector<std::vector<Pixel>>> split_grid(const std::vector<std::vector<Pixel>> &grid) {
        int m = static_cast<int>(grid.size());
        int n = static_cast<int>(grid[0].size());
        // Create a vector of 4 sub-grids with dimensions m/2 x n/2
        std::vector<std::vector<std::vector<Pixel>>> grids_vec(4, std::vector<std::vector<Pixel>>(m / 2,
                                                                                                  std::vector<Pixel>(
                                                                                                          n / 2)));
        // Iterate over the original grid and populate the subgrids
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                int subgrid_index = 2 * (i / (m / 2)) + (j / (n / 2));
                grids_vec[subgrid_index][i % (m / 2)][j % (n / 2)] = grid[i][j];
            }
        }
        return grids_vec;
    }

    Pixel promPixel(int size, std::vector<std::vector<Pixel>> &grid){
        Pixel return_pixel;
        std::vector<int> prom_vec(4, 0);
        for (int i = 0; i < size; i++){
            for (int j = 0; j < size; j++){
                prom_vec = {prom_vec[0]+grid[i][j].R_, prom_vec[1]+grid[i][j].G_, prom_vec[2]+grid[i][j].B_, prom_vec[3]+grid[i][j].A_};
            }
        }
        for (int i = 0; i < 4; i++){
            prom_vec[i] = prom_vec[i] / (size*size);
        }
        return_pixel = {prom_vec[0], prom_vec[1], prom_vec[2], prom_vec[3]};
        return return_pixel;
    }

    QuadTreeNode *buildQuadTree(std::vector<std::vector<Pixel>> &grid, int x, int y, int size) {
        Pixel median_pixel_ = promPixel(size, grid);
        for (int q = 0; q < size; q++) {
            for (int p = 0; p < size; p++) {
                if (median_pixel_ != grid[q][p]){
                    std::vector<std::vector<std::vector<Pixel>>> splitGrid = split_grid(grid);
                    auto *node = new QuadTreeNode(false, {0, 0, 0, 0});
                    for (int i = 0; i < 4; ++i) {
                        node->children_[i] = buildQuadTree(splitGrid[i], 0, 0, size / 2);
                    }
                    return node;
                };
            }
        }
        return new QuadTreeNode(true, median_pixel_);
    }

    explicit QuadTree(const std::vector<std::vector<std::vector<int>>> &input_grid, int tol_ = 0) {
        setTolerance(tol_);
        std::vector<std::vector<Pixel>> grid_built(input_grid.size(),
                                                   std::vector<Pixel>(input_grid[0].size(), Pixel()));
        int a = -1;
        for (auto &x: input_grid) {
            a++;
            int b = -1;
            for (auto &y: x) {
                b++;
                Pixel Pixel_ = {y[0], y[1], y[2], y[3]};
                grid_built[a][b] = Pixel_;
            }
        }
        size_ = static_cast<int>(grid_built.size());
        root_ = *buildQuadTree(grid_built, 0, 0, size_);
    }

    void buildGrid() {
        main_matrix_.resize(size_, std::vector<std::vector<int>>(size_, std::vector<int>(4, 0)));
        printGrid(&root_, 0, 0, size_);
    }

    void printGrid(QuadTreeNode *Node, int i, int j, int size) {
        if (Node->isLeaf_) {
            for (int k = i; k < i + size; k++) {
                for (int l = j; l < j + size; l++) {
                    main_matrix_[k][l][0] = Node->pixel_information_.R_;
                    main_matrix_[k][l][1] = Node->pixel_information_.G_;
                    main_matrix_[k][l][2] = Node->pixel_information_.B_;
                    main_matrix_[k][l][3] = Node->pixel_information_.A_;
                }
            }
        } else {
            printGrid(Node->children_[0], i, j, size / 2);
            printGrid(Node->children_[1], i, j + (size / 2), size / 2);
            printGrid(Node->children_[2], i + (size / 2), j, size / 2);
            printGrid(Node->children_[3], i + (size / 2), j + (size / 2), size / 2);
        }
    }

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


    void ensurePNGExtension(std::string& fileName) {
        if (fileName.size() < 4 || fileName.substr(fileName.size() - 4) != ".png") {
            fileName += ".png";
        }
    }

    const std::vector<std::vector<std::vector<int>>> &getMainMatrix() {
        this->buildGrid();
        return main_matrix_;
    }

public:

    QuadTree(){
        root_ = QuadTreeNode();
    };

    void compress(std::string filename_, int tol_){
        ensurePNGExtension(filename_);
        std::vector<std::vector<std::vector<int>>> input_grid_ = readPNGImage(filename_);
        QuadTree qt_ = QuadTree(input_grid_, tol_);
        filename_.insert(filename_.size() - 4, "-EXIT");
        saveImage(qt_.getMainMatrix(), filename_);
        main_matrix_.clear();
    }


};


#endif //QUADTREERGB_QUADTREE_H
