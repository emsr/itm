I tried to rewrite the ITS in Linux-friendly way.
Gave up for lack of time.
Probably not hard.

I think 
  int ITM_P2P_CR(double h_tx__meter, double h_rx__meter, double pfl[], int climate, double N_0, double f__mhz,
      int pol, double epsilon, double sigma, int mdvar, double confidence, double reliability,
      double *A__db, long *warnings)
is the analog of what I run over on ITM.

Anyway,
g++ -shared -o poop.so src/*.cpp
seems to make a library.

Try making a runner.
