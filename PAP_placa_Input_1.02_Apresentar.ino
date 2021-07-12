#include <Ethernet.h>
#include <SPI.h>

byte mac[] = {   0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };  // Variaveis ou Objeto para a Ligação á Internet 
EthernetClient client;


//  IP do SIC
char server[] = "192.168.1.3"; // 

long ct=0; 
long ct2=0;

// variáveis para o butão e led

//estados dos leds
const int ledvermelho = 0; //Estado lógico inicial
const int ledverde = 1;    
const int gasPin = A1; //GAS sensor output pin to Ar+duino analog A0 pin 
float GAS;
//variáveis dos pinos
const int buttonPin = 12; 
const int buttonPin2 = 9;
const int buttonPin3 = 8;
const int buttonPin4 = 7;     
const int ledPin =  10;   
int led2 = 11; //led vermelho
int buttonState = 0; //Estado lógico inicial
int buttonState2 =0; //Estado lógico inicial
int buttonState3 =0; //Estado lógico inicial
int buttonState4= 0; //Estado lógico inicial

int contador=1;  //Estado lógico inicial
int contador2=1;  //Estado lógico inicial


//TMP36 Pin Variables
float temp; 
int sensorPin = A0; //O PIN ANALÓGICO DO tmp36 Vout, tem uma resolução de 10mV por grau centígrado com limite de 500mV para permitir temperaturas negativas.
                        
                        
int tempMin = 22;   // temperatura mínima
int tempMax = 35;   //temperatura máxima
int fan = 6;       // o pin onde o ventilador esta ligado
int fanSpeed = 0;
int tolerancia = 6; //Tolerância em graus, para evitar o ligar e desligar constante sempre que a temperatura máxima é atingida, sengo agora a tempMax = tempMin+tolerancia = 22+6 = 28 ºc
                    //Graças a isto, o ventilador só para de trabalhar quando a temperatura chega a 22 ºc
                    //Sem isto, o ventilador começava e parava constantemente entre valores próximos de 28 ºc para manter a temperatura abaixo dos 28 ºc
int GASMAX =750;
int GASMIN =35;
int TOLERANCIA=200;
int FAN =3;
int FAN2 =4;

int Led = 13 ; // define LED Interface
int SENSOR = 5 ; // define the Hall magnetic sensor interface
int val ; // define numeric variables val
int estado1 = 0;

//Geiger
// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
//LiquidCrystal lcd(3,4,5,6,7,8);


// Threshold values for the led bar
#define TH1 45
#define TH2 95
#define TH3 200
#define TH4 400
#define TH5 600

// Conversion factor - CPM to uSV/h
#define CONV_FACTOR 0.00812

// Variables
//int ledArray [] = {10,11,12,13,9};
int geiger_input = 2;
long count = 0;
long countPerMinute = 0;
long timePrevious = 0;
long timePreviousMeassure = 0;
long time = 0;
long countPrevious = 0;
float radiationValue = 0.0;
// Fim Geiger

//função de configuração 

void setup()
{

  Serial.begin(115200);   


//Auto Ethernet
  // iniciação da ligação à internet:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Verifique o cabo de RJ45");
    // Não faz mais nada se o problema for físico. Aqui isto somente acontece quando o mac address entra em conflito, ou seja, quando não houver internet ou falha do cabo.
    resetEthernetShield();
    //por algum motivo ele não chama resetEthernetShield().
    for(;;)
      ;
  }
  // Imprime o endereço de IP local atribuido à placa arduino
  Serial.println("IP adquirido");
  Serial.println(Ethernet.localIP());
//Fim auto Ethernet

   Ethernet.begin(mac,Ethernet.localIP()); 
  //server.begin();
  delay(1000);
  Serial.println("Ligando...");
  postData();
  
  //lcd.begin(16, 2);
 // lcd.setCursor(0,0);
   //Serial.begin(115200);  

 // o led a ser ativado
  pinMode(ledPin, OUTPUT);
   pinMode(led2, OUTPUT); 
  // o butão de este para premir
  pinMode(buttonPin, INPUT);

  // ativar as colunas e linhas do lcd:
  


