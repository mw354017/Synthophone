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

  m = new Meter(this, 25, 100);
  
  m.setTitleFontSize(20);
  m.setTitleFontName("Arial bold");
  m.setTitle("Breath");
  
  // Change meter scale values
  String[] scaleLabels = {"0", "20", "20", "30", "40", "50", "60", "70", "127"};
  m.setScaleLabels(scaleLabels);
  m.setScaleFontSize(18);
  m.setScaleFontName("Times new roman bold");
  m.setScaleFontColor(color(200, 30, 70));
  
  // We can also display the value of meter
  m.setDisplayDigitalMeterValue(true);
  
  // Lets do some more modifications so our meter looks nice
  m.setArcColor(color(141, 113, 178));
  m.setArcThickness(15);
  
  m.setMaxScaleValue(80);
  
  m.setMinInputSignal(0);
  m.setMaxInputSignal(80);
  
  m.setNeedleThickness(3);
}

void draw() {
  background(57,162,183);
  
  textSize(30);
  text("Sythesizer Controls",250,40);
  m.updateMeter(5);
  
  // if (port.available() > 0){
  //  String val = port.readString();
  //  myTextarea.setText(val);
  // }
}