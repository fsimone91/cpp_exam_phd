#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <math.h>
#include "muon_analysis.h"

//CSV_READER
int csv_reader::open(const std::string &file_name)
{
  infile.open(file_name.c_str());
  if (!infile)
  {
    return -1;
  }
  return 0;
}

std::vector<std::string> csv_reader::read_row(){
    std::vector<std::string> row;
    std::string column;
    std::string line_break = "\n";
    
    char c;
    while(infile.get(c)){
        if(c==','){
            row.push_back(column);
            column.clear();
        }
        else{
            column += c;
            if(column.find(line_break)!= std::string::npos) {
                row.push_back(column);
                column.clear();
                break;
            }
        }
    }
    return row;
}


//MUON
double Muon::mass(){
    double P = pt * cosh(eta); //momentum
    double M2 = pow(en,2.0) - pow(P,2.0); //4-vector magnitude
    return M2 < 0.0 ? -sqrt(-M2) : sqrt(M2);
}

void Muon::print(){
    std::cout << "pt = " << pt << ", eta = " << eta << ", phi = " << phi << ", energy " <<en << std::endl;
}

Muon& Muon::operator+=(const Muon& rmuon) {
   //sum 3-momentum components
   double px_tot = px() + rmuon.px();
   double py_tot = py() + rmuon.py();
   double pz_tot = pz() + rmuon.pz();
   //convert rapresentation from (px,py,pz) to (pt, eta, phi)
   double pt_tot = sqrt( pow(px_tot,2.0) + pow(py_tot,2.0) );
   double theta = atan2(pt_tot,pz_tot);
   double eta_tot = -log(tan(theta/2));
   double phi_tot = atan2(py_tot,px_tot);

   this->pt = pt_tot;
   this->eta = eta_tot;
   this->phi = phi_tot;
   this->en = rmuon.en+en;
   return *this;
}

bool Muon::isEqual(const Muon& m1, const Muon& m2) {
   if (m1.pt == m2.pt && m1.eta == m2.eta && m1.phi == m2.phi && m1.en == m2.en)
       return true;
   else return false;
}


//EVENTCANDIDATE
bool EventCandidate::isGoodEvent() {
    double m = candidate.mass();
    if(m < mass_high && m > mass_low){
        if(chi2 < chi2_high && chi2 > chi2_low){
            return true;
        }
        else return false;
    }
    else return false;
}

EventCandidate EventCandidate::analyseRow(std::vector<std::string> row) {
    double mupt, mueta, muphi, muen; 
    double inchi2;
    Muon incandidate;
    //File-specific!! //take muon info from row
    for(int i = 3; i<15; i+=4){
        //string to double conversion done using stod from <string> which requires std=c++11
        mupt  = std::stod(row.at(i));
        mueta = std::stod(row.at(i+1));
        muphi = std::stod(row.at(i+2));
        muen  = std::stod(row.at(i+3));

        Muon mu(mupt, mueta, muphi, muen);
        incandidate += mu; //operator += overloaded in class Muon sums up the muon momenta and energies
    }
    //File-specific!! //take event info from row
    Event inevt(std::stod(row.at(0)),  std::stod(row.at(1)), std::stod(row.at(2)));
    //File-specific!! //take 3-mu vertex from row
    inchi2 = std::stod(row.at(15));

    //Build EventCandidate to be analysed
    EventCandidate output(inevt , incandidate, inchi2);
    return output;
}


//MAIN
int main() { 

    csv_reader input_mu;
    std::vector<Muon> candidate_list;

    const std::string input_file_name("CMSdata_2016H_tau3mu_muon.csv");
    if (input_mu.open(input_file_name) < 0)
    {
        std::cout << "Cannot open file " << input_file_name.c_str() << std::endl;
        return 1;
    } else std::cout << "Reading file " << input_file_name.c_str() << std::endl;

    std::vector<std::string> row;

    int line_index = 0;
    int header_index = 0; //header position
    while(true){
        if(line_index == header_index) {
            row = input_mu.read_row();
            line_index++; //skip header
        } else {
            //read current line
            row = input_mu.read_row();
            if(!(row.size() == 0)) {
                line_index++;
            }
            else break;

            //expected 3 columns for event info, plus 3 * 4 columns for 3mu info
            if(row.size()<15) break;

            EventCandidate current_candidate = current_candidate.analyseRow(row);
            if(current_candidate.isGoodEvent()) 
                current_candidate.print();
        }
    }
    return 0;
}
