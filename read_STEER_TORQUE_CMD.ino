 
 float STEER_TORQUE_CMD = 0;
 

 //0x2e4 msg GAS_PEDAL
    if (CAN.packetId() == 0x2e4)
      {
      uint8_t dat_2e4[5];
      for (int ii = 0; ii <= 4; ii++) {
        dat_2e4[ii]  = (char) CAN.read();
        }
        STEER_TORQUE_CMD = (dat_2e4[1] << 8 | dat_2e4[2] << 0);
      }
