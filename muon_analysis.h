#include <fstream>
#include <string>
#include <vector>

class csv_reader
{
public:
  csv_reader(){};

  int open(const std::string &file_name);
  void read_file(int header);
  std::vector<std::string> read_row();

private:
  std::ifstream infile;
};


class Muon
{
public:
    Muon(){};

    Muon(double inpt, double ineta, double inphi, double inen)
        : pt_(inpt), eta_(ineta), phi_(inphi), en_(inen)
    {}

    Muon(const Muon & m){
        pt_  = m.pt_;
        eta_ = m.eta_;
        phi_ = m.phi_;
        en_  = m.en_;
    }

    double pt()  const { return pt_; }
    double eta() const { return eta_; }
    double phi() const { return phi_; }
    double en()  const { return en_; }
    double mass();
    double px() const { return (pt_ * cos(phi_ )); }
    double py() const { return (pt_ * sin(phi_ )); }
    double pz() const { return (pt_ * sinh(eta_)); }
    void print();

    // Overload += operator to sum up tracks.
    Muon& operator+=(const Muon& rmuon);

    bool isEqual(const Muon& m);

private:
    double pt_ {0};
    double eta_{0};
    double phi_{0};
    double en_ {0};

};

//Sum of muons implemented as helper function
Muon operator+(const Muon& m1, const Muon& m2);

//Comparison between muons implemented as helper function
bool operator==(const Muon& m1, const Muon& m2);

class Event
{
public:
    Event(){};

    Event(int inevt, int inlumi, int inrun)
        : evt_(inevt), lumi_(inlumi), run_(inrun)
    {}

    int evt()  const { return evt_; }
    int lumi() const { return lumi_; }
    int run()  const { return run_; }
    // Overload == operator to compare events
    //bool operator==(const Event);

private:
    int evt_ {0};
    int lumi_{0};
    int run_ {0};
};

//constant declaration and initialization outside class
const double mass_low = 1.75, mass_high = 1.90;
const double chi2_low = 0.0, chi2_high = 50.0;

class EventCandidate
{
public:
    EventCandidate(){}

    EventCandidate(Event inevent, Muon incandidate, double inchi2)
        : event(inevent), candidate(incandidate), chi2(inchi2)
    {}

    bool isGoodEvent();
    EventCandidate analyseRow(std::vector<std::string> row);
    void print() {candidate.print();}

private:
    Event event; //call Event()
    Muon candidate; //call Muon()
    double chi2{0};
};
