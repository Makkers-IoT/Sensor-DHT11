#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <DHT.h>

// Configurações Wi-Fi
const char *ssid = "LOJA MaKKERS_2.4G";   // Substitua pelo nome da sua rede Wi-Fi
const char *password = "newMakkers@2024"; // Substitua pela senha da sua rede Wi-Fi

// Configurações do DHT11
#define DHTPIN 22
#define DHTTYPE DHT11   // Definindo o tipo de sensor como DHT11
DHT dht(DHTPIN, DHTTYPE);

// Servidor Web
WebServer server(80);

void handleRoot()
{
  float temperature = dht.readTemperature();  // Lê a temperatura em Celsius
  float humidity = dht.readHumidity();        // Lê a umidade

  // Verificar se a leitura foi bem-sucedida
  if (isnan(temperature) || isnan(humidity)) {
    String html = R"rawliteral(
    <html>
    <head><title>Erro na leitura</title></head>
    <body><h1>Erro ao ler dados do sensor DHT11</h1></body>
    </html>
    )rawliteral";
    server.send(500, "text/html", html);
    return;
  }

  // HTML para exibir os dados
  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="pt-br">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Makkers Leitor</title>
    <style>
        body {
            height: 100vh;
            margin: 0;
            font-family: Arial, sans-serif;
            background-color: #f0f0f0;
        }

        div.dados {
            text-align: center;
            padding: 10px;
            margin: 15px;
        }

        h1 {
            font-size: 1.8em;
            color: #3ec1ac;
            margin-bottom: 30px;
        }

        p {
            font-size: 1.3em;
            color: #555;
        }

        .makkers {
            text-decoration: none;
            font-size: 3em;
            color: #349e8c;
            margin-top: 30vh;
            display: flex;
            flex-direction: column;
            justify-content: center;
            align-items: center;
        }

        .dados a {
            display: inline-block;
            margin-top: 15px;
            padding: 10px 20px;
            text-decoration: none;
            color: white;
            background-color: #3ec1ac;
            border-radius: 5px;
            font-size: 1.2em;
        }

        .dados a:hover {
            background-color: #349e8c;
        }
    </style>
</head>

<body>

    <div class="dados">
        <h1>Leitura de Temperatura e Umidade</h1>
        <p>Temperatura: )rawliteral" + String(temperature) + R"rawliteral( °C</p>
        <p>Umidade: )rawliteral" + String(humidity) + R"rawliteral( %</p>
        <a href="/">Atualizar</a>
    </div>
    <br>
    <a class="makkers" href="https://www.makkers.com.br">Makkers</a>
</body>

</html>
  )rawliteral";

  server.send(200, "text/html", html);
}

void setup()
{
  Serial.begin(9600);
  dht.begin();  // Inicializa o sensor DHT11

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Conectando ao WiFi...");
  }

  Serial.println("Conectado ao WiFi");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.begin();
}

void loop()
{
  server.handleClient();
}
