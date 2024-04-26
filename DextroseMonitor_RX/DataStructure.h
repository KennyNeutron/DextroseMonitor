//Communication Data Structure
// Max size of this struct is 32 bytes - NRF24L01 buffer limit
struct Payload_Data {
  uint8_t ID = 0xff;
  float DXweight = 0.0;
};

Payload_Data payload;  // Create a variable with the above structure