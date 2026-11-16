#define UNICODE
#define _UNICODE
#include <windows.h>
#include <string>

namespace {
constexpr COLORREF BG = RGB(13, 23, 37);
constexpr COLORREF PANEL = RGB(24, 39, 56);
constexpr COLORREF MINT = RGB(112, 225, 181);
constexpr COLORREF WHITE = RGB(244, 247, 251);
constexpr COLORREF MUTED = RGB(157, 174, 192);

void text(HDC dc, int x, int y, const wchar_t* value, int size, COLORREF color, bool bold=false) {
    HFONT font = CreateFontW(-size,0,0,0,bold?FW_BOLD:FW_NORMAL,FALSE,FALSE,FALSE,
        DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,CLEARTYPE_QUALITY,
        DEFAULT_PITCH|FF_DONTCARE,L"Segoe UI");
    auto old = SelectObject(dc,font); SetBkMode(dc,TRANSPARENT); SetTextColor(dc,color);
    TextOutW(dc,x,y,value,lstrlenW(value)); SelectObject(dc,old); DeleteObject(font);
}
void round_panel(HDC dc, RECT r, COLORREF fill, COLORREF edge) {
    HBRUSH b=CreateSolidBrush(fill); HPEN p=CreatePen(PS_SOLID,1,edge);
    auto ob=SelectObject(dc,b); auto op=SelectObject(dc,p);
    RoundRect(dc,r.left,r.top,r.right,r.bottom,16,16);
    SelectObject(dc,ob); SelectObject(dc,op); DeleteObject(b); DeleteObject(p);
}
void logo(HDC dc,int x,int y) {
    HPEN p=CreatePen(PS_SOLID,4,MINT); auto old=SelectObject(dc,p);
    HBRUSH hollow=(HBRUSH)GetStockObject(HOLLOW_BRUSH); auto ob=SelectObject(dc,hollow);
    RoundRect(dc,x,y,x+40,y+40,10,10); Ellipse(dc,x+12,y+8,x+28,y+32);
    SelectObject(dc,ob); SelectObject(dc,old); DeleteObject(p);
}
LRESULT CALLBACK WndProc(HWND hwnd,UINT msg,WPARAM wp,LPARAM lp) {
    if(msg==WM_PAINT) {
        PAINTSTRUCT ps{}; HDC dc=BeginPaint(hwnd,&ps); RECT client{}; GetClientRect(hwnd,&client);
        HBRUSH bg=CreateSolidBrush(BG); FillRect(dc,&client,bg); DeleteObject(bg);
        logo(dc,34,28); text(dc,88,28,L"HelloFace",27,WHITE,true);
        text(dc,88,62,L"WINDOWS FACE MATCHING  /  PREVIEW BUILD",10,MUTED,true);
        text(dc,34,112,L"Profile management",25,WHITE,true);
        text(dc,34,151,L"Manage up to four local profile slots. Biometric capture is not enabled in this preview.",13,MUTED);
        const wchar_t* names[4]={L"Slot 01",L"Slot 02",L"Slot 03",L"Slot 04"};
        const wchar_t* status[4]={L"EMPTY",L"EMPTY",L"EMPTY",L"EMPTY"};
        for(int i=0;i<4;i++) {
            int x=34+(i%2)*440, y=194+(i/2)*142; RECT r{x,y,x+414,y+120};
            round_panel(dc,r,PANEL,RGB(48,67,86));
            text(dc,x+20,y+17,names[i],17,WHITE,true);
            text(dc,x+20,y+51,status[i],10,MINT,true);
            text(dc,x+20,y+78,L"No face template enrolled",12,MUTED);
            RECT badge{x+328,y+39,x+390,y+76}; round_panel(dc,badge,RGB(30,61,58),RGB(49,104,88));
            text(dc,x+344,y+51,L"EMPTY",10,MINT,true);
        }
        RECT info{34,500,914,570}; round_panel(dc,info,RGB(28,48,64),RGB(54,83,103));
        text(dc,54,515,L"PREVIEW MODE",11,MINT,true);
        text(dc,54,538,L"Camera, ONNX inference, TPM vault and Windows sign-in are not implemented yet.",12,WHITE);
        text(dc,34,594,L"PIN and password sign-in remain your fallback. Never disable Windows security providers.",11,MUTED);
        EndPaint(hwnd,&ps); return 0;
    }
    if(msg==WM_ERASEBKGND) return 1;
    if(msg==WM_DESTROY){PostQuitMessage(0);return 0;}
    return DefWindowProcW(hwnd,msg,wp,lp);
}
}
int WINAPI wWinMain(HINSTANCE instance,HINSTANCE, PWSTR,int show) {
    WNDCLASSW wc{}; wc.lpfnWndProc=WndProc; wc.hInstance=instance;
    wc.lpszClassName=L"HelloFacePreviewWindow"; wc.hCursor=LoadCursor(nullptr,IDC_ARROW);
    wc.hbrBackground=(HBRUSH)(COLOR_WINDOW+1); RegisterClassW(&wc);
    HWND hwnd=CreateWindowExW(0,wc.lpszClassName,L"HelloFace Preview",WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX,
        CW_USEDEFAULT,CW_USEDEFAULT,980,690,nullptr,nullptr,instance,nullptr);
    if(!hwnd)return 1; ShowWindow(hwnd,show); UpdateWindow(hwnd);
    MSG msg{}; while(GetMessageW(&msg,nullptr,0,0)>0){TranslateMessage(&msg);DispatchMessageW(&msg);}
    return static_cast<int>(msg.wParam);
}
