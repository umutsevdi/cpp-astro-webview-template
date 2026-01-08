#include <wx/ffile.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/webview.h>
#include <wx/wx.h>

class MainFrame : public wxFrame {
public:
    MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
        : wxFrame(nullptr, wxID_ANY, title, pos, size)
    {
        prepareMenuBar();
        _webview = wxWebView::New(this, wxID_ANY);
        _webview->AddScriptMessageHandler("external");
        wxString exeDir
            = wxPathOnly(wxStandardPaths::Get().GetExecutablePath());
        wxString url = wxT("file://") + exeDir + wxT("/web-assets/index.html");
        _webview->LoadURL(url);

        wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
        sizer->Add(_webview, 1, wxEXPAND);
        wxButton* updateButton = new wxButton(this, wxID_ANY, "Update Webview");

        sizer->Add(updateButton, 0, wxEXPAND | wxALL, 5);
        SetSizer(sizer);

        Bind(wxEVT_MENU, &MainFrame::onMenuExit, this, wxID_EXIT);
        Bind(wxEVT_MENU, &MainFrame::onMenuAbout, this, wxID_ABOUT);
        _webview->Bind(wxEVT_WEBVIEW_SCRIPT_MESSAGE_RECEIVED,
            &MainFrame::onJsMessage, this);
        updateButton->Bind(wxEVT_BUTTON, &MainFrame::onButtonPress, this);
    }

private:
    void onJsMessage(wxWebViewEvent& event)
    {
        wxString message = event.GetString();
        wxLogMessage("Received message from JS: %s", message);

        if (message == "showMessage") {
            wxMessageBox("Hello from C++!", "Interop Example",
                wxOK | wxICON_INFORMATION);
        }
    }

    void onButtonPress(wxCommandEvent&)
    {

        wxString script
            = wxString::Format("window.updateFromCpp(%d);", ++counter);
        _webview->RunScript(script);
    }

    void onMenuExit(wxCommandEvent&) { Close(true); }

    void onMenuAbout(wxCommandEvent&)
    {
        wxMessageBox("This is a sample application.", "About",
            wxOK | wxICON_INFORMATION);
    }

    void prepareMenuBar()
    {
        wxMenuBar* menuBar = new wxMenuBar();
        wxMenu* fileMenu   = new wxMenu();
        fileMenu->Append(wxID_OPEN, "&Open\tCtrl+O");
        fileMenu->Append(wxID_SAVE, "&Save\tCtrl+S");
        fileMenu->AppendSeparator();
        fileMenu->Append(wxID_EXIT, "&Exit\tCtrl+Q");
        menuBar->Append(fileMenu, "&File");
        wxMenu* editMenu = new wxMenu();
        editMenu->Append(wxID_UNDO, "&Undo\tCtrl+Z");
        editMenu->Append(wxID_REDO, "&Redo\tCtrl+Y");
        editMenu->AppendSeparator();
        editMenu->Append(wxID_CUT, "Cu&t\tCtrl+X");
        editMenu->Append(wxID_COPY, "&Copy\tCtrl+C");
        editMenu->Append(wxID_PASTE, "&Paste\tCtrl+V");
        menuBar->Append(editMenu, "&Edit");
        wxMenu* viewMenu = new wxMenu();
        viewMenu->Append(wxID_ZOOM_IN, "Zoom &In\tCtrl++");
        viewMenu->Append(wxID_ZOOM_OUT, "Zoom &Out\tCtrl+-");
        menuBar->Append(viewMenu, "&View");
        wxMenu* helpMenu = new wxMenu();
        helpMenu->Append(wxID_ABOUT, "&About");
        menuBar->Append(helpMenu, "&Help");
        SetMenuBar(menuBar);
    }

    wxWebView* _webview;
    int counter = 0;
};

class Application : public wxApp {
public:
    virtual bool OnInit()
    {
        (new MainFrame("Application", wxPoint(50, 50), wxSize(800, 600)))
            ->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(Application);
