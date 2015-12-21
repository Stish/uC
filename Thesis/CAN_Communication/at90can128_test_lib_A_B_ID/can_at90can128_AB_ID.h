#ifdef can_at90can128_AB_ID
void can_set_baudrate(uint16_t baud);
void can_set_interrupt(uint8_t mode);
void can_select_mob(uint8_t mob);
void can_set_mob_id_mask(uint32_t idm, uint8_t id_type);
void can_set_mob_id(uint32_t id, uint8_t id_type);
void can_set_mob_mode(uint8_t mode, uint8_t id_type);
void can_set_mob_data(uint8_t *data, uint8_t length);
void can_set_mob_interrupt(uint8_t object);
void can_clear_mob_interrupt(uint8_t object);
void can_init(uint16_t baud, uint8_t intmode);
void can_enable_mob(uint8_t object, uint8_t mode, can_message msg);
void can_disable_mob(uint8_t object);
void can_send_data(uint8_t mob, can_message msg);
void clear_all_mobs(void);
#endif