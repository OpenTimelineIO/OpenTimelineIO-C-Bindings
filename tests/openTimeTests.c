#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdlib.h>
#include <cmocka.h>
#include "epsilon.h"

#include <copentime/rationalTime.h>
#include <copentime/timeRange.h>
#include <copentime/timeTransform.h>

struct RationalTimeTestState {
    RationalTime rationalTime;
};

static int setupRationalTimeTests(void **state) {
    struct RationalTimeTestState *testState = malloc(sizeof(struct RationalTimeTestState));
    testState->rationalTime = RationalTime_create(48, 24);
    *state = testState;
    return 0;
}

static int teardownRationalTimeTests(void **state) {
    struct RationalTimeTestState *testState = *state;
    free(testState);
    return 0;
}

static void opentime_rational_time_invalid_time_test(void **state) {
    struct RationalTimeTestState *testState = *state;
    RationalTime rationalTime = testState->rationalTime;
    RationalTime invalidTime = RationalTime_create(48, -24);
    assert_true(RationalTime_is_invalid_time(invalidTime));
    assert_false(RationalTime_is_invalid_time(rationalTime));
}

static void opentime_rational_time_get_value_test(void **state) {
    struct RationalTimeTestState *testState = *state;
    RationalTime rationalTime = testState->rationalTime;
    assert_int_equal(RationalTime_value(rationalTime), 48);
}

static void opentime_rational_time_get_rate_test(void **state) {
    struct RationalTimeTestState *testState = *state;
    RationalTime rationalTime = testState->rationalTime;
    assert_double_equal(RationalTime_rate(rationalTime), 24, DBL_EPSILON);
}

static void opentime_rational_time_rescale_to_rate_test(void **state) {
    struct RationalTimeTestState *testState = *state;
    RationalTime rationalTime = testState->rationalTime;
    RationalTime rescaledTime = RationalTime_rescaled_to(rationalTime, 48);
    assert_double_equal(RationalTime_value(rescaledTime), 96, DBL_EPSILON);
    assert_double_equal(RationalTime_rate(rescaledTime), 48, DBL_EPSILON);
}

static void opentime_rational_time_rescale_to_rational_time_test(void **state) {
    struct RationalTimeTestState *testState = *state;
    RationalTime rationalTime = testState->rationalTime;
    RationalTime scaleTime = RationalTime_create(48, 48);
    RationalTime rescaledTime =
            RationalTime_rescaled_to_rational_time(rationalTime, scaleTime);
    assert_double_equal(RationalTime_value(rescaledTime), 96, DBL_EPSILON);
    assert_double_equal(RationalTime_rate(rescaledTime), 48, DBL_EPSILON);
}

static void opentime_rational_time_value_rescaled_to_rate_test(void **state) {
    struct RationalTimeTestState *testState = *state;
    RationalTime rationalTime = testState->rationalTime;
    assert_double_equal(RationalTime_value_rescaled_to_rate(rationalTime, 48), 96, DBL_EPSILON);
}

static void opentime_rational_time_value_rescaled_to_rational_time_test(void **state) {
    struct RationalTimeTestState *testState = *state;
    RationalTime rationalTime = testState->rationalTime;
    RationalTime scaleTime = RationalTime_create(48, 48);
    assert_double_equal(RationalTime_value_rescaled_to_rational_time(rationalTime, scaleTime), 96, DBL_EPSILON);
}

static void opentime_rational_time_almost_equal_test(void **state) {
    struct RationalTimeTestState *testState = *state;
    RationalTime rationalTime = testState->rationalTime;
    RationalTime otherTime = RationalTime_create(50, 24);
    assert_true(RationalTime_almost_equal(rationalTime, otherTime, 5));
}

static void opentime_rational_time_duration_from_start_end_time_test(void **state) {
    RationalTime startTime = RationalTime_create(0, 24);
    RationalTime endTime = RationalTime_create(24, 24);
    RationalTime result =
            RationalTime_duration_from_start_end_time(startTime, endTime);
    assert_true(RationalTime_equal(result, endTime));
}

static void opentime_rational_time_is_valid_timecode_test(void **state) {
    assert_true(RationalTime_is_valid_timecode_rate(23.97));
    assert_false(RationalTime_is_valid_timecode_rate(24.97));
}

static void opentime_rational_time_from_frames_test(void **state) {
    float fps[7] = {24, 30, 48, 60, 23.98f, 29.97f, 59.94f};
    for (int i = 0; i < 7; i++) {
        RationalTime t1 = RationalTime_create(101, fps[i]);
        RationalTime t2 = RationalTime_from_frames(101, fps[i]);
        assert_true(RationalTime_equal(t1, t2));
    }
}

static void opentime_rational_time_seconds_test(void **state) {
    int s1 = 1834;
    RationalTime t1 = RationalTime_from_seconds(s1);
    assert_double_equal(RationalTime_value(t1), 1834, DBL_EPSILON);
    assert_double_equal(RationalTime_rate(t1), 1, DBL_EPSILON);
    assert_double_equal(RationalTime_to_seconds(t1), s1, DBL_EPSILON);
    assert_double_equal(RationalTime_value(t1) / RationalTime_rate(t1), s1, DBL_EPSILON);

    double s2 = 248474.345;
    RationalTime t2 = RationalTime_from_seconds(s2);
    assert_double_equal(RationalTime_value(t2), s2, DBL_EPSILON);
    assert_double_equal(RationalTime_rate(t2), 1.0, DBL_EPSILON);
    assert_double_equal(RationalTime_to_seconds(t2), s2, DBL_EPSILON);
    assert_double_equal(RationalTime_value(t2) / RationalTime_rate(t2), s2, DBL_EPSILON);

    float v3 = 3459;
    float r3 = 24;
    float s3 = v3 / r3;
    RationalTime t3 = RationalTime_create(v3, r3);
    RationalTime t4 = RationalTime_from_seconds(s3);
    assert_double_equal(RationalTime_to_seconds(t3), s3, DBL_EPSILON);
    assert_double_equal(RationalTime_to_seconds(t4), s3, DBL_EPSILON);
}

static void opentime_rational_time_timecode24_test(void **state) {
    OpenTimeErrorStatus *errorStatus = OpenTimeErrorStatus_create();
    const char *timecode = "00:00:01:00";
    RationalTime t = RationalTime_create(24, 24);
    RationalTime fromTimeCode =
            RationalTime_from_timecode(timecode, 24, errorStatus);
    assert_true(RationalTime_equal(t, fromTimeCode));

    timecode = "00:01:00:00";
    t = RationalTime_create(24 * 60, 24);
    fromTimeCode = RationalTime_from_timecode(timecode, 24, errorStatus);
    assert_true(RationalTime_equal(t, fromTimeCode));

    timecode = "01:00:00:00";
    t = RationalTime_create(24 * 60 * 60, 24);
    fromTimeCode = RationalTime_from_timecode(timecode, 24, errorStatus);
    assert_true(RationalTime_equal(t, fromTimeCode));

    timecode = "24:00:00:00";
    t = RationalTime_create(24 * 60 * 60 * 24, 24);
    fromTimeCode = RationalTime_from_timecode(timecode, 24, errorStatus);
    assert_true(RationalTime_equal(t, fromTimeCode));

    timecode = "23:59:59:23";
    t = RationalTime_create(24 * 60 * 60 * 24 - 1, 24);
    fromTimeCode = RationalTime_from_timecode(timecode, 24, errorStatus);
    assert_true(RationalTime_equal(t, fromTimeCode));
}

static void opentime_rational_time_timecode23967fps_test(void **state) {
    OpenTimeErrorStatus *errorStatus = OpenTimeErrorStatus_create();
    const char *timecode = "00:00:01:00";
    RationalTime t = RationalTime_create(24, 23.976);
    RationalTime fromTimeCode =
            RationalTime_from_timecode(timecode, 23.976, errorStatus);
    assert_true(RationalTime_equal(t, fromTimeCode));

    timecode = "00:01:00:00";
    t = RationalTime_create(24 * 60, 23.976);
    fromTimeCode = RationalTime_from_timecode(timecode, 23.976, errorStatus);
    assert_true(RationalTime_equal(t, fromTimeCode));

    timecode = "01:00:00:00";
    t = RationalTime_create(24 * 60 * 60, 23.976);
    fromTimeCode = RationalTime_from_timecode(timecode, 23.976, errorStatus);
    assert_true(RationalTime_equal(t, fromTimeCode));

    timecode = "24:00:00:00";
    t = RationalTime_create(24 * 60 * 60 * 24, 23.976);
    fromTimeCode = RationalTime_from_timecode(timecode, 23.976, errorStatus);
    assert_true(RationalTime_equal(t, fromTimeCode));

    timecode = "23:59:59:23";
    t = RationalTime_create(24 * 60 * 60 * 24 - 1, 24000.0 / 1001.0);
    fromTimeCode =
            RationalTime_from_timecode(timecode, 24000.0 / 1001.0, errorStatus);
    assert_true(RationalTime_equal(t, fromTimeCode));
}

static void opentime_rational_time_timecodentsc2997fps_test(void **state) {
    OpenTimeErrorStatus *errorStatus = OpenTimeErrorStatus_create();
    double frames = 1084319;
    double rate_float = 30000.0 / 1001.0;
    RationalTime t = RationalTime_create(frames, rate_float);
    const char *dftc = RationalTime_to_timecode(
            t, rate_float, OpenTime_IsDropFrameRate_ForceYes, errorStatus);
    assert_string_equal(dftc, "10:03:00;05");

    const char *tc = RationalTime_to_timecode(
            t, rate_float, OpenTime_IsDropFrameRate_ForceNo, errorStatus);
    assert_string_equal(tc, "10:02:23:29");

    /* Detect DFTC from rate for backward compatibility with old versions */
    const char *tc_auto = RationalTime_to_timecode(
            t, rate_float, OpenTime_IsDropFrameRate_InferFromRate, errorStatus);
    assert_string_equal(tc_auto, "10:03:00;05");

    free((char *) dftc);
    free((char *) tc);
    free((char *) tc_auto);
    OpenTimeErrorStatus_destroy(errorStatus);
}

