int AC_LOAD = 3;// Dará o pulso no Triac pin
int dimming = 125;// Dimming level (0-128) 0 = ON, 128 = OFF
unsigned long time;//Contará o tempo
unsigned long timeout;//Estouro
int brilho[3];//Recebe os valores da serial
int i=0;//Quantidade de caracteres recebida pela serial
int flag=0;//Indica que foi recebida informação pela serial
char temp;//Armazena dados recebidos pela serial
int x,y;//Variaveis auxiliares

void setup()
{
  Serial.begin(9600);//Inicia a serial com baud rate; 9600 bits por segundo
  pinMode(AC_LOAD, OUTPUT);// Disparo do Triac pino 3; AC_LOAD = 3
  attachInterrupt(0, zero_crosss_int, RISING);// 0 corresponde a porta digital 2, ou seja, quando a porta 2 sera uma entrada; Interrupção detecta a passagem por 0, quando passa de nível baixo para auto(Borda de subida)
  pinMode(4, INPUT);// Configura o pino 4 como entrada
  pinMode(5, INPUT);// Configura o pino 5 como entrada
}

void zero_crosss_int() // Função que detecta a passagem por 0
{
  // Calculo do ângulo de disparo :: 60Hz-> 8.33ms (1/2 Cycle)
  // (8333us - 8.33us) / 128 = 65 (Approx)
  int dimtime = (65*dimming); // 65 equivale a 1 de 128
  delayMicroseconds(dimtime); // Off cycle
  digitalWrite(AC_LOAD, HIGH); // disparando o Triac
  delayMicroseconds(8.33); // Aguarda o tempo de disparo
  digitalWrite(AC_LOAD, LOW); // finaliza o pulso do Triac
}

void loop()
{
  //------------------------------------------------------------------------------------------
  // Retorna o tempo em milisegundos desde que o Arduíno começou a rodar,
  time = millis()%2;//Divide por 2 e salva o resto
  if(time==1||time==0)//Compara se o resto é igual a 0 ou 1 e sempre vai ser
  {
  timeout++;// Cronômetro
    if (timeout>500)// Compara se houve estouro do tempo
    {
    i=0;// Zera o quantidades de caracteres recebidos
    flag=0;// Zera o aviso de que foi recebido algum dado na serial
    }
   }

  //-------------------------------------------------------------------------------------------

  //Aumenta a intensidade
  if(digitalRead(4)==1 && dimming >10)// Compara se o botão foi pressionado e se o dimming é maior que 10 seu limite máximo
  {
   dimming= dimming -5;// Aumenta a intensidade de 5 em 5
    y=dimming/1.25;//Converte em %
    y=(y-100)*(-1);//O dimming é inversamente proporcional à porcentagem
    Serial.print(y);
    Serial.println("%");
    Serial.print("Resolucao: ");
    Serial.println(dimming);
    delay(100);// Aguarda um tempo até o próximo acionamento
  }
  //Diminui a intensidade
  if(digitalRead(5)==1&& dimming<125)
  {
    dimming= dimming +5;
    y=dimming/1.25;
    y=(y-100)*(-1);
    Serial.print(y);
    Serial.println("%");
    Serial.print("Resolucao: ");
    Serial.println(dimming);
    delay(100);
  }
  //--------------------------------------------------------------------------------------------
  if (Serial.available()>0)// Verifica se houve recepção
  {
    flag=1;//Sinaliza que houve recepção
    timeout=0;// Zera o o tempo de reset
    //------------------------------------------------------------------------------------------
    temp = Serial.read();// Armazena o que foi recebido pela serial
    brilho[i]=temp-48;// Decrementa 48 na tabela ascii para converter de decimal para char
    i++;// Contabiliza um recebimento
  }
   //---------------------------------------------------------------------------
if (timeout>200&&flag==1)// Compara se houve estouro do timeout(se ficou um tempo sem recepção) e se houve recepção
{
  flag=0;// Sera aviso de recepção

   // Verifica a quantidade de informação recebida
   switch(i)
   {
   case 1:
   x=brilho[0];//Unidade
   break;

   case 2:
   x=brilho[0]*10 + brilho[1];//Dezena e unidade
   break;

   case 3:
   x=brilho[0]*100 + brilho[1]*10 + brilho[2];//Centena, Dezena, Unidade
   break;
   }
   // Envia para serial informações em % e resolução de disparo
    Serial.print(x);
    Serial.println("%");
     delay(30);

    if(x>100||x<0)// Proteção para se foi inserido um valor mair que 100%
    {
    Serial.println("Excedeu o limite");
    }
    else// Se estiver tudo OK
    {
    x=100-x;// Inversamente proporcional a %
    dimming=x*1.25;// Resolução diminuida de 128 para 125 para garantir um tempo bom de resposta
      if(dimming<8)
      {
      dimming=8;// O dimming será sempre maior que 10 para garantir um tempo bom de resposta
      }
    //Envia para a serial o valor da resolução
    Serial.print("Resolucao: ");
    Serial.println(dimming);
    }
  }
}
