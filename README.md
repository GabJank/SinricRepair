# SinricRepair
Changes to make to fake sinric functional

# ProductConfigManager.h

## loadConfig
```c++
bool ProductConfigManager::loadConfig() {
  Serial.printf("[ProductConfigManager.loadConfig()]: Loading config...\r\n");

  File configFile = SPIFFS.open(PRODUCT_CONFIG_FILE, "r");

  if (!configFile) {
      Serial.printf("[ProductConfigManager.loadConfig()]: Failed to open config file!!\r\n");
      return false;
  }

  if (configFile.size() == 0) {
      Serial.println("[ProductConfigManager.loadConfig()]: Empty config file!");
      configFile.close();
      return false;
  }

  Serial.println("Conteúdo do arquivo antes de carregar:");
  while (configFile.available()) {
      Serial.write(configFile.read());
  }
  Serial.println();
  configFile.seek(0);

  StaticJsonDocument<2048> doc;
  DeserializationError err = deserializeJson(doc, configFile);

  configFile.close();

  if (err) {
      Serial.printf("[ProductConfigManager.loadConfig()]: deserializeJson() failed: %s\r\n", err.c_str());
      return false;
  }

  serializeJsonPretty(doc, Serial);

  if (!doc.containsKey("devices") || !doc["devices"].is<JsonArray>() || doc["devices"].size() < 5) {
      Serial.println("[ProductConfigManager.loadConfig()]: JSON não contém dispositivos suficientes!");
      return false;
  }

  strlcpy(config.appKey, doc[F("credentials")][F("appkey")] | "", sizeof(config.appKey));
  strlcpy(config.appSecret, doc[F("credentials")][F("appsecret")] | "", sizeof(config.appSecret));

  strlcpy(config.light_1_id, doc[F("devices")][0][F("id")] | "", sizeof(config.light_1_id));
  strlcpy(config.light_1_name, doc[F("devices")][0][F("name")] | "", sizeof(config.light_1_name));
  strlcpy(config.dimmable_switch_2_id, doc[F("devices")][1][F("id")] | "", sizeof(config.dimmable_switch_2_id));
  strlcpy(config.dimmable_switch_2_name, doc[F("devices")][1][F("name")] | "", sizeof(config.dimmable_switch_2_name));
  strlcpy(config.switch_3_id, doc[F("devices")][2][F("id")] | "", sizeof(config.switch_3_id));
  strlcpy(config.switch_3_name, doc[F("devices")][2][F("name")] | "", sizeof(config.switch_3_name));
  strlcpy(config.switch_4_id, doc[F("devices")][3][F("id")] | "", sizeof(config.switch_4_id));
  strlcpy(config.switch_4_name, doc[F("devices")][3][F("name")] | "", sizeof(config.switch_4_name));
  strlcpy(config.switch_5_id, doc[F("devices")][4][F("id")] | "", sizeof(config.switch_5_id));
  strlcpy(config.switch_5_name, doc[F("devices")][4][F("name")] | "", sizeof(config.switch_5_name));


  Serial.printf("\r\n[ProductConfigManager.loadConfig()]: Configuração carregada com sucesso!\r\n");

  return true;
}
```

