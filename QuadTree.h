//
// Created by Juan Salazar on 15/12/22.
//

#ifndef QUADTREERGB_QUADTREE_H
#define QUADTREERGB_QUADTREE_H

#include <iostream>
#include <vector>
#include <list>

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



private:

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
        bool equal_ = true;
        Pixel test_ = grid[0][0];
        for (int q = 0; q < size; q++) {
            for (int p = 0; p < size; p++) {
                if (test_ != grid[q][p]){
                    std::vector<std::vector<std::vector<Pixel>>> splitGrid = split_grid(grid);
                    auto *node = new QuadTreeNode(false, {0, 0, 0, 0});
                    for (int i = 0; i < 4; ++i) {
                        node->children_[i] = buildQuadTree(splitGrid[i], 0, 0, size / 2);
                    }
                    return node;
                };
            }
        }
        Pixel median_pixel_ = promPixel(size, grid);
        return new QuadTreeNode(true, median_pixel_);
    }

public:

    QuadTree() {
        root_ = QuadTreeNode();
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

    void printGrid() {
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

    const std::vector<std::vector<std::vector<int>>> &getMainMatrix() {
        this->printGrid();
        return main_matrix_;
    }
};


#endif //QUADTREERGB_QUADTREE_H
