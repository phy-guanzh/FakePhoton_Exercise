// -*- C++ -*-
//
// Package:    photonAna/photonAnalyzer
// Class:      photonAnalyzer
//
/**\class photonAnalyzer photonAnalyzer.cc photonAna/photonAnalyzer/plugins/photonAnalyzer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Zhe Guan
//         Created:  Sun, 12 Apr 2020 08:41:50 GMT
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
 #include "FWCore/Utilities/interface/InputTag.h"
 #include "DataFormats/TrackReco/interface/Track.h"
 #include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/PatCandidates/interface/Photon.h" //Zhe
//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<>
// This will improve performance in multithreaded jobs.


using reco::TrackCollection;

class photonAnalyzer : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit photonAnalyzer(const edm::ParameterSet&);
      ~photonAnalyzer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      // ----------member data ---------------------------
      edm::EDGetTokenT<TrackCollection> tracksToken_;  //used to select what tracks to read from configuration file
      edm::EDGetTokenT<edm::View<pat::Photon>> photonToken_;   //Zhe
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
photonAnalyzer::photonAnalyzer(const edm::ParameterSet& iConfig)
 :
 // tracksToken_(consumes<TrackCollection>(iConfig.getUntrackedParameter<edm::InputTag>("tracks")))
   photonToken_(consumes<edm::View<pat::Photon>>(iConfig.getParameter<edm::InputTag>("Photons"))) //Zhe
{
   //now do what ever initialization is needed

}


photonAnalyzer::~photonAnalyzer()
{

   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
photonAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

 //   Handle<TrackCollection> tracks;
 //   iEvent.getByToken(tracksToken_, tracks);
 //   for(TrackCollection::const_iterator itTrack = tracks->begin();
 //       itTrack != tracks->end();
 //       ++itTrack) {
      // do something with track parameters, e.g, plot the charge.
      // int charge = itTrack->charge();
      
// }

#ifdef THIS_IS_AN_EVENT_EXAMPLE
   Handle<ExampleData> pIn;
   iEvent.getByLabel("example",pIn);
#endif

#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
   ESHandle<SetupData> pSetup;
   iSetup.get<SetupRecord>().get(pSetup);
#endif
    Handle<edm::View<pat::Photon>> photons;
    iEvent.getByToken(photonToken_,photons);
    std::cout<<"photon size :"<<photons->size()<<std::endl;
    for (size_t ip=0; ip<photons->size();ip++){
	std::cout<<"reco photon pT:" <<(*photons)[ip].pt()<<std::endl;
    }
}




// ------------ method called once each job just before starting event loop  ------------
void
photonAnalyzer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void
photonAnalyzer::endJob()
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
photonAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);

  //Specify that only 'tracks' is allowed
  //To use, remove the default given above and uncomment below
  //ParameterSetDescription desc;
  //desc.addUntracked<edm::InputTag>("tracks","ctfWithMaterialTracks");
  //descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(photonAnalyzer);
