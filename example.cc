/*
    Copyright [2016] <Peter Jung> 
    Released under GPL License via Github
    https://github.com/kongzii/
*/

#include <iostream>
#include "xiaomi-yi.h"

int main() {
    using std::cout; using std::cerr; using std::endl; using std::boolalpha;
    using kongzii::xiaomi_yi::XiaomiYi;

    XiaomiYi yi;  // Create XiaomiYi object with default values
                  // (IP: 192.168.42.1, PORT: 7878, TIMEOUT: 5 seconds)

    if (!(yi.start())) {  // Will return false if connection to Yi fails
        cerr << "Something went wrong." << endl;
    }

    yi.shoot();  // Take photo
    sleep(5);  // Give it some time..

    yi.record();  // Start recording
    sleep(15);  // Record for 15 seconds
    yi.record();  // Stop
    sleep(5);

    // You can send your own commands, too, some you will find in commands.txt
    std::string custom = "{\"msg_id\":2,\"token\": "
        + yi.status().token  // token from status is std::string, 
                             // so you can easily join them with commands
        + ", \"type\":\"video_resolution\", \"param\":\"1920x1080 60P 16:9\"}"; 
    yi.send(custom);  // custom have to be std::string with proper format, as above
    sleep(5);

    yi.stop();  // Disconnect from Yi
    return 0;
}
