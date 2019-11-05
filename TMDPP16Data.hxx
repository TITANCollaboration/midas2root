#ifndef TMDPP16Data_hxx_seen
#define TMDPP16Data_hxx_seen

#include <vector>

#include "TGenericData.hxx"

class ADCMDPP16 {

  friend class TMDPP16Data;

public:
  /// Fields to hold the header, measurement, extendedtimestamp words.
  uint32_t adc_header_word;
  uint32_t adc_measurement_word;
  uint32_t adc_extendedtimestamp_word;

  /// Get the ADC measurement
  uint32_t GetMeasurement() const {return (adc_measurement_word & 0xFFFF);};
 // uint32_t GetMeasurement() const;
  /// Get Module ID
  uint32_t GetModuleID() const {return (adc_header_word >> 16) & 0xFF;};

  /// Get the channel number
  uint32_t GetChannel() const {return ((adc_measurement_word >> 16) & 0x1F);}

  /// Is Out Of Range?
//  bool IsOutOfRange() const {return ((adc_measurement_word & 0x4000) == 0x4000);}

  uint32_t GetFlag()  const {return ((adc_measurement_word >> 22) & 0x3);}  //  pileup/overrange

  /// Constructor; need to pass in header and measurement.
  ADCMDPP16(uint32_t header, uint32_t measurement):
    adc_header_word(header),
    adc_measurement_word(measurement),
    adc_extendedtimestamp_word(0){};


  ADCMDPP16();
};




/// Class to store data from Mesytec MADC32 module.
/// For details on this module see:
/// http://daq-plone.triumf.ca/HR/VME/mesytec-modules/MADC-32_V14_fw0126.pdf/at_download/file
class TMDPP16Data: public TGenericData {

public:

  /// Constructor
  TMDPP16Data(int bklen, int bktype, const char* name, void *pdata);
  void Print();
//  void Print();

  /// Get the Vector of ADC Measurements.
  std::vector<ADCMDPP16>& GetMeasurements() {return fMeasurements;};


  uint32_t GetADCResolution() const {return (fGlobalHeader >>10) & 0x7;};

  /// Get Module ID
  uint32_t GetModuleID() const {return (fGlobalHeader & 0xff0000) >> 16;};

  uint32_t GetHeader() const {return (fGlobalHeader);};



  /// Get Output format (???)
 // uint32_t GetOutputFormat() const {return (fGlobalHeader & 0x8000) >> 15;};

  // Get timestamp from footer
  uint32_t GetTimeStamp() const {return (fGlobalTrailer >> 2);};

private:

  /// The overall global header
  uint32_t fGlobalHeader;

  /// The overall global trailer
  uint32_t fGlobalTrailer;

  /// Vector of ADC Measurements.
  std::vector<ADCMDPP16> fMeasurements;

};

#endif
