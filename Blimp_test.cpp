//To compile on Mac: Open up terminal and nagivate to director where this file is stored using cd
//Once you are there type the following command: g++ --std=c++11 Blimp_test.cpp -o Blimp 
//To run: type ./Blimp

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
    if(in_min_val >= in_max_val) in_min_val = in_max_val - 1;
    if(in_max_val <= in_min_val) in_max_val = in_min_val + 1;
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
    if(diff_thrust_val < offset && -1*diff_thrust_val > -1*offset) {
        thrust_l = thrust_val;
        thrust_r = thrust_val;
    } 
    //Stick is to the left
    else if (diff_thrust_val < -1*offset) {
        thrust_r = thrust_val;
        thrust_l = thrust_val * (100.0 -diff_thrust_val)/100.0;
    } 
    //Stick is to the right
    else {
        thrust_l = thrust_val;
        thrust_r = thrust_val * (100.0 - diff_thrust_val)/100.0;
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

//Can change this code-------------------------------------------------------
//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
//Test values for channel values. Format: channel 1 - min value, channel 1 - max value, 
//channel 2 - min value, channel 2 - max value, etc. 
const int val_arr[8] = {1000, 2000, 1100, 1900, 800, 2100, 910, 2200};

//Test cases for code to test the scaling each channel. Will not be in final Arduino program
//This will always be called to test any code changes made to the scaling function aka
//Channel_convert 
void run_test_cases() {
    Channel * channel_arr[4];
    //Do not delete this line
    initialize_channels(channel_arr, 4, val_arr);
    cout << "Begin test cases" << endl;
    //Test cases for channel 1
    int val = val_arr[0];
    int converted = Channel_convert(channel_arr[0], val);
    assert(converted == 1000);
    cout << "Channel 1 test 1 passed!" << endl;
    val = val_arr[1];
    converted = Channel_convert(channel_arr[0], val);
    assert(converted == 2000);
    cout << "Channel 1 test 2 passed!" << endl;
    val = (val_arr[1]+val_arr[0])/2;
    converted = Channel_convert(channel_arr[0], val);
    assert(converted == 1500);
    cout << "Channel 1 test 3 passed!" << endl;

    //Test cases for channel 2
    val = val_arr[2];
    converted = Channel_convert(channel_arr[1], val);
    assert(converted == 1000);
    cout << "Channel 2 test 1 passed!" << endl;
    val = val_arr[3];
    converted = Channel_convert(channel_arr[1], val);
    assert(converted == 2000);
    cout << "Channel 2 test 2 passed!" << endl;
    val = (val_arr[3]+val_arr[2])/2;
    converted = Channel_convert(channel_arr[1], val);
    assert(converted == 1500);
    cout << "Channel 2 test 3 passed!" << endl;

    //Test cases for channel 3
    val = val_arr[4];
    converted = Channel_convert(channel_arr[2], val);
    assert(converted == 0);
    cout << "Channel 3 test 1 passed!" << endl;
    val = val_arr[5];
    converted = Channel_convert(channel_arr[2], val);
    assert(converted == 255);
    cout << "Channel 3 test 2 passed!" << endl;
    val = (val_arr[5]+val_arr[4])/2;
    converted = Channel_convert(channel_arr[2], val);
    assert(converted == 127 || converted == 128);
    cout << "Channel 3 test 3 passed!" << endl;

    //Test cases for channel 4
    val = val_arr[6];
    converted = Channel_convert(channel_arr[3], val);
    assert(converted == -100);
    cout << "Channel 4 test 1 passed!" << endl;
    int middle = (val_arr[6] + val_arr[7])/2;
    val = (val_arr[6]+middle)/2;
    converted = Channel_convert(channel_arr[3], val);
    assert(converted == -50);
    cout << "Channel 4 test 2 passed!" << endl;
    val = middle;
    converted = Channel_convert(channel_arr[3], val);
    assert(converted == 0);
    cout << "Channel 4 test 3 passed!" << endl;
    val = (middle + val_arr[7])/2;
    converted = Channel_convert(channel_arr[3], val);
    assert(converted == 50);
    cout << "Channel 4 test 4 passed!" << endl;
    val = val_arr[7];
    converted = Channel_convert(channel_arr[3], val);
    assert(converted == 100);
    cout << "Channel 4 test 5 passed!" << endl;
    cout << "End test cases" << endl;
    cout << endl;

    //Test cases for calculate_trust
    /*
    int offset = 10;
    int thrust_val = 100;
    */
    //Do not delete this line
    destroy_channels(channel_arr, 4);
}

int main() {
    run_test_cases();
    //Array to store channels
    Channel * channel_arr[4];
    //Do NOT delete this line
    initialize_channels(channel_arr, 4, val_arr);
    string str = "";
    /*
    do {
        int ch1, ch2, ch3, ch4
        cout << "Provide an input for channel 1 ";
        cin >> ch1;
        cout << "Provide an input for channel 2: ";
        cin >> ch2;
        cout << "Provide an input for channel 3: ";
        cin >> ch3;
        cout << "Provide an input for channel 4: ";
        cin >> ch4;
        int servo_1_output = Channel_convert(channel_arr[0], ch1);
        int servo_2_output = Channel_convert(channel_arr[1], ch2);
        cout << "Continue providing input: Y/N" << endl;
        cin >> str;
    } while (str != "N");
    */
    //Do NOT delete this line or place code after this line
    destroy_channels(channel_arr, 4);
}

