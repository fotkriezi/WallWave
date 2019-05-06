import org.openkinect.freenect.*;
import org.openkinect.processing.*;
import org.openkinect.tests.*;

// import the fingertracker library
// and the SimpleOpenNI library for Kinect access
import fingertracker.*;

// import serial lib
import processing.serial.*;

// declare FignerTracker and SimpleOpenNI objects
FingerTracker fingers;
Kinect2 kinect;

// set a default threshold distance:
// 625 corresponds to about 2-3 feet from the Kinect
int threshold = 625;

Serial port;
String portName = "/dev/tty.SLAB_USBtoUART";

boolean traceOutline = true;

int countLoops = 0;

void setup() {
  size(512, 424);
  //port = new Serial(this, portName, 9600); // change to index of ardunio port name
  
  // initialize your SimpleOpenNI object
  // and set it up to access the depth image
  kinect = new Kinect2(this);
  kinect.initDepth();
  kinect.initDevice();
  // mirror the depth image so that it is more natural
  // kinect.enableMirror(true);
  // initialize the FingerTracker object
  // with the height and width of the Kinect
  // depth image
  fingers = new FingerTracker(this, kinect.depthWidth, kinect.depthHeight);
  // the "melt factor" smooths out the contour
  // making the finger tracking more robust
  // especially at short distances
  // farther away you may want a lower number
  fingers.setMeltFactor(50);
}

void draw() {
  // get new depth data from the kinect
  // get a depth image and display it
  background(0);
  PImage depthImage = kinect.getDepthImage();
  image(depthImage, 0, 0);

  // update the depth threshold beyond which
  // we'll look for fingers
  fingers.setThreshold(threshold);
  
  // access the "depth map" from the Kinect
  // this is an array of ints with the full-resolution
  // depth data (i.e. 500-2047 instead of 0-255)
  // pass that data to our FingerTracker
  int[] depthMap = kinect.getRawDepth();
  fingers.update(depthMap);

  // iterate over all the contours found
  // and display each of them with a green line
  if(traceOutline) {
    stroke(0,255,0);
    for (int k = 0; k < fingers.getNumContours(); k++) {
      fingers.drawContour(k);
    }
  }
  
  // iterate over all the fingers found
  // calculate average coordinate of all of them, and also draw mark each with a red circle
  noStroke();
  fill(255,0,0);
  int sumX = 0;
  int sumY = 0;
  int count = 0;
  for (int i = 0; i < fingers.getNumFingers(); i++) {
    PVector position = fingers.getFinger(i);
    ellipse(position.x - 5, position.y -5, 10, 10);
    sumX += position.x - 5;
    sumY += position.y - 5;
    count++;
  }
  
  // draw circle at calculated average coordinate of fingers
  if(count != 0) {
    int avgX = sumX/count;
    int avgY = sumY/count;
    fill(0,0,255);
    ellipse(avgX, avgY, 50, 50);
    
    
    // out dist data to serial
    int[] pos = new int[4];
    pos[0] = (int)dist(avgX, avgY, 0, 0);
    pos[1] = (int)dist(avgX, avgY, 512, 0);
    pos[2] = (int)dist(avgX, avgY, 0, 424);
    pos[3] = (int)dist(avgX, avgY, 512, 424);
    
    String out = "<" + convertDist(pos[0], false) + "," + convertDist(pos[1], true) + "," + convertDist(pos[2], false) + "," + convertDist(pos[3],true) + ">" ;
    if (countLoops % 60 == 0) {
      //port.write(out);
      print("SENT DATAPOINT");
    }
    countLoops++;
    
    print(convertDist(pos[0], false));
    print(", ");
    print(convertDist(pos[1], true));
    print(", ");
    print(convertDist(pos[2], false));
    print(", ");
    println(convertDist(pos[3], true));
  }
  
  // TODO: use serial communication to send 
  
  // show the threshold on the screen
  fill(255,0,0);
  text(threshold, 10, 20);
}

// keyPressed event:
// pressing the '-' key lowers the threshold by 10
// pressing the '+/=' key increases it by 10 
void keyPressed(){
  if(key == '-'){
    threshold -= 1;
  }
  
  if(key == '='){
    threshold += 1;
  }
  
  if(key == 't') {
    traceOutline = !traceOutline;
  }
}

int convertDist(int d, boolean reverse) {
  if(d < 100) {
    if (reverse) {
      return 0;
    }
    return 2;
  }
  if(d < 200) {
    return 1;
  }
  if (reverse) {
      return 2;
    }
  return 0;
}
