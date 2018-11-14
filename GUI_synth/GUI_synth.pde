import controlP5.*;
import processing.serial.*;
import meter.*;

ControlP5 cp5;
Textarea myTextarea;
Meter m, m2;

Serial port;

void setup(){
  size(800,800);
  
  cp5 = new ControlP5(this);
  
  //port = new Serial(this, "COM3", 9600);
  
  myTextarea = cp5.addTextarea("txt")
                  .setPosition(100,100);
}

void draw() {
  background(57,162,183);
  
  textSize(30);
  text("Sythesizer Controls",250,40);
  
  //if (port.available() > 0){
  //  String val = port.readString();
  //  myTextarea.setText(val);
  //}
}