## saveJsonConfig
```c++
bool ProductConfigManager::saveJsonConfig(const JsonDocument &doc) {
  Serial.printf("[ProductConfigManager.saveJsonConfig()]: Saving config...\r\n");

  String appKey = doc["credentials"]["appkey"] | "";
  String appSecret = doc["credentials"]["appsecret"] | "";

  if (appKey.length() == 0 || appSecret.length() == 0) {
      Serial.printf("[ProductConfigManager.saveJsonConfig()]: Failed! Invalid configurations!\r\n");
      return false;
  }

  Serial.printf("[ProductConfigManager.saveJsonConfig()]: config: \r\n");
  serializeJsonPretty(doc, Serial);

  File configFile = SPIFFS.open(PRODUCT_CONFIG_FILE, FILE_WRITE);

  if (!configFile) {
      Serial.printf("[ProductConfigManager.saveJsonConfig()] Open config file failed!!!\r\n");
      return false;
  }

  size_t bytesWritten = serializeJson(doc, configFile);
  configFile.flush();  
  configFile.close();

  Serial.printf("[ProductConfigManager.saveJsonConfig()] Bytes written: %u\r\n", bytesWritten);

  if (bytesWritten == 0) {
      Serial.printf("[ProductConfigManager.saveJsonConfig()] Erro! Nenhum byte foi gravado no arquivo!\r\n");
      return false;
  }

  strlcpy(config.appKey, appKey.c_str(), sizeof(config.appKey));
  strlcpy(config.appSecret, appSecret.c_str(), sizeof(config.appSecret));

  strlcpy(config.light_1_id, doc[F("devices")][0][F("id")] | "", sizeof(config.light_1_id));
  strlcpy(config.light_1_name, doc[F("devices")][0][F("name")] | "", sizeof(config.light_1_name));
  strlcpy(config.dimmable_switch_2_id, doc[F("devices")][1][F("id")] | "", sizeof(config.dimmable_switch_2_id));
  strlcpy(config.dimmable_switch_2_name, doc[F("devices")][1][F("name")] | "", sizeof(config.dimmable_switch_2_name));
  strlcpy(config.switch_3_id, doc[F("devices")][2][F("id")] | "", sizeof(config.switch_3_id));
  strlcpy(config.switch_3_name, doc[F("devices")][2][F("name")] | "", sizeof(config.switch_3_name));
  strlcpy(config.switch_4_id, doc[F("devices")][3][F("id")] | "", sizeof(config.switch_4_id));
  strlcpy(config.switch_4_name, doc[F("devices")][3][F("name")] | "", sizeof(config.switch_4_name));
  strlcpy(config.switch_5_id, doc[F("devices")][4][F("id")] | "", sizeof(config.switch_5_id));
  strlcpy(config.switch_5_name, doc[F("devices")][4][F("name")] | "", sizeof(config.switch_5_name));


  Serial.printf("[ProductConfigManager.saveJsonConfig()]: Success!\r\n");
  return true;
}
```
## clear
```
bool ProductConfigManager::clear(){
  Serial.printf("[ProductConfigManager.clear()]: Clear config..."); 
   
  SPIFFS.begin();
  if (SPIFFS.exists(PRODUCT_CONFIG_FILE)) {
    SPIFFS.remove(PRODUCT_CONFIG_FILE);
  }

  SPIFFS.end();

  memset(config.appKey, 0, sizeof(config.appKey));
  memset(config.appSecret, 0, sizeof(config.appSecret));
  memset(config.light_1_id, 0, sizeof(config.light_1_id));
  memset(config.light_1_name, 0, sizeof(config.light_1_name));
  memset(config.dimmable_switch_2_id, 0, sizeof(config.dimmable_switch_2_id));
  memset(config.dimmable_switch_2_name, 0, sizeof(config.dimmable_switch_2_name));
  memset(config.switch_3_id, 0, sizeof(config.switch_3_id));
  memset(config.switch_3_name, 0, sizeof(config.switch_3_name));
  memset(config.switch_4_id, 0, sizeof(config.switch_4_id));
  memset(config.switch_4_name, 0, sizeof(config.switch_4_name));
  memset(config.switch_5_id, 0, sizeof(config.switch_5_id));
  memset(config.switch_5_name, 0, sizeof(config.switch_5_name));
 
  Serial.printf("Done...\r\n"); 
  return true;
```

# SinricProSignature.cpp

