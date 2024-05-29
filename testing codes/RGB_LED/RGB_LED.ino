#define RED_LED 25
#define GREEN_LED 26
#define BLUE_LED 27

void setup(){
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);    
  pinMode(RED_LED, OUTPUT);
}

void loop(){
    rgb_color(0,0,255);     //blue
    delay(1000);
    rgb_color(0,255,0);     //green
    delay(1000);
    rgb_color(255,0,0);     //red
    delay(1000);
}

void rgb_color(int r, int g, int b){
    analogWrite(RED_LED, r);  
    analogWrite(GREEN_LED, g);
    analogWrite(BLUE_LED, b);
}