static void opentime_rational_time_timecode2997_test(void **state) {
    OpenTimeErrorStatus *errorStatus = OpenTimeErrorStatus_create();
    int ref_values_val[6] = {10789, 10790, 17981, 17982, 17983, 17984};
    const char *ref_values_tc[6] = {"00:05:59:19", "00:05:59:20",
                                    "00:09:59:11", "00:09:59:12",
                                    "00:09:59:13", "00:09:59:14"};
    const char *ref_values_dftc[6] = {"00:05:59;29", "00:06:00;02",
                                      "00:09:59;29", "00:10:00;00",
                                      "00:10:00;01", "00:10:00;02"};
    const char *to_dftc = "";
    const char *to_tc = "";
    const char *to_auto_tc = "";
    RationalTime t;
    RationalTime t1;
    RationalTime t2;
    for (int i = 0; i < 6; i++) {
        t = RationalTime_create(ref_values_val[i], 29.97);
        to_dftc = RationalTime_to_timecode(
                t, 29.97, OpenTime_IsDropFrameRate_ForceYes, errorStatus);
        to_tc = RationalTime_to_timecode(
                t, 29.97, OpenTime_IsDropFrameRate_ForceNo, errorStatus);
        to_auto_tc = RationalTime_to_timecode(
                t, 29.97, OpenTime_IsDropFrameRate_InferFromRate, errorStatus);

        /* 29.97 should auto-detect dftc for backward compatability */
        assert_string_equal(to_dftc, to_auto_tc);

        /* check calculated against reference */
        assert_string_equal(to_dftc, ref_values_dftc[i]);
        assert_string_equal(to_tc, ref_values_tc[i]);

        /* Check they convert back */
        t1 = RationalTime_from_timecode(ref_values_dftc[i], 29.97, errorStatus);
        assert_true(RationalTime_equal(t1, t));

        t2 = RationalTime_from_timecode(ref_values_tc[i], 29.97, errorStatus);
        assert_true(RationalTime_equal(t2, t));
    }

    free((char *) to_dftc);
    free((char *) to_tc);
    free((char *) to_auto_tc);
    OpenTimeErrorStatus_destroy(errorStatus);
}

static void opentime_rational_time_timestring24_test(void **state) {
    OpenTimeErrorStatus *errorStatus = OpenTimeErrorStatus_create();
    const char *time_string = "00:00:00.041667";
    RationalTime t = RationalTime_create(1.0, 24);
    RationalTime time_obj =
            RationalTime_from_time_string(time_string, 24, errorStatus);
    assert_true(RationalTime_almost_equal(t, time_obj, 0.001));
    assert_double_equal(RationalTime_rate(time_obj), 24, DBL_EPSILON);

    time_string = "00:00:01";
    t = RationalTime_create(24, 24);
    time_obj = RationalTime_from_time_string(time_string, 24, errorStatus);
    assert_true(RationalTime_almost_equal(t, time_obj, 0.001));

    time_string = "00:01:00";
    t = RationalTime_create(24 * 60, 24);
    time_obj = RationalTime_from_time_string(time_string, 24, errorStatus);
    assert_true(RationalTime_almost_equal(t, time_obj, 0.001));

    time_string = "01:00:00";
    t = RationalTime_create(24 * 60 * 60, 24);
    time_obj = RationalTime_from_time_string(time_string, 24, errorStatus);
    assert_true(RationalTime_almost_equal(t, time_obj, 0.001));

    time_string = "24:00:00";
    t = RationalTime_create(24 * 60 * 60 * 24, 24);
    time_obj = RationalTime_from_time_string(time_string, 24, errorStatus);
    assert_true(RationalTime_almost_equal(t, time_obj, 0.001));

    time_string = "23:59:59.958333";
    t = RationalTime_create(24 * 60 * 60 * 24 - 1, 24);
    time_obj = RationalTime_from_time_string(time_string, 24, errorStatus);
    assert_true(RationalTime_almost_equal(t, time_obj, 0.001));
}

