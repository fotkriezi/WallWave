import processing.serial.*;

Serial myPort;

void setup() {
  myPort = new Serial(this, "/dev/tty.SLAB_USBtoUART", 9600);
}

void draw() {}

void mouseClicked() {
   String  out = "2,1,3,4";
   // myPort.write('<');
   myPort.write(out);
   // myPort.write('>');
}
