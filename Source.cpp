#include <iostream>
#include <string>
#include <stdlib.h>
#include <fstream>
#include <Windows.h>
#include <CommCtrl.h>
#include "resource.h"
using namespace std;

struct dosdate {
    char dia[3];
    char mes[12];
    char ano[5];
};

struct CARNET {
    CARNET* ant;
    char persocurp[16];
    char vacuna[20];
    char dosis[10];
    dosdate fechadosis;
    char lote[10];
    char vacunacenter[50];
    CARNET* sig;
};

CARNET* inicic, * auxc, * finc, * krnet = nullptr;
CARNET* registrc = nullptr;
dosdate completa;
SYSTEMTIME date;

struct USER {
    USER* ant;
    char userkey[20];
    char password[20];
    char apaterno[15];
    char amaterno[15];
    char nombres[30];
    USER* sig;
};

USER* iniciu, * auxu, * finu, * usr = nullptr;
USER* registru = nullptr;

struct PERSONA {
    PERSONA* ant;
    char apatern[15];
    char amatern[15];
    char pername[30];
    char percurp[16];
    char perfc[16];
    dosdate fecnam;
    char percalle[30];
    char percolon[30];
    char percity[40];
    char perestado[30];
    char perestciv[20];
    int pertel;
    char peridentif[500];
    char persex[10];
    char pergrupocup[20];
    char perfriesgo[20];
    PERSONA* sig;
};

PERSONA* inicip, * auxp, * finp, * prson = nullptr;
PERSONA* registrp = nullptr;
HBITMAP bmp;

struct VACUNA {
    VACUNA* ant;
    char vactipo[30];
    char marca[30];
    int numdosis;
    char vacode[10];
    char vacdesc[100];
    char vacprize[5];
    VACUNA* sig;
};

VACUNA* iniciv, * auxv, * finv, * vcuna = nullptr;
VACUNA* registrv = nullptr;

void regCarnet(CARNET* nuevo);
void delCarnet(CARNET* auxc);
void modCarnet(CARNET* carnetmod, char krnetmod[5000]);
void regUser(USER* nuevo);
void regPersona(PERSONA* nuevo);
void regVacuna(VACUNA* nuevo);
void writeCarnetBinary();
void readCarnetBinary();
void writeUserBinary();
void readUserBinary();
void writePersonBinary();
void readPersonBinary();
void writeVacunaBinary();
void readVacunaBinary();
void menu1(WPARAM wParam, HWND hWnd);
BOOL CALLBACK vLogin(HWND hWNd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK vRegUser(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK vRegCarnet(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK vRegPersona(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK vRegVacuna(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

HINSTANCE hInstGlobal;
HWND hWndLogin;
HWND hWndRegUser;
HWND hWndRegCarnet;
HWND hWndPersona;
HWND hWndVacuna;

WNDPROC lpEditWndProc;

int ventRegis = 1;

int func = 0;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow) {
    hInstGlobal = hInstance;
    readCarnetBinary();
    readUserBinary();
    readPersonBinary();
    auxu = iniciu;

    hWndLogin = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_LOGIN), 0, vLogin);

    ShowWindow(hWndLogin, cmdshow);

    MSG Message;

    ZeroMemory(&Message, sizeof(Message));

    while (GetMessage(&Message, 0, 0, 0)) {
        TranslateMessage(&Message);
        DispatchMessage(&Message);
    }

    return Message.wParam;
}

BOOL CALLBACK vLogin(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    hWndLogin = hWnd;

    switch (msg) {

        case WM_INITDIALOG: {
            EndDialog(hWndRegUser, 0);
        }break;

        case WM_COMMAND: {

            switch (LOWORD(wParam)) {
            case IDC_LOGIN: {
                auxu = iniciu;

                char userki[20];
                char passwor[20];

                if (iniciu == nullptr) {
                    MessageBox(hWnd, "Usuario Incorrecto. Por favor, intentelo de nuevo", "AVISO", MB_OK | MB_ICONWARNING);
                }
                else {
                    GetDlgItemText(hWnd, IDC_USERKEY, userki, 20);
                    GetDlgItemText(hWnd, IDC_PASSWORD, passwor, 20);

                    while (auxu != nullptr && nullptr && strcmp(userki, auxu->userkey)) {
                        auxu = auxu->sig;
                    }

                    if (auxu == nullptr) {
                        MessageBox(hWnd, "Usuario Incorrecto. Por favor, intentelo de nuevo", "AVISO", MB_OK | MB_ICONWARNING);
                    }
                    else {
                        if (strcmp(userki, auxu->userkey) == 0) {
                            usr = auxu;
                            EndDialog(hWndLogin, 0);
                            DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_REGCARNET), NULL, vRegCarnet);
                        }
                        else {
                            MessageBox(hWnd, "COntraseña Incorrecta. Por favor, intentalo de nuevo", "AVISO", MB_OK | MB_ICONWARNING);
                        }
                    }
                }
            }break;

            case IDC_REGISTER: {
                DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_REGUSER), NULL, vRegUser);
            }break;

            case WM_CLOSE: {
                int opc = MessageBox(hWnd, "Esta seguro que desea salir?", "AVISO", MB_YESNO | MB_ICONQUESTION);
                    switch (opc) {
                    case IDYES:
                        DestroyWindow(hWnd);
                        PostQuitMessage(0);
                    }break;
                }break;
            }

        }break;
    }
    return 0;
}

