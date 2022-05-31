#include <HTTPClient.h>
#include "EmonLib.h"
#include <WiFiManager.h> //https://github.com/tzapu/WiFiManager

EnergyMonitor sensor;

int pinSCT = 13;
int tensao = 220;
int potencia;


void setup() {
    Serial.begin(115200);
    
    sensor.current(pinSCT, 100);
    
    WiFi.mode(WIFI_STA);
    
    
    //inicia o ponto de acesso local
    WiFiManager wm;

    //reseta as configurações do ponto de acesso
    wm.resetSettings();

    //Cria o ponto de acesso com nome e senha desejada
    bool res;
    res = wm.autoConnect("AutoConnectAP","password");

    //verifica se as credenciais passadas pelo user estão corretas e inicia a conexão
    if(!res) {
        Serial.println("Falha ao conectar");
        ESP.restart();
    } 
    else { 
        Serial.println("conectado com sucesso! :)");
    }

    
}

void loop() {

    //Finaliza os processos http
    http.end();

    //Calculo de medição
    double corrente = sensor.calcIrms(1480);

      potencia = corrente * tensao;

      if(corrente<0.02){
        corrente=0; 
        potencia=0;
      }

      Serial.println("Potencia = ");
      Serial.println(potencia);
      Serial.println("Corrente = ");
      Serial.println(corrente);
    
      HTTPClient http;
      http.begin("http://192.168.0.109:8080/esp");
      http.addHeader("Content-Type", "text/plain");             //especifique os tipos do header
  
      String content = "value" + potencia
    
      int httpResponseCode = http.POST(content);   //Envia o comando de post atual
  
      if(httpResponseCode>0){
  
        String response = http.getString();   
  
        Serial.println(httpResponseCode);   //Printa o retorno do codigo
        Serial.println(response);           //Printa a reposta
  
      }else{

        Serial.print("Error on sending POST: ");
        Serial.println(httpResponseCode);
  
        }

      delay(10000);
}
