#include <iostream>
#include <string>
#include <stdlib.h>
#include <fstream>
#include <Windows.h>
#include <CommCtrl.h>
#include "resource.h"
#include <cstdlib>
using namespace std;

struct dosdate {
    char dia[3];
    char mes[12];
    char ano[5];
};

struct CARNET {
    CARNET* ant;
    int numcarnet;
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
    char numdosis[10];
    char vacode[10];
    char vacdesc[100];
    char vacprize[5];
    VACUNA* sig;
};

VACUNA* iniciv, * auxv, * finv, * vcuna = nullptr;
VACUNA* registrv = nullptr;

void regCarnet(CARNET* nuevo);
void delCarnet(CARNET* auxc);
void modCarnet(CARNET* carnetmod);
void searCarnet();
void searCarnet2();
void regUser(USER* nuevo);
void regPersona(PERSONA* nuevo);
void regVacuna(VACUNA* nuevo);
void writeCarnet();
void readCarnet();
void writeUser();
void readUser();
void writePersona();
void readPersona();
void quicksort(int *A, int izq, int der);
void menu1(WPARAM wParam, HWND hWnd);
wstring s2ws(string& s);
wstring s2ws(char* c);
BOOL CALLBACK vLogin(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK vRegUser(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK vRegCarnet(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK vDelCarnet(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK vModCarnet(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK vSearCarnet(HWND hWNd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK vSearCarnet2(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK vRegPersona(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK vRegVacuna(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

HINSTANCE hInstGlobal;
HWND hWndLogin;
HWND hWndRegUser;
HWND hWndRegCarnet;
HWND hWndModCarnet;
HWND hWndDelCarnet;
HWND hWndSearCarnet;
HWND hWndSearCarnet2;
HWND hWndPersona;
HWND hWndVacuna;

WNDPROC lpEditWndProc;

int ventRegis = 1;

int func = 0;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow) {
    readUser();
    hInstGlobal = hInstance;
    auxu = iniciu;

    hWndLogin = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_LOGIN), 0, (DLGPROC)vLogin);

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

    readUser();

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
                WCHAR tmps[100] = { 0 };
                if (iniciu == nullptr) {
                    MessageBox(hWnd, L"Usuario Incorrecto. Por favor, intentelo de nuevo", L"AVISO", MB_OK | MB_ICONWARNING);
                }
                else {
                    GetDlgItemText(hWnd, IDC_USERKEY, tmps, 20);
                    wstring userkiw(tmps);
                    string userkis(userkiw.begin(), userkiw.end());
                    strcpy_s(userki, 20, userkis.c_str());
                    GetDlgItemText(hWnd, IDC_PASSWORD, tmps, 20);
                    wstring passworw(tmps);
                    string passwors(passworw.begin(), userkiw.end());
                    strcpy_s(passwor, 20, passwors.c_str());

                    while (auxu != nullptr && nullptr && strcmp(userki, auxu->userkey)) {
                        auxu = auxu->sig;
                    }

                    if (auxu == nullptr) {
                        MessageBox(hWnd, L"Usuario Incorrecto. Por favor, intentelo de nuevo", L"AVISO", MB_OK | MB_ICONWARNING);
                    }
                    else {
                        if (strcmp(userki, auxu->userkey) == 0) {
                            usr = auxu;
                            EndDialog(hWndLogin, 0);
                            DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_REGCARNET), NULL, (DLGPROC)vRegCarnet);
                        }
                        else {
                            MessageBox(hWnd, L"Contraseña Incorrecta. Por favor, intentalo de nuevo", L"AVISO", MB_OK | MB_ICONWARNING);
                        }
                    }
                }
            }break;

            case IDC_REGISTER: {
                DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_REGUSER), NULL, (DLGPROC)vRegUser);
            }break;

            case WM_CLOSE: {
                int opc = MessageBox(hWnd, L"Esta seguro que desea salir?", L"AVISO", MB_YESNO | MB_ICONQUESTION);
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
        }break;

        case WM_COMMAND: {
            menu1(LOWORD(wParam), hWnd);

            switch (LOWORD(wParam)) {

                case IDOK: {
                    USER* registru = new USER;
                    WCHAR tmps[100] = { 0 };
                    GetDlgItemText(hWnd, IDC_USERKER, (LPWSTR)s2ws(registru->userkey).c_str(), 20);
                    GetDlgItemText(hWnd, IDC_PASSWORR, (LPWSTR)s2ws(registru->password).c_str(), 20);
                    GetDlgItemText(hWnd, IDC_APATERNR, (LPWSTR)s2ws(registru->apaterno).c_str(), 15);
                    GetDlgItemText(hWnd, IDC_APATERNR, (LPWSTR)s2ws(registru->amaterno).c_str(), 15);
                    GetDlgItemText(hWnd, IDC_NOMBRER, (LPWSTR)s2ws(registru->nombres).c_str(), 30);
                    writeCarnet();
                    regUser(registru);

                    delete registru;

                    MessageBox(hWnd, L"Usuario Registrado", L"AVISO", MB_OK | MB_ICONINFORMATION);

                }break;

                case IDCANCEL: {

                    int opc = MessageBox(hWnd, L"Seguro que deseas salir?", L"AVISO", MB_YESNO | MB_ICONQUESTION);

                    switch (opc) {
                        case IDYES:
                            DestroyWindow(hWnd);
                            PostQuitMessage(0);
                        break;
                    }
                }break;
            }
        }break;

        case WM_CLOSE: {
            int opc = MessageBox(0, L"Seguro que desea salir?", L"AVISO", MB_YESNO | MB_ICONQUESTION);

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

BOOL CALLBACK vRegCarnet(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    hWndRegCarnet = hWnd;
    char numcar[10];
    switch (msg) {
        case WM_INITDIALOG: {
            DestroyWindow(hWnd);
        }break;

        case WM_COMMAND: {
            menu1(LOWORD(wParam), hWnd);

            switch (LOWORD(wParam)) {

                case IDORC: {
                    CARNET* registrc = new CARNET;
                    writeCarnet();
                    int numcarnet = rand();
                    GetDlgItemText(hWnd, IDC_NUMCAR, (LPWSTR)s2ws(numcar).c_str(), 10);
                    registrc->numcarnet = atoi(numcar);
                    GetDlgItemText(hWnd, IDC_CURPAT, (LPWSTR)s2ws(registrc->persocurp).c_str(), 16);
                    GetDlgItemText(hWnd, IDC_VACMARC, (LPWSTR)s2ws(registrc->vacuna).c_str(), 20);
                    GetDlgItemText(hWnd, IDC_RNOMDOS, (LPWSTR)s2ws(registrc->dosis).c_str(), 10);
                    GetDlgItemText(hWnd, IDC_RLOT, (LPWSTR)s2ws(registrc->lote).c_str(), 10);
                    GetDlgItemText(hWnd, IDC_RCENTVAC, (LPWSTR)s2ws(registrc->vacunacenter).c_str(), 50);
                    strcpy_s(registrc->fechadosis.dia, "");
                    strcpy_s(registrc->fechadosis.mes, "");
                    strcpy_s(registrc->fechadosis.ano, "");

                    regCarnet(registrc);

                    delete registrc;

                    MessageBox(hWnd, L"Carnet registrado", L"AVISO", MB_OK | MB_ICONINFORMATION);

                    delete registrc;
                }break;

                case IDCANCERC: {
                    int opc;
                    MessageBox(hWnd, L"Seguro que desea salir?", L"AVISO", MB_YESNO | MB_ICONQUESTION);

                    switch (opc) {
                    case IDYES:
                    DestroyWindow(hWnd);
                    PostQuitMessage(0);
                    break;
                    }

                }break;
            }
        }break;
    }
    return 0;
}

BOOL CALLBACK vDelCarnet(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    hWndDelCarnet = hWnd;
    char deldate[16] = { 0 };
    SYSTEMTIME fechadosis;
    switch (msg) {

        case WM_INITDIALOG: {

        }break;

        case WM_COMMAND: {
            menu1(wParam, hWnd);
            switch (LOWORD(wParam)) {

                char seardelcar[100];
                char carfecha[16];
                case IDC_EBUSCAR: {
                    GetDlgItemText(hWnd, IDC_ECURPAT, (LPWSTR)s2ws(seardelcar).c_str(), 100);

                    auxc = inicic;
                    while (auxp != nullptr && strcmp(seardelcar, auxc->persocurp) != 0) {
                        auxc = auxc->sig;
                    }

                    if (auxc == nullptr) {
                        MessageBox(hWnd, L"Carnet No Encontrado", L"AVISO", MB_OK | MB_ICONERROR);
                    }
                    else {
                        MessageBox(0, L"Producto encontrado", L"AVISO", MB_OK | MB_ICONINFORMATION);
                        SetDlgItemText(hWnd, IDC_EDCURPAT, (LPWSTR)s2ws(auxc->persocurp).c_str());
                        SetDlgItemText(hWnd, IDC_EMARVAC, (LPWSTR)s2ws(auxc->vacuna).c_str());
                        SetDlgItemText(hWnd, IDC_ENUMDOS, (LPWSTR)s2ws(auxc->dosis).c_str());

                        strcat_s(deldate, auxc->fechadosis.dia);
                        strcat_s(deldate, "%d");
                        strcat_s(deldate, auxc->fechadosis.mes);
                        strcat_s(deldate, "%d");
                        strcat_s(deldate, auxc->fechadosis.ano);
                        SetDlgItemText(hWnd, IDC_EFECDOS, (LPWSTR)s2ws(auxc->fechadosis).c_str());

                        SetDlgItemText(hWnd, IDC_ELOTE, (LPWSTR)s2ws(auxc->lote).c_str());
                        SetDlgItemText(hWnd, IDC_ECENVAC, (LPWSTR)s2ws(auxc->vacunacenter).c_str());
                        EnableWindow(GetDlgItem(hWnd, IDOD), TRUE);
                    }
                }break;

                case IDOD: {
                    int opc = MessageBox(0, L"Seguro que desea eliminar el carnet?", L"AVISO", MB_YESNO | MB_ICONQUESTION);
                    if (opc == IDYES) {
                        delCarnet(auxc);
                        writeCarnet();
                        SetWindowText(GetDlgItem(hWnd, IDC_EDCURPAT), L"");
                        SetWindowText(GetDlgItem(hWnd, IDC_EMARVAC), L"");
                        SetWindowText(GetDlgItem(hWnd, IDC_ENUMDOS), L"");
                        SetWindowText(GetDlgItem(hWnd, IDC_EFECDOS), L"");
                        SetWindowText(GetDlgItem(hWnd, IDC_ELOTE), L"");
                        SetWindowText(GetDlgItem(hWnd, IDC_ECENVAC), L"");
                        EnableWindow(GetDlgItem(hWnd, IDOD), FALSE);
                    }
                }break;

                case IDCANCED: {
                    hWndLogin = CreateDialog(hInstGlobal, MAKEINTRESOURCE(IDD_LOGIN), NULL, (DLGPROC)vLogin);
                    ShowWindow(hWndLogin, SW_SHOW);
                }break;
            }
        }break;

        case WM_CLOSE: {
            DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_REGCARNET), NULL, (DLGPROC)vRegCarnet);
        }break;
    }
    return 0;
}

BOOL CALLBACK vModCarnet(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    hWndModCarnet = hWnd;
    SYSTEMTIME fechadosis;
    switch (msg) {

        case WM_INITDIALOG: {

        }break;

        case WM_COMMAND: {
            menu1(wParam, hWnd);
            switch (LOWORD(wParam)) {
                case IDC_MBUSCAR: {
                    char searmodcar[100];
                    GetDlgItemText(hWnd, IDC_MCURPAT, (LPWSTR)s2ws(searmodcar).c_str(), 100);

                    auxc = inicic;
                    while (auxc != nullptr && strcmp(searmodcar, auxc->persocurp) != 0) {
                        auxc = auxc->sig;
                    }

                    if (auxc == nullptr) {
                        MessageBox(hWnd, L"Carnet No Encontrado", L"AVISO", MB_OK | MB_ICONWARNING);
                    }
                    else {
                        MessageBox(hWnd, L"Carnet Encontrado", L"AVISO", MB_OK | MB_ICONINFORMATION);
                        SetDlgItemText(hWnd, IDC_MOCURPAT, (LPWSTR)s2ws(auxc->persocurp).c_str());
                        SetDlgItemText(hWnd, IDC_MMARVAC, (LPWSTR)s2ws(auxc->vacuna).c_str());
                        SetDlgItemText(hWnd, IDC_MNUMDOS, (LPWSTR)s2ws(auxc->dosis).c_str());
                        SendDlgItemMessage(hWnd, IDC_MFECDOS, DTM_GETSYSTEMTIME, 0, (LPARAM)&date);
                        sprintf_s(auxc->fechadosis.dia, "%d", date.wDay);
                        sprintf_s(auxc->fechadosis.mes, "%d", date.wMonth);
                        sprintf_s(auxc->fechadosis.ano, "%d", date.wYear);
                        SetDlgItemText(hWnd, IDC_MLOTE, (LPWSTR)s2ws(auxc->lote).c_str());
                        SetDlgItemText(hWnd, IDC_MCENVAC, (LPWSTR)s2ws(auxc->vacunacenter).c_str());
                        EnableWindow(GetDlgItem(hWnd, IDOM), TRUE);
                    }
                }break;

                case IDOM: {
                    int opc = MessageBox(0, L"Seguro que desea modificar este carnet", L"AVISO", MB_YESNO | MB_ICONQUESTION);
                    if (opc == IDYES) {

                        writeCarnet();
                        GetDlgItemText(hWnd, IDC_MOCURPAT, (LPWSTR)s2ws(auxc->persocurp).c_str(), 16);
                        GetDlgItemText(hWnd, IDC_MMARVAC, (LPWSTR)s2ws(auxc->vacuna).c_str(), 20);
                        GetDlgItemText(hWnd, IDC_MNUMDOS, (LPWSTR)s2ws(auxc->dosis).c_str(), 20);
                        SendDlgItemMessage(hWnd, IDC_MFECDOS, DTM_GETSYSTEMTIME, 0, (LPARAM)&date);
                        sprintf(auxc->fechadosis.dia, "%d", date.wDay);
                        sprintf(auxc->fechadosis.mes, "%d", date.wMonth);
                        sprintf(auxc->fechadosis.ano, "%d", date.wYear);
                        GetDlgItemText(hWnd, IDC_MLOTE, (LPWSTR)s2ws(auxc->lote).c_str(), 10);
                        GetDlgItemText(hWnd, IDC_MCENVAC, (LPWSTR)s2ws(auxc->vacunacenter).c_str(), 50);

                        MessageBox(hWnd, L"Carnet Modificado", L"AVISO", MB_OK | MB_ICONINFORMATION);

                        SetWindowText(GetDlgItem(hWnd, IDC_MCURPAT), L"");
                        SetWindowText(GetDlgItem(hWnd, IDC_MOCURPAT), L"");
                        SetWindowText(GetDlgItem(hWnd, IDC_MMARVAC), L"");
                        SetWindowText(GetDlgItem(hWnd, IDC_MNUMDOS), L"");
                        SetWindowText(GetDlgItem(hWnd, IDC_MLOTE), L"");
                        SetWindowText(GetDlgItem(hWnd, IDC_MCENVAC), L"");
                        EnableWindow(GetDlgItem(hWnd, IDOM), FALSE);
                    }
                }break;
            }
        }break;

        case IDOM: {
            DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_REGCARNET), NULL, (DLGPROC)vRegCarnet);
        }break;
    }

    return 0;
}