BOOL CALLBACK vRegUser(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    hWndRegUser = hWnd;
    
    switch (msg) {
        case WM_INITDIALOG: {
            DestroyWindow(hWndLogin);
            EndDialog(hWndRegUser, 0);

            if (ventRegis == 1) {
                SetDlgItemText(hWnd, IDC_USERKER, usr->userkey);
                SetDlgItemText(hWnd, IDC_PASSWORR, usr->password);
                SetDlgItemText(hWnd, IDC_APATERNR, usr->apaterno);
                SetDlgItemText(hWnd, IDC_AMATERNR, usr->amaterno);
                SetDlgItemText(hWnd, IDC_NOMBRER, usr->nombres);
            }
        }break;

        case WM_COMMAND: {
            menu1(LOWORD(wParam), hWnd);

            switch (LOWORD(wParam)) {

                case IDOK: {
                    USER* registru = new USER;

                    GetDlgItemText(hWnd, IDC_USERKER, registru->userkey, 20);
                    GetDlgItemText(hWnd, IDC_PASSWORR, registru->password, 20);
                    GetDlgItemText(hWnd, IDC_APATERNR, registru->apaterno, 15);
                    GetDlgItemText(hWnd, IDC_APATERNR, registru->amaterno, 15);
                    GetDlgItemText(hWnd, IDC_NOMBRER, registru->nombres, 30);
                    regUser(registru);

                    delete registru;

                    writeUserBinary();

                    MessageBox(hWnd, "Usuario Registrado", "AVISO", MB_OK | MB_ICONINFORMATION);

                }break;

                case IDCANCEL: {
                    if (ventRegis == 1) {

                        hWndLogin = CreateDialog(hInstGlobal, MAKEINTRESOURCE(IDD_LOGIN), NULL, vLogin);
                        ShowWindow(hWndLogin, SW_SHOW);
                    }
                    else if (ventRegis == 2) {
                        int opc = MessageBox(hWnd, "Segur que deseas salir?", "AVISO", MB_YESNO | MB_ICONQUESTION);

                        switch (opc) {
                        case IDYES:
                            DestroyWindow(hWnd);
                            PostQuitMessage(0);
                        break;
                        }
                    }
                }break;
            }
        }break;

        case WM_CLOSE: {
            int opc = MessageBox(0, "Seguro que desea salir?", "AVISO", MB_YESNO | MB_ICONQUESTION);

            switch (opc) {
            case IDYES:
                DestroyWindow(hWnd);
                PostQuitMessage(0);
            break;
            }
        }break;
    }

return 0;
}

