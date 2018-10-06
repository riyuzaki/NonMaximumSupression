#include<iostream>
#include<stdio.h>

#include<vector>
#include<algorithm>
#include<string>
#include <sstream>

#include <fstream>
#include <iterator>

#include <experimental/filesystem> //C++17

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;
namespace fs = std::experimental::filesystem;

float ComputeIOUScore(vector<float> a, vector<float> b);

int main(){

    string filename = "0";

    std::ifstream ifs(filename);

    string line;
    vector<vector<float> > input;
    while ( getline( ifs, line ) ) {
        istringstream is( line );
        input.push_back(
            std::vector<float>( std::istream_iterator<float>(is),
                              std::istream_iterator<float>() ) );
    }
    //at this point, we have a vector of all the tuples

    //now, sort(reverse way) the outer vector elements based on the certainty!
      // sort indexes based on comparing values in v
    sort(input.begin(), input.end(),
            [](const vector<float>& a, const vector<float>& b) {
    return a[0] > b[0];
    });

    vector<vector<float>> final_nms_list;
    //Add the top probability input into a vector!
    final_nms_list.push_back(input[0]);
    const float IOU_threshold = 0.6;

    for(int i = 1; i < input.size(); i++){

            int j = 0;
            while(j < final_nms_list.size()){
                    if(ComputeIOUScore(input[i], final_nms_list[j]) > IOU_threshold) break;
                    j++;
             }

             if (j == final_nms_list.size()){
                 final_nms_list.push_back(input[i]);
             }

    }

    cout << final_nms_list.size() << endl;

    string out_filename, out_folder;
    out_folder = "Solution";
    out_filename = out_folder + "/temp_out.txt";

    if(fs::exists(out_folder) == 0)
    {
        fs::create_directory(out_folder);
    }

    ofstream output_file(out_filename);

    for(const auto& row : final_nms_list) {
        copy(row.cbegin() + 1, row.cend(), ostream_iterator<float>(output_file, " "));
        output_file << '\n';
    }


    Mat img;

    img = imread("0.png", CV_LOAD_IMAGE_COLOR);

    if(! img.data )                              // Check for invalid input
    {
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }

    for(int i = 0; i < final_nms_list.size(); i++){
        Rect RectangleToDraw(final_nms_list[i][1], final_nms_list[i][2], final_nms_list[i][4], final_nms_list[i][3]);
        rectangle(img, RectangleToDraw.tl(), RectangleToDraw.br(), Scalar(0, 0, 255), 2, 8, 0);
    }

    string out_image = out_folder + "/" + filename + ".png";

    imwrite(out_image, img);

    namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
    imshow( "Display window", img );                   // Show our image inside it.

    waitKey(0);


    return 0;

}

float ComputeIOUScore(vector<float> a, vector<float> b){

    float area_a;
    float area_b;

    area_a = a[3] * a[4];
    area_b = b[3] * b[4];

    //intersection area
    float area_of_intersection;
    float x_diff = (min(a[1] + a[3], b[1] + b[3]) -
                 max(a[1], b[1]));

    float y_diff = (min(a[2] + a[4], b[2] + b[4]) -
                 max(a[2], b[2]));

    if (x_diff < 0 || y_diff < 0){
        area_of_intersection = 0;
    }else{
        area_of_intersection = x_diff * y_diff;
    }

    float area_of_union = area_a + area_b - area_of_intersection;

    float IOU = area_of_intersection/area_of_union;

    return IOU;


}
