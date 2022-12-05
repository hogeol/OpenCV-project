#ifndef _IMAGE_PROCESSING_H_
#define _IMAGE_PROCESSING_H_

#include <opencv2/opencv.hpp>

#define SMALL(a, b) (a)<(b)?(a):(b)
#define BIG(a, b) (a)>(b)?(a):(b)

namespace ImageProcessing {
  struct rect_point {
    cv::Point2i s_start_point{};
    cv::Point2i s_end_point{};
  };

  class imageProcessing
  {
  private:
    cv::Mat m_noise_img;
    cv::Mat m_text_img;
    cv::Mat m_cube_img;
    cv::Mat m_freq_img;
    cv::Size m_img_size;
    cv::Size m_filter_rect_size;
    double m_filter_radius;
    rect_point m_rect;
    bool m_mouse_flag;
    cv::Mat m_gray_text_img;

  public:
    void init(const cv::Size& img_size);
    void pj1Init(const cv::Mat& noise_img, const cv::Mat& text_img);
    void removeNoise(cv::Mat& img_out);
    void findText(cv::Mat& img_out);
    void skeletonization(const cv::Mat& gray_img_in, cv::Mat& img_out);
    void hitOrMiss(const cv::Mat& img_in, cv::Mat& img_out);
    void pj2Init(const cv::Mat& cube_img);
    void splitBGR(cv::Mat& img_b, cv::Mat& img_g, cv::Mat& img_r);
    void hsvProcessing(cv::Mat& image_out, const std::string& type);
    void pj3Init(const cv::Mat& img, const cv::Size& rectangle_size = cv::Size2f{ 0.0, 0.0 });
    void pj3Init(const cv::Mat& img, const double& circle_radius = 0.0);
    void fourierTransform(const std::string& filter, cv::Mat& img_out);
    void shuffleDft(cv::Mat& img_out);
    void rectFilter(const cv::Size& img_size, cv::Mat& filter_out);
    void butterWorthFilter(const cv::Size& img_size, const double& filter_radius, cv::Mat& filter_out);
    void mouseROI(int event, int x, int y);
    static void mouseROI(int event, int x, int y, int flags, void* param);
    inline void setRectPoint();
    cv::Mat getNoiseImage() const;
    cv::Mat getTextImage() const;
    cv::Mat getCubeImage() const;
    cv::Mat getFreqImage() const;
    imageProcessing(void);
  };
}
#endif