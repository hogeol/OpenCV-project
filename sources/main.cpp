/*
***Important***
Project -> properties -> c++ language standard -> std ISO C++17 Standard (/std:c++17)
*/

//STL
#include <iostream>
#include <string>
#include <filesystem>
//OPENCV
#include <opencv2/opencv.hpp>
//LOCAL
#include "imageProcessing.hpp"

#define IMG_FOLDER_PATH "../test_image/"
#define OWH_IMG "owh.png"
#define BOOK_TEXT_IMG "book_text.jpg"
#define CUBE_IMG "cube_image.jpg"

int main(int argc, char** argv)
{
  ImageProcessing::imageProcessing image_processing_class;
  std::filesystem::path img_folder_path{ IMG_FOLDER_PATH };
  std::string img_folder = std::filesystem::absolute(img_folder_path).string();

  std::string owh_path = img_folder + OWH_IMG;
  std::string book_text_path = img_folder + BOOK_TEXT_IMG;
  std::string cube_img_path = img_folder + CUBE_IMG;

  cv::Mat owh_img = cv::imread(owh_path, 1);
  if (owh_img.empty()) {
    std::cerr << "\nERROR! owh.png file is missing\n";
    return -1;
  }
  cv::Mat book_text_img = cv::imread(book_text_path, 1);
  if (book_text_img.empty()) {
    std::cerr << "\nERROR! book_text.jpg is missing\n";
    return -1;
  }
  cv::Mat cube_img = cv::imread(cube_img_path, 1);
  if (cube_img.empty()) {
    std::cerr << "\nERROR! cube_image.jpg is missing\n";
    return -1;
  }
  cv::Size img_size{ 640, 480 };
  image_processing_class.init(img_size);

  ////1. Morphological Filter
  image_processing_class.pj1Init(owh_img, book_text_img);
  ////1_1.
  //cv::Mat rm_noise_img;
  //image_processing_class.removeNoise(rm_noise_img);
  //cv::imshow(OWH_IMG, image_processing_class.getNoiseImage());
  //cv::imshow("remove_noise_owh", rm_noise_img);

  ////1_2.
  //cv::Mat find_text_img;
  //int cnt = 0;
  //image_processing_class.findText(find_text_img);

  ////cv::imshow("find text", find_text_img);
  //cv::imshow(BOOK_TEXT_IMG, image_processing_class.getTextImage());
  
  //2. Color Processing
  image_processing_class.pj2Init(cube_img);
  ////2_1.
  //cv::Mat after_color_division_b;
  //cv::Mat after_color_division_g;
  //cv::Mat after_color_division_r;
  //image_processing_class.splitBGR(after_color_division_b, after_color_division_g, after_color_division_r);
  //cv::imshow(CUBE_IMG, image_processing_class.getCubeImage());
  //cv::imshow("cube_r", after_color_division_r);
  //cv::imshow("cube_g", after_color_division_g);
  //cv::imshow("cube_b", after_color_division_b);

  ////2_2.
  //cv::Mat h_processing_image_plus;
  //image_processing_class.hsvProcessing(h_processing_image_plus, "h_plus");
  //cv::Mat h_processing_image_minus;
  //image_processing_class.hsvProcessing(h_processing_image_minus, "h_minus");
  //cv::Mat s_processing_image_plus;
  //image_processing_class.hsvProcessing(s_processing_image_plus, "s_plus");
  //cv::Mat s_processing_image_minus;
  //image_processing_class.hsvProcessing(s_processing_image_minus, "s_minus");
  //cv::imshow(CUBE_IMG, image_processing_class.getCubeImage());
  //cv::imshow("after h plus", h_processing_image_plus);
  //cv::imshow("after h minus", h_processing_image_minus);
  //cv::imshow("after s plus", h_processing_image_plus);
  //cv::imshow("after s minus", h_processing_image_plus);

  ////3. Frequency Domain Processing
  //cv::Mat freq_image;
  //int filter_select = 0;
  //std::string filter_string{};
  //std::cout << "\nSelect filter\n\n1. rectangle\n2. radius\n\n>>";
  //std::cin.clear();
  //std::cin >> filter_select;
  //std::cin.clear();
  //switch (filter_select) {
  //case 1: {
  //  filter_string = "rect";
  //  cv::Size rect_size{ 0, 0 };
  //  std::cout << "\nInput rectangle size\nwidth >> ";
  //  std::cin.clear();
  //  std::cin >> rect_size.width;
  //  std::cin.clear();
  //  std::cout << "height >> ";
  //  std::cin >> rect_size.height;
  //  std::cin.clear();
  //  image_processing_class.pj3Init(cube_img, rect_size);
  //  image_processing_class.fourierTransform(filter_string, freq_image);
  //  cv::imshow("origin image", image_processing_class.getFreqImage());
  //  cv::imshow("rect filter image", freq_image);
  //  break;
  //}
  //case 2: {
  //  filter_string = "radius";
  //  double filter_radius{ 0.0 };
  //  std::cout << "\nInput circle radius >> ";
  //  std::cin >> filter_radius;
  //  std::cin.clear();
  //  image_processing_class.pj3Init(cube_img, filter_radius);
  //  image_processing_class.fourierTransform(filter_string, freq_image);
  //  cv::imshow("origin image", image_processing_class.getFreqImage());
  //  cv::imshow("radius filter image", freq_image);
  //  break;
  //}
  //default: {
  //  break;
  //}
  //}

  cv::waitKey(0);
  return 0;
}