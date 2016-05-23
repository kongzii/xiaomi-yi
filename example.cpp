#include <iostream>
#include "xiaomi-yi.h"

int main() {
	using std::cout; using std::cerr; using std::endl; using std::boolalpha;
    using kongzii::xiaomi_yi::XiaomiYi;
    
    XiaomiYi yi; // Create XiaomiYi object with default values (IP: 192.168.42.1, PORT: 7878, TIMEOUT: 5 seconds)

    if(!(yi.start())) { // Will return false if connection to Yi fails
    	cerr << "Something went wrong." << endl;
    }

    yi.shoot(); // Take photo

    yi.stop(); // Disconnect from Yi

    return 0;
}