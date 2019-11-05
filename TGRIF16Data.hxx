#ifndef TGRIF16Data_hxx_seen
#define TGRIF16Data_hxx_seen

#include <vector>

#include "TGenericData.hxx"

class ADCGRIF16 {

  friend class TGRIF16Data;

public:
  /// Fields to hold the header, measurement, extendedtimestamp words.
  uint32_t adc_header_word;
  uint32_t adc_measurement_word;
  uint32_t adc_extendedtimestamp_word;

  /// Get the ADC measurement
  uint32_t GetMeasurement() const {return (adc_measurement_word & 0x1FFFFFF);};//& 0x1FFFFFF);};
  //  uint32_t GetMeasurement() const {return (adc_measurement_word & 0xFFFF);};//& 0x1FFFFFF);};

 // uint32_t GetMeasurement() const;
  /// Get Module ID
  uint32_t GetModuleID() const {return (adc_header_word & 0x0E000000) >> 25; };

  /// Get the channel number
  uint32_t GetChannel() const {return ((adc_header_word & 0x000000FF) >> 0);}

  /// Is Out Of Range?
//  bool IsOutOfRange() const {return ((adc_measurement_word & 0x4000) == 0x4000);}

  uint32_t GetFlag()  const {return ((adc_measurement_word >> 22) & 0x3);}  //  pileup/overrange

  /// Constructor; need to pass in header and measurement.
  ADCGRIF16(uint32_t header, uint32_t measurement):
    adc_header_word(header),
    adc_measurement_word(measurement),
    adc_extendedtimestamp_word(0){};


  ADCGRIF16();
};




/// Class to store data from GRIF16 module.
/// For details on this module see:
class TGRIF16Data: public TGenericData {

public:

  /// Constructor
  TGRIF16Data(int bklen, int bktype, const char* name, void *pdata);
  void Print();
//  void Print();

  /// Get the Vector of ADC Measurements.
  std::vector<ADCGRIF16>& GetMeasurements() {return fMeasurements;};


  uint32_t GetADCResolution() const {return (fGlobalHeader >>10) & 0x7;};

  /// Get Module ID
  uint32_t GetMyModuleID() const {return (fGlobalHeader & 0x0E000000) >> 25;};

  uint32_t GetChannel() const {return ((fGlobalHeader & 0x000000FF) >> 0);}


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
  std::vector<ADCGRIF16> fMeasurements;

};

#endif
