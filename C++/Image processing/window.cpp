#include <wx/wx.h>
#include "window.h"
#include <wx/image.h>
#include <wx/dcmemory.h>
#include <wx/dcclient.h>
#include <wx/dc.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
using namespace std; 

void MyFrame::OnInvertImage(wxCommandEvent & event){
lastImage = new wxImage(bitmap.ConvertToImage());
  
    printf("Inverting...");
    free(loadedImage);
    loadedImage = new wxImage(bitmap.ConvertToImage());

	    for( int i=minY;i<maxY;i++){
	       for(int j=minX;j<maxY;j++){
 	loadedImage->SetRGB(i,j,255-loadedImage->GetRed(i,j), 
				255-loadedImage->GetGreen(i,j),
				255-loadedImage->GetBlue(i,j));
    }
}

    printf(" Finished inverting.\n");
    Refresh();
}



void MyFrame::OnScaleImage(wxCommandEvent & event){
lastImage = new wxImage(bitmap.ConvertToImage());
	wxTextEntryDialog *openTextEntry = new wxTextEntryDialog ( this, _T("Scale Value"), _T(""), _T(""));  
	if(openTextEntry->ShowModal() == wxID_OK){
	double scaleAmount;
	if(openTextEntry->GetValue().ToDouble(&scaleAmount)){ 
	    printf("Scaling...");
	    lastImage = new wxImage(bitmap.ConvertToImage());
	    free(loadedImage);
	    loadedImage = new wxImage(bitmap.ConvertToImage());
            unsigned char r,g,b;
	    for( int i=minY;i<maxY;i++){
	       for(int j=minX;j<maxY;j++){
	 	r = loadedImage->GetRed(i,j);
		g = loadedImage->GetGreen(i,j);		
		b = loadedImage->GetBlue(i,j);

		r = scalePixel(r,scaleAmount);
		g = scalePixel(g,scaleAmount);
		b = scalePixel(b,scaleAmount);
		
		
		loadedImage->SetRGB(i,j,r,g,b);
	    }
	}
	    printf(" Finished scaling.\n");
	}
	}
    Refresh();
}

void MyFrame::OnShiftImage(wxCommandEvent & event){
	wxTextEntryDialog *openTextEntry = new wxTextEntryDialog ( this, _T("Shift Value"), _T(""), _T(""));  
	if(openTextEntry->ShowModal() == wxID_OK){
	int shiftAmount;
	if(shiftAmount = wxAtoi(openTextEntry->GetValue())){ 
	    printf("Shifting...");
            lastImage = new wxImage(bitmap.ConvertToImage());
	    free(loadedImage);
	    loadedImage = new wxImage(bitmap.ConvertToImage());
            unsigned char r,g,b;
	    for( int i=minY;i<maxY;i++){
	       for(int j=minX;j<maxY;j++){
	 	r = loadedImage->GetRed(i,j);
		g = loadedImage->GetGreen(i,j);		
		b = loadedImage->GetBlue(i,j);

		r = shiftPixel(r,shiftAmount);
		g = shiftPixel(g,shiftAmount);
		b = shiftPixel(b,shiftAmount);
		
		
		loadedImage->SetRGB(i,j,r,g,b);
	    }
	   }
	    printf(" Finished scaling.\n");
	}
	}
    Refresh();
}

