#include "wokwi-api.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  pin_t ena1; // Enable pin for H-Bridge 1
  pin_t in1;  // Logic input for OUT1
  pin_t in2;  // Logic input for OUT2
  pin_t ena2; // Enable pin for H-Bridge 2
  pin_t in3;  // Logic input for OUT3
  pin_t in4;  // Logic input for OUT4

  pin_t out1; // Output pin 1
  pin_t out2; // Output pin 2
  pin_t out3; // Output pin 3
  pin_t out4; // Output pin 4
} chip_state_t;

static void update_output(chip_state_t *chip) {
  uint32_t ena1 = pin_read(chip->ena1);
  uint32_t in1 = pin_read(chip->in1);
  uint32_t in2 = pin_read(chip->in2);
  uint32_t ena2 = pin_read(chip->ena2);
  uint32_t in3 = pin_read(chip->in3);
  uint32_t in4 = pin_read(chip->in4);

  // H-Bridge 1 (OUT1, OUT2)
  if (ena1) {
    pin_write(chip->out1, in1);
    pin_write(chip->out2, in2);
  } else {
    pin_write(chip->out1, 0);
    pin_write(chip->out2, 0);
  }

  // H-Bridge 2 (OUT3, OUT4)
  if (ena2) {
    pin_write(chip->out3, in3);
    pin_write(chip->out4, in4);
  } else {
    pin_write(chip->out3, 0);
    pin_write(chip->out4, 0);
  }

  printf("ENA1=%d, IN1=%d, IN2=%d => OUT1=%d, OUT2=%d\n", ena1, in1, in2, pin_read(chip->out1), pin_read(chip->out2));
  printf("ENA2=%d, IN3=%d, IN4=%d => OUT3=%d, OUT4=%d\n", ena2, in3, in4, pin_read(chip->out3), pin_read(chip->out4));
}

static void chip_pin_change(void *user_data, pin_t pin, uint32_t value) {
  update_output((chip_state_t *)user_data);
}

void chip_init() {
  chip_state_t *chip = malloc(sizeof(chip_state_t));

  // Initialize input pins
  chip->ena1 = pin_init("ENA1", INPUT);
  chip->in1 = pin_init("IN1", INPUT);
  chip->in2 = pin_init("IN2", INPUT);
  chip->ena2 = pin_init("ENA2", INPUT);
  chip->in3 = pin_init("IN3", INPUT);
  chip->in4 = pin_init("IN4", INPUT);

  // Initialize output pins
  chip->out1 = pin_init("OUT1", OUTPUT);
  chip->out2 = pin_init("OUT2", OUTPUT);
  chip->out3 = pin_init("OUT3", OUTPUT);
  chip->out4 = pin_init("OUT4", OUTPUT);

  // Set up pin watchers for input pins
  const pin_watch_config_t watch_config = {
      .edge = BOTH,
      .pin_change = chip_pin_change,
      .user_data = chip,
  };
  pin_watch(chip->ena1, &watch_config);
  pin_watch(chip->in1, &watch_config);
  pin_watch(chip->in2, &watch_config);
  pin_watch(chip->ena2, &watch_config);
  pin_watch(chip->in3, &watch_config);
  pin_watch(chip->in4, &watch_config);

  printf("L293D Custom Chip Initialized!\n");
}
