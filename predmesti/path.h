#ifndef PATH_H
#define PATH_H

#include <Arduino.h>

enum class VPathStatus { clear, occupied, reserved };

class VPath
{
  public:
    VPath(const char* name);
    void car_in();
    void car_out();
    void occupy();
    void release();
    void reserve();
    void cancel_reservation();
    bool is_clear();
    
  private: 
    const char* _name;
    VPathStatus _state;

};
#endif /* PATH_H */
