# SpectrumId
# Nuclids identification software for high resolution gamma-ray spectra

## Smoothing
The first step for spectra pocessing is smoothing. It's very usefull for cases when we have emission.
On this test data from root tutorial we could see that Markov smoothing allows to avoid lost of data, so it's looks preferable for usage. 
![Smoothing Comparison](https://pp.userapi.com/c855436/v855436189/a31c/MAvmU4q16-g.jpg)

The next investigation will confirm this suggestion.

We could try to test smoothing of simple arrays:

**For x^2 array with pol2 curve:**

|Orig vector|Zlokazov|TH1::SmoothArray|TSpectrum::SmoothMarkov|
|---|---|---|---|---|
|0.000|-0.000|0.000|27.124|
1.000   |   1.000   |   1.000   |   29.766
4.000   |   4.000   |   4.000   |   33.987
9.000   |   9.000   |   9.000   |   39.626   
16.000   |   16.000   |   16.000   |   46.673   
25.000   |   25.000   |   25.000   |   80.935   
36.000   |   36.000   |   38.812   |   140.971   
**1049.000**   |   **49.000**   |   **57.625**   |   **592.777**   
64.000   |   64.000   |   71.875   |   170.713   
81.000   |   81.000   |   82.125   |   130.876   
100.000   |   100.000   |   99.188   |   99.398   
121.000   |   121.000   |   121.000   |   113.156   

**For x^2 array with pol4 curve:**
|Orig vector|Zlokazov|TH1::SmoothArray|TSpectrum::SmoothMarkov|
|---|---|---|---|---|
0.000   |   -0.287   |   0.000   |   27.124   
1.000   |   1.000   |   1.000   |   29.766   
4.000   |   4.000   |   4.000   |   33.987   
9.000   |   9.000   |   9.000   |   39.626   
16.000   |   16.000   |   16.000   |   46.673   
25.000   |   -93.497   |   25.000   |   80.935   
36.000   |   1052.533   |   38.812   |   140.971   
**1049.000**   |   **502.245**   |   **57.625**   |  **592.777**
64.000   |   184.515   |   71.875   |   170.713   
81.000   |   70.899   |   82.125   |   130.876   
100.000   |   100.000   |   99.188   |   99.398   
121.000   |   93.044   |   121.000   |   113.156   

**For x^4 array with pol2 curve:**
|Orig vector|Zlokazov|TH1::SmoothArray|TSpectrum::SmoothMarkov|
|---|---|---|---|---|
0.000   |   0.000   |   0.000   |   140.124   
1.000   |   61.000   |   0.000   |   146.897   
16.000   |   16.000   |   14.500   |   164.159   
81.000   |   81.000   |   79.500   |   199.214   
256.000   |   256.000   |   254.500   |   265.132   
625.000   |   625.000   |   631.750   |   401.763   
1296.000   |   1296.000   |   1537.000   |   671.002   
**3401.000**   |   **3401.000**   |   **2847.000**   |   **1356.895**   
4096.000   |   4096.000   |   4295.000   |   2317.406   
6561.000   |   6561.000   |   6561.000   |   4856.866   
10000.000   |   10000.000   |   10085.500   |   10254.420   
14641.000   |   14641.000   |   14511.500   |   20200.123   


**For x^4 array with pol4 curve:**
|Orig vector|Zlokazov|TH1::SmoothArray|TSpectrum::SmoothMarkov|
|---|---|---|---|---|
0.000   |   -0.683   |   0.000   |   140.124   
1.000   |   3.591   |   0.000   |   146.897   
16.000   |   16.000   |   14.500   |   164.159   
81.000   |   81.000   |   79.500   |   199.214   
256.000   |   256.000   |   254.500   |   265.132   
625.000   |   503.241   |   631.750   |   401.763   
1296.000   |   1771.440   |   1537.000   |   671.002   
**3401.000**   |   **2801.271**   |   **2847.000**   |   **1356.895**   
4096.000   |   4297.650   |   4295.000   |   2317.406   
6561.000   |   6622.876   |   6561.000   |   4856.866   
10000.000   |   10000.000   |   10085.500   |   10254.420   
14641.000   |   14641.000   |   14511.500   |   20200.123  

**For N(0,1) array with pol2 curve:**
|Orig vector|Zlokazov|TH1::SmoothArray|TSpectrum::SmoothMarkov|
|---|---|---|---|---|
0.999   |   0.999   |   0.999   |   41.334   
-0.435   |   -0.435   |   0.836   |   37.665   
0.782   |   0.782   |   0.572   |   40.050   
-0.030   |   -0.030   |   0.153   |   38.455   
0.824   |   -7550.210   |   -0.057   |   40.076   
-0.057   |   -0.057   |   -0.057   |   61.239   
-0.901   |   -0.901   |   -0.034   |   96.192   
**999.925**   |   **0.006**   |   **0.006**   |   **411.713**   
0.008   |   0.275   |   -0.022   |   96.371   
-0.411   |   -0.411   |   -0.171   |   61.994   
1.391   |   1.391   |   -0.487   |   41.032   
-0.985   |   -0.985   |   -0.985   |   34.990 


Let's generate Gaus distribution random value and add to it noise and emissions. Then we applied our smoothers to it:
![](https://pp.userapi.com/c846321/v846321487/1eb31e/DKzXXLAxVJA.jpg)


This picture let us to understand one of important detail:

>**Markov smoothing doesn't eliminate peak, it decrease amplitude of emission, but increase amplitude of peak and allow to avoid the lost of the data. In such case key feature of peak is FHFW.**

*Zlokazov smoothing in this case just repeat the data.*

## Background