## signMessage
```c++
String signMessage(String key, JsonDocument &jsonMessage) {
  Serial.println("===== DEBUG signMessage =====");
  
  Serial.printf("[DEBUG] Chave Secreta (key): %s\n", key.c_str());

  if (jsonMessage.containsKey("payload")) {
      Serial.print("[DEBUG] JSON Payload: ");
      serializeJsonPretty(jsonMessage["payload"], Serial);
      Serial.println();
  } else {
      Serial.println("[ERRO] O JSON não contém um campo 'payload'!");
  }
  
  char payloadBuffer[1024] = {0};
  size_t payloadSize = serializeJson(jsonMessage["payload"], payloadBuffer, sizeof(payloadBuffer) - 1);
  payloadBuffer[payloadSize] = '\0';

  String payload = String(payloadBuffer);
  jsonMessage["signature"]["HMAC"] = calculateSignature(key.c_str(), payload);

  char responseBuffer[1024] = {0};
  size_t jsonSize = serializeJson(jsonMessage, responseBuffer, sizeof(responseBuffer) - 1);
  responseBuffer[jsonSize] = '\0';

  String messageStr = String(responseBuffer);

  Serial.println("===== FIM DEBUG signMessage =====");
  return messageStr;
}
```

# SinricPro.h

## handleDeviceRequest
```c++
void SinricProClass::handleDeviceRequest(JsonDocument& requestMessage, interface_t Interface) {
    DEBUG_SINRIC("[SinricPro.handleDeviceRequest()]: handling device sope request\r\n");
#ifndef NODEBUG_SINRIC
    serializeJsonPretty(requestMessage, DEBUG_ESP_PORT);
#endif

    JsonDocument responseMessage = prepareResponse(requestMessage);

    // handle devices
    bool        success        = false;
    const char* deviceId       = requestMessage[FSTR_SINRICPRO_payload][FSTR_SINRICPRO_deviceId];
    String      action         = requestMessage[FSTR_SINRICPRO_payload][FSTR_SINRICPRO_action] | "";
    String      instance       = requestMessage[FSTR_SINRICPRO_payload][FSTR_SINRICPRO_instanceId] | "";
    JsonObject  request_value  = requestMessage[FSTR_SINRICPRO_payload][FSTR_SINRICPRO_value];
    JsonObject  response_value = responseMessage[FSTR_SINRICPRO_payload][FSTR_SINRICPRO_value];

    for (auto& device : devices) {
        if (device->getDeviceId() == deviceId && success == false) {
            SinricProRequest request{
                action,
                instance,
                request_value,
                response_value};
            success                                                         = device->handleRequest(request);
            responseMessage[FSTR_SINRICPRO_payload][FSTR_SINRICPRO_success] = success;
            if (!success) {
                if (responseMessageStr.length() > 0) {
                    responseMessage[FSTR_SINRICPRO_payload][FSTR_SINRICPRO_message] = responseMessageStr;
                    responseMessageStr                                              = "";
                } else {
                    responseMessage[FSTR_SINRICPRO_payload][FSTR_SINRICPRO_message] = "Device did not handle \"" + action + "\"";
                }
            }
        }
    }

    DEBUG_SINRIC("[SinricPro.handleDeviceRequest()]: Verificando responseMessage antes da serialização...\r\n");
    serializeJsonPretty(responseMessage, Serial);

    char responseBuffer[1024] = {0};  // Inicializa tudo com '\0'
    size_t jsonSize = serializeJson(responseMessage, responseBuffer, sizeof(responseBuffer) - 1);  // Deixa espaço para '\0'

    if (jsonSize == 0) {
        DEBUG_SINRIC("[SinricPro.handleDeviceRequest()]: ERRO! serializeJson() falhou!\r\n");
        return;
    }

    DEBUG_SINRIC("[SinricPro.handleDeviceRequest()]: JSON serializado com sucesso (%d bytes)\r\n", jsonSize);

    // Garante que o buffer tem '\0'
    responseBuffer[jsonSize] = '\0';

    // Converte para String de forma segura
    String responseString = String(responseBuffer);
    DEBUG_SINRIC("[SinricPro.handleDeviceRequest()]: Response String: %s\r\n", responseString.c_str());

    sendQueue.push(new SinricProMessage(Interface, responseString.c_str()));
}
```

## handleReceiveQueue

```c++
void SinricProClass::handleReceiveQueue() {
    if (receiveQueue.size() == 0) return;
    Serial.printf("AppSecret: %s\n", appSecret.c_str());
    DEBUG_SINRIC("[SinricPro.handleReceiveQueue()]: %i message(s) in receiveQueue\r\n", receiveQueue.size());

    while (receiveQueue.size() > 0) {
        SinricProMessage* rawMessage = receiveQueue.front();
        receiveQueue.pop();

        // 🚨 Verifique se a mensagem não é NULL antes de processar
        if (!rawMessage || !rawMessage->getMessage()) {
            DEBUG_SINRIC("[SinricPro.handleReceiveQueue()]: ERRO! Mensagem nula recebida. Ignorando...\r\n");
            continue;
        }

        JsonDocument jsonMessage;
        DeserializationError err = deserializeJson(jsonMessage, rawMessage->getMessage());

        // 🚨 Verificar se `deserializeJson()` falhou
        if (err) {
            DEBUG_SINRIC("[SinricPro.handleReceiveQueue()]: ERRO! deserializeJson() falhou: %s\r\n", err.c_str());
            DEBUG_SINRIC("[SinricPro.handleReceiveQueue()]: Mensagem recebida: %s\r\n", rawMessage->getMessage());
            delete rawMessage;
            continue;
        }

        bool sigMatch = false;

        if (strncmp(rawMessage->getMessage(), "{\"timestamp\":", 13) == 0 && strlen(rawMessage->getMessage()) <= 26) {
            sigMatch = true;  // timestamp message has no signature...ignore sigMatch for this!
        } else {
            // ✅ Verifique se 'signature' existe antes de tentar acessá-la
            if (jsonMessage.containsKey(FSTR_SINRICPRO_signature)) {
                String signature = jsonMessage[FSTR_SINRICPRO_signature][FSTR_SINRICPRO_HMAC] | "";
                String payload = extractPayload(rawMessage->getMessage());
                String calculatedSignature = calculateSignature(appSecret.c_str(), payload);
                Serial.printf("[DEBUG] Assinatura recebida: %s\n", signature.c_str());
                Serial.printf("[DEBUG] Payload extraído: %s\n", payload.c_str());
                Serial.printf("[DEBUG] Assinatura calculada: %s\n", calculatedSignature.c_str());

                sigMatch = (calculatedSignature == signature);
            } else {
                DEBUG_SINRIC("[SinricPro.handleReceiveQueue()]: Nenhuma assinatura na mensagem. Pulando verificação.\r\n");
                sigMatch = true;  // Assumimos que a mensagem sem assinatura é válida
            }
        }

        // 🚨 Verificar se `payload` existe no JSON antes de acessar
        if (!jsonMessage.containsKey(FSTR_SINRICPRO_payload)) {
            DEBUG_SINRIC("[SinricPro.handleReceiveQueue()]: ERRO! Mensagem recebida sem 'payload'. Ignorando...\r\n");
            delete rawMessage;
            continue;
        }

        // 🚨 Verificar se `type` existe no `payload`
        if (!jsonMessage[FSTR_SINRICPRO_payload].containsKey(FSTR_SINRICPRO_type)) {
            DEBUG_SINRIC("[SinricPro.handleReceiveQueue()]: ERRO! Mensagem sem 'type'. Ignorando...\r\n");
            delete rawMessage;
            continue;
        }

        String messageType = jsonMessage[FSTR_SINRICPRO_payload][FSTR_SINRICPRO_type];

        if (sigMatch) {
            DEBUG_SINRIC("[SinricPro.handleReceiveQueue()]: Signature válida. Processando mensagem...\r\n");
            extractTimestamp(jsonMessage);

            if (messageType == FSTR_SINRICPRO_response) handleResponse(jsonMessage);
            if (messageType == FSTR_SINRICPRO_request) {
                String scope = jsonMessage[FSTR_SINRICPRO_payload][FSTR_SINRICPRO_scope] | FSTR_SINRICPRO_device;
                if (strcmp(FSTR_SINRICPRO_module, scope.c_str()) == 0) {
                    handleModuleRequest(jsonMessage, rawMessage->getInterface());
                } else {
                    handleDeviceRequest(jsonMessage, rawMessage->getInterface());
                }
            }
        } else {
            DEBUG_SINRIC("[SinricPro.handleReceiveQueue()]: Signature inválida! \r\n");
            if (messageType == FSTR_SINRICPRO_request) handleDeviceRequest(jsonMessage, rawMessage->getInterface());
        }

        delete rawMessage;
    }
}
```

