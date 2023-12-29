#include "weather.hpp"
#include "home_assistant_manager/home_assistant_manager.hpp"
#include "mqtt_manager/mqtt_manager.hpp"
#include "mqtt_manager_config/mqtt_manager_config.hpp"
#include "openhab_manager/openhab_manager.hpp"
#include <bits/types/time_t.h>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/bind.hpp>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <fmt/core.h>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <spdlog/spdlog.h>
#include <string>

void MQTTManagerWeather::update_config() {
  if (MqttManagerConfig::weather_controller.compare("home_assistant") == 0) {
    SPDLOG_INFO("Initializing weather controller for Home Assistant.");
    HomeAssistantManager::attach_event_observer(this);
    OpenhabManager::detach_event_observer(MqttManagerConfig::openhab_current_weather_item, boost::bind(&MQTTManagerWeather::openhab_current_weather_callback, this, _1));
    OpenhabManager::detach_event_observer(MqttManagerConfig::openhab_forecast_weather_item, boost::bind(&MQTTManagerWeather::openhab_forecast_weather_callback, this, _1));
  } else if (MqttManagerConfig::weather_controller.compare("openhab") == 0) {
    SPDLOG_INFO("Initializing weather controller for OpenHAB.");
    SPDLOG_DEBUG("Current weather item: {}", MqttManagerConfig::openhab_current_weather_item);
    SPDLOG_DEBUG("Forecast weather item: {}", MqttManagerConfig::openhab_forecast_weather_item);
    HomeAssistantManager::detach_event_observer(this);
    OpenhabManager::attach_event_observer(MqttManagerConfig::openhab_current_weather_item, boost::bind(&MQTTManagerWeather::openhab_current_weather_callback, this, _1));
    OpenhabManager::attach_event_observer(MqttManagerConfig::openhab_forecast_weather_item, boost::bind(&MQTTManagerWeather::openhab_forecast_weather_callback, this, _1));
  } else {
    SPDLOG_ERROR("Unsupported weather controller '{}'.", MqttManagerConfig::weather_controller);
  }
}

