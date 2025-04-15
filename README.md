# USB adapter for Sega Mega Drive gamepads  

Article on habr.com: https://habr.com/p/900300/  

`SegaGamepad` library: https://registry.platformio.org/libraries/ivoryrubble/SegaGamepad   

Adapter features:  
- Supports two gamepads
- No gamepads modification needed
- Supports both 3-buttons and 6-buttons gamepads
- Recognized by PC as USB keyboard or two USB gamepads (working mode can be switched any time)  
  
![](photos/adapter_connected_to_PC.jpg)  

Video demo:  
https://youtu.be/ejnmY7F8hcU   
https://youtu.be/ySdsnPrBBsU  
https://vkvideo.ru/video-228936534_456239017   
https://vkvideo.ru/video-228936534_456239020  

# SegaGamepad library 

Yet another library to read Sega Mega Drive (aka Sega Genesis) gamepad.  
Supports both three and six buttons controllers.  

## Sources  

Gamepad protocol description:  
http://web.archive.org/web/20171229105419/http://www.cs.cmu.edu/~chuck/infopg/segasix.txt  
https://github.com/jonthysell/SegaController/wiki/How-To-Read-Sega-Controllers

Gamepad protocol reverse engineering:  
https://www.raspberryfield.life/2019/03/25/sega-mega-drive-genesis-6-button-xyz-controller/#SMD6-protocol-overview  

Another good library:  
https://github.com/jonthysell/SegaController  

## Pinout  
Sega Mega Drive gamepad connector pinout, female DB9 plug:  

````
   +----------- pin5, +5V
   | +--------- pin4, set by gamepad            
   | | +------- pin3, set by gamepad  
   | | | +----- pin2, set by gamepad   
   | | | | +--- pin1, set by gamepad
   | | | | |            
+-------------+             
 \ O O O O O /                
  \ O O O O /                  
   +-------+                 
    | | | |
    | | | +---- pin6, set by gamepad
    | | +------ pin7, Select pin, set to 0 or 1 by microcontroller        
    | +-------- pin8, GND
    +---------- pin9, set by gamepad         
````

## Connection                                

Connect `pin8` and `pin5` to GND and +5V.   
Connect `pin1`, `pin2`, `pin3`, `pin4`, `pin6`, `pin7`, `pin9` to microcontroller I/O pins.            

## Usage  

Set correcponding `pin1`, `pin2`, `pin3`, `pin4`, `pin6`, `pin7`, `pin9` numbers in `SegaGamepad::SegaGamepad()` constructor.  
Optionally set custom `delayBeforeReadMicros` and `delayBeforeNextUpdateMicros` delays.  
Example:  
````C++
SegaGamepad segaGamepad(1, 2, 3, 4, 5, 6, 7);
````

Call `SegaGamepad::init()` method in setup() procedure to initialize microcontroller ports for gamepad.  
Example:  
````C++
segaGamepad.init(); 
```` 

Call `SegaGamepad::update()` method to read new values from controller.  
Then you can keys get values from `segaGamepad` object fields. True value means corresponding key is pressed.  
You also can check if controller is connected by reading `isConnected` flag.  
And you can check type of controller (three or six buttons) by reading `isSixBtns` flag.  
When controller is not connected then `isConnected` flag is set to `False`, all buttons values are set to `False`, is `isSixBtns` flag is set to `False`.  
Example:  
````C++
segaGamepad.update();
Serial.println(segaGamepad.isConnected);
Serial.println(segaGamepad.btnUp);
Serial.println(segaGamepad.btnA);
Serial.println(segaGamepad.btnStart);
````      

# Some photos 

![](photos/adapter_connected_1.jpg)   

![](photos/adapter_connected_2.jpg)   

![](photos/adapter_assembled_and_connected.jpg) 
