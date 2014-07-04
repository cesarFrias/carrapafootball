#ifndef _TIMED_H
#define _TIMED_D
class TimedExecution {

  private:
    unsigned long last_run;
    uint16_t delay;

  public:
    TimedExecution(uint16_t minimum_delay){
      this->delay = minimum_delay;
    }
    TimedExecution(){
      this->delay = 250;
    }

    void set_delay(uint16_t d){
      this->delay = d;
    }

    uint16_t get_delay(){
      return this->delay;    
    }

    bool expired(){
      unsigned long now = millis();

      if (now - last_run >= delay){
        this->last_run = now;
        return true;
      }
      return false;
    }

    void reset(){
      this->last_run = millis();
    }

};

#endif
