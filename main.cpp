#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

Mat calcGrayHist(const Mat& img) {
    CV_Assert(img.type() == CV_8UC1);
    Mat hist;
    int channels[] = { 0 };
    int dims = 1;
    const int histSize[] = { 256 };
    float graylevel[] = { 0, 256 };
    const float* ranges[] = { graylevel };
    calcHist(&img, 1, channels, noArray(), hist, dims, histSize, ranges);
    return hist;
}

int main() {
    Mat src = imread("lenna.bmp", IMREAD_GRAYSCALE);
    if (src.empty()) {
        cerr << "Image load failed!" << endl;
        return -1;
    }

    // --- 1. 히스토그램 스트레칭 수행 ---
    double minVal, maxVal;
    minMaxLoc(src, &minVal, &maxVal);

    // 공식 적용: (src - min) * 255 / (max - min)
    Mat dst = (src - minVal) * 255 / (maxVal - minVal);

    // --- 2. 콘솔 출력 내용 ---
    Mat hist = calcGrayHist(src);
    double maxFreq;
    Point maxLoc;
    minMaxLoc(hist, nullptr, &maxFreq, nullptr, &maxLoc);

    cout << "영상의 전체 픽셀수 : " << src.rows * src.cols << endl;
    cout << "영상에서 픽셀값의 최소값 : " << (int)minVal << endl;
    cout << "영상에서 픽셀값의 최대값 : " << (int)maxVal << endl;
    cout << "빈도수가 가장많은 픽셀값과 빈도수 : " << maxLoc.y << ", " << (int)maxFreq << endl;
    cout << "픽셀값 80의 빈도수 : " << (int)hist.at<float>(80) << endl;

    // --- 3. 영상 화면 출력 (이 부분이 있어야 이미지가 보입니다) ---
    imshow("Original (lenna)", src);       // 원본 영상
    imshow("Stretched (lenna)", dst);     // 스트레칭 된 영상

    // 키보드 입력을 기다림 (아무 키나 누르면 창이 닫힘)
    waitKey(0);
    destroyAllWindows();

    return 0;
}