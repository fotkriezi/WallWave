import org.openkinect.processing.*;

Kinect2 kinect;
PImage img; // create image
int thresh = 300;
int c = 1200;

void setup() {
  size(512, 424);
  kinect = new Kinect2(this);
  
  kinect.initDepth(); // initializes depthImage/depthCamera
  kinect.initDevice();
  img = createImage(kinect.depthWidth, kinect.depthHeight, RGB); // init image
}

void draw() {
  background(0); // clears bg
  img.loadPixels(); // get prev image
  int[] depth = kinect.getRawDepth();
  
  int sumX = 0; 
  int sumY= 0;
  int count = 0;
  int highestY = Integer.MAX_VALUE;
  for(int x = 0; x < kinect.depthWidth; x++) {
    for(int y = 0; y < kinect.depthHeight; y++) {
      int offset =  x + y * kinect.depthWidth;
      int d = depth[offset];
      // threshold logic
      if(d > thresh && d < thresh + c) {
        if(highestY == Integer.MAX_VALUE) highestY = y;
        img.pixels[offset] = color(0, 255, 0);
        sumX += x;
        sumY += y;
        count++;
      } else {
         img.pixels[offset] = color(0, 0, 0); 
      }
    }
  }
  
  
  
  img.updatePixels(); // update image
  image(img, 0, 0); // put new image on screen
  
  // Any Additional Drawing
  if(count != 0) {
    fill(255, 0, 0);
    ellipse((int)sumX/count, (int)sumY/count, 50, 50);
    rect((int)sumX/count - 150/2, (int)sumY/count - 300/2, 150, 300);
  }
  int rectX = (int)sumX/count - 150/2;
  int rectY = (int)sumY/count - 300/2;
  int w = 150;
  int h = 300;
  int[] out = 
  
  for(int y = 0; y < 424; y += 141) {
    for(int x = 0; x < 512; x += 512) {
      if(y >= rectY && y <= rectY + h) {
        
      }
    }
  }
  
  fill(255,0,0);
  text(thresh, 10, 20);
}

void keyPressed() {
 switch(key) {
  case '-':
    thresh -= 2;
    break;
  case '=':
    thresh += 2;
    break;
 }
}
