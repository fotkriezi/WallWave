import org.openkinect.processing.*;

Kinect2 kinect;
PImage img; // create image

void setup() {
  size(512, 424);
  kinect = new Kinect2(this);
  
  kinect.initDepth(); // initializes depthImage/depthCamera
  kinect.initDevice();
  img = createImage(kinect.depthWidth, kinect.depthHeight, RGB); // init image
}

void draw() {
  background(0);
  img.loadPixels(); // get prev image
  int[] depth = kinect.getRawDepth();
  
  for(int x = 0; x < kinect.depthWidth; x++) {
    for(int y = 0; y < kinect.depthHeight; y++) {
      int offset =  x + y * kinect.depthWidth;
      int d = depth[offset];
      // threshold logic
      if(d > 300 && d < 1500) {
        img.pixels[offset] = color(255, 0, 255);
      } else {
         img.pixels[offset] = color(0, 0, 0); 
      }
    }
  }
  
  img.updatePixels(); // update image
  image(img, 0, 0); // put new image on screen
}