void MyFrame::OnIterThreshImage(wxCommandEvent & event){
	    printf("Acting...");
            lastImage = new wxImage(bitmap.ConvertToImage());
	    free(loadedImage);
	    loadedImage = new wxImage(bitmap.ConvertToImage());
            unsigned char r,g,b;
	    double backRMean = 0;
	    double objRMean = 0;
	    double backGMean = 0;
	    double objGMean = 0;
	    double backBMean = 0;
	    double objBMean = 0;
	    for( int i=minY;i<maxY;i++){
	       for(int j=minX;j<maxY;j++){
	 	r = loadedImage->GetRed(i,j);
		g = loadedImage->GetGreen(i,j);		
		b = loadedImage->GetBlue(i,j);
		int rInt = (int) r;
		int gInt = (int) g;
		int bInt = (int) b;
		if(i == 0 && j == 0 || i == imgHeight-1 && j == 0 || j ==imgWidth-1 && i ==0 || i == imgHeight-1 && j == imgWidth-1){
			backRMean = backRMean + rInt;
			backGMean = backGMean + gInt;
			backBMean = backBMean + bInt;
		}
		else{
			objRMean = objRMean + rInt;
			objGMean = objGMean + gInt;
			objBMean = objBMean + bInt;
		}
		
	    }
	   }
	backRMean = backRMean/4;
	backGMean = backGMean/4;
	backBMean = backBMean/4;
	int sum = imgHeight*imgWidth-4;
	objRMean = objRMean/sum;
	objGMean = objGMean/sum;
	objBMean = objBMean/sum;

	int threshR = ((backRMean+objRMean)/2) + 0.5;
	int threshG = ((backGMean+objGMean)/2) + 0.5;
	int threshB = ((backBMean+objBMean)/2) + 0.5;


	    for( int i=minY;i<maxY;i++){
	       for(int j=minX;j<maxY;j++){
	 	r = loadedImage->GetRed(i,j);
		g = loadedImage->GetGreen(i,j);		
		b = loadedImage->GetBlue(i,j);
		int rInt = (int) r;
		int gInt = (int) g;
		int bInt = (int) b;
		if(rInt< threshR){
			rInt = 255;		
		}
		if(gInt< threshG){
			gInt = 255;		
		}
		if(bInt< threshB){
			bInt = 255;		
		}
		r = (unsigned char) rInt;
		g = (unsigned char) gInt;
		b = (unsigned char) bInt;
		loadedImage->SetRGB(i,j,r,g,b);
	    }
	   }

	
    Refresh();
}

void MyFrame::OnThreshImage(wxCommandEvent & event){
	wxTextEntryDialog *openTextEntry = new wxTextEntryDialog ( this, _T("Threshold Value"), _T(""), _T(""));  
	if(openTextEntry->ShowModal() == wxID_OK){
	int threshVal;
	if(threshVal = wxAtoi(openTextEntry->GetValue())){ 
            printf("Acting...");
            lastImage = new wxImage(bitmap.ConvertToImage());
	    free(loadedImage);
	    loadedImage = new wxImage(bitmap.ConvertToImage());
            unsigned char r,g,b;
	    for( int i=minY;i<maxY;i++){
	       for(int j=minX;j<maxY;j++){
	 	r = loadedImage->GetRed(i,j);
		g = loadedImage->GetGreen(i,j);		
		b = loadedImage->GetBlue(i,j);
		int rInt = (int) r;
		int gInt = (int) g;
		int bInt = (int) b;
		if(rInt< threshVal){
			rInt = 255;		
		}
		if(gInt< threshVal){
			gInt = 255;		
		}
		if(bInt< threshVal){
			bInt = 255;		
		}
		r = (unsigned char) rInt;
		g = (unsigned char) gInt;
		b = (unsigned char) bInt;
		loadedImage->SetRGB(i,j,r,g,b);
	    }
	   }
	}
	}
    Refresh();
}


