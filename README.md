# 🔐 Projeto de Cofre Eletrônico

Integrantes:
Daniel Barroso de Oliveira R.A: 22.124.054-2

Kaio Lopes Custódio da Silva R.A:

Gustavo Reis Teixeira R.A:

---

## Descrição

Sistema de controle de acesso para um compartimento seguro, implementado com Arduino Uno. O usuário insere uma senha de 4 dígitos por meio de botões físicos; o sistema valida a entrada, controla uma tranca via servomotor e fornece feedback visual (LCD e LEDs) e sonoro (buzzer). Após 3 tentativas incorretas, o cofre entra em modo de alerta com alarme contínuo.

---

## Hardware Utilizado

| Nome | Quantidade | Componente |
| :--- | :---: | :--- |
| U1 | 1 | Arduino Uno R3 |
| U2 | 1 | LCD 16 x 2 |
| Rpot1 | 1 | 250 kΩ Potenciômetro |
| S1<br>S2<br>S3<br>S5<br>S4 | 5 | Botão |
| D1<br>D2<br>D3 | 3 | Vermelho LED |
| R6<br>R7<br>R8<br>R1<br>R2 | 5 | 1 kΩ Resistor |
| PIEZO1 | 1 | Piezo |
| SERVO1 | 1 | Posicional Micro servo |
| D4 | 1 | Verde LED |
| U3 | 1 | Sensor de infravermelho |

---

## Mapeamento de Pinos

| Pino Arduino | Componente | Descrição |
|---|---|---|
| D2 | Botão Reset | Interrupção externa – reinicia o sistema |
| D3 | Botão 2 | Interrupção externa – digita o número 2 |
| D4 | Botão 4 | Digita o número 4 |
| D5 | Botão 3 | Digita o número 3 |
| D6 | Botão 1 | Digita o número 1 |
| D7–D10 | LCD (D4–D7, E, RS) | Dados e controle do display |
| D11 | LCD Enable | Controle do display |
| D12 | LCD RS | Controle do display |
| D13 | Servomotor | Sinal PWM para a tranca |
| A0 | LED Vermelho 1 | 1º erro |
| A1 | LED Vermelho 2 | 2º erro |
| A2 | LED Vermelho 3 | 3º erro (ativa alerta) |
| A3 | Buzzer | Saída de som |
| A4 | LED Verde | Cofre aberto |
| A5 | Sensor IR | Monitoramento da porta |

---

## Funcionalidades 

### Entrada de Senha
- 4 botões físicos representam os dígitos 1, 2, 3 e 4
- O usuário pressiona a sequência de 4 dígitos; o LCD exibe cada dígito na posição correspondente
- Cada tecla emite um bipe de frequência distinta como confirmação

### Validação e Estados
O sistema opera como uma **Máquina de Estados Finita (FSM)** com três estados:

| Estado | Valor | Descrição |
|---|---|---|
| Fechado | 0 | Aguardando senha; servo em 0° |
| Aberto | 1 | Senha correta + sensor detecta porta aberta; servo em 95° |
| Alerta | 2 | 3 erros consecutivos; alarme sonoro ativado |

### Sistema de Bloqueio
- **1º erro:** acende LED vermelho 1 (A0)
- **2º erro:** acende LED vermelho 2 (A1)
- **3º erro:** acende LED vermelho 3 (A2) e transita para estado de **ALERTA**
- Em alerta: o LCD exibe `"ALERTA!"` e o buzzer dispara 10 ciclos de alarme bitonado

### Interrupções de Hardware
- **Pino 2 (FALLING):** `resetar_cofre()` — zera erros, estados, LEDs e senha digitada instantaneamente
- **Pino 3 (FALLING):** `dDois()` — registra o dígito 2 sem polling no `loop()`

### Feedback Multissensorial
| Evento | Som | Visual |
|---|---|---|
| Tecla pressionada | Bipe único (600–900 Hz) | Dígito exibido no LCD |
| Senha correta | Dois bipes ascendentes | LED verde aceso, servo abre |
| Senha errada | Dois bipes graves (70 Hz) | LED de erro acende |
| Alerta (3 erros) | Alarme bitonado (900/1500 Hz) | `"ALERTA!"` no LCD |
| Reset | Bipe de confirmação (500 Hz) | Display zerado |

---

## 📁 Estrutura do Repositório

```
/
├── cofre.ino          # Código-fonte principal (comentado)
├── Mapeamento do Circuito.pdf          # Diagrama de conexão do circuito (Tinkercad)
├── cofre.cpp          # Código completo comentado
├── cofreAberto.jpg          # Circuito montando
└── README.md          # Este arquivo
```

---
![Texto alternativo (opcional)](https://github.com/dBarrosooliv/CofreIOT/blob/main/cofreAberto.jpg)
