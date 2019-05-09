import org.openkinect.processing.*;
import processing.serial.*;

Kinect2 kinect;
PImage img; // create image
int thresh = 300;
int c = 1200;

Serial port;
String portName = "/dev/cu.usbmodem14101";

void setup() {
  size(512, 424);
  kinect = new Kinect2(this);
  port = new Serial(this, portName, 9600);
  kinect.initDepth(); // initializes depthImage/depthCamera
  kinect.initDevice();
  img = createImage(kinect.depthWidth, kinect.depthHeight, RGB); // init image
}

void draw() {
  // DRAW NEXT FRAME
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
  
  // DRAW METADATA
  fill(255,0,0);
  text(thresh, 10, 20);
  
  // DRAW SHAPES
  if(count != 0) {
    fill(255, 0, 0);
    // ellipse((int)sumX/count, (int)sumY/count, 50, 50);
    // rect((int)sumX/count - 150/2, (int)sumY/count - 300/2, 150, 300);

    int w = 130;
    int h = 400;
    int rectX = (int)sumX/count - w/2;
    int rectY = (int)sumY/count - h/2;
    rect(rectX, rectY, w, h);
    
    StringBuilder out = new StringBuilder();
    for(int y = 0; y <= 424; y += 141) {
      for(int x = 0; x < 512; x += 85) {
        if(y >= rectY && y <= rectY + h && x >= rectX && x <= rectX + w) {
          out.append('1');
          print("1");
        } else {
          out.append('0');
          print("0");
        }
      }
      println();
    }
    println();
    
    // SERIAL AND SYNC
    if (port.available() > 0) {
      print("RECEIVED ACK\n");
      port.clear();
      port.write(out.toString());
    }
  }
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