void MyFrame::OnHistoImage(wxCommandEvent & event){
	    printf("Acting...");
            lastImage = new wxImage(bitmap.ConvertToImage());
	    free(loadedImage);
	    loadedImage = new wxImage(bitmap.ConvertToImage());
            unsigned char r,g,b;
	    double histoR[256];
	    double histoG[256];
	    double histoB[256];
	    for(int i = 0; i<256;i++){
		histoR[i] = 0;
		histoG[i] = 0;
		histoB[i] = 0;
	    }// fill with zeros
	    for( int i=minY;i<maxY;i++){
	       for(int j=minX;j<maxY;j++){
	 	r = loadedImage->GetRed(i,j);
		g = loadedImage->GetGreen(i,j);		
		b = loadedImage->GetBlue(i,j);
		int rInt = (int) r;
		int gInt = (int) g;
		int bInt = (int) b;
		histoR[rInt] = histoR[rInt]+1;
		histoG[gInt] = histoG[gInt]+1;
		histoB[bInt] = histoB[bInt]+1;
	    }
	   } //Have histogram
	double sumR = 0;
	double sumG = 0;
	double sumB = 0;
	for(int i = 0; i<256;i++){
		sumR = sumR + histoR[i];
		sumG = sumG + histoG[i];
		sumB = sumB + histoB[i];
	}
	for(int i = 0; i<256;i++){
		histoR[i] = (histoR[i]/sumR);
		histoG[i] = (histoG[i]/sumG);
		histoB[i] = (histoB[i]/sumB);
	} //Histogram normalized
	double maxR = 0;
	double maxG = 0;
	double maxB = 0;
	for (int i = 0; i<256;i++){ //Get Maximum R G B values
		if(histoR[i]>0){
			maxR = i;
		}
		if(histoG[i]>0){
			maxG = i;
		}
		if(histoB[i]>0){
			maxB = i;
		}
	}
	float cuR = 0;
	float cuG = 0;
	float cuB = 0;
	for (int i = 0; i <256;i++){
		cuR = cuR + histoR[i];
		double tempR = (cuR*maxR);
		tempR = tempR +0.5;
		histoR[i] = (int) tempR;
	
		cuG = cuG + histoG[i];
		double tempG = (cuG*maxG);
		tempG = tempG +0.5;
		histoG[i] = (int) tempG;

		cuB = cuB + histoB[i];
		double tempB = (cuB*maxB);
		tempB = tempB +0.5;
		histoB[i] = (int) tempB;
	}

	    for( int i=minY;i<maxY;i++){ //apply table
	       for(int j=minX;j<maxY;j++){
	 	r = loadedImage->GetRed(i,j);
		g = loadedImage->GetGreen(i,j);		
		b = loadedImage->GetBlue(i,j);
		int rInt = (int) r;
		int gInt = (int) g;
		int bInt = (int) b;
		rInt = histoR[rInt];
		gInt = histoG[gInt];
		bInt = histoB[bInt];
		r = (unsigned char) rInt;
		g = (unsigned char) gInt;
		b = (unsigned char) bInt;
		loadedImage->SetRGB(i,j,r,g,b);
             }
          }
    Refresh();
}

void MyFrame::OnNegativeImage(wxCommandEvent & event){
	    printf("Acting...");
            lastImage = new wxImage(bitmap.ConvertToImage());
	    free(loadedImage);
	    loadedImage = new wxImage(bitmap.ConvertToImage());
            unsigned char r,g,b;
	    for( int i=minY;i<maxY;i++){
	       for(int j=minX;j<maxY;j++){
	 	r = loadedImage->GetRed(i,j);
		g = loadedImage->GetGreen(i,j);		
		b = loadedImage->GetBlue(i,j);
		int rInt = (int) r;
		int gInt = (int) g;
		int bInt = (int) b;
		r = (unsigned char) (255-rInt);
		g = (unsigned char) (255-gInt);
		b = (unsigned char) (255-bInt);
		if(rInt>255){
			rInt = 255;		
		}
		if(gInt>255){
			gInt = 255;		
		}
		if(bInt>255){
			bInt = 255;		
		}
		if(rInt<0){
			rInt = 0;		
		}
		if(gInt<0){
			gInt = 0;		
		}
		if(bInt<0){
			bInt = 0;		
		}
		loadedImage->SetRGB(i,j,r,g,b);
	    }
	
	
	}
    Refresh();
}

void MyFrame::OnLogImage(wxCommandEvent & event){
	    printf("Acting...");
            lastImage = new wxImage(bitmap.ConvertToImage());
	    free(loadedImage);
	    loadedImage = new wxImage(bitmap.ConvertToImage());
            unsigned char r,g,b;
	    for( int i=minY;i<maxY;i++){
	       for(int j=minX;j<maxY;j++){
	 	r = loadedImage->GetRed(i,j);
		g = loadedImage->GetGreen(i,j);		
		b = loadedImage->GetBlue(i,j);
		int rInt = (int) r;
		int gInt = (int) g;
		int bInt = (int) b;
		rInt = 10*log(1+rInt) + 0.5;
		gInt = 10*log(1+gInt) + 0.5;
		bInt = 10*log(1+bInt) + 0.5;
		r = (unsigned char) rInt;
		g = (unsigned char) gInt;
		b = (unsigned char) bInt;
		if(rInt>255){
			rInt = 255;		
		}
		if(gInt>255){
			gInt = 255;		
		}
		if(bInt>255){
			bInt = 255;		
		}
		if(rInt<0){
			rInt = 0;		
		}
		if(gInt<0){
			gInt = 0;		
		}
		if(bInt<0){
			bInt = 0;		
		}
		
		loadedImage->SetRGB(i,j,r,g,b);
	    }
          }
	
	
    Refresh();
}

