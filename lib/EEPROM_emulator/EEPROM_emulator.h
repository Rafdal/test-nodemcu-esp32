#ifndef EEPROM_H
#define EEPROM_H

class EEPROM_emulator
{
private:
    const uint16_t _size = 512;
    uint8_t data[512];
public:
    EEPROM_emulator() {}
    ~EEPROM_emulator() {}

    uint16_t length(){ return _size; }
    uint8_t read(int pos);
    void write(int pos, uint8_t val);
    void commit(){};

}EEPROM;

uint8_t EEPROM_emulator::read(int pos)
{
    if (pos >= 0 || pos < _size)
    {
        return data[pos];
    }
    return 0;    
}

void EEPROM_emulator::write(int pos, uint8_t val)
{
    if (pos >= 0 || pos < _size)
    {
        data[pos] = val;
    }
}


#endif