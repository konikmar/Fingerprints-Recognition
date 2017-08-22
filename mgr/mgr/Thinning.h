#ifndef _THINNING_H_
#define _THINNING_H_

#include "opencv2/opencv.hpp"
using namespace cv;

class Thinning{
public:

	void Negative(Mat& img);
	
	// Thinning 1 - ZHANG-SUEN
	void Thinning1(const Mat& src, Mat& dst);

	// Thinning iteration method
	void ThinningIteration(Mat& img, int iter);

	// Thinninh Negative 1 - ZHANG-SUEN
	void ThinningNegative1(const Mat& src, Mat& dst);


	// overloaded constructor
	Thinning();

	// Virtual descrutor
	~Thinning();

	// Zhang-Suen
	void ZhangSuenThinning(const Mat& src, Mat& dst);

	// Guo-Hall
	void GuoHallThinning(const Mat& src, Mat& dst);

	// Lu and Wang
	void LuWangThinning(const Mat& src, Mat& dst);

	// Enhanced Parallel Thinning Algorithm (KWK)
	void KwonWoongKangThinning(const Mat& src, Mat& dst);

	// Modified Thinning Algorithm (Zhang Wang)
	void ZhangWangThinning(const Mat& src, Mat& dst);

	// Hilditch Thinning Algorithmn (presented by Naccache and Shinghal)
	void HilditchThinning(const Mat& src, Mat& dst);

	// Arabic Parallel Thinning Algorithm
	void ArabicParallelThinning(const Mat& src, Mat& dst);

	// Efficient Parallel Thinning Algorithm (Aparajeya and Sanyal)
	void EfficientParallelThinning(const Mat& src, Mat& dst);

	// Improved Arabic Parallel Thinning Algorithm (Proposed method - should provide 8-connectivity only / delete unnecessary pixels)
	// ZAPROPONOWA£EM SWOJ¥ W£ASN¥ METODÊ - POSZERZENIE ARABIC PARALLEL O PASS2 - ZOBACZYMY MO¯E SIÊ PRZYDA, JAK TAK TO SPOKO, JAK NIE TO TRUDNO :D
	// MOIM ZDANIEM MO¯E SIÊ PRZYDAC, BO NP. NIE WYKRYWA TERAZ ROZWIDLEÑ TAM GDZIE ICH NIE MA
	// TU PORÓWNANIE (PO LEWEJ ZWYK£Y ARABIC, PO PRAWEJ NOWA):
	// https://zapodaj.net/6f470ca119036.png.html
	// https://zapodaj.net/ec10950b2a33c.png.html
	void ImprovedArabicParallelThinning(const Mat& src, Mat& dst);

	// PRZYDATNE
	// http://shodhganga.inflibnet.ac.in/bitstream/10603/3466/10/10_chapter%202.pdf
	// https://www.ancient-asia-journal.com/articles/10.5334/aa.06114/print/
	//https://www.researchgate.net/publication/316476002_Comparison_of_thinning_algorithms_for_vectorization_of_engineering_drawings
	//https://www.ancient-asia-journal.com/articles/10.5334/aa.06114/print/
	//http://shodhganga.inflibnet.ac.in/bitstream/10603/3466/10/10_chapter%202.pdf
	//https://link.springer.com/article/10.1007/s11801-008-7108-5
	//https://www.idc-online.com/technical_references/pdfs/information_technology/Enhanced%20Thinning.pdf
	//http://www.jatit.org/volumes/Vol94No2/2Vol94No2.pdf
	//http://www.ijsrp.org/research-paper-0616/ijsrp-p5425.pdf
	//http://www.arpnjournals.com/jeas/research_papers/rp_2010/jeas_0410_326.pdf

private:
	Mat fingerprint;
};

#endif