//Pinos do ventilador e sensor
 pinMode(fan, OUTPUT);
 pinMode(sensorPin, INPUT);

   pinMode (Led, OUTPUT) ;    // define LED as output interface
  pinMode (SENSOR, INPUT) ;  // define the Hall magnetic sensor line as input

analogReference(EXTERNAL);

//Geiger
  pinMode(geiger_input, INPUT);
  digitalWrite(geiger_input,HIGH);
  for (int i=0;i<5;i++){
//    pinMode(ledArray[i],OUTPUT);
  }



  attachInterrupt(0,countPulse,FALLING);
//Fim Geiger
}


// postData, para enviar os dados
void postData() {

 

  // Se a ligação for bem sucedida
  if (client.connect(server, 22000)) {
    Serial.println("Ligado");
    client.println("Arduino ligado");
//O cliente notifica
   
  } 
  else {
    // Se a ligação falhar
    Serial.println("Ligação falhou");
    Serial.println("Deslligando");
    resetEthernetShield();
    //avisa e chama a função reset. isto funciona quando o servidor está desligado, o arduino insiste indefenidamente pela ligação IP que foi configurada.
    //é ótimo para não termos que fazer reset nós mesmos.
  }
}

void loop() {

   carregarBotao1();
   delay(1);        // delay para estabilidade
   carregarBotao2();
   delay(1);        // delay para estabilidade

   carregarBotao3();
   delay(1);        // delay para estabilidade
   
   carregarBotao4();
   delay(1);        // delay para estabilidade

//os botões foram postos separadamente para melhor segurança e organização
   ventilador1();
   delay(1);

   gas();
   delay(1);

   doorsensor();
   delay(1);

   GEIGER();
   delay(1);

}

void carregarBotao1() {
if((debounceButton(buttonPin)==1) && (buttonState==0))
{
  buttonState=1;
   Serial.println("Nurse Call");
   client.println("Nurse Call");
   client.println("Nurse Call");
   client.println("Nurse Call");
   client.println("Nurse Call");
       digitalWrite(ledPin, HIGH);
    digitalWrite(led2, LOW);
  //aqui teremos a menságem dependente do estado do botão pressionado
}

if((debounceButton(buttonPin)==0) && (buttonState==1))
{
  buttonState=0;
      digitalWrite(ledPin, LOW);
    digitalWrite(led2, HIGH);
}  //
  
}

void carregarBotao2() {
if((debounceButton2(buttonPin2)==1) && (buttonState2==0))
{
  buttonState2=1;
   Serial.println("Next Patient");
      client.println("Next Patient");
      client.println("Next Patient");
      client.println("Next Patient");
      client.println("Next Patient");
       digitalWrite(ledPin, HIGH);
    digitalWrite(led2, LOW);
   //aqui teremos mais uma menságem dependente do estado do botão pressionado
}

if((debounceButton2(buttonPin2)==0) && (buttonState2==1))
{
  buttonState2=0;
      digitalWrite(ledPin, LOW);
    digitalWrite(led2, HIGH);
}  //  
}

void carregarBotao3() {

if((debounceButton3(buttonPin3)==1) && (buttonState3==0))
{
  buttonState3=1;
   Serial.println("X Ray room");
      client.println("X Ray room");
      client.println("X Ray room");
      client.println("X Ray room");
      client.println("X Ray room");
       digitalWrite(ledPin, HIGH);
    digitalWrite(led2, LOW);
 //aqui teremos mais outra menságem dependente do estado do botão pressionado
}

if((debounceButton3(buttonPin3)==0) && (buttonState3==1))
{
  buttonState3=0;
      digitalWrite(ledPin, LOW);
    digitalWrite(led2, HIGH);
}  //  
    
}

void carregarBotao4() {
if((debounceButton4(buttonPin4)==1) && (buttonState4==0))
{
  buttonState4=1;
   Serial.println("Maternity Ward");
      client.println("Maternity Ward");
      client.println("Maternity Ward");
      client.println("Maternity Ward");
      client.println("Maternity Ward");
       digitalWrite(ledPin, HIGH);
    digitalWrite(led2, LOW);
 //e a menságem do último botão
}

if((debounceButton4(buttonPin4)==0) && (buttonState4==1))
{
  buttonState4=0;
      digitalWrite(ledPin, LOW);
    digitalWrite(led2, HIGH);
}  //      
}

