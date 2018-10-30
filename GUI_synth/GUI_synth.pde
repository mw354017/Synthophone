import controlP5.*;
import processing.serial.*;

ControlP5 cp5;
Textarea myTextarea;

Serial port;

void setup(){
  size(300,400);
  
  cp5 = new ControlP5(this);
  
  port = new Serial(this, "COM3", 9600);
  
  myTextarea = cp5.addTextarea("txt")
                  .setPosition(100,100);
}

void draw() {
  background(150,0,15);
  
  fill(0,255,0);
  text("Sythesizer Controls",100,30);
  
  if (port.available() > 0){
    String val = port.readString();
    myTextarea.setText(val);
  }
}
