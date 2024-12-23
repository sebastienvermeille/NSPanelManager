#ifndef MQTT_MANAGER_NSPM_SCENE_H
#define MQTT_MANAGER_NSPM_SCENE_H

#include "entity/entity.hpp"
#include "light/light.hpp"
#include "room/room.hpp"
#include <list>
#include <nlohmann/json.hpp>
#include <scenes/scene.hpp>
#include <string>

struct LightState {
  Light *_light;
  uint16_t light_id;
  uint8_t brightness;
  uint16_t color_temperature;
  uint16_t hue;
  uint16_t saturation;
  std::string color_mode;
};

class NSPMScene : public Scene {
public:
  NSPMScene(nlohmann::json &data);
  void update_config(nlohmann::json &config);
  void activate();
  void save();
  void remove();
  uint16_t get_id();
  void post_init();
  MQTT_MANAGER_ENTITY_TYPE get_type();
  MQTT_MANAGER_ENTITY_CONTROLLER get_controller();
  void remove_light(Light *light);

  static void light_destroyed_callback(Light *light);

private:
  bool _is_global_scene;
  uint16_t _id;
  std::string _name;
  uint16_t _room_id;
  Room *_room;
  std::list<LightState> _light_states;
};

#endif // !MQTT_MANAGER_NSPM_SCENE_H
