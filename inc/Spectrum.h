#ifndef SPECTRUM_H
#define SPECTRUM_H

 /* Copyright(c) 2018, REGATA Experiment at JINR, All rights reserved. *
 * See cxx source for full Copyright notice                               */
 /*!
 * \brief Class for identification of peaks from genie2000 cnf files.
 *
 * \author <a href="mailto:bdrum@jinr.ru">Boris Rumyantsev</a>
 */

class Spectrum {
  private:

  public:

    Spectrum(const *char cnfFileName);
    Spectrum(std::string_view cnfFileName);

    ClassDef(Spectrum, 1);
};

#endif