static void opentime_rational_time_timestring25_test(void **state) {
    OpenTimeErrorStatus *errorStatus = OpenTimeErrorStatus_create();
    const char *time_string = "00:00:01";
    RationalTime t = RationalTime_create(25, 25);
    RationalTime time_obj =
            RationalTime_from_time_string(time_string, 25, errorStatus);
    assert_true(RationalTime_almost_equal(t, time_obj, 0.001));

    time_string = "00:01:00";
    t = RationalTime_create(25 * 60, 25);
    time_obj = RationalTime_from_time_string(time_string, 25, errorStatus);
    assert_true(RationalTime_almost_equal(t, time_obj, 0.001));

    time_string = "01:00:00";
    t = RationalTime_create(25 * 60 * 60, 25);
    time_obj = RationalTime_from_time_string(time_string, 25, errorStatus);
    assert_true(RationalTime_almost_equal(t, time_obj, 0.001));

    time_string = "24:00:00";
    t = RationalTime_create(25 * 60 * 60 * 24, 25);
    time_obj = RationalTime_from_time_string(time_string, 25, errorStatus);
    assert_true(RationalTime_almost_equal(t, time_obj, 0.001));

    time_string = "23:59:59.92";
    t = RationalTime_create(25 * 60 * 60 * 24 - 2, 25);
    time_obj = RationalTime_from_time_string(time_string, 25, errorStatus);
    assert_true(RationalTime_almost_equal(t, time_obj, 0.001));
}

static void opentime_rational_time_timestring23976fps_test(void **state) {

    int ref_values_val[16] = {1025, 179900, 180000, 360000,
                              720000, 1079300, 1080000, 1080150,
                              1440000, 1800000, 1978750, 1980000,
                              46700, 225950, 436400, 703350};
    const char *ref_values_ts[16] = {
            "00:00:01.708333", "00:04:59.833333", "00:05:00.0", "00:10:00.0",
            "00:20:00.0", "00:29:58.833333", "00:30:00.0", "00:30:00.25",
            "00:40:00.0", "00:50:00.0", "00:54:57.916666", "00:55:00.0",
            "00:01:17.833333", "00:06:16.583333", "00:12:07.333333", "00:19:32.25"
    };
    RationalTime t;
    for (int i = 0; i < 16; i++) {
        t = RationalTime_create(ref_values_val[i], 600);
        assert_string_equal(ref_values_ts[i], RationalTime_to_time_string(t));
    }
}

static void opentime_rational_time_to_frames_test(void **state) {
    struct RationalTimeTestState *testState = *state;
    RationalTime rationalTime = testState->rationalTime;
    assert_double_equal(RationalTime_to_frames(rationalTime), 48, DBL_EPSILON);
}

static void opentime_rational_time_to_frames_with_rate_test(void **state) {
    struct RationalTimeTestState *testState = *state;
    RationalTime rationalTime = testState->rationalTime;
    assert_double_equal(RationalTime_to_frames_with_rate(rationalTime, 48), 96, DBL_EPSILON);
}

static void opentime_rational_time_math_time_test(void **state) {
    RationalTime a = RationalTime_from_frames(100, 24);
    RationalTime gap = RationalTime_from_frames(50, 24);
    RationalTime b = RationalTime_from_frames(150, 24);
    RationalTime b_minus_a = RationalTime_subtract(b, a);
    RationalTime a_plus_gap = RationalTime_add(a, gap);
    RationalTime b_minus_gap = RationalTime_subtract(b, gap);

    assert_true(RationalTime_equal(b_minus_a, gap));

    assert_true(RationalTime_equal(a_plus_gap, b));

    assert_true(RationalTime_equal(b_minus_gap, a));
}

static void opentime_rational_time_compare_time_test(void **state) {
    RationalTime t1 = RationalTime_create(15.2, 1);
    RationalTime t2 = RationalTime_create(15.6, 1);
    RationalTime comparisonResult = RationalTime_compare(t1, t2);
    assert_true(RationalTime_value(comparisonResult) > 0);

    RationalTime t3 = RationalTime_create(30.4, 2);
    comparisonResult = RationalTime_compare(t1, t3);
    assert_true(RationalTime_value(comparisonResult) == 0);
    assert_true(RationalTime_equal(t1, t3));
    assert_false(RationalTime_not_equal(t1, t3));
}

static void opentime_time_range_create_test(void **state) {
    TimeRange tr = TimeRange_create();
    RationalTime blank = RationalTime_create(0, 1);
    RationalTime tr_start_time = TimeRange_start_time(tr);
    RationalTime tr_duration = TimeRange_duration(tr);
    assert_true(RationalTime_equal(tr_start_time, blank));
    assert_true(RationalTime_equal(tr_duration, blank));

    tr_start_time = RationalTime_create(10, 48);
    TimeRange tr1 = TimeRange_create_with_start_time(tr_start_time);
    RationalTime st = TimeRange_start_time(tr1);
    RationalTime dur = TimeRange_duration(tr1);
    assert_double_equal(RationalTime_rate(st), RationalTime_rate(dur), DBL_EPSILON);
}

