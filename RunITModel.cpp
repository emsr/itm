/*
g++ -I include -g -shared -o libitm.so src/*.cpp
g++ -I include -g -o run_itm run_itm.cpp -L. -litm
LD_LIBRARY_PATH=. ./run_itm < itm_1.in
*/

#include <iostream>
#include <iomanip>
#include <fstream>

#include <itm.h>
#include <Enums.h>

void ReadProfile(const std::string & filename, std::vector<double> & elevation);

int
main(int num, char**/*args*/)
{
  using std::cin, std::cout, std::vector, std::string, std::quoted;

  bool tls = false;
  if (num > 1)
    tls = true;

  vector<double> elevation;
  double tx_antenna_height, rx_antenna_height,
         permittivity, conductivity, refractivity,
         frequency;
  int climate, polarization;
  double confidence, reliability;
  double time, location, situation;
  double dbloss = 0.0;
  int mdvar = 12;//MDVAR__ACCIDENTAL_MODE;//MDVAR__SINGLE_MESSAGE_MODE;
  //char strmode[200] = "";

  // Read profile data.
  string filename;
  cout << "\n  Enter profile file name: ";
#if defined(__cpp_lib_quoted_string_io)
  cin >> quoted(filename);
#else
  cin >> filename;
#endif
  ReadProfile(filename, elevation);
  if (elevation.size() < 5)
    std::cerr << "\n  Error reading profile data file \"" << filename << "\".\n";

  cout << "\n  Enter transmit antenna height (>= " << 0.0 << ", <= " << 30000.0 << ") in meters: ";
  cin >> tx_antenna_height;

  cout << "\n  Enter receive antenna height (>= " << 0.0 << ", <= " << 30000.0 << ") in meters: ";
  cin >> rx_antenna_height;

  cout << "\n  Enter transmit frequency (>= " << 1.0 << ", <= " << 40000.0 << ") in MHz: ";
  cin >> frequency;

  if (tls)
  {
    cout << "\n  Enter time % (>= " << 0.0 << ", <= " << 100.0 << ") %: ";
    cin >> time;

    cout << "\n  Enter location % (>= " << 0.0 << ", <= " << 100.0 << ") %: ";
    cin >> location;

    cout << "\n  Enter situation % (>= " << 0.0 << ", <= " << 100.0 << ") %: ";
    cin >> situation;
  }
  else
  {
    cout << "\n  Enter confidence % (>= " << 0.0 << ", <= " << 100.0 << ") %: ";
    cin >> confidence;

    cout << "\n  Enter reliability % (>= " << 0.0 << ", <= " << 100.0 << ") %: ";
    cin >> reliability;
  }

  cout << "\n  Enter variability type: ";
  cin >> mdvar;

  char pol = '\0';
  cout << "\n  Enter polarization (V|H): ";
  cin >> pol;
  if ( pol == 'v' || pol == 'V' )
  {
    polarization = 1;
  }
  else if ( pol == 'h' || pol == 'H' )
  {
    polarization = 0;
  }
  else
  {
    polarization = 1;
  }

  cout << "\n  Enter relative permittivity (>= " << 0.0 << ", <= " << 100.0 << "): ";
  cin >> permittivity;

  cout << "\n  Enter conductivity (>= " << 0.00001 << ", <= " << 100.0 << ") in Siemens per meter: ";
  cin >> conductivity;

  cout << "\n  Enter refractivity (>= " << 200.0 << ", <= " << 450.0 << ") in M-units: ";
  cin >> refractivity;

  cout << "  Radio Climates:\n";
  cout << "  1) Equatorial\n";
  cout << "  2) Continental Subtropical\n";
  cout << "  3) Maritime Tropical\n";
  cout << "  4) Desert\n";
  cout << "  5) Continental Temperate\n";
  cout << "  6) Maritime Temperate, Over Land\n";
  cout << "  7) Maritime Temperate, Over Sea\n";
  cout << "  Enter Climate: ";
  cin >> climate;

  long warnings = 0;
  IntermediateValues interValues;
  int err;
  if (tls)
    err = ITM_P2P_TLS_Ex(tx_antenna_height, rx_antenna_height, elevation.data(), climate, refractivity, frequency,
                        polarization, permittivity, conductivity, mdvar, time, location, situation,
                        &dbloss, &warnings, &interValues);
  else
    err = ITM_P2P_CR_Ex(tx_antenna_height, rx_antenna_height, elevation.data(), climate, refractivity, frequency,
                        polarization, permittivity, conductivity, mdvar, confidence, reliability,
                        &dbloss, &warnings, &interValues);

  cout << "\n  ITM Output\n";
  cout << "  Loss    : " << dbloss << " dB\n";
  //cout << "  Mode    : " << strmode << '\n';
  cout << "  Errors  : " << err << '\n';
  cout << "  Warnings: " << warnings << '\n';

  cout << "  Terminal horizon angle, mrad              : " << 1000.0 * interValues.theta_hzn[0] << '\n';
  cout << "  Terminal horizon angle, mrad              : " << 1000.0 * interValues.theta_hzn[1] << '\n';
  cout << "  Terminal horizon distance, in meters      : " << interValues.d_hzn__meter[0] << '\n';
  cout << "  Terminal horizon distance, in meters      : " << interValues.d_hzn__meter[1] << '\n';
  cout << "  Terminal effective height, in meters      : " << interValues.h_e__meter[0] << '\n';
  cout << "  Terminal effective height, in meters      : " << interValues.h_e__meter[1] << '\n';
  cout << "  Surface refractivity, in N-Units          : " << interValues.N_s << '\n';
  cout << "  Terrain irregularity parameter, in meters : " << interValues.delta_h__meter << '\n';
  cout << "  Reference attenuation, in dB              : " << interValues.A_ref__db << '\n';
  cout << "  Free space basic transmission loss, in dB : " << interValues.A_fs__db << '\n';
  cout << "  Path distance, in km                      : " << interValues.d__km << '\n';
  cout << "  Mode of propagation value                 : " << interValues.mode << '\n';

  return 0;
}


//  Read a terrain profile.
void
ReadProfile(const std::string & filename,
            std::vector<double> & elevation)
{
    elevation.clear();

    std::ifstream dat(filename);
    if (dat.fail())
        return;

    double num_points = 0.0;
    dat >> num_points;
    if (num_points <= 0.0)
        return;

    double delta_dist = 0.0;
    dat >> delta_dist;
    if (delta_dist <= 0.0)
        return;

    elevation.resize(1 + 1 + 1 + static_cast<long>(num_points));
    elevation[0] = num_points;
    elevation[1] = delta_dist;

    for (long i = 0; i <= static_cast<long>(num_points); ++i)
        dat >> elevation[2 + i];
}
