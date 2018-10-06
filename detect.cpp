#include "detect.h"

using namespace std;
using namespace cv;
namespace fs = std::experimental::filesystem;


int main(int argc, char * argv[]){

  pthread_attr_t attr;
  struct sched_param prio;
  pthread_attr_init(&attr);
  pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
  prio.sched_priority = 1;
  pthread_attr_setschedparam(&attr, &prio);

  DetectCars detector;
  detector.Init(&attr);

  vector<String> filenames;
  String folder = "input";//argv[1];

  //cout << "eoQueue.size() main " << eoQueue.size() << endl;

  glob(folder, filenames);

  for(int i = 0; i < filenames.size(); i++){
      detector.PushInput(filenames[i]);
  }

  while(1)
  {
      sleep(3);
  }

  return 0;

}

void* detectcars_thread_work(void* sender);

void* detectcars_thread_work(void* sender)
{
    /* Call the sender's thread routine. */
    ((DetectCars*)sender)->Work();
    return NULL;
}

void DetectCars::Init(pthread_attr_t* p_attr)
{
    for(uint32_t j = 0; j < NUM_THINGFINDER_THREADS; j++)
    {
        pthread_create(&workThread[j], p_attr, &detectcars_thread_work, this);
    }
}


void DetectCars::Work()
{
    string input;
    while(1)
    {
        input = eoQueue.pop(); //input

        aggregate(input);

    }

}


void DetectCars::PushInput(string input)
{
        if(eoQueue.try_push(input) != 0)
        {
            cout << "eoQueue queue full" << endl;
            eoQueue.push(input);
        }
}

void DetectCars::aggregate(string filepath){

  //{
  //unique_lock<mutex> lck(findMutex);
  //auto begin = chrono::high_resolution_clock::now();
  //string filename = "0";
  std::ifstream ifs(filepath);
  string filename =  filepath.substr(6, filepath.length());

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

  string out_filename, out_folder;
  out_folder = "Solution";
  out_filename = out_folder + "/" + filename + ".txt";

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
  string img_path = "img/" + filename + ".png";
  img = imread(img_path, CV_LOAD_IMAGE_COLOR);

  if(! img.data )                              // Check for invalid input
  {
      cout <<  "Could not open or find the image" << std::endl ;
      //return -1;
  }

  for(int i = 0; i < final_nms_list.size(); i++){
      Rect RectangleToDraw(final_nms_list[i][1], final_nms_list[i][2], final_nms_list[i][4], final_nms_list[i][3]);
      rectangle(img, RectangleToDraw.tl(), RectangleToDraw.br(), Scalar(0, 0, 255), 2, 8, 0);
  }

  string out_image = out_folder + "/" + filename + ".png";

  imwrite(out_image, img);

  string filename_truth =  filename + ".txt";
  string filepath_truth =  "truth/" + filename_truth;
  std::ifstream ifs2(filepath_truth);
  string line_truth;
  vector<vector<float> > truth;
  while ( getline( ifs2, line_truth ) ) {
      istringstream is( line_truth );
      truth.push_back(
          std::vector<float>( std::istream_iterator<float>(is),
                            std::istream_iterator<float>() ) );
  }

  for (int j = 0; j < truth.size(); j++){
    truth[j].insert(truth[j].begin(), -1);
  }

  vector<float> scores;
  for(int i = 0; i < final_nms_list.size(); i++){
      float max_score = 0;
      for (int j = 0; j < truth.size(); j++){
        //cout << ComputeIOUScore(truth[i], final_nms_list[i]) <<endl;
          max_score = max(max_score, ComputeIOUScore(truth[j], final_nms_list[i]));
      }
      scores.push_back(max_score);

  }

  string out_filename_task_2, out_folder_task_2;
  out_folder_task_2 = "scoring";
  out_filename_task_2 = out_folder_task_2 + "/" + filename + ".txt";

  if(fs::exists(out_folder_task_2) == 0)
  {
      fs::create_directory(out_folder_task_2);
  }

  ofstream output_file_2(out_filename_task_2);

  for(const auto& score : scores) {
      output_file_2 << score;
      output_file_2 << '\n';
  }

  //auto end = chrono::high_resolution_clock::now();
  //auto dur = end - begin;
  //auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
  //cout << "time "<< ms << endl;
  //}

}

float DetectCars::ComputeIOUScore(vector<float> a, vector<float> b){

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
