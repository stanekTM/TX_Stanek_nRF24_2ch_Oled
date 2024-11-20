
//*********************************************************************************************************************
// Setup radio comunication
//*********************************************************************************************************************
RF24 radio(PIN_CE, PIN_CSN); //setup CE and CSN pins

void radio_setup()
{
  radio.begin();
  radio.setAutoAck(true);
  radio.enableAckPayload();
  radio.enableDynamicPayloads();
  radio.setRetries(5, 15);
  radio.setChannel(RADIO_CHANNEL);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_MIN); //RF24_PA_MIN (-18dBm), RF24_PA_LOW (-12dBm), RF24_PA_HIGH (-6dbm), RF24_PA_MAX (0dBm)
  radio.stopListening();
  radio.openWritingPipe(address);
}

//*********************************************************************************************************************
// send and receive data **********************************************************************************************
//*********************************************************************************************************************
bool rf_state = 0;
byte telemetry_counter = 0;
byte packet_state = 0;
byte value_tssi = 0;
byte tssi = 0;
unsigned long arc_time = 0;
byte retries = 0; 

void send_and_receive_data()
{
  rc_packet.ch1 = pots_value[0]; //A0
  rc_packet.ch2 = pots_value[1]; //A1
  
  
  if (radio.write(&rc_packet, sizeof(rc_packet_size)))
  {
    if (radio.available())
    {
      radio.read(&telemetry_packet, sizeof(telemetry_packet_size));
      
      rf_state = 1;
      
      RX_batt_check();
      
      telemetry_counter++;
    }
  }
  
  if (packet_state++ > 253)
  {
    value_tssi = telemetry_counter;
    telemetry_counter = 0;
    packet_state = 0;
  }
  tssi = map(value_tssi, 0, 255, 0, 100);
  
  /*
  retries = radio.getARC();
  
  if (millis() - arc_time > 900)
  {
    arc_time = millis();
    
    tssi = map(retries, 15, 0, 0, 100);
  }
  */
}
 
