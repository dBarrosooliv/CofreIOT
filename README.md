# 🔐 Cofre Inteligente com Feedback Multissensorial

Projeto final da disciplina **CCM520 – Internet das Coisas**  
Centro Universitário FEI – 1º Semestre de 2026

---

## 📋 Descrição

Sistema de controle de acesso para um compartimento seguro, implementado com Arduino Uno. O usuário insere uma senha de 4 dígitos por meio de botões físicos; o sistema valida a entrada, controla uma tranca via servomotor e fornece feedback visual (LCD e LEDs) e sonoro (buzzer). Após 3 tentativas incorretas, o cofre entra em modo de alerta com alarme contínuo.

---

## 🧰 Hardware Utilizado

| Componente | Quantidade | Função |
|---|---|---|
| Arduino Uno | 1 | Microcontrolador principal |
| Display LCD 16x2 | 1 | Interface visual com o usuário |
| Servomotor | 1 | Tranca mecânica da porta |
| Buzzer | 1 | Feedback sonoro (teclas, erro, acerto, alarme) |
| LED Vermelho | 3 | Indicadores de tentativas erradas (A0, A1, A2) |
| LED Verde | 1 | Indicador de acesso liberado (A4) |
| Botão (Push Button) | 5 | Entrada da senha (dígitos 1–4) + Reset |
| Resistores 1kΩ | 6 | Limitadores de corrente dos LEDs |
| Potenciômetro 250kΩ | 1 | Ajuste de contraste do LCD |
| Sensor Infravermelho | 1 | Monitoramento do estado físico da porta (A5) |
| Protoboard + Jumpers | — | Montagem do circuito |

---

## 🔌 Mapeamento de Pinos

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

## ⚙️ Funcionalidades Implementadas

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

### Monitoramento da Porta
- O sensor infravermelho em A5 verifica se a porta está fisicamente aberta
- O servo só destrava (`write(95)`) quando a senha está correta **e** o sensor confirma a abertura da porta
- Enquanto a porta não for aberta, o display permanece em estado de espera

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

## 🖥️ Interface do LCD

**Linha 0 (status):**
```
Estado: fechado
Estado: Aberto..
Estado: ALERTA!
```

**Linha 1 (entrada da senha):**
```
< > < > < > < >
```
À medida que o usuário pressiona os botões, os `<` e `>` são substituídos pelos dígitos digitados nas colunas 1, 5, 9 e 13.

---

## 🚀 Como Reproduzir

### Requisitos
- Arduino IDE 1.8+ ou 2.x
- Bibliotecas: `LiquidCrystal` (nativa) e `Servo` (nativa)

### Montagem
1. Monte o circuito conforme o diagrama de conexão (`Cofre.pdf`)
2. Conecte o Arduino ao computador via USB

### Upload
1. Abra o arquivo `cofre.ino` na Arduino IDE
2. Selecione a placa **Arduino Uno** e a porta COM correta
3. Clique em **Upload**

### Uso
1. Ao iniciar, o LCD exibe `"Estado: fechado"`
2. Pressione os 4 botões na sequência correta: **4 → 2 → 2 → 3**
3. Se a senha estiver correta, o servo destrava e o LED verde acende
4. Pressione o botão de **Reset** (pino 2) a qualquer momento para reiniciar o sistema

> ⚠️ A senha padrão é `4 2 2 3`. Para alterá-la, modifique o array `senha[4]` no código-fonte.

---

## 📁 Estrutura do Repositório

```
/
├── cofre.ino          # Código-fonte principal (comentado)
├── Cofre.pdf          # Diagrama de conexão do circuito (Tinkercad)
├── relatorio.docx     # Relatório técnico completo
└── README.md          # Este arquivo
```

---

## 👥 Autores

Projeto desenvolvido para a disciplina CCM520 – Internet das Coisas  
Centro Universitário FEI – 2026
