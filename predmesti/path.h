#ifndef PATH_H
#define PATH_H

#include <Arduino.h>

enum class VPathStatus { clear, clear_soon, occupied, reserved };

const int PATH_TIMEOUT = 20000;
const int PATH_CLEAR_SOON = 300;

class VPath
{
  public:
    VPath(int id, const char* name);
    VPathStatus get_state();
    const char* get_name();
    void occupy();
    void release();
    void release_soon();
    void reserve();
    void cancel_reservation();
    bool is_clear();
    void occupation_timeout();

    int vehicle_pull();
    void vehicle_push(int vehicle);
    
  private: 
    const int _id;
    const char* _name;
    VPathStatus _state;
    int _vehicle;
    unsigned long _lastOccupiedTime;

};
#endif /* PATH_H */
