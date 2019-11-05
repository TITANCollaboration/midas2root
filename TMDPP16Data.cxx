#include "TMDPP16Data.hxx"
#include <bitset>




TMDPP16Data::TMDPP16Data(int bklen, int bktype, const char* name, void *pdata):
    TGenericData(bklen, bktype, name, pdata)
{
  /// Save the current header.
  fGlobalHeader = 0;
  fGlobalTrailer = 0;
  int chan, mod_id = 0;
  for(int i = 0; i < GetSize(); i++){
    uint32_t word = GetData32()[i];
  //  if(i == 0) {
    if((word & 0xc0000000) == 0xc0000000){ // Check if we have b11 to end
      fGlobalTrailer = word;
      }
      else if((word & 0x40000000) == 0x40000000) {  // check if header b01
      fGlobalHeader = word;
    }
     else {  // Anything else should be data
        chan = (word>>16) & 0x1F;
        fMeasurements.push_back(ADCMDPP16(fGlobalHeader,word));
      }
  }
}


void TMDPP16Data::Print(){

  std::cout << "Data for Mesytec module in bank " << GetName() << std::endl;
  std::cout << "Module ID = " << GetModuleID() << std::endl;
  std::cout << "Bit resolution : 0x" << std::hex << std::endl;

  std::cout << "Timestamp = " << GetTimeStamp() << std::endl;

  std::vector<ADCMDPP16> measurements = GetMeasurements();
  std::cout << "Number of measurements: " << measurements.size() << std::endl;

  for(unsigned int i = 0; i < measurements.size(); i++){
    ADCMDPP16 adcmeas = measurements[i];
      uint32_t adc_value = adcmeas.GetMeasurement();
    std::cout << "Measurement : " << adc_value << "Dsig : " << std::bitset<32>(adc_value) << "Channel: " << adcmeas.GetChannel() << "\n";
  }
  std::cout <<  std::endl;


}
