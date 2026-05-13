#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

// [실습과제 2] 직접 만든 히스토그램 계산 함수
Mat mycalcGrayHist(const Mat& img) {
    CV_Assert(img.type() == CV_8UC1);
    Mat hist = Mat::zeros(256, 1, CV_32F);

    for (int y = 0; y < img.rows; y++) {
        for (int x = 0; x < img.cols; x++) {
            uchar bin = img.at<uchar>(y, x);
            hist.at<float>(bin)++;
        }
    }
    return hist;
}

// [실습과제 3] 직접 만든 꺾은선 히스토그램 그래프 함수
Mat mygetGrayHistImage(const Mat& hist) {
    Mat imgHist(200, 256, CV_8UC1, Scalar(255));
    double maxVal;
    minMaxLoc(hist, nullptr, &maxVal);

    for (int i = 0; i < 254; i++) {
        int h1 = cvRound((hist.at<float>(i) * 200) / maxVal);
        int h2 = cvRound((hist.at<float>(i + 1) * 200) / maxVal);
        line(imgHist, Point(i, 200 - h1), Point(i + 1, 200 - h2), Scalar(0), 1);
    }
    return imgHist;
}

int main() {
    // [중요] 대상을 camera.bmp로 변경
    Mat src = imread("camera.bmp", IMREAD_GRAYSCALE);

    if (src.empty()) {
        cerr << "camera.bmp 파일을 찾을 수 없습니다!" << endl;
        return -1;
    }

    // 1. 직접 만든 함수로 히스토그램 계산 (과제 2 방식)
    Mat hist = mycalcGrayHist(src);

    // 2. 직접 만든 함수로 꺾은선 그래프 생성 (과제 3 방식)
    Mat histImg = mygetGrayHistImage(hist);

    // 3. 화면 출력
    imshow("src", src);           // 원본 영상 창
    imshow("srcHist", histImg);   // 히스토그램 그래프 창

    waitKey(0);
    destroyAllWindows();

    return 0;
}