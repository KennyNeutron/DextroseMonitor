void NRF_Send() {
  payload.ID=0x02;
  payload.DXweight=currentWeight;

  Serial.println("NRF Sent");
  NRF.write(&payload, sizeof(Payload_Data));
}