import org.openkinect.processing.*;

Kinect2 kinect;

void setup() {
  size(512, 424);
  kinect = new Kinect2(this);
  
  kinect.initDepth(); // initializes depthImage/depthCamera
  kinect.initDevice();
}

void draw() { 
  background(0);
  outBasicDepthImage();
  // outDepthImgAtResolution(5);
}

void outBasicDepthImage() {
  PImage img = kinect.getDepthImage(); // store depth image feed in variable
  image(img, 0, 0); // output image on screen
}

void outDepthImgAtResolution(int skip) {
  PImage img = kinect.getDepthImage(); // store depth image feed in variable
  image(img, 0, 0); // output image on screen
  
  for(int x = 0; x < img.width; x+=skip) {
    for(int y = 0; y < img.height; y+=skip) {
      int index = x + y * img.width;
      float b = brightness(img.pixels[index]);
      fill(b);
      rect(x, y, skip, skip);
    }
  }
}


/* 
When working with depth image, you commonly you want to iterate through
each of the pixels and find which are the closest

output is given as a 1-D array of color values
  we use x+y * width to access pixel (x, y)

NOTE: colors are as seen by depth image camera in this example i.e. a shade of grey
NOTE: this example doesn't make use of depth data, just the depth image
*/