BOOL CALLBACK vSearCarnet(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    SYSTEMTIME DateFileCarnet;
    switch (msg) {

        case WM_INITDIALOG: {
            EndDialog(hWndRegCarnet, 0);

            char fulldate[12] = { 0 };
            auxc = inicic;
            while (auxc != nullptr) {
                strcat_s(fulldate, auxc->fechadosis.dia);
                strcat_s(fulldate, "/");
                strcat_s(fulldate, auxc->fechadosis.mes);
                strcat_s(fulldate, "/");
                strcat_s(fulldate, auxc->fechadosis.ano);

                SendMessage(GetDlgItem(hWnd, IDC_BUNUMCAR), LB_ADDSTRING, 0, (LPARAM)auxc->numcarnet);
                SendMessage(GetDlgItem(hWnd, IDC_BUCURPAT), LB_ADDSTRING, 0, (LPARAM)auxc->persocurp);
                SendMessage(GetDlgItem(hWnd, IDC_BUMARVAC), LB_ADDSTRING, 0, (LPARAM)auxc->vacuna);
                SendMessage(GetDlgItem(hWnd, IDC_BUNUMDOS), LB_ADDSTRING, 0, (LPARAM)auxc->dosis);
                SendMessage(GetDlgItem(hWnd, IDC_BUFECDOS), LB_ADDSTRING, 0, (LPARAM)fulldate);
                SendMessage(GetDlgItem(hWnd, IDC_BULOTE), LB_ADDSTRING, 0, (LPARAM)auxc->lote);
                SendMessage(GetDlgItem(hWnd, IDC_BUCENVAC), LB_ADDSTRING, 0, (LPARAM)auxc->vacunacenter);
                strcpy_s(fulldate, "");

                auxc = auxc->sig;
            }
        }break;

        case WM_COMMAND: {
            menu1(wParam, hWnd);

            switch (LOWORD(wParam)) {
            case IDC_BBUSCAR: {
                char searcar[100];
                GetDlgItemText(hWnd, IDC_BNUMCAR, (LPWSTR)s2ws(searcar).c_str(), 100);

                readCarnet();

                auxc = inicic;
                while (auxc != nullptr && strcmp(searcar, auxc->persocurp) != 0) {
                    auxc = auxc->sig;
                }
                if (auxc == nullptr) {
                    MessageBox(0, L"Carnet NO Encontrado", L"AVISO", MB_OK | MB_ICONWARNING);
                }
                else {
                    MessageBox(0, L"Carnet Encontrado", L"AVISO", MB_OK | MB_ICONINFORMATION);
                    SetDlgItemInt(hWnd, IDC_BNUMCA, auxc->numcarnet, FALSE);
                    SetDlgItemText(hWnd, IDC_BCURPPAT, (LPWSTR)s2ws(auxc->persocurp).c_str());
                    SetDlgItemText(hWnd, IDC_BVACUNA, (LPWSTR)s2ws(auxc->vacuna).c_str());
                    SetDlgItemText(hWnd, IDC_BDOSIS, (LPWSTR)s2ws(auxc->dosis).c_str());
                    SetDlgItemText(hWnd, IDC_BLOT, (LPWSTR)s2ws(auxc->lote).c_str());
                    SetDlgItemText(hWnd, IDC_BVACCEN, (LPWSTR)s2ws(auxc->vacunacenter).c_str());
                }
            }break;
            }
        }break;

        case IDOB: {
            DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_REGCARNET), NULL, (DLGPROC)vRegCarnet);
        }break;
    }

    return 0;
}


