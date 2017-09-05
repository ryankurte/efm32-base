/**
 * Gatt database struct definitions for simple api
 */
#ifndef GATT_DEF_H
#define GATT_DEF_H

#include "bg_types.h"

/* Datatype of attribute value */
enum gatt_att_datatype{
  gatt_att_datatype_const,
  gatt_att_datatype_dynamic,  /**<Fixed length attribute*/
  gatt_att_datatype_dynamic_varlen,   /**<Variable length attribute*/
  gatt_att_datatype_clientconfig,
  gatt_att_datatype_serverconfig,
  gatt_att_datatype_privacy,  /**< GAP privacy flag */
  gatt_att_datatype_reconnection_address,
  gatt_att_datatype_user,  /**<Attribute information is asked from user*/

  gatt_att_datatype_data_parser=0xf0,  /**< Internal datatype used by attr parser TODO: get rid of this*/
};

/* Attribute permissions */
enum gatt_att_perm{
  gatt_att_perm_readable                = 0x0001,
  gatt_att_perm_writable                = 0x0002,
  gatt_att_perm_write_no_response       = 0x0004,
  gatt_att_perm_encrypted_read          = 0x0010,
  gatt_att_perm_authenticated_read      = 0x0020,
  gatt_att_perm_bonded_read             = 0x0040,
  gatt_att_perm_encrypted_write         = 0x0100,
  gatt_att_perm_authenticated_write     = 0x0200,
  gatt_att_perm_bonded_write            = 0x0400,
  gatt_att_perm_discoverable            = 0x0800
};

/* Characteristic properties */
enum gatt_char_prop{
  gatt_char_prop_broadcast    = 0x01,
  gatt_char_prop_read         = 0x02,
  gatt_char_prop_writenoresp  = 0x04,
  gatt_char_prop_write        = 0x08,
  gatt_char_prop_notify       = 0x10,
  gatt_char_prop_indicate     = 0x20,
  gatt_char_prop_writesign    = 0x40,
  gatt_char_prop_extended     = 0x80,
};

enum gatt_char_client_conf {
  gatt_char_client_conf_notification = 0x0001,
  gatt_char_client_conf_indication   = 0x0002
};

/*Generic databuffer where first byte is length*/
struct bg_gattdb_buffer_with_len{
  uint16_t len;  //current size of data in buffer
  uint8_t data[];  //size is maxlen in attribute_chrvalue-struct
};

struct bg_gattdb_attribute_chrvalue{
  uint8_t properties;   //Characteristic properties, same as in characteristic descriptor gatt_char_prop
  uint8_t index;  //index used to map sending notifications and indications to correct bit in enable bitmask
  uint16_t max_len;  //Maximum length of data in buffer
  union {
    uint8_t *data;
    struct bg_gattdb_buffer_with_len *data_varlen;
  };
};

struct bg_gattdb_attribute_config{
  uint8_t flags;   //client characteristic flags allowed 1-notification, 2-indication)
  uint8_t index;   //reverse mapping to dynamic table
  uint8_t clientconfig_index;   // index to client config.
};
struct bg_gattdb_attribute{
  uint16_t uuid;
  uint16_t permissions;   //gatt_attribute_permission
  uint16_t caps;   // Capability bit flags
  uint8_t min_key_size;   // minumum key size required in bytes to access attribute
  uint8_t datatype;   //Just use uint8_t Do not use enum type, may cause compatibility troubles
  union {
    const struct bg_gattdb_buffer_with_len *constdata;     //generic constant data
    const struct bg_gattdb_attribute_chrvalue *dynamicdata;     //Modifiable data
    struct bg_gattdb_attribute_config configdata;
  };
};

struct bg_gattdb_def{
  const struct bg_gattdb_attribute       *attributes;
  int                                     attributes_max;
  int                                     uuidtable_16_size;
  const uint16_t                         *uuidtable_16;
  int                                     uuidtable_128_size;
  const uint8_t                          *uuidtable_128;
  int                                     attributes_dynamic_max;
  const uint16_t                         *attributes_dynamic_mapping;   //Table mapping dynamic indexes to attribute table index

  //Advertised services tables
  const uint8_t                          *adv_uuid16;
  int                                     adv_uuid16_num;
  const uint8_t                          *adv_uuid128;
  int                                     adv_uuid128_num;
  uint16_t                                caps_mask;
  uint16_t                                enabled_caps;
};

extern const struct bg_gattdb_def *bg_gattdb;

#endif
