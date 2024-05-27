Para implementar esse sistema de alarme com RFID e um LED, você precisará montar um circuito com componentes específicos e fazer as conexões corretamente. Aqui está uma explicação detalhada de como o esquema funcionaria:

### Componentes Necessários
1. **Arduino Uno** (ou qualquer outra placa Arduino compatível)
2. **Módulo RFID MFRC522**
3. **Buzzer**
4. **Relé** (opcional, não usado neste exemplo)
5. **Sensor Magnético** (como um interruptor reed)
6. **Botão de Alarme**
7. **LED**
8. **Resistores** (10kΩ para o botão, conforme necessário)
9. **Jumpers** e **Protoboard**

### Conexões dos Componentes

#### RFID MFRC522
- **SDA (CS)**: Pino 10 no Arduino
- **SCK**: Pino 13 no Arduino
- **MOSI**: Pino 11 no Arduino
- **MISO**: Pino 12 no Arduino
- **IRQ**: Não conectado
- **GND**: GND no Arduino
- **RST**: Pino 9 no Arduino
- **3.3V**: 3.3V no Arduino

#### Buzzer
- **Pino Positivo**: Pino 4 no Arduino
- **Pino Negativo**: GND no Arduino

#### Sensor Magnético
- **Um Terminal**: Pino 2 no Arduino
- **Outro Terminal**: GND no Arduino (ou através de um resistor pull-up para 5V)

#### Botão de Alarme
- **Um Terminal**: Pino 5 no Arduino
- **Outro Terminal**: GND no Arduino (usando um resistor pull-up interno)

#### LED
- **Anodo (perna longa)**: Pino 6 no Arduino
- **Cátodo (perna curta)**: GND no Arduino (através de um resistor, normalmente 220Ω)

### Diagrama de Conexão
Aqui está um diagrama básico para as conexões:

```plaintext
               +-------------+
               |    Arduino  |
               +----+---+----+
                    |   |   |
                    |   |   +--------+
                    |   +----------+ |
                    |              | |
                    |  RFID        | |
                    +-----+--------+ |
                          |           |
             Pino 10 (SDA)+           |
             Pino 13 (SCK)+           |
             Pino 11 (MOSI)+          |
             Pino 12 (MISO)+          |
             Pino 9 (RST)+            |
             3.3V +-----------+       |
             GND +------------+       |
                          |   |       |
             +------------+   |       |
             |   Pino 4 (Buzzer)+     |
             |              GND +     |
             |              +----+----+
             |              |
             |          Pino 2 (Sensor Magnético)+
             |                GND +-------+
             |                        |    |
             +---+                    |    |
                 |                 Pino 5 (Botão de Alarme)+
                 |                      GND +----+
                 |                            |
                 |                        Pino 6 (LED)+
                 |                             GND +----+
                 |                                  |
                 +----------------------------------+
```

### Explicação do Circuito

1. **RFID MFRC522**: Conectado aos pinos SPI do Arduino (10, 11, 12, 13) e ao pino de reset (9). O módulo é alimentado com 3.3V.
2. **Buzzer**: Conectado ao pino 4 do Arduino e GND. O código faz com que ele emita sons intermitentes em diferentes eventos.
3. **Sensor Magnético**: Conectado ao pino 2 do Arduino e GND. Detecta a abertura de uma porta ou janela.
4. **Botão de Alarme**: Conectado ao pino 5 do Arduino e GND. Usado para ativar o sistema de alarme.
5. **LED**: Conectado ao pino 6 do Arduino e GND através de um resistor. Acende quando o sensor magnético é ativado, indicando uma possível intrusão.

### Funcionamento do Sistema

1. **Inicialização (`setup()`)**:
   - Configura os pinos como entrada ou saída.
   - Inicializa o módulo RFID.
   - Inicializa o sistema com o status de alarme desativado.

2. **Loop Principal (`loop()`)**:
   - **Estado `DESATIVADO`**: O sistema aguarda o pressionamento do botão de alarme. Quando pressionado, o sistema emite um som curto e muda para o estado `ATIVAR`.
   - **Estado `ATIVAR`**: O sistema aguarda a leitura de uma tag RFID cadastrada. Se lida dentro do tempo especificado, o sistema emite um som curto e muda para o estado `ATIVANDO`.
   - **Estado `ATIVANDO`**: O sistema aguarda um período para a ativação completa, emitindo bipes intermitentes. Pode ser cancelado pressionando o botão novamente.
   - **Estado `ATIVADO`**: O sistema monitora o sensor magnético. Se detectada uma mudança (por exemplo, a abertura de uma porta), o sistema muda para o estado `DESATIVAR`.
   - **Estado `DESATIVAR`**: O sistema aguarda a leitura de uma tag RFID cadastrada para desativar. Se a tag não for lida no tempo especificado, muda para o estado `ACIONAR`.
   - **Estado `ACIONAR`**: O sistema acende o LED, indicando uma possível intrusão.

### Testando o Sistema
Para testar o sistema:
1. Certifique-se de que todas as conexões estão corretas e firmes.
2. Carregue o código no Arduino.
3. Abra o monitor serial (configurado para 9600 baud) para ver as mensagens de depuração.
4. Pressione o botão para ativar o alarme.
5. Aproxime uma tag RFID cadastrada para completar a ativação.
6. Acione o sensor magnético (simulando a abertura de uma porta) e observe o LED acender.

 de montar e testar o sistema de alarme com RFID e LED.# UnB-files