BOOL CALLBACK vSearCarnet2(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    SYSTEMTIME DateFileCarnet;
    switch (msg) {

        case WM_INITDIALOG: {
            EndDialog(hWndRegCarnet, 0);

            char fulldate[12] = { 0 };
            auxc = inicic;
            while (auxc != nullptr) {
                strcat_s(fulldate, auxc->fechadosis.dia);
                strcat_s(fulldate, "/");
                strcat_s(fulldate, auxc->fechadosis.mes);
                strcat_s(fulldate, "/");
                strcat_s(fulldate, auxc->fechadosis.ano);

                SendMessage(GetDlgItem(hWnd, IDC_B2BUNUMCAR), LB_ADDSTRING, 0, (LPARAM)auxc->numcarnet);
                SendMessage(GetDlgItem(hWnd, IDC_B2BUCURPAT), LB_ADDSTRING, 0, (LPARAM)auxc->persocurp);
                SendMessage(GetDlgItem(hWnd, IDC_B2BUMARVAC), LB_ADDSTRING, 0, (LPARAM)auxc->vacuna);
                SendMessage(GetDlgItem(hWnd, IDC_B2BUNUMDOS), LB_ADDSTRING, 0, (LPARAM)auxc->dosis);
                SendMessage(GetDlgItem(hWnd, IDC_B2BUFECDOS), LB_ADDSTRING, 0, (LPARAM)fulldate);
                SendMessage(GetDlgItem(hWnd, IDC_B2BULOTE), LB_ADDSTRING, 0, (LPARAM)auxc->lote);
                SendMessage(GetDlgItem(hWnd, IDC_B2BUCENVAC), LB_ADDSTRING, 0, (LPARAM)auxc->vacunacenter);
                strcpy_s(fulldate, "");

                auxc = auxc->sig;
            }
        }break;

        case WM_COMMAND: {
            menu1(wParam, hWnd);

            switch (LOWORD(wParam)) {
                case IDC_B2BUSCAR: {
                    char searcar[100];
                    GetDlgItemText(hWnd, IDC_B2NUMCAR, (LPWSTR)s2ws(searcar).c_str(), 100);

                    CARNET* buscarCarnetEnArchivo(string persocurp); {
                        ifstream carnetfile;
                        carnetfile.open("file.txt", ios::binary | ios::in);
                        bool encontrado = false;
                        CARNET* krnet = new CARNET;
                        if (carnetfile.is_open()) {
                            //get length of file:
                            carnetfile.seekg(0, carnetfile.end);
                            int length = carnetfile.tellg();
                            carnetfile.seekg(0, carnetfile.beg);
                            int bytes = 0;
                            while (bytes <= length) {
                                char persocurp[16];
                                carnetfile.read((char*)krnet, sizeof(krnet));
                                if (strcmpi(krnet->persocurp, (LPWSTR)s2ws(persocurp).c_str())) {
                                    encontrado = true;
                                    break;
                                }
                                bytes += sizeof(CARNET);
                            }

                            if (!encontrado) {
                                delete krnet;
                                krnet = NULL;
                            }
                            carnetfile.close();
                        }
                    }
 
                    MessageBox(0, L"Carnet Encontrado", L"AVISO", MB_OK | MB_ICONINFORMATION);
                    SetDlgItemInt(hWnd, IDC_BNUMCA, auxc->numcarnet, FALSE);
                    SetDlgItemText(hWnd, IDC_BCURPPAT, (LPWSTR)s2ws(auxc->persocurp).c_str());
                    SetDlgItemText(hWnd, IDC_BVACUNA, (LPWSTR)s2ws(auxc->vacuna).c_str());
                    SetDlgItemText(hWnd, IDC_BDOSIS, (LPWSTR)s2ws(auxc->dosis).c_str());
                    SetDlgItemText(hWnd, IDC_BLOT, (LPWSTR)s2ws(auxc->lote).c_str());
                    SetDlgItemText(hWnd, IDC_BVACCEN, (LPWSTR)s2ws(auxc->vacunacenter).c_str());
                }break;
            }
        }break;

        case IDOB2: {
            hWndRegCarnet = CreateDialog(hInstGlobal, MAKEINTRESOURCE(IDD_REGCARNET), NULL, (DLGPROC)vRegCarnet);
            ShowWindow(hWndRegCarnet, SW_SHOW);
        }break;
    }

    return 0;
}

