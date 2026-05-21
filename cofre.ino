#include <LiquidCrystal.h>
#include <Servo.h>

// Pinos do LCD (RS, E, D4, D5, D6, D7)
LiquidCrystal lcd_1(12, 11, 10, 9, 8, 7);

volatile int counter = 0;

int estado = 0; // 0 = Fechado, 1 = Aberto, 2 = Alerta/Bloqueado
int bt;
int posicoes[4] = {0,0,0,0}; // Guarda os números digitados
int senha[4] = {4,2,2,3};    // Senha correta do cofre

int colunas[4] = {1,5,9,13}; // Posições no LCD para os números da senha
int erros = 0;
int porta = A5;
Servo servoMotor;

int lastCounter = -1; // Para atualizar o LCD apenas quando o valor mudar

void setup () {
  lcd_1.begin (16, 2);
  
  // Configura os pinos dos botões com resistores internos
  pinMode (2, INPUT_PULLUP);
  pinMode (3, INPUT_PULLUP);
  pinMode (4, INPUT_PULLUP); 
  pinMode (5, INPUT_PULLUP);
  pinMode (6, INPUT_PULLUP);
  
  // LEDs indicadores de erro
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  
  // LED indicador de acerto (Cofre aberto)
  pinMode(A4, OUTPUT);
  
  // Buzzer para feedback sonoro
  pinMode(A3, OUTPUT);
  
  // Configuração inicial do Servo (Tranca)
  servoMotor.attach(13);
  servoMotor.write(0);
  
  // Sensor infravermelho da porta
  pinMode(porta, INPUT);
  
  // Configura as interrupções de hardware para o botão de reset e o botão 2
  attachInterrupt(digitalPinToInterrupt(2), resetar_cofre, FALLING);
  attachInterrupt(digitalPinToInterrupt(3), dDois, FALLING);
}

void loop () {
  
  int estadoSensor = digitalRead(A5);
  
  // Leitura dos botões 1, 3 e 4 (o botão 2 está na interrupção)
  if (digitalRead(6) == LOW){ 
    delay(200); // Debounce simples
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
  
  // Atualiza o sistema apenas se um botão foi pressionado (counter mudou)
  if (counter != lastCounter) {

    // Grava o botão pressionado na primeira posição vazia
    for(int i = 0; i < 4; i++){
        if(posicoes[i] == 0){
          posicoes[i] = bt;
          break;
        }
    }
    
    // Verifica se as 4 posições já foram preenchidas
    if(posicoes[3] != 0){
      int ok = 1;

      // Compara a senha digitada com a senha correta
      for(int i = 0; i < 4; i++){
          if(posicoes[i] != senha[i]){
              ok = 0;
          }
      }
      
      // Se a senha estiver correta
      if(ok == 1){
        estado = 1;
        erros = 0;
        
        // Toca som de sucesso
        tone(A3, 1200, 100);
        delay(80);
        tone(A3, 1600, 100);
        
      } else { // Se a senha estiver errada
        // Toca som de erro
        tone(A3, 70, 100);
        delay(80);
        tone(A3, 70, 100);
        
        estado = 0;
        erros++; // Aumenta o contador de erros
      }
      
      // Mostra a última tentativa no LCD temporariamente
      for(int i = 0; i < 4; i++){
          if(posicoes[i] != 0){
            lcd_1.setCursor(colunas[i], 1);
            lcd_1.print(posicoes[i]);
          }
      }
      
      delay(500);
      
      // Limpa as posições para uma nova tentativa
      for (int i = 0; i < 4; i++){
        posicoes[i] = 0;
      }
    }

    // Acende os LEDs de erro progressivamente
    if(erros == 1){
        digitalWrite(A0, HIGH);
    } else if(erros == 2){
        digitalWrite(A1, HIGH);
    } else if(erros >= 3){
        digitalWrite(A2, HIGH);
        estado = 2; // Bloqueia o cofre após 3 erros
    }
    
    // Atualiza o Display LCD de acordo com o estado do cofre
    lcd_1.setCursor(0, 0);
    lcd_1.print("Estado:"); 
    lcd_1.setCursor(7, 0);
    
    if (estado == 0){
      lcd_1.print("fechado");
      digitalWrite(A4, LOW);
      servoMotor.write(0); // Tranca a porta
    }
    else if(estado == 1 && estadoSensor == LOW){
      lcd_1.print("Aberto..");
      erros = 0;
      // Apaga LEDs de erro e acende o de sucesso
      digitalWrite(A0, LOW);
      digitalWrite(A1, LOW);
      digitalWrite(A2, LOW);
      digitalWrite(A4, HIGH);
      servoMotor.write(95); // Destranca a porta
    }
    else if(estado == 2){ // Estado de Alerta (3 erros)
      lcd_1.print("ALERTA!");
      digitalWrite(A4, LOW);
      
      // Dispara o alarme sonoro
      for(int i = 0; i < 10; i++){
        delay(1000);
        tone(A3, 900, 200);
        delay(200);
        tone(A3, 1500, 200);
      }
    }
    
    // Desenha a interface inferior do LCD
    lcd_1.setCursor(0, 1);
    lcd_1.print("< > < > < > < >");
    
    // Preenche a interface com os números que já foram digitados
    for(int i = 0; i < 4; i++){
        if(posicoes[i] != 0){
          lcd_1.setCursor(colunas[i], 1);
          lcd_1.print(posicoes[i]);
        }
    }
    
    Serial.print(erros);
      
  } // Fim do if (counter != lastCounter)
  
  lastCounter = counter;  
}

// --- Funções de leitura dos botões ---
void dUm () {
    bt = 1;
    counter++;
    tone(A3, 600, 110);
}

void dDois () {
    bt = 2;
    counter++;
    tone(A3, 700, 110);
}

void dTres () {
    bt = 3;
    counter++;
    tone(A3, 800, 110);
}

void dQuatro () {
    bt = 4;
    counter++;
    tone(A3, 900, 110);
}

// --- Função chamada pela interrupção para resetar tudo ---
void resetar_cofre () {
    noTone(A3);          
    erros = 0;           
    estado = 0;          
    counter++;           
    
    // Apaga todos os LEDs
    digitalWrite(A0, LOW);
    digitalWrite(A1, LOW);
    digitalWrite(A2, LOW);
    digitalWrite(A4, LOW);
    
    // Zera a senha digitada
    for (int i = 0; i < 4; i++){
        posicoes[i] = 0;
    }
    
    // Toca som confirmando o reset
    tone(A3, 500, 200);  
}
