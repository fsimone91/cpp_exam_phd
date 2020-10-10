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
        : pt(inpt), eta(ineta), phi(inphi), en(inen)
    {}

    double mass();
    double px() const { return (pt * cos(phi )); }
    double py() const { return (pt * sin(phi )); }
    double pz() const { return (pt * sinh(eta)); }
    void print();

    // Overload += operator to sum up tracks.
    Muon& operator+=(const Muon& rmuon);

    //helper function to support class Muon
    bool isEqual(const Muon&, const Muon&);

private:
    double pt {0};
    double eta{0};
    double phi{0};
    double en {0};

};


class Event
{
public:
    Event(){};

    Event(int inevt, int inlumi, int inrun)
        : evt(inevt), lumi(inlumi), run(inrun)
    {}

    // Overload == operator to compare events
    //bool operator==(const Event);

private:
    int evt {0};
    int lumi{0};
    int run {0};
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