BOOL CALLBACK vRegPersona(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    hWndPersona = hWnd;
    SYSTEMTIME nacimiento;
    char identif[500], pertel[10];

    switch (msg) {
        case WM_INITDIALOG: {
            DestroyWindow(hWnd);
            EndDialog(hWndRegCarnet, 0);
        }break;

        case WM_COMMAND: {
            menu1(LOWORD(wParam), hWnd);

            switch (LOWORD(wParam)) {

                case IDOP: {
                    PERSONA* registrp = new PERSONA;
                    GetDlgItemText(hWnd, IDC_APATERNP, (LPWSTR)s2ws(registrp->apatern).c_str(), 15);
                    GetDlgItemText(hWnd, IDC_AMATERNP, (LPWSTR)s2ws(registrp->amatern).c_str(), 15);
                    GetDlgItemText(hWnd, IDC_NOMBREP, (LPWSTR)s2ws(registrp->pername).c_str(), 30);
                    GetDlgItemText(hWnd, IDC_CURPP, (LPWSTR)s2ws(registrp->percurp).c_str(), 16);
                    GetDlgItemText(hWnd, IDC_RFCP, (LPWSTR)s2ws(registrp->perfc).c_str(), 16);
                    GetDlgItemText(hWnd, IDC_CALLP, (LPWSTR)s2ws(registrp->percalle).c_str(), 30);
                    GetDlgItemText(hWnd, IDC_COLONIP, (LPWSTR)s2ws(registrp->percolon).c_str(), 30);
                    GetDlgItemText(hWnd, IDC_CITYP, (LPWSTR)s2ws(registrp->percity).c_str(), 40);
                    GetDlgItemText(hWnd, IDC_ESTADP, (LPWSTR)s2ws(registrp->perestado).c_str(), 30);
                    GetDlgItemText(hWnd, IDC_ESTACIVIP, (LPWSTR)s2ws(registrp->perestciv).c_str(), 20);
                    GetDlgItemText(hWnd, IDC_TELEFP, (LPWSTR)s2ws(pertel).c_str(), 10);
                    registrp->pertel = atoi(pertel);
                    GetDlgItemText(hWnd, IDC_IDENTIFP1, (LPWSTR)s2ws(registrp->peridentif).c_str(), 500);
                    GetDlgItemText(hWnd, IDC_SEXP, (LPWSTR)s2ws(registrp->persex).c_str(), 10);
                    GetDlgItemText(hWnd, IDC_GRUOCUP, (LPWSTR)s2ws(registrp->pergrupocup).c_str(), 20);
                    GetDlgItemText(hWnd, IDC_PERRIEP, (LPWSTR)s2ws(registrp->perfriesgo).c_str(), 20);
                    strcpy_s(registrp->fecnam.dia, "");
                    strcpy_s(registrp->fecnam.mes, "");
                    strcpy_s(registrp->fecnam.ano, "");
                    regPersona(registrp);

                    delete registrp;
                         
                    MessageBox(hWnd, L"Persona Registrada", L"AVISO", MB_OK | MB_ICONINFORMATION);

                }break;
                
                case IDC_IDENTIFP2: {
                    OPENFILENAME ofn;

                    char zFile[500];

                    ZeroMemory(&ofn, sizeof(ofn));
 
                    ofn.lStructSize = sizeof(ofn);
                    ofn.hwndOwner = hWnd;
                    ofn.lpstrFilter = "ALL\0 * .*\0Bitmaps\0 * .bmp\0";
                    ofn.lpstrFile = (LPWSTR)s2ws(zFile).c_str();
                    ofn.lpstrFile[0] = '\0';
                    ofn.nMaxFile = sizeof(zFile);
                    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
                    ofn.nFilterIndex = 2;

                    if (GetOpenFileName(&ofn) == TRUE) {
                        SetDlgItemText(hWnd, IDC_IDENTIFP1, (LPWSTR)s2ws(zFile).c_str());
                        //SetDindowText(GetDlgItem(hWnd, IDC_RPICPATH), zfile);
                        bmp = (HBITMAP)LoadImage(NULL, (LPWSTR)s2ws(zFile).c_str(), IMAGE_BITMAP, 200, 200, LR_LOADFROMFILE | LR_DEFAULTSIZE);
                        SendDlgItemMessage(hWnd, IDC_IDENTIFP3, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
                    }
                    else {
                        MessageBox(hWnd, L"No se ha seleccionado una imagen", L"AVISO", MB_OK | MB_ICONERROR);
                    }

                }break;

                case IDCANCEP: {

                        DestroyWindow(hWnd);
                        PostQuitMessage(0);
                }break;
            }
        }break;

        case WM_CLOSE: {
            int opc = MessageBox(0, L"Seguro que desea salir?", L"AVISO", MB_YESNO | MB_ICONQUESTION);

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

BOOL CALLBACK vRegVacuna(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    hWndVacuna = hWnd;
    switch (msg) {
        case WM_INITDIALOG: {
            DestroyWindow(hWnd);
        }break;
    

        case WM_COMMAND: {
            menu1(LOWORD(wParam), hWnd);

            switch (LOWORD(wParam)) {

                case IDOV: {
                    VACUNA* registrv = new VACUNA;

                    GetDlgItemText(hWnd, IDC_NOMDOS, (LPWSTR)s2ws(vcuna->numdosis).c_str(), 10);
                    GetDlgItemText(hWnd, IDC_TIPOVAC, (LPWSTR)s2ws(vcuna->vactipo).c_str(), 30);
                    GetDlgItemText(hWnd, IDC_MARCAVAC, (LPWSTR)s2ws(vcuna->marca).c_str(), 30);
                    GetDlgItemText(hWnd, IDC_NOMDOS, (LPWSTR)s2ws(vcuna->numdosis).c_str(), 10);
                    GetDlgItemText(hWnd, IDC_CLAVAC, (LPWSTR)s2ws(vcuna->vacode).c_str(), 10);
                    GetDlgItemText(hWnd, IDC_DESCVAC, (LPWSTR)s2ws(vcuna->vacdesc).c_str(), 100);
                    GetDlgItemText(hWnd, IDC_PREVAC, (LPWSTR)s2ws(vcuna->vacprize).c_str(), 5);

                    regVacuna(registrv);

                    MessageBox(hWnd, L"Vacuna registrada", L"AVISO", MB_OK | MB_ICONINFORMATION);

                    delete registrv;
                }break;

                case IDCANCEV: {
                        int opc = MessageBox(hWnd, L"Seguro que desea salir?", L"AVISO", MB_YESNO | MB_ICONQUESTION);

                        switch (opc) {
                        case IDYES:
                        DestroyWindow(hWnd);
                        PostQuitMessage(0);
                        break;
                        }
                }break;
            }
        }break;

        case WM_CLOSE: {

            int opc = MessageBox(0, L"Esta seguro que desea salir", L"SALIR", MB_YESNO | MB_ICONQUESTION);

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
            DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_REGCARNET), NULL, (DLGPROC)vRegCarnet);
        }break;

        case ID_CARNET_MODIFICARCARNET: {
            EndDialog(hWnd, 0);
            DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_MODCAR), NULL, (DLGPROC)vModCarnet);
        }break;

        case ID_CARNET_ELIMINARCARNET: {
            EndDialog(hWnd, 0);
            DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_DELCAR), NULL, (DLGPROC)vDelCarnet);
        }break;

        case ID_CARNET_BUSCARPORNUMERODECARNET: {
            EndDialog(hWnd, 0);
            DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_BUSCAR), NULL, (DLGPROC)vSearCarnet2);
        }break;

        case ID_CARNET_BUSCARPORNOMBREDEPERSONA: {
            EndDialog(hWnd, 0);
            DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_BUSCAR2), NULL, (DLGPROC)vSearCarnet2);
        }break;

        case ID_VACUNA_AGREGARVACUNA: {
            EndDialog(hWnd, 0);
            DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_REGVACUNA), NULL, (DLGPROC)vRegVacuna);
        }break;

        case ID_PERSONA_REGISTRARPERSONA: {
            EndDialog(hWnd, 0);
            DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_REGPERSONA), NULL, (DLGPROC)vRegPersona);
        }break;

        case ID_ACERCADE_SALIR: {
            DestroyWindow(hWnd);
            PostQuitMessage(0);
        }break;
    }
}

