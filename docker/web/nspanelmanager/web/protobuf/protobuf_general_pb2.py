# -*- coding: utf-8 -*-
# Generated by the protocol buffer compiler.  DO NOT EDIT!
# NO CHECKED-IN PROTOBUF GENCODE
# source: protobuf_general.proto
# Protobuf Python Version: 5.27.0
"""Generated protocol buffer code."""
from google.protobuf import descriptor as _descriptor
from google.protobuf import descriptor_pool as _descriptor_pool
from google.protobuf import runtime_version as _runtime_version
from google.protobuf import symbol_database as _symbol_database
from google.protobuf.internal import builder as _builder
_runtime_version.ValidateProtobufRuntimeVersion(
    _runtime_version.Domain.PUBLIC,
    5,
    27,
    0,
    '',
    'protobuf_general.proto'
)
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()


import web.protobuf.protobuf_formats_pb2 as protobuf__formats__pb2


DESCRIPTOR = _descriptor_pool.Default().AddSerializedFile(b'\n\x16protobuf_general.proto\x1a\x16protobuf_formats.proto\"x\n\x13HomeAssistantEntity\x12 \n\x18home_assistant_entity_id\x18\x02 \x01(\t\x12\x0f\n\x07\x63\x61n_dim\x18\x03 \x01(\x08\x12\x1d\n\x15\x63\x61n_color_temperature\x18\x04 \x01(\x08\x12\x0f\n\x07\x63\x61n_rgb\x18\x05 \x01(\x08\"\xce\x01\n\x12OpenhabLightEntity\x12\x12\n\nopenhab_id\x18\x01 \x01(\t\x12\x1b\n\x13openhab_dim_channel\x18\x02 \x01(\t\x12)\n!openhab_color_temperature_channel\x18\x03 \x01(\t\x12\x1b\n\x13openhab_rgb_channel\x18\x04 \x01(\t\x12\x0f\n\x07\x63\x61n_dim\x18\x05 \x01(\x08\x12\x1d\n\x15\x63\x61n_color_temperature\x18\x06 \x01(\x08\x12\x0f\n\x07\x63\x61n_rgb\x18\x07 \x01(\x08\"\xf6\x0e\n\x0fNSPanelSettings\x12\x0f\n\x02id\x18\x01 \x01(\x05H\x00\x88\x01\x01\x12\x13\n\x0bmac_address\x18\x02 \x01(\t\x12\x0c\n\x04name\x18\x03 \x01(\t\x12\x0c\n\x04home\x18\x04 \x01(\x05\x12\x39\n\x0c\x64\x65\x66\x61ult_page\x18\x05 \x01(\x0e\x32#.NSPanelSettings.NSPanelDefaultPage\x12 \n\x18raise_to_100_light_level\x18\x06 \x01(\x05\x12\x16\n\x0e\x63olor_temp_min\x18\x07 \x01(\x05\x12\x16\n\x0e\x63olor_temp_max\x18\x08 \x01(\x05\x12\x1a\n\x12reverse_color_temp\x18\t \x01(\x08\x12\x1c\n\x14min_button_push_time\x18\n \x01(\x05\x12\x1e\n\x16\x62utton_long_press_time\x18\x0b \x01(\x05\x12!\n\x19special_mode_trigger_time\x18\x0c \x01(\x05\x12!\n\x19special_mode_release_time\x18\r \x01(\x05\x12\x18\n\x10mqtt_ignore_time\x18\x0e \x01(\x05\x12\x18\n\x10screen_dim_level\x18\x0f \x01(\x05\x12\x1d\n\x15screensaver_dim_level\x18\x10 \x01(\x05\x12&\n\x1escreensaver_activation_timeout\x18\x11 \x01(\x05\x12\x41\n\x10screensaver_mode\x18\x12 \x01(\x0e\x32\'.NSPanelSettings.NSPanelScreensaverMode\x12\"\n\x0c\x63lock_format\x18\x13 \x01(\x0e\x32\x0c.time_format\x12-\n\x10temperature_unit\x18\x14 \x01(\x0e\x32\x13.temperature_format\x12\x13\n\x0bis_us_panel\x18\x15 \x01(\x08\x12\x1c\n\x14lock_to_default_room\x18\x16 \x01(\x08\x12\x16\n\x0ereverse_relays\x18\x17 \x01(\x08\x12\x1b\n\x13relay1_default_mode\x18\x18 \x01(\x08\x12\x1b\n\x13relay2_default_mode\x18\x19 \x01(\x08\x12\x1f\n\x17temperature_calibration\x18\x1a \x01(\x05\x12\x38\n\x0c\x62utton1_mode\x18\x1b \x01(\x0e\x32\".NSPanelSettings.NSPanelButtonMode\x12\x38\n\x0c\x62utton2_mode\x18\x1c \x01(\x0e\x32\".NSPanelSettings.NSPanelButtonMode\x12\x1f\n\x12\x62utton1_mqtt_topic\x18\x1d \x01(\tH\x01\x88\x01\x01\x12\x1f\n\x12\x62utton2_mqtt_topic\x18\x1e \x01(\tH\x02\x88\x01\x01\x12!\n\x14\x62utton1_mqtt_payload\x18\x1f \x01(\tH\x03\x88\x01\x01\x12!\n\x14\x62utton2_mqtt_payload\x18  \x01(\tH\x04\x88\x01\x01\x12&\n\x19\x62utton1_detached_light_id\x18! \x01(\x05H\x05\x88\x01\x01\x12&\n\x19\x62utton2_detached_light_id\x18\" \x01(\x05H\x06\x88\x01\x01\x12\x0e\n\x06\x64\x65nied\x18- \x01(\x08\x12\x10\n\x08\x61\x63\x63\x65pted\x18. \x01(\x08\x12\r\n\x05rooms\x18/ \x03(\x05\x12\x0e\n\x06scenes\x18\x30 \x03(\x05\x12\x12\n\nip_address\x18\x31 \x01(\t\x12@\n\x14relay1_register_type\x18\x32 \x01(\x0e\x32\".NSPanelSettings.RelayRegisterType\x12@\n\x14relay2_register_type\x18\x33 \x01(\x0e\x32\".NSPanelSettings.RelayRegisterType\"C\n\x12NSPanelDefaultPage\x12\r\n\tMAIN_PAGE\x10\x00\x12\x0f\n\x0bSCENES_PAGE\x10\x01\x12\r\n\tROOM_PAGE\x10\x02\"\xa8\x01\n\x16NSPanelScreensaverMode\x12\x1b\n\x17WEATHER_WITH_BACKGROUND\x10\x00\x12\x1e\n\x1aWEATHER_WITHOUT_BACKGROUND\x10\x01\x12\x1c\n\x18\x44\x41TETIME_WITH_BACKGROUND\x10\x03\x12\x1f\n\x1b\x44\x41TETIME_WITHOUT_BACKGROUND\x10\x04\x12\x12\n\x0eNO_SCREENSAVER\x10\x05\"Y\n\x11NSPanelButtonMode\x12\x0f\n\x0b\x44IRECT_MODE\x10\x00\x12\x11\n\rDETACHED_MODE\x10\x01\x12\x0f\n\x0b\x43USTOM_MQTT\x10\x02\x12\x0f\n\x0b\x46OLLOW_MODE\x10\x03\"*\n\x11RelayRegisterType\x12\n\n\x06SWITCH\x10\x00\x12\t\n\x05LIGHT\x10\x01\x42\x05\n\x03_idB\x15\n\x13_button1_mqtt_topicB\x15\n\x13_button2_mqtt_topicB\x17\n\x15_button1_mqtt_payloadB\x17\n\x15_button2_mqtt_payloadB\x1c\n\x1a_button1_detached_light_idB\x1c\n\x1a_button2_detached_light_idb\x06proto3')

