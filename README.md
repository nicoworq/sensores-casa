# Medición de Temperatura con ESP

Este es un proyecto simple que utiliza un ESP para medir la temperatura ambiente y mostrar los resultados. El ESP se comunica con un sensor de temperatura y envía los datos a través de una conexión Wi-Fi a un servidor para su visualización.

## Características

- Medición precisa de la temperatura ambiente.
- Conexión Wi-Fi para el envío de datos.
- Visualización remota de los datos de temperatura.

## Componentes

- Placa ESP8266 o ESP32.
- Sensor de temperatura compatible (por ejemplo, DHT11, DHT22, DS18B20).
- Conexión a una red Wi-Fi.

## Instrucciones de Uso

1. Clona o descarga este repositorio.
2. Conecta el sensor de temperatura al ESP siguiendo el esquema proporcionado en el archivo `connections.png`.
3. Abre el archivo `config.h` y configura tus credenciales Wi-Fi:

```cpp
const char *ssid = "Nombre_de_Tu_Red";
const char *password = "Tu_Contraseña";