bool MQTTManagerWeather::home_assistant_event_callback(nlohmann::json &event_data) {
  if (std::string(event_data["event"]["data"]["entity_id"]).compare(MqttManagerConfig::home_assistant_weather_entity) == 0) {
    nlohmann::json new_state = event_data["event"]["data"]["new_state"];

    this->_forecast_weather_info.clear();
    for (nlohmann::json forecast : new_state["attributes"]["forecast"]) {
      weather_info info;
      SPDLOG_DEBUG("Loading weather forecast for {}", std::string(forecast["datetime"]));
      info.condition = forecast["condition"];
      info.wind_speed = forecast["wind_speed"];
      info.precipitation = forecast["precipitation"];
      info.precipitation_probability = forecast["precipitation_probability"];
      info.temperature_low = forecast["templow"];
      info.temperature_high = forecast["temperature"];

      // Get day of week
      std::string datetime = forecast["datetime"];
      std::vector<std::string> datetime_parts;
      size_t pos = datetime.find("T");
      std::string date = datetime.substr(0, pos);
      std::string year = date.substr(0, date.find("-"));
      date.erase(0, date.find("-") + 1);
      std::string month = date.substr(0, date.find("-"));
      date.erase(0, date.find("-") + 1);
      std::string day_of_month = date;

      std::tm tm = {01, 00, 00, atoi(day_of_month.c_str()), atoi(month.c_str()) - 1, atoi(year.c_str()) - 1900};
      std::time_t utc_time = std::mktime(&tm);
      const std::tm *localtime = std::localtime(&utc_time);
      info.time = *localtime;

      switch (localtime->tm_wday) {
      case 0:
        info.day = "Sun";
        break;
      case 1:
        info.day = "Mon";
        break;
      case 2:
        info.day = "Tue";
        break;
      case 3:
        info.day = "Wed";
        break;
      case 4:
        info.day = "Thu";
        break;
      case 5:
        info.day = "Fri";
        break;
      case 6:
        info.day = "Sat";
        break;
      default:
        info.day = std::to_string(localtime->tm_wday);
        break;
      }

      this->_forecast_weather_info.push_back(info);
    }

    SPDLOG_DEBUG("Loaded forecast for {} days.", this->_forecast_weather_info.size());

    if (this->_forecast_weather_info.size() > 0) {
      this->_current_condition = this->_forecast_weather_info[0].condition;
      this->_current_precipitation_probability = this->_forecast_weather_info[0].precipitation_probability;
      this->_current_wind_speed = new_state["attributes"]["wind_speed"];
      this->_current_temperature = new_state["attributes"]["temperature"];
      std::time_t time = std::time({});
      this->_current_weather_time = *std::localtime(&time);
    } else {
      SPDLOG_ERROR("Failed to process forecast information from Home Assistant.");
      return true;
    }

    this->send_state_update();
    return true;
  } else if (std::string(event_data["event"]["data"]["entity_id"]).compare(MqttManagerConfig::home_assistant_sun_entity) == 0) {
    std::string next_rising = event_data["event"]["data"]["new_state"]["attributes"]["next_rising"];
    std::string next_setting = event_data["event"]["data"]["new_state"]["attributes"]["next_setting"];

    // Get time parts from sunrise
    std::string rising_date = next_rising.substr(0, next_rising.find("T"));
    next_rising.erase(0, next_rising.find("T") + 1);
    std::string rising_time = next_rising.substr(0, next_rising.find("+"));
    next_rising.erase(0, next_rising.find("+") + 1);

    std::string rising_hour = rising_time.substr(0, rising_time.find(":"));
    rising_time.erase(0, rising_time.find(":") + 1);
    std::string rising_minute = rising_time.substr(0, rising_time.find(":"));
    rising_time.erase(0, rising_time.find(":") + 1);

    // Get time parts from sunrise
    std::string setting_date = next_setting.substr(0, next_setting.find("T"));
    next_setting.erase(0, next_setting.find("T") + 1);
    std::string setting_time = next_setting.substr(0, next_setting.find("+"));
    next_setting.erase(0, next_setting.find("+") + 1);

    std::string setting_hour = setting_time.substr(0, setting_time.find(":"));
    setting_time.erase(0, setting_time.find(":") + 1);
    std::string setting_minute = setting_time.substr(0, setting_time.find(":"));
    setting_time.erase(0, setting_time.find(":") + 1);

    this->_next_sunrise_hour = atoi(rising_hour.c_str());
    this->_next_sunrise = fmt::format("{}:{}", rising_hour, rising_minute);
    this->_next_sunset_hour = atoi(setting_hour.c_str());
    this->_next_sunset = fmt::format("{}:{}", setting_hour, setting_minute);
  }
  return false;
}

void MQTTManagerWeather::openhab_current_weather_callback(nlohmann::json event_data) {
  std::string weather_state;
  if (std::string(event_data["type"]).compare("ItemStateChangedEvent") == 0) {
    nlohmann::json payload = nlohmann::json::parse(std::string(event_data["payload"]));
    weather_state = payload["value"];
  } else if (std::string(event_data["type"]).compare("ItemStateFetched") == 0) {
    weather_state = event_data["payload"]["state"];
  }

  if (weather_state.size() > 0) {
    nlohmann::json weather_json = nlohmann::json::parse(weather_state);

    this->_current_condition = weather_json["weather"][0]["description"];
    this->_current_condition_id = weather_json["weather"][0]["id"];

    this->_current_temperature = weather_json["main"]["temp"];
    this->_current_wind_speed = weather_json["wind"]["speed"];

    time_t weather_time = uint64_t(weather_json["dt"]);
    this->_current_weather_time = *std::localtime(&weather_time);

    time_t sunrise = weather_json["sys"]["sunrise"];
    time_t sunset = weather_json["sys"]["sunset"];
    std::tm sunrise_time = *std::localtime(&sunrise);
    std::tm sunset_time = *std::localtime(&sunset);
    this->_next_sunrise_hour = sunrise_time.tm_hour;
    this->_next_sunrise = fmt::format("{:0>2}:{:0>2}", sunrise_time.tm_hour, sunrise_time.tm_min);
    this->_next_sunset_hour = sunset_time.tm_hour;
    this->_next_sunset = fmt::format("{:0>2}:{:0>2}", sunset_time.tm_hour, sunset_time.tm_min);
  }

  this->send_state_update();
}

