// -*- C++ -*-
//
// Package:    HGCal/RecHitPlotter_LowGain_New
// Class:      RecHitPlotter_LowGain_New
//
/**\class RecHitPlotter_LowGain_New RecHitPlotter_LowGain_New.cc HGCal/RecHitPlotter_LowGain_New/plugins/RecHitPlotter_LowGain_New.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Rajdeep Mohan Chatterjee
//         Created:  Mon, 15 Feb 2016 09:47:43 GMT
//
//


// system include files
#include <memory>
#include <iostream>
#include "TH2Poly.h"
#include "TProfile.h"
#include "TH1F.h"
// user include files
#include "FWCore/Framework/interface/ESHandle.h" 
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "HGCal/DataFormats/interface/HGCalTBRecHitCollections.h"
#include "HGCal/DataFormats/interface/HGCalTBDetId.h"
#include "HGCal/DataFormats/interface/HGCalTBRecHit.h"
#include "HGCal/Geometry/interface/HGCalTBCellVertices.h"
#include "HGCal/Geometry/interface/HGCalTBTopology.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "HGCal/CondObjects/interface/HGCalCondObjects.h"
#include "HGCal/CondObjects/interface/HGCalCondObjectTextIO.h"
#include "HGCal/CondObjects/interface/HGCalTBNumberingScheme.h"

//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<> and also remove the line from
// constructor "usesResource("TFileService");"
// This will improve performance in multithreaded jobs.

static const double delta = 0.00001;//Add/subtract delta = 0.00001 to x,y of a cell centre so the TH2Poly::Fill doesnt have a problem at the edges where the centre of a half-hex cell passes through the sennsor boundary line.

class RecHitPlotter_LowGain_New : public edm::one::EDAnalyzer<edm::one::SharedResources>
{

public:
	explicit RecHitPlotter_LowGain_New(const edm::ParameterSet&);
	~RecHitPlotter_LowGain_New();
	static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
	virtual void beginJob() override;
	void analyze(const edm::Event& , const edm::EventSetup&) override;
	virtual void endJob() override;

	// ----------member data ---------------------------
	edm::EDGetToken HGCalTBRecHitCollection_;
	HGCalTBTopology IsCellValid;
	HGCalTBCellVertices TheCell;
	int sensorsize = 128;// The geometry for a 256 cell sensor hasnt been implemted yet. Need a picture to do this.
	std::vector<std::pair<double, double>> CellXY;
	std::pair<double, double> CellCentreXY;
	std::vector<std::pair<double, double>>::const_iterator it;
	const static int NLAYERS  = 1;
	TH2Poly *h_RecHit_layer[NLAYERS][5000];
	const static int cellx = 15;
	const static int celly = 15;
	int Sensor_Iu = 0;
	int Sensor_Iv = 0;
        TH1F    *h_RecHit_layer_summed[NLAYERS];
	TH1F  *h_RecHit_layer_cell[NLAYERS][cellx][celly];
        TH1F* Sum_Cluster_ADC;
        TProfile* SKI1_Ped_Event;
        TProfile* SKI2_Ped_Event;
        TH1F* CG_X;
        TH1F* CG_Y;
	char name[50], title[50];
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
RecHitPlotter_LowGain_New::RecHitPlotter_LowGain_New(const edm::ParameterSet& iConfig)
{
	//now do what ever initialization is needed
	usesResource("TFileService");
	edm::Service<TFileService> fs;
	HGCalTBRecHitCollection_ = consumes<HGCalTBRecHitCollection>(iConfig.getParameter<edm::InputTag>("HGCALTBRECHITS"));

//Booking 2 "hexagonal" histograms to display the sum of Rechits and the Occupancy(Hit > 5 GeV) in 1 sensor in 1 layer. To include all layers soon. Also the 1D Rechits per cell in a sensor is booked here.
	const int HalfHexVertices = 4;
	double HalfHexX[HalfHexVertices] = {0.};
	double HalfHexY[HalfHexVertices] = {0.};
	const int FullHexVertices = 6;
	double FullHexX[FullHexVertices] = {0.};
	double FullHexY[FullHexVertices] = {0.};
	int iii = 0;
        CG_X = fs->make<TH1F>("CG_X","CG X[cm]", 100,-5,5);
        CG_Y = fs->make<TH1F>("CG_Y","CG Y[cm]", 100,-5,5);
        SKI1_Ped_Event = fs->make<TProfile>("SKI1_Ped_Event","Profile per event of pedestal for SKI1",5000,1,5000,0,400);
        SKI2_Ped_Event = fs->make<TProfile>("SKI2_Ped_Event","Profile per event of pedestal for SKI2",5000,1,5000,0,400);
        Sum_Cluster_ADC = fs->make<TH1F>("Sum_Cluster_ADC","Sum_Cluster_ADC", 200, -100., 100.);
	for(int nlayers = 0; nlayers < NLAYERS; nlayers++) {
                sprintf(name, "FullLayer_RecHits_Layer%i_Summed", nlayers + 1);
                sprintf(title, "Sum of RecHits Layer%i Summed over the cells", nlayers + 1);
                h_RecHit_layer_summed[nlayers] = fs->make<TH1F>(name, title, 200, -100., 100.);
                h_RecHit_layer_summed[nlayers]->GetXaxis()->SetTitle("RecHits[GeV]");
            for(int eee = 0; eee< 5000; eee++){ 
		sprintf(name, "FullLayer_RecHits_Layer%i_Event%i", nlayers + 1,eee);
		sprintf(title, "RecHits Layer%i Event%i", nlayers + 1,eee);
		h_RecHit_layer[nlayers][eee] = fs->make<TH2Poly>();
		h_RecHit_layer[nlayers][eee]->SetName(name);
		h_RecHit_layer[nlayers][eee]->SetTitle(title);
//                h_RecHit_layer[nlayers][eee]->SetMinimum(-10.);
//                h_RecHit_layer[nlayers][eee]->SetMaximum(10.);
		for(int iv = -7; iv < 8; iv++) {
			for(int iu = -7; iu < 8; iu++) {
				if(!IsCellValid.iu_iv_valid(nlayers, Sensor_Iu, Sensor_Iv, iu, iv, sensorsize)) continue;
//Some thought needs to be put in about the binning and limits of this 1D histogram, probably different for beam type Fermilab and cern.
				CellXY = TheCell.GetCellCoordinatesForPlots(nlayers, Sensor_Iu, Sensor_Iv, iu, iv, sensorsize);
				int NumberOfCellVertices = CellXY.size();
				iii = 0;
				if(NumberOfCellVertices == 4) {
					for(it = CellXY.begin(); it != CellXY.end(); it++) {
						HalfHexX[iii] =  it->first;
						HalfHexY[iii++] =  it->second;
					}
//Somehow cloning of the TH2Poly was not working. Need to look at it. Currently physically booked another one.
					h_RecHit_layer[nlayers][eee]->AddBin(NumberOfCellVertices, HalfHexX, HalfHexY);
				} else if(NumberOfCellVertices == 6) {
					iii = 0;
					for(it = CellXY.begin(); it != CellXY.end(); it++) {
						FullHexX[iii] =  it->first;
						FullHexY[iii++] =  it->second;
					}
					h_RecHit_layer[nlayers][eee]->AddBin(NumberOfCellVertices, FullHexX, FullHexY);
				}

			}
		}
           }// loop over events ends here
	}//loop over layers end here


}//contructor ends here


RecHitPlotter_LowGain_New::~RecHitPlotter_LowGain_New()
{

	// do anything here that needs to be done at desctruction time
	// (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
RecHitPlotter_LowGain_New::analyze(const edm::Event& event, const edm::EventSetup& setup)
{

	using namespace edm;
	using namespace std;

	edm::Handle<HGCalTBRecHitCollection> Rechits;
	event.getByToken(HGCalTBRecHitCollection_, Rechits);
        edm::Handle<HGCalTBRecHitCollection> Rechits1;
        event.getByToken(HGCalTBRecHitCollection_, Rechits1);

#ifdef ESPRODUCER
// the conditions should be defined by ESProduers and available in the iSetup
 edm::ESHandle<HGCalCondPedestals> pedestalsHandle;
 HGCalCondPedestals pedestals = setup.get<HGCalCondPedestals>();
#else
HGCalCondPedestals pedestals;
HGCalCondObjectTextIO condIO(HGCalTBNumberingScheme::scheme());
assert(condIO.load("Ped_LowGain_ByChannelNo_8272_RMS.txt", pedestals));
#endif
        double Average_Pedestal_Per_Event1_Full = 0, Average_Pedestal_Per_Event1_Half = 0, Average_Pedestal_Per_Event2_Full = 0, Average_Pedestal_Per_Event2_Half = 0;
        int Cell_counter1_Full = 0, Cell_counter2_Full = 0, Cell_counter1_Half = 0, Cell_counter2_Half = 0;   
        double iux_Max=0., iyy_Max=0.;
        int ADC_TMP=0;
//        int Sum_Cluster_Tmp;
//        double iux_CG=0., iyy_CG=0.;
//        int CG_counter=0;


        for(auto RecHit1 : *Rechits1) {
             CellCentreXY = TheCell.GetCellCentreCoordinatesForPlots((RecHit1.id()).layer(), (RecHit1.id()).sensorIU(), (RecHit1.id()).sensorIV(), (RecHit1.id()).iu(), (RecHit1.id()).iv(), sensorsize);
             double iux = (CellCentreXY.first < 0 ) ? (CellCentreXY.first + delta) : (CellCentreXY.first - delta) ;
             double iyy = (CellCentreXY.second < 0 ) ? (CellCentreXY.second + delta) : (CellCentreXY.second - delta);
//             cout<<endl<<" Cell Type = "<<(RecHit1.id()).cellType()<<endl;

             if((RecHit1.energy() > ADC_TMP) && ((RecHit1.id()).cellType() == 0) && (RecHit1.energy() < 20)){
                 ADC_TMP = RecHit1.energy();
                 iux_Max = iux;
                 iyy_Max = iyy;
               } 

             if(!((RecHit1.id()).iu() == -1 && (RecHit1.id()).iv() == 2) && !((RecHit1.id()).iu() == 2 && (RecHit1.id()).iv() == -4) && (RecHit1.energy() < 20)){

                  if((iux <= -0.25 && iux >= -3.25) && (iyy <= -0.25 && iyy>= -5.25 ) && (RecHit1.id()).cellType() == 0){
                       Cell_counter1_Full++;
                       Average_Pedestal_Per_Event1_Full += RecHit1.energy();
                     }

                  if((iux >= 0.25 && iux <= 3.25) && (iyy <= -0.25 && iyy>= -5.25 ) && (RecHit1.id()).cellType() == 0){
                       Cell_counter2_Full++;
                      Average_Pedestal_Per_Event2_Full += RecHit1.energy();
                                  }

                  if((RecHit1.id()).cellType() == 2 || (RecHit1.id()).cellType() == 3 || (RecHit1.id()).cellType() == 5 ){
                      if((iux <= 0.25 && iyy>= -0.25 ) || (iux < -0.5) ){
                          Cell_counter1_Half++;
                          Average_Pedestal_Per_Event1_Half += RecHit1.energy();
                        }
                      if((iux >= 0.25 && iyy <= -0.5) || (iux>= 0.5) ){
                          Cell_counter2_Half++;
                          Average_Pedestal_Per_Event2_Half += RecHit1.energy();
                         }
                    }
               }
           }
CG_X->Fill(iux_Max);
CG_Y->Fill(iyy_Max);
Sum_Cluster_ADC->Fill(ADC_TMP);
	for(auto RecHit : *Rechits) {
		if(!IsCellValid.iu_iv_valid((RecHit.id()).layer(), (RecHit.id()).sensorIU(), (RecHit.id()).sensorIV(), (RecHit.id()).iu(), (RecHit.id()).iv(), sensorsize))  continue;
		int n_layer = (RecHit.id()).layer();
                CellCentreXY = TheCell.GetCellCentreCoordinatesForPlots((RecHit.id()).layer(), (RecHit.id()).sensorIU(), (RecHit.id()).sensorIV(), (RecHit.id()).iu(), (RecHit.id()).iv(), sensorsize);
                double iux = (CellCentreXY.first < 0 ) ? (CellCentreXY.first + delta) : (CellCentreXY.first - delta) ;
                double iyy = (CellCentreXY.second < 0 ) ? (CellCentreXY.second + delta) : (CellCentreXY.second - delta);
                if((RecHit.id()).cellType() !=4){ 
//                          h_RecHit_layer[n_layer - 1][event.id().event()]->Fill(iux , iyy, RecHit.energy());
                          if(event.id().event() == 1)  h_RecHit_layer[n_layer - 1][1]->Fill(iux , iyy, pedestals.get(RecHit.id())->value);
                  }    
//                          h_RecHit_layer_summed[n_layer - 1]->Fill(RecHit.energy() - (Average_Pedestal_Per_Event2_Half/Cell_counter2_Half)); 
//                      }

	}


}//analyze method ends here


// ------------ method called once each job just before starting event loop  ------------
void
RecHitPlotter_LowGain_New::beginJob()
{

}

// ------------ method called once each job just after ending the event loop  ------------
void
RecHitPlotter_LowGain_New::endJob()
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
RecHitPlotter_LowGain_New::fillDescriptions(edm::ConfigurationDescriptions& descriptions)
{
	//The following says we do not know what parameters are allowed so do no validation
	// Please change this to state exactly what you do use, even if it is no parameters
	edm::ParameterSetDescription desc;
	desc.setUnknown();
	descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(RecHitPlotter_LowGain_New);
