//
// This class contains the secondary correction and the double hit
// correction used in low-flux events.
//
#ifndef ALIFMDCORRMERGINGEFFICIENCY_H
#define ALIFMDCORRMERGINGEFFICIENCY_H
/**
 * @file   AliFMDCorrMergingEfficiency.h
 * @author Christian Holm Christensen <cholm@dalsgaard.hehi.nbi.dk>
 * @date   Wed Mar 23 14:01:28 2011
 * 
 * @brief  
 * 
 * 
 * @ingroup pwglf_forward_corr
 */
#include <TObject.h>
#include <TObjArray.h>
#include <TAxis.h>
class TH1D;

/**
 * This class contains the merging efficiency correction.
 *
 * The secondary correction is given by 
 * @f[
 *   m_{r,v}(\eta) = 
 * @f]
 *
 * These are generated from Monte-Carlo truth and ESD information. 
 *
 * @ingroup pwglf_forward_corr
 */
class AliFMDCorrMergingEfficiency : public TObject 
{
public:
  /** 
   * Default constructor 
   */
  AliFMDCorrMergingEfficiency();
  /** 
   * Copy constructor 
   * 
   * @param o Object to copy from 
   */
  AliFMDCorrMergingEfficiency(const AliFMDCorrMergingEfficiency& o);
  /**
   * Destructor 
   * 
   */
  virtual ~AliFMDCorrMergingEfficiency();
  /** 
   * @{ 
   * @name Get corrections and parameters 
   */
  /** 
   * Assignment operator 
   * 
   * @param o Object to assign from 
   * 
   * @return Reference to this object 
   */
  AliFMDCorrMergingEfficiency& operator=(const AliFMDCorrMergingEfficiency& o);
  /** 
   * Get the secondary correction @f$ c_{r,v}@f$ 
   *
   * @param d  Detector number (1-3)
   * @param r  Ring identifier (I or O)
   * @param v  Primary interaction point @f$z@f$ coordinate
   * 
   * @return The correction @f$ c_{r,v}@f$ 
   */
  TH1D* GetCorrection(UShort_t d, Char_t r, Double_t v) const;
  /** 
   * Get the secondary correction @f$ c_{r,v}@f$ 
   *
   * @param d  Detector number (1-3)
   * @param r  Ring identifier (I or O)
   * @param b  Bin corresponding to the primary interaction point 
   *           @f$z@f$ coordinate (1 based)
   * 
   * @return The correction @f$ c_{r,v}@f$ 
   */
  TH1D* GetCorrection(UShort_t d, Char_t r, UShort_t b) const;
  /** 
   * Get the vertex axis used 
   * 
   * @return vertex axis 
   */
  const TAxis& GetVertexAxis() const { return fVertexAxis; }
  /* @} */

  /** 
   * @{ 
   * @name Set corrections and parameters 
   */
  /** 
   * Set the secondary map correction @f$ m_{r,v}(\eta)@f$.
   * Note, that the object takes ownership of the passed pointer.
   * 
   * @param d    Detector number (1-3)
   * @param r    Ring identifier (I or O)
   * @param v    Primary interaction point @f$z@f$ coordinate  
   * @param h    @f$ m_{r,v}(\eta)@f$ 
   * 
   * @return true if operation succeeded 
   */
  Bool_t SetCorrection(UShort_t d, Char_t r, Double_t v, TH1D* h);
  /** 
   * Set the secondary map correction @f$ m_{r,v}(\eta)@f$ 
   * Note, that the object takes ownership of the passed pointer.
   * 
   * @param d    Detector number (1-3)
   * @param r    Ring identifier (I or O)
   * @param b    Bin corresponding to the primary interaction point 
   *             @f$z@f$ coordinate  (1 based)
   * @param h    @f$ m_{r,v}(\eta)@f$ 
   * 
   * @return true if operation succeeded 
   */
  Bool_t SetCorrection(UShort_t d, Char_t r, UShort_t b, TH1D* h);
  /** 
   * Set the vertex axis to use 
   * 
   * @param axis Vertex axis 
   */
  void SetVertexAxis(const TAxis& axis);
  /** 
   * Set the vertex axis to use 
   * 
   * @param nBins   Number of bins
   * @param min     Minimum
   * @param max     Maximum
   */
  void SetVertexAxis(Int_t nBins, Double_t min, Double_t max);
  /* @} */

  /** 
   * @{ 
   * @name Auxiliary member functions 
   */
  /** 
   * Declare this as a folder
   * 
   * @return Always true 
   */
  Bool_t IsFolder() const { return true; }
  /** 
   * Browse this object in the browser
   * 
   * @param b 
   */
  void Browse(TBrowser* b);
  /** 
   * Print this object 
   * 
   * @param option 
   */  
  void Print(Option_t* option="R") const; //*MENU*
  /* @} */
protected:
  /** 
   * Find the vertex bin that corresponds to the passed vertex 
   * 
   * @param vertex The interaction points @f$z@f$-coordinate 
   * 
   * @return Vertex bin in @f$[1,N_{\mbox{vertex}}]@f$ or negative if 
   * out of range 
   */
  Int_t FindVertexBin(Double_t vertex) const;
  /** 
   * Get the index corresponding to the given ring 
   * 
   * @param d Detector
   * @param r Ring 
   * 
   * @return Index (0 based) or negative in case of errors
   */
  Int_t GetRingIndex(UShort_t d, Char_t r) const;
  /** 
   * Get the ring array corresponding to the specified ring
   * 
   * @param d Detector 
   * @param r Ring 
   * 
   * @return Pointer to ring array, or null in case of problems
   */
  TObjArray* GetRingArray(UShort_t d, Char_t r) const;
  /** 
   * Get the ring array corresponding to the specified ring
   * 
   * @param d Detector 
   * @param r Ring 
   * 
   * @return Pointer to ring array, or newly created container 
   */
  TObjArray* GetOrMakeRingArray(UShort_t d, Char_t r);

  TObjArray fRingArray;      // Array of per-ring, per-vertex 2nd map
  TAxis     fVertexAxis;     // The vertex axis 
  ClassDef(AliFMDCorrMergingEfficiency,1); // 
};

//____________________________________________________________________
inline void 
AliFMDCorrMergingEfficiency::SetVertexAxis(Int_t nBins, Double_t min, 
					   Double_t max)
{
  fVertexAxis.Set(nBins, min, max);
}
//____________________________________________________________________
inline void 
AliFMDCorrMergingEfficiency::SetVertexAxis(const TAxis& e)
{
  fVertexAxis.Set(e.GetNbins(), e.GetXmin(), e.GetXmax());
}
#endif
// Local Variables:
//  mode: C++
// End:
