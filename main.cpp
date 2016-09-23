#include <iostream>
#include <log.h>
#include <frame.h>
#include <framereader.h>
#include <newfeatureextractor.h>

using namespace std;
using namespace cv;

INITIALIZE_EASYLOGGINGPP

int main(int argc, char *argv[])
{
    LOG(INFO) << "Program start";

    FrameReader fr;
    NewFeatureExtractor fe;
    Frame f;
    f.setFileName("/media/sf_ubuntu/tof_1/data/D27000O14000/7_empty/TOF_ES_21824297__20160804_173521527_0001.tiff");
    f.load();
    cv::Mat mat = f.getImage();
  //  Mat m = fe.imageTransformation(mat, 20000, 15000, 30000, 14000);

    fe.bedRecognition(mat);

    mat = fe.changeIntensity(mat, 57);
    mat = fe.imageTransformation(mat, 27000, 14000, 30000, 14000);

    fe.setEmptyImage(mat);
    fe.setMaxNoiseArea(0.004);
    fe.setMinHumanArea(0.018);
    fe.setFrameAngle(57);
    //vector<float> r1 = {0, 4000, 8000, 16000, 25000, 65000};
    shared_ptr<float> r1(new float[10], []( float *p ) { delete[] p; });
    r1.get()[0] = 0.1;
    r1.get()[1] = 4000;
    r1.get()[2] = 8000;
    r1.get()[3] = 16000;
    r1.get()[4] = 25000;
    r1.get()[5] = 65000;
    fe.setBedHistRanges(r1);

        fr.setDirectoryPath("/media/sf_ubuntu/tof_1/data/D27000O14000/");
    fr.load_frames();

    LOG(INFO) << "Image loading finished";

    for (auto ptr : fr.getFrameArray())
    {
        cv::Mat mat = ptr.get()->getImage();
        mat = fe.changeIntensity(mat, 57);
        mat = fe.imageTransformation(mat, 27000, 14000, 30000, 14000);
        bool emp = fe.is_empty(mat);
        bool in = fe.is_human_in_bed(mat);
        array<double, 2> area = fe.getHumanArea(mat);
        double t = fe.getHumanLength(mat);

        fe.getBedIntensityHistogram(mat);
        fe.getFLoorIntensityHistogram(mat);
        if (!in)
        {
            imshow("ooo", mat);
            waitKey(0);
        }

           //     namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
             //   imshow( "Display window", mat );                   // Show our image inside it.
               // waitKey(0);                                          // Wait for a keystroke in the window

    }

    LOG(INFO) << "Program end";

    return 0;
}
