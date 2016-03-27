#pragma once
#include <atlimage.h>

// CSurfaceTexStatic
class CSurfaceTexStatic : public CStatic
{
	DECLARE_DYNAMIC(CSurfaceTexStatic)

public:
	CSurfaceTexStatic( const CString& strTextureName );
	virtual ~CSurfaceTexStatic();

	CString m_strTextureName;
	CImage m_kImage;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};