_globals = globals()
_builder.BuildMessageAndEnumDescriptors(DESCRIPTOR, _globals)
_builder.BuildTopDescriptorsAndMessages(DESCRIPTOR, 'protobuf_general_pb2', _globals)
if not _descriptor._USE_C_DESCRIPTORS:
  DESCRIPTOR._loaded_options = None
  _globals['_HOMEASSISTANTENTITY']._serialized_start=50
  _globals['_HOMEASSISTANTENTITY']._serialized_end=170
  _globals['_OPENHABLIGHTENTITY']._serialized_start=173
  _globals['_OPENHABLIGHTENTITY']._serialized_end=379
  _globals['_NSPANELSETTINGS']._serialized_start=382
  _globals['_NSPANELSETTINGS']._serialized_end=2292
  _globals['_NSPANELSETTINGS_NSPANELDEFAULTPAGE']._serialized_start=1756
  _globals['_NSPANELSETTINGS_NSPANELDEFAULTPAGE']._serialized_end=1823
  _globals['_NSPANELSETTINGS_NSPANELSCREENSAVERMODE']._serialized_start=1826
  _globals['_NSPANELSETTINGS_NSPANELSCREENSAVERMODE']._serialized_end=1994
  _globals['_NSPANELSETTINGS_NSPANELBUTTONMODE']._serialized_start=1996
  _globals['_NSPANELSETTINGS_NSPANELBUTTONMODE']._serialized_end=2085
  _globals['_NSPANELSETTINGS_RELAYREGISTERTYPE']._serialized_start=2087
  _globals['_NSPANELSETTINGS_RELAYREGISTERTYPE']._serialized_end=2129
# @@protoc_insertion_point(module_scope)
