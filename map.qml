import QtQuick 2.0
import QtLocation 5.6
import QtPositioning 5.5
import QtQuick.Dialogs 1.2


MapItemView {

    PositionSource{
        id: position_id
    }

    Plugin {
        id: mapPlugin
        name: "osm"
    }

    Map{
        id: map_id
        anchors.fill: parent
        plugin: mapPlugin
        center: position_id.position.coordinate
        zoomLevel: 7        
        MapItemView{
                    model: markerModel
                    delegate: mapComponent
        }
        // Легенда карты
        Rectangle{
            anchors{
                left: map_id.left
                top: map_id.top
                margins: 2
            }
            width: 380
            height: content.height + 50
            color: "transparent"
            border.color: "green"
            border.width: 2
            Rectangle{
                width: 380; height: content.height + 50
                opacity: 0.6
            }

            Text {
                id: content
                x: 4
                y: 2
                color: "green"
                font.pixelSize: 14
                text: qsTr("Уровень масштаба: " + Math.floor(map_id.zoomLevel)+" / Координаты центра: "+
                      Math.ceil(map_id.center.latitude*100)/100+"  "+Math.ceil(map_id.center.longitude*100)/100)
            }
            Image {
                id: imageRed
                source: "img/imageRed.png"
                x: 4
                y: 22
            }
            Text {
                id: imageRedText
                x: 24
                y: 22
                color: "green"
                text: qsTr("- метеостанции")
            }
            Image {
                id: imageBlue
                x: 4
                y: 44
                source: "img/imageBlue.png"
            }
            Text {
                id: imageBlueText
                x: 24
                y: 44
                color: "green"
                text: qsTr("- гидропосты")
            }
        }
    }
    Component {
           id: mapComponent
           MapQuickItem {
               id: marker_id
               property var listInfo: textInfo
               sourceItem: Column {
                               Image{ id: image; source: setImage(textInfo)}
                               Text { text: textInfo[0]; font.bold: true }
               }
               coordinate: position
               anchorPoint.x: imageRed.width * 0.5
               anchorPoint.y: imageRed.height
               //Кликнуть по координате
               MouseArea{
                  id:mouse_id
                  anchors.fill: parent
                  onClicked: {
                      messageDialog.text = getTextInfo(listInfo)
                      messageDialog.visible = true
                  }

               }
            }
    }
    //idCoordinate - пока не использую
    function setCoordinate(lat, lon, listNameIdStation) {

        markerModel.addMarker( QtPositioning.coordinate(lat, lon), listNameIdStation )
    }

    //синия для гидропостов, красная для метеостанций
    function setImage(textInfo){

        if ( textInfo[2] === "Метеостанция" ) return "img/imageRed.png"
        else return "img/imageBlue.png"
    }

    //вывод текстовой информации
    function getTextInfo(listInfo){
        var str = qsTr("Выбрана точка на карте:\n");
        str += qsTr("Объект: " + listInfo[2] + "\n");         //метеостанция/гидропост
        str += qsTr("Название: " + listInfo[0] + "\n");       //название
        str += qsTr("Идентификатор: " + listInfo[1]);  //id

        return str;
    }

    MessageDialog {
        id: messageDialog
        title: "Информация"
        onAccepted: {
            Qt.quit()
        }
        Component.onCompleted: visible = false
    }
}
