# SpectrumId
### Nuclids identification software for high resolution gamma-ray spectra

#### Smoothing
The first step for spectra pocessing is smoothing. It's very usefull for cases when we have emission.
On this test data from root tutorial we could see that Markov smoothing increase peaks amplitude, that looks not good.
Anyway we need test in on more special cases.
![Smoothing Comparison](https://pp.userapi.com/c855436/v855436189/a31c/MAvmU4q16-g.jpg)


Original vector
0; 1; 4; 9; 16; 25; 36; 490; 64; 91; 100; 121; 144; 169; 196; 225;

Zlokazov smoothing suggestion:
0; 1; 4; 9; 16; 18.1386; 55.2749; 63.5742; 64; 91; 107.677; 124.711; 145.182; 169.103; 195.527; 225.218;

TH1::SmoothArray
0; 1; 4; 9; 16; 25; 39.3125; 61.9375; 82; 91; 101.188; 120.562; 144; 169; 196; 224.5;

TSpectrum::SmoothMarkov
10.591; 12.1364; 14.7395; 18.4542; 23.4504; 42.2202; 76.2627; 309.042; 108.745; 98.0899; 82.5462; 105.184; 134.182; 173.04; 218.343; 263.973;

Based on this output we could also suspect that quality of MarkovSmoothing it's not good.


Only one case left:

Special part of real spectra...