void MyFrame::OnPowerImage(wxCommandEvent & event){
	wxTextEntryDialog *openTextEntry = new wxTextEntryDialog ( this, _T("Exponent Value"), _T(""), _T(""));  
	if(openTextEntry->ShowModal() == wxID_OK){
	double expVal;
	if(openTextEntry->GetValue().ToDouble(&expVal)){ 
	    printf("Acting...");
            lastImage = new wxImage(bitmap.ConvertToImage());
	    free(loadedImage);
	    loadedImage = new wxImage(bitmap.ConvertToImage());
            unsigned char r,g,b;
	    for( int i=minY;i<maxY;i++){
	       for(int j=minX;j<maxY;j++){
	 	r = loadedImage->GetRed(i,j);
		g = loadedImage->GetGreen(i,j);		
		b = loadedImage->GetBlue(i,j);
		int rInt = (int) r;
		int gInt = (int) g;
		int bInt = (int) b;
		rInt = pow(r,expVal) + 0.5;
		gInt = pow(g,expVal) + 0.5;
		bInt = pow(b,expVal) + 0.5;
		if(rInt>255){
			rInt = 255;		
		}
		if(gInt>255){
			gInt = 255;		
		}
		if(bInt>255){
			bInt = 255;		
		}
		if(rInt<0){
			rInt = 0;		
		}
		if(gInt<0){
			gInt = 0;		
		}
		if(bInt<0){
			bInt = 0;		
		}
		r = (unsigned char) rInt;
		g = (unsigned char) gInt;
		b = (unsigned char) bInt;
		
		loadedImage->SetRGB(i,j,r,g,b);
	    }
	   }
	   }
	}
    Refresh();
}

void MyFrame::OnSaltPepperImage(wxCommandEvent & event){
	wxTextEntryDialog *openTextEntry = new wxTextEntryDialog ( this, _T("Amount of Salt & Pepper"), _T(""), _T(""));  
	if(openTextEntry->ShowModal() == wxID_OK){
	int noiseAmount;
	if(noiseAmount = wxAtoi(openTextEntry->GetValue())){ 
	    printf("Salt And Peppering...");
	    lastImage = new wxImage(bitmap.ConvertToImage());
	    free(loadedImage);
	    loadedImage = new wxImage(bitmap.ConvertToImage());
            unsigned char r,g,b;
	    for( int i=0;i<noiseAmount;i++){
		int type = rand() % 2; // 0 or 1 (pepper or salt)
		type = type*255;
		int y = rand() % maxY + minX; 
		int x = rand() % maxY + minY; 
		loadedImage->SetRGB(y,x,type,type,type);
	    }
	
	    printf(" Finished Salt and Peppering.\n");
	}
	}
    Refresh();
}

void MyFrame::OnMinFilterImage(wxCommandEvent & event){
		printf("Applying min filter...");
		lastImage = new wxImage(bitmap.ConvertToImage());
		free(loadedImage);
		loadedImage = new wxImage(bitmap.ConvertToImage());
		wxImage *newImage = new wxImage(bitmap.ConvertToImage());
		unsigned char r,g,b;
		for( int i=minY+1;i<maxY-1;i++){
			for(int j=minX+1;j<maxX-1;j++){
				int minr = 255;
				int ming = 255;
				int minb = 255;
				for(int k = i-1; k<i+2;k++){ // vertical loop
					for(int p = j-1; p<j+2;p++){ // horizontal loop
						r = loadedImage->GetRed(k,p);
						g = loadedImage->GetGreen(k,p);		
						b = loadedImage->GetBlue(k,p);
						int rInt = (int) r;
						int gInt = (int) g;
						int bInt = (int) b;
						if(minr>rInt){
							minr = rInt;
						}
						if(ming>gInt){
							ming = gInt;
						}
						if(minb>bInt){
							minb = bInt;
						}

					}
				}
				r = (unsigned char) minr;
				g = (unsigned char) ming;
				b = (unsigned char) minb;


				newImage->SetRGB(i,j,r,g,b);
			}
	      }
	      loadedImage = newImage;

	printf(" Finished min filtering.\n");

Refresh();
}

