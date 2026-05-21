#include <LiquidCrystal.h>
#include <Servo.h>

// Pinos do LCD (RS, E, D4, D5, D6, D7)
LiquidCrystal lcd_1(12, 11, 10, 9, 8, 7);

volatile int counter = 0;

int estado = 0;
int bt;
int posicoes[4]={0,0,0,0};
int senha[4]={4,2,2,3};

int colunas[4] = {1,5,9,13};
int erros =0;
int porta = A5;
Servo servoMotor;

int lastCounter = -1; // Para atualizar o LCD apenas quando o valor mudar

void setup () {
	lcd_1.begin (16, 2);
  
// Configura os pinos dos botões com Pull-up interno
	pinMode (2, INPUT_PULLUP);
	pinMode (3, INPUT_PULLUP);
	pinMode (4, INPUT_PULLUP); 
	pinMode (5, INPUT_PULLUP);
  	pinMode (6, INPUT_PULLUP);
  
//LED's de erro
  	pinMode(A0, OUTPUT);
  	pinMode(A1, OUTPUT);
  	pinMode(A2, OUTPUT);
//LED Acerto
  	pinMode(A4, OUTPUT);
  
//Buzzer
  	pinMode(A3, OUTPUT);
  
//Servo
  	servoMotor.attach(13);
  	servoMotor.write(0);
  
 //Infra
  	pinMode(porta, INPUT);
  
// Configura as interrupções
	attachInterrupt(digitalPinToInterrupt (2), resetar_cofre, FALLING);
	attachInterrupt(digitalPinToInterrupt (3), dDois, FALLING);
}

void loop () {
  
  int estadoSensor = digitalRead(A5);
  
  if (digitalRead(6) == LOW){ 
    delay(200);
    dUm(); 
  }  
  if (digitalRead(5) == LOW){ 
    delay(200);
    dTres(); 
  }
  if (digitalRead(4) == LOW){ 
    delay(100);
    dQuatro(); 
  }
  
  //Somente escrever quando necessário
  if (counter != lastCounter) {

    for(int i=0; i<4; i++){
        if(posicoes[i]==0){
          posicoes[i]= bt;
          break;
        }
    }
    
  	
    if(posicoes[3]!=0){
      int ok = 1;

      //verifica senha
      for(int i =0; i<4; i++){
          if(posicoes[i]!=senha[i]){
              ok = 0;
          }
      }
      
      if(ok==1){
        estado = 1;
		erros = 0;
        
        tone(A3, 1200, 100);
      	delay(80);
      	tone(A3, 1600, 100);
        
      }else{
        tone(A3, 70, 100);
      	delay(80);
      	tone(A3, 70, 100);
        
        estado =0;
        erros++;
      }
	  
      //Mostra no LCD
      for(int i=0; i<4; i++){
          if(posicoes[i]!=0){
            lcd_1.setCursor(colunas[i],1);
            lcd_1.print(posicoes[i]);
          }
      }
      
      delay(500);
      for (int i = 0; i < 4; i++){
        posicoes[i] = 0;
      }
    }


    if(erros==1){
        digitalWrite(A0, HIGH);
    }else if(erros==2){
        digitalWrite(A1,HIGH);
    }else if(erros>=3){
        digitalWrite(A2,HIGH);
        estado =2;
    }
    
    lcd_1.setCursor(0, 0);
    lcd_1.print("Estado:"); 
    lcd_1.setCursor(7, 0);
    if (estado == 0){
      lcd_1.print("fechado");
      digitalWrite(A4, LOW);
      servoMotor.write(0);
    }
    else if(estado == 1 && estadoSensor == LOW){
      lcd_1.print("Aberto..");
      erros = 0;
      digitalWrite(A0, LOW);
      digitalWrite(A1, LOW);
      digitalWrite(A2, LOW);
      digitalWrite(A4, HIGH);
      servoMotor.write(95);
      
    }
    else if(estado == 2){
      lcd_1.print("ALERTA!");
      digitalWrite(A4, LOW);
      
      for(int i =0; i<10; i++){
      	delay(1000);
        tone(A3,900,200);
        delay(200);
      	tone(A3,1500,200);
      }
    }
    
    lcd_1.setCursor(0, 1);
    lcd_1.print("< > < > < > < >");
    
    //Mostra no LCD
	for(int i=0; i<4; i++){
        if(posicoes[i]!=0){
          lcd_1.setCursor(colunas[i],1);
          lcd_1.print(posicoes[i]);
        }
    }
    
    Serial.print(erros);
      
  }
      lastCounter = counter;  
  	  
    
}


void dUm () {
  	bt = 1;
  	counter++;
 	tone(A3,600,110);
}

void dDois () {
	bt = 2;
  	counter++;
    tone(A3,700,110);
}

void dTres () {
	bt = 3;
  	counter++;
  	tone(A3,800,110);
}

void dQuatro () {
  	bt = 4;
  	counter++;
  	tone(A3,900,110);
}

void resetar_cofre () {
    noTone(A3);          
    erros = 0;           
    estado = 0;          
    counter++;           
    
    // Apaga os LEDs de erro
    digitalWrite(A0, LOW);
    digitalWrite(A1, LOW);
    digitalWrite(A2, LOW);
    digitalWrite(A4, LOW);
    
    for (int i = 0; i < 4; i++){
        posicoes[i] = 0;
    }
    
    tone(A3, 500, 200);  
}


