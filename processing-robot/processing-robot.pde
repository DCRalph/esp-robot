import processing.serial.*;

String data="";

boolean conn = false;

int mode = 1;

PFont font;

boolean[] keys = new boolean[255];
boolean[] keys_c = new boolean[255];

int speed = 100;
int left = 0;
int right = 0;

String c = ",";
String out;

Serial myPort;  
void setup() 
{
  size(300, 320);
  //fullScreen();s
  //noCursor();

  textFont(createFont("font2.ttf", 50));
  //textSize(50);
  textAlign(CENTER);

  //String portName = Serial.list()[1];
  myPort = new Serial(this, "/dev/tty.usbserial-1420", 115200);
  //myPort = new Serial(this, portName, 115200);
}

void draw() {
  background(0, 0, 0);

  if (conn) {
    if (mode == 1) {
      
      if (keys['w']) {
        if(keys['a']){
          left = 0;
          right = speed;
        } else if(keys['d']){
          left = speed;
          right = 0;
        } else {
          left = speed;
          right = speed;
        }
      } else if (keys['s']) {
        if(keys['a']){
          left = -speed;
          right = 0;
        } else if(keys['d']){
          left = 0;
          right = -speed;
        } else {
          left = -speed;
          right = -speed;
        }
      } else if (keys['a']) {
        left = -speed;
        right = speed;
      } else if (keys['d']) {
        left = speed;
        right = -speed;
      } else {
        left = 0;
        right = 0;
      }
 
    } else {
      
    }
      
    speed = constrain(speed, 0, 255);
    left = constrain(left, -speed, speed);
    right = constrain(right, -speed, speed);

    text("Mode:  " + mode, width/2, 50);

    text("Speed:  " + speed, width/2, 100);
    text("Left:  " + left, width/2, 150);
    text("Right:  " + right, width/2, 200);
  } else {
    left = 0;
    right = 0;
    text("No Conn", width/2, 50);
  }

  out = left + c + right;
  println(conn ? "Connected" : "Disconnected" + out);
  myPort.write(out);

}

void keyPressed() {
  if (key != CODED) {
    keys[key] = true;
  } else {
    keys_c[keyCode] = true;
  }

  if (key == '1') {
    speed = 0;
  }
  if (key == '2') {
    speed = 50;
  }
  if (key == '3') {
    speed = 100;
  }
  if (key == '4') {
    speed = 150;
  }
  if (key == '5') {
    speed = 255;
  }
}

void keyReleased() {
  if (key != CODED) {
    keys[key] = false;
  } else {
    keys_c[keyCode] = false;
  }
}

void serialEvent (Serial myPort) { 
  data = myPort.readStringUntil('\n');

  if (data != null) {
    data = trim(data);
    String items[] = split(data, ',');

    if (items.length > 1) {
      conn = int(items[0]) == 1;
    }
  }
}
