
#include <PS4Controller.h>

void setup() {

	Serial.begin(9600);
	//Serial.print("Starting...");

	PS4.begin();
}

unsigned long currentMillis, previousMillis, period = 10;
uint8_t arr[6] = { 3, 0, 0, 0, 0, 0 };

void loop() {

	currentMillis = millis();
	if (currentMillis >= previousMillis && currentMillis - previousMillis < period)
		return;
	previousMillis = currentMillis;

	int left = PS4.LStickY() * 20;
	int right = PS4.RStickY() * 20;

	arr[1] = (0xff00 & left) >> 8;
	arr[2] = 0xff & left;

	arr[3] = (0xff00 & right) >> 8;
	arr[4] = 0xff & right;

	//CheckSum
	arr[5] = ~(arr[0] + arr[1] + arr[2] + arr[3] + arr[4]);

	Serial.write(arr, 6);
}