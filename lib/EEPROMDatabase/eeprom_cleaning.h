
void Make_Sure_EEPROM_is_Clear(uint8_t BYTE0, uint8_t BYTE1)
{
	int length = (int)EEPROM.length() ;
	if (EEPROM.read(length-2) != BYTE0 || EEPROM.read(length-1) != BYTE1)
	{
		DEBUG("EEPROM Error")
		for (int i = 0 ; i < length; i++) {
			#ifndef ESP_H
    		EEPROM.update(i, 0);
			#else
			EEPROM.write(i, 0);
			#endif
  		}
		#ifndef ESP_H
		EEPROM.update(length-2,BYTE0);
		EEPROM.update(length-1,BYTE1);
		#else
		EEPROM.write(length-2, BYTE0);
		EEPROM.write(length-1, BYTE1);
		#endif
		DEBUG("EEPROM cleared")
	}
	else
	{
		DEBUG("EEPROM is ok")
	}
}