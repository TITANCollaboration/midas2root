#include "TGRIF16Data.hxx"
#include <bitset>




TGRIF16Data::TGRIF16Data(int bklen, int bktype, const char* name, void *pdata):
    TGenericData(bklen, bktype, name, pdata)
{
  /// Save the current header.
  fGlobalHeader = 0;
  fGlobalTrailer = 0;
  int chan, mod_id = 0;
  for(int i = 0; i < GetSize(); i++){
    uint32_t word = GetData32()[i];
     if((word & 0xF0000000) >> 28 == 0x8) {  // check if header
      fGlobalHeader = word;
    //   std::cout << "Got a header : " << std::hex << fGlobalHeader << "\n";
    }
    else if((word & 0xF0000000) >> 28 == 0xe){ // Check if we have b11 to end
      fGlobalTrailer = word;
     // std::cout << "Got a trailer : " << std::hex << fGlobalTrailer << "\n";
    } else {
      //std::cout << "Word : " << std::hex << word << "\n";
      fMeasurements.push_back(ADCGRIF16(fGlobalHeader,word));

    }


 //    else {  // Anything else should be data
  //      chan = (word>>16) & 0x1F;
   //     fMeasurements.push_back(ADCGRIF16(fGlobalHeader,word));
    //  }
  }
}


void TGRIF16Data::Print(){

  std::cout << "Data for GRIF module in bank " << GetName() << std::endl;
  std::cout << "Module ID = " << GetMyModuleID() << std::endl;
  std::cout << "Channel ID = " << GetChannel() << std::endl;

  //std::cout << "Bit resolution : 0x" << std::hex << std::endl;

 // std::cout << "Timestamp = " << GetTimeStamp() << std::endl;

  //std::vector<ADCGRIF16> measurements = GetMeasurements();
  //std::cout << "Number of measurements: " << measurements.size() << std::endl;

//  for(unsigned int i = 0; i < measurements.size(); i++){
 //   ADCGRIF16 adcmeas = measurements[i];
  //    uint32_t adc_value = adcmeas.GetMeasurement();
   // std::cout << "Measurement : " << adc_value << "Dsig : " << std::bitset<32>(adc_value) << "Channel: " << adcmeas.GetChannel() << "\n";
 // }
  //std::cout <<  std::endl;


}