## handleSendQueue

```c++
void SinricProClass::handleSendQueue() {
    if (!isConnected()) return;
    if (!timestamp.getTimestamp()) return;
    while (sendQueue.size() > 0) {
        DEBUG_SINRIC("[SinricPro:handleSendQueue()]: %i message(s) in sendQueue\r\n", sendQueue.size());
        DEBUG_SINRIC("[SinricPro:handleSendQueue()]: Sending message...\r\n");

        SinricProMessage* rawMessage = sendQueue.front();
        sendQueue.pop();

        JsonDocument jsonMessage;
        deserializeJson(jsonMessage, rawMessage->getMessage());
        jsonMessage[FSTR_SINRICPRO_payload][FSTR_SINRICPRO_createdAt] = timestamp.getTimestamp();
        DEBUG_SINRIC("[SinricPro:handleSendQueue()]: appSecret: %s\r\n", appSecret.c_str());

        if (appSecret.length() == 0) {
            DEBUG_SINRIC("[SinricPro:handleSendQueue()]: ERRO! appSecret está vazio!\r\n");
            return;
        }

        DEBUG_SINRIC("[SinricPro:handleSendQueue()]: JSON antes de assinar:\r\n");
        serializeJsonPretty(jsonMessage, Serial);
        Serial.println();
        signMessage(appSecret, jsonMessage);

        char responseBuffer[1024] = {0};  // Inicializa tudo com '\0'
        size_t jsonSize = serializeJson(jsonMessage, responseBuffer, sizeof(responseBuffer) - 1);  // Deixa espaço para '\0'

        if (jsonSize == 0) {
            DEBUG_SINRIC("[SinricPro.handleSendQueue()]: ERRO! serializeJson() falhou!\r\n");
            return;
        }

        DEBUG_SINRIC("[SinricPro.handleSendQueue()]: JSON serializado com sucesso (%d bytes)\r\n", jsonSize);

        responseBuffer[jsonSize] = '\0';

        String messageStr = String(responseBuffer);
        DEBUG_SINRIC("[SinricPro.handleSendQueue()]: Response String: %s\r\n", messageStr.c_str());

#ifndef NODEBUG_SINRIC
        serializeJsonPretty(jsonMessage, DEBUG_ESP_PORT);
        Serial.println();
#endif

        switch (rawMessage->getInterface()) {
            case IF_WEBSOCKET:
                DEBUG_SINRIC("[SinricPro:handleSendQueue]: Sending to websocket\r\n");
                _websocketListener.sendMessage(messageStr);
                break;
            case IF_UDP:
                DEBUG_SINRIC("[SinricPro:handleSendQueue]: Sending to UDP\r\n");
                _udpListener.sendMessage(messageStr);
                break;
            default:
                break;
        }
        delete rawMessage;
        DEBUG_SINRIC("[SinricPro:handleSendQueue()]: message sent.\r\n");
     }
}
```