static void opentime_time_range_end_time_test(void **state) {
    /* Test whole number duration */
    RationalTime rt_start = RationalTime_create(1, 24);
    RationalTime rt_dur = RationalTime_create(5, 24);
    TimeRange tr =
            TimeRange_create_with_start_time_and_duration(rt_start, rt_dur);
    RationalTime tr_dur = TimeRange_duration(tr);

    assert_true(RationalTime_equal(tr_dur, rt_dur));

    RationalTime tr_ete = TimeRange_end_time_exclusive(tr);
    RationalTime rt_start_plus_rt_dur = RationalTime_add(rt_start, rt_dur);

    assert_true(RationalTime_equal(tr_ete, rt_start_plus_rt_dur));

    RationalTime tr_eti = TimeRange_end_time_inclusive(tr);
    RationalTime unit_rt = RationalTime_create(1, 24);
    RationalTime eti = RationalTime_subtract(rt_start_plus_rt_dur, unit_rt);

    assert_true(RationalTime_equal(tr_eti, eti));

    /* Test non-integer duration value */
    rt_dur = RationalTime_create(5.5, 24);
    eti = RationalTime_create(6, 24);
    rt_start_plus_rt_dur = RationalTime_add(rt_start, rt_dur);
    tr = TimeRange_create_with_start_time_and_duration(rt_start, rt_dur);
    tr_ete = TimeRange_end_time_exclusive(tr);

    assert_true(RationalTime_equal(tr_ete, rt_start_plus_rt_dur));

    tr_eti = TimeRange_end_time_inclusive(tr);

    assert_true(RationalTime_equal(tr_eti, eti));
}

static void opentime_time_range_compare_test(void **state) {
    RationalTime start_time1 = RationalTime_create(18, 24);
    RationalTime duration1 = RationalTime_create(7, 24);
    TimeRange tr1 =
            TimeRange_create_with_start_time_and_duration(start_time1, duration1);
    RationalTime start_time2 = RationalTime_create(18, 24);
    RationalTime duration2 = RationalTime_create(14, 48);
    TimeRange tr2 =
            TimeRange_create_with_start_time_and_duration(start_time2, duration2);

    assert_true(TimeRange_equal(tr1, tr2));

    assert_false(TimeRange_not_equal(tr1, tr2));

    RationalTime start_time3 = RationalTime_create(20, 24);
    RationalTime duration3 = RationalTime_create(3, 24);
    TimeRange tr3 =
            TimeRange_create_with_start_time_and_duration(start_time3, duration3);

    assert_true(TimeRange_not_equal(tr1, tr3));

    assert_false(TimeRange_equal(tr1, tr3));
}

static void opentime_time_range_clamped_test(void **state) {
    RationalTime test_point_min = RationalTime_create(-2, 24);
    RationalTime test_point_max = RationalTime_create(6, 24);
    RationalTime rt1 = RationalTime_create(-1, 24);
    RationalTime rt2 = RationalTime_create(7, 24);
    TimeRange tr =
            TimeRange_create_with_start_time_and_duration(rt1, test_point_max);
    TimeRange other_tr =
            TimeRange_create_with_start_time_and_duration(test_point_min, rt2);
    RationalTime tr_start_time = TimeRange_start_time(tr);
    RationalTime tr_ete = TimeRange_end_time_inclusive(tr);
    RationalTime tr_clamped_min =
            TimeRange_clamped_with_rational_time(tr, test_point_min);
    RationalTime tr_clamped_max =
            TimeRange_clamped_with_rational_time(tr, test_point_max);

    assert_true(RationalTime_equal(tr_clamped_min, tr_start_time));

    assert_true(RationalTime_equal(tr_clamped_max, tr_ete));

    TimeRange tr_clamped_other_tr =
            TimeRange_clamped_with_time_range(tr, other_tr);

    assert_true(TimeRange_equal(tr_clamped_other_tr, tr));
}

static void opentime_time_range_contains_test(void **state) {
    RationalTime tstart = RationalTime_create(12, 25);
    RationalTime tdur = RationalTime_create(3.3, 25);
    TimeRange tr = TimeRange_create_with_start_time_and_duration(tstart, tdur);

    assert_true(TimeRange_contains_rational_time(tr, tstart));

    RationalTime tstart_plus_tdur = RationalTime_add(tstart, tdur);
    RationalTime tstart_minus_tdur = RationalTime_subtract(tstart, tdur);

    assert_false(TimeRange_contains_rational_time(tr, tstart_plus_tdur));

    assert_false(TimeRange_contains_rational_time(tr, tstart_minus_tdur));

    assert_false(TimeRange_contains_time_range(tr, tr));

    TimeRange tr_2 =
            TimeRange_create_with_start_time_and_duration(tstart_minus_tdur, tdur);

    assert_false(TimeRange_contains_time_range(tr, tr_2));

    assert_false(TimeRange_contains_time_range(tr_2, tr));
}

static void opentime_time_range_overlaps_rational_time_test(void **state) {
    RationalTime tstart = RationalTime_create(12, 25);
    RationalTime tdur = RationalTime_create(3, 25);
    TimeRange tr = TimeRange_create_with_start_time_and_duration(tstart, tdur);
    RationalTime rt1 = RationalTime_create(13, 25);
    RationalTime rt2 = RationalTime_create(1, 25);

    assert_true(TimeRange_overlaps_rational_time(tr, rt1));

    assert_false(TimeRange_overlaps_rational_time(tr, rt2));
}