BOOL CALLBACK vRegPersona(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    hWndPersona = hWnd;
    SYSTEMTIME nacimiento;
    char identif[500];

    switch (msg) {
        case WM_INITDIALOG: {
            DestroyWindow(hWnd);
            EndDialog(hWndRegCarnet, 0);

            if (ventRegis == 1) {
                EnableWindow(GetDlgItem(hWnd, IDC_DATETIMEPICKERP), false);
                EnableWindow(GetDlgItem(hWnd, IDC_IDENTIFP2), false);
            }
            else if (ventRegis == 2) {
                SetDlgItemText(hWnd, IDC_APATERNP, prson->apatern);
                SetDlgItemText(hWnd, IDC_AMATERNP, prson->amatern);
                SetDlgItemText(hWnd, IDC_NOMBREP, prson->pername);
                SetDlgItemText(hWnd, IDC_CURPP, prson->percurp);
                SetDlgItemText(hWnd, IDC_RFCP, prson->perfc);
                SetDlgItemText(hWnd, IDC_CALLP, prson->percalle);
                SetDlgItemText(hWnd, IDC_COLONIP, prson->percolon);
                SetDlgItemText(hWnd, IDC_CITYP, prson->percity);
                SetDlgItemText(hWnd, IDC_ESTADP, prson->perestado);
                SetDlgItemText(hWnd, IDC_ESTACIVIP, prson->perestciv);
                SetDlgItemText(hWnd, IDC_TELEFP, prson->pertel);
                SetDlgItemText(hWnd, IDC_IDENTIFP1, prson->peridentif);
                SetDlgItemText(hWnd, IDC_SEXP, prson->persex);
                SetDlgItemText(hWnd, IDC_GRUOCUP, prson->pergrupocup);
                SetDlgItemText(hWnd, IDC_PERRIEP, prson->perfriesgo);

                ZeroMemory(&date, sizeof(date));
                date.wDay = atoi(prson->fecnam.dia);
                date.wMonth = atoi(prson->fecnam.mes);
                date.wYear = atoi(prson->fecnam.ano);
                SendDlgItemMessage(hWnd, IDC_DATETIMEPICKERP, DTM_SETSYSTEMTIME, (WPARAM)GDT_VALID, (LPARAM)&date);

                bmp = (HBITMAP)LoadImage(NULL, prson->peridentif, IMAGE_BITMAP, 200, 200, LR_LOADFROMFILE);
                SendDlgItemMessage(hWnd, IDC_IDENTIFP3, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
            }

            if (func == 1) {
                SendMessage(GetDlgItem(hWnd, IDOP), WM_SETTEXT, 0, (LPARAM)"Modificar Datos");
            }

        }break;

        case WM_COMMAND: {
            menu1(LOWORD(wParam), hWnd);

            switch (LOWORD(wParam)) {

                case IDOP: {
                    PERSONA* registrp = new PERSONA;
                
                    GetDlgItemText(hWnd, IDC_APATERNP, registrp->apatern, 15);
                    GetDlgItemText(hWnd, IDC_AMATERNP, registrp->amatern, 15);
                    GetDlgItemText(hWnd, IDC_NOMBREP, registrp->pername, 30);
                    GetDlgItemText(hWnd, IDC_CURPP, registrp->percurp, 16);
                    GetDlgItemText(hWnd, IDC_RFCP, registrp->perfc, 16);
                    GetDlgItemText(hWnd, IDC_CALLP, registrp->percalle, 30);
                    GetDlgItemText(hWnd, IDC_COLONIP, registrp->percolon, 30);
                    GetDlgItemText(hWnd, IDC_CITYP, registrp->percity, 40);
                    GetDlgItemText(hWnd, IDC_ESTADP, registrp->perestado, 30);
                    GetDlgItemText(hWnd, IDC_ESTACIVIP, registro->perestciv, 20);
                    GetDlgItemText(hWnd, IDC_TELEFP, registrp->pertel, 10);
                    GetDlgItemText(hWnd, IDC_IDENTIFP1, registrp->peridentif, 500);
                    GetDlgItemText(hWnd, IDC_SEXP, registrp->persex, 10);
                    GetDlgItemText(hWnd, IDC_GRUOCUP, registrp->pergrupocup, 20);
                    GetDlgItemText(hWnd, IDC_PERRIEP, registrp->perfriesgo, 20);
                    strcpy_s(registrp->fecnam.dia, "");
                    strcpy_s(registrp->fecnam.mes, "");
                    strcpy_s(registrp->fecnam.ano, "");
                    regPersona(registrp);

                    delete registrp;

                    writePersonBinary();

                    MessageBox(hWnd, "Persona Registrada", "AVISO", MB_OK | MB_ICONINFORMATION);

                }break;
                
                case IDC_IDENTIFP2: {
                    OPENFILENAME ofn;

                    char zFile[500];

                    ZeroMemory(&ofn, sizeof(ofn));
 
                    ofn.lStructSize = sizeof(ofn);
                    ofn.hwndOwner = hWnd;
                    ofn.lpstrFilter = "ALL\0 * .*\0Bitmaps\0 * .bmp\0";
                    ofn.lpstrFile = zFile;
                    ofn.lpstrFile[0] = '\0';
                    ofn.nMaxFile = sizeof(zFile);
                    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
                    ofn.nFilterIndex = 2;

                    if (GetOpenFileName(&ofn) == TRUE) {
                        SetDlgItemText(hWnd, IDC_IDENTIFP1, zFile);
                        //SetDindowText(GetDlgItem(hWnd, IDC_RPICPATH), zfile);
                        bmp = (HBITMAP)LoadImage(NULL, zFile, IMAGE_BITMAP, 200, 200, LR_LOADFROMFILE | LR_DEFAULTSIZE);
                        SendDlgItemMessage(hWnd, IDC_IDENTIFP3, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
                    }
                    else {
                        MessageBox(hWnd, "No se ha seleccionado una imagen", "AVISO", MB_OK | MB_ICONERROR);
                    }

                }break;

                case IDCANCEP: {
                    if (ventRegis == 1) {

                        hWndLogin = CreateDialog(hInstGlobal, MAKEINTRESOURCE(IDD_LOGIN), NULL, vLogin);
                        ShowWindow(hWndLogin, SW_SHOW);
                    }
                    else if (ventRegis == 2) {
                        int opc = MessageBox(hWnd, "Seguro que desea salir?", "AVISO", MB_YESNO | MB_ICONQUESTION);

                        switch (opc) {
                        case IDYES:
                        DestroyWindow(hWnd);
                        PostQuitMessage(0);
                        break;
                        }
                    }
                }break;
            }
        }break;

        case WM_CLOSE: {
            int opc = MessageBox(0, "Seguro que desea salir?", "AVISO", MB_YESNO | MB_ICONQUESTION);

            switch (opc) {
            case IDYES:
            DestroyWindow(hWnd);
            PostQuitMessage(0);
            break;
            }
        }break;
    }

    return 0;
}

BOOL CALLBACK vRegVacuna(HWND hWNd, UINT msg, WPARAM wParam, LPARAM lParam) {
    hWndVacuna = hWnd;
    switch (msg) {
    case WM_INITDIALOG: {
        DestroyWindow(hWnd);

        if (ventRegis == 1) {
            SetDlgItemText(hWNd, IDC_TIPOVAC, vcuna->vactipo);
            SetDlgItemText(hWNd, IDC_MARCAVAC, vcuna->marca);
            SetDlgItemText(hWNd, IDC_CLAVAC, vcuna->vacode);
            SetDlgItemText(hWNd, IDC_DESCVAC, vcuna->vacdesc);
            SetDlgItemText(hWNd, IDC_PREVAC, vcuna->vacprize);
        }
    }break;

    case WM_COMMAND: {
        menu1(LOWORD(wParam), hWnd);

        switch (LOWORD(wParam)) {

        case IDOV: {
            VACUNA* registrv = new VACUNA;

            registrv->numdosis = atoi(numdosis);
            GetDlgItemText(hWNd, IDC_TIPOVAC, vcuna->vactipo, 30);
            GetDlgItemText(hWNd, IDC_MARCAVAC, vcuna->marca, 30);
            GetDlgItemText(hWnd, IDC_NOMDOS, vcuna->numdosis, 10);
            GetDlgItemText(hWNd, IDC_CLAVAC, vcuna->vacode, 10);
            GetDlgItemText(hWNd, IDC_DESCVAC, vcuna->vacdesc, 100);
            GetDlgItemText(hWNd, IDC_PREVAC, vcuna->vacprize, 5);

            regVacuna(registrv);
            writeVacunaBinary();

            MessageBox(hWnd, "Vacuna registrada", "AVISO", MB_OK | MB_ICONINFORMATION);

            delete registrv;
        }break;

        case IDCANCEV: {
            if (ventRegis == 1) {
                hWndLogin = CreateDialog(hInstGlobal, MAKEINTRESOURCE(IDD_LOGIN), NULL, vLogin);
                ShowWindow(hWndLogin, SW_SHOW);
            }
            else if (ventRegis == 2) {
                int opc = MessageBox(hWnd, "Seguro que desea salir?", "AVISO", MB_YESNO | MB_ICONQUESTION);

                switch (opc) {
                case IDYES:
                    DestroyWindow(hWnd);
                    PostQuitMessage(0);
                    break;
                }
            }
        }break;
        }
    }break;

    case WM_CLOSE: {

        int opc = MessageBox(0, "Esta seguro que desea salir de AuraShop?", "SALIR", MB_YESNO | MB_ICONQUESTION);

        switch (opc) {
        case IDYES:
            DestroyWindow(hWnd);
            PostQuitMessage(0);
            break;
        }
    }break;
    }

return 0;
}

void menu1(WPARAM wParam, HWND hWnd) {
    switch (LOWORD(wParam)) {

        case ID_CARNET_REGISTRARCARNET: {
            EndDialog(hWnd, 0);
            DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_REGCARNET), NULL, vRegCarnet);
        }break;

        case ID_VACUNA_AGREGARVACUNA: {
            EndDialog(hWnd, 0);
            DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_REGVACUNA), NULL, vRegVacuna);
        }break;

        case ID_PERSONA_REGISTRARPERSONA: {
            EndDialog(hWnd, 0);
            DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_REGPERSONA), NULL, vRegPersona);
        }break;

        case ID_ACERCADE_SALIR: {
            DestroyWindow(hWnd);
            PostQuitMessage(0);
        }break;
    }
}

