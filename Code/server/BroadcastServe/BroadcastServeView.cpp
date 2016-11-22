
// BroadcastServeView.cpp : CBroadcastServeView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "BroadcastServe.h"
#endif

#include "BroadcastServeDoc.h"
#include "BroadcastServeView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBroadcastServeView

IMPLEMENT_DYNCREATE(CBroadcastServeView, CView)

BEGIN_MESSAGE_MAP(CBroadcastServeView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CBroadcastServeView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CBroadcastServeView ����/����

CBroadcastServeView::CBroadcastServeView()
{
	// TODO: �ڴ˴���ӹ������

}

CBroadcastServeView::~CBroadcastServeView()
{
}

BOOL CBroadcastServeView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CBroadcastServeView ����

void CBroadcastServeView::OnDraw(CDC* /*pDC*/)
{
	CBroadcastServeDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CBroadcastServeView ��ӡ


void CBroadcastServeView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CBroadcastServeView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CBroadcastServeView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CBroadcastServeView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}

void CBroadcastServeView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CBroadcastServeView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CBroadcastServeView ���

#ifdef _DEBUG
void CBroadcastServeView::AssertValid() const
{
	CView::AssertValid();
}

void CBroadcastServeView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CBroadcastServeDoc* CBroadcastServeView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBroadcastServeDoc)));
	return (CBroadcastServeDoc*)m_pDocument;
}
#endif //_DEBUG


// CBroadcastServeView ��Ϣ�������
