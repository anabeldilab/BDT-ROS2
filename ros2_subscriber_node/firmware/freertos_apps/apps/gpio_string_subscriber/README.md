# Subscriber node for string messages

The first implementation of this code is a micro-ROS app that subscribes to a topic of type `std_msgs/msg/String` and change a LED state depending on the received message.

# Servomotors implementation

The second implementation of this code is a micro-ROS app that subscribes to a topic of type `std_msgs/msg/String` and change the position of two servomotors depending on the received message.

There are two servomotors connected to the board, one in the pin X and the other in the pin X. The servomotors are controlled by PWM signals, so the position is controlled by the duty cycle of the signal. The duty cycle is calculated from the position in degrees, so the position is given in degrees.

Un prototipo de cambio de pwm sería el siguiente:
  
  ```c

  void set_pwm(int pin, int duty_cycle){
    ledcWrite(pin, duty_cycle);
  }

  void set_servo(int pin, int position){
    int duty_cycle = (position * 100) / 180;
    set_pwm(pin, duty_cycle);
  }
  
  void set_servos(int position1, int position2){
    set_servo(PIN1, position1);
    set_servo(PIN2, position2);
  }
  
  void set_servos_from_string(char * string){
    int position1 = 0;
    int position2 = 0;
    sscanf(string, "%d %d", &position1, &position2);
    set_servos(position1, position2);
  }

  
  
  void callback(const std_msgs::msg::String & msg){
    set_servos_from_string(msg.data.c_str());
  }

  ```
