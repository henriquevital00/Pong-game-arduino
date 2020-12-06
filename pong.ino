#include <Adafruit_NeoPixel.h>
#include <LiquidCrystal.h>

#define PIN1 8
#define PIN2 9
#define PIN3 10
#define PIN4 11
#define PIN5 12

#define NUMPIXELS 6
#define MAX_COMPONENTS 5
#define LEDS_BAR 3
#define START_BUTTON 4

#define BALL_X_INIT_POS 2
#define BALL_Y_INIT_POS 2

struct Point{
  int x=NULL;
  int y=NULL;
};

LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);

Adafruit_NeoPixel enemy_bar = Adafruit_NeoPixel(NUMPIXELS, PIN1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel background1 = Adafruit_NeoPixel(NUMPIXELS, PIN2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel background2 = Adafruit_NeoPixel(NUMPIXELS, PIN3, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel background3 = Adafruit_NeoPixel(NUMPIXELS, PIN4, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel user_bar = Adafruit_NeoPixel(NUMPIXELS, PIN5, NEO_GRB + NEO_KHZ800);

Adafruit_NeoPixel array_obj[] = {enemy_bar, background1, background2, background3, user_bar};

int score = 0;
String moviment="move_down";
int delayval = 800;
bool game=false;
Point ball_position = {BALL_X_INIT_POS, BALL_Y_INIT_POS};
unsigned int user_position = ball_position.y;
unsigned int enemy_position = ball_position.y;
int button_start_state = 0;

void verify_colision(){
  if (moviment == "move_down"){
    if (ball_position.x == MAX_COMPONENTS-1){
      moviment = "game_off";
    }
    else if (ball_position.x+1 == MAX_COMPONENTS-1 && ball_position.y == user_position){
      moviment="move_up_left";
    }
    else if (ball_position.x+1 == MAX_COMPONENTS-1 && ball_position.y == user_position+1){
      moviment="move_up";
    }
    else if (ball_position.x+1 == MAX_COMPONENTS-1 && ball_position.y == user_position+2){
      moviment="move_up_right";
    }
    else{
      move_down();
    }
  }
  else if (moviment == "move_up"){
    if (ball_position.x-1 == 0 && ball_position.y == enemy_position){
      moviment="move_down_left";
      score++;
    }
    else if (ball_position.x-1 == 0 && ball_position.y == enemy_position+1){
      moviment="move_down";
      score++;
    }
    else if (ball_position.x-1 == 0 && ball_position.y == enemy_position+2){
      moviment="move_down_right";
      score++;
    }
    else{
      move_up();
    }
  }
  else if (moviment == "move_down_left"){
    if (ball_position.x == MAX_COMPONENTS-1){
      moviment = "game_off";
    }
    else if (ball_position.x+1 == MAX_COMPONENTS-1 && ball_position.y-1 == user_position){
      moviment="move_up_left";
    }
    else if (ball_position.x+1 == MAX_COMPONENTS-1 && ball_position.y-1 == user_position+1){
      ball_position.y -= 1;
      moviment="move_up";
    }
    else if (ball_position.x+1 == MAX_COMPONENTS-1 && ball_position.y-1 == user_position+2){
      moviment="move_up_right";
    }
    else if (ball_position.y-1 < 0){
      moviment="move_down_right";
    }
    else{
      move_down_left();
    }
  }
  else if (moviment == "move_down_right"){
    if (ball_position.x == MAX_COMPONENTS-1){
      moviment = "game_off";
    }
    else if (ball_position.x+1 == MAX_COMPONENTS-1 && ball_position.y+1 == user_position){
      moviment="move_up_left";
    }
    else if (ball_position.x+1 == MAX_COMPONENTS-1 && ball_position.y+1 == user_position+1){
      ball_position.y += 1;
      moviment="move_up";
    }
    else if (ball_position.x+1 == MAX_COMPONENTS-1 && ball_position.y+1 == user_position+2){
      moviment="move_up_right";
    }
    else if (ball_position.y+1 > NUMPIXELS-1){
      moviment="move_down_left";
    }
    else{
      move_down_right();
    }
  }
  else if (moviment == "move_up_left"){
    if (ball_position.x-1 == 0 && ball_position.y-1 == enemy_position){
      moviment="move_down_left";
      score++;
    }
    else if (ball_position.x-1 == 0 && ball_position.y-1 == enemy_position+1){
      ball_position.y -= 1;
      moviment="move_down";
      score++;
    }
    else if (ball_position.x-1 == 0 && ball_position.y-1 == enemy_position+2){
      moviment="move_down_right";
      score++;
    }
    else if (ball_position.y-1 < 0){
      moviment="move_up_right";
    }
    else{
      move_up_left();
    }
  }
  else if (moviment == "move_up_right"){
    if (ball_position.x-1 == 0 && ball_position.y+1 == enemy_position){
      score++;
      moviment="move_down_left";
    }
    else if (ball_position.x-1 == 0 && ball_position.y+1 == enemy_position+1){
      ball_position.y += 1;
      score++;
      moviment="move_down";
    }
    else if (ball_position.x-1 == 0 && ball_position.y+1 == enemy_position+2){
      score++;
      moviment="move_down_right";
    }
    else if (ball_position.y+1 > NUMPIXELS-1){
      moviment="move_up_left";
    }
    else{
      move_up_right();
    }
  }
  else{
    game = false;
    game_over();
  }
}

void move_down_right(){
  for (int i=0; i < MAX_COMPONENTS-1; i++){
    if (i == ball_position.x){
        array_obj[i+1].setPixelColor(ball_position.y + 1, array_obj[i+1].Color(0, 0, 255));
        ball_position.x += 1;
        ball_position.y += 1;
        break;
    }
  }
}

void move_down_left(){
  for (int i=0; i < MAX_COMPONENTS-1; i++){
    if (i == ball_position.x){
        array_obj[i+1].setPixelColor(ball_position.y-1, array_obj[i+1].Color(0, 0, 255));
        ball_position.x += 1;
        ball_position.y -= 1;
        break;
    }
  }
}

void move_up_right(){
  for (int i=MAX_COMPONENTS-1; i > 0; i--){
    if (i == ball_position.x){
        array_obj[i-1].setPixelColor(ball_position.y + 1, array_obj[i-1].Color(0, 0, 255));
        ball_position.x -= 1;
        ball_position.y += 1;
        break;
    }
  }
}

void move_up_left(){
  for (int i=MAX_COMPONENTS-1; i > 0; i--){
    if (i == ball_position.x){
        array_obj[i-1].setPixelColor(ball_position.y-1, array_obj[i-1].Color(0, 0, 255));
        ball_position.x -= 1;
        ball_position.y -= 1;
        break;
    }
  }
}

void move_up(){
  for (int i=MAX_COMPONENTS-1; i > 0; i--){
    if (i == ball_position.x){
      array_obj[i-1].setPixelColor(ball_position.y, array_obj[i-1].Color(0, 0, 255));
      ball_position.x -= 1;
      break;
    }
  }
}

void move_down(){
  for (int i=0; i < MAX_COMPONENTS; i++){
    if (i == ball_position.x){
      array_obj[i+1].setPixelColor(ball_position.y, array_obj[i+1].Color(0, 0, 255));
      ball_position.x += 1;
      break;
    }
  }
}

void update_ball(){
  for (int i=0; i < MAX_COMPONENTS; i++){
    if (i == ball_position.x){
      array_obj[i].setPixelColor(ball_position.y, user_bar.Color(0, 0, 255));
    }
  }
}

void clear_leds(){
  for (int i=0; i < MAX_COMPONENTS; i++){
  	array_obj[i].clear();
  }
}

void begin_neo_pixels(){
  for (int i=0; i < MAX_COMPONENTS; i++){
  	array_obj[i].begin();
  }
}

void show_leds(){
  for (int i=0; i < MAX_COMPONENTS; i++){
  	array_obj[i].show();
  }
}

void decrement(){
  if (user_position != 0 ){
  	user_position--;
  }
}

void increment(){
  if (user_position+LEDS_BAR != NUMPIXELS){
    user_position++;
  }
}

void updateUserBar(){
  for (int i=user_position; i< user_position+LEDS_BAR; i++){
    user_bar.setPixelColor(i, user_bar.Color(0, 255, 0));
  }
}

void updateEnemyBar(){
  if (ball_position.y+LEDS_BAR <= NUMPIXELS){
    for (int i=ball_position.y; i< ball_position.y+LEDS_BAR; i++){
          enemy_position=ball_position.y;
          enemy_bar.setPixelColor(i, enemy_bar.Color(255, 0, 0));
    }
  }
  else{
    for (int i=enemy_position; i< enemy_position+LEDS_BAR; i++){
          enemy_bar.setPixelColor(i, enemy_bar.Color(255, 0, 0));
    }
  }
}

void update_lcd(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SCORE:");
  lcd.setCursor(7, 0);
  lcd.print(score);
}

void setup() {
  attachInterrupt(digitalPinToInterrupt(2), increment, RISING);
  attachInterrupt(digitalPinToInterrupt(3), decrement, RISING);
  pinMode(START_BUTTON, INPUT);
  begin_neo_pixels();
  lcd.begin(16, 2);
  lcd.print("Welcome to");
  lcd.setCursor(0, 1);
  lcd.print("Pong Game");
  Serial.begin(9600);
}

void reset_variables(){
  game ? game=false : game=true;
  ball_position.x = BALL_X_INIT_POS;
  ball_position.y = BALL_Y_INIT_POS;
  user_position = ball_position.y;
  enemy_position = ball_position.y;
  moviment="move_down";
  score = 0;
  if (!game){
    game_over();
  }
}

void game_over(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Game Over");
  lcd.setCursor(0, 1);
  lcd.print("Final Score:");
  lcd.setCursor(12, 1);
  lcd.print(score);
}

void loop() {
  button_start_state = digitalRead(START_BUTTON);
  if (button_start_state){
    reset_variables();
  }
  if (game){
    clear_leds();
    update_lcd();
    updateUserBar();
    updateEnemyBar();
    update_ball();
    show_leds();
    verify_colision();
    delay(delayval);
  }
}

