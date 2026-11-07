#ifndef EEPROM_Address_h 
#define EEPROM_Address_h 

#define EEPROM_HOME_START_ADDR 0            // uint16_t
#define EEPROM_V_UPPER_LIMIT_START_ADDR 2   // uint16_t
#define EEPROM_V_LOWER_LIMIT_START_ADDR 4   // uint16_t
#define EEPROM_POLARITY_H_ADDR 6            // StmPolarity
#define EEPROM_POLARITY_V_ADDR 7            // StmPolarity
#define EEPROM_7_IN_START_ADDR 8            // uint16_t
#define EEPROM_10_IN_START_ADDR 10          // uint16_t
#define EEPROM_12_IN_START_ADDR 12          // uint16_t
#define EEPROM_AZ_OFFSET 14                 // uint16_t
#define EEPROM_SELECTED_SIZE_ADDR 16        // RecordSize
#define EEPROM_SELECTED_SPEED_START_ADDR 17 // float
#define EEPROM_POLARITY_H_MOTOR_ADDR 21     // StmPolarity

#endif 