static void opentime_time_range_overlaps_time_range_test(void **state) {
    RationalTime tstart = RationalTime_create(12, 25);
    RationalTime tdur = RationalTime_create(3, 25);
    TimeRange tr = TimeRange_create_with_start_time_and_duration(tstart, tdur);

    tstart = RationalTime_create(0, 25);
    tdur = RationalTime_create(3, 25);
    TimeRange tr_t =
            TimeRange_create_with_start_time_and_duration(tstart, tdur);

    assert_false(TimeRange_overlaps_time_range(tr, tr_t));

    tstart = RationalTime_create(10, 25);
    tdur = RationalTime_create(3, 25);
    tr_t = TimeRange_create_with_start_time_and_duration(tstart, tdur);

    assert_false(TimeRange_overlaps_time_range(tr, tr_t));

    tstart = RationalTime_create(13, 25);
    tdur = RationalTime_create(1, 25);
    tr_t = TimeRange_create_with_start_time_and_duration(tstart, tdur);

    assert_false(TimeRange_overlaps_time_range(tr, tr_t));

    tstart = RationalTime_create(2, 25);
    tdur = RationalTime_create(30, 25);
    tr_t = TimeRange_create_with_start_time_and_duration(tstart, tdur);

    assert_false(TimeRange_overlaps_time_range(tr, tr_t));

    tstart = RationalTime_create(2, 50);
    tdur = RationalTime_create(60, 50);
    tr_t = TimeRange_create_with_start_time_and_duration(tstart, tdur);

    assert_false(TimeRange_overlaps_time_range(tr, tr_t));

    tstart = RationalTime_create(2, 50);
    tdur = RationalTime_create(14, 50);
    tr_t = TimeRange_create_with_start_time_and_duration(tstart, tdur);

    assert_false(TimeRange_overlaps_time_range(tr, tr_t));

    tstart = RationalTime_create(-100, 50);
    tdur = RationalTime_create(400, 50);
    tr_t = TimeRange_create_with_start_time_and_duration(tstart, tdur);

    assert_false(TimeRange_overlaps_time_range(tr, tr_t));

    tstart = RationalTime_create(100, 50);
    tdur = RationalTime_create(400, 50);
    tr_t = TimeRange_create_with_start_time_and_duration(tstart, tdur);

    assert_false(TimeRange_overlaps_time_range(tr, tr_t));
}

static void opentime_time_range_range_from_start_end_time_test(void **state) {
    RationalTime tstart = RationalTime_create(0, 25);
    RationalTime tend = RationalTime_create(12, 25);
    TimeRange tr = TimeRange_range_from_start_end_time(tstart, tend);
    RationalTime tr_start = TimeRange_start_time(tr);
    RationalTime tr_dur = TimeRange_duration(tr);

    assert_true(RationalTime_equal(tr_start, tstart));

    assert_true(RationalTime_equal(tr_dur, tend));

    RationalTime tr_ete = TimeRange_end_time_exclusive(tr);
    RationalTime tr_eti = TimeRange_end_time_inclusive(tr);
    RationalTime unit_time = RationalTime_create(1, 25);
    RationalTime tend_minus_unit = RationalTime_subtract(tend, unit_time);

    assert_true(RationalTime_equal(tr_ete, tend));

    assert_true(RationalTime_equal(tr_eti, tend_minus_unit));
}

static void opentime_time_range_adjacent_time_ranges_test(void **state) {
    double d1 = 0.3;
    double d2 = 0.4;
    RationalTime zeroTime = RationalTime_create(0, 1);
    RationalTime d1Time = RationalTime_create(d1, 1);
    RationalTime d2Time = RationalTime_create(d2, 1);
    RationalTime d1Plusd2Time = RationalTime_create(d1 + d2, 1);
    TimeRange r1 =
            TimeRange_create_with_start_time_and_duration(zeroTime, d1Time);
    RationalTime r1_ete = TimeRange_end_time_exclusive(r1);
    TimeRange r2 =
            TimeRange_create_with_start_time_and_duration(r1_ete, d2Time);
    TimeRange full =
            TimeRange_create_with_start_time_and_duration(zeroTime, d1Plusd2Time);

    assert_false(TimeRange_overlaps_time_range(r1, r2));

    TimeRange r1Extendedr2 = TimeRange_extended_by(r1, r2);
    RationalTime ex_start = TimeRange_start_time(r1Extendedr2);
    RationalTime ex_ete = TimeRange_end_time_exclusive(r1Extendedr2);

    assert_true(RationalTime_equal(ex_start, zeroTime));

    assert_true(RationalTime_equal(ex_ete, d1Plusd2Time));

    assert_true(TimeRange_equal(r1Extendedr2, full));
}