void MyFrame::OnMaxFilterImage(wxCommandEvent & event){
		printf("Applying max filter...");
		lastImage = new wxImage(bitmap.ConvertToImage());
		free(loadedImage);
		loadedImage = new wxImage(bitmap.ConvertToImage());
		wxImage *newImage = new wxImage(bitmap.ConvertToImage());
		unsigned char r,g,b;
		for( int i=minY+1;i<maxY-1;i++){
			for(int j=minX+1;j<maxX-1;j++){
				int maxr = 0;
				int maxg = 0;
				int maxb = 0;
				for(int k = i-1; k<i+2;k++){ // vertical loop
					for(int p = j-1; p<j+2;p++){ // horizontal loop
						r = loadedImage->GetRed(k,p);
						g = loadedImage->GetGreen(k,p);		
						b = loadedImage->GetBlue(k,p);
						int rInt = (int) r;
						int gInt = (int) g;
						int bInt = (int) b;
						if(maxr<rInt){
							maxr = rInt;
						}
						if(maxg<gInt){
							maxg = gInt;
						}
						if(maxb<bInt){
							maxb = bInt;
						}

					}
				}
				r = (unsigned char) maxr;
				g = (unsigned char) maxg;
				b = (unsigned char) maxb;


				newImage->SetRGB(i,j,r,g,b);
			}
	      }
	      loadedImage = newImage;

	printf(" Finished max filtering.\n");

Refresh();
}

void MyFrame::OnMidFilterImage(wxCommandEvent & event){
		printf("Applying max filter...");
		lastImage = new wxImage(bitmap.ConvertToImage());
		free(loadedImage);
		loadedImage = new wxImage(bitmap.ConvertToImage());
		wxImage *newImage = new wxImage(bitmap.ConvertToImage());
		unsigned char r,g,b;
		for( int i=minY+1;i<maxY-1;i++){
			for(int j=minX+1;j<maxX-1;j++){
				int maxr = 0;
				int maxg = 0;
				int maxb = 0;
				int minr = 255;
				int ming = 255;
				int minb = 255;
				for(int k = i-1; k<i+2;k++){ // vertical loop
					for(int p = j-1; p<j+2;p++){ // horizontal loop
						r = loadedImage->GetRed(k,p);
						g = loadedImage->GetGreen(k,p);		
						b = loadedImage->GetBlue(k,p);
						int rInt = (int) r;
						int gInt = (int) g;
						int bInt = (int) b;
						if(maxr<rInt){
							maxr = rInt;
						}
						if(maxg<gInt){
							maxg = gInt;
						}
						if(maxb<bInt){
							maxb = bInt;
						}
						if(minr>rInt){
							minr = rInt;
						}
						if(ming>gInt){
							ming = gInt;
						}
						if(minb>bInt){
							minb = bInt;
						}

					}
				}
				int midr = (((minr + maxr) / 2)+0.5);
				int midg = (((ming + maxg) / 2)+0.5);
				int midb = (((minb + maxb) / 2)+0.5);
				r = (unsigned char) midr;
				g = (unsigned char) midg;
				b = (unsigned char) midb;


				newImage->SetRGB(i,j,r,g,b);
			}
	      }
	      loadedImage = newImage;

	printf(" Finished max filtering.\n");

Refresh();
}