void regCarnet(CARNET* nuevo) {
    if (inicic = nullptr) {
        inicic = new CARNET;
        auxc = inicic;

        auxc->sig = nullptr;
        auxc->ant = nullptr;

        strcpy_s(auxc->persocurp, nuevo->persocurp);
        strcpy_s(auxc->vacuna, nuevo->vacuna);
        strcpy_s(auxc->dosis, nuevo->dosis);
        strcpy_s(auxc->fechadosis.dia, nuevo->fechadosis.dia);
        strcpy_s(auxc->fechadosis.mes, nuevo->fechadosis.mes);
        strcpy_s(auxc->fechadosis.ano, nuevo->fechadosis.ano);
        strcpy_s(auxc->lote, nuevo->lote);
        strcpy_s(auxc->vacunacenter, nuevo->vacunacenter);

        finc = auxc;
    }
    else {
        auxc = inicic;

        while (auxc->sig != nullptr) {
            auxc = auxc->sig;
        }

        auxc->sig = new CARNET;

        auxc->sig->sig = nullptr;
        auxc->sig->ant = auxc;

        auxc = auxc->sig;

        strcpy_s(auxc->persocurp, nuevo->persocurp);
        strcpy_s(auxc->vacuna, nuevo->vacuna);
        strcpy_s(auxc->dosis, nuevo->dosis);
        strcpy_s(auxc->lote, nuevo->lote);
        strcpy_s(auxc->vacunacenter, nuevo->vacunacenter);

        finc = auxc;
    }
}

