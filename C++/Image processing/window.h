
class BasicApplication : public wxApp {
 public:
    virtual bool OnInit();
};


class MyFrame : public wxFrame {    
 protected:
    wxMenuBar  *menuBar;//main menu bar
    wxMenu     *fileMenu;//file menu
    wxBitmap *back_bitmap; // offscreen memory buffer for drawing
    wxToolBar *toolbar;//tollbar not necessary to use
    int oldWidth, oldHeight; // save old dimensions

    wxBitmap bitmap;  //structure for the edited image
    wxImage *loadedImage; // image loaded from file
    wxImage *lastImage;
    int minX,minY,maxX,maxY;
    int imgWidth, imgHeight; // image dimensions
    int stuffToDraw; 

   /* declear message handler */
    void OnInvertImage(wxCommandEvent & event); //Invert Image Colours
    void OnScaleImage(wxCommandEvent & event); //Scale image colours (add or subtract brightness)
    void OnSaveImage(wxCommandEvent & event); //Save the Image.
    void OnShiftImage(wxCommandEvent & event); //Shift pixels
    void OnMaskImage(wxCommandEvent & event); //Apply a mask to the image.
    void OnSaltPepperImage(wxCommandEvent & event); //Apply salt and pepper noise onto the image.
    void OnMinFilterImage(wxCommandEvent & event); //Apply the min-filter
    void OnMaxFilterImage(wxCommandEvent & event); //Apply the max-filter.
    void OnMidFilterImage(wxCommandEvent & event); //Apply the mid-filter.
    void OnNegativeImage(wxCommandEvent & event); //Get the negative scale of the image.
    void OnLogImage(wxCommandEvent & event); //Get the logarithmic scale of the image.
    void OnPowerImage(wxCommandEvent & event); //Get the exponent-scaled values of a image.
    void OnHistoImage(wxCommandEvent & event); //Apply a histogram equalization.
    void OnThreshImage(wxCommandEvent & event); // Threshold the image.
    void OnIterThreshImage(wxCommandEvent & event); //Use an iterative thresholding algorithm
    void OnUndoImage(wxCommandEvent & event); //Undo last action
    void OnRoIImage(wxCommandEvent & event); //Apply any action to Region of Interest
    
 public:
    MyFrame(const wxString title, int xpos, int ypos, int width, int height);
    virtual ~MyFrame();
    
    void OnExit(wxCommandEvent & event);       
    void OnOpenFile(wxCommandEvent & event);       
    void OnPaint(wxPaintEvent & event);	
    
    DECLARE_EVENT_TABLE()
	
};

enum { NOTHING = 0,
       ORIGINAL_IMG,
};

enum { EXIT_ID = wxID_HIGHEST + 1,      
       LOAD_FILE_ID,
       INVERT_IMAGE_ID,
       SCALE_IMAGE_ID,
       SAVE_IMAGE_ID,
       SHIFT_IMAGE_ID,
       MASK_IMAGE_ID,
       SALTPEPPER_IMAGE_ID,
       MINFILTER_IMAGE_ID,
       MAXFILTER_IMAGE_ID,
       MIDFILTER_IMAGE_ID,
       NEGATIVE_IMAGE_ID,
       LOG_IMAGE_ID,
       POWER_IMAGE_ID,
       HISTO_IMAGE_ID,
       THRESH_IMAGE_ID,
       ITERTHRESH_IMAGE_ID,
       UNDO_IMAGE_ID,
       ROI_IMAGE_ID,
};
