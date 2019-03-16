#define LED_VERMELHO 2
#define LED_AZUL     3
#define LED_AMARELO  4
#define LED_VERDE    5

//Aumentar sempre que adicionar um LED
#define LED_MAXIMO   6

#define BOTAO_VERMELHO 8
#define BOTAO_AZUL     9
#define BOTAO_AMARELO  10
#define BOTAO_VERDE    11

#define INDEFINIDO -1

#define UM_SEGUNDO   1000
#define MEIO_SEGUNDO 500

#define TAMANHO_SEQUENCIA 10

enum Estados {
  PRONTO_PARA_PROXIMA_RODADA,
  USUARIO_RESPONDENDO,
  JOGO_FINALIZADO_SUCESSO,
  JOGO_FINALIZADO_FALHA
};

int sequenciaLuzes[TAMANHO_SEQUENCIA];
int rodada = 0;
int leds_respondidos = 0;

void setup(){
  Serial.begin(9600);
  iniciaPortas();
  iniciaJogo();
}

void loop(){

  switch (estadoAtual()) {
    case PRONTO_PARA_PROXIMA_RODADA:
      Serial.println("Pronto para a proxima rodada");
      preparaNovaRodada();
      break;
    case USUARIO_RESPONDENDO:
      Serial.println("Jogador respondendo");
      processaRespostaUsuario();
      break;
    case JOGO_FINALIZADO_SUCESSO:
      Serial.println("Jogo finalizado com sucesso");
      jogoFinalizadoSucesso();
      break;
    case JOGO_FINALIZADO_FALHA:
      Serial.println("Jogo finalizado com falha");
      jogoFinalizadoFalha();
      break;
  }
  delay(75);
}

void iniciaPortas(){

  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(LED_AZUL, OUTPUT);
  pinMode(LED_AMARELO, OUTPUT);
  pinMode(LED_VERDE, OUTPUT);
  
  pinMode(BOTAO_VERMELHO, INPUT_PULLUP);
  pinMode(BOTAO_AZUL, INPUT_PULLUP);
  pinMode(BOTAO_AMARELO, INPUT_PULLUP);
  pinMode(BOTAO_VERDE, INPUT_PULLUP);
}

void iniciaJogo(){
  int seed = analogRead(0);
  randomSeed( seed );
  
  for(int i = 0; i < TAMANHO_SEQUENCIA; i++){
    sequenciaLuzes[ i ] = sorteiaCor();
  }
}

void preparaNovaRodada(){
  rodada++;
  leds_respondidos = 0;
  tocaLedRodada();
}

void processaRespostaUsuario(){
  int resposta = checaRespostaJogador();

  if(resposta == INDEFINIDO){
    return;
  }

  if(resposta == sequenciaLuzes[leds_respondidos]){
    leds_respondidos++;
  }else{
    rodada = TAMANHO_SEQUENCIA + 2; //novo
  }
}

int estadoAtual(){
  if(rodada <= TAMANHO_SEQUENCIA){
    if(leds_respondidos == rodada){
      return PRONTO_PARA_PROXIMA_RODADA;
    }else{
      return USUARIO_RESPONDENDO;
    }
  }else if(rodada == TAMANHO_SEQUENCIA + 1){ //novo if else
    return JOGO_FINALIZADO_SUCESSO;
  }else{ //novo else
    return JOGO_FINALIZADO_FALHA;
  }
}

int sorteiaCor(){
  return random(LED_VERMELHO, LED_MAXIMO);
}  

int checaRespostaJogador(){
  if(digitalRead(BOTAO_VERMELHO) == LOW){
    return piscaLed( LED_VERMELHO );
  }
  
  if(digitalRead(BOTAO_AZUL) == LOW){
    return piscaLed( LED_AZUL );
  }
  
  if(digitalRead(BOTAO_AMARELO) == LOW){
    return piscaLed( LED_AMARELO );
  }
  
  if(digitalRead(BOTAO_VERDE) == LOW){
    return piscaLed( LED_VERDE );
  }
  
  return INDEFINIDO;
}

int piscaLed( int portaLed ){
  digitalWrite(portaLed,HIGH);
  delay(UM_SEGUNDO);
  digitalWrite(portaLed,LOW);
  delay(MEIO_SEGUNDO);
  return portaLed;
}

void tocaLedRodada(){
  for(int i = 0; i < rodada; i++){
     piscaLed( sequenciaLuzes[ i ] );
  }
}

void jogoFinalizadoSucesso(){
  piscaLed(LED_VERDE);
  piscaLed(LED_AMARELO);
  piscaLed(LED_AZUL);
  piscaLed(LED_VERMELHO);
  delay(MEIO_SEGUNDO);
}

void jogoFinalizadoFalha(){
  digitalWrite(LED_VERDE,HIGH);
  digitalWrite(LED_AMARELO,HIGH);
  digitalWrite(LED_VERMELHO,HIGH);
  digitalWrite(LED_AZUL,HIGH);
  delay(UM_SEGUNDO);
  digitalWrite(LED_VERDE,LOW);
  digitalWrite(LED_AMARELO,LOW);
  digitalWrite(LED_VERMELHO,LOW);
  digitalWrite(LED_AZUL,LOW);
  delay(MEIO_SEGUNDO);
}