void delCarnet(CARNET* auxc) {
    char cardel[100];
    auxc = inicic;

    if (auxc == nullptr)
    {
        MessageBox(hWndSearCarnet, L"No hay carnets todavia.", L"AVISO", MB_OK | MB_ICONWARNING);
    }
    else {
        while (auxc != nullptr && strcmp(auxc->persocurp, cardel) != 0) {
            auxc = auxc->sig;
        }

        if (auxc == nullptr)
        {
            MessageBox(hWndSearCarnet, L"Carnet no encontrado", L"AVISO", MB_OK | MB_ICONWARNING);
        }
        else if (auxc == inicic) {
            if (auxc->sig == nullptr) {
                delete auxc;

                inicic = nullptr;
                auxc = inicic;
            }
            else {
                inicic = auxc->sig;

                delete auxc;

                auxc = inicic;
                auxc->ant = nullptr;
            }
            MessageBox(hWndSearCarnet, L"Carnet Eliminado", L"AVISO", MB_OK | MB_ICONINFORMATION);
        }
        else {
            if (auxc->sig == nullptr) {
                auxc->ant->sig = nullptr;
            }
            else {
                auxc->sig->ant = auxc->ant;
                auxc->ant->sig = auxc->sig;
            }

            delete auxc;
            auxc = inicic;

            MessageBox(hWndSearCarnet, L"Carnet Eliminado", L"AVISO", MB_OK | MB_ICONINFORMATION);
        }
    }
}

