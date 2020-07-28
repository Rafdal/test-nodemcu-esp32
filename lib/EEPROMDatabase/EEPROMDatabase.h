
#ifndef EEPROM_DATABASE
#define EEPROM_DATABASE

// #include <EEPROM.h>

#ifdef ESP_H
#define ASD
#endif

class EEPROMDatabase
{
private:
    uint8_t max_idx; // Max cant de registros
    uint16_t first_idx;
    uint8_t reg_size;

    uint8_t* regptr; // Usado para devolver el puntero del registro

    int _eepos_(uint8_t idx);

    #ifdef ESP_H
        void _esp32update(int epos, uint8_t data){
            if (EEPROM.read(epos) != data)
            {
                EEPROM.write(epos, data);
                EEPROM.commit();
            }
        }
    #endif

    bool marker;

public:
    EEPROMDatabase(uint16_t _max_mem, uint8_t registry_size, uint16_t _first_idx, bool _marker); // (MAX Bytes,  Size per Reg,  Starting mem Index)
    ~EEPROMDatabase(){
        if(regptr)
            delete[] regptr;
    }
    uint8_t size(); // Max index
    void clear(); // Borra toda la BD

    uint8_t search(uint8_t reg[]); // (idx) del reg si hay match. (-1) Si no hay

    void del(uint8_t idx);
    void set(uint8_t reg[], uint8_t idx);
    bool available(uint8_t idx); // true si esa posicion esta libre
    int available(); //  > 0 si hay espacio libre
    bool add(uint8_t reg[]);
    uint8_t* get(uint8_t idx);
};

// (MAX Bytes,  Size per Reg,  Starting mem Index, uso otro byte marcador)
EEPROMDatabase::EEPROMDatabase(uint16_t max_memory, uint8_t registry_size, uint16_t _first_idx, bool _marker)
{
    marker = _marker;
    if (marker)
    {
        max_idx = max_memory / (registry_size+1); // +1 por el indicador de registro usado
    }
    else
    {
        max_idx = max_memory / (registry_size);
    }
    reg_size = registry_size;
    first_idx = _first_idx;

    regptr = new uint8_t[reg_size];
    for (uint8_t i = 0; i < reg_size; i++)
        regptr[i] = 0;    
}

// Devuelve EPOS, (error = -1)
int EEPROMDatabase::_eepos_(uint8_t idx)
{
    if (idx < max_idx)
    {
        if (marker)
        {
            return ((int)first_idx + ((int)idx)*((int)reg_size+1));
        }
        return ((int)first_idx + ((int)idx)*((int)reg_size));
    }
    return -1;
}

// Devuelve el max index de la tabla
uint8_t EEPROMDatabase::size()
{
    return max_idx;
}

// BORRA toda la tabla
void EEPROMDatabase::clear() 
{
    for (uint8_t  i = 0; i < max_idx; i++)
    {
        del(i);
    }
}

// Elimina un registro
void EEPROMDatabase::del(uint8_t idx)
{
    int epos = _eepos_(idx);
    if (epos != -1)
    {
        #ifndef ESP_H
            EEPROM.update(epos, 0); // Actualizo el indicador
        #else
            _esp32update(epos, 0);
        #endif
    }
}

// (idx) del reg si hay match. (-1) Si no hay
uint8_t EEPROMDatabase::search(uint8_t reg[])
{
    for (uint8_t i = 0; i < max_idx; i++)
    {
        if (!available(i))
        {
            get(i);
            for (uint8_t j = 0; j < reg_size; j++)
            {
                if (regptr[j] != reg[j])
                {
                    break;
                }
                if (regptr[j] == reg[j] && j == reg_size-1)
                {
                    return i;
                }
            }
        }
    }
    return -1;
}

void EEPROMDatabase::set(uint8_t reg[], uint8_t idx)
{
    int epos = _eepos_(idx);
    if (epos != -1)
    {
        if(marker)
        {
            #ifndef ESP_H
                EEPROM.update(epos, HIGH);
            #else
                _esp32update(epos, HIGH);
            #endif
            for (uint8_t i = 0; i < reg_size; i++)
            {
                #ifndef ESP_H
                    EEPROM.update(epos+1+i, reg[i]);
                #else
                    _esp32update(epos+1+i, reg[i]);
                #endif
            }   
        }
        else
        {
            for (uint8_t i = 0; i < reg_size; i++)
            {
                #ifndef ESP_H
                    EEPROM.update(epos+i, reg[i]);
                #else
                    _esp32update(epos+i, reg[i]);
                #endif
            }  
        }
    }
}

// true si esta VACIO. false si esta USADO
bool EEPROMDatabase::available(uint8_t idx)
{
    return (EEPROM.read(_eepos_(idx)) == 0) && (_eepos_(idx) != -1);
}

// -1 si no hay espacio libre. (>= 0) si hay
int EEPROMDatabase::available()
{
    for (uint8_t i = 0; i < max_idx; i++)
    {
        if (available(i))
        {
            return i;
        }
    }
    return -1;
}

bool EEPROMDatabase::add(uint8_t reg[])
{
    for (uint8_t i = 0; i < max_idx; i++) // Busco un registro libre
    {
        if(available(i)) // Si esta libre
        {
            set(reg, i);
            return true;
            break;
        }
    }
    return false;
}

uint8_t* EEPROMDatabase::get(uint8_t idx)
{
    int epos = _eepos_(idx);
    for (uint8_t i = 0; i < reg_size; i++)
        regptr[i]=0;

    if (epos != -1 && !available(idx))
    {
        for (uint8_t i = 0; i < reg_size; i++)
        {
            if (marker)
            {
                regptr[i] = EEPROM.read(epos + 1 + i);
            }
            else
            {
                regptr[i] = EEPROM.read(epos + i);
            }
        }
    }
    return regptr;
}

#endif