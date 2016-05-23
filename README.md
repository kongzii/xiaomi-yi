# xiaomi-yi

Library to control your Xiaomi Yi from C++.

## Building

The first step is to make sure you have all dependencies installed:

```
sudo apt-get install g++ build-essential
```

Then compile.

```
g++ example.cpp -o yi.out -std=c++14
```
This will produce yi.out executable which you can run. 
## Sample usage

```
#include <iostream>
#include "xiaomi-yi.h"

int main() {
	using std::cout; using std::cerr; using std::endl; using std::boolalpha;
    using kongzii::xiaomi_yi::XiaomiYi;
    
    XiaomiYi yi; // Create XiaomiYi object with default values
    		 	 //(IP: 192.168.42.1, PORT: 7878, TIMEOUT: 5 seconds)

    if(!(yi.start())) { // Will return false if connection to Yi fails
    	cerr << "Something went wrong." << endl;
    }

    yi.shoot(); // Take photo
    sleep(5); // Give it some time..

    yi.record(); // Start recording
    sleep(15); // Record for 15 seconds
    yi.record(); // Stop
    sleep(5);

    // You can send your own commands, too, some you will find in commands.txt
    const std::string custom = "{\"msg_id\":2,\"token\": "
    	 + yi.status().token // token from status is std::string, so you can easily join them with commands
    	 + ", \"type\":\"video_resolution\", \"param\":\"1920x1080 60P 16:9\"}"; 
    yi.send(custom); // custom have to be std::string with proper format, as above
    sleep(5);

    yi.stop(); // Disconnect from Yi
    return 0;
}
```

## Requirements

The only explicit requirement is a C++11 compatible compiler like gcc. Developed and tested on Ubuntu 16.04 with G++.  

## Notes

Default password for Yi´s Wi-Fi is `1234567890`.
Default connection values for my camera are:
```
IP: 192.168.42.1
PORT: 7878
```
But if for some reason yours are different you can easily override them:
```
using kongzii::xiaomi_yi::XiaomiYi;
    
XiaomiYi yi("192.168.42.1", 7878);
```

## Contributing

There are many commands for Yi, those are only basics for shooting and recording, so every contributor is welcomed! :)
