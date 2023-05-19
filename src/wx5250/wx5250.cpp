//
// Created by Yvan Janssens on 19/05/2023.
//

// Start of wxWidgets "Hello World" Program
#include <wx/wx.h>

class MyApp : public wxApp
{
public:
    bool OnInit() override;
};

wxIMPLEMENT_APP(MyApp);

class MyFrame : public wxFrame
{
public:
    MyFrame();

private:
    void OnHello(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
};

enum
{
    ID_Hello = 1,
    ID_Connect = 2,
    ID_ConnectDefault = 3,
    ID_PUB400Connect = 4,
    ID_HONECKERConnect = 5,
    ID_BACKPAINConnect = 6
};

bool MyApp::OnInit()
{
    MyFrame *frame = new MyFrame();
    frame->Show(true);
    return true;
}

MyFrame::MyFrame()
        : wxFrame(nullptr, wxID_ANY, "wx5250")
{
    wxMenu *menuFile = new wxMenu;

    menuFile->Append(ID_ConnectDefault, "Connect default",
                     "Connect to the default IBM i system.");
    menuFile->Append(ID_Connect, "&Connect...",
                     "Connect to an IBM i system.");


    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);

    wxMenu *publicSystems = new wxMenu;
    publicSystems->Append(ID_PUB400Connect, "PUB400",
                           "Connect to Holger's PUB400 system.");
    publicSystems->Append(ID_HONECKERConnect, "HONECKER.QSECO.FR",
                          "Connect to friedkiwi's P6 system.");
    publicSystems->Append(ID_BACKPAINConnect, "BACKPAIN.QSECO.FR",
                          "Connect to friedkiwi's P8 system.");

    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(publicSystems, "Public systems");
    menuBar->Append(menuHelp, "&Help");

    SetMenuBar( menuBar );

    CreateStatusBar();
    SetStatusText("Welcome to wx5250!");

    Bind(wxEVT_MENU, &MyFrame::OnHello, this, ID_Hello);
    Bind(wxEVT_MENU, &MyFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MyFrame::OnExit, this, wxID_EXIT);
}

void MyFrame::OnExit(wxCommandEvent& event)
{
    Close(true);
}

void MyFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("This is a wxWidgets Hello World example",
                 "About Hello World", wxOK | wxICON_INFORMATION);
}

void MyFrame::OnHello(wxCommandEvent& event)
{
    wxLogMessage("Hello world from wxWidgets!");
}