void modCarnet(CARNET* carnetmod) {
    char krnwtmod[100];
    auxc = inicic;

    if (auxc == nullptr) {
        MessageBox(hWndSearCarnet, L"No hay carnetes registrados todavia. Anada un nuevo carnet.", L"AVISO", MB_OK | MB_ICONWARNING);
    }
    else {
        while (auxp != nullptr && strcmp(auxc->persocurp, krnwtmod) != 0) {
            auxc = auxc->sig;
        }
        if (auxc == nullptr) {
            MessageBox(hWndSearCarnet, L"Carnet no encontrado", L"AVISO", MB_OK | MB_ICONINFORMATION);
        }
        else {

            auxc->numcarnet = krnwtmod->numcarnet;
            strcpy(auxc->persocurp, krnwtmod->persocurp);
            strcpy(auxc->vacuna, krnwtmod->vacuna);
            strcpy(auxc->dosis, krnwtmod->dosis);
            strcpy(auxc->fechadosis.dia, krnwtmod->fechadosis.dia);
            strcpy(auxc->fechadosis.mes, krnwtmod->fechadosis.mes);
            strcpy(auxc->fechadosis.ano, krnwtmod->fechadosis.ano);
            strcpy(auxc->lote, krnwtmod->lote);
            strcpy(auxc->vacunacenter, krnwtmod->vacunacenter);

            MessageBox(hWndSearCarnet, L"Carnet modificado", L"AVISO", MB_OK | MB_ICONINFORMATION);
        }
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
        iniciv = new VACUNA;
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

        finv = auxv;
    }
}

void writeCarnet() {
    auxc = inicic;

    ofstream carnetwrite;
    carnetwrite.open("C\\Users\\José Miguel\\source\\repos\\1985194GonzalezCastilleroJoseMiguelEDDPROYECTO\\carnet.txt", ios::out | ios::app);

    if (carnetwrite.is_open()) {
        while (auxc != nullptr) {
            carnetwrite.write((char*)auxc, sizeof(CARNET));
            auxc = auxc->sig;
        }

        carnetwrite.close();
    }
}