void regUser(USER* nuevo) {
    if (iniciu == nullptr) {
        iniciu = new USER;
        auxu = iniciu;

        auxu->sig = nullptr;
        auxu->ant = nullptr;

        strcpy_s(auxu->userkey, nuevo->userkey);
        strcpy_s(auxu->password, nuevo->password);
        strcpy_s(auxu->apaterno, nuevo->apaterno);
        strcpy_s(auxu->amaterno, nuevo->amaterno);
        strcpy_s(auxu->nombres, nuevo->nombres);

        finu = auxu;
    }
    else {
        auxu = iniciu;

        while (auxu->sig != nullptr) {
            auxu = auxu->sig;
        }

        auxu->sig = new USER;

        auxu->sig->sig = nullptr;
        auxu->sig->ant = auxu;

        auxu = auxu->sig;

        strcpy_s(auxu->userkey, nuevo->userkey);
        strcpy_s(auxu->password, nuevo->password);
        strcpy_s(auxu->apaterno, nuevo->apaterno);
        strcpy_s(auxu->amaterno, nuevo->amaterno);
        strcpy_s(auxu->nombres, nuevo->nombres);

        finu = auxu;
    }
}

void regPersona(PERSONA* nuevo) {
    if (inicip == nullptr) {
        inicip = new PERSONA;
        auxp = inicip;


        auxp->sig = nullptr;
        auxp->ant = nullptr;


        auxp->pertel = nuevo->pertel;
        strcpy_s(auxp->apatern, nuevo->apatern);
        strcpy_s(auxp->amatern, nuevo->amatern);
        strcpy_s(auxp->pername, nuevo->pername);
        strcpy_s(auxp->percurp, nuevo->percurp);
        strcpy_s(auxp->perfc, nuevo->perfc);
        strcpy_s(auxp->fecnam.dia, nuevo->fecnam.dia);
        strcpy_s(auxp->fecnam.mes, nuevo->fecnam.mes);
        strcpy_s(auxp->fecnam.ano, nuevo->fecnam.ano);
        strcpy_s(auxp->percalle, nuevo->percalle);
        strcpy_s(auxp->percolon, nuevo->percolon);
        strcpy_s(auxp->percity, nuevo->percity);
        strcpy_s(auxp->perestado, nuevo->perestado);
        strcpy_s(auxp->perestciv, nuevo->perestciv);
        strcpy_s(auxp->peridentif, nuevo->peridentif);
        strcpy_s(auxp->persex, nuevo->persex);
        strcpy_s(auxp->pergrupocup, nuevo->pergrupocup);
        strcpy_s(auxp->perfriesgo, nuevo->perfriesgo);

        finp = auxp;
    }
    else {
        auxp = inicip;

        while (auxp->sig != nullptr) {
            auxu = auxu->sig;
        }

        auxp->sig = new PERSONA;

        auxp->sig->sig = nullptr;
        auxp->sig->ant = auxp;

        auxp = auxp->sig;

        auxp->pertel = nuevo->pertel;
        strcpy_s(auxp->apatern, nuevo->apatern);
        strcpy_s(auxp->amatern, nuevo->amatern);
        strcpy_s(auxp->pername, nuevo->pername);
        strcpy_s(auxp->percurp, nuevo->percurp);
        strcpy_s(auxp->perfc, nuevo->perfc);
        strcpy_s(auxp->percalle, nuevo->percalle);
        strcpy_s(auxp->percolon, nuevo->percolon);
        strcpy_s(auxp->percity, nuevo->percity);
        strcpy_s(auxp->perestado, nuevo->perestado);
        strcpy_s(auxp->perestciv, nuevo->perestciv);
        strcpy_s(auxp->peridentif, nuevo->peridentif);
        strcpy_s(auxp->persex, nuevo->persex);
        strcpy_s(auxp->pergrupocup, nuevo->pergrupocup);
        strcpy_s(auxp->perfriesgo, nuevo->perfriesgo);

        finp = auxp;
    }
}

void regVacuna(VACUNA* nuevo) {
    if (iniciv == nullptr) {
        iniciv == new VACUNA;
        auxv = iniciv;

        strcpy_s(auxv->vactipo, nuevo->vactipo);
        strcpy_s(auxv->marca, nuevo->marca);
        strcpy_s(auxv->numdosis, nuevo->numdosis);
        strcpy_s(auxv->vacode, nuevo->vacode);
        strcpy_s(auxv->vacdesc, nuevo->vacdesc);
        strcpy_s(auxv->vacprize, nuevo->vacprize);

        finv = auxv;
    }
    else {
        auxv = iniciv;

        while (auxv->sig != nullptr) {
            auxv = auxv->sig;
        }

        auxv->sig = new VACUNA;

        auxv->sig->sig = nullptr;
        auxv->sig->ant = auxv;

        strcpy_s(auxv->vactipo, nuevo->vactipo);
        strcpy_s(auxv->marca, nuevo->marca);
        strcpy_s(auxv->numdosis, nuevo->numdosis);
        strcpy_s(auxv->vacode, nuevo->vacode);
        strcpy_s(auxv->vacdesc, nuevo->vacdesc);
        strcpy_s(auxv->vacprize, nuevo->vacprize);

        finv = auxv
    }
}