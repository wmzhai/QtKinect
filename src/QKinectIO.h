#pragma once

#include <QObject>
#include "QKinectGrabber.h"

class QKinectIO
{

public:
	QKinectIO();
	virtual ~QKinectIO();

	void setKinecReader(QKinectGrabber* kinect);
	
	void setMaxSizeInMegaBytes(unsigned int size_in_Mbytes);

	void copyDepthFrame(std::vector<unsigned short>& info, 
						std::vector<unsigned short>& buffer, 
						std::vector<unsigned short>::iterator position, 
						unsigned int frame_index);

	void appendFrame();
	void saveFrame(const QString& filename);
	void save(const QString& filename);
	void load(const QString& filename);

	

	void clear();
	unsigned int size() const;

	static void save(QString filename, std::vector<unsigned short> info, std::vector<unsigned short> depthBuffer);
	static void load(QString filename, std::vector<unsigned short>& info, std::vector<unsigned short>& depthBuffer);
	
	static void exportObj(QString filename, std::vector<unsigned short> info, std::vector<unsigned short> depthBuffer);


	static void loadFrame(const std::string& filename, KinectFrameBuffer& frame);

	static void saveFrame(const std::string& filename, const KinectFrameBuffer& frame);

	static void save(QString filename, std::vector<unsigned short> info, std::vector<unsigned char> color_buffer, std::vector<unsigned short> depthBuffer);

private:

	QKinectGrabber*				kinectReader;
	std::vector<unsigned short> depthBufferStream;
	unsigned int				maxSizeInMegaBytes;
};


