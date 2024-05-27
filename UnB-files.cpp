#include <MFRC522.h>
#include <SPI.h>

#define SENSOR_MAGNETICO 2
#define RELE 3
#define BUZZER 4
#define BOTAO_ALARME 5
#define LED 6
#define PINO_RST 9
#define PINO_SDA 10
#define DELAY 100
#define TEMPO_PARA_LER_TAG 5000 - DELAY
#define TEMPO_PARA_ATIVAR 10000 - DELAY
#define TEMPO_PARA_DESATIVAR 10000 - DELAY
#define QTD_TAGS(tags)  (sizeof(tags) / sizeof((tags)[0]))
#define TAGS "42 36 5a 1a", "19 0a e5 c2", "c5 26 31 23"

MFRC522 rfid(PINO_SDA, PINO_RST);
char *tagsCadastradas[]{TAGS};

unsigned long tempo;
unsigned long tempo_bip;

enum StatusAlarme
{
  DESATIVADO,
  ATIVADO,
  ATIVAR,
  ATIVANDO,
  DESATIVAR,
  ACIONAR
};

StatusAlarme statusAlarme;

bool tagCadastrada()
{
  bool cadastrada = false;
  String tag = "";

  if (!rfid.PICC_IsNewCardPresent()) 
  {
    return false;
  }
  
  if (!rfid.PICC_ReadCardSerial()) 
  {
    return false;
  }
  
  for (byte i = 0; i < rfid.uid.size; i++) 
  {
     tag.concat(String(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ")); 
     tag.concat(String(rfid.uid.uidByte[i], HEX)); 
  }

  for(int i = 0; i < QTD_TAGS(tagsCadastradas); i++)
  {
    if (tag.substring(1) == tagsCadastradas[i]) 
    {
      Serial.println("Existe");
      cadastrada = true;
      break;
    }
    else
    {
      Serial.println("Não Existe");
    }
  }

  return cadastrada;
}

void somBotaoPressionado()
{
  for(int i = 0; i <= 5; i++)
  {
    digitalWrite(BUZZER, HIGH);
    delay(30);
    digitalWrite(BUZZER, LOW);
    delay(30);
  }
}

void somTagCadastrada()
{  
  for(int i = 0; i <= 2; i++)
  {
    digitalWrite(BUZZER, HIGH);
    delay(100);
    digitalWrite(BUZZER, LOW);
    delay(100);
  }
}

void buzzerBip(unsigned long duracao)
{
  if(millis() - tempo_bip >= duracao)
  {
    digitalWrite(BUZZER, !digitalRead(BUZZER));
    tempo_bip = millis();
  }
}

void setup() 
{
  pinMode(BOTAO_ALARME, INPUT_PULLUP);
  pinMode(BUZZER, OUTPUT);
  pinMode(RELE, OUTPUT);
  pinMode(LED, OUTPUT); // Adicionado para o LED

  digitalWrite(RELE, HIGH); //Mantém relé desativado
  digitalWrite(LED, LOW);   // Inicialmente, mantenha o LED apagado

  Serial.begin(9600); //Inicializa serial para debug

  SPI.begin(); // Inicia SPI bus
  rfid.PCD_Init();

  statusAlarme = StatusAlarme::DESATIVADO;
  tempo = 0;
}

void loop() 
{
  if(statusAlarme == StatusAlarme::DESATIVADO)
  {
    if(!digitalRead(BOTAO_ALARME))
    {
      somBotaoPressionado();
      statusAlarme = StatusAlarme::ATIVAR;
      tempo = millis();
    }

    Serial.println("Desativado");
  }
  else if(statusAlarme == StatusAlarme::ATIVAR)
  {
    bool cadastrada = false;
    
    if(millis() - tempo <= TEMPO_PARA_LER_TAG)
    {
      cadastrada =  tagCadastrada();

      if(cadastrada)
      {
        somTagCadastrada();
        statusAlarme = StatusAlarme::ATIVANDO;
        tempo = millis();
        tempo_bip = millis();
      }
      Serial.println(tagCadastrada());
    }
    else
    {
      tempo = 0;
      statusAlarme = StatusAlarme::DESATIVADO;
    }
  }
  else if(statusAlarme == StatusAlarme::ATIVANDO)
  {
     buzzerBip(500);
     
    if(!digitalRead(BOTAO_ALARME))
    {
      somBotaoPressionado();
      statusAlarme = StatusAlarme::DESATIVADO;
    }

    if(millis() - tempo >= TEMPO_PARA_ATIVAR)
    {
      statusAlarme = StatusAlarme::ATIVADO;
      somBotaoPressionado();
    }

    Serial.println("Ativando");
  }
  else if(statusAlarme == StatusAlarme::ATIVADO)
  {
    bool magneticoAtivado = digitalRead(SENSOR_MAGNETICO);

    if(magneticoAtivado)
    {
      statusAlarme = StatusAlarme::DESATIVAR;
      tempo = millis();
      tempo_bip = millis();
    }

    Serial.println("Ativado");
  }
  else if(statusAlarme == StatusAlarme::DESATIVAR)
  {
    buzzerBip(400);

    bool cadastrada = false;

    cadastrada =  tagCadastrada();
    
    if(cadastrada)
    {
      somTagCadastrada();
      statusAlarme = StatusAlarme::DESATIVADO;
      digitalWrite(LED, LOW); // Apaga o LED quando desativado
    }

    if(millis() - tempo >= TEMPO_PARA_DESATIVAR)
    {
      statusAlarme = StatusAlarme::ACIONAR;
      digitalWrite(BUZZER, LOW);
    }

    Serial.println("Desativar");
  }
  else if(statusAlarme == StatusAlarme::ACIONAR)
  {
    digitalWrite(BUZZER, LOW);
    digitalWrite(LED, HIGH); // Acende o LED em vez de ativar o relé

    Serial.println("Tocando");
  }