void MQTTManagerWeather::openhab_forecast_weather_callback(nlohmann::json event_data) {
  std::string forecast_state;
  if (std::string(event_data["type"]).compare("ItemStateChangedEvent") == 0) {
    nlohmann::json payload = nlohmann::json::parse(std::string(event_data["payload"]));
    forecast_state = payload["value"];
  } else if (std::string(event_data["type"]).compare("ItemStateFetched") == 0) {
    forecast_state = event_data["payload"]["state"];
  }

  if (forecast_state.size() > 0) {
    nlohmann::json forecast_json = nlohmann::json::parse(forecast_state);

    struct weather_info day_summary;
    std::tm day_info_time;

    this->_forecast_weather_info.clear();
    for (nlohmann::json day_info : forecast_json["list"]) {
      time_t dt = uint64_t(day_info["dt"]);
      std::tm current_time = *std::localtime(&dt);
      SPDLOG_DEBUG("Processing OpenHAB weather(OpenWeatherMap) for {}-{}-{} {}:{}", current_time.tm_year + 1900, current_time.tm_mon, current_time.tm_mday, current_time.tm_hour, current_time.tm_min);

      if (day_info_time.tm_year == 0 || current_time.tm_hour <= 12) {
        day_summary.condition = day_info["weather"][0]["description"];
        day_summary.condition_id = day_info["weather"][0]["id"];
        day_summary.wind_speed = day_info["wind"]["speed"];
        day_summary.temperature_low = day_info["main"]["temp_min"];
        day_summary.temperature_low = day_info["main"]["temp_max"];
        day_summary.precipitation = 0; // Unsed
        day_summary.precipitation_probability = float(day_info["pop"]) * 100;
        day_summary.time = current_time;

        switch (current_time.tm_wday) {
        case 0:
          day_summary.day = "Sun";
          break;
        case 1:
          day_summary.day = "Mon";
          break;
        case 2:
          day_summary.day = "Tue";
          break;
        case 3:
          day_summary.day = "Wed";
          break;
        case 4:
          day_summary.day = "Thu";
          break;
        case 5:
          day_summary.day = "Fri";
          break;
        case 6:
          day_summary.day = "Sat";
          break;
        default:
          day_summary.day = std::to_string(current_time.tm_wday);
          break;
        }
      }

      if (day_info_time.tm_year != 0 && day_info_time.tm_mday != current_time.tm_mday) {
        this->_forecast_weather_info.push_back(day_summary);
        SPDLOG_DEBUG("Adding OpenHAB weather(OpenWeatherMap) for {}-{}-{} {}:{} to forecast list.", current_time.tm_year + 1900, current_time.tm_mon, current_time.tm_mday, current_time.tm_hour, current_time.tm_min);
      }

      day_info_time = current_time;
    }
    this->_forecast_weather_info.push_back(day_summary); // Add the last forecast item
    this->send_state_update();
  }
}

