#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#endif

// Windows Header Files
#include <windows.h>

// Kinect Header files
#include <Kinect.h>
#include <Shlobj.h>

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

// Safe release for interfaces
template<class Interface>
inline void SafeRelease(Interface *& pInterfaceToRelease)
{
	if (pInterfaceToRelease != NULL)
	{
		pInterfaceToRelease->Release();
		pInterfaceToRelease = NULL;
	}
}

#include <QMutex>
#include <QSize>
#include <QThread>
#include <QWaitCondition>
#include <QImage>


struct KinectFrameBuffer
{
	std::vector<unsigned short> info; // Frame Information
	std::vector<unsigned char> color; // ColorFrame Data Buffer
	std::vector<unsigned short> depth; // DepthFrame Data Buffer

	// info : [0] color_width
	// info : [1] color_height
	// info : [2] color_channels
	// info : [3] depth_width
	// info : [4] depth_width
	// info : [5] depth_min_distance
	// info : [6] depth_max_distance
	enum
	{
		ColorWidth = 0, 
		ColorHeight,
		ColorChannels,
		DepthWidth,
		DepthHeight,
		DepthMinDistance,
		DepthMaxDistance
	};
	
	KinectFrameBuffer()
	{
		reset();
	}

	KinectFrameBuffer(const std::vector<unsigned short>& _info,
		const std::vector<unsigned char>& _color,
		const std::vector<unsigned short>& _depth) :
		info(_info), color(_color), depth(_depth){}

	void reset()
	{
		info.resize(7, 0);
		color.resize(1920 * 1080 * 4, 0);
		depth.resize(512 * 424, 0);
	}

	void clear()
	{
		info.clear();
		color.clear();
		depth.clear();
	}

	unsigned short color_width() const { return info[ColorWidth]; }
	unsigned short color_height() const { return info[ColorHeight]; }
	unsigned short depth_width() const { return info[DepthWidth]; }
	unsigned short depth_height() const { return info[DepthHeight]; }
	unsigned short depth_min_distance() const { return info[DepthMinDistance]; }
	unsigned short depth_max_distance() const { return info[DepthMaxDistance]; }
};

class QKinectGrabber : public QThread
{
	Q_OBJECT

public:
	QKinectGrabber(QObject *parent = 0);
	~QKinectGrabber();

	void enableImageSending(bool value);

	void copyFrameBuffer(KinectFrameBuffer& frame);

	void getColorData(	signed __int64& timespan, 
						unsigned short& width, 
						unsigned short& height, 
						unsigned short& channels);

	void copyColorBuffer(std::vector<unsigned char>& buffer,
						 std::vector<unsigned char>::iterator position);


	void getDepthData(	signed __int64& timespan,
						std::vector<unsigned short>& info);

	void getDepthData(	signed __int64& timespan, 
						unsigned short& width, 
						unsigned short& height, 
						unsigned short& minDistance, 
						unsigned short& maxDistance);

	void getDepthBuffer(std::vector<unsigned short>& info,
						std::vector<unsigned short>& buffer);

	
	void copyDepthBuffer(std::vector<unsigned short>& buffer,
						 std::vector<unsigned short>::iterator position);


public slots:
	void stop();

signals:
	void colorImage(const QImage &image);
	void depthImage(const QImage &image);
	void infraredImage(const QImage &image);
	void frameUpdated();

protected:
	void run() Q_DECL_OVERRIDE;

private:
	bool initializeSensor();
	bool updateColor();
	bool updateDepth();
	bool updateInfrared(QImage& infraredImage);
	void uninitializeSensor();
	
	IKinectSensor*				m_pKinectSensor;		// Current Kinect	
	IColorFrameReader*			m_pColorFrameReader;	// Color reader	
	IDepthFrameReader*			m_pDepthFrameReader;	// Depth reader	
	IInfraredFrameReader*		m_pInfraredFrameReader;	// Infrared reader

	unsigned short				colorFrameWidth;		// = 1920;
	unsigned short				colorFrameHeight;		// = 1080;
	const unsigned short		colorFrameChannels;		// = 4;
	signed __int64				colorFrameTime;			// timestamp

	unsigned short				depthFrameWidth;		// = 512;
	unsigned short				depthFrameHeight;		// = 424;
	signed __int64				depthFrameTime;			// timestamp

	unsigned short				depthMinReliableDistance;
	unsigned short				depthMaxDistance;

	std::vector<unsigned char>	colorBuffer;
	std::vector<unsigned short>	depthBuffer;

	bool						emitImageEnabled;
	QVector<QRgb>				colorTable;

	QMutex						mutex;
	bool						running;
};



