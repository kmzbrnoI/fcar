#ifndef PATH_H
#define PATH_H

#include <Arduino.h>

/* Constant for signals */
enum class VPathStatus { clear, occupied };

/* Semaphore logic */
class VPath
{
  public:
    VPath(const char* name);
    void occupy();
    void release();
    
  private: 
    const char* _name;
    VPathStatus _state;

};
#endif /* PATH_H */
