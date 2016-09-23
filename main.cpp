#include <iostream>
#include <log.h>
#include <frame.h>
#include <framereader.h>
#include <newfeatureextractor.h>
#include <newbodylocator.h>
#include <newframeconverter.h>
#include <vector>

using namespace std;
using namespace cv;

INITIALIZE_EASYLOGGINGPP

int main(int argc, char *argv[])
{
    LOG(INFO) << "Program start";

    FrameReader fr;

    Frame f;
    RecognitionSettings rs;
    f.setFileName("/media/sf_ubuntu/tof_1/data/D27000O14000/7_empty/TOF_ES_21824297__20160804_173521527_0001.tiff");
    f.load();
    cv::Mat empty = f.getImage();
  //  Mat m = fe.imageTransformation(mat, 20000, 15000, 30000, 14000);
    rs.setBedHistBins(5);
    rs.setConvertScale(256);
    rs.setMaxNoiseArea(0.004);
    rs.setMinHumanArea(0.018);


    shared_ptr<float> r1(new float[10], []( float *p ) { delete[] p; });
    r1.get()[0] = 0.1;
    r1.get()[1] = 4000;
    r1.get()[2] = 8000;
    r1.get()[3] = 16000;
    r1.get()[4] = 25000;
    r1.get()[5] = 65000;

    shared_ptr<float> r2(new float[10], []( float *p ) { delete[] p; });
   r2.get()[0] = 0.1;
   r2.get()[1] = 8000;
   r2.get()[2] = 16000;
   r2.get()[3] = 25000;
   r2.get()[4] = 65000;

   rs.setBedHistRanges(r1);
   rs.setFloorHistBins(4);
   rs.setFloorHistRanges(r2);
   rs.setMedianBlurScale(5);
   rs.setBedRecThresholdScale(60);
   rs.setWidth(640);
   rs.setHeight(480);
   rs.setHumContThresholdScale(3);
    shared_ptr<RecognitionSettings> rs_ptr(&rs);
   NewBodyLocator nbl(rs_ptr);
    NewFrameConverter nfc;
    nfc.setFrameAngle(57);
    nfc.setMaxIntensityValue(65535);
    nfc.setHeight(480);
    nfc.setWidth(640);

  //  fe.bedRecognition(mat);
    rs.bedRecognition(empty);

   empty = nfc.convert(empty, 27000, 14000, 30000, 14000);



    //vector<float> r1 = {0, 4000, 8000, 16000, 25000, 65000};


        fr.setDirectoryPath("/media/sf_ubuntu/tof_1/data/D27000O14000/");
    fr.load_frames();

    LOG(INFO) << "Image loading finished";
    NewFeatureExtractor fe(rs_ptr);

    for (auto ptr : fr.getFrameArray())
    {

        Mat mat = ptr.get()->getImage();
        mat = nfc.convert(mat, 27000, 14000, 30000, 14000);

        mat = nbl.locateBody(mat, empty);

         bool emp = fe.is_empty(mat);
         bool in = fe.is_human_in_bed(mat);

         array<double, 2> area = fe.getHumanArea(mat);
         double length = fe.getHumanLength(mat);
         vector<double> bed_int= fe.getBedIntensityHistogram(mat);


//        fe.getBedIntensityHistogram(mat);
//        fe.getFLoorIntensityHistogram(mat);
//        if (!in)
//        {
//            imshow("ooo", mat);
//            waitKey(0);
//        }

           //     namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
             //   imshow( "Display window", mat );                   // Show our image inside it.
               // waitKey(0);                                          // Wait for a keystroke in the window

    }

    LOG(INFO) << "Program end";

    return 0;
}
