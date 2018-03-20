/*
#include<math.h>
#include<Servo.h>
//^^^^^^^^^^^^^^^^^^
//Do NOT delete-----

//README: This program allows you to see how the input values from the transmitters will be mapped to the
//output values to the servos and motors. Channels 1 and 2 are mapped to the 2 servos. Channel 3 controls
//thrust and channel 4 controls differential thrust such that moving the stick to the left decreases the
//thrust of the left motor and vice versa.
//Servo: full range of rotation occurs from 1000-2000
//Motor: Idle: 0, Full power: 255
//Currently this program is memory safe. It is very important that this program not have any dangling pointers
//or memory leaks. If you edit the sections using the "new" or "delete" keywords, please ensure that you 
//do not create any errors.
 
//Represents an input channel from the controller
//Each channel contains a a minimum input value,
//a maximum input value, and an identifying
//channel number
struct Channel {
    double min_val;
    double max_val;
    int channel_num;
};

//REQUIRES: channel points to a channel
//          0 <= in_min_val < in_max_val
//          in_max_val > in_max_val
//          1 <= in_channel_num <= 4
//MODIFIES: *channel
//EFFECTS:  Initializes image with the given minimum value, maximum value, and channel number.
//Note: Do NOT use assert statements. Assert statements can stop code from executing, which is bad
void Channel_init(Channel *channel, int in_min_val, int in_max_val, int in_channel_num) {
    //Enforcing requires clause without use of assert statement
    if(in_min_val < 0) in_min_val = 0;
    if(in_min_val >= in_max_val) in_min_val = in_max_val;
    if(in_max_val <= in_min_val) in_max_val = in_min_val;
    channel->min_val = (double) in_min_val;
    channel->max_val = (double) in_max_val;
    channel->channel_num = in_channel_num;
}

//REQUIRES: channel points to a channel
//          channel's minimum <= value <= channel's maximum value
//EFFECTS:  Scales the input value to be outputed to the output channel (servo/motor) controlled by the
//          given input channel
//Note: Do NOT use assert statements here
int Channel_convert(const Channel *channel, int value) {
    //Enforcing requires clause without use of assert statement
    if(value < channel->min_val) value = channel->min_val;
    if(value > channel->max_val) value = channel->max_val;
    double range = channel->max_val - channel->min_val;
    //Control surfaces
    if(channel->channel_num == 1 || channel->channel_num == 2) {
        double scaled = value - channel->min_val;
        double frac = scaled/range;
        double result = frac * 1000;
        int converted = (int) (result + 1000);
        return converted;
    } 
    //Thrust
    else if(channel->channel_num == 3) {
        double scaled = value - channel->min_val;
        double frac = scaled/range;
        int converted = (int) (frac * 255);
        return converted;
    }
    //Differential Thrust 
    else {
        double middle = (channel->max_val+channel->min_val)/2;
        if(value == middle)
            return 0;
        if(value < middle) {
            double scaled = value - channel->min_val;
            range = middle - channel->min_val;
            double frac = scaled/range;
            int converted = (int) round(-100 + frac * 100);
            return converted;
        } else {
            double scaled = value - middle;
            range = channel->max_val - middle;
            double frac = scaled/range;
            int converted = (int) round(frac *100);
            return converted;
        }
    }
}

//REQUIRES: 0 th 
//          -100 <= diff_thurst_val <= 100
//MODIFIES: thrust_l and thrust_r
//EFFECTS:  Calculates the thrust of each engine based on the values read in from channels 3 and 4.
//          If diff_thrust_val < 0 then then left engine will produce less thrust than the right engine
//          If diff_thrust_val > 0 then the right engine will produce less thrust than the left engine
//          If diff_thrust_val = 0, then the right and left engines will produce the same amount of thrust
void calculate_thrust(int thrust_val, int diff_thrust_val, int offset, int & thrust_l, int & thrust_r) {
    if(diff_thrust_val < offset && diff_thrust_val > -1*offset) {
        thrust_l = thrust_val;
        thrust_r = thrust_val;
    } 
    //Stick is to the left
    else if (diff_thrust_val < -1*offset) {
        thrust_r = thrust_val;
        double range = 100.0  - offset;
        double offset_thrust_val = -1*diff_thrust_val - offset;
        thrust_l = thrust_val * (range - offset_thrust_val)/range;
    } 
    //Stick is to the right
    else {
        thrust_l = thrust_val;
        double range = 100.0 - offset;
        double offset_thrust_val = diff_thrust_val - offset;
        thrust_r = thrust_val * (range - offset_thrust_val)/range;
    }
}

//USES DYNAMIC MEMORY - BE CAREUFL CHANGING (test with valgrind after changing)
//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
//REQUIRES: length = 4
//          length of val_arr be 8
//EFFECTS: Initializes all 4 input channels. Serves as a pseudo-constructor for the program
//Note: Uses new keyword length times, so delete keyword should be called length times somewhere else
void initialize_channels(Channel* channel_arr[], int length, const int val_arr[]) {
    int j = 0;
    for(int i = 0; i < length; ++i) {
        int min_val = val_arr[j++];
        int max_val = val_arr[j++];
        Channel *channel = new Channel;
        Channel_init(channel, min_val, max_val, i + 1);
        channel_arr[i] = channel;
    }
}

//REQUIRES: length = 4
//EFFECTS:  Deletes all input channels in channel_arr[], freeing up all dynamic memory
//          Acts as a pseudo destructor for the program
//Notes: Calls the delete keyword length times
void destroy_channels(Channel *channel_arr[], int length) {
    for(int i = 0; i < length; ++i) {
        Channel *channel = channel_arr[i];
        delete channel;
        channel = nullptr;
    }
}
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//USES DYNAMIC MEMORY - BE CAREFUL CHANGING (test with valgrind after changing)

//Begin reading input and outputing to servos and motors-------------------------
//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

//Can safely edit code below here-----------------------------------------------
//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

//Minimum and maximum values for each channel.  Format: channel 1 - min value, channel 1 - max value, 
//channel 2 - min value, channel 2 - max value, etc. 
const int val_arr[8] = {937, 2063, 871, 2088, 876, 2093, 912, 2058};

//Array to store the channels 
Channel * channel_arr[4];

//Necessary Arduino board values (tehcnically this is bad coding practice
//but it will save memory I think)
//Do not access these variables above this line-------------------------
const int ch1Pin = 8; //Channel 1 input
const int ch2Pin = 7; //Channel 2 input
const int ch3Pin = A1; //Channel 3 input
const int ch4Pin = A3; //Channel 4 input
const int servo_pin_1 = 3; 
const int servo_pin_2 = 2;
const int motorboard_pin_1 = 5; //Left motor
const int motorboard_pin_2 = 6; //Right motor
const int offset = 10;
int servo_value_1 = 90;
int servo_value_2 = 90;
int ch1Val = 0;
int ch2Val = 0;
int ch3Val = 0;
int ch4Val = 0;
int thrust_val = 0;
int diff_thrust_val = 0;
int thrust_l = 0;
int thrust_r = 0;
Servo servo_1, servo_2;

void setup() {
  pinMode(ch1Pin, INPUT);
  pinMode(ch2Pin, INPUT);
  pinMode(ch3Pin, INPUT);
  pinMode(ch4Pin, INPUT);
  pinMode(servo_pin_1, OUTPUT);
  pinMode(servo_pin_2, OUTPUT);
  servo_1.attach(servo_pin_1);
  servo_1.write(servo_value_1);
  servo_2.attach(servo_pin_2);
  servo_2.write(servo_pin_2);
  pinMode(motorboard_pin_1, OUTPUT);
  pinMode(motorboard_pin_2, OUTPUT);
  //Strictly speaking, this is bad programming practice
  //But since we are only allocating memory once and it will
  //be cleaned up in the end and deallocated takes time,
  //this is OK (in this context only). If you are editing
  //just be aware that this memory will exist for the duration
  //of the entire program
  initialize_channels(channel_arr, 4, val_arr);
  //Serial.begin(9600);
}

void loop() {
  //Input
  ch1Val = pulseIn(ch1Pin, HIGH);
  ch2Val = pulseIn(ch2Pin, HIGH);
  ch3Val = pulseIn(ch3Pin, HIGH);
  ch4Val = pulseIn(ch4Pin, HIGH);
  //Output to servos
  servo_value_1 = Channel_convert(channel_arr[0], ch1Val);
  servo_value_2 = Channel_convert(channel_arr[1], ch2Val);
  servo_1.writeMicroseconds(servo_value_1);
  servo_2.writeMicroseconds(servo_value_2);
  //Output to motors
  //Serial.println(ch1Val);
  //Serial.println(ch2Val);
  //Serial.println(ch3Val);
  //Serial.println(ch4Val);
  //Serial.println();
  thrust_val = Channel_convert(channel_arr[2], ch3Val);
  diff_thrust_val = Channel_convert(channel_arr[3], ch4Val);
  calculate_thrust(thrust_val, diff_thrust_val, offset, thrust_l, thrust_r);
  //Serial.println(thrust_l);
  //Serial.println(thrust_r);
  analogWrite(motorboard_pin_1, thrust_l);
  analogWrite(motorboard_pin_2, thrust_r);
  //delay(750);
}
*/