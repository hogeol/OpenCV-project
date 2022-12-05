#include "imageProcessing.hpp"

namespace ImageProcessing {
  void imageProcessing::init(const cv::Size& img_size)
  {
    m_img_size = cv::Size{ 640, 480 };
    m_mouse_flag = false;
  }

  void imageProcessing::pj1Init(const cv::Mat& noise_img, const cv::Mat& text_img)
  {
    m_noise_img = noise_img.clone();
    m_text_img = text_img.clone();
    cv::resize(m_noise_img, m_noise_img, m_img_size, 0.0, 0.0, CV_INTER_NN);
    cv::resize(m_text_img, m_text_img, m_img_size, 0.0, 0.0, CV_INTER_NN);
  }

  void imageProcessing::removeNoise(cv::Mat& img_out)
  {
    cv::Mat noise_image = m_noise_img.clone();
    cv::Size open_kernel_size{ 5, 5 };
    cv::Size close_kernel_size{ 3, 3 };
    cv::Size dilate_kernel_size{ 3, 3 };
    cv::Mat open_kernel = cv::getStructuringElement(cv::MORPH_CROSS, open_kernel_size);
    cv::Mat close_kernel = cv::getStructuringElement(cv::MORPH_CROSS, close_kernel_size);
    cv::Mat dilate_kernel = cv::getStructuringElement(cv::MORPH_RECT, dilate_kernel_size);

    cv::morphologyEx(noise_image, noise_image, cv::MORPH_OPEN, open_kernel);
    cv::morphologyEx(noise_image, noise_image, cv::MORPH_CLOSE, close_kernel);
    cv::dilate(noise_image, img_out, dilate_kernel);
  }

  void imageProcessing::findText(cv::Mat& img_out)
  {
    cv::Mat gray_text_img;
    cv::cvtColor(m_text_img, gray_text_img, CV_BGR2GRAY);
    skeletonization(gray_text_img, gray_text_img);
    m_gray_text_img = gray_text_img.clone();
    cv::imshow("skeleton", gray_text_img);
    cv::setMouseCallback("skeleton", mouseROI, this);
      
    //hitOrMiss(gray_text_img, img_out);
  }

