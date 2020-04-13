// -*- C++ -*-
//
// Package:    photonAna/photonAnalyzer2
// Class:      photonAnalyzer2
//
/**\class photonAnalyzer2 photonAnalyzer2.cc photonAna/photonAnalyzer2/plugins/photonAnalyzer2.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Zhe Guan
//         Created:  Mon, 13 Apr 2020 03:05:24 GMT
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
#include "DataFormats/PatCandidates/interface/Photon.h"
//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<>
// This will improve performance in multithreaded jobs.


using reco::TrackCollection;

class photonAnalyzer2 : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit photonAnalyzer2(const edm::ParameterSet&);
      ~photonAnalyzer2();

     static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
     void findFirstNonPhotonMother(const reco::Candidate *particle, int &motherid);

   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      // ----------member data ---------------------------
      edm::EDGetTokenT<TrackCollection> tracksToken_;  //used to select what tracks to read from configuration file
	edm::EDGetTokenT<edm::View<pat::Photon>> photonToken_;
	edm::EDGetTokenT<edm::View<reco::GenParticle>> genSrc_;
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
photonAnalyzer2::photonAnalyzer2(const edm::ParameterSet& iConfig)
 :
//  tracksToken_(consumes<TrackCollection>(iConfig.getUntrackedParameter<edm::InputTag>("tracks")))
    photonToken_(consumes<edm::View<pat::Photon>>(iConfig.getParameter<edm::InputTag>("Photons"))),
    genSrc_(consumes<edm::View<reco::GenParticle>>(iConfig.getParameter<edm::InputTag>("genSrc")))

{
   //now do what ever initialization is needed

}


photonAnalyzer2::~photonAnalyzer2()
{

   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
photonAnalyzer2::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

//    Handle<TrackCollection> tracks;
//    iEvent.getByToken(tracksToken_, tracks);
//    for(TrackCollection::const_iterator itTrack = tracks->begin();
//        itTrack != tracks->end();
//        ++itTrack) {
      // do something with track parameters, e.g, plot the charge.
      // int charge = itTrack->charge();
//    }

#ifdef THIS_IS_AN_EVENT_EXAMPLE
   Handle<ExampleData> pIn;
   iEvent.getByLabel("example",pIn);
#endif

#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
   ESHandle<SetupData> pSetup;
   iSetup.get<SetupRecord>().get(pSetup);
#endif
   edm::Handle<edm::View<reco::GenParticle> > genParticles; 
   iEvent.getByToken(genSrc_, genParticles);
   for(size_t i=0; i<genParticles->size();i++){
	int motherid=-999;
	const reco::Candidate *particle = &(*genParticles)[i];
	if( abs(particle->pdgId())== 22 && particle->status()== 1){
	findFirstNonPhotonMother(particle,motherid);
	std::cout<<"gen photon:"<<particle->pt()<<", photon mother:"<<motherid<<std::endl;}
   }

   Handle<edm::View<pat::Photon>> photons;
   iEvent.getByToken(photonToken_, photons);
   std::cout<<"photon size:"<<photons->size()<<std::endl;
   for(size_t ip=0; ip<photons->size();ip++){
	std::cout<<"reco:"<<(*photons)[ip].pt()<<std::endl;
	const auto pho = photons->ptrAt(ip);
	if(pho->genPhoton()) {
		std::cout<<pho->genPhoton()->pt()<<std::endl;
	}
}
}

// ------------ method called once each job just before starting event loop  ------------
void photonAnalyzer2::findFirstNonPhotonMother(const reco::Candidate *particle, int &motherid)
{
	if(abs(particle->pdgId()) == 22) findFirstNonPhotonMother(particle->mother(0), motherid);
	else {motherid=particle->pdgId();}
	return;
}

void
photonAnalyzer2::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void
photonAnalyzer2::endJob()
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
photonAnalyzer2::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
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
DEFINE_FWK_MODULE(photonAnalyzer2);
