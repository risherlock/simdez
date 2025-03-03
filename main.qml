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

        Sat
        {
          scale: Qt.vector3d(50, 50, 50)
          rotation: Qt.quaternion(Math.cos(angle / 2), Math.sin(angle / 2) * axis.x, Math.sin(angle / 2) * axis.y, Math.sin(angle / 2) * axis.z)

          NumberAnimation on eulerRotation.y
          {
            from: 0
            to: 360
            duration: 5000
            loops: Animation.Infinite
          }
        }
    }
}
