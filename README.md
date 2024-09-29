# Proyecto: Vehículo IoT Controlado con ESP32

Este proyecto consiste en la creación de un vehículo controlado de forma remota mediante un **ESP32**, utilizando **Wi-Fi** y **MQTT** para la comunicación. El vehículo puede moverse hacia adelante y atrás, girar a la izquierda y derecha, y ajustar su velocidad. Además, incorpora sensores ultrasónicos para detectar obstáculos y ajustar su comportamiento en consecuencia.

## Características Principales

- **Control Remoto vía MQTT**: El ESP32 se conecta a una red Wi-Fi y utiliza el protocolo MQTT para recibir comandos desde un servidor (en este caso, `test.mosquitto.org`).
- **Control de Motores**: Mediante pines GPIO, el ESP32 controla los motores del vehículo para moverse en distintas direcciones.
- **Dirección con Servomotor**: Un servomotor permite el control de la dirección, girando las ruedas delanteras hacia la izquierda o derecha.
- **Sensores Ultrasónicos**: Dos sensores HC-SR04 miden la distancia a obstáculos, permitiendo implementar lógica de evitación.
- **Multitarea con TaskScheduler**: Uso de la librería `TaskScheduler.h` para ejecutar tareas en paralelo, como leer sensores y controlar motores.
- **Conexión Wi-Fi y MQTT**: Funciones para conectar el ESP32 a una red Wi-Fi y suscribirse a tópicos MQTT para comunicación en tiempo real.

## Requisitos de Hardware

- **ESP32**
- **Motores y Driver de Motores**: Conexión a los pines especificados en el código.
- **Servomotor**: Conectado al pin 2.
- **Sensores Ultrasónicos HC-SR04**: Conectados a los pines definidos para `TRIGER` y `ECO`.
- **Fuente de Alimentación Adecuada**
- **Cableado y Protoboard (opcional)**

## Requisitos de Software

- **Arduino IDE** (o plataforma compatible con ESP32)
- **Librerías Necesarias**:
  - `ESP32Servo.h`
  - `WiFi.h`
  - `PubSubClient.h`
  - `TaskScheduler.h`

## Configuración Inicial

1. **Instalar Librerías**: Asegúrate de tener instaladas todas las librerías mencionadas.
2. **Configurar Credenciales Wi-Fi**: Modifica las variables `ssid` y `password` con el nombre y contraseña de tu red Wi-Fi.

   ```cpp
   const char* ssid = "TU_SSID";
   const char* password = "TU_CONTRASEÑA";
   ```

3. **Configurar Servidor MQTT**: Verifica que el servidor MQTT (`mqtt_server`) esté accesible o configura uno propio.

   ```cpp
   const char* mqtt_server = "test.mosquitto.org";
   ```

4. **Conexiones de Hardware**: Conecta los motores, servomotor y sensores ultrasónicos a los pines especificados en el código.

## Funcionalidades del Código

- **Conexión Wi-Fi y MQTT**: Funciones para establecer conexión y reconexión en caso de pérdida.
- **Callback MQTT**: Función `callback` para recibir y procesar mensajes de los tópicos suscritos:
  - `velocidad_R`: Controla la velocidad del vehículo.
  - `direccion_R`: Controla el movimiento hacia adelante, atrás o detenerse.
  - `direccion_S`: Controla la dirección del servomotor (giro izquierda/derecha).
- **Control de Motores**: Funciones para manejar el estado de los motores (`motorAdelante`, `motorAtras`, `motorStop`).
- **Control del Servomotor**: Ajuste del ángulo para girar las ruedas delanteras.
- **Lectura de Sensores**: Funciones (comentadas) para leer los valores de los sensores ultrasónicos y calcular distancias.
- **Multitarea**: Implementación de tareas paralelas para lectura de sensores y control de motores usando `TaskScheduler`.

## Uso del Proyecto

1. **Compilar y Subir el Código**: Utiliza Arduino IDE para compilar y cargar el código en el ESP32.
2. **Preparar el Servidor MQTT**: Asegúrate de que el servidor MQTT esté funcionando y accesible.
3. **Enviar Comandos**: Utiliza una aplicación o script que publique mensajes en los tópicos MQTT:
   - **`direccion_R`**:
     - `0`: Detener
     - `1`: Adelante
     - `2`: Atrás
   - **`velocidad_R`**: Valor numérico para la velocidad (0-255).
   - **`direccion_S`**:
     - `1`: Girar a la derecha
     - `2`: Girar a la izquierda
4. **Monitorear**: Observa el comportamiento del vehículo y ajusta los parámetros si es necesario.

## Notas Adicionales

- **Seguridad**: Considera implementar autenticación en el servidor MQTT para mayor seguridad.
- **Optimización**: Las funciones de lectura de sensores y control de motores pueden ajustarse según las necesidades.
- **Depuración**: Usa `Serial.println` para imprimir información en el monitor serial y facilitar la depuración.
- **Comentarios**: Algunas partes del código están comentadas (por ejemplo, la lectura de sensores). Descoméntalas si deseas utilizarlas.

## Contribuciones y Mejoras

Este proyecto es una base para desarrollar vehículos controlados por IoT. Se pueden añadir mejoras como:

- Implementación de lógica de evitación de obstáculos utilizando los sensores ultrasónicos.
- Añadir más sensores o actuadores para aumentar la funcionalidad.
- Desarrollo de una interfaz de usuario para controlar el vehículo desde una aplicación móvil o web.

---

1. [Fork](https://github.com/aarturodev/Car-IoT/fork) the repository.
   

¡Disfruta construyendo y programando tu vehículo IoT!
