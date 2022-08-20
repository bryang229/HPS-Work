#define U 3
#define V 5
#define X 7
#define Y 8


void setup() {
  pinMode(U, OUTPUT);
  pinMode(V, OUTPUT);
  pinMode(X, OUTPUT);
  pinMode(Y, OUTPUT);
  Serial.begin(9600);
}

void countUp(){
  
int num = 0;
  for(int i = 0; i < 16; i++){
    int u =  num >> 3 & 00000001;//  num >> 3; 
    int v =  num >> 2 & 00000001; // num >> 2 & 1;
    int x = num  >> 1 & 00000001;
    int y = num & 00000001;
    
    Serial.print(u);
    Serial.print(v);
    Serial.print(x);
    Serial.print(y);
    Serial.print(":\t ");
    Serial.print(i);
    Serial.println();
//    Serial.println(11>>1 & 00000001);
    digitalWrite(U, u == 1 ? HIGH : LOW);
    digitalWrite(V, v == 1 ? HIGH : LOW);
    digitalWrite(X, x == 1 ? HIGH : LOW);
    digitalWrite(Y, y == 1 ? HIGH : LOW);
    delay(600);
    num++;
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  countUp();
  delay(200);
}
