void NRF_Send() {
  payload.ID=0x01;
  payload.DXweight=int(currentWeight);

  Serial.println("NRF Sent");
  NRF.write(&payload, sizeof(Payload_Data));
}