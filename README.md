# NonMaximumSupression

The inputs are in the following directories :

1. `img` - input images showing vehicles
2. `input` - a list of candidate detections per image resulting from our vehicle detection algorithm.

Each item in the list is a 5-tuple that describe a rectangle in the image coordinates, along with its confidence measure, a number between 0.0 and 1.0.
For example, this candidate rectangle:
```
0.816 962 169 153 256
```
has

* confidence = 0.816
* top-left point: (x=962, y=169)
* dimensions: (width=153, height=256)


## function 1
This implements an `aggregate` function that takes the _candidate_ rectangles list as an argument and returns a list of _detection_ rectangles, one per object.

Think of this as a the last stage in a vehicle detection algorithm. Our detection algorithm generates a candidate rectangle everywhere in the image it thinks it's found a vehicle, and this can lead to multiple rectangles of slightly different sizes and shapes for the same vehicle. Your `aggregate` function will get rid of all but the highest-confidence rectangle for each vehicle, such as seen in the figure above.

Pseudocode:
```
list<Rect> cars = aggregate(detections) {
    // your code here
}
```

The code should read a file from the `input/` folder and save the result to a `Solution/` folder, in similar format to the files in the `truth/` folder. Two files per image

* A text file with a space-separated 4-tuple (x y width height) per detection rectangle, with each detection in a new line
* An image file showing the rectangles from the text file drawn on top of the input image

## function 2

In some of the images, the highest ranked candidate is not a good quality detection. For example, in `img/2.png` and `img/3.png` more than one vehicle gets grouped into one rectangle. Can you do better?

That brings up the question, how would you quantify the overall quality of your solution? The answer is to use the Intersection over Union (IoU) algorithm to compare your detection rectangles with the "truth" rectangles that we've drawn by hand (in the `truth` folder). The code should compare each detection rectangle with the best-fitting truth rectangle and generate a value between 0.0 (no overlap) and 1.0 (perfect overlap). Save these results to a `scoring` folder with one file per image, one detection score per line. Then also save the overall average for all five images to a file named `overall_score`.