void MyFrame::OnMaskImage(wxCommandEvent & event){
	wxTextEntryDialog *openTextEntry = new wxTextEntryDialog ( this, _T("Input Mask in format 111222333 (left to right, where 1 shows row 1, 2 row 2, etc.)"), _T(""), _T(""));  
	if(openTextEntry->ShowModal() == wxID_OK){
		wxString mask = openTextEntry->GetValue();
		printf("Applying mask...");
		lastImage = new wxImage(bitmap.ConvertToImage());
		free(loadedImage);
		loadedImage = new wxImage(bitmap.ConvertToImage());
		wxImage *newImage = new wxImage(bitmap.ConvertToImage());
		unsigned char r,g,b;
		for( int i=minY+1;i<maxY-1;i++){
			for(int j=minX+1;j<maxX-1;j++){
				float sumr = 0;
				float sumg = 0;
				float sumb = 0;
				int xcount = 0;
				int ycount = 0;
				for(int k = i-1; k<i+2;k++){ // vertical loop
					for(int p = j-1; p<j+2;p++){ // horizontal loop
						r = loadedImage->GetRed(k,p);
						g = loadedImage->GetGreen(k,p);		
						b = loadedImage->GetBlue(k,p);
						sumr = sumr +  ((int)  ((int)r) * (char((mask.GetChar(ycount*3+xcount)))-48));
						sumg = sumg +  ((int)  ((int)g) * (char((mask.GetChar(ycount*3+xcount)))-48));
						sumb = sumb +  ((int)  ((int)b) * (char((mask.GetChar(ycount*3+xcount)))-48));
						xcount++;
					}
					xcount = 0;
					ycount++;

				}
				float sum = 0;
				for(int q = 0; q<9;q++)
					sum = sum + (char(mask.GetChar(q))-48);
				
				sumr = (sumr/sum) + 0.5;
				sumg = (sumg/sum) + 0.5;
				sumb = (sumb/sum) + 0.5;
				r = ((unsigned char) ((int) sumr));
				g = ((unsigned char) ((int) sumg));
				b = ((unsigned char) ((int) sumb));


				newImage->SetRGB(i,j,r,g,b);
			}
	      }
	      loadedImage = newImage;
	}

	printf(" Finished masking.\n");

Refresh();
}


void MyFrame::OnUndoImage(wxCommandEvent & event){

    free(loadedImage);
    loadedImage = lastImage;
    Refresh();
}

void MyFrame::OnRoIImage(wxCommandEvent & event){
    wxTextEntryDialog *openTextEntry = new wxTextEntryDialog ( this, _T("Input ROI in format 0000111122223333 (0 = min x  1 = min y   2 = max x  3 = max y (ensure all 4 numbers are input (0048), then apply any changes, they will be performed then you are no longer 'selecting' area."), _T(""), _T(""));  
	if(openTextEntry->ShowModal() == wxID_OK){
		wxString str = openTextEntry->GetValue();
	minX = wxAtoi(str.SubString(0,3));
	minY = wxAtoi(str.SubString(4,7));
	maxX = wxAtoi(str.SubString(8,11));
	maxY = wxAtoi(str.SubString(12,15));
	}
}

  EVT_MENU ( INVERT_IMAGE_ID,  MyFrame::OnInvertImage)
  EVT_MENU ( SCALE_IMAGE_ID,  MyFrame::OnScaleImage)
  EVT_MENU ( SAVE_IMAGE_ID,  MyFrame::OnSaveImage)
  EVT_MENU ( SHIFT_IMAGE_ID,  MyFrame::OnShiftImage)
  EVT_MENU ( MASK_IMAGE_ID, MyFrame::OnMaskImage)
  EVT_MENU ( SALTPEPPER_IMAGE_ID, MyFrame::OnSaltPepperImage)
  EVT_MENU ( MINFILTER_IMAGE_ID, MyFrame::OnMinFilterImage)
  EVT_MENU ( MAXFILTER_IMAGE_ID, MyFrame::OnMaxFilterImage)
  EVT_MENU ( MIDFILTER_IMAGE_ID, MyFrame::OnMidFilterImage)
  EVT_MENU ( NEGATIVE_IMAGE_ID, MyFrame::OnNegativeImage)
  EVT_MENU ( LOG_IMAGE_ID, MyFrame::OnLogImage)
  EVT_MENU ( POWER_IMAGE_ID, MyFrame::OnPowerImage)
  EVT_MENU ( HISTO_IMAGE_ID, MyFrame::OnHistoImage)
  EVT_MENU ( THRESH_IMAGE_ID, MyFrame::OnThreshImage)
  EVT_MENU ( ITERTHRESH_IMAGE_ID, MyFrame::OnIterThreshImage)
  EVT_MENU ( UNDO_IMAGE_ID,  MyFrame::OnUndoImage)
  EVT_MENU ( ROI_IMAGE_ID, MyFrame::OnRoIImage)

//###########################################################//
//----------------------END MY EVENTS -----------------------//
//###########################################################// 

  EVT_PAINT (MyFrame::OnPaint)
END_EVENT_TABLE()
