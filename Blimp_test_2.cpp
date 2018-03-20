//To compile on Mac: Open up terminal and nagivate to director where this file is stored using cd
//Once you are there type the following command: g++ --std=c++11 Blimp_test_2.cpp -o Blimp2 
//To run: type ./Blimp2

//README: This program allows you to see how the input values from the transmitters will be mapped to the
//output values to the servos and motors. Channels 1 and 2 are mapped to the 2 servos. Channel 3 controls
//thrust and channel 4 controls differential thrust such that moving the stick to the left decreases the
//thrust of the left motor and vice versa.
//Servo: full range of rotation occurs from 1000-2000
//Motor: Idle: 0, Full power: 255
//Currently this program is memory safe. It is very important that this program not have any dangling pointers
//or memory leaks. If you use dynamic memory, please be careful

//Do not delete----
//vvvvvvvvvvvvvvvvvv
#include <string>
#include <iostream>
#include <cassert>
#include <math.h>
using namespace std;
//^^^^^^^^^^^^^^^^^^
//Do not delete-----

//Represents an input channel from the controller
//Each channel contains a a minimum input value,
//a maximum input value, and an identifying
//channel number
struct Channel {
    //Trim down, stick down
    double min_val;
    //Trim middle, stick down
    double min_stick_val;
    //Trim middle, stick up
    double max_stick_val;
    //Trim up, stick up
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
void Channel_init(Channel *channel, int in_min_val, int in_min_stick_val, int in_max_stick_val, int in_max_val, int in_channel_num) {
    //Enforcing requires clause without use of assert statement
    //DO NOT modify
    if(in_min_val < 0) in_min_val = 0;
    if(in_min_val >= in_max_val) in_min_val = in_max_val - 1;
    if(in_max_val <= in_min_val) in_max_val = in_min_val + 1;
    channel->min_val = (double) in_min_val;
    channel->min_stick_val = (double) in_min_stick_val;
    channel->max_stick_val = (double) in_max_stick_val;
    channel->max_val = (double) in_max_val;
    channel->channel_num = in_channel_num;
}

//REQUIRES: channel points to a valid hannel
//          channel's minimum <= value <= channel's maximum value
//EFFECTS:  Scales the input value to be outputed to the output channel (servo/motor) controlled by the
//          given input channel
//Note: Do NOT use assert statements. Assert statements can stopc code from executing, which is bad
int Channel_convert(const Channel *channel, int value) {
    //Enforcing requires clause without use of assert statement
    //DO NOT modify
    if(value < channel->min_val) value = channel->min_val;
    if(value > channel->max_val) value = channel->max_val;
    double range = channel->max_val - channel->min_val;
    //Control surfaces
    if(channel->channel_num == 1 || channel->channel_num == 2) {
        if(value < channel->min_stick_val) {

        } else if (value >= channel->min_stick_val && value <= channel->max_stick_val) {

        } else {

        }
        double scaled = value - channel->min_val;
        double frac = scaled/range;
        double result = frac * 1000;
        int converted = (int) (result + 1000);
        return converted;
    } 
    //Thrust
    else if(channel->channel_num == 3) {
        if(value < channel->min_stick_val) {
            
        } else if (value >= channel->min_stick_val && value <= channel->max_stick_val) {

        } else {
            
        }
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

//Helper function for scaling input to output
//REQUIRES: *channel points to a valid channel
//          channel's minimum <= value <= channel's maximum value
//EFFECTS:  Scales the input value to be outputed to the output channel (servo/motor) controlled by the
//          given input channel as if stick and trim had same effect
//Note: This is intended to be called in Channel_convert only (but the requires clause will still be enforced)
//Note: Do NOT use assert statements. Assert statements can stopc code from executing, which is bad.
int Channel_conver_no_trim(const Channel *channel, int value) {
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

//REQUIRES: thrust_val >= 0 
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

//DO NOT EDIT WITHOUT NOTIFYING TEAM
//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
//REQUIRES: length = 4
//          length of val_arr be 16
//EFFECTS: Initializes all 4 input channels. Serves as a pseudo-constructor for the program
//Note: Does not use the new keyword. This technically doesn't follow the program style being 
//used (C-style ADT), but the Arduino is a memory limited environment so less memory > bad style
void initialize_channels(Channel * channel_arr, int length, const int * val_arr) {
    int vi = 0;
    for(int i = 0; i < length; ++i) {
        Channel c;
        int min_val = val_arr[vi++];
        int min_stick_val = val_arr[vi++];
        int max_stick_val = val_arr[vi++];
        int max_val = val_arr[vi++];
        Channel_init(&c, min_val, min_stick_val, max_stick_val, max_val, i+1);
        channel_arr[i] = c;
    }
}
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//DO NOT EDIT WITHOUT NOTIFYING TEAM

//Begin reading input and outputing to servos and motors-------------------------
//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

//Can change this code-------------------------------------------------------
//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

//Array of channel values in microseconds. Format: channel 1 - min value, channel 1 - min stick value, 
//channel 1 - max stick value, channel 1 - max value, etc.
const int val_arr[16] = {1000, 1100, 1900, 2000, 1100, 1300, 1700, 1900, 800, 900, 2000, 2100, 910, 1010, 2100, 2200};

//Code below here should NOT be transferred to the Arduino sketch-------------------------
//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

//Test cases for code to test the scaling each channel. Will not be in final Arduino program
//This will always be called to test any code changes made to the scaling function aka
//Channel_convert 

int main() {
    //run_test_cases();
    //Array to store channels
    Channel channel_arr[4];
    //Do NOT delete this line
    initialize_channels(channel_arr, 4, val_arr);
    string str = "";
    Channel c = channel_arr[0];
    cout << c.min_val << endl;
    cout << c.max_val << endl;
    int servo_1_output = Channel_convert(&(channel_arr[0]), 0);
    int servo_2_output = Channel_convert(&(channel_arr[1]), 0);
    cout << "Servo 1 output: " << servo_1_output << endl;
    cout << "Servo 2 output: " << servo_2_output << endl;
    int thrust_val = Channel_convert(&(channel_arr[2]), 0);
    int diff_thrust_val = Channel_convert(&(channel_arr[3]), 0);
    int thrust_l = 0;
    int thrust_r = 0;
    calculate_thrust(thrust_val, diff_thrust_val, 10, thrust_l, thrust_r);
    cout << "Left motor thrust: " << thrust_l << endl;
    cout << "Right motor thrust: " << thrust_r << endl;
    /*
    do {
        int ch1, ch2, ch3, ch4;
        cout << "Provide an input for channel 1 ";
        cin >> ch1;
        cout << "Provide an input for channel 2: ";
        cin >> ch2;
        cout << "Provide an input for channel 3: ";
        cin >> ch3;
        cout << "Provide an input for channel 4: ";
        cin >> ch4;
        int servo_1_output = Channel_convert(&(channel_arr[0]), ch1);
        int servo_2_output = Channel_convert(&(channel_arr[1]), ch2);
        cout << "Servo 1 output: " << servo_1_output << endl;
        cout << "Servo 2 output: " << servo_2_output << endl;
        int thrust_val = Channel_convert(&(channel_arr[2]), ch3);
        int diff_thrust_val = Channel_convert(&(channel_arr[3]), ch4);
        int thrust_l = 0;
        int thrust_r = 0;
        calculate_thrust(thrust_val, diff_thrust_val, 10, thrust_l, thrust_r);
        cout << "Left motor thrust: " << thrust_l << endl;
        cout << "Right motor thrust: " << thrust_r << endl;
        cout << "Continue providing input: Y/N" << endl;
        cin >> str;
    } while (str == "Y");
    */
}

