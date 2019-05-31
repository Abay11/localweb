#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include <QLabel>

namespace VideoModule
{
	class Camera;

	class VideoWidget : public QLabel
	{
		Q_OBJECT
	public:
		VideoWidget(QWidget* parent = nullptr);

		void setCameraInstance(Camera* cam);

		~VideoWidget() override;

		void setCallerNick(QString nick);

	signals:
		void videoWidgetClosed(QString caller);

	public slots:
		void slotDraw(QByteArray& data);


	private:
		QSize size;

		Camera* cam;

		QString caller_nick;

		const QString DTAG = "VideoModule::VideoWidget:";

		// QWidget interface
	protected:
		void closeEvent(QCloseEvent* event) override;

		void showEvent(QShowEvent* event) override;
	};
}

#endif // VIDEOWIDGET_H
