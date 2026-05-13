#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

// 1. [실습과제 2] 직접 구현한 히스토그램 계산 함수
Mat mycalcGrayHist(const Mat& img)
{
    CV_Assert(img.type() == CV_8UC1); // 8비트 1채널 확인

    // 256행 1열의 float 행렬 생성 및 0 초기화
    Mat hist = Mat::zeros(256, 1, CV_32F);

    // 모든 픽셀을 순회하며 빈도수 직접 카운트
    for (int y = 0; y < img.rows; y++) {
        for (int x = 0; x < img.cols; x++) {
            uchar bin = img.at<uchar>(y, x);
            hist.at<float>(bin)++;
        }
    }
    return hist;
}

// 2. 히스토그램 데이터를 '산 모양' 그래프로 그리는 함수
Mat getHistImage(const Mat& hist)
{
    // 흰색 바탕의 이미지 생성 (가로 256, 세로 200)
    Mat imgHist(200, 256, CV_8UC1, Scalar(255));

    // 그래프가 창에 꽉 차게 보이도록 최대값 기준으로 정규화
    double maxVal;
    minMaxLoc(hist, nullptr, &maxVal);

    for (int i = 0; i < 256; i++) {
        // 빈도수 비율에 따라 선의 길이를 결정 (이미지 높이 200 기준)
        int h = cvRound((hist.at<float>(i) * 200) / maxVal);

        // (i, 200)에서 (i, 200-h)까지 검은색 선을 그림
        line(imgHist, Point(i, 200), Point(i, 200 - h), Scalar(0));
    }
    return imgHist;
}

int main()
{
    // [이미지 로드] 파일이름과 경로를 확인하세요!
    Mat src = imread("lenna.bmp", IMREAD_GRAYSCALE);

    if (src.empty()) {
        cerr << "영상을 불러올 수 없습니다. 파일명을 확인하세요!" << endl;
        return -1;
    }

    // [데이터 계산] 직접 만든 함수 호출
    Mat hist = mycalcGrayHist(src);

    // [통계 정보 추출] 실습과제 1 내용
    double minVal, maxVal;
    minMaxLoc(src, &minVal, &maxVal);

    double maxFreq;
    Point maxLoc;
    minMaxLoc(hist, nullptr, &maxFreq, nullptr, &maxLoc);

    // [콘솔 출력]
    cout << "영상의 전체 픽셀수 : " << src.rows * src.cols << endl;
    cout << "영상에서 픽셀값의 최소값 : " << (int)minVal << endl;
    cout << "영상에서 픽셀값의 최대값 : " << (int)maxVal << endl;
    cout << "빈도수가 가장많은 픽셀값과 빈도수 : " << maxLoc.y << ", " << (int)maxFreq << endl;
    cout << "픽셀값 80의 빈도수 : " << (int)hist.at<float>(80) << endl;

    // [시각화 및 출력]
    Mat histImg = getHistImage(hist);

    imshow("입력 영상", src);       // 레나 이미지
    imshow("srcHist", histImg);    // 산 모양 그래프

    waitKey(0); // 키 입력 대기
    destroyAllWindows();

    return 0;
}