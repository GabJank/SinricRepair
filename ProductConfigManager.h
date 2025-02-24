#pragma once 

#include <ArduinoJson.h>
#include <Preferences.h>
#include "SPIFFS.h"

/**
 * @struct ProductConfig_t
 * @brief Holds the configuration data of the product.
 * 
 * @note If you need additional security for app secret, please save this to NVS
 */
struct ProductConfig_t {
  char appKey[38];
  char appSecret[76];  
  
  char light_1_id[26];
  char light_1_name[32];
  
  char dimmable_switch_2_id[26];
  char dimmable_switch_2_name[32];
  
  char switch_3_id[26];
  char switch_3_name[32];
  
  char switch_4_id[26];
  char switch_4_name[32];
  
  char switch_5_id[26];
  char switch_5_name[32];
  };

/**
 * @class ProductConfigManager
 * @brief Manages the loading, saving, and clearing of device configuration.
 */
class ProductConfigManager {
public:
  /**
     * @brief Constructor for ProductConfigManager.
     * @param config Reference to a ProductConfig_t object to store the configuration.
     */
  ProductConfigManager(ProductConfig_t &config);

  /**
     * @brief Destructor for ProductConfigManager.
     */
  ~ProductConfigManager();

  /**
     * @brief Loads the configuration from the file system.
     * @return bool True if loading was successful, false otherwise.
     */
  bool loadConfig();

  /**
     * @brief Saves the configuration to the file system.
     * @param doc JsonDocument containing the configuration to save.
     * @return bool True if saving was successful, false otherwise.
     */
  bool saveJsonConfig(const JsonDocument &doc);

  /**
     * @brief Clears the configuration from the file system and memory.
     * @return bool True if clearing was successful, false otherwise.
     */
  bool clear();

private:
  ProductConfig_t &config;  ///< Reference to the ProductConfig_t object
  Preferences preferences;  ///< Preferences object for storing data
};

ProductConfigManager::ProductConfigManager(ProductConfig_t &config) : config(config) { }
ProductConfigManager::~ProductConfigManager(){ }
 
bool ProductConfigManager::loadConfig() {
  Serial.printf("[ProductConfigManager.loadConfig()]: Loading config...\r\n");

  File configFile = SPIFFS.open(PRODUCT_CONFIG_FILE, "r");

  if (!configFile) {
      Serial.printf("[ProductConfigManager.loadConfig()]: Failed to open config file!!\r\n");
      return false;
  }

  if (configFile.size() == 0) {  // Verifica se o arquivo está vazio
      Serial.println("[ProductConfigManager.loadConfig()]: Empty config file!");
      configFile.close();
      return false;
  }

  Serial.println("Conteúdo do arquivo antes de carregar:");
  while (configFile.available()) {
      Serial.write(configFile.read());
  }
  Serial.println();
  configFile.seek(0);  // Voltar ao início do arquivo antes de carregar o JSON

  StaticJsonDocument<2048> doc;
  DeserializationError err = deserializeJson(doc, configFile);

  configFile.close();  // Fechar o arquivo logo após a leitura

  if (err) {
      Serial.printf("[ProductConfigManager.loadConfig()]: deserializeJson() failed: %s\r\n", err.c_str());
      return false;
  }

  serializeJsonPretty(doc, Serial);

  // Garantir que "devices" contém pelo menos 5 elementos antes de acessar
  if (!doc.containsKey("devices") || !doc["devices"].is<JsonArray>() || doc["devices"].size() < 5) {
      Serial.println("[ProductConfigManager.loadConfig()]: JSON não contém dispositivos suficientes!");
      return false;
  }

  // Copy configuration data from JSON to config struct
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

  // Gravar diretamente no arquivo SPIFFS
  size_t bytesWritten = serializeJson(doc, configFile);
  configFile.flush();  
  configFile.close();

  Serial.printf("[ProductConfigManager.saveJsonConfig()] Bytes written: %u\r\n", bytesWritten);

  if (bytesWritten == 0) {
      Serial.printf("[ProductConfigManager.saveJsonConfig()] Erro! Nenhum byte foi gravado no arquivo!\r\n");
      return false;
  }

  // Update config struct with new values
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
 

/**
* @brief Clear stored configuations
* @return
*      ok
*/ 
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
}