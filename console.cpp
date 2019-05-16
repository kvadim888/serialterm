/****************************************************************************
**
** Copyright (C) 2012 Denis Shienkov <denis.shienkov@gmail.com>
** Copyright (C) 2012 Laszlo Papp <lpapp@kde.org>
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtSerialPort module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "console.h"
#include <QScrollBar>
#include <QDebug>

Console::Console(QWidget *parent) :
    QPlainTextEdit(parent)
{
    document()->setMaximumBlockCount(100);
    QPalette p = palette();
    p.setColor(QPalette::Base, Qt::black);
    p.setColor(QPalette::Text, Qt::green);
    setPalette(p);
}

#define TWOCOMP14(x) (static_cast<int16_t>(x)<0x2000) \
						? static_cast<int16_t>(x) \
						: -(0x4000 - (static_cast<int16_t>(x)))

#define TWOCOMP12(x) (((int16_t)(x)<0x0800)?(int16_t)(x):-(0x1000-(int16_t)(x)))

std::vector<int16_t> Console::parse(char *data, size_t size)
{
	std::vector<int16_t> output;

	output.reserve(10);
	output = {0};

	if (size < 24)
		return output;

//	//acceleration
//	output[0] = TWOCOMP14(((data[9] << 8) | data[10]) & 0x3FFF);
//	output[1] = TWOCOMP14(((data[11] << 8) | data[12]) & 0x3FFF);
//	output[2] = TWOCOMP14(((data[13] << 8) | data[14]) & 0x3FFF);

//	//angular velocity
//	output[3] = TWOCOMP14(((data[3] << 8) | data[4]) & 0x3FFF);
//	output[4] = TWOCOMP14(((data[5] << 8) | data[6]) & 0x3FFF);
//	output[5] = TWOCOMP14(((data[7] << 8) | data[8]) & 0x3FFF);

	//acceleration
	output[0] = ((data[9] << 8) | data[10]) & 0x3FFF;
	output[1] = ((data[11] << 8) | data[12]) & 0x3FFF;
	output[2] = ((data[13] << 8) | data[14]) & 0x3FFF;

	//angular velocity
	output[3] = ((data[3] << 8) | data[4]) & 0x3FFF;
	output[4] = ((data[5] << 8) | data[6]) & 0x3FFF;
	output[5] = ((data[7] << 8) | data[8]) & 0x3FFF;

	return output;
}

void Console::putData(const QByteArray &data)
{
	char const *raw = data.data();
	qDebug()<<"len = "<<data.length();
	while (*raw)
	{
		insertPlainText(QString::number(*raw, 16));
		raw++;
	}
//    bar->setValue(bar->maximum());
}

void Console::setLocalEchoEnabled(bool set)
{
    m_localEchoEnabled = set;
}

void Console::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
    case Qt::Key_Backspace:
    case Qt::Key_Left:
    case Qt::Key_Right:
    case Qt::Key_Up:
    case Qt::Key_Down:
        break;
    default:
        if (m_localEchoEnabled)
            QPlainTextEdit::keyPressEvent(e);
        emit getData(e->text().toLocal8Bit());
    }
}

void Console::mousePressEvent(QMouseEvent *e)
{
    Q_UNUSED(e)
    setFocus();
}

void Console::mouseDoubleClickEvent(QMouseEvent *e)
{
    Q_UNUSED(e)
}

void Console::contextMenuEvent(QContextMenuEvent *e)
{
    Q_UNUSED(e)
}