static void opentime_time_range_distant_time_ranges_test(void **state) {
    double start = 0.1;
    double d1 = 0.3;
    double gap = 1.7;
    double d2 = 0.4;
    RationalTime start_time = RationalTime_create(start, 1);
    RationalTime d1_time = RationalTime_create(d1, 1);
    RationalTime start_gap_d1_time = RationalTime_create(start + gap + d1, 1);
    RationalTime d2_time = RationalTime_create(d2, 1);
    RationalTime d1_gap_d2_time = RationalTime_create(d1 + gap + d2, 1);
    TimeRange r1 =
            TimeRange_create_with_start_time_and_duration(start_time, d1_time);
    TimeRange r2 = TimeRange_create_with_start_time_and_duration(
            start_gap_d1_time, d2_time);
    TimeRange full = TimeRange_create_with_start_time_and_duration(
            start_time, d1_gap_d2_time);

    assert_false(TimeRange_overlaps_time_range(r1, r2));

    TimeRange r1Extendedr2 = TimeRange_extended_by(r1, r2);
    TimeRange r2Extendedr1 = TimeRange_extended_by(r2, r1);

    assert_true(TimeRange_equal(full, r1Extendedr2));

    assert_true(TimeRange_equal(full, r2Extendedr1));
}

static void opentime_time_transform_identity_transform_test(void **state) {
    RationalTime tstart = RationalTime_create(12, 25);
    TimeTransform txform = TimeTransform_create();
    RationalTime transformedTime =
            TimeTransform_applied_to_rational_time(txform, tstart);
    assert_true(RationalTime_equal(transformedTime, tstart));

    RationalTime blank = RationalTime_create(0, 1);
    txform = TimeTransform_create_with_offset_scale_rate(blank, 1, 50);
    transformedTime = TimeTransform_applied_to_rational_time(txform, tstart);
    assert_double_equal(RationalTime_value(transformedTime), 24, DBL_EPSILON);
}

static void opentime_time_transform_offset_test(void **state) {
    RationalTime tstart = RationalTime_create(12, 25);
    RationalTime toffset = RationalTime_create(10, 25);
    TimeTransform txform =
            TimeTransform_create_with_offset_scale_rate(toffset, 1, -1);
    RationalTime tstart_plus_offset = RationalTime_add(tstart, toffset);
    RationalTime transformedTime =
            TimeTransform_applied_to_rational_time(txform, tstart);

    assert_true(RationalTime_equal(tstart_plus_offset, transformedTime));

    TimeRange tr =
            TimeRange_create_with_start_time_and_duration(tstart, tstart);
    TimeRange transformedRange =
            TimeTransform_applied_to_time_range(txform, tr);
    TimeRange baseRange = TimeRange_create_with_start_time_and_duration(
            tstart_plus_offset, tstart);

    assert_true(TimeRange_equal(transformedRange, baseRange));
}

static void opentime_time_transform_scale_test(void **state) {
    RationalTime tstart = RationalTime_create(12, 25);
    RationalTime blank = RationalTime_create(0, 1);
    TimeTransform txform =
            TimeTransform_create_with_offset_scale_rate(blank, 2, -1);
    RationalTime baseTime = RationalTime_create(24, 25);
    RationalTime transformedTime =
            TimeTransform_applied_to_rational_time(txform, tstart);

    assert_true(RationalTime_equal(baseTime, transformedTime));

    TimeRange tr =
            TimeRange_create_with_start_time_and_duration(tstart, tstart);
    TimeRange transformedRange =
            TimeTransform_applied_to_time_range(txform, tr);
    TimeRange baseRange =
            TimeRange_create_with_start_time_and_duration(baseTime, baseTime);

    assert_true(TimeRange_equal(baseRange, transformedRange));
}

static void opentime_time_transform_rate_test(void **state) {
    RationalTime blank = RationalTime_create(0, 1);
    TimeTransform txform1 = TimeTransform_create();
    TimeTransform txform2 =
            TimeTransform_create_with_offset_scale_rate(blank, 1, 50);
    TimeTransform transformedTransform =
            TimeTransform_applied_to_time_transform(txform1, txform2);

    assert_double_equal(TimeTransform_rate(txform2), TimeTransform_rate(transformedTransform), DBL_EPSILON);
}

static void opentime_time_transform_comparison_test(void **state) {
    RationalTime tstart = RationalTime_create(12, 25);
    TimeTransform txform =
            TimeTransform_create_with_offset_scale_rate(tstart, 2, -1);
    TimeTransform txform2 =
            TimeTransform_create_with_offset_scale_rate(tstart, 2, -1);

    assert_true(TimeTransform_equal(txform, txform2));

    assert_false(TimeTransform_not_equal(txform, txform2));

    tstart = RationalTime_create(23, 25);
    TimeTransform txform3 =
            TimeTransform_create_with_offset_scale_rate(tstart, 2, -1);

    assert_true(TimeTransform_not_equal(txform, txform3));

    assert_false(TimeTransform_equal(txform, txform3));
}

