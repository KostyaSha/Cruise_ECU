#include <CAN.h>

//________________flags for cruise state
boolean flag1 = true;
boolean flag2 = true;

//________________CAN default messages
uint8_t set_speed = 0x0;
int addr[] = {0xfd, 0xfe, 0xff};
int data[] = {0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7};

//________________pedal State
int gas_pedal_state = 1;
int brake_pedal_state = 1;

//________________average(current speed in km/h)
double average = 50;

void setup() {
  //________________start the Serial
  Serial.begin(250000);

  CAN.setPins(9, 2);
  //________________start the CAN bus at 500 kbps
  if (!CAN.begin(500E3)) {
    Serial.println("Starting CAN failed!");
    while (1);
  }
}

void loop() {
  //________________serial read for Buttons
  if (Serial.available() > 0) {
    String s1 = Serial.readStringUntil('\n');
    if (s1 == "a") {
      flag1 = !flag1;
    }
    if (s1 == "b") {
      flag2 = !flag2;
    }
  }

  //________________send Fingerprint msgs
  for (int ii = 0; ii < 3; ii++) {
    CAN.beginPacket(addr[ii]);
    for (int i = 0; i < 8; i++) {
      CAN.write(data[i]);
    }
    CAN.endPacket();
  }

  //________________send 0x1d2 msg PCM_CRUISE
  uint8_t dat[8];
  dat[0] = (flag2 << 5) & 0x20 | (!gas_pedal_state << 4) & 0x10;
  dat[1] = 0x0;
  dat[2] = 0x0;
  dat[3] = 0x0;
  dat[4] = 0x0;
  dat[5] = 0x0;
  dat[6] = (flag2 << 7) & 0x80;
  dat[7] = can_cksum(dat, 7, 0x1d2);
  CAN.beginPacket(0x1d2);
  for (int ii = 0; ii < 8; ii++) {
    CAN.write(dat[ii]);
  }
  CAN.endPacket();

  //________________send 0x1d3 msg PCM_CRUISE_2
  uint8_t dat2[8];
  dat2[0] = 0x0;
  dat2[1] = (flag1 << 7) & 0x80 | 0x28;
  dat2[2] = set_speed;
  dat2[3] = 0x0;
  dat2[4] = 0x0;
  dat2[5] = 0x0;
  dat2[6] = 0x0;
  dat2[7] = can_cksum(dat2, 7, 0x1d3);
  CAN.beginPacket(0x1d3);
  for (int ii = 0; ii < 8; ii++) {
    CAN.write(dat2[ii]);
  }
  CAN.endPacket();

  //________________send 0xaa msg defaults 1a 6f WHEEL_SPEEDS
  uint8_t dat3[8];
  uint16_t wheelspeed = 0x1a6f + (average * 100);
  dat3[0] = (wheelspeed >> 8) & 0xFF;
  dat3[1] = (wheelspeed >> 0) & 0xFF;
  dat3[2] = (wheelspeed >> 8) & 0xFF;
  dat3[3] = (wheelspeed >> 0) & 0xFF;
  dat3[4] = (wheelspeed >> 8) & 0xFF;
  dat3[5] = (wheelspeed >> 0) & 0xFF;
  dat3[6] = (wheelspeed >> 8) & 0xFF;
  dat3[7] = (wheelspeed >> 0) & 0xFF;
  CAN.beginPacket(0xaa);
  for (int ii = 0; ii < 8; ii++) {
    CAN.write(dat3[ii]);
  }
  CAN.endPacket();

  //________________send 0x3b7 msg ESP_CONTROL
  uint8_t dat5[8];
  dat5[0] = 0x0;
  dat5[1] = 0x0;
  dat5[2] = 0x0;
  dat5[3] = 0x0;
  dat5[4] = 0x0;
  dat5[5] = 0x0;
  dat5[6] = 0x0;
  dat5[7] = 0x08;
  CAN.beginPacket(0x3b7);
  for (int ii = 0; ii < 8; ii++) {
    CAN.write(dat5[ii]);
  }
  CAN.endPacket();

  //________________send 0x620 msg STEATS_DOORS
  uint8_t dat6[8];
  dat6[0] = 0x10;
  dat6[1] = 0x0;
  dat6[2] = 0x0;
  dat6[3] = 0x1d;
  dat6[4] = 0xb0;
  dat6[5] = 0x40;
  dat6[6] = 0x0;
  dat6[7] = 0x0;
  CAN.beginPacket(0x620);
  for (int ii = 0; ii < 8; ii++) {
    CAN.write(dat6[ii]);
  }
  CAN.endPacket();

  //________________send 0x3bc msg GEAR_PACKET
  uint8_t dat7[8];
  dat7[0] = 0x0;
  dat7[1] = 0x0;
  dat7[2] = 0x0;
  dat7[3] = 0x0;
  dat7[4] = 0x0;
  dat7[5] = 0x80;
  dat7[6] = 0x0;
  dat7[7] = 0x0;
  CAN.beginPacket(0x3bc);
  for (int ii = 0; ii < 8; ii++) {
    CAN.write(dat7[ii]);
  }
  CAN.endPacket();

  //________________send 0x2c1 msg GAS_PEDAL
  uint8_t dat10[8];
  dat10[0] = (!gas_pedal_state << 3) & 0x08;
  dat10[1] = 0x0;
  dat10[2] = 0x0;
  dat10[3] = 0x0;
  dat10[4] = 0x0;
  dat10[5] = 0x0;
  dat10[6] = 0x0;
  dat10[7] = 0x0;
  CAN.beginPacket(0x2c1);
  for (int ii = 0; ii < 8; ii++) {
    CAN.write(dat10[ii]);
  }
  CAN.endPacket();

  //________________send 0x224 msg fake brake module
  uint8_t dat11[8];
  dat11[0] = 0x0;
  dat11[1] = 0x0;
  dat11[2] = 0x0;
  dat11[3] = 0x0;
  dat11[4] = 0x0;
  dat11[5] = 0x0;
  dat11[6] = 0x0;
  dat11[7] = 0x8;
  CAN.beginPacket(0x224);
  for (int ii = 0; ii < 8; ii++) {
    CAN.write(dat11[ii]);
  }
  CAN.endPacket();

  //________________send 0x262 fake EPS_STATUS
  uint8_t dat8[8];
  dat8[0] = 0x0;
  dat8[1] = 0x0;
  dat8[2] = 0x0;
  dat8[3] = 0x3;
  dat8[4] = 0x6c;
  CAN.beginPacket(0x262);
  for (int ii = 0; ii < 5; ii++) {
    CAN.write(dat8[ii]);
  }
  CAN.endPacket();

  //________________send 0x260 fake STEER_TORQUE_SENSOR
  uint8_t dat9[8];
  dat9[0] = 0x08;
  dat9[1] = 0xff;
  dat9[2] = 0xfb;
  dat9[3] = 0x0;
  dat9[4] = 0x0;
  dat9[5] = 0xff;
  dat9[6] = 0xdc;
  dat9[7] = 0x47;
  CAN.beginPacket(0x260);
  for (int ii = 0; ii < 8; ii++) {
    CAN.write(dat9[ii]);
  }
  CAN.endPacket();

  //________________send 0x423 fake EPS message
  CAN.beginPacket(0x423);
  CAN.write(0x00);
  CAN.endPacket();

  //________________send 0xb4 speed and encoder for throttle ECU
  uint16_t kmh = (average * 100);
  CAN.beginPacket(0xb4);
  CAN.print(kmh);
  CAN.endPacket();
    
  //________________read ACC_CMD from CANbus
  CAN.parsePacket();
  if (CAN.packetId() == 0x200)
      {
      uint8_t dat[8];
      for (int ii = 0; ii <= 7; ii++) {
        dat[ii]  = (char) CAN.read();
        }
        ACC_CMD = (dat[0] << 8 | dat[1] << 0); 
       } 
   //________________calculating ACC_CMD into ACC_CMD_PERCENT
  if (ACC_CMD >= minACC_CMD) {
      ACC_CMD1 = ACC_CMD;
      }
  else {
      ACC_CMD1 = minACC_CMD;
      }
  ACC_CMD_PERCENT = ((100/(maxACC_CMD - minACC_CMD)) * (ACC_CMD1 - minACC_CMD));
    
  Serial.print("Gas pedal position");
  Serial.print("__");
  Serial.print(ACC_CMD_PERCENT);
  Serial.print("%");
  Serial.println("");

}


//TOYOTA CAN CHECKSUM
int can_cksum (uint8_t *dat, uint8_t len, uint16_t addr) {
  uint8_t checksum = 0;
  checksum = ((addr & 0xFF00) >> 8) + (addr & 0x00FF) + len + 1;
  //uint16_t temp_msg = msg;
  for (int ii = 0; ii < len; ii++) {
    checksum += (dat[ii]);
  }
  return checksum;
}