  void imageProcessing::skeletonization(const cv::Mat& gray_img_in, cv::Mat& img_out)
  {
    cv::Mat threshold_img;
    cv::threshold(gray_img_in, threshold_img, 127, 255, cv::THRESH_BINARY);
    cv::Mat skel{ threshold_img.size(), CV_8UC1, cv::Scalar(0) };
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_CROSS, cv::Size{ 3,3 });
    cv::Mat tmp, eroded;
    do {
      cv::erode(threshold_img, eroded, kernel);
      cv::dilate(eroded, tmp, kernel);
      cv::subtract(threshold_img, tmp, tmp);
      cv::bitwise_or(skel, tmp, skel);
      eroded.copyTo(threshold_img);
    } while ((cv::countNonZero(threshold_img) != 0));
    skel.copyTo(img_out);
  }

  void imageProcessing::hitOrMiss(const cv::Mat& img_in, cv::Mat& img_out)
  {
    cv::Mat kernel = (cv::Mat_<int>(4, 3) <<
      1, 0, 0,
      1, 0, 0,
      1, 0, 0,
      1, 0, 0);
    cv::morphologyEx(img_in, img_out, cv::MORPH_HITMISS, kernel);
  }
  void imageProcessing::pj2Init(const cv::Mat& cube_img)
  {
    m_cube_img = cube_img.clone();
    cv::resize(m_cube_img, m_cube_img, m_img_size, 0.0, 0.0, CV_INTER_NN);
  }

  void imageProcessing::splitBGR(cv::Mat& img_b, cv::Mat& img_g, cv::Mat& img_r)
  {
    cv::Mat cube_image = m_cube_img.clone();
    cv::Mat cube_b = cv::Mat::zeros(cube_image.size(), CV_8UC1);
    cv::Mat cube_g = cv::Mat::zeros(cube_image.size(), CV_8UC1);
    cv::Mat cube_r = cv::Mat::zeros(cube_image.size(), CV_8UC1);

    for (int cols_iter = 0; cols_iter < cube_image.cols; cols_iter++) {
      for (int rows_iter = 0; rows_iter < cube_image.rows; rows_iter++) {
        cube_b.at<uchar>(rows_iter, cols_iter) = cube_image.at<cv::Vec3b>(rows_iter, cols_iter)[0];
        cube_g.at<uchar>(rows_iter, cols_iter) = cube_image.at<cv::Vec3b>(rows_iter, cols_iter)[1];
        cube_r.at<uchar>(rows_iter, cols_iter) = cube_image.at<cv::Vec3b>(rows_iter, cols_iter)[2];
      }
    }
    img_b = cube_b;
    img_g = cube_g;
    img_r = cube_r;
  }

  void imageProcessing::hsvProcessing(cv::Mat& image_out, const std::string& type)
  {
    cv::Mat cube_image = m_cube_img.clone();
    cv::cvtColor(cube_image, cube_image, cv::COLOR_BGR2HSV);
    image_out = cube_image.clone();
    if (type == "h_plus") {
      for (int cols_iter = 0; cols_iter < cube_image.cols; cols_iter++) {
        for (int rows_iter = 0; rows_iter < cube_image.rows; rows_iter++) {
          image_out.at<cv::Vec3b>(rows_iter, cols_iter)[0] += 60;
        }
      }
    }
    else if (type == "h_minus") {
      for (int cols_iter = 0; cols_iter < cube_image.cols; cols_iter++) {
        for (int rows_iter = 0; rows_iter < cube_image.rows; rows_iter++) {
          image_out.at<cv::Vec3b>(rows_iter, cols_iter)[0] -= 60;
        }
      }
    }
    else if (type == "s_plus") {
      for (int cols_iter = 0; cols_iter < cube_image.cols; cols_iter++) {
        for (int rows_iter = 0; rows_iter < cube_image.rows; rows_iter++) {
          image_out.at<cv::Vec3b>(rows_iter, cols_iter)[1] += 60;
        }
      }
    }
    else if (type == "s_minus") {
      for (int cols_iter = 0; cols_iter < cube_image.cols; cols_iter++) {
        for (int rows_iter = 0; rows_iter < cube_image.rows; rows_iter++) {
          image_out.at<cv::Vec3b>(rows_iter, cols_iter)[1] -= 60;
        }
      }
    }
  }

  void imageProcessing::pj3Init(const cv::Mat& img, const cv::Size& rectangle_size)
  {
    m_freq_img = img;
    m_filter_rect_size = rectangle_size;
    m_filter_radius = 0.0;
    cv::resize(m_freq_img, m_freq_img, m_img_size, 0.0, 0.0, CV_INTER_NN);
  }

  void imageProcessing::pj3Init(const cv::Mat& img, const double& circle_radius)
  {
    m_freq_img = img;
    m_filter_rect_size = cv::Size2f{ 0.0, 0.0 };
    m_filter_radius = circle_radius;
    cv::resize(m_freq_img, m_freq_img, m_img_size, 0.0, 0.0, CV_INTER_NN);
  }

  void imageProcessing::fourierTransform(const std::string& filter, cv::Mat& img_out)
  {
    cv::Mat freq_img_gray;
    cv::Mat freq_img_float;
    cv::Mat dft_img;
    cv::cvtColor(m_freq_img, freq_img_gray, CV_BGR2GRAY);
    freq_img_gray.convertTo(freq_img_float, CV_32FC1, 1.0 / 255.0);
    cv::dft(freq_img_float, dft_img, cv::DFT_COMPLEX_OUTPUT);
    shuffleDft(dft_img);
    cv::Mat low_pass;
    if (filter == "radius") {
      butterWorthFilter(dft_img.size(), m_filter_radius, low_pass);
    }
    else if (filter == "rect") {
      rectFilter(dft_img.size(), low_pass);
    }
    cv::Mat after_filter;
    cv::multiply(dft_img, low_pass, after_filter);
    cv::Mat dft_img_array[2] = { cv::Mat::zeros(dft_img.size(), CV_32F), cv::Mat::zeros(dft_img.size(), CV_32F) };
    cv::split(after_filter, dft_img_array);
    cv::Mat dft_mag_img;
    cv::magnitude(dft_img_array[0], dft_img_array[1], dft_mag_img);
    dft_mag_img += cv::Scalar::all(1);
    cv::log(dft_mag_img, dft_mag_img);
    cv::normalize(dft_mag_img, dft_mag_img, 0, 1, CV_MINMAX);
    dft_mag_img.copyTo(img_out);
  }

  void imageProcessing::shuffleDft(cv::Mat& img_out)
  {
    int cX = img_out.cols / 2;
    int cY = img_out.rows / 2;
    cv::Mat quad_1(img_out, cv::Rect(0, 0, cX, cY));
    cv::Mat quad_2(img_out, cv::Rect(cX, 0, cX, cY));
    cv::Mat quad_3(img_out, cv::Rect(0, cY, cX, cY));
    cv::Mat quad_4(img_out, cv::Rect(cX, cY, cX, cY));
    cv::Mat out_img_tmp;
    quad_1.copyTo(out_img_tmp);
    quad_4.copyTo(quad_1);
    out_img_tmp.copyTo(quad_4);
    quad_2.copyTo(out_img_tmp);
    quad_3.copyTo(quad_2);
    out_img_tmp.copyTo(quad_3);
  }

  void imageProcessing::rectFilter(const cv::Size& img_size, cv::Mat& filter_out)
  {
    cv::Mat filter_tmp{ img_size, CV_32FC2, cv::Vec2f{0.0, 0.0} };
    cv::Point rect_center{ img_size.width / 2, img_size.height / 2 };
    cv::Rect filter{ cv::Point2i{(img_size.width / 2) - (m_filter_rect_size.width / 2), (img_size.height / 2) - (m_filter_rect_size.height / 2)}, m_filter_rect_size };
    cv::rectangle(filter_tmp, filter, cv::Vec2f{ 1, 1 }, -1);
    filter_out = filter_tmp.clone();
  }

  void imageProcessing::butterWorthFilter(const cv::Size& img_size, const double& filter_radius, cv::Mat& filter_out)
  {
    cv::Mat filter_tmp{ img_size, CV_32F };
    cv::Point filter_center{ filter_tmp.rows / 2, filter_tmp.cols / 2 };
    double radius{ 0.0 };
    double degree{ 2.0 };
    double skirt_d{ filter_radius };
    for (int rows_iter = 0; rows_iter < filter_tmp.rows; rows_iter++) {
      for (int cols_iter = 0; cols_iter < filter_tmp.cols; cols_iter++) {
        radius = (double)sqrt(pow(((double)rows_iter - filter_center.x), 2.0) + pow(((double)cols_iter - filter_center.y), 2.0));
        filter_tmp.at<float>(rows_iter, cols_iter) = (float)(1.0 / (1.0 + pow((radius / skirt_d), (2 * degree))));
      }
    }
    cv::Mat to_merge[]{ filter_tmp, filter_tmp };
    cv::merge(to_merge, 2, filter_out);
  }

  void imageProcessing::mouseROI(int event, int x, int y)
  {
    if (event == CV_EVENT_LBUTTONDOWN) {
      m_rect.s_start_point.x = x;
      m_rect.s_start_point.y = y;
      m_mouse_flag = true;
    }
    else if (event == CV_EVENT_MOUSEMOVE && m_mouse_flag == true) {
      m_rect.s_end_point.x = x;
      m_rect.s_end_point.y = y;
    }
    else if (event == CV_EVENT_LBUTTONUP && m_mouse_flag == true) {
      m_rect.s_end_point.x = x;
      m_rect.s_end_point.y = y;
      m_mouse_flag = false;
      setRectPoint();
      cv::Rect roi_rect{ m_rect.s_start_point.x, m_rect.s_start_point.y, std::abs(m_rect.s_end_point.x - m_rect.s_start_point.x), std::abs(m_rect.s_end_point.y - m_rect.s_start_point.y) };
      cv::Mat roi_image = m_gray_text_img(roi_rect);
      cv::imshow("roi_image", roi_image);
      system("cls");
      std::cout << roi_image;
    }
  }

  void imageProcessing::mouseROI(int event, int x, int y, int flags, void* param)
  {
    imageProcessing* interrupts = reinterpret_cast<imageProcessing*>(param);
    interrupts->mouseROI(event, x, y);
  }

  inline void imageProcessing::setRectPoint()
  {
    cv::Point2i start_point{ SMALL(m_rect.s_start_point.x, m_rect.s_end_point.x), SMALL(m_rect.s_start_point.y, m_rect.s_end_point.y) };
    cv::Point2i end_point{ BIG(m_rect.s_start_point.x, m_rect.s_end_point.x), BIG(m_rect.s_start_point.y, m_rect.s_end_point.y) };
    m_rect.s_start_point = start_point;
    m_rect.s_end_point = end_point;
    if (m_rect.s_start_point.x < 0) {
      m_rect.s_start_point.x = 0;
    }
    if (m_rect.s_start_point.y < 0) {
      m_rect.s_start_point.y = 0;
    }
    if (m_rect.s_end_point.x > m_img_size.width) {
      m_rect.s_end_point.x = m_img_size.width - 1;
    }
    if (m_rect.s_end_point.y > m_img_size.height) {
      m_rect.s_end_point.y = m_img_size.height - 1;
    }
  }

  cv::Mat imageProcessing::getNoiseImage() const
  {
    return m_noise_img;
  }

  cv::Mat imageProcessing::getTextImage() const
  {
    return m_text_img;
  }

  cv::Mat imageProcessing::getCubeImage() const
  {
    return m_cube_img;
  }

  cv::Mat imageProcessing::getFreqImage() const
  {
    return m_freq_img;
  }

  imageProcessing::imageProcessing(void)
  {
  }
}