#include "cvmatandqimage.h"
#include <QString>
#include <QtTest>
#include <QImage>
#include <QGuiApplication>
#include <opencv2/core/core.hpp>

using namespace QtOcv;

class CvmatandimageTest : public QObject
{
    Q_OBJECT
    
public:
    CvmatandimageTest();
    
private Q_SLOTS:
    void testMatToImage_data();
    void testMatToImage();

    void testImageToMat_data();
    void testImageToMat();
};

CvmatandimageTest::CvmatandimageTest()
{
}

void CvmatandimageTest::testMatToImage_data()
{
    QTest::addColumn<bool>("useShared");
    QTest::addColumn<int>("channels");
    QTest::newRow("shared data convert: Gray") << true << 1;
    QTest::newRow("standard convert: Gray") << false << 1;
    QTest::newRow("shared data convert: RGB888") << true << 3;
    QTest::newRow("standard convert: RGB888") << false << 3;
    QTest::newRow("shared data convert: RGB32") << true << 4;
    QTest::newRow("standard convert:RGB32") << false << 4;
}

void CvmatandimageTest::testMatToImage()
{
    QFETCH(bool, useShared);
    QFETCH(int, channels);

    const int width = 512;
    cv::Mat mat_8UC1(width, width, CV_8UC1);
    for (int i=0; i<width; ++i) {
        for (int j=0; j<width; ++j)
            mat_8UC1.at<uchar>(i,j) = (i*j)%255;
    }
    cv::Mat mat_8UC3;
    std::vector<cv::Mat> channelVector;
    channelVector.push_back(mat_8UC1);
    channelVector.push_back(mat_8UC1);
    channelVector.push_back(mat_8UC1);
    cv::merge(channelVector, mat_8UC3);

    cv::Mat mat_8UC4;
    channelVector.push_back(cv::Mat(512, 512, CV_8UC1, cv::Scalar_<uchar>(128)));
    cv::merge(channelVector, mat_8UC4);

    QImage img;

    if (!useShared) {
        if (channels == 1) {
            QBENCHMARK {
                img = mat2Image(mat_8UC1, QImage::Format_Indexed8);
            }
        } else if (channels == 3) {
            QBENCHMARK {
                img = mat2Image(mat_8UC3, QImage::Format_RGB888);
            }
        } else {
            QBENCHMARK {
                img = mat2Image(mat_8UC4, QImage::Format_RGB32);
            }
        }
    } else {
        if (channels == 1) {
            QBENCHMARK {
                img = mat2Image_shared(mat_8UC1);
            }
        } else if (channels == 3) {
            QBENCHMARK {
                img = mat2Image_shared(mat_8UC3);
            }
        } else {
            QBENCHMARK {
                img = mat2Image_shared(mat_8UC4);
            }
        }
    }
}

void CvmatandimageTest::testImageToMat_data()
{
    QTest::addColumn<bool>("useShared");
    QTest::addColumn<int>("channels");
    QTest::newRow("shared data convert: Gray") << true << 1;
    QTest::newRow("standard convert: Gray") << false << 1;
    QTest::newRow("shared data convert: RGB888") << true << 3;
    QTest::newRow("standard convert: RGB888") << false << 3;
    QTest::newRow("shared data convert: RGB32") << true << 4;
    QTest::newRow("standard convert:RGB32") << false << 4;
}

void CvmatandimageTest::testImageToMat()
{
    QFETCH(bool, useShared);
    QFETCH(int, channels);

    QImage index8 = QImage(512, 512, QImage::Format_Indexed8);
    QImage rgb888 = QImage(512, 512, QImage::Format_RGB888);
    QImage rgb32 = QImage(512, 512, QImage::Format_RGB32);

    cv::Mat mat;
    if (!useShared) {
        if (channels == 1) {
            QBENCHMARK {
                mat = image2Mat(index8);
            }
        } else if (channels == 3) {
            QBENCHMARK {
                mat = image2Mat(rgb888);
            }
        } else {
            QBENCHMARK {
                mat = image2Mat(rgb32);
            }
        }
    } else {
        if (channels == 1) {
            QBENCHMARK {
                mat = image2Mat_shared(index8);
            }
        } else if (channels == 3) {
            QBENCHMARK {
                mat = image2Mat_shared(rgb888);
            }
        } else {
            QBENCHMARK {
                mat = image2Mat_shared(rgb32);
            }
        }
    }
}

QTEST_MAIN(CvmatandimageTest)

#include "tst_cvmatandimagetest.moc"