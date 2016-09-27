#include <iostream>
#include <log.h>
#include <frame.h>
#include <framereader.h>
#include <newfeatureextractor.h>
#include <newbodylocator.h>
#include <newframeconverter.h>
#include <vector>
#include <svm.h>



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
    vector<shared_ptr<PoseFeatures>> res;
    int i = 0;
    for (auto ptr : fr.getFrameArray())
    {
        i++;
        Mat mat = ptr.get()->getImage();
        mat = nfc.convert(mat, 27000, 14000, 30000, 14000);
        mat = nbl.locateBody(mat, empty);
        res.push_back(fe.extractFeatures(mat));
        if (!(i%20))
            LOG(INFO) <<"COUNT  " << i ;
    }
    LOG(INFO) <<"HERE";
    svm_model svm;
    svm_problem prob;
    prob.l = res.size();
    vector<int> fc = fr.get_frames_classes();
    double *y  = new double[fc.size()];
    for (int i = 0; i < fc.size(); i++)
        y[i] = fc[i];
    svm_node ** node_array = new svm_node*[res.size()];
    for (int i = 0; i < res.size(); i++)
        node_array[i] = new svm_node[14];

    for (int i = 0; i < res.size(); i++)
    {
        shared_ptr<PoseFeatures> tmp = res[i];
        node_array[i][0].index = 1;
        node_array[i][1].index = 2;
        node_array[i][2].index = 3;
        node_array[i][3].index = 4;
        node_array[i][4].index = 5;

        if (tmp)
        {
            node_array[i][0].value = tmp.get()->getInBed();
            node_array[i][1].value = tmp.get()->getAreaInBed();
            node_array[i][2].value = tmp.get()->getAreaNotInBed();
            node_array[i][3].value = tmp.get()->getRelArea();
            node_array[i][4].value = tmp.get()->getHumanLength();
            vector<double> intens = tmp.get()->getBedIntensity();
            int j  = 0;
            for (; j < intens.size(); j++)
            {
                node_array[i][5 + j].index = 5 + j;
                node_array[i][5 + j].value = intens[j];
            }

            intens = tmp.get()->getFloorIntensity();
            for (int k  = 0; j < intens.size(); j++)
            {
                node_array[i][j + k].index = j + k;
                node_array[i][j + k].value = intens[j];
            }
        }
        else
        {
            node_array[i][0].value = 0;
            node_array[i][1].value = 0;
            node_array[i][2].value = 0;
            node_array[i][3].value = 0;
            node_array[i][4].value = 0;
            for (int j = 5; i < 15; i++)
            {
                node_array[i][j].index = j;
                node_array[i][j].value = 0;
            }
        }
    }


    //svm = svm_train();
    LOG(INFO) << "Program end";

    return 0;
}
