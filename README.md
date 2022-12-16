# QuadTreeRGB
A Quad Tree Implementation to compress a PNG file keeping the original RGB information.


### QuadTree for PNG Compression
This repository contains a QuadTree implementation for compressing .png files with a tolerance parameter. The QuadTree is used to store and manipulate the RGBA pixel data of the .png image.

A QuadTree is a data structure that is used to store and manipulate large amounts of data, such as the pixel data of an image. It works by recursively dividing the data into smaller quadrants until each quadrant contains a small enough number of data points.

In the context of an RGBA QuadTree implementation for compressing PNG files, each node in the QuadTree represents a block of pixels in the image, and each child node represents a smaller block of pixels within the parent node's block. The QuadTree is constructed by recursively dividing the image into smaller quadrants until each quadrant meets a certain criterion, such as having a uniform color or a small enough number of distinct colors.

To compress the PNG file, the QuadTree implementation compares the pixel data of each block to a tolerance value. If the pixel data meets the tolerance criteria (e.g. the difference in color values between the pixels is within a certain range), the block is considered uniform and can be represented by a single pixel value, rather than storing the pixel data for each individual pixel in the block. This reduces the amount of data that needs to be stored, leading to a smaller file size.

When the image is decompressed, the QuadTree is used to reconstruct the original pixel data by using the stored pixel values and the structure of the QuadTree. This allows the image to be reconstructed with minimal loss of information, as long as the tolerance value used for compression is not too high.

Tolerance is a user provided parameter, and the criterium is calculated checking every pixel in the quad_tree node vs the average pixel (The average value for each RGBA value of all matrix values).






Here's a example (tolerance 64):

#### Original image:
![image](https://user-images.githubusercontent.com/119984041/208050918-316d5ae3-9cb6-4685-a070-80a6faae6db5.png)


#### Result image:
![image](https://user-images.githubusercontent.com/119984041/208050822-079b4c10-834c-4d9d-852b-9e36f45a0492.png)


### Features
Compress .png files using a tolerance parameter to specify the level of compression
Store and manipulate RGBA pixel data using a QuadTree data structure
Support for retrieving and updating individual pixels or blocks of pixels in the image
Installation
To use the QuadTree for PNG compression, clone this repository and include the quadtree.h file in your project.
```
git clone https://github.com/DrThorium/QuadTreeRGB.git
```
The code uses the lodepng to load and export the png files, here's the repo:  
https://github.com/lvandeve/lodepng
### Usage
Here is an example of how to use the QuadTree to compress a .png file with a tolerance of 10:
```
#include "quadtree.h"

int main() {
  QuadTree tree;
  tree.Compress("image.png", 10);
  return 0;
}
```
Tolerance is the sum of all differences between to pixels, so Tolerance = 1.R - 2.R + 1.G - 2.G ... 

### Contributing
We welcome contributions to this project. If you have an idea for a new feature or have found a bug, please open an issue to discuss it.  
Now the code only acepts png_size % 4 == 0 squared images, like 512x512.

### License
This project is licensed under the MIT License. See the LICENSE file for more details.