std::string MQTTManagerWeather::_get_icon_from_mapping(std::string &condition, uint8_t hour) {
  SPDLOG_DEBUG("Checking for icon matching condition: {}", condition);
  if (MqttManagerConfig::weather_controller.compare("homeassistant") == 0) {
    for (nlohmann::json mapping : MqttManagerConfig::icon_mapping["home_assistant_weather_mappings"]) {
      if (std::string(mapping["condition"]).compare(condition) == 0) {
        return std::string(mapping["character-mapping"]);
      }
    }
  } else if (MqttManagerConfig::weather_controller.compare("openhab") == 0) {
    for (nlohmann::json mapping : MqttManagerConfig::icon_mapping["openweathermap_weather_mappings"]) {
      if (std::string(mapping["id"]).compare(condition) == 0) {
        if (mapping.contains("character-mapping")) {
          return std::string(mapping["character-mapping"]);
        } else if (mapping.contains("character-mapping-day") && hour >= MQTTManagerWeather::_next_sunrise_hour && hour <= MQTTManagerWeather::_next_sunset_hour) {
          return mapping["character-mapping-day"];
        } else if (mapping.contains("character-mapping-night") && (hour <= MQTTManagerWeather::_next_sunrise_hour || hour >= MQTTManagerWeather::_next_sunset_hour)) {
          return mapping["character-mapping-night"];
        } else {
          SPDLOG_ERROR("Found matching condition for {} but no icon-mapping!", condition);
          SPDLOG_ERROR("Matching condition current hour: {}, sunrise: {}, sunset: {}", hour, MQTTManagerWeather::_next_sunrise_hour, MQTTManagerWeather::_next_sunset_hour);
        }
      }
    }
  } else {
    SPDLOG_ERROR("Unknown controller {}.", MqttManagerConfig::weather_controller);
  }

  SPDLOG_ERROR("Couldn't find a mapping for condition {} using controller {}.", condition, MqttManagerConfig::weather_controller);
  return "";
}

void MQTTManagerWeather::send_state_update() {
  nlohmann::json weather_info;
  std::list<nlohmann::json> forecast;
  for (struct weather_info info : this->_forecast_weather_info) {
    nlohmann::json forecast_data;
    if (MqttManagerConfig::weather_controller.compare("homeassistant") == 0) {
      forecast_data["icon"] = this->_get_icon_from_mapping(info.condition, info.time.tm_hour);
    } else if (MqttManagerConfig::weather_controller.compare("openhab") == 0) {
      std::string condition_id = std::to_string(info.condition_id);
      forecast_data["icon"] = this->_get_icon_from_mapping(condition_id, info.time.tm_hour);
    }
    std::string pre = std::to_string((int)(info.precipitation + 0.5));
    pre.append(this->_precipitation_unit);
    forecast_data["pre"] = pre;
    std::string prepro = std::to_string((int)(info.precipitation_probability + 0.5));
    prepro.append("%");
    forecast_data["prepro"] = prepro;

    std::string templow = std::to_string((int)(info.temperature_low + 0.5));
    templow.append("°");
    std::string temphigh = std::to_string((int)(info.temperature_high + 0.5));
    temphigh.append("°");
    std::string temp_display = temphigh;
    temp_display.append("/");
    temp_display.append(templow);
    forecast_data["maxmin"] = temp_display;

    std::string wind = std::to_string((int)(info.wind_speed + 0.5));
    wind.append(this->_windspeed_unit);
    forecast_data["wind"] = wind;
    forecast_data["day"] = info.day;
    forecast.push_back(forecast_data);
  }
  weather_info["forecast"] = forecast;
  if (MqttManagerConfig::weather_controller.compare("homeassistant") == 0) {
    weather_info["icon"] = this->_get_icon_from_mapping(this->_current_condition, this->_current_weather_time.tm_hour);
  } else if (MqttManagerConfig::weather_controller.compare("openhab") == 0) {
    std::string condition_id = std::to_string(this->_current_condition_id);
    weather_info["icon"] = this->_get_icon_from_mapping(condition_id, this->_current_weather_time.tm_hour);
  }
  std::string temp = std::to_string((int)(this->_current_temperature + 0.5));
  temp.append("°");
  weather_info["temp"] = temp;
  std::string wind = std::to_string((int)(this->_current_wind_speed + 0.5));
  wind.append(this->_windspeed_unit);
  weather_info["wind"] = wind;

  weather_info["sunrise"] = this->_next_sunrise;
  weather_info["sunset"] = this->_next_sunset;

  MQTT_Manager::publish("nspanel/status/weather", weather_info.dump(), true);
}