int main(void) {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test_setup_teardown(opentime_rational_time_invalid_time_test, setupRationalTimeTests,
                                            teardownRationalTimeTests),
            cmocka_unit_test_setup_teardown(opentime_rational_time_invalid_time_test, setupRationalTimeTests,
                                            teardownRationalTimeTests),
            cmocka_unit_test_setup_teardown(opentime_rational_time_get_rate_test, setupRationalTimeTests,
                                            teardownRationalTimeTests),
            cmocka_unit_test_setup_teardown(opentime_rational_time_rescale_to_rate_test, setupRationalTimeTests,
                                            teardownRationalTimeTests),
            cmocka_unit_test_setup_teardown(opentime_rational_time_rescale_to_rational_time_test,
                                            setupRationalTimeTests, teardownRationalTimeTests),
            cmocka_unit_test_setup_teardown(opentime_rational_time_value_rescaled_to_rate_test, setupRationalTimeTests,
                                            teardownRationalTimeTests),
            cmocka_unit_test_setup_teardown(opentime_rational_time_value_rescaled_to_rational_time_test,
                                            setupRationalTimeTests, teardownRationalTimeTests),
            cmocka_unit_test_setup_teardown(opentime_rational_time_almost_equal_test, setupRationalTimeTests,
                                            teardownRationalTimeTests),
            cmocka_unit_test_setup_teardown(opentime_rational_time_duration_from_start_end_time_test,
                                            setupRationalTimeTests, teardownRationalTimeTests),
            cmocka_unit_test_setup_teardown(opentime_rational_time_is_valid_timecode_test, setupRationalTimeTests,
                                            teardownRationalTimeTests),
            cmocka_unit_test_setup_teardown(opentime_rational_time_from_frames_test, setupRationalTimeTests,
                                            teardownRationalTimeTests),
            cmocka_unit_test_setup_teardown(opentime_rational_time_seconds_test, setupRationalTimeTests,
                                            teardownRationalTimeTests),
            cmocka_unit_test_setup_teardown(opentime_rational_time_timecode24_test, setupRationalTimeTests,
                                            teardownRationalTimeTests),
            cmocka_unit_test_setup_teardown(opentime_rational_time_timecode23967fps_test, setupRationalTimeTests,
                                            teardownRationalTimeTests),
            cmocka_unit_test_setup_teardown(opentime_rational_time_timecodentsc2997fps_test, setupRationalTimeTests,
                                            teardownRationalTimeTests),
            cmocka_unit_test_setup_teardown(opentime_rational_time_timecode2997_test, setupRationalTimeTests,
                                            teardownRationalTimeTests),
            cmocka_unit_test_setup_teardown(opentime_rational_time_timestring24_test, setupRationalTimeTests,
                                            teardownRationalTimeTests),
            cmocka_unit_test_setup_teardown(opentime_rational_time_timestring25_test, setupRationalTimeTests,
                                            teardownRationalTimeTests),
            cmocka_unit_test_setup_teardown(opentime_rational_time_timestring23976fps_test, setupRationalTimeTests,
                                            teardownRationalTimeTests),
            cmocka_unit_test_setup_teardown(opentime_rational_time_to_frames_test, setupRationalTimeTests,
                                            teardownRationalTimeTests),
            cmocka_unit_test_setup_teardown(opentime_rational_time_to_frames_with_rate_test, setupRationalTimeTests,
                                            teardownRationalTimeTests),
            cmocka_unit_test_setup_teardown(opentime_rational_time_math_time_test, setupRationalTimeTests,
                                            teardownRationalTimeTests),
            cmocka_unit_test_setup_teardown(opentime_rational_time_compare_time_test, setupRationalTimeTests,
                                            teardownRationalTimeTests),
            cmocka_unit_test(opentime_time_range_create_test),
            cmocka_unit_test(opentime_time_range_end_time_test),
            cmocka_unit_test(opentime_time_range_compare_test),
            cmocka_unit_test(opentime_time_range_clamped_test),
            cmocka_unit_test(opentime_time_range_contains_test),
            cmocka_unit_test(opentime_time_range_overlaps_rational_time_test),
            cmocka_unit_test(opentime_time_range_overlaps_time_range_test),
            cmocka_unit_test(opentime_time_range_range_from_start_end_time_test),
            cmocka_unit_test(opentime_time_range_adjacent_time_ranges_test),
            cmocka_unit_test(opentime_time_range_distant_time_ranges_test),
            cmocka_unit_test(opentime_time_transform_identity_transform_test),
            cmocka_unit_test(opentime_time_transform_offset_test),
            cmocka_unit_test(opentime_time_transform_scale_test),
            cmocka_unit_test(opentime_time_transform_rate_test),
            cmocka_unit_test(opentime_time_transform_comparison_test),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
