/*
 *  Copyright 2014 Weng Xuetian <wengxt@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  2.010-1301, USA.
 */

import QtQuick 2.0
import QtQuick.Layouts 1.1
import org.kde.plasma.plasmoid 2.0
import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents
import org.kde.plasma.private.kimpanel 0.1 as Kimpanel


PlasmaCore.Dialog {
    id: inputpanel
    type: PlasmaCore.Dialog.PopupMenu
    flags: Qt.ToolTip | Qt.WindowStaysOnTopHint | Qt.WindowDoesNotAcceptFocus
    property bool verticalLayout: false
    property int highlightCandidate: -1
    property int baseSize: theme.mSize(theme.defaultFont).height
    property var position: {'x': 0, 'y': 0, 'w': 0, 'h': 0};
    
    onPositionChanged : updatePosition();
    onWidthChanged : updatePosition();
    onHeightChanged : updatePosition();

    mainItem: Column {
        width: childrenRect.width
        height: childrenRect.height
        Row {
            PlasmaComponents.Label {
                id: auxLabel
                anchors.top: parent.top
            }
            Item {
                id: preedit
                width: preeditLabel1.width + preeditLabel2.width + 2
                height: Math.max(preeditLabel1.height, preeditLabel2.height)
                clip: true
                PlasmaComponents.Label {
                    id: preeditLabel1
                    anchors.top: parent.top
                    anchors.left: parent.left
                }
                Rectangle {
                    color: theme.textColor
                    height: baseSize
                    width: 2
                    opacity: 0.8
                    z: 1
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: preeditLabel1.right
                }
                PlasmaComponents.Label {
                    id: preeditLabel2
                    anchors.top: parent.top
                    anchors.left: preeditLabel1.right
                }
            }
        }
      
        GridLayout {
            flow: inputpanel.verticalLayout ? GridLayout.TopToBottom : GridLayout.LeftToRight
            columns: inputpanel.verticalLayout ? 1 : tableList.count + 1
            rows: inputpanel.verticalLayout ? tableList.count + 1 : 1
            Repeater {
                model: ListModel {
                    id: tableList
                    dynamicRoles: true
                }
                delegate: Item {
                    Layout.minimumWidth: candidate.width
                    Layout.minimumHeight: candidate.height
                    Item {
                        id: candidate
                        width: childrenRect.width
                        height: childrenRect.height
                        property bool highlight: (inputpanel.highlightCandidate == model.index) != candidateMouseArea.containsMouse
                        PlasmaComponents.Label {
                            id: tableLabel
                            anchors.top: parent.top
                            anchors.left: parent.left
                            text: model.label 
                            color: candidate.highlight ? theme.textColor : theme.highlightColor
                        }
                        PlasmaComponents.Label {
                            id: textLabel
                            anchors.top: parent.top
                            anchors.left: tableLabel.right
                            text: model.text
                            color: candidate.highlight ? theme.highlightColor : theme.textColor
                        }
                    }
                    MouseArea {
                        id: candidateMouseArea
                        anchors.fill: parent
                        hoverEnabled: true
                        onReleased: selectCandidate(model.index)
                    }
                }
            }
            Row {
                id: button
                width: childrenRect.width
                height: childrenRect.height
                PlasmaCore.IconItem {
                    id: prevButton
                    source: "arrow-left"
                    width: inputpanel.baseSize
                    height: width
                    scale: prevButtonMouseArea.pressed ? 0.9 : 1
                    active: prevButtonMouseArea.containsMouse
                    MouseArea {
                        id: prevButtonMouseArea
                        anchors.fill: parent
                        hoverEnabled: true
                        onReleased: action("LookupTablePageUp")
                    }
                }
                PlasmaCore.IconItem {
                    id: nextButton
                    source: "arrow-right"
                    width: inputpanel.baseSize
                    height: width
                    scale: nextButtonMouseArea.pressed ? 0.9 : 1
                    active: nextButtonMouseArea.containsMouse
                    MouseArea {
                        id: nextButtonMouseArea
                        anchors.fill: parent
                        hoverEnabled: true
                        onReleased: action("LookupTablePageDown")
                    }
                }
            }
        }
        PlasmaCore.DataSource {
            id: inputPanelEngine
            engine: "kimpanel"
            connectedSources: ["inputpanel"]
            onDataChanged: {
                 var data = inputPanelEngine.data["inputpanel"];
                 var auxVisible = data["AuxVisible"] ? true : false;
                 var preeditVisible = data["PreeditVisible"] ? true : false;
                 var lookupTableVisible = data["LookupTableVisible"] ? true : false;
                 var pos = data["Position"] ? { 'x': data["Position"].x, 
                                                'y': data["Position"].y,
                                                'w': data["Position"].width,
                                                'h': data["Position"].height } : {'x' : 0, 'y': 0, 'w': 0, 'h': 0 };
                 inputpanel.position = pos;

                 auxLabel.text = (auxVisible && data["AuxText"]) ? data["AuxText"] : ""
                 var preeditText = (preeditVisible && data["PreeditText"]) ? data["PreeditText"] : ""
                 var caret = data["CaretPos"] ? data["CaretPos"] : 0;
                 preeditLabel1.text = preeditText.substring(0, caret);
                 preeditLabel2.text = preeditText.substring(caret);
                 preedit.visible = preeditVisible;
                 var layout = data["LookupTableLayout"] !== undefined ? data["LookupTableLayout"] : 0;
                 inputpanel.highlightCandidate = data["LookupTableCursor"] !== undefined ? data["LookupTableCursor"] : -1;
                 inputpanel.verticalLayout = (layout === 1) || (layout == 0 && plasmoid.configuration.vertical_lookup_table);
                 button.visible = lookupTableVisible

                 if (data["LookupTable"]) {
                     var table = data["LookupTable"];
                     while (tableList.count > table.length) {
                         tableList.remove(tableList.count - 1);
                     }
                     for (var i = 0; i < table.length; i ++) {
                         if (i >= tableList.count) {
                              tableList.append({'label' : table[i].label, 'text': table[i].text, 'index': i});
                         } else {
                              tableList.set(i, {'label' : table[i].label, 'text': table[i].text, 'index': i});
                         }
                     }
                 }

                 inputpanel.visible = auxVisible || preeditVisible || lookupTableVisible;
            }
        }

        Kimpanel.Screen {
            id: screen
        }
    }

    function updatePosition() {
         var rect = screen.geometryForPoint(position.x, position.y);
         var x, y;
         if (position.x < rect.x) {
             x = rect.x;
         } else {
             x = position.x;
         }
         if (position.y < rect.y) {
             y = rect.y;
         } else {
             y = position.y + position.h;
         }

         if (x + inputpanel.width > rect.x + rect.width) {
             x = rect.x + rect.width - inputpanel.width;
         }
         
         if (y + inputpanel.height > rect.y + rect.height) {
             if (y > rect.y + rect.height) {
                 y = rect.y - window.height - 40;
             } else {
                 y = y - inputpanel.height - (position.h == 0 ? 40 : position.h);
             }
         }

         inputpanel.x = x;
         inputpanel.y = y;
    }

    function action(key) {
        var service = inputPanelEngine.serviceForSource("inputpanel");
        var operation = service.operationDescription(key);
        service.startOperationCall(operation);
    }

    function selectCandidate(index) {
        var service = inputPanelEngine.serviceForSource("inputpanel");
        var operation = service.operationDescription("SelectCandidate");
        operation.candidate = index;
        service.startOperationCall(operation);
    }
}
