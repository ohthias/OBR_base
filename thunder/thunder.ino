// Sentidos
#define MOTOR_ESQUERDO_FRENTE 5
#define MOTOR_ESQUERDO_TRAS 6
#define MOTOR_DIREITO_FRENTE 9
#define MOTOR_DIREITO_TRAS 10

// Encoders
#define PINO_ENCODER_ESQUERDO 4
#define PINO_ENCODER_DIREITO 11

volatile int contagemEncoderEsquerdo = 0;
volatile int contagemEncoderDireito = 0;

// Sensores Refletância
#define SENSOR_EXTREMO_ESQUERDO A0
#define SENSOR_ESQUERDO A1
#define SENSOR_CENTRO A2
#define SENSOR_DIREITO A3
#define SENSOR_EXTREMO_DIREITO A4

// Sensores LDR
#define LDR_esquerda A7
#define LDR_direita A8

// Flags Verde
enum class Flags_verde {
  curvaDireita,
  curvaEsquerda,
  beco,
  nenhum
};
Flags_verde flags = Flags_verde::nenhum;

/* PID */
float Kp = 1.2;  // Ganho Proporcional
float Ki = 0.0;  // Ganho Integral
float Kd = 0.8;  // Ganho Derivativo

int erro = 0;
int erro_anterior = 0;
int integral = 0;
int derivativo = 0;


/* Calibração */
int preto = 200;
int verde = 50;

/* Velocidade */
int velocidade_esquerda, velocidade_direita;
int velocidade_base = 10;  // Velocidade base dos motores (aumentada para melhor movimento)
int velocidade_max = 200;

void setup() {
  pinMode(MOTOR_ESQUERDO_FRENTE, OUTPUT);
  pinMode(MOTOR_ESQUERDO_TRAS, OUTPUT);
  pinMode(MOTOR_DIREITO_FRENTE, OUTPUT);
  pinMode(MOTOR_DIREITO_TRAS, OUTPUT);

  pinMode(PINO_ENCODER_ESQUERDO, INPUT_PULLUP);
  pinMode(PINO_ENCODER_DIREITO, INPUT_PULLUP);

  Serial.begin(9600);
}

void loop() {
  while (flags == Flags_verde::nenhum) {
    segueLinha();
  }

  //Flags de curva
  switch(flags) {
    case Flags_verde::curvaDireita:
      Serial.println("Curva para a direita detectada");
      curvaDireita90Graus();
      flags = Flags_verde::nenhum;
      break;

    case Flags_verde::curvaEsquerda:
      Serial.println("Curva para a esquerda detectada");
      curvaEsquerda90Graus();
      delay(1000);
      flags = Flags_verde::nenhum;
      break;

    case Flags_verde::beco:
      Serial.println("Beco detectado");
      meiaVolta();
      delay(1000);
      flags = Flags_verde::nenhum;
      break;

    case Flags_verde::nenhum:
      Serial.println("Nenhuma curva ou beco detectado");
      break;
  }
  delay(100);
}