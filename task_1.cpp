/*#include<iostream>
#include<stdio.h>

#include<vector>
#include<algorithm>
#include<string>
#include <sstream>

#include <fstream>
#include <iterator>

//#include <filesystem>

using namespace std;
//namespace fs = std::filesystem;

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

    string out_filename = "temp_out.txt";
    ofstream output_file(out_filename);

    for(const auto& row : final_nms_list) {
        copy(row.cbegin(), row.cend(), ostream_iterator<float>(output_file, " "));
        output_file << '\n';
    }

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
*/