void readCarnet() {
    auxc = inicic;

    ifstream carnetfile;
    carnetfile.open("C\\Users\\José Miguel\\source\\repos\\1985194GonzalezCastilleroJoseMiguelEDDPROYECTO\\carnet.txt", ios::in | ios::app);

    if (carnetfile.is_open()) {
        CARNET* readcarnet = new CARNET;

        while (carnetfile.read((char*)readcarnet, sizeof(CARNET)).eof()) {
            while (auxc != nullptr && auxc->sig != nullptr) {
                auxc = auxc->sig;
            }
            if (auxc == nullptr) {
                inicic = readcarnet;
                inicic->sig = nullptr;
                inicic->ant = nullptr;
                auxc = inicic;
                finc = auxc;
            }
            else {
                auxc->sig = readcarnet;
                auxc->sig->sig = auxc;
                auxc = auxc->sig;
                auxc->sig = nullptr;
                finc = auxc;
            }

            readcarnet = new CARNET;
        }

        carnetfile.close();

    }
}

void writeUser() {
    auxu = iniciu;

    ofstream userwrite;
    userwrite.open("C\\Users\\José Miguel\\source\\repos\\1985194GonzalezCastilleroJoseMiguelEDDPROYECTO\\user.txt", ios::out | ios::app);

    if (userwrite.is_open()) {
        while (auxu != nullptr) {
            userwrite.write((char*)auxu, sizeof(USER));
            auxu = auxu->sig;
        }

        userwrite.close();
    }
}

void readUser() {
    auxu = iniciu;

    ifstream userfile;
    userfile.open("C\\Users\\José Miguel\\source\\repos\\1985194GonzalezCastilleroJoseMiguelEDDPROYECTO\\user.txt", ios::in | ios::app);

    if (userfile.is_open()) {
        USER* readuser = new USER;

        while (userfile.read((char*)readuser, sizeof(USER)).eof()) {
            while (auxu != nullptr && auxu->sig != nullptr) {
                auxu = auxu->sig;
            }
            if (auxu == nullptr) {
                iniciu = readuser;
                iniciu->sig = nullptr;
                iniciu->ant = nullptr;
                auxu = iniciu;
                finu = auxu;
            }
            else {
                auxu->sig = readuser;
                auxu->sig->sig = auxu;
                auxu = auxu->sig;
                auxu->sig = nullptr;
                finu = auxu;
            }

            readuser = new USER;
        }

        userfile.close();

    }
}

void writePersona() {
    auxp = inicip;

    ofstream personwrite;
    personwrite.open("C\\Users\\José Miguel\\source\\repos\\1985194GonzalezCastilleroJoseMiguelEDDPROYECTO\\person.txt", ios::out | ios::app);

    if (personwrite.is_open()) {
        while (auxp != nullptr) {
            personwrite.write((char*)auxp, sizeof(CARNET));
            auxp = auxp->sig;
        }

        personwrite.close();
    }
}

void readPersona() {
    auxp = inicip;

    ifstream personfile;
    personfile.open("C\\Users\\José Miguel\\source\\repos\\1985194GonzalezCastilleroJoseMiguelEDDPROYECTO\\person.txt", ios::in | ios::app);

    if (personfile.is_open()) {
        PERSONA* readperson = new PERSONA;

        while (personfile.read((char*)readperson, sizeof(PERSONA)).eof()) {
            while (auxp != nullptr && auxp->sig != nullptr) {
                auxp = auxp->sig;
            }
            if (auxp == nullptr) {
                inicip = readperson;
                inicip->sig = nullptr;
                inicip->ant = nullptr;
                auxp = inicip;
                finp = auxp;
            }
            else {
                auxp->sig = readperson;
                auxp->sig->sig = auxp;
                auxp = auxp->sig;
                auxp->sig = nullptr;
                finp = auxp;
            }

            readperson = new PERSONA;
        }

        personfile.close();

    }
}

void quicksort(int* A, int izq, int der) {

    int pivote = A[izq];
    int i = izq;
    int j = der;
    int aux;

    while (i < j) {
        while (A[i] <= pivote && i < j) i++;
        while (A[i] > pivote) j--;
        if (i < j) {
            aux = A[i];
            A[i] = A[j];
            A[j] = aux;
        }

        A[izq] = A[j];
        A[j] = pivote;

        if (izq < j - 1)
            quicksort(A, izq, j - 1);
        if (j + 1 < der)
            quicksort(A, j + 1, der);
    }
}

wstring s2ws(string& s) {
    int len;
    int slen = s.length() + 1;
    len = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), slen, 0, 0);
    wstring buf;
    buf.resize(len);
    MultiByteToWideChar(CP_UTF8, 0, s.c_str(), slen, const_cast<wchar_t*>(buf.c_str()), len);
    return buf;
}

wstring s2ws(char *c) {
    string s(c);
    int len;
    int slen = s.length() + 1;
    len = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), slen, 0, 0);
    wstring buf;
    buf.resize(len);
    MultiByteToWideChar(CP_UTF8, 0, s.c_str(), slen, const_cast<wchar_t*>(buf.c_str()), len);
    return buf;
}