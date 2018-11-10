#include "clientservice.h"

void ClientService::save()
{
// QFile file("data.bin");
// if(file.open(QFile::WriteOnly))
//	{
//	 QDataStream out(&file);
//	 //at first save current user info
//	 out<<usernick<<username;

//	 //then save server address and port
//	 out<<*pserverAddress<<*pserverPort;
//	 qDebug()<<"saving address info: "<<*pserverAddress<<" "<<*pserverPort;

//	 //then if clientbase is not empty save it too
//	 if(!clients.isEmpty())
//		out<<clients;

//	 file.close();
//	}
// else
//	qCritical()<<"Error saving clients base";
}

void ClientService::restore()
{
// QFile file("data.bin");
// if(file.open(QFile::ReadOnly))
//	{
//	 QDataStream in(&file);
//	 //at first read current user info
//	 in>>usernick>>username;

//	 //then read server address and port
//	 //and update only if they not null (after registration they may be null)
//	 QString addr, port;
//	 in>>addr>>port;
//	 if(addr!="")
//		{
//		 *pserverAddress=addr;
//		pleAddress->setText(*pserverAddress);
//		}
//	 if(port!="")
//		{
//		 *pserverPort=port;
//		 plePort->setText(port);
//		}

//	 //then other clients
//	 in>>clients;
//	 file.close();

//		ponlineList->addItem(new QListWidgetItem(QIcon(":/Res/Icons/online.png"), QString("Вы: %1").arg(usernick)));

//	 for(auto iter=clients.begin(), end=clients.end(); iter!=end; ++iter)
//		if(usernick!=iter.key())
//			pofflineList->addItem(new QListWidgetItem(QIcon(":/Res/Icons/offline.png"), iter.key()));
//	}
 //сразу после регистрации не создается файл сохранения
 //поэтому, чтобы лишний раз не выбрасывать ошибку
 //сперва проверяется существует ли вообще сохранения
 //и только если оно существует, и не удалось ее прочитать
 //кидать ошибку
// else if(QFile::exists(file.fileName()))
//	qWarning()<<"Error restoring clients base";
}

ClientService::ClientService(QWidget *prnt)
 :QObject (prnt)
 ,pparent(prnt)
 ,psocket(new QTcpSocket)
 ,pserverAddress(new QString)
 ,pserverPort(new QString)
 ,ponline(new QStringListModel)
 ,poffline(new QStringListModel)
{
 connect(psocket, SIGNAL(connected()), SLOT(slotConnected()));
 connect(psocket, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(slotError(QAbstractSocket::SocketError)));


}

void ClientService::slotConnected()
{
 // pinfo->append(QDateTime::currentDateTime().toString("[hh:mm:ss] ")
 //							 +"Соединение с сервером установлено.");
 // qInfo()<<"Соединение с сервером установлено.";

	//запрос серверу чтобы тот прислал список клиентов
//	QByteArray arrBlock;
//	QDataStream out(&arrBlock, QIODevice::WriteOnly);

//	out.setVersion(QDataStream::Qt_5_11);

//	out<<quint16(0)<<static_cast<int>(DATATYPE::CONNECT)
//		<<usernick
//	 <<clients.size();
//	out.device()->seek(0);
//	out<<quint16(static_cast<size_t>(arrBlock.size())-sizeof(quint16));

//	pserverSocket->write(arrBlock);
//	qInfo()<<"Отправлен запрос на получение списка";
}