void ventilador1() {
  //fan function
//getting the voltage reading from the temperature sensor
 int reading = analogRead(sensorPin);  
 
 // converting that reading to voltage, for 3.3v arduino use 3.3
 float voltage = reading * 5.0;
 voltage /= 1024.0; 
 
 // print out the voltage

 
 // now print out the temperature
 float temperatureC = (voltage - 0.5) * 100 ;  //converting from 10 mv per degree wit 500 mV offset
                                               //to degrees ((voltage - 500mV) times 100)

 // agora para fahrenheit
 float temperatureF = (temperatureC * 9.0 / 5.0) + 32.0;
 if (ct==100)
 {
   //Serial.print("Voltage ");
   Serial.print(voltage); 
   Serial.println(" volts");
 //  Serial.print("Degrees C");
   Serial.print(temperatureC);
   Serial.print(" Celsius ");
   client.println(temperatureC);//  && client.print(" ºC");
 //Serial.print(temperatureF); Serial.println(" degrees F");
 //Serial.println();
 ct=0;
 }
 delayMicroseconds(1);                                     //waiting a second
ct++;
if(temperatureC < tempMin) {   // se a temperatura for mais baixa que o valor mínimo+tolerancia
       fanSpeed = 0;      // ventilador fica imóvel
       digitalWrite(fan, LOW);
   } 
   if((temperatureC >= tempMin+tolerancia) && (temperatureC <= tempMax))  //se a temperatura for mais alta que o valor mínimo+tolerancia
   { 
       fanSpeed = map(temp, tempMin, tempMax, 240, 255); // A velocidade atual do ventilador 0-64-127-255
       analogWrite(fan, fanSpeed);  //  Rodar o ventilador
       // client.println("Ventilador ativo");
       if(contador2 != 0){
        client.print("Cooling active");
        client.print("Cooling active");
        client.print("Cooling active");
        client.print("Cooling active");
        contador2=1;
       }
   } 
// 
  }

  void gas(){


      //fan function
//getting the voltage reading from the temperature sensor
 int reading2 = analogRead(gasPin);  

      if (ct2=10000000)
  {
  Serial.println(analogRead(gasPin));
   Serial.print("GAS ");
  ct2=0;
  }
  delayMicroseconds(1);
  ct2++;

if(reading2 < GASMIN) {   // se o valor do gás for mais baixo que o valor mínimo+tolerancia
       fanSpeed = 0;      // ventilador fica imóvel
       digitalWrite(FAN, LOW);
       digitalWrite(FAN2, LOW);
   } 
   if((reading2 >= GASMIN+TOLERANCIA) && (reading2 <= GASMAX))  //se o valor do gás for mais alto que o valor mínimo+tolerancia
   { 
       fanSpeed = map(GAS, GASMIN, GASMAX, 255, 255); // A velocidade atual do ventilador 0-64-127-255
       analogWrite(FAN, fanSpeed);  //  Rodar o ventilador
        analogWrite(FAN2, fanSpeed);  //  Rodar o ventilador
        if(contador != 0){
          client.println("Exhaust active");
          client.println("Exhaust active");
          client.println("Exhaust active");
          client.println("Exhaust active");
          contador=0;
        }   
      //  Serial.print(" Ventilador ativo ");
   } 
 }

    void doorsensor(){
       val = digitalRead (SENSOR) ; // read sensor line
       
  if((val==1) && (estado1==0))
  {
   estado1=1;
     client.println("Door Open");
     client.println("Door Open");
     client.println("Door Open");
     client.println("Door Open");
     Serial.print(" Door Open ");
     
  }
  if((digitalRead(SENSOR)==0) && (estado1==1))
  {
    estado1=0;
     client.println("Door Closed");
     client.println("Door Closed");
     client.println("Door Closed");
     client.println("Door Closed");
     Serial.print(" Door Closed");
  } 
      }


  

