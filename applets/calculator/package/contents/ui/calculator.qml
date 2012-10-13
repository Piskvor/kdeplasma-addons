/***************************************************************************
 *   Copyright (C) 2012 by Davide Bettio <davide.bettio@kdemail.net>       *
 *   Copyright (C) 2007 by Henry Stanaland <stanaland@gmail.com>           *
 *   Copyright (C) 2008 by Laurent Montel  <montel@kde.org>                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/

import QtQuick 1.0;
import org.kde.plasma.components 0.1 as PlasmaComponents
import org.kde.locale 0.1 as Locale

Item
{
    property int minimumWidth: 200;
    property int minimumHeight: 250;

    property int buttonHeight: (height - display.height - 6 * buttonsGrid.spacing) / 5;
    property int buttonWidth: (width / 4) - buttonsGrid.spacing;

    property real factor;
    property real sum;
    property bool waitingForDigit;
    property string prevOperator;

    property int maxInputLength: 19;

    focus: true;

    Locale.Locale {
        id: locale;
    }

    Column {
        spacing: 4;

        PlasmaComponents.TextArea {
            id: display;
            text: "0";
            font.pointSize: 16;
            font.weight: Font.Bold;
            horizontalAlignment: TextEdit.AlignRight;
            verticalAlignment: TextEdit.AlignVCenter;
            height: 2 * font.pixelSize;
            width: buttonsGrid.width;
            readOnly: true;
        }


        Grid {
            focus: true;
            id: buttonsGrid;
            columns: 4;
            rows: 5;
            spacing: 4;

            PlasmaComponents.Button {
                text: "C";
                height: buttonHeight;
                width: buttonWidth;
                onClicked: clearClicked();
            }

            PlasmaComponents.Button {
                text: "÷";
                height: buttonHeight;
                width: buttonWidth;
                onClicked: divideClicked();
            }

            PlasmaComponents.Button {
                text: "×";
                height: buttonHeight;
                width: buttonWidth;
                onClicked: multiplyClicked();
            }

            PlasmaComponents.Button {
                text: "AC";
                height: buttonHeight;
                width: buttonWidth;
                onClicked: allClearClicked();
            }


            PlasmaComponents.Button {
                text: "7";
                height: buttonHeight;
                width: buttonWidth;
                onClicked: digitClicked(7);
            }

            PlasmaComponents.Button {
                text: "8";
                height: buttonHeight;
                width: buttonWidth;
                onClicked: digitClicked(8);
            }

            PlasmaComponents.Button {
                text: "9";
                height: buttonHeight;
                width: buttonWidth;
                onClicked: digitClicked(9);
            }

            PlasmaComponents.Button {
                text: "-";
                height: buttonHeight;
                width: buttonWidth;
                onClicked: subtractClicked();
            }


            PlasmaComponents.Button {
                height: buttonHeight;
                width: buttonWidth;
                text: "4";
                onClicked: digitClicked(4);
            }

            PlasmaComponents.Button {
                text: "5";
                height: buttonHeight;
                width: buttonWidth;
                onClicked: digitClicked(5);
            }

            PlasmaComponents.Button {
                text: "6";
                height: buttonHeight;
                width: buttonWidth;
                onClicked: digitClicked(6);
            }

            PlasmaComponents.Button {
                text: "+";
                height: buttonHeight;
                width: buttonWidth;
                onClicked: addClicked();
            }


            PlasmaComponents.Button {
                text: "1";
                height: buttonHeight;
                width: buttonWidth;
                onClicked: digitClicked(1);
            }

            PlasmaComponents.Button {
                text: "2";
                height: buttonHeight;
                width: buttonWidth;
                onClicked: digitClicked(2);
            }

            PlasmaComponents.Button {
                text: "3";
                height: buttonHeight;
                width: buttonWidth;
                onClicked: digitClicked(3);
            }

            Item {
                id: ansPlaceHolder;
                height: buttonHeight;
                width: buttonWidth;
            }

            Item {
                id: zeroPlaceHolder;
                height: buttonHeight;
                width: buttonWidth;
            }

            Item {
                id: zeroPlaceHolder2;
                height: buttonHeight;
                width: buttonWidth;
            }

            PlasmaComponents.Button {
                text: locale.decimalSymbol;
                height: buttonHeight;
                width: buttonWidth;
                onClicked: decimalClicked();
            }

            Item {
                id: ansPlaceHolder2;
                height: buttonHeight;
                width: buttonWidth;
            }
        }
    }

    PlasmaComponents.Button {
        x: zeroPlaceHolder.x + buttonsGrid.x;
        y: zeroPlaceHolder.y + buttonsGrid.y;
        text: "0";
        onClicked: digitClicked(0);
        height: buttonHeight;
        width: buttonWidth * 2 + buttonsGrid.spacing;
    }

    PlasmaComponents.Button {
        id: ansButton;
        x: ansPlaceHolder.x + buttonsGrid.x;
        y: ansPlaceHolder.y + buttonsGrid.y;
        text: "=";
        height: buttonHeight * 2 + buttonsGrid.spacing;
        width: buttonWidth;
        onClicked: equalsClicked();
    }

    Keys.onDigit0Pressed: { digitClicked(0); }
    Keys.onDigit1Pressed: { digitClicked(1); }
    Keys.onDigit2Pressed: { digitClicked(2); }
    Keys.onDigit3Pressed: { digitClicked(3); }
    Keys.onDigit4Pressed: { digitClicked(4); }
    Keys.onDigit5Pressed: { digitClicked(5); }
    Keys.onDigit6Pressed: { digitClicked(6); }
    Keys.onDigit7Pressed: { digitClicked(7); }
    Keys.onDigit8Pressed: { digitClicked(8); }
    Keys.onDigit9Pressed: { digitClicked(9); }
    Keys.onEscapePressed: { allClearClicked(); }
    Keys.onDeletePressed: { clearClicked(); }
    Keys.onPressed: {
        switch (event.key) {
        case Qt.Key_Plus:
            addClicked();
            break;
        case Qt.Key_Minus:
            subtractClicked();
            break;
        case Qt.Key_Asterisk:
            multiplyClicked();
            break;
        case Qt.Key_Slash:
            divideClicked();
            break;
        case Qt.Key_Period:
            decimalClicked();
            break;
        case Qt.Key_Equal:
        case Qt.Key_Return:
        case Qt.Key_Enter:
            equalsClicked();
            break;
        default:
            break;
        }
    }


    function digitClicked(digit)
    {
        if (display.text.length >= maxInputLength) {
            return;
        }

        if (waitingForDigit) {
            waitingForDigit = false;
            display.text = digit;
        } else if (display.text.indexOf(locale.decimalSymbol) > 0) {
            display.text += digit;
        } else {
            display.text = locale.formatNumber(locale.readNumber(display.text) * 10 + digit, 0);
        }
    }

    function decimalClicked()
    {
        if (waitingForDigit) {
            display.text = "0";
        }

        if (display.text.indexOf(locale.decimalSymbol) == -1) {
            display.text += locale.decimalSymbol;
        }

        waitingForDigit = false;
    }

    function addClicked()
    {
        waitingForDigit = true;
        sum = locale.readNumber(display.text);
        prevOperator = "+";
    }

    function subtractClicked()
    {
        waitingForDigit = true;
        sum = locale.readNumber(display.text);
        prevOperator = "-";
    }

    function multiplyClicked()
    {
        waitingForDigit = true;
        factor = locale.readNumber(display.text);
        prevOperator = "*";
    }

    function divideClicked()
    {
        waitingForDigit = true;
        factor = locale.readNumber(display.text);
        prevOperator = "/";
    }

    function equalsClicked()
    {
        switch (prevOperator){
            case "+":
                display.text = locale.formatNumber(sum + locale.readNumber(display.text), 0);
                break;
            case "-":
                display.text = locale.fortmatNumber(sum - locale.readNumber(display.text), 0);
                break;
            case "*":
                display.text = locale.formatNumber(factor * locale.readNumber(display.text), 0);
                break;
            case "/":
                display.text = locale.formatNumber(factor / locale.readNumber(display.text), 0);
                break;
        }

        waitingForDigit = true;
    }

    function clearClicked()
    {
        display.text = "0";
        waitingForDigit = true;
    }

    function allClearClicked()
    {
        display.text = "0";
        sum = 0;
        factor = 0;
        waitingForDigit = true;
    }
}
