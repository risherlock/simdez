import QtQuick
import QtQuick3D
import QtQuick3D.Helpers

Item {
    width: 800
    height: 600

    View3D {
        anchors.fill: parent

        environment: SceneEnvironment {
            clearColor: "lightblue"
        }

        PerspectiveCamera {
            position: Qt.vector3d(0, 0, 600)
        }

        DirectionalLight {
            brightness: 1
        }

        Model {
            position: Qt.vector3d(0, -200, 0)
            source: "#Cylinder"
            scale: Qt.vector3d(2, 0.2, 1)
            materials: [ DefaultMaterial {
                    diffuseColor: "red"
                }
            ]
        }

        Model {
            position: Qt.vector3d(0, 150, 0)
            source: "#Sphere"

            materials: [ DefaultMaterial {
                    diffuseColor: "blue"
                }
            ]

            SequentialAnimation on y {
                loops: Animation.Infinite
                NumberAnimation {
                    duration: 3000
                    to: -150
                    from: 150
                    easing.type:Easing.InQuad
                }
                NumberAnimation {
                    duration: 3000
                    to: 150
                    from: -150
                    easing.type:Easing.OutQuad
                }
            }
        }
    }
}
