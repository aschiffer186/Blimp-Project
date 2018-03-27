#include "unit_test_framework.h"
#include "Blimp_test_2.h"

/*
const Channel ch1 = {(double) val_arr[0], (double) val_arr[1], (double) val_arr[2], (double) val_arr[3]};
const Channel ch2 = {(double) val_arr[4], (double) val_arr[5], (double) val_arr[6], (double) val_arr[7]};
const Channel ch3 = {(double) val_arr[8], (double) val_arr[9], (double) val_arr[10], (double) val_arr[11]};
const Channel ch4 = {(double) val_arr[12], (double) val_arr[13], (double) val_arr[14], (double) val_arr[15]};
*/
Channel channel_arr[4];

TEST(test_initialize_channels) {
    initialize_channels(channel_arr, 4, val_arr);
    ASSERT_EQUAL(1, channel_arr[0].channel_num);
    ASSERT_EQUAL(2, channel_arr[1].channel_num);
    ASSERT_EQUAL(3, channel_arr[2].channel_num);
    ASSERT_EQUAL(4, channel_arr[3].channel_num);
    ASSERT_EQUAL(2063, channel_arr[0].max_val);
    ASSERT_EQUAL(2092, channel_arr[1].max_val);
    ASSERT_EQUAL(2092, channel_arr[2].max_val);
    ASSERT_EQUAL(2062, channel_arr[3].max_val);
}

TEST(test_elevation_controls_1) {

}

TEST(test_elevation_controls_2) {

}

TEST(test_elevation_controls_3) {

}

TEST(test_rudder_controls_1) {

}

TEST(test_rudder_controls_2) {

}

TEST(test_rudder_controls_3) {

}

TEST(test_thrust_1) {

}

TEST(test_thrust_2) {

}

TEST(test_thrust_3) {

}

TEST(test_differential_thrust_1) {

}

TEST(test_differential_thrust_2) {

}

TEST(test_error_control_1) {

}

TEST(test_error_control_2) {

}

TEST_MAIN()