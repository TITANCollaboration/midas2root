// Example Program for converting MIDAS format to ROOT format.
//
// T. Lindner (Jan 2016)
//
// Updated 17/7/2019 by Jon Ringuette to create histograms and data for the GRIF16

#include <stdio.h>
#include <iostream>
#include <time.h>
#include <vector>

#include "TRootanaEventLoop.hxx"
#include "TFile.h"
#include "TTree.h"
#include "midas.h"
#include "rmidas.h"


#include <TH1F.h>
#include <TH2F.h>
#include <TTree.h>
#include <TDirectory.h>
//#include "TAnaManager.hxx"


#include "TGRIF16Data.hxx"

#define NUM_ADC_CHANNELS    16  // Number of ADC channels
#define HIT_CHAN    64
#define ADC_CHAN    65536


/*-- Module declarations -------------------------------------------*/


class Analyzer: public TRootanaEventLoop {

public:
  // An analysis manager.  Define and fill histograms in
  // analysis manager.
  //TAnaManager *anaManager;

  int midas_timestamp;
  int midas_serialnumber;
  int chan;
  int adc_values[16];
  int event_flag;
  uint32_t adc_value;
  uint32_t tdc_value;
  uint32_t adc_timestamp;
  TFile *rootFile;
  TH1F *adc_per_channel_histograms[NUM_ADC_CHANNELS];
  TTree *tree_all_midas_data;

  Analyzer() {};

  virtual ~Analyzer() {};

  void Initialize() {
    DisableRootOutput(true);  // Disable the system from making it's own root files so we can do it ourselves and have more access
  };

  void BeginRun(int transition, int run, int time) {
    std::cout << "Lets get started!  ...\n";
    char root_filename[100];
    char hist_name[100];
    char hist_title[400];

    sprintf(root_filename, "%i.root", run); // Set output filename based on run #
    rootFile = new TFile(root_filename,"recreate");   // Open root file
    tree_all_midas_data = new TTree("midas_data","MIDAS data");
   // tree_all_midas_data->Branch("midas_timestamp",&midas_timestamp,"midas_timestamp/I");
   // tree_all_midas_data->Branch("midas_serialnumber",&midas_serialnumber,"midas_serialnumber/I");
   // tree_all_midas_data->Branch("adc_timestamp",&adc_timestamp,"adc_timestamp/I");
    tree_all_midas_data->Branch("channel",&chan,"chan/I");
    tree_all_midas_data->Branch("adc_value",&adc_value,"adc_value/I");
//   // tree_all_midas_data->Branch("tdc_value",&tdc_value,"tdc_value/I");
   // tree_all_midas_data->Branch("flag",&event_flag,"event_flag/I");

    for(int my_channel = 0; my_channel<NUM_ADC_CHANNELS; my_channel++) {  // Loop over all the channels creating a directory for each
      sprintf(hist_title, "ADC values for channel: %i", my_channel);
      sprintf(hist_name, "adc_chan_%i", my_channel);
      adc_per_channel_histograms[my_channel] = new TH1F(hist_name, hist_title, ADC_CHAN, 0, 33527390); // 67108863
    }

  };

  bool ProcessMidasEvent(TDataContainer& dataContainer) {
    int event_pos = 0;

    midas_serialnumber = dataContainer.GetMidasEvent().GetSerialNumber();
    midas_timestamp = dataContainer.GetMidasEvent().GetTimeStamp();

   // std::cout << "Midias Serial # : " << midas_serialnumber << "\n";
   // std::cout << "Midias timestamp # : " << midas_timestamp << "\n";

    TGRIF16Data *data = dataContainer.GetEventData<TGRIF16Data>("GRF4");  // I think GRF3 is the bank name and represents the format version
    if (data) {
 //     data->Print();
      std::vector<ADCGRIF16> measurements = data->GetMeasurements();
      /// Get the Vector of ADC Measurements.
   //   std::cout << "My chan : " << data->GetChannel() << "\n";
      chan = data->GetChannel();
    //  adc_timestamp = data->GetTimeStamp();
 //     std::cout << "Numer of Measurements : " << measurements.size() << "\n";

      for (unsigned int i = 0; i < measurements.size(); i++) { // loop over measurements
        if(event_pos == 4) {
        //std::cout << "My word : " << std::hex <<(int)measurements[i].GetMeasurement() << " Event_Pos : " << event_pos << "\n";

          adc_value = measurements[i].GetMeasurement();
          printf("ADC Value : %i \n", adc_value);
          if(chan <= NUM_ADC_CHANNELS) {  // Check if this is really a TDC value

            tree_all_midas_data->Fill();  // Fill the main TTree branches
            adc_per_channel_histograms[chan]->Fill(adc_value);  // Fill the histograms
          }
        }
        event_pos++;

      //  chan = measurements[i].GetChannel();
 //       adc_value = measurements[i].GetMeasurement();
  // //     if(chan >= NUM_ADC_CHANNELS) {  // Check if this is really a TDC value
  // //       std::cout << "Chan"<< chan  << " ADC " << adc_value<<"\n";
  // //       tdc_value = adc_value;
   //     if (chan < NUM_ADC_CHANNELS) {   // Just grabbing ADC events via this conditional
    //      event_flag = measurements[i].GetFlag();

//          tree_all_midas_data->Fill();  // Fill the main TTree branches
 //         adc_per_channel_histograms[chan]->Fill(adc_value);  // Fill the histograms

        }
  //}
      }
    return true;

  };

  void EndRun(int transition, int run, int time) {
    rootFile->Write();
    printf("Wrote ROOT file : %i.root\n", run);
    delete rootFile;
  }
};


int main(int argc, char *argv[])
{
  Analyzer::CreateSingleton<Analyzer>();
  return Analyzer::Get().ExecuteLoop(argc, argv);
}

