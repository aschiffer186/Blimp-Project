//There is a set of test cases set up to test changes, see Blimp_tests.cpp

//README: This program allows you to see how the input values from the transmitters will be mapped to the
//output values to the servos and motors. Channels 1 and 2 are mapped to the 2 servos. Channel 3 controls
//thrust and channel 4 controls differential thrust such that moving the stick to the left decreases the
//thrust of the left motor and vice versa.
//Servo: full range of rotation occurs from 1000-2000
//Motor: Idle: 0, Full power: 255
//Currently this program is memory safe. It is very important that this program not have any dangling pointers
//or memory leaks. If you use dynamic memory, please be careful. 
//Note: strictly sepaking .h files don't have implementations but since this is just testing it's ok

//Do not delete----
//vvvvvvvvvvvvvvvvvv
#include <math.h>
//^^^^^^^^^^^^^^^^^^
//Do not delete-----
//Code below this comment goes into the Arudino sketch

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
    //Identifying number
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

//Helper function for scaling input to output
//REQUIRES: *channel points to a valid channel
//          channel's minimum <= value <= channel's maximum value
//EFFECTS:  Scales the input value to be outputed to the output channel (servo/motor) controlled by the
//          given input channel as if stick and trim had same effect
//Note: This is intended to be called in Channel_convert only (but the requires clause will still be enforced)
//Note: Do NOT use assert statements. Assert statements can stopc code from executing, which is bad.
int Channel_convert_no_trim(const Channel *channel, int value) {
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
    int channel_num = channel->channel_num;
    double mi = channel->min_val;
    double Mi = channel->max_val;
    double msi = channel->min_stick_val;
    double Msi = channel->max_stick_val;
    double msv = Channel_convert_no_trim(channel, msi);
    double Msv = Channel_convert_no_trim(channel, Msi);
    double range = 1.1*(Msv-msv);
    double delta_range = range - (Msv-msv);
    msv = msv - delta_range/2;
    Msv = Msv + delta_range/2;
    //Control surfaces
    //Works for both thrust and control surfaces
    if(channel_num <= 3) {
        if(value < channel->min_stick_val) {
            double scaled = value - mi;
            double frac = scaled/(msi-mi);
            return (int) (msv*frac + 1000);
        } else if (value >= msi && value <= Msi) {
            double scaled = value - msi;
            double frac = scaled/(Msi-msi);
            return (int) (range*frac + msv);
        } else {
            double scaled = value - Msi;
            //double frac = scaled/(Mi - Msi);
            return (int) (Msv * scaled + Msv);
        }
    } 
    //Thrust
    /*
    else if(channel->channel_num == 3) {
        if(value < channel->min_stick_val) {
            double scaled = value - mi;
            double frac = scaled/(msi-mi);
            return (int) (msv*frac + 0);
        } else if (value >= channel->min_stick_val && value <= channel->max_stick_val) {
            double scaled = value - msi;
            double frac = scaled/(Msi-msi);
            return (int) (range*frac + msv);
        } else {
            
        }

    }
    */
    //Differential Thrust 
    else {
        double middle = (Mi+mi)/2;
        range = Mi-mi;
        if(value == middle)
            return 0;
        if(value < middle) {
            double scaled = value - Mi;
            range = middle - mi;
            double frac = scaled/range;
            int converted = (int) round(-100 + frac * 100);
            return converted;
        } else {
            double scaled = value - middle;
            range = Mi - middle;
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

//Array of channel values in microseconds. Format: channel 1 - min value, channel 1 - min stick value, 
//channel 1 - max stick value, channel 1 - max value, etc.
const int val_arr[16] = {936, 1107, 1888, 2063, 871, 1077, 1933, 2092, 877, 1048, 1912, 2092, 912, 1067, 1867, 2062};

/**
 * //REQUIRES: 1000 <= new_pos <= 2000
 * //          servo is a valid servo
 * //EFFECTS: Adjusts the position of the servo, but slows down the rotation 
 * //of the servo that it is not so violent
 * void servo_adjust(int new_pos, int delay, const Servo &servo) {
 *      int old_val = servo.read();
 *      double frac = old_val/180;
 *      old_val = (int) frac * 2000;
 *      if(old_val <=  new_pos) {
 *          for(int i = old_pos; i <= new_pos; i += 10) {
 *              servo.writeMicroseconds(i);
 *              delay(5);
 *          }
 *      } else {
 *          for(int i = old_pos; i >= new_pos; i -= 10) {
 *              servo.writeMicroseconds(i);
 *              delay(5);
 *          }
 *      }
 * }
 * 
 */