void ClientService::slotReadyRead()
{
 QDataStream in(psocket) ;
 in.setVersion(QDataStream::Qt_5_11);
 forever
 {
	if(!mnNextBlockSize)
	 {
		if(static_cast<size_t>(psocket->bytesAvailable())<sizeof(quint16))
		 break;

		in>>mnNextBlockSize;
	 }

	if(psocket->bytesAvailable()<mnNextBlockSize)
	 break;

	QTime time;
	QString msg;

	DATATYPE type;
	in>>type;
	qDebug()<<"новое событие ";

	switch (type) {
	 case DATATYPE::CONNECT:
	 {
		 qDebug()<<"Receiving client's list";
		 //получение списка пользователей от сервера

		 int serverBaseSize;
		 in>>serverBaseSize;
		 //если база у сервера не совпадает с нашим,
		 //получаем актуальную базу
		 if(serverBaseSize!=clients.size())
			{
			 qDebug()<<"короче тут у нас база не сошлась, поэтому ждем новый";
			in>>clients;
			}

		 //нужно получить количество доступных
		 int count=0;
		 in>>count;

		 for(int i=0; i<count; ++i)
			{
			 QString nick;
			 in>>nick;
//			 onlines.push_back(nick);
			}

		 //кидаем полученный список онлайнов в список доступных
//		 for(auto i : onlines)
//			 if(i!=usernick)
//				ponlineList->addItem(new QListWidgetItem(QIcon(":/Res/Icons/online.png"),i));


		 //очищаем список недоступных
		 //и начинаем заного добавлять туда
		 //только не имеющихся в полученном списке-онлайн
//			 pofflineList->clear();
//			 for(auto iter=clients.begin(),
//					 end=clients.end(); iter!=end; ++iter)
//				if(usernick!=iter.key()
//					 && !onlines.contains(iter.key()))
//				 pofflineList->addItem(new QListWidgetItem(QIcon(":/Res/Icons/offline.png"), iter.key()));

		 break;
	 }
	 case DATATYPE::DISCONNECTION:
		{
		 qDebug()<<"кто-то отсоединился";
		 QString disconnected;
		 in>>disconnected;
		 qDebug()<<disconnected<<" отсоединился";
//		 onlines.removeOne(disconnected);

//		 ponlineList->findItems(disconnected, Qt::MatchExactly).first();
//		 for(auto i=0, s=ponlineList->count();
//				 i<s; ++i)
//			{
//			 if(ponlineList->item(i)->text()==disconnected)
//				{
//				 delete ponlineList->takeItem(i);
//				 break;
//				}
//			}

//		 pofflineList->addItem(new QListWidgetItem(QIcon(":/Res/Icons/offline.png"),disconnected));
//		 pofflineList->sortItems();

		 QString msg=disconnected;
		 msg+=" вышел";

//		 if(parentWidget()->parentWidget()->isMinimized()
//				|| parentWidget()->parentWidget()->isHidden())
//			ptray->showMessage("Новое событие", msg, QSystemTrayIcon::Information, 3000);
//		 else
//			popup->showNotify(std::move(msg), mapToGlobal(pos()));

		 break;
		}
	 case DATATYPE::NOTIFYING:
		{
		 qDebug()<<"есть новый подсоединившийся";
		 QString connected;
		 in>>connected;
		 //убираем из недоступных подсоединившегося
//		 for(auto i=0;
//				 i<pofflineList->count(); ++i)
//			{
//			 if(pofflineList->item(i)->text()==connected)
//				{
//				delete pofflineList->takeItem(i);
//				 break;
//				}
//			}

		 //и добавляем к доступным
//		 ponlineList->addItem(new QListWidgetItem(QIcon(":/Res/Icons/online.png"), connected));
//		 ponlineList->sortItems();

		 QString msg=connected;
		 msg+=" доступен";

//		 if(parentWidget()->parentWidget()->isMinimized()
//				|| parentWidget()->parentWidget()->isHidden())
//			ptray->showMessage("Новое событие", msg, QSystemTrayIcon::Information, 3000);
//		 else
//			popup->showNotify(std::move(msg), mapToGlobal(pos()));


		 break;
		}
	 case DATATYPE::MESSAGE:
	 {
		 //новое сообщение
		 qDebug()<<"новое  сообщение";
		 in>>time>>msg;
		 msg.prepend(time.toString("[hh:mm:ss] "));
//		 msg=time.toString("[hh:mm:ss] ")+"Новое сообщение: "+msg;
//		 pinfo->append(msg);

//		 if(parentWidget()->parentWidget()->isMinimized()
//				|| parentWidget()->parentWidget()->isHidden())
//			ptray->showMessage("Новое событие", "Новое сообщение", QSystemTrayIcon::Information, 3000);
//		 else
//			popup->showNotify("***Новое сообщение***", mapToGlobal(pos()));

		 break;
	 }
	 default:
		//что-то пошло не так, клиент не может получить иную команду
		 qDebug()<<"что-то пошло не так я полагаю";
		 qDebug()<<"но что?!?!";
		 qDebug()<<"но это уже не моя забота";

		qCritical()<<"Неизвестная ошибка при получении сообщения.";
	 }

	mnNextBlockSize=0;
 }
}

void ClientService::slotError(QAbstractSocket::SocketError nerr)
{
 QString info;
 switch(nerr)
	{
	case QAbstractSocket::HostNotFoundError:
	 info.append("Удаленный сервер не найден. "
							 "Удостоверьте, что введен правильный адрес и порт сервера.");
	 break;
	case QAbstractSocket::RemoteHostClosedError:
	 info.append("Удаленный сервер закрыл соединение. "
							 "Обратитесь к администратору сети.");
	 break;
	case QAbstractSocket::ConnectionRefusedError:
	 info.append("Удаленный сервер отказал в доступе. "
							 "Обратитесь к администратору сети.");
	 break;
	default:
	 info.append("Произошла критическая ошибка. "
							 "Обратитесь к администратору сети.");
	}

	QMessageBox::critical(pparent,
											 "Ошибка подключения к серверу", info);

// pcmdDisconnect->setEnabled(false);
// pcmdConnect->setEnabled(true);

	qCritical()<<"Ошибка соединения с сервером: "<<psocket->errorString();
	psocket->close();
}

void ClientService::slotConnectToServer()
{
 psocket->connectToHost(*pserverAddress,
												static_cast<quint16>(pserverPort->toInt()));
}

void ClientService::slotDisconnectFromServer()
{
 psocket->close();

// ponline->removeRows(0, ponline->rowCount()-1);
}

void ClientService::slotSentToServer(QString msg)
{
 QByteArray arrBlock;
 QDataStream out(&arrBlock, QIODevice::WriteOnly);

 out.setVersion(QDataStream::Qt_5_11);

 out<<quint16(0)
	 <<static_cast<int>(DATATYPE::MESSAGE)
	<<QTime::currentTime()
 <<msg;

 //когда общая рассылка, другой клиент не сможет узнать,
 //кто прислал мсг, т.к. получит от сервера, поэтому отправителю
 //нужно приписать, что это он.
 //Если это будет Р2Р, возможно это будет не обяз. Нужно еще раз обдумать...
 //а пока так.
// QString msg=pmsgField->toPlainText();
// msg.prepend(usernick+": ");


 //вывод у себя

 out.device()->seek(0);
 out<<quint16(static_cast<size_t>(arrBlock.size())-sizeof(quint16));

 psocket->write(arrBlock);
}

void ClientService::slotSetAddress(QString addr, QString port)
{
 *pserverAddress=addr;
 *pserverPort=port;
}
