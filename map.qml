import QtQuick 2.12
import QtLocation 5.12
import QtPositioning 5.12
import QtQuick.Dialogs 1.2
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.2

Map {
    property double oldLat: 55.75
    property double oldLon: 37.36

    Plugin {
        id: mapPlugin
        name: "osm"
    }

    Map {
        id: mapView
        anchors.fill: parent
        plugin: mapPlugin
        center: QtPositioning.coordinate(oldLat, oldLon)
        zoomLevel: 8
         // для точек на карте
        MapItemView {
               model: searchModel
               delegate: MapQuickItem {
                   coordinate: place.location.coordinate

                   anchorPoint.x: image.width * 0.5
                   anchorPoint.y: image.height

                   sourceItem: Column {
                       Image { id: image; source: "http://maps.gstatic.com/mapfiles/ridefinder-images/mm_20_red.png" }
                       Text { text: title; font.bold: true }
                   }
               }
           }
    }

    function setCenter(lat, lon){
        mapView.pan(oldLat - lat, oldLon - lon)
        oldLat = lat
        oldLon = lon
    }

   // для точек на карте
    property variant locationOslo: QtPositioning.coordinate( 55.75, 37.36)
    PlaceSearchModel {
        id: searchModel
        plugin: mapPlugin
        searchTerm: "Метеостанция"
        searchArea: QtPositioning.circle(locationOslo)
        Component.onCompleted: update()
    }
}

//    Component {
//            id: mapcomponent
//            MapQuickItem {
//                id: marker
//                anchorPoint.x: image.width/4
//                anchorPoint.y: image.height
//                coordinate: position

//                sourceItem: Image {
//                    id: image
//                    source: "http://maps.gstatic.com/mapfiles/ridefinder-images/mm_20_red.png"
//                }
//            }
//        }
//}

//Rectangle {
//    width: 1000//Screen.width
//    height: 950//Screen.height
//    property double oldLat: 55.75
//    property double oldLon: 37.36
//    visible: true


//    Plugin {
//        id: mapPlugin
//        name: ["osm", "esri"]
//    }

//    Map {
//        id: mapView
//        anchors.fill: parent
//        plugin: mapPlugin
//        center: QtPositioning.coordinate(oldLat, oldLon)
//        zoomLevel: 8
//        // для точек на карте
//        MapItemView {
//               model: MarkerModel
//               delegate: mapcomponent
//        }
//    }

//    Component {
//        id: mapcomponent
//        MapQuickItem {
//            id: marker
//            anchorPoint.x: image.width/2
//            anchorPoint.y: image.height/2
//            coordinate: place.location.coordinate

//            sourceItem:
//                Image {
//                    id: imageMeteostation
//                    source: "http://maps.gstatic.com/mapfiles/ridefinder-images/mm_20_red.png"
//                }
//                Image {
//                    id: imageGidropost
//                    source: "http://maps.gstatic.com/mapfiles/ridefinder-images/mm_20_blue.png"
//                }
//        }
//    }

//    MouseArea {
//        //anchors.fill: parent
//        onPressAndHold:  {
//            var coordinate = mapview.toCoordinate(Qt.point(mouse.x,mouse.y))
//            MarkerModel.addMarker(coordinate)
//        }
//    }


//}














//Для дальнейшего развития
//    MessageDialog {
//        id: messageDialog
//        title: "Сообщение"
//        text: "Загрузка карты..."
//        onAccepted: {
//            Qt.quit()
//        }
//        Component.onCompleted: visible = true
//    }
   // для поиска точек на карте
//    property variant locationOslo: QtPositioning.coordinate( 55.75, 37.36)
//    PlaceSearchModel {
//        id: searchModel
//        plugin: mapPlugin
//        searchTerm: "Метеостанция"
//        searchArea: QtPositioning.circle(locationOslo)
//        Component.onCompleted: update()
//    }

//MapQuickItem {
//coordinate: place.location.coordinate

//anchorPoint.x: image_red.width / 2
//anchorPoint.y: image_red.height / 2

//sourceItem:
//Column {
//    id: meteostation;
//    Image { id: image_red; source: "http://maps.gstatic.com/mapfiles/ridefinder-images/mm_20_red.png" }
//    Text { text: title; font.bold: true }
//}
//                   Column {
//                         id: gidropost;
//                         Image { id: image_blue; source: "http://maps.gstatic.com/mapfiles/ridefinder-images/mm_20_blue.png" }
//                         Text { text: title; font.bold: true }
//                   }