//aqui estão as funções debounce que serão chamadas para evitar contagens erradas devido às oscilações metálicas dos botões
boolean debounceButton(boolean state)
{
  boolean stateNow = digitalRead(buttonPin);
  if(state!=stateNow)
  {
    delay(10);
    stateNow = digitalRead(buttonPin);
  }
  return stateNow;
  
}

boolean debounceButton2(boolean state)
{
  boolean stateNow = digitalRead(buttonPin2);
  if(state!=stateNow)
  {
    delay(10);
    stateNow = digitalRead(buttonPin2);
  }
  return stateNow;
  
}

boolean debounceButton3(boolean state)
{
  boolean stateNow = digitalRead(buttonPin3);
  if(state!=stateNow)
  {
    delay(10);
    stateNow = digitalRead(buttonPin3);
  }
  return stateNow;
  
}

boolean debounceButton4(boolean state)
{
  boolean stateNow = digitalRead(buttonPin4);
  if(state!=stateNow)
  {
    delay(10);
    stateNow = digitalRead(buttonPin4);
  }
  return stateNow;
  
}


void GEIGER(){
    if (millis()-timePreviousMeassure > 10000){
    countPerMinute = 6*count;
    radiationValue = countPerMinute * CONV_FACTOR;
    timePreviousMeassure = millis();
    Serial.print("cpm = "); 
    Serial.print(countPerMinute,DEC);
    Serial.print(" - ");
    Serial.print("uSv/h = ");
    Serial.println(radiationValue,4);
        //client.println("         cpm = "); 
    client.println(countPerMinute,DEC);
   
//  lcd.clear();    
//  lcd.setCursor(0, 0);
 // lcd.print("CPM=");
  //lcd.setCursor(4,0);
 // lcd.print(countPerMinute);  //  lcd.setCursor(0,1);
  //lcd.print(radiationValue,4);
  //lcd.setCursor(6,1);
  //lcd.print(" uSv/h");

    //led var setting  
    if(countPerMinute <= TH1) ledVar(0);
    if((countPerMinute <= TH2)&&(countPerMinute>TH1)) ledVar(1);
    if((countPerMinute <= TH3)&&(countPerMinute>TH2)) ledVar(2);
    if((countPerMinute <= TH4)&&(countPerMinute>TH3)) ledVar(3);
    if((countPerMinute <= TH5)&&(countPerMinute>TH4)) ledVar(4);
    if(countPerMinute>TH5) ledVar(5);

    count = 0;

  }
    }


    

  

void countPulse(){
  detachInterrupt(0);
  count++;
  while(digitalRead(2)==0){
  }
  attachInterrupt(0,countPulse,FALLING);
}

void ledVar(int value){
  if (value > 0){
    for(int i=0;i<=value;i++){
//      digitalWrite(ledArray[i],HIGH);
    }
    for(int i=5;i>value;i--){
   //   digitalWrite(ledArray[i],LOW);
    }
  }
  else {
    for(int i=5;i>=0;i--){
   //   digitalWrite(ledArray[i],LOW);
    }
  }
}







//provavelmente uma das funções mais interessantes, resetEthernetShield()torna possível mandar o arduino tentar ligar-se de novo à internet quando a ligação falhar.

//estou a pensar numa estrutura if client disconected..... resetEthernetShield(), mais no futuro.
void resetEthernetShield() //
{  
  delay(100);
  Serial.println("Fazendo reset à Ethernet");
  client.stop();
 //Auto Ethernet
  // iniciação da ligação à internet:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Verifique o cabo de RJ45");
    // Não faz mais nada se o problema for físico
    for(;;)
      ;
  }
  // Imprime o endereço de IP local atribuido à placa arduino
  Serial.println("IP adquirido");
  Serial.println(Ethernet.localIP());
//Fim auto Ethernet

   Ethernet.begin(mac,Ethernet.localIP()); 
  //server.begin();
  delay(1000);
  Serial.println("Ligando...");
  delay(100);
    postData();